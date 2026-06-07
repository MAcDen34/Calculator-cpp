#include "graphwindow.h"
#include <QMetaObject>
#include <stdexcept>

GraphWindow::GraphWindow(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Function Grapher");
    setFixedSize(600, 500);
    setStyleSheet("background-color: #1c1c1e; color: white;");

    auto* root = new QVBoxLayout(this);
    root->setSpacing(10);
    root->setContentsMargins(16, 16, 16, 16);

    // ── Controls ──────────────────────────────────────────
    auto* controls = new QHBoxLayout();

    fnCombo_ = new QComboBox();
    fnCombo_->addItems({"sin", "cos", "tan", "sqrt", "ln", "exp", "x^2", "1/x", "|x|"});
    fnCombo_->setStyleSheet(R"(
        QComboBox { background-color: #3a3a3c; color: white;
                    border-radius: 8px; padding: 6px 12px; font-size: 14px; }
        QComboBox::drop-down { border: none; }
        QComboBox QAbstractItemView { background-color: #3a3a3c; color: white; }
    )");

    xMinEdit_ = new QLineEdit("-10");
    xMaxEdit_ = new QLineEdit("10");
    for (auto* e : {xMinEdit_, xMaxEdit_}) {
        e->setFixedWidth(60);
        e->setStyleSheet(R"(
            QLineEdit { background-color: #3a3a3c; color: white;
                        border-radius: 8px; padding: 6px; font-size: 14px; }
        )");
    }

    plotBtn_ = new QPushButton("Plot");
    plotBtn_->setFixedSize(80, 36);
    plotBtn_->setStyleSheet(R"(
        QPushButton { background-color: #ff9f0a; color: white;
                      border-radius: 8px; font-size: 14px; font-weight: 600; }
        QPushButton:pressed { background-color: #ffcc80; }
    )");

    controls->addWidget(new QLabel("f(x) ="));
    controls->addWidget(fnCombo_);
    controls->addWidget(new QLabel("x min:"));
    controls->addWidget(xMinEdit_);
    controls->addWidget(new QLabel("x max:"));
    controls->addWidget(xMaxEdit_);
    controls->addStretch();
    controls->addWidget(plotBtn_);
    root->addLayout(controls);

    // ── Plot ──────────────────────────────────────────────
    plot_ = new QCustomPlot();
    plot_->setBackground(QBrush(QColor("#2c2c2e")));
    plot_->xAxis->setBasePen(QPen(Qt::white));
    plot_->yAxis->setBasePen(QPen(Qt::white));
    plot_->xAxis->setTickPen(QPen(Qt::white));
    plot_->yAxis->setTickPen(QPen(Qt::white));
    plot_->xAxis->setTickLabelColor(Qt::white);
    plot_->yAxis->setTickLabelColor(Qt::white);
    plot_->xAxis->setLabelColor(Qt::white);
    plot_->yAxis->setLabelColor(Qt::white);
    plot_->xAxis->grid()->setPen(QPen(QColor("#3a3a3c"), 1, Qt::DotLine));
    plot_->yAxis->grid()->setPen(QPen(QColor("#3a3a3c"), 1, Qt::DotLine));
    plot_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    root->addWidget(plot_);

    // ── Status ────────────────────────────────────────────
    statusLabel_ = new QLabel("Select a function and click Plot");
    statusLabel_->setStyleSheet("color: #8e8e93; font-size: 12px;");
    root->addWidget(statusLabel_);

    connect(plotBtn_, &QPushButton::clicked, this, &GraphWindow::onPlotClicked);
    connect(this, &GraphWindow::onPlotReady, this, &GraphWindow::onPlotReady);
}

void GraphWindow::onPlotClicked() {
    QString fn   = fnCombo_->currentText();
    double xMin  = xMinEdit_->text().toDouble();
    double xMax  = xMaxEdit_->text().toDouble();

    if (xMin >= xMax) {
        statusLabel_->setText("Error: x min must be less than x max");
        return;
    }

    plotBtn_->setEnabled(false);
    statusLabel_->setText("Plotting...");

    // Fire background thread
    future_ = std::async(std::launch::async, [this, fn, xMin, xMax]() {
        computePoints(fn, xMin, xMax, 1000);
    });
}

void GraphWindow::computePoints(const QString& fn, double xMin, double xMax, int points) {
    QVector<double> xs, ys;
    double step = (xMax - xMin) / points;

    for (double x = xMin; x <= xMax; x += step) {
        double y = 0;
        bool valid = true;
        try {
            if      (fn == "sin")  y = calc_.calc_sin(x);
            else if (fn == "cos")  y = calc_.calc_cos(x);
            else if (fn == "tan")  y = calc_.calc_tan(x);
            else if (fn == "sqrt") y = calc_.calc_sqrt(x);
            else if (fn == "ln")   y = calc_.calc_ln(x);
            else if (fn == "exp")  y = calc_.calc_exp(x);
            else if (fn == "x^2")  y = x * x;
            else if (fn == "1/x")  y = (x != 0) ? 1.0/x : 0;
            else if (fn == "|x|")  y = calc_.calc_abs(x);
        } catch (...) { valid = false; }

        if (valid) {
            xs.append(x);
            ys.append(y);
        }
    }

    // Send back to GUI thread safely
    QMetaObject::invokeMethod(this, [this, xs, ys, fn]() {
        onPlotReady(xs, ys, fn);
    }, Qt::QueuedConnection);
}

void GraphWindow::onPlotReady(QVector<double> xs, QVector<double> ys, const QString& label) {
    plot_->clearGraphs();
    plot_->addGraph();
    plot_->graph(0)->setData(xs, ys);
    plot_->graph(0)->setPen(QPen(QColor("#ff9f0a"), 2));
    plot_->graph(0)->setName(label);
    plot_->xAxis->setLabel("x");
    plot_->yAxis->setLabel("f(x)");
    plot_->rescaleAxes();
    plot_->replot();

    plotBtn_->setEnabled(true);
    statusLabel_->setText(QString("Plotted %1 points for %2(x)").arg(xs.size()).arg(label));
}

#include <QDebug>
#include "mainwindow.h"
#include "graphwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSizePolicy>
#include <QFont>
#include <QMenu>
#include <QToolButton>
#include <QAction>
#include <QScrollArea>
#include <QFrame>
#include <QGesture>
#include <QSwipeGesture>
#include <QPropertyAnimation>
#include <QDateTime>
#include <cmath>
#include <stdexcept>
 
// ── Styles ────────────────────────────────────────────────
static QString digitBtnStyle() { return R"(
    QPushButton {
        background-color: #3a3a3c; color: white;
        border-radius: 37px; font-size: 26px; font-weight: 400;
    }
    QPushButton:pressed { background-color: #636366; }
)"; }
 
static QString opBtnStyle() { return R"(
    QPushButton {
        background-color: #ff9f0a; color: white;
        border-radius: 37px; font-size: 26px; font-weight: 500;
    }
    QPushButton:pressed { background-color: #ffcc80; color: #ff9f0a; }
)"; }
 
static QString utilBtnStyle() { return R"(
    QPushButton {
        background-color: #636366; color: white;
        border-radius: 37px; font-size: 22px; font-weight: 400;
    }
    QPushButton:pressed { background-color: #9a9a9e; }
)"; }
 
static QString sciBtnStyle() { return R"(
    QPushButton {
        background-color: #2c2c2e; color: #ff9f0a;
        border-radius: 20px; font-size: 15px; font-weight: 500;
    }
    QPushButton:pressed { background-color: #ff9f0a; color: white; }
)"; }
 
// ── Button factories ──────────────────────────────────────
QPushButton* MainWindow::makeDigitBtn(const QString& label) {
    auto* btn = new QPushButton(label);
    btn->setFixedSize(74, 74);
    btn->setStyleSheet(digitBtnStyle());
    connect(btn, &QPushButton::clicked, this, [this, label]() { onDigitClicked(label); });
    return btn;
}
 
QPushButton* MainWindow::makeOpBtn(const QString& label) {
    auto* btn = new QPushButton(label);
    btn->setFixedSize(74, 74);
    btn->setStyleSheet(opBtnStyle());
    connect(btn, &QPushButton::clicked, this, [this, label]() {
        if (label == "=") onEqualsClicked();
        else onOperatorClicked(label);
    });
    return btn;
}
 
QPushButton* MainWindow::makeUtilBtn(const QString& label) {
    auto* btn = new QPushButton(label);
    btn->setFixedSize(74, 74);
    btn->setStyleSheet(utilBtnStyle());
    connect(btn, &QPushButton::clicked, this, [this, label]() {
        if (label == "AC") onClearClicked();
        else if (label == "+/-") onToggleSign();
        else if (label == "%") onPercent();
    });
    return btn;
}
 
QPushButton* MainWindow::makeSciBtn(const QString& label) {
    auto* btn = new QPushButton(label);
    btn->setMinimumHeight(52);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btn->setStyleSheet(sciBtnStyle());
    connect(btn, &QPushButton::clicked, this, [this, label]() { onScientificFunction(label); });
    return btn;
}
 
void MainWindow::updateDisplay(const QString& text) {
    // Auto-shrink font for long numbers
    int len = text.length();
    int size = len > 12 ? 32 : len > 8 ? 44 : 64;
    display_->setStyleSheet(QString(R"(
        QLabel { color: white; font-size: %1px; font-weight: 200;
                 padding: 8px 24px; background: transparent; }
    )").arg(size));
    display_->setText(text);
}
 
// ── History ───────────────────────────────────────────────
void MainWindow::addToHistory(const QString& expr, const QString& result) {
    HistoryRecord rec;
    rec.expression = expr;
    rec.result     = result;
    rec.timestamp  = QDateTime::currentDateTime();
    history_records_.push_back(rec);
    refreshHistoryPanel();
}
 
void MainWindow::refreshHistoryPanel() {
    // Clear existing items
    QLayoutItem* item;
    while ((item = history_list_layout_->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
 
    QDateTime now = QDateTime::currentDateTime();
 
    auto addSection = [&](const QString& title) {
        auto* lbl = new QLabel(title);
        lbl->setStyleSheet("color: white; font-size: 17px; font-weight: 700; padding: 12px 16px 4px 16px;");
        history_list_layout_->addWidget(lbl);
    };
 
    auto addEntry = [&](const HistoryRecord& rec) {
        auto* frame = new QFrame();
        frame->setStyleSheet("border-bottom: 1px solid #2c2c2e; background: transparent;");
        auto* layout = new QVBoxLayout(frame);
        layout->setContentsMargins(16, 8, 16, 8);
        layout->setSpacing(2);
 
        auto* exprLbl = new QLabel(rec.expression);
        exprLbl->setStyleSheet("color: #8e8e93; font-size: 14px;");
        auto* resLbl = new QLabel(rec.result);
        resLbl->setStyleSheet("color: white; font-size: 22px; font-weight: 500;");
 
        layout->addWidget(exprLbl);
        layout->addWidget(resLbl);
        history_list_layout_->addWidget(frame);
    };
 
    // Group into Today and Previous 7 Days
    bool hasToday = false, hasPrev7 = false;
    for (auto it = history_records_.rbegin(); it != history_records_.rend(); ++it) {
        int daysAgo = it->timestamp.daysTo(now);
        if (daysAgo == 0 && !hasToday) { addSection("Today"); hasToday = true; }
        if (daysAgo > 0 && daysAgo <= 7 && !hasPrev7) { addSection("Previous 7 Days"); hasPrev7 = true; }
    }
 
    // Add entries
    bool inToday = false, inPrev7 = false;
    for (auto it = history_records_.rbegin(); it != history_records_.rend(); ++it) {
        int daysAgo = it->timestamp.daysTo(now);
        if (daysAgo == 0)             { if (!inToday) inToday = true; addEntry(*it); }
        else if (daysAgo > 0 && daysAgo <= 7) { if (!inPrev7) inPrev7 = true; addEntry(*it); }
    }
 
    history_list_layout_->addStretch();
}
 
void MainWindow::showHistory() {
    history_visible_ = true;
    history_panel_->raise();
    auto* anim = new QPropertyAnimation(history_panel_, "geometry");
    anim->setDuration(300);
    anim->setStartValue(QRect(0, height(), width(), height()));
    anim->setEndValue(QRect(0, 0, width(), height()));
    anim->setEasingCurve(QEasingCurve::OutCubic);
    history_panel_->show();
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
 
void MainWindow::hideHistory() {
    history_visible_ = false;
    auto* anim = new QPropertyAnimation(history_panel_, "geometry");
    anim->setDuration(300);
    anim->setStartValue(QRect(0, 0, width(), height()));
    anim->setEndValue(QRect(0, height(), width(), height()));
    anim->setEasingCurve(QEasingCurve::InCubic);
    connect(anim, &QPropertyAnimation::finished, history_panel_, &QWidget::hide);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
 
void MainWindow::buildHistoryPanel() {
    history_panel_ = new QWidget(this);
    history_panel_->setGeometry(0, height(), width(), height());
    history_panel_->setStyleSheet("background-color: #1c1c1e;");
    history_panel_->hide();
 
    auto* layout = new QVBoxLayout(history_panel_);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
 
    // Header
    auto* header = new QWidget();
    header->setFixedHeight(56);
    header->setStyleSheet("background-color: #2c2c2e; border-bottom: 1px solid #3a3a3c;");
    auto* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(16, 0, 16, 0);
 
    auto* titleLbl = new QLabel("History");
    titleLbl->setStyleSheet("color: white; font-size: 17px; font-weight: 600;");
 
    auto* closeBtn = new QPushButton("Done");
    closeBtn->setStyleSheet(R"(
        QPushButton { color: #ff9f0a; font-size: 17px; background: transparent; border: none; }
        QPushButton:pressed { color: #ffcc80; }
    )");
    connect(closeBtn, &QPushButton::clicked, this, &MainWindow::hideHistory);
 
    headerLayout->addWidget(titleLbl);
    headerLayout->addStretch();
    headerLayout->addWidget(closeBtn);
    layout->addWidget(header);
 
    // Scrollable list
    auto* scroll = new QScrollArea();
    scroll->setStyleSheet("QScrollArea { border: none; background: transparent; }");
    scroll->setWidgetResizable(true);
 
    auto* listWidget = new QWidget();
    listWidget->setStyleSheet("background-color: #1c1c1e;");
    history_list_layout_ = new QVBoxLayout(listWidget);
    history_list_layout_->setContentsMargins(0, 0, 0, 0);
    history_list_layout_->setSpacing(0);
    history_list_layout_->addStretch();
 
    scroll->setWidget(listWidget);
    layout->addWidget(scroll);
}
 
// ── Build basic panel ─────────────────────────────────────
void MainWindow::buildBasicPanel() {
    basic_widget_ = new QWidget();
    auto* grid = new QGridLayout(basic_widget_);
    grid->setSpacing(10);
    grid->setContentsMargins(14, 4, 14, 14);
 
    grid->addWidget(makeUtilBtn("AC"),  0, 0);
    grid->addWidget(makeUtilBtn("+/-"), 0, 1);
    grid->addWidget(makeUtilBtn("%"),   0, 2);
    grid->addWidget(makeOpBtn("÷"),     0, 3);
    grid->addWidget(makeDigitBtn("7"),  1, 0);
    grid->addWidget(makeDigitBtn("8"),  1, 1);
    grid->addWidget(makeDigitBtn("9"),  1, 2);
    grid->addWidget(makeOpBtn("×"),     1, 3);
    grid->addWidget(makeDigitBtn("4"),  2, 0);
    grid->addWidget(makeDigitBtn("5"),  2, 1);
    grid->addWidget(makeDigitBtn("6"),  2, 2);
    grid->addWidget(makeOpBtn("−"),     2, 3);
    grid->addWidget(makeDigitBtn("1"),  3, 0);
    grid->addWidget(makeDigitBtn("2"),  3, 1);
    grid->addWidget(makeDigitBtn("3"),  3, 2);
    grid->addWidget(makeOpBtn("+"),     3, 3);
 
    auto* zeroBtn = new QPushButton("0");
    zeroBtn->setFixedSize(158, 74);
    zeroBtn->setStyleSheet(digitBtnStyle() + "QPushButton { border-radius: 37px; text-align: left; padding-left: 26px; }");
    connect(zeroBtn, &QPushButton::clicked, this, [this]() { onDigitClicked("0"); });
    grid->addWidget(zeroBtn,           4, 0, 1, 2);
    grid->addWidget(makeDigitBtn("."), 4, 2);
    grid->addWidget(makeOpBtn("="),    4, 3);
}
 
// ── Build scientific panel ────────────────────────────────
void MainWindow::buildScientificPanel() {
    scientific_widget_ = new QWidget();
    auto* mainLayout = new QVBoxLayout(scientific_widget_);
    mainLayout->setSpacing(4);
    mainLayout->setContentsMargins(14, 4, 14, 14);
 
    auto* sciGrid = new QGridLayout();
    sciGrid->setSpacing(6);
    for (int c = 0; c < 5; c++) sciGrid->setColumnStretch(c, 1);
 
    QStringList row0 = {"sin",  "cos",  "tan",  "ln",   "log₁₀"};
    QStringList row1 = {"asin", "acos", "atan", "eˣ",   "10ˣ"  };
    QStringList row2 = {"√",    "x²",   "1/x",  "|x|",  "n!"   };
    QStringList row3 = {"π",    "e",    "(",    ")",     "^"    };
 
    for (int c = 0; c < 5; c++) sciGrid->addWidget(makeSciBtn(row0[c]), 0, c);
    for (int c = 0; c < 5; c++) sciGrid->addWidget(makeSciBtn(row1[c]), 1, c);
    for (int c = 0; c < 5; c++) sciGrid->addWidget(makeSciBtn(row2[c]), 2, c);
    for (int c = 0; c < 5; c++) sciGrid->addWidget(makeSciBtn(row3[c]), 3, c);
    mainLayout->addLayout(sciGrid);
 
    // Basic numpad below sci functions
    auto* grid = new QGridLayout();
    grid->setSpacing(10);
 
    grid->addWidget(makeUtilBtn("AC"),  0, 0);
    grid->addWidget(makeUtilBtn("+/-"), 0, 1);
    grid->addWidget(makeUtilBtn("%"),   0, 2);
    grid->addWidget(makeOpBtn("÷"),     0, 3);
    grid->addWidget(makeDigitBtn("7"),  1, 0);
    grid->addWidget(makeDigitBtn("8"),  1, 1);
    grid->addWidget(makeDigitBtn("9"),  1, 2);
    grid->addWidget(makeOpBtn("×"),     1, 3);
    grid->addWidget(makeDigitBtn("4"),  2, 0);
    grid->addWidget(makeDigitBtn("5"),  2, 1);
    grid->addWidget(makeDigitBtn("6"),  2, 2);
    grid->addWidget(makeOpBtn("−"),     2, 3);
    grid->addWidget(makeDigitBtn("1"),  3, 0);
    grid->addWidget(makeDigitBtn("2"),  3, 1);
    grid->addWidget(makeDigitBtn("3"),  3, 2);
    grid->addWidget(makeOpBtn("+"),     3, 3);
 
    auto* zeroBtn = new QPushButton("0");
    zeroBtn->setFixedSize(158, 74);
    zeroBtn->setStyleSheet(digitBtnStyle() + "QPushButton { border-radius: 37px; text-align: left; padding-left: 26px; }");
    connect(zeroBtn, &QPushButton::clicked, this, [this]() { onDigitClicked("0"); });
    grid->addWidget(zeroBtn,           4, 0, 1, 2);
    grid->addWidget(makeDigitBtn("."), 4, 2);
    grid->addWidget(makeOpBtn("="),    4, 3);
 
    mainLayout->addLayout(grid);
}
 
// ── event filter (swipe on display) ──────────────────────
bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (obj == display_) {
        if (event->type() == QEvent::MouseButtonPress) {
            if (!history_visible_) showHistory();
            else hideHistory();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
 
// ── Constructor ───────────────────────────────────────────
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    history_writer_ = new HistoryWriter();
    calc_worker_    = new CalcWorker([this](double result, const std::string& err) {
        QMetaObject::invokeMethod(this, [this, result, err]() {
            onCalcResult(result, QString::fromStdString(err));
        }, Qt::QueuedConnection);
    });
    setWindowTitle("Calculator");
    setStyleSheet("QMainWindow, QWidget { background-color: #1c1c1e; }");
    setFixedSize(400, 820);
 
    auto* central = new QWidget(this);
    setCentralWidget(central);
    auto* root = new QVBoxLayout(central);
    root->setSpacing(0);
    root->setContentsMargins(0, 0, 0, 0);
 
    // ── Top bar ───────────────────────────────────────────
    auto* topBar = new QHBoxLayout();
    topBar->setContentsMargins(16, 12, 16, 0);
 
    // History button
    auto* histBtn = new QToolButton();
    histBtn->setText("⏱");
    histBtn->setStyleSheet(R"(
        QToolButton { background-color: #3a3a3c; color: white;
                      border-radius: 14px; font-size: 16px; padding: 4px 10px; }
        QToolButton::menu-indicator { image: none; width: 0px; }
        QToolButton:pressed { background-color: #636366; }
    )");
    connect(histBtn, &QToolButton::clicked, this, [this]() {
        if (!history_visible_) showHistory(); else hideHistory();
    });
 
    // Mode menu button
    auto* modeBtn = new QToolButton();
    modeBtn->setText("⊞");
    modeBtn->setStyleSheet(R"(
        QToolButton { background-color: #3a3a3c; color: white;
                      border-radius: 14px; font-size: 16px; padding: 4px 10px; }
        QToolButton::menu-indicator { image: none; width: 0px; }
    )");
    auto* modeMenu = new QMenu(this);
    modeMenu->setStyleSheet(R"(
        QMenu { background-color: #2c2c2e; color: white; border-radius: 8px; padding: 4px; }
        QMenu::item { padding: 8px 24px; }
        QMenu::item:selected { background-color: #3a3a3c; border-radius: 6px; }
    )");
    modeMenu->addAction("Basic",      this, [this]() { switchMode("Basic"); });
    modeMenu->addAction("Scientific", this, [this]() { switchMode("Scientific"); });
    modeBtn->setMenu(modeMenu);
    modeBtn->setPopupMode(QToolButton::InstantPopup);
 
    // Graph button
    auto* graphBtn = new QToolButton();
    graphBtn->setText("📈");
    graphBtn->setStyleSheet(R"(
        QToolButton { background-color: #3a3a3c; color: white;
                      border-radius: 14px; font-size: 16px; padding: 4px 10px; }
        QToolButton::menu-indicator { image: none; width: 0px; }
        QToolButton:pressed { background-color: #636366; }
    )");
    connect(graphBtn, &QToolButton::clicked, this, [this]() {
        auto* gw = new GraphWindow(this);
        gw->show();
    });

    topBar->addWidget(histBtn);
    topBar->addStretch();
    topBar->addWidget(graphBtn);
    topBar->addSpacing(8);
    topBar->addWidget(modeBtn);
    root->addLayout(topBar);
 
    // ── Display ───────────────────────────────────────────
    display_ = new QLabel("0");
    display_->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    display_->setStyleSheet(R"(
        QLabel { color: white; font-size: 64px; font-weight: 200;
                 padding: 8px 24px; background: transparent; }
    )");
    display_->setMinimumHeight(130);
    display_->setCursor(Qt::PointingHandCursor);
    display_->installEventFilter(this);
    root->addWidget(display_);
 
    // ── Panels ────────────────────────────────────────────
    buildBasicPanel();
    buildScientificPanel();
 
    stack_ = new QStackedWidget();
    stack_->addWidget(basic_widget_);
    stack_->addWidget(scientific_widget_);
    stack_->setCurrentIndex(0);
    root->addWidget(stack_);
 
    // ── History panel (overlay) ───────────────────────────
    buildHistoryPanel();
}
 
// ── Slots ─────────────────────────────────────────────────
void MainWindow::switchMode(const QString& mode) {
    stack_->setCurrentIndex(mode == "Scientific" ? 1 : 0);
}
 
void MainWindow::onDigitClicked(const QString& digit) {
    if (result_shown_) { current_input_.clear(); result_shown_ = false; }
    if (digit == "." && current_input_.contains(".")) return;
    if (current_input_ == "0" && digit != ".") current_input_.clear();
    current_input_ += digit;
    updateDisplay(current_input_);
}
 
void MainWindow::onOperatorClicked(const QString& op) {
    if (!current_input_.isEmpty())
        pending_value_ = current_input_.toDouble();
    current_input_.clear();
    QString mapped = op;
    if (op == "÷") mapped = "/";
    if (op == "×") mapped = "*";
    if (op == "−") mapped = "-";
    pending_op_    = mapped;
    result_shown_  = false;
}
 
void MainWindow::onEqualsClicked() {
    if (pending_op_.isEmpty() || current_input_.isEmpty()) return;
    updateDisplay("...");
    pending_expr_ = QString::number(pending_value_) + " " + pending_op_ + " " + current_input_;
    calc_worker_->compute(pending_expr_.toStdString());
}
 
void MainWindow::onClearClicked() {
    current_input_.clear(); pending_op_.clear();
    pending_value_ = 0; result_shown_ = false;
    updateDisplay("0");
}
 
void MainWindow::onToggleSign() {
    if (current_input_.isEmpty()) return;
    double val = current_input_.toDouble() * -1;
    current_input_ = QString::number(val, 'g', 10);
    updateDisplay(current_input_);
}
 
void MainWindow::onPercent() {
    if (current_input_.isEmpty()) return;
    double val = current_input_.toDouble() / 100.0;
    current_input_ = QString::number(val, 'g', 10);
    updateDisplay(current_input_);
}
 
void MainWindow::onScientificFunction(const QString& fn) {
    double val = current_input_.isEmpty() ? 0 : current_input_.toDouble();
    try {
        double result = 0;
        bool   computed = true;
        if      (fn == "sin")   result = calc_.calc_sin(val);
        else if (fn == "cos")   result = calc_.calc_cos(val);
        else if (fn == "tan")   result = calc_.calc_tan(val);
        else if (fn == "asin")  result = std::asin(val);
        else if (fn == "acos")  result = std::acos(val);
        else if (fn == "atan")  result = std::atan(val);
        else if (fn == "ln")    result = calc_.calc_ln(val);
        else if (fn == "log₁₀") result = calc_.calc_log10(val);
        else if (fn == "eˣ")    result = calc_.calc_exp(val);
        else if (fn == "10ˣ")   result = std::pow(10, val);
        else if (fn == "√")     result = calc_.calc_sqrt(val);
        else if (fn == "x²")    result = calc_.power(val, 2);
        else if (fn == "1/x")   result = calc_.divide(1, val);
        else if (fn == "|x|")   result = calc_.calc_abs(val);
        else if (fn == "n!") {
            int n = (int)val; result = 1;
            for (int i = 2; i <= n; i++) result *= i;
        }
        else if (fn == "π") { current_input_ = QString::number(M_PI, 'g', 10); updateDisplay(current_input_); return; }
        else if (fn == "e")  { current_input_ = QString::number(M_E,  'g', 10); updateDisplay(current_input_); return; }
        else if (fn == "(" || fn == ")" || fn == "^") {
            current_input_ += fn; updateDisplay(current_input_); return;
        }
        else computed = false;
 
        if (computed) {
            QString res = QString::number(result, 'g', 10);
            addToHistory(fn + "(" + QString::number(val) + ")", res);
            current_input_ = res;
            updateDisplay(current_input_);
            result_shown_ = true;
        }
    } catch (const std::exception& e) {
        updateDisplay(QString("Error: ") + e.what());
        current_input_.clear();
    }
}

// ── Threading setup ───────────────────────────────────────
void MainWindow::onCalcResult(double result, const QString& error) {
    if (!error.isEmpty()) {
        updateDisplay("Error: " + error);
        current_input_.clear();
        pending_op_.clear();
        return;
    }
    QString res  = QString::number(result, 'g', 10);
    updateDisplay(res);
    addToHistory(pending_expr_, res);
    history_writer_->enqueue(pending_expr_.toStdString() + " = " + res.toStdString());
    current_input_ = res;
    pending_op_.clear();
    result_shown_  = true;
}

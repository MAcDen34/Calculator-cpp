#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <future>
#include <vector>
#include "qcustomplot/qcustomplot.h"
#include "calculator.h"

class GraphWindow : public QDialog {
    Q_OBJECT

public:
    explicit GraphWindow(QWidget* parent = nullptr);

private slots:
    void onPlotClicked();
    void onPlotReady(QVector<double> xs, QVector<double> ys, const QString& label);

private:
    void computePoints(const QString& fn, double xMin, double xMax, int points);

    // UI
    QCustomPlot* plot_;
    QComboBox*   fnCombo_;
    QLineEdit*   xMinEdit_;
    QLineEdit*   xMaxEdit_;
    QPushButton* plotBtn_;
    QLabel*      statusLabel_;

    Calculator   calc_;
    std::future<void> future_;
};

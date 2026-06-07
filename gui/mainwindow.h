#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QMenu>
#include <QToolButton>
#include <QScrollArea>
#include <QDateTime>
#include <QPropertyAnimation>
#include <QGestureEvent>
#include <vector>
#include "calculator.h"
#include "parser.h"
#include "history.h"
#include "calc_worker.h"
#include "history_writer.h"

struct HistoryRecord {
    QString expression;
    QString result;
    QDateTime timestamp;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onDigitClicked(const QString& digit);
    void onOperatorClicked(const QString& op);
    void onEqualsClicked();
    void onClearClicked();
    void onToggleSign();
    void onPercent();
    void onScientificFunction(const QString& fn);
    void switchMode(const QString& mode);
    void showHistory();
    void hideHistory();
    void onCalcResult(double result, const QString& error);  // ← new

private:
    // Core
    Calculator  calc_;
    QString     current_input_;
    QString     pending_op_;
    QString     pending_expr_;
    double      pending_value_ = 0;
    bool        result_shown_  = false;
    std::vector<HistoryRecord> history_records_;

    // Threading
    CalcWorker*    calc_worker_    = nullptr;  // ← new
    HistoryWriter* history_writer_ = nullptr;  // ← new

    // UI
    QLabel*         display_;
    QWidget*        basic_widget_;
    QWidget*        scientific_widget_;
    QStackedWidget* stack_;
    QWidget*        history_panel_;
    QVBoxLayout*    history_list_layout_;
    bool            history_visible_ = false;

    // Button factories
    QPushButton* makeDigitBtn(const QString& label);
    QPushButton* makeOpBtn(const QString& label);
    QPushButton* makeUtilBtn(const QString& label);
    QPushButton* makeSciBtn(const QString& label);

    void buildBasicPanel();
    void buildScientificPanel();
    void buildHistoryPanel();
    void updateDisplay(const QString& text);
    void addToHistory(const QString& expr, const QString& result);
    void refreshHistoryPanel();
};

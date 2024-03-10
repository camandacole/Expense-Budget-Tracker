#ifndef EXPENSETRACKER_H
#define EXPENSETRACKER_H

#include <QWidget>
#include <QStandardItemModel>
#include <QCategoryAxis>
#include <QLineSeries>
#include <QMainWindow>
#include "expense.h"
#include "user.h"
#include "expensemanager.h"
#include <QTableView>

QT_BEGIN_NAMESPACE
namespace Ui { class ExpenseTracker; }
QT_END_NAMESPACE

/*
 * Expense Budget Tracker widget class that layout
 * a form to enter expenses and display them in tables and graphs
*/
class ExpenseTracker : public QWidget
{
    Q_OBJECT

public:
    ExpenseTracker(User, QWidget *parent = nullptr);
    void populateCategory();
    void populateSubCategory();
    void populateYearlyBox();
    QVector<Expense> prepareGraphData(QVector<Expense>);
    double getSumOfExpenses(QVector<Expense>);
    double getSumofIncome();
    void displayTotalExpenses();
    void displayTotalIncome();
    void sortExpenses(QVector<Expense>&);
    QChart* buildGraph(QVector<Expense>, int);
    QTableView* buildTableModel(QVector<Expense>, int);
    void setLoggedUser(User&);
    User getLoggedUser();
    ~ExpenseTracker();

private slots:
    void submitExpenseInfo();
    void displayTable(const QString&);
    void displayYearlyTable(const QString&);
    void displayGraph(const QString&);
    void addIncome();

private:
    Ui::ExpenseTracker *ui;
    QMainWindow tableReport;
    QMainWindow yearlyTableReport;
    QMainWindow* graphReport;
    User loggedUser;
    ExpenseManager manager;

};
#endif // EXPENSETRACKER_H

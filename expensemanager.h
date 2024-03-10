#ifndef EXPENSEMANAGER_H
#define EXPENSEMANAGER_H
#include <string>
#include <QVector>
#include <QTextStream>
#include "expense.h"
#include <QString>
#include "user.h"

/*
 * An ExpenseManger Class that reads and writes an expense
 * data to a text file
*/
class ExpenseManager
{
private:
    QString mTextFile = "./expenses.txt";
    User loggedUser;
public:
    ExpenseManager() = default;
    void setUser(User user);
    Expense getExpense(int id);
    QVector<Expense> getAllExpenses();
    void storeExpense(Expense& expense);
    QVector<Expense> filterBy(std::function<bool(Expense&)> predicate);
};

#endif // EXPENSEMANAGER_H

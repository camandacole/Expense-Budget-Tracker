#include "expensemanager.h"
#include "expense.h"
#include <QFile>
#include "category.h"
#include <QDebug>
#include <QDate>
#include <QString>

void ExpenseManager::setUser(User user){
    loggedUser = user;
}

/*
 * A static method that gets an Expense by Id
 * from a "expenses.txt" file
 * @param Id of Expense
 * @return The expense object
 * @throws Logic_error if no expense with the ID found
*/
Expense ExpenseManager::getExpense(int id)  {
    QVector<Expense> expenses = getAllExpenses();
    for(Expense& expense : expenses){
        if(expense.getId() == id){

            return expense;
        }
    }
    throw std::logic_error{"No expense found with this ID"};

}

/*
 * A static method that gets all expenses
 * from a "expenses.txt" file
 * @return A vector list of Expenses
 * @throws Logic_error if no record of expense is found
*/
QVector<Expense> ExpenseManager::getAllExpenses() {
    int counter = 0;
    QVector<Expense> expenses;
    QFile data(mTextFile);
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        while(!in.atEnd()){
            QString line{in.readLine()};
            counter++;
            if(counter == 1){
                continue;
            }
            if(!line.isEmpty()){
                QStringList list = line.split(",");
                int userID = list.at(5).toInt();
                if(userID == loggedUser.getUserId()){
                    Expense expense{};
                    expense.setId(list.at(0).toInt(new bool(true)));
                    std::string category = Category::getCategoryById(list.at(1).toInt(new bool(true))).getName();
                    expense.setCategory(category);
                    expense.setSubCategory(list.at(2).toStdString());
                    expense.setAmount(list.at(3).toDouble());
                    expense.setDate(list.at(4).toStdString());
                    expense.setUser(loggedUser);
                    expenses.push_back(expense);
                }
             }
          }
    }

    if(expenses.isEmpty()){
         throw std::logic_error{"No record of expenses was found"};
    }
    return expenses;
}

/*
 * A static method that stores expense
 * from a "expenses.txt" file
 * @param Expense object to store in file
*/
void ExpenseManager::storeExpense(Expense& expense) {
    QTextStream out;
    QFile data(mTextFile);
    out.setDevice(&data);
    int id{0};
    try {
        id = getAllExpenses().length();
        id++;
    } catch (std::logic_error e) {
        id = 1;
    }
    expense.setId(id);
    if(!data.exists()){
        data.open(QIODevice::WriteOnly | QIODevice::Append);
        out << "ExpenseID,CategoryId,SubCategory,Amount,Date,UserID" << "\n";
    }
    if(!data.isOpen()){
        data.open(QIODevice::WriteOnly | QIODevice::Append);
    }
     out << expense.toString().data() << "\n";
     data.close();

}

/*
 * A method that filters an Expense based on a certain predicate
 * from a "expenses.txt" file
 * @param predicate the lambda expression that is used to filter expenses
 * @return The vector list of expense object
 * @throws Logic_error if no expense with the subcategory is found
*/
QVector<Expense> ExpenseManager::filterBy(std::function<bool(Expense&)> predicate){
     QVector<Expense> expenses = getAllExpenses();
     QVector<Expense> results;
     for(Expense& expense : expenses){
         if(predicate(expense)){
             results.push_back(expense);
         }
     }
     if(results.isEmpty()){
         throw std::logic_error{"No record of expenses with this predicate was found"};
    }
    return results;
}

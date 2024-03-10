#ifndef EXPENSE_H
#define EXPENSE_H

#include <string>
#include "category.h"
#include "user.h"

/*
 * Expense Class that stores an expense information
*/
class Expense
{
private:
    int expenseID{-1};
    Category category;
    std::string subCategory;
    double amount{0.0};
    std::string date{"none"};
    User user;
public:
    Expense() = default;
    Expense(std::string_view, std::string_view, double, std::string_view);
    void setId(int);
    void setCategory(std::string);
    void setSubCategory(std::string);
    void setAmount(double);
    void setDate(std::string);
    void setUser(User);
    int getId() const;
    Category getCategory() const;
    std::string getSubCategory() const;
    double getAmount() const;
    std::string getDate() const;
    User getUser() const;
    std::string toString();
    bool operator==(const Expense& exp) const;
};

#endif // EXPENSE_H

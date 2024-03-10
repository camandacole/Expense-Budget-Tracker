#include "expense.h"
#include <sstream>

Expense::Expense(std::string_view categoryName, std::string_view subCategory, double amount, std::string_view date)
    : subCategory{ subCategory }, amount{ amount }, date{ date }
{
       setCategory(categoryName.data());
}

/*
 * sets the ID of an expense
 * @param ID of expense
*/
void Expense::setId(int ID) {
    this->expenseID = ID;
}

/*
 * sets the Category of an expense
 * @param Name of category
*/
void Expense::setCategory(std::string name) {
    this->category = Category::getCategoryByName(name.data());
}

/*
 * sets the subcategory of an expense
 * @param Subcategory of expense
*/
void Expense::setSubCategory(std::string subCategory) {
    this->subCategory = subCategory;
}

/*
 * sets the amount of an expense
 * @param Amount of expense
*/
void Expense::setAmount(double amount) {
    this->amount = amount;
}

/*
 * sets the date of an expense
 * @param Date of expense
*/
void Expense::setDate(std::string date) {
    this->date = date;
}

/*
 * sets the User of an expense
 * @param User of expense
*/
void Expense::setUser(User user) {
    this->user = user;
}

/*
 * gets the ID of an expense
 * @return ID of expense
*/
int Expense::getId() const {
    return this->expenseID;
}

/*
 * gets the Category of an expense
 * @return Category object of expense
*/
Category Expense::getCategory() const {
    return this->category;
}

/*
 * gets the Subcategory of an expense
 * @return Subcategory of expense
*/
std::string Expense::getSubCategory() const {
    return this->subCategory;
}

/*
 * gets the Amount of an expense
 * @return Amount of expense
*/
double Expense::getAmount() const {
    return this->amount;
}

/*
 * Gets the Date of an expense
 * @return Date of expense
*/
std::string Expense::getDate() const {
    return this->date;
}

/*
 * Gets the User of an expense
 * @return User of expense
*/
User Expense::getUser() const {
    return this->user;
}

/*
 * Gets the string representation of an expense
 * @return String representation of expense
*/
std::string Expense::toString() {
     std::stringstream ss;
     ss << expenseID << "," << category.getId() << "," << subCategory << "," << amount << "," << date << "," << user.getUserId();
     return ss.str();

}

bool Expense::operator==(const Expense& exp) const{
    return this->getDate() == exp.getDate();
}

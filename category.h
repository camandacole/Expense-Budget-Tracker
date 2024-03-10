#ifndef CATEGORY_H
#define CATEGORY_H
#include <string>
#include <QVector>
#include <QString>

/*
 * Category class that stores a category information
*/
class Category
{
private:
    int categoryID{-1};
    std::string name;
    static QString mTextFile;

public:
    Category() = default;
    Category(std::string_view);
    int getId();
    void setId(int);
    std::string getName();
    void setName(std::string_view);
    std::string toString();
    static Category getCategoryById(int);
    static Category getCategoryByName(QString);
    static QStringList getAllCategories();
    static void addCategory(QString);
};

#endif // CATEGORY_H

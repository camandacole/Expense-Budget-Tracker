#include "category.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

QString Category::mTextFile = ":/categories.txt";

Category::Category(std::string_view name)
   : name{ name.data() }
{

}

/*
 * Gets Id for category
 * @return Id of category
*/
int Category::getId() {
    return categoryID;
}


/*
 * Set Id for category
 * @param Id of category
*/
void Category::setId(int categoryID) {
    this->categoryID = categoryID;
}

/*
 * Gets Name of category
 * @return Name of category
*/
std::string Category::getName() {
    return name;
}

/*
 * Sets Name of category
 * @param Name of category
*/
void Category::setName(std::string_view name) {
    this->name = name.data();
}

/*
 * retruns string representation of category properties
 * @returns The string containing all category property values
*/
std::string Category::toString() {
    std::stringstream ss;
    ss << categoryID << " " << name;
    return ss.str();
}

/*
 * A static method that gets a category by Id
 * from a "categories.txt" file
 * @param Id of category
 * @return The category object
 * @throws Logic_error if no category with the ID found
*/
Category Category::getCategoryById(int id) {
    QStringList categories = getAllCategories();
    for(int i = 0; i < categories.length(); i++){
        int catId = i + 1;
        if(catId == id){
            Category category{};
            category.setId(catId);
            category.setName(categories.at(i).toStdString());

            return category;
        }
    }

    throw std::logic_error{"No category found with this ID"};
}

/*
 * A static method that gets a category by name
 * from a "categories.txt" file
 * @param Name of category
 * @return The category object
 * @throws Logic_error if no category with the Name found
*/
Category Category::getCategoryByName(QString name) {
    QStringList categories = getAllCategories();
    for(int i = 0; i < categories.length(); i++){
        int catId = i + 1;
        if(categories.at(i) == name){
            Category category{};
            category.setId(catId);
            category.setName(categories.at(i).toStdString());

            return category;
        }
    }
    throw std::logic_error{"No category found with this Name"};
}

/*
 * A static method that gets all categories
 * from a "categories.txt" file
 * @return The list of categories
 * @throws Logic_error if no category record is found
*/
QStringList Category::getAllCategories() {
    int counter{0};
    QStringList categories;
    QFile data(mTextFile);
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        while(!in.atEnd()){
            QString line{in.readLine()};
            counter++;
            if(counter == 1){
                continue;
            }
            QStringList list = line.split(",");
            categories.append(list.at(1));
        }
    }

    if(categories.isEmpty()){
        throw std::logic_error{"File has no record of categories"};
    }
    return categories;
}

/*
 * A static method that add a category to a
 * "categories.txt" file.
 * @param Name of category
*/
void Category::addCategory(QString name) {
    if(!getAllCategories().contains(name)){
        QTextStream out;
        QFile data(mTextFile);
        out.setDevice(&data);
        int id{0};
        try {
            id = getAllCategories().length();
            id++;
        } catch (std::logic_error e) {
            out << "ID,Name" << "\n";
            id = 1;
        }
        if (data.open(QFile::WriteOnly | QFile::Append)) {
            out << id << "," << name << "\n";
        }
    }

}


#include "databasemanager.h"
#include <QSqlQuery>

DatabaseManager::DatabaseManager()
{

}

/*
 * executes sql scripts that creates user and userlogin table
*/
void DatabaseManager::runTableScripts(){
   QSqlQuery query(QSqlDatabase::database("qtconnect"));
   query.exec("use ExpenseBudget;");
   query.exec("CREATE TABLE User ("
              "userID INTEGER AUTO_INCREMENT PRIMARY KEY,"
              "firstname VARCHAR(255) NOT NULL,"
              "lastname VARCHAR(255) NOT NULL,"
              "position VARCHAR(255) NOT NULL);");

   query.exec("CREATE TABLE UserLogin ("
               "loginID INTEGER AUTO_INCREMENT PRIMARY KEY,"
               "userID INT,"
               "FOREIGN KEY (userID) REFERENCES User(userID),"
               "username VARCHAR(255) NOT NULL,"
               "password VARCHAR(255) NOT NULL,"
               "accesslevel VARCHAR(255) NOT NULL);");
}

/*
 * Add user data to user mysql table
 * @param user the user object containing user credentials
 * @return true if insertion was successful
*/
bool DatabaseManager::addUser(User& user){
    QString firstname = user.getFirstName().data();
    QString lastname = user.getLastName().data();
    QString position = user.getPosition().data();

    QSqlQuery query(QSqlDatabase::database("qtconnect"));
    query.prepare("INSERT INTO user (firstname, lastname, position)"
                   "VALUES (:firstname, :lastname, :position)");
    query.bindValue(":firstname", firstname);
    query.bindValue(":lastname", lastname);
    query.bindValue(":position", position);

    return query.exec();
}

/*
 * Gets all users from the user table
 * @return vector of users
*/
QVector<User> DatabaseManager::getAllUsers(){
    QVector<User> users;
    QSqlQuery query(QSqlDatabase::database("qtconnect"));
    query.exec("SELECT * FROM user");
    while (query.next()) {
           User user;
           user.setUserId(query.value(0).toInt(new bool(true)));
           user.setFirstName(query.value(1).toString().toStdString());
           user.setLastName(query.value(2).toString().toStdString());
           user.setPosition(query.value(3).toString().toStdString());
           users.append(user);
       }
    return users;
}

/*
 * Adds data to user login table
 * @param userlogin object containing user credentials like username, password
 * @return true if insertion was successful
*/
bool DatabaseManager::addUserLogin(UserLogin& userLogin){
    int userId = userLogin.getUserId();
    QString username = userLogin.getUserName().data();
    QString password = userLogin.getPassword().data();
    QString accesslevel = userLogin.convertByteToString().data();

    QSqlQuery query(QSqlDatabase::database("qtconnect"));
    query.prepare("INSERT INTO userlogin (userID, username, password, accesslevel)"
                   "VALUES (:userID, :username, :password, :accesslevel)");
    query.bindValue(":userID", userId);
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":accesslevel", accesslevel);
    return query.exec();
}

/*
 * Gets user login credentials from user login table
 * @return vector of user logins
*/
QVector<UserLogin> DatabaseManager::getAllUserLogins(){
    QVector<UserLogin> userLogins;
    QSqlQuery query(QSqlDatabase::database("qtconnect"));
    query.exec("SELECT * FROM userlogin");
    while (query.next()) {
           UserLogin userLogin;
           userLogin.setLoginId(query.value(0).toInt(new bool(true)));
           userLogin.setUserId(query.value(1).toInt(new bool(true)));
           userLogin.setUserName(query.value(2).toString().toStdString());
           userLogin.setPassword(query.value(3).toString().toStdString());
           userLogin.setAccessLevel(query.value(4).toString().toStdString());
           userLogins.append(userLogin);
       }
    return userLogins;
}

/*
 * Updates upassword credential in user login table
 * @param username
 * @param password
 * @return true if update was successful
*/
bool DatabaseManager::updateUserLoginPassword(QString username, QString password){
    QSqlQuery query(QSqlDatabase::database("qtconnect"));

    query.prepare("UPDATE userlogin SET password=:pvalue WHERE username=:uvalue");
    query.bindValue(":pvalue", password);
    query.bindValue(":uvalue", username);
    bool isSuccess = query.exec();
    return isSuccess;
}

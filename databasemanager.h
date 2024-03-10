#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QVector>
#include "user.h"
#include "userlogin.h"
#include <qsqldatabase.h>
#include <QSql>

/*
 * Handles User and UserLogin tables by performing CRUD operations
*/
class DatabaseManager
{
public:
    DatabaseManager();
    void runTableScripts();
    bool addUser(User& user);
    QVector<User> getAllUsers();
    bool addUserLogin(UserLogin& user);
    QVector<UserLogin> getAllUserLogins();
    bool updateUserLoginPassword(QString username, QString password);


};

#endif // DATABASEMANAGER_H

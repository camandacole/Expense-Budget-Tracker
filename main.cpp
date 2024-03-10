#include "loginwidget.h"

#include <QApplication>
#include <QSqlDatabase>
#include "databasemanager.h"

/*
 * CHISOM AMANDA ONYENJI
 * CSC 284-ADV C++
 * ELIZABETH MILLER
 * FINAL PROJECT - EXPENSE BUDGET TRACKER
 * DATE:12/19/2022
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase connection = QSqlDatabase::addDatabase("QMYSQL", "qtconnect");
    connection.setHostName("127.0.0.1");
    connection.setDatabaseName("expensebudget");
    connection.setUserName("root");
    connection.setPassword("mysql45#");
    if(connection.open()){
        qInfo() << "connection sucessfully opened";
        DatabaseManager m;
        m.runTableScripts();
    }else{
        qInfo() << "connection sucessfully opened";
    }

    LoginWidget w;
    w.activateWindow();
    w.show();
    return a.exec();
}

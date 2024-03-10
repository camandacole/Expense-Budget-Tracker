#include "forgotpasswordserver.h"
#include <QTcpSocket>
#include "databasemanager.h"
#include "mythread.h"
#include <QMessageBox>


ForgotPasswordServer::ForgotPasswordServer(QObject *parent)
    :QTcpServer(parent)
{
    if (!this->listen(QHostAddress::LocalHost, 3000)) {
       qInfo() << "server is not listening";
    }
}

/*
 * sets the username and password data members
 * @param username
 * @param password
*/
void ForgotPasswordServer::setCredentials(QString username, QString password){
    mUsername = username;
    mPassword = password;
}

/*
 * This is an overriden function of QTcpserver that is called when a connection
 * is availale. It updates password credential and sends a message using a socket
 * It uses thread object to send message with socket
 * @param socketDescriptor uniquely identifies a socket
*/
void ForgotPasswordServer::incomingConnection(qintptr socketDescriptor)
{
    DatabaseManager manager;
    QString message;
    if(manager.updateUserLoginPassword(mUsername, mPassword)){
        message = "Your password was sucessfully updated.";
    }else{
        message = "Your was not sucessfully updated. Try Again";
    }
    MyThread *thread = new MyThread(socketDescriptor, message, this);
    connect(thread, &MyThread::finished, thread, &MyThread::deleteLater);
    thread->start();


}

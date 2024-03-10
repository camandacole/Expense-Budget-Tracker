#ifndef FORGOTPASSWORDSERVER_H
#define FORGOTPASSWORDSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

/*
 * A server responsible for updating password crdential for users
 * that forgot their passwords
*/
class ForgotPasswordServer : public QTcpServer
{
    Q_OBJECT

private:
    QString mUsername;
    QString mPassword;

public:
    explicit ForgotPasswordServer(QObject *parent);
    void setCredentials(QString username, QString password);

protected:
    void incomingConnection(qintptr socketDescriptor) override;


};

#endif // FORGOTPASSWORDSERVER_H

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QObject>
#include <QTcpSocket>

/*
 * Thread class that updates password aand sends a success or error message using socket
 * It is used by ForgotPassword Server
*/
class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(int socketDescriptor, const QString &text, QObject *parent);
    void run() override;

private:
    int socketDescriptor;
    QString text;
};

#endif // MYTHREAD_H

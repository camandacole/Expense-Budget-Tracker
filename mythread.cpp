#include "mythread.h"

MyThread::MyThread(int socketDescriptor, const QString &text, QObject *parent)
    : QThread{parent}, socketDescriptor(socketDescriptor), text(text)
{
}

/*
 * Overriden from the QThreaf class. It uses
 * socket to send message
*/
void MyThread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
           qInfo() << "PROBLEM: socket was not successfully set";
           return;
       }
      QByteArray block;
      QDataStream out(&block, QIODevice::WriteOnly);
      out.setVersion(QDataStream::Qt_4_0);
      out << text;

      tcpSocket.write(block);
      tcpSocket.disconnectFromHost();
      tcpSocket.waitForDisconnected();
}

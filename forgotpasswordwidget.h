#ifndef FORGOTPASSWORDWIDGET_H
#define FORGOTPASSWORDWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include "forgotpasswordserver.h"

namespace Ui {
class ForgotPasswordWidget;
}

/*
 * Forgot password window for updating passwords
*/
class ForgotPasswordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ForgotPasswordWidget(QWidget *parent = nullptr);
    ~ForgotPasswordWidget();

private slots:
    void resetPassword();
    void readMessage();

private:
    Ui::ForgotPasswordWidget *ui;
    QTcpSocket* socket;
    ForgotPasswordServer* server;
    QDataStream in;
};

#endif // FORGOTPASSWORDWIDGET_H

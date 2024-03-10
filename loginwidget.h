#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWidget; }
QT_END_NAMESPACE
#include "homewidget.h"
#include "registerwidget.h"
#include "forgotpasswordwidget.h"

/*
 * Login window for logginh in users
*/
class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

private slots:
    void loginUser();
    void redirectToRegister();
    void redirectToReset();

private:
    Ui::LoginWidget *ui;
    HomeWidget homePage;
    RegisterWidget registerPage;
    ForgotPasswordWidget resetPasswordPage;

};
#endif // LOGINWIDGET_H

#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "databasemanager.h"
#include "homewidget.h"
#include "registerwidget.h"
#include <algorithm>
#include <QVector>
#include <QMessageBox>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    QObject::connect(ui->loginButton, SIGNAL(clicked(bool)), this, SLOT(loginUser()));
    QObject::connect(ui->redirectButton, SIGNAL(clicked(bool)), this, SLOT(redirectToRegister()));
    QObject::connect(ui->redirectResetButton, SIGNAL(clicked(bool)), this, SLOT(redirectToReset()));
}

LoginWidget::~LoginWidget()
{
    delete ui;
}


/*
 * Gets login credentials, checks if it exists in User login table
 * If credential exists, it grants access to the welcome page
*/
void LoginWidget::loginUser(){
 try{
    // content here
    DatabaseManager manager;
    QString username = ui->userNameTextField->text();
    QString password = ui->passwordTextField->text();
    QVector<UserLogin> userlogins = manager.getAllUserLogins();
    auto it = std::find_if(userlogins.begin(), userlogins.end(),
    [=](UserLogin& userlogin){return userlogin.getUserName().data() == username && userlogin.getPassword().data() == password;});
    if(it == userlogins.end()){
        QMessageBox::information(this, tr("Problem"),
                                        tr("No credential matches the info"));
        ui->userNameTextField->clear();
        ui->passwordTextField->clear();
    }else{
        ui->userNameTextField->clear();
        ui->passwordTextField->clear();
        this->close();
        homePage.setText(username);
        homePage.show();
    }

    }catch(std::logic_error& e){
        ui->loginLabel->setText(e.what());
    }

}

/*
 * redirects to register page
*/
void LoginWidget::redirectToRegister(){
    registerPage.show();

}

/*
 * redirects to reset password page
*/
void LoginWidget::redirectToReset(){
    resetPasswordPage.show();
}


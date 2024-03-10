#include "registerwidget.h"
#include "ui_registerwidget.h"
#include "user.h"
#include "userlogin.h"
#include "databasemanager.h"
#include <QSqlQuery>
#include <QMessageBox>

RegisterWidget::RegisterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterWidget)
{
    ui->setupUi(this);
    QObject::connect(ui->registerButton, SIGNAL(clicked(bool)), this, SLOT(registerUser()));
    QObject::connect(ui->positionBox, SIGNAL(currentTextChanged(QString)), this, SLOT(displayField(QString)));
    ui->positionBox->addItems({"USER", "ADMIN", "DEVELOPER"});
    ui->codeTextField->setPlaceholderText("Enter Access Code");
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}

/*
 * Gets user credentials, and register users by inserting to the Userlogin and User Tables
 * If username and password credentials already exists, it outputs an error message
 * If it does not exist, it successfully inserts credentials and redirects to login page
*/
void RegisterWidget::registerUser()
{
    DatabaseManager manager;
    bool isSuccess = false;
    bool isSuccess_2 = false;
    User user;
    UserLogin userLogin;
    QString firstname = ui->firstnameTextField->text();
    QString lastname = ui->lastnameTextField->text();
    QString username = ui->usernameTextField->text();
    QString password = ui->passwordTextField->text();
    QString position = ui->positionBox->currentText();
    QString accesscode = ui->codeTextField->text();
    if(ui->codeTextField->isEnabled()){
        if(accesscode != "expenseb55"){
            QMessageBox::information(this, tr("Problem"),
                                            tr("%1 : You Access code is not valid").arg(position));
            return;
        }
    }
    user.setFirstName(firstname.toStdString());
    user.setLastName(lastname.toStdString());
    user.setPosition(position.toStdString());
    isSuccess = manager.addUser(user); // add user

    QVector<UserLogin> userlogins = manager.getAllUserLogins(); // get userlogin
    // check if the username already exists
    auto it = std::find_if(userlogins.begin(), userlogins.end(),
    [=](UserLogin& userlogin){return userlogin.getUserName().data() == username || userlogin.getPassword().data() == password;});
    if(it == userlogins.end()){
        QSqlQuery query(QSqlDatabase::database("qtconnect"));
        query.exec("SELECT * FROM user ORDER BY userID DESC LIMIT 1");
        while (query.next()) { user.setUserId(query.value(0).toInt(new bool(true))); }
        userLogin.setUserId(user.getUserId());
        userLogin.setUserName(username.toStdString());
        userLogin.setPassword(password.toStdString());
        userLogin.setAccessLevel(determinePrivilege(position));
        isSuccess_2 = manager.addUserLogin(userLogin);
    }
     // if queries were successful, print success message
    if(isSuccess && isSuccess_2){
        QMessageBox::information(this, tr("Success!"),
                                        tr("You Sucessfully Registered!!"));
        ui->firstnameTextField->clear();
        ui->lastnameTextField->clear();
        ui->usernameTextField->clear();
        ui->passwordTextField->clear();
        this->close();

    }else{
        QMessageBox::information(this, tr("Problem"),
                                        tr("You did not sucessfully register because your username or password already exists"));
    }

}

/*
 * Determines the priviledge of a user based on their position
 * @param position the poition of the user: Admin, user, developer
 * @return the privileges of the user in byte
*/
std::byte RegisterWidget::determinePrivilege(QString position){
    if(position == "USER"){
         return (std::byte)(READ | WRITE);
    }else if(position == "ADMIN"){
        return (std::byte)(READ | WRITE | EDIT | DELETE);

    }else if(position == "DEVELOPER"){
        return (std::byte)(READ | WRITE | EDIT | DELETE);

    }

   return (std::byte)0b00000000;
}

/*
 * Shows a text field for users who are admins or developers
 * This field is used to enter a private access code inorder to register
 * as as admin or developer because it gives more priviledges
 * @param arg position of the user
*/
void RegisterWidget::displayField(QString arg){
    if(arg == "ADMIN" || arg == "DEVELOPER"){
        ui->codeTextField->setMinimumHeight(41);
        ui->codeTextField->setEnabled(true);

    }else{
        ui->codeTextField->setMinimumHeight(0);
        ui->codeTextField->setEnabled(false);
    }

}


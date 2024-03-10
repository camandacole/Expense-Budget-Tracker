#include "forgotpasswordwidget.h"
#include "ui_forgotpasswordwidget.h"
#include <QMessageBox>
#include <QTcpSocket>

ForgotPasswordWidget::ForgotPasswordWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ForgotPasswordWidget),
    socket(new QTcpSocket(this)),
    server(new ForgotPasswordServer(this))
{
    ui->setupUi(this);

    QObject::connect(ui->submitButton, SIGNAL(clicked(bool)), this, SLOT(resetPassword()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));

}

/*
 * Gets username and password data and connects to a server on
 * listening on localhost 3000
*/
void ForgotPasswordWidget::resetPassword(){
         QString username = ui->usernameTextField->text();
         QString password = ui->passwordTextField->text();
         QString confirmPassword = ui->confirmPasswordField->text();
         if(password == confirmPassword){
               server->setCredentials(username, password);
               in.setDevice(socket);
               socket->abort();
               socket->connectToHost(QHostAddress::LocalHost,3000);
               ui->usernameTextField->clear();
               ui->passwordTextField->clear();
               ui->confirmPasswordField->clear();

         }else{
             QMessageBox::information(this, tr("Problem!"),
                                             tr("Your password and confirm password does not match."));
         }
}


/*
 * Reads the message from the server and outputs to a message box
*/
void ForgotPasswordWidget::readMessage(){
   in.startTransaction();

    QString message;
    in >> message;

    if (!in.commitTransaction())
        return;

    QMessageBox::information(this, tr(""),
                                        tr("%1").arg(message));
    this->close();
}

ForgotPasswordWidget::~ForgotPasswordWidget()
{
    delete ui;
}

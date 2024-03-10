#include "homewidget.h"
#include "ui_homewidget.h"
#include "databasemanager.h"

HomeWidget::HomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWidget)
{
    ui->setupUi(this);
    button = new QPushButton("Go to budget App");
    QObject::connect(button, SIGNAL(clicked(bool)), this, SLOT(redirectToMainApp()));
    layout.addWidget(&welcomeMeassage);
    layout.addWidget(button);
    layout.setAlignment(Qt::AlignHCenter);
    setLayout(&layout);
    this->resize(400, 300);
    this->setWindowTitle("Home");
}


/*
 * Sets welcome message
 * @param text
*/
void HomeWidget::setText(QString text){
    QFont font;
    font.setPointSize(20);
    username = text;
    welcomeMeassage.setText(tr("You are logged in %1 !!!").arg(username));
    welcomeMeassage.setFont(font);
}

void HomeWidget::redirectToMainApp(){
    DatabaseManager manager;
    User loggedUser;
    for(UserLogin& login : manager.getAllUserLogins()){
         if(login.getUserName().data() == username){
             for(User& user : manager.getAllUsers()){
                  if(user.getUserId() == login.getUserId()){
                      loggedUser = user;
                  }
             }
         }
    }
    app = new ExpenseTracker(loggedUser);
    app->show();
}

HomeWidget::~HomeWidget()
{
    delete ui;
}

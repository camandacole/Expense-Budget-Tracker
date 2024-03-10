#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>

namespace Ui {
class RegisterWidget;
}

/*
 * Register window for registering users
*/
class RegisterWidget : public QWidget
{
    Q_OBJECT

public:
    enum PRIVILEDGE{READ = 0b00000001, WRITE = 0b00000010, EDIT = 0b00000100, DELETE = 0b00001000};
    explicit RegisterWidget(QWidget *parent = nullptr);
    std::byte determinePrivilege(QString);
    ~RegisterWidget();

private slots:
    void registerUser();
    void displayField(QString);

private:
    Ui::RegisterWidget *ui;
};

#endif // REGISTERWIDGET_H

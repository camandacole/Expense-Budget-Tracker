#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "expensetracker.h"

namespace Ui {
class HomeWidget;
}

/*
 * Home window that redirects user to the expense page
*/
class HomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);
    void setText(QString text);
    ~HomeWidget();

private slots:
    void redirectToMainApp();

private:
    Ui::HomeWidget *ui;
    QLabel welcomeMeassage;
    QString username;
    QPushButton* button;
    QVBoxLayout layout;
    ExpenseTracker* app;
};

#endif // HOMEWIDGET_H

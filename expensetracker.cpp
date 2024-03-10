#include "expensetracker.h"
#include "./ui_expensetracker.h"
#include "category.h"
#include "expense.h"
#include "expensemanager.h"
#include <QVector>
#include <QTableView>
#include <QtCharts>
#include <QTime>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QScrollArea>
#include <algorithm>
#include "tablenumberformat.h"

ExpenseTracker::ExpenseTracker(User user, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExpenseTracker)
{
    ui->setupUi(this);
    QObject::connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(submitExpenseInfo()));
    QObject::connect(ui->incomeButton, SIGNAL(clicked(bool)), this, SLOT(addIncome()));
    QObject::connect(ui->displayBox, SIGNAL(currentTextChanged(QString)), this, SLOT(displayTable(QString)));
    QObject::connect(ui->displayBox_2, SIGNAL(currentTextChanged(QString)), this, SLOT(displayGraph(QString)));
    QObject::connect(ui->yearlyBox, SIGNAL(currentTextChanged(QString)), this, SLOT(displayYearlyTable(QString)));
    ui->displayYear->setText(tr("Add income sources without tax for this month %1/%2")
                             .arg(QDate::currentDate().month()).arg(QDate::currentDate().year()));
    manager.setUser(user);
    loggedUser = user;
    populateCategory();
    populateSubCategory();
    displayTotalIncome();
    displayTotalExpenses();
    populateYearlyBox();
}

/*
 * populate category values into the comboboxes used to select
 * a category for the expense
*/
void ExpenseTracker::populateCategory(){
    try {
         QStringList categories = Category::getAllCategories();
         ui->categoryBox->addItems(categories);
         ui->displayBox->addItem("Display All");
         ui->displayBox->addItems(categories);

    } catch (std::logic_error err) {
        qInfo() << "from populate category" << err.what();
    }
}

void ExpenseTracker::populateYearlyBox(){
    try {
         QStringList s;
         for(Expense& exp : manager.getAllExpenses()){
             int year = QDate::fromString(exp.getDate().data(), "MM/d/yyyy").year();
             s.append(QString::number(year));
         }
         s.removeDuplicates();
         ui->yearlyBox->addItem("Display All");
         ui->yearlyBox->addItems(s);
    } catch (std::logic_error err) {
        ui->yearlyBox->addItem("Display All");
        qInfo() << "populateYearlyBox():" << err.what();
    }

}

/*
 * get userincome and store it in a file of incomes
*/
void ExpenseTracker::addIncome(){
    QString sources = ui->sourceTextField->text();
    QString income = ui->incomeTextField->text();
    QTextStream out;
    QFile data("./incomes.txt");
    out.setDevice(&data);
    if(!data.exists()){
        data.open(QIODevice::WriteOnly | QIODevice::Append);
        out << "source,amount,monthperiod,userID" << "\n";
    }
    if(!data.isOpen()){
       data.open(QIODevice::WriteOnly | QIODevice::Append);
     }
    out << sources << "," << income << "," << QDate::currentDate().month()
        << "/" << QDate::currentDate().year() << "," << loggedUser.getUserId() << "\n";
    data.close();
    ui->sourceTextField->clear();
    ui->incomeTextField->clear();
    QMessageBox::information(this, tr("Expense Budget Tracker"),
                                    tr("You successful added an income!!!"));
    displayTotalIncome();
}

/*
 * Gets expense info from input form and a stores it
 * in a "expenses.txt" file
*/
void ExpenseTracker::submitExpenseInfo()
{
    QDate date = ui->dateTextField->date();
    if(date.month() != QDate::currentDate().month()  || date.year() != QDate::currentDate().year()){
        QMessageBox::information(this, tr("Expense Budget Tracker"),
                                        tr("Please enter an expense date within the current month and year"));
        return;
    }
    Expense expense;
    expense.setCategory(ui->categoryBox->currentText().toStdString());
    expense.setSubCategory(ui->subcategoryTextField->text().toLower().toStdString());
    expense.setAmount(ui->amountTextField->text().toDouble(new bool(true)));
    expense.setDate(ui->dateTextField->text().toStdString());
    expense.setUser(loggedUser);

    try {
       manager.storeExpense(expense);
       ui->subcategoryTextField->clear();
       ui->amountTextField->clear();
       QMessageBox::information(this, tr("Expense Budget Tracker"),
                                       tr("You successful added a budget!!!"));
       displayTotalExpenses();
       if(ui->displayBox_2->findText(expense.getSubCategory().data()) == -1){
             ui->displayBox_2->addItem(expense.getSubCategory().data());
       }
       QString year_string = QString::number(date.year());
       if(ui->yearlyBox->findText(year_string) == -1){
             ui->yearlyBox->addItem(year_string);
       }
    } catch (std::logic_error err) {
       qInfo() << "submitExpenseInfo()" << err.what();
    }

}

/*
 * Display a table of expenses based on the expense category selected
 * @param category, the name of category that expenses should be
 * displayed by
*/
void ExpenseTracker::displayTable(const QString& categoryName)
{
    QVector<Expense> expenses;
    try {
        if(categoryName.toLower() == "display all"){
            expenses = manager.filterBy([=](Expense& exp){
                return QDate::fromString(exp.getDate().data(), "MM/d/yyyy").month() == QDate::currentDate().month()
                && QDate::fromString(exp.getDate().data(), "MM/d/yyyy").year() == QDate::currentDate().year();});
        }else{
             expenses = manager.filterBy([=](Expense& exp){ return exp.getCategory().getName().data() == categoryName
                && QDate::fromString(exp.getDate().data(), "MM/d/yyyy").month() == QDate::currentDate().month()
                && QDate::fromString(exp.getDate().data(), "MM/d/yyyy").year() == QDate::currentDate().year();});
        }
        sortExpenses(expenses);
    } catch (std::logic_error err) {
       qInfo() << "displayTable(): " << err.what();
    }

    QTableView* tableView = buildTableModel(expenses, 4);

    tableReport.resize(430, 250);
    tableReport.setCentralWidget(tableView);
    tableReport.setWindowTitle(tr("Your Expense Report for the month %1/%2")
                               .arg(QDate::currentDate().month())
                               .arg(QDate::currentDate().year()));
    tableReport.activateWindow();
    tableReport.show();
}

/*
 * Display a table of expenses based on the year. e.g if 2022 is selected, it displays all expenses for that year
 * @param year the year expenses should be displayed by
*/
void ExpenseTracker::displayYearlyTable(const QString& year){
    QVector<Expense> expenses;
    try {
        if(year.toLower() == "display all"){
            expenses = manager.getAllExpenses();
        }else{
             expenses = manager.filterBy([=](Expense& exp){
                 return QDate::fromString(exp.getDate().data(), "MM/d/yyyy").year() == QDate::currentDate().year();});
        }
        sortExpenses(expenses);
    } catch (std::logic_error err) {
       qInfo() << "displayTable(): " << err.what();
    }

    QTableView* tableView = buildTableModel(expenses, 4);
    yearlyTableReport.resize(430, 250);
    yearlyTableReport.setCentralWidget(tableView);
    yearlyTableReport.setWindowTitle(tr("Your Expense Report for the year %1")
                               .arg(year));
    yearlyTableReport.activateWindow();
    yearlyTableReport.show();
}

/*
 * Builds the table of data and returns a tableview
 * @param expenses, the expenses to be displayed in table
 * @param col, the number of column for the table
 * @returns view of the table data
*/
QTableView* ExpenseTracker::buildTableModel(QVector<Expense> expenses, int col){
    QStandardItemModel* model = new QStandardItemModel(expenses.length() + 1, col);
    QStringList headers = {"Category", "Subcategory", "Amount", "Date"};
    model->setHorizontalHeaderLabels(headers);
    int lastrow = 0;
    for(int row = 0; row < expenses.length(); row++){
        QStringList texts;
        texts.append(expenses.at(row).getCategory().getName().data());
        texts.append(expenses.at(row).getSubCategory().data());
        texts.append(QString::number(expenses.at(row).getAmount(), 'g', 3));
        texts.append(expenses.at(row).getDate().data());
        for(int col = 0; col < 4; col++){
            QModelIndex index = model->index(row, col, QModelIndex());
            model->setData(index, texts.at(col));
        }
    }
    lastrow = expenses.length();

    // DISPLAY THE TOTAL EXPENSES HERE
    QModelIndex index = model->index(lastrow, 0, QModelIndex());
    model->setData(index, "");
    QModelIndex index1 = model->index(lastrow, 1, QModelIndex());
    model->setData(index1, "TOTAL->>>");
    QModelIndex index2 = model->index(lastrow, 2, QModelIndex());
    model->setData(index2, QString::number(getSumOfExpenses(expenses), 'g', 3));
    QModelIndex index3 = model->index(lastrow, 3, QModelIndex());
    model->setData(index3, "");

    QTableView* tableView = new QTableView();
    tableView->setItemDelegateForColumn(2, new TableNumberFormat(this));
    tableView->setModel(model);
    tableView->setEnabled(true);
    tableView->setColumnWidth(0, 200);
    tableView->setColumnWidth(3, 200);
    return tableView;
}

/*
 * populate subcategory values into the comboboxes to be used
 * to display graph
*/
void ExpenseTracker::populateSubCategory(){
   try {
        QStringList s;
        for(Expense& exp : manager.getAllExpenses()){
             s.append(exp.getSubCategory().data());
        }
        s.removeDuplicates();
        ui->displayBox_2->addItems(s);
   } catch (std::logic_error err) {
       qInfo() << "populateSubCategory()" << err.what();
   }

}

/*
 * Display a graph of expenses based on the expense subcategory selected
 * @param subcategory, the name of subcategory that expenses should be
 * displayed by in the graph
*/
void ExpenseTracker::displayGraph(const QString& subcategoryName){
    QVector<Expense> expenses;
    int maxAmount;
    Expense maxExpense; // the maximum expenses. needed when labelling the YAXIS
    try {
        expenses = manager.filterBy([=](Expense& exp){ return exp.getSubCategory().data() == subcategoryName
           && QDate::fromString(exp.getDate().data(), "MM/d/yyyy").month() == QDate::currentDate().month()
           && QDate::fromString(exp.getDate().data(), "MM/d/yyyy").year() == QDate::currentDate().year();});
       sortExpenses(expenses);
       expenses = prepareGraphData(expenses);
       auto max = std::max_element(expenses.begin(), expenses.end(),
                        [](const Expense& first, const Expense& second){ return first.getAmount() < second.getAmount();});
       maxExpense = *max;
       maxAmount = maxExpense.getAmount();
    } catch (std::logic_error err) {
       qInfo() << "displayGraph(): " << err.what();
    }
    QChart *chart = buildGraph(expenses, maxAmount);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    graphReport = new QMainWindow(this);
    graphReport->resize(430, 250);
    graphReport->setCentralWidget(chartView);
    graphReport->setWindowTitle(tr("Your Graph Report For for the month %1/%2: %3(%4)")
                                .arg(QDate::currentDate().month())
                                .arg(QDate::currentDate().year())
                                .arg(maxExpense.getCategory().getName().data())
                                .arg(subcategoryName.toUpper()));
    graphReport->activateWindow();
    graphReport->show();

}

/*
 * Builds the graph chart and returns it. This chart returned can be displayed with chartview object
 * @param expenses the expenses to be displayed in the graph
 * @param maxAmount the expenses with the maximum amount. This is usely to label the YAXIS up to the max amount
 * @return chart of the plotted data
*/
QChart* ExpenseTracker::buildGraph(QVector<Expense> expenses, int maxAmount){
    QLineSeries *series = new QLineSeries();
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);

    QCategoryAxis *axisX = new QCategoryAxis();
    QCategoryAxis *axisY = new QCategoryAxis();

    int x {1};
    /*int increaseBy = 0;
    if((maxAmount % 2) == 0){ // dertermines wether to label y axis based on 10s or 5s
        increaseBy = 10;
    }else{
        increaseBy = 5;
    }*/
    int increaseBy = 0;
    if((maxAmount % 10) == 0){ // dertermines wether to label y axis based on 10s or 5s
        increaseBy = maxAmount / 5;
    }else{
        increaseBy = maxAmount % 10;
    }
    for(int i = 0; i <= maxAmount; i+=increaseBy){
        axisY->append(QString("$%1").arg(i), i);
    }
   *series << QPointF(0, 0);
   axisX->append("", 0);
   for(Expense& exp : expenses){
       *series << QPointF(x, exp.getAmount());
        axisX->append(exp.getDate().data(), x);
        x += 1;
   }

    //setting the range of the axist
    axisX->setRange(0, expenses.size());
    axisY->setRange(0, maxAmount);
    //Align Axis
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    //Attach axis to series
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    // Customize axis label font
    QFont labelsFont;
    labelsFont.setPixelSize(12);
    axisX->setLabelsFont(labelsFont);
    axisY->setLabelsFont(labelsFont);
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    axisY->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    return chart;

}


/*
 * This method first sorts vector of expenses in accending order by dates(earlist to latest)
 * It then merges expenses with the same dates into one expense object by suming the amounts
 * This is useful for displaying the expenses in graph by increase the amountvalues in the y-axis and avoiding
 * duplicated dates in the graph
 * from a "expenses.txt" file
 * @param vector of expenses
 * @return Vector of expenses
*/
QVector<Expense> ExpenseTracker::prepareGraphData(QVector<Expense> expenses){
    QVector<Expense> results;

    int i = 0;
    int j = 1;
         while(i < expenses.size()){
             int count = expenses.count(expenses.at(i));
                 double amount = 0;
                 Expense exp;
                 while(j <= count){
                     exp = expenses.at(i);
                     exp.setAmount(0.0);
                     amount += expenses.at(i).getAmount();
                     i++;
                     j++;
                 }
                 j = 1;
                 exp.setAmount(amount);
                 results.append(exp);
         }

         return results;
}

/*
 * Calculates the sum of all expenses to be displayed in Table
 * @param vector of expenses
 * @return the sum of all expenses
*/
double ExpenseTracker::getSumOfExpenses(QVector<Expense> expenses){
    double sum = 0;
    for(Expense& exp : expenses){
        sum += exp.getAmount();
    }
    return sum;
}

/*
 * calculates and returns the sum of all user incomes
*/
double ExpenseTracker::getSumofIncome(){
    int counter = 0;
    double total = 0;
    QFile data("./incomes.txt");
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        while(!in.atEnd()){
           QString line{in.readLine()};
           counter++;
           if(counter == 1){
               continue;
           }
           QStringList list = line.split(",");
           QString currentDate = tr("%1/%2").arg(QDate::currentDate().month()).arg(QDate::currentDate().year());
           QString period = list.at(2);
           int userID = list.at(3).toDouble();
           if(loggedUser.getUserId() == userID && period == currentDate){
                 total += list.at(1).toDouble();
           }

        }
    }

    return total;
}

/*
 * Displays the total expenses to the user so far
*/
void ExpenseTracker::displayTotalExpenses(){
    try{
        double sum = 0;
        QVector<Expense> expenses = manager.getAllExpenses();
        sum = getSumOfExpenses(expenses);
        sortExpenses(expenses);
        QString firstDate = expenses.at(0).getDate().data();
        QString secondDate = expenses.at(expenses.size() - 1).getDate().data();
        ui->displayTotalExp->setText(tr("Total Expenses Spent From %1 to %2 is %3").arg(firstDate).arg(secondDate).arg(sum));
    } catch (std::logic_error err) {
        qInfo() << "displayTotalExpenses()" << err.what();
    }
}

/*
 * displays total income to the user
*/
void ExpenseTracker::displayTotalIncome(){
    double currenTotal = getSumofIncome();
    ui->displayTotalInc->setText(tr("Total Income: %1").arg(currenTotal));
}

/*
 * sorts a vector of expenses in accending order
*/
void ExpenseTracker::sortExpenses(QVector<Expense>& expenses){
    std::sort(expenses.begin(), expenses.end(),
              [](const Expense& first, const Expense& second)
    { return QDate::fromString(first.getDate().data(), "MM/d/yyyy") < QDate::fromString(second.getDate().data(), "MM/d/yyyy");});

}

/*
 * sets the current logged in user
*/
void ExpenseTracker::setLoggedUser(User& user){
    loggedUser = user;
}

/*
 * gets the current logged in user
*/
User ExpenseTracker::getLoggedUser(){
    return loggedUser;
}

ExpenseTracker::~ExpenseTracker()
{
    delete ui;
}

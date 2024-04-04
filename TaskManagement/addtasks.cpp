#include "addtasks.h"
#include "ui_addtasks.h"

addTasks::addTasks(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addTasks)
{
    ui->setupUi(this);
    // ui->dependanceBox->addItem("text");
}

addTasks::~addTasks()
{
    delete ui;
}

void addTasks::fillAllTaks(QList<QString> L)
{
    // Q_FOREACH(const QString s , L)
    // {
    //     allTasks.append(s);
    // }

    Q_FOREACH(const QString s , L)
    {
        ui->dependanceBox->addItem(s);
    }
}

QString addTasks::getNameString()
{
    return nameString;
}

int addTasks::getDurationInt()
{
    return durationInt;
}

QString addTasks::getDependanceString()
{
    return dependanceString;
}


void addTasks::on_buttonBox_accepted()
{
    nameString = ui->nameString->text();
    durationInt = ui->durationInt->value();
    dependanceString = ui->dependanceBox->currentText();
    accept();
}


void addTasks::on_buttonBox_rejected()
{
    reject();
}


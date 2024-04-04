#include "addtasks.h"
#include "ui_addtasks.h"

addTasks::addTasks(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addTasks)
{
    ui->setupUi(this);
}

addTasks::~addTasks()
{
    delete ui;
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
    dependanceString = ui->dependanceString->text();
    accept();
}


void addTasks::on_buttonBox_rejected()
{
    reject();
}


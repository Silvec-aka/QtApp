#include "addtask.h"
#include "ui_addtask.h"

addTask::addTask(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addTask)
{
    ui->setupUi(this);
}

addTask::~addTask()
{
    delete ui;
}

QString addTask::getNameString()
{
    return nameString;
}

int addTask::getDurationInt()
{
    return durationInt;
}

QString addTask::getDependanceString()
{
    return dependanceString;
}

void addTask::on_buttonBox_accepted()
{
    nameString = ui->nameString->document()->toRawText();
    durationInt = ui->durationInt->value();
    dependanceString = ui->dependanceString->document()->toRawText();
    accept();
}


void addTask::on_buttonBox_rejected()
{
    reject();
}

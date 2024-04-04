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

QString addTask::getDurationString()
{
    return durationString;
}

QString addTask::getDependanceString()
{
    return dependanceString;
}

void addTask::on_buttonBox_accepted()
{
    nameString = ui->nameString->document()->toRawText();;
    durationString = ui->durationString->document()->toRawText();
    dependanceString = ui->dependanceString->document()->toRawText();
    accept();
}


void addTask::on_buttonBox_rejected()
{
    reject();
}

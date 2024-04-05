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

void addTasks::fillAllTaks(QList<QString> L)
{
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

bool addTasks::getIsTerminal() const
{
    return isTerminal;
}

bool addTasks::getIsPrincipale() const
{
    return isPrincipale;
}

bool addTasks::getAccepted() const
{
    return accepted;
}

void addTasks::on_buttonBox_accepted()
{
    nameString = ui->nameString->text();
    durationInt = ui->durationInt->value();
    dependanceString = ui->dependanceBox->currentText();
    isTerminal = ui->isTerminal->isChecked();
    isPrincipale = ui->isPrincipale->isChecked();

    accepted = true;

    accept();
}


void addTasks::on_buttonBox_rejected()
{
    accepted = false;
    reject();
}


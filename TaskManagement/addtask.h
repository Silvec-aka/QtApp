#ifndef ADDTASK_H
#define ADDTASK_H

#include <QDialog>
#include <QWidget>
namespace Ui {
    class addTask;
}

class addTask : public QDialog
{
public:
    explicit addTask(QWidget *parent = nullptr);
    ~addTask();

    QString getNameString();
    QString getDurationString();
    QString getDependanceString();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::addTask *ui;
    QString nameString;
    QString durationString;
    QString dependanceString;
};

#endif // ADDTASK_H

#ifndef ADDTASKS_H
#define ADDTASKS_H

#include <QDialog>

namespace Ui {
class addTasks;
}

class addTasks : public QDialog
{
    Q_OBJECT

public:
    explicit addTasks(QWidget *parent = nullptr);
    ~addTasks();

    QString getNameString();
    int getDurationInt();
    QString getDependanceString();
    void fillAllTaks(QList<QString>);
    bool getIsTerminal() const;
    bool getIsPrincipale() const;

    bool getAccepted() const;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::addTasks *ui;
    QString nameString;
    int durationInt;
    QString dependanceString;
    bool isTerminal;
    bool isPrincipale;

    bool accepted;

    QList<QString> allTasks;
};

#endif // ADDTASKS_H

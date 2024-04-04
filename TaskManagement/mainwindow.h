#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tache.h"
#include "tachecomposite.h"
#include <QModelIndexList>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSauvegarder_triggered();
    void on_actionAjouter_triggered();
    void on_actionOuvrir_triggered();
    void onTableViewElementSelected(const QModelIndex &index);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<Tache*> *taches;
    int id_ = 0;
    Tache* descriptionTask_ = NULL; // tâche dont la description est affichée

    QList<int>* idsComposite;

    QString nameString;
    int durationInt;
    QString dependanceString;
    bool isTerminal;
    bool isPrincipale;
    bool writeToJson(const QString & filename);
    bool loadFromJson(const QString & filename);
    Tache* findTache(int id) const;
    Tache* findTacheByName(QString name) const;
    void AddTaskTerminal(const QString nom, int duree, const QString dependances, bool isPrincipale);
    void AddTaskComposite(const QString nom, int duree, const QString dependances, bool isPrincipale);
    TacheComposite* findTacheComposite(int id) const;
    void UpdateTreeView();
    void UpdateTableView();

    void saveFile(const QString & filename);

    int GenerateId();
    void updateTableSignals();
};
#endif // MAINWINDOW_H

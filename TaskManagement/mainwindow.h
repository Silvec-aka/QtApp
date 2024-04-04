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
    void update_listView();

    void on_actionAjouter_triggered();

private:
    Ui::MainWindow *ui;
    QList<Tache*> *taches;
    int id_ = 0;

    QString nameString;
    QString durationString;
    QString dependanceString;
    bool writeToJson(const QString & filename);
    bool loadFromJson(const QString & filename);
    Tache findTache(int id) const;
    void AddTask(const QString nom, int duree, const QString dependances);
    const TacheComposite* findTacheComposite(int id) const;
    void CreateTree();

    void saveFile(const QString & filename);

    int GenerateId();
};
#endif // MAINWINDOW_H

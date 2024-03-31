#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tache.h"
#include "tachecomposite.h"

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

private:
    Ui::MainWindow *ui;
    QList<Tache*> *taches;

    bool writeToJson(const QString & filename);
    bool loadFromJson(const QString & filename);
    Tache findTache(int id) const;
    const TacheComposite* findTacheComposite(int id) const;

    void saveFile(const QString & filename);
    void saveModified();
};
#endif // MAINWINDOW_H

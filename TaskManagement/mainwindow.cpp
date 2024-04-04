#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tacheterminale.h"
#include "addtask.h"

#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QFileInfo>
#include <QFileDialog>
#include <QStandardItemModel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveFile(const QString & filename)
{
    bool success = writeToJson(filename);

    if (success)
    {
        QMessageBox::information(this, tr("Sauvegader"), tr("Le fichier ") + filename + tr("a bien été enregistré"));
    }
    else
    {
        QMessageBox::warning(this, tr("Sauvegader"), tr("Le fichier ") + filename + tr("n'a pas pû être enregistré"));
    }
}

void MainWindow::on_actionSauvegarder_triggered()
{
    QString filename;
    filename = QFileDialog::getSaveFileName(this, tr("Sauvegarder ?"), QDir::currentPath(), "Text Files (*.json)");

    if (!filename.isEmpty())
    {
        QFileInfo fi(filename);
        int button = QMessageBox::question(this, tr("Sauvegarder ?"), tr("Sauvegader le fichier ") + fi.fileName());
        if (button == QMessageBox::Yes)
        {
            saveFile(filename);
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Nom vide"), tr("Impossible de sauvegarder un fichier avec un nom vide"));
    }
}


bool MainWindow::writeToJson(const QString & filename)
{
    QJsonArray taskArray;

    //QtPrivate::QForeachContainer<QList<Tache*>*>
    for (const Tache& t : *taches)
    {
        taskArray.append(t.toJson());
    }

    QFile file(filename);

    if (file.open(QIODevice::WriteOnly))
    {
        QJsonDocument doc(taskArray);
        file.write(doc.toJson());
        file.close();
    }
}

bool MainWindow::loadFromJson(const QString & filename)
{
    // Création des dictionnaires pour les dépendances des tâches
    QMap<int, QList<int>> *suivantesMap = new QMap<int, QList<int>>();
    QMap<int, QList<int>> *precedentesMap = new QMap<int, QList<int>>();
    QMap<int, QList<int>> *composantsMap = new QMap<int, QList<int>>();

    QList<Tache> tasks;
    QFile file(filename);

    /*
     * On commence par créer toutes les tâches présentes dans le JSON
     */
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray jsonData = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonArray taskArray = doc.array();

        for (const QJsonValue& taskValue : taskArray)
        {
            QJsonObject taskObject = taskValue.toObject();
            int id = taskObject["id"].toInt();
            double num = taskObject["num"].toDouble();
            QString nom = taskObject["nom"].toString();
            int duree = taskObject["duree"].toInt();
            double completion = taskObject["completion"].toDouble();

            // Gestion des listes
            QJsonArray suivantesArray = taskObject["suivantes"].toArray();
            QJsonArray precedentesArray = taskObject["precedentes"].toArray();

            QList<int> suivantesId;
            QList<int> precedentesId;
            for (const QJsonValue& suivanteValue : suivantesArray)
            {
                suivantesId.append(suivanteValue.toInt());
            }
            suivantesMap->insert(id, suivantesId);

            for (const QJsonValue& precedenteValue : precedentesArray)
            {
                precedentesId.append(precedenteValue.toInt());
            }
            precedentesMap->insert(id, precedentesId);

            Tache* t;
            // Gestion du cas où l'on à une classe composite
            if (taskObject.contains("composants"))
            {
                QJsonArray composantsArray = taskObject["composants"].toArray();

                QList<int> composantsId;
                for (const QJsonValue& composantValue : composantsArray)
                {
                    suivantesId.append(composantValue.toInt());
                }
                composantsMap->insert(id, composantsId);

                TacheComposite t(id, num, nom, duree, completion);
            }
            else
            {
                TacheTerminale t(id, num, nom, duree, completion);
            }

            // On ajoute la atche à notre liste de tâche
            taches->append(t);
        }
    }

    /*
     * Une fois toutes les tâches créées, on peut ajouter les dépendances (suivantes, precedentes et composantes)
     * à l'aide des dictionnaires créés
     */
    QList<int> keys;
    QList<int> values;

    // Suivantes
    keys = suivantesMap->keys();
    for (int i=0; i < keys.count(); i++)
    {
        // On détermine la tâche à modifier
        Tache t = findTache(keys[i]);
        values = suivantesMap->value(keys[i]);

        for (int j=0; i < values.count(); j++)
        {
            // On lui ajoute sa suivante
            t.ajouterSuivante(findTache(values[j]));
        }
    }

    // Precedentes
    keys = precedentesMap->keys();
    for (int i=0; i < keys.count(); i++)
    {
        // On détermine la tâche à modifier
        Tache t = findTache(keys[i]);
        values = precedentesMap->value(keys[i]);

        for (int j=0; i < values.count(); j++)
        {
            // On lui ajoute sa suivante
            t.ajouterPrecedente(findTache(values[j]));
        }
    }

    // Composants
    keys = composantsMap->keys();
    for (int i=0; i < keys.count(); i++)
    {
        // On détermine la tâche à modifier
        TacheComposite t = *findTacheComposite(keys[i]);
        values = composantsMap->value(keys[i]);

        for (int j=0; i < values.count(); j++)
        {
            // On lui ajoute sa suivante
            t.ajouterComposant(findTache(values[j]));
        }
    }

}

Tache MainWindow::findTache(int id) const
{
    for (const Tache& t : *taches)
    {
        if (t.getId() == id) return t;
    }
}

const TacheComposite* MainWindow::findTacheComposite(int id) const
{
    for (const Tache* t : *taches)
    {
        if (const TacheComposite* tc = dynamic_cast<const TacheComposite*>(t))
        {
            if (t->getId() == id) return tc;
        }
    }
}

void MainWindow::update_listView()
{
    // ui->listView->reset();

    // // Create a new QStandardItemModel for the QListView
    // QStandardItemModel* model = new QStandardItemModel(ui->listView);

    // // On itère sur les tâches
    // for (const Tache& tache : *taches)
    // {
    //     // Creation d'un QStandardItem avec le numéro de la tâche
    //     QStandardItem* item = new QStandardItem(tache.getNum());

    //     item->setData("Task " + QString::number(tache.getNum()), Qt::DisplayRole);

    //     // Ajout de l'item dans le model
    //     model->appendRow(item);
    // }

    // model->sort(0, Qt::AscendingOrder); // On tri la liste
    // ui->listView->setModel(model);
}

int MainWindow::GenerateId()
{
    return id_ + 1;
}

void MainWindow::AddTask(const QString nom, int duree, const QString dependances)
{
    // TODO

    Tache t(GenerateId(), 0.0, nom, duree, 0.0);

    // Parser la QString des dépendaces et déterminer si les tâches existent.
    // Si oui, les ajouter aux tâches précèdentes de t et ajouter t dans les suivantes
    // des tâches précèdentes

    // Regarder si c'est une tâche composite ou terminale
}


void MainWindow::UpdateTreeView()
{
    QList<QStandardItem*> row;
    for (const Tache& tache : *taches)
    {
        row = tache.addToTree();
        // TODO créer le tree quelque part (voir avec RSQD si pb)
        // tree->appendRow(row);
    }
}

void MainWindow::UpdateListView()
{
    QList<QStandardItem*> row;
    for (const Tache& tache : *taches)
    {
        row = tache.addToTree();
        // TODO créer le tree quelque part (voir avec RSQD si pb)
        // tree->appendRow(row);
    }
}

void MainWindow::on_actionAjouter_triggered()
{
    addTask* dialogBox = new addTask();

    dialogBox->exec();

    nameString = dialogBox->getNameString();
    durationInt = dialogBox->getDurationInt();
    dependanceString = dialogBox->getDependanceString();

    AddTask(nameString, 0, dependanceString);
    UpdateTreeView();
    UpdateListView();

}


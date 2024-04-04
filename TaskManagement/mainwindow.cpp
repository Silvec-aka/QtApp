#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tacheterminale.h"
#include "addtasks.h"

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
    taches = new QList<Tache*>();
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
    // for (const Tache& t : *taches)
    // {
    //     taskArray.append(t.toJson());
    // }
    Q_FOREACH(const Tache& t , *taches)
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

    return true;
}

bool MainWindow::loadFromJson(const QString & filename)
{
    // Création des dictionnaires pour les dépendances des tâches
    QMap<int, QList<int>> *suivantesMap = new QMap<int, QList<int>>();
    QMap<int, QList<int>> *precedentesMap = new QMap<int, QList<int>>();
    QMap<int, QList<int>> *composantsMap = new QMap<int, QList<int>>();

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
            if (id > id_) id_ = id; // We set the id to start at the higher id in the JSON
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

                TacheComposite* t = new TacheComposite(id, num, nom, duree, completion);
                // On ajoute la atche à notre liste de tâche
                taches->append(t);
            }
            else
            {
                TacheTerminale* t = new TacheTerminale(id, num, nom, duree, completion);
                // On ajoute la atche à notre liste de tâche
                taches->append(t);
            }
        }
    }

    qDebug() << "1";

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
        Tache* t = findTache(keys[i]);
        values = suivantesMap->value(keys[i]);

        for (int j=0; i < values.count(); j++)
        {
            // On lui ajoute sa suivante
            t->ajouterSuivante(*findTache(values[j]));
        }
    }

    // Precedentes
    keys = precedentesMap->keys();
    for (int i=0; i < keys.count(); i++)
    {
        // On détermine la tâche à modifier
        Tache* t = findTache(keys[i]);
        values = precedentesMap->value(keys[i]);

        for (int j=0; i < values.count(); j++)
        {
            // On lui ajoute sa suivante
            t->ajouterPrecedente(*findTache(values[j]));
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
            t.ajouterComposant(*findTache(values[j]));
        }
    }

    return true;
}

Tache* MainWindow::findTache(int id) const
{
    for (Tache* t : *taches)
    {
        if (t->getId() == id) return t;
    }
}

TacheComposite* MainWindow::findTacheComposite(int id) const
{
    for (Tache* t : *taches)
    {
        if (TacheComposite* tc = dynamic_cast<TacheComposite*>(t))
        {
            if (t->getId() == id) return tc;
        }
    }
}


int MainWindow::GenerateId()
{
    return id_ += 1;
}

void MainWindow::AddTask(const QString nom, int duree, const QString dependances)
{
    // TODO

    Tache* t = new Tache(GenerateId(), 0.0, nom, duree, 0.0);
    qDebug() << "AddTask 1" << t->getNom();
    taches->append(t);
    for (const Tache &t : *taches)
    {
        qDebug() << "AddTask 2" << t.getNom();
    }

    qDebug() << "AddTask 3";

    // Parser la QString des dépendaces et déterminer si les tâches existent.
    // Si oui, les ajouter aux tâches précèdentes de t et ajouter t dans les suivantes
    // des tâches précèdentes

    // Regarder si c'est une tâche composite ou terminale
}


void MainWindow::UpdateTreeView()
{
    // TODO CLASSER LES TÂCHES
    // TODO SET LE NOM DES COLS

    // Création du modèle
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(1); // 1 colonne : le nom de la tâche

    qDebug() << "Update TreeView 1";

    // Création des lignes du modèle : une ligne pour chaque tâche
    QList<QStandardItem*> row;
    // for (const Tache& tache : *taches)
    // {
    //     row = tache.addToTree();
    //     model->appendRow(row);
    // }
    Q_FOREACH(const Tache &t , *taches)
    {
        row = t.addToTree();
        model->appendRow(row);
    }
    qDebug() << "Update TreeView 2";

    ui->treeView->setModel(model);
    ui->treeView->expandAll();
    ui->treeView->show();
}

void MainWindow::UpdateTableView()
{
    // Création du modèle
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(2); // 2 colonnes : num, nom

    qDebug() << "Update TableView 1";

    // Création des lignes du modèle : une ligne pour chaque tâche
    QList<QStandardItem*> row;

    int debug = 0;
    for (const Tache& tache : *taches)
    {
        qDebug() << "debug TableView " << debug;
        row = tache.addToList();
        model->appendRow(row);
        row.clear();
        debug++;
    }
    qDebug() << "Update TableView 2";

    ui->tableView->setModel(model);
    ui->tableView->show();
}

void MainWindow::on_actionAjouter_triggered()
{
    addTasks* dialogBox = new addTasks();

    dialogBox->exec();

    nameString = dialogBox->getNameString();
    qDebug() << nameString;
    durationInt = dialogBox->getDurationInt();
    dependanceString = dialogBox->getDependanceString();

    AddTask(nameString, durationInt, dependanceString);
    UpdateTreeView();
    UpdateTableView();

    qDebug() << "Update Ajouter";
}


void MainWindow::on_actionOuvrir_triggered()
{

    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath(), "Text Files (*.json)");
    if(!filename.isEmpty())
    {
        bool b = loadFromJson(filename);
    }
    else
    {
        QMessageBox::warning(this, tr("Empty file name"), tr("Can't load empty file name"));
    }

    UpdateTreeView();
    UpdateTableView();
}


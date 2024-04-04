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
            QString num = taskObject["num"].toString();
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
    return NULL;
}

Tache* MainWindow::findTacheByName(QString name) const
{
    for (Tache* t : *taches)
    {
        if (t->getNom() == name) return t;
    }
    return NULL;
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
    return NULL;
}


int MainWindow::GenerateId()
{
    return id_ += 1;
}

void MainWindow::AddTaskTerminal(const QString nom, int duree, const QString dependance, bool isPrincipale)
{
    Tache* t = new TacheTerminale(GenerateId(), "0", nom, duree, 0.0);
    if (dependance != "")
    {
        Tache* tacheDependance = findTacheByName(dependance);
        QString numDep = tacheDependance->getNum();

        if (isPrincipale)
        {
            int newNum = numDep.toInt() +1 ;
            t->setNum(QString::number(newNum));

            t->ajouterPrecedente(tacheDependance);
            tacheDependance->ajouterSuivante(*t);
        }
        else
        {
            TacheComposite* tacheDepComp = new TacheComposite(tacheDependance);

            t->ajouterPrecedente(tacheDepComp);
            tacheDepComp->ajouterComposant(*t);

            t->setNum(numDep + "." + QString::number(tacheDepComp->getComposante().length()) );
        }

        // t->setNum();
    }
    else
    {
        t->setNum("1");
    }
    
    taches->append(t);
}

void MainWindow::AddTaskComposite(const QString nom, int duree, const QString dependance, bool isPrincipale)
{
    Tache* t = new TacheComposite(GenerateId(), "0", nom, duree, 0.0);
    if (dependance != "")
    {
        Tache* tacheDependance = findTacheByName(dependance);
        QString numDep = tacheDependance->getNum();

        if (isPrincipale)
        {
            int newNum = numDep.toInt() +1 ;
            t->setNum(QString::number(newNum));

            t->ajouterPrecedente(tacheDependance);
            tacheDependance->ajouterSuivante(*t);
        }
        else
        {
            TacheComposite* tacheDepComp = new TacheComposite(tacheDependance);

            t->ajouterPrecedente(tacheDepComp);
            tacheDepComp->ajouterComposant(*t);

            t->setNum(numDep + "." + QString::number(tacheDepComp->getComposante().length()) );
        }
    }
    else
    {
        t->setNum("1");
    }

    taches->append(t);
}


void MainWindow::UpdateTreeView()
{
    // TODO CLASSER LES TÂCHES

    // Création du modèle
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(1); // 1 colonne : le nom de la tâche

    // Création des lignes du modèle : une ligne pour chaque tâche
    QList<QStandardItem*> row;

    Q_FOREACH(const Tache &t , *taches)
    {
        row = t.addToTree();
        model->appendRow(row);
    }

    ui->treeView->setModel(model);
    ui->treeView->expandAll();
    ui->treeView->show();
}

void MainWindow::UpdateTableView()
{
    // Création du modèle
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(2); // 2 colonnes : num, nom

    // Modification des labels des colonnes
    QStringList columnLabels;
    columnLabels << "Num" << "Nom";
    model->setHorizontalHeaderLabels(columnLabels);


    // Création des lignes du modèle : une ligne pour chaque tâche
    QList<QStandardItem*> row;

    for (const Tache& tache : *taches)
    {
        row = tache.addToList();
        model->appendRow(row);
        row.clear();
    }

    ui->tableView->setModel(model);
    ui->tableView->show();

    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableViewElementSelected(const QModelIndex &)));
}

void MainWindow::on_actionAjouter_triggered()
{
    addTasks* dialogBox = new addTasks();
    QList<QString> l;
    Q_FOREACH(const Tache &t , *taches)
    {
        if (t.isComposite || (!(t.getNum().contains(".")) && t.getSuivantes().length()==0))
        {
            qDebug() << t.isComposite;
            l.append(t.getNom());
        }
    }

    dialogBox->fillAllTaks(l);
    dialogBox->exec();

    nameString = dialogBox->getNameString();
    durationInt = dialogBox->getDurationInt();
    dependanceString = dialogBox->getDependanceString();
    isTerminal = dialogBox->getIsTerminal();
    isPrincipale = dialogBox->getIsPrincipale();

    if (isTerminal)
    {
        AddTaskTerminal(nameString, durationInt, dependanceString, isPrincipale);
    }
    else
    {
        AddTaskComposite(nameString, durationInt, dependanceString, isPrincipale);
    }

    UpdateTreeView();
    UpdateTableView();
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

void MainWindow::onTableViewElementSelected(const QModelIndex &index)
{
    if (index.isValid())
    {
        QVariant data = ui->tableView->model()->data(index);

        Tache* t = findTacheByName(data.toString());
        if (t==NULL) return;

        ui->DescriptionLabel->setText("<center><b>Tache n°" + t->getNum() + "</b></center>");
        ui->nameString->setText(t->getNom());
        ui->time->setValue(t->getDuree());
        ui->completion->setValue(t->getCompletion());
        //QString type;
        //if (t->getTerminale()) type = "tâche terminale";
        //else type = "tâche composite";
        //ui->taskType->setText("Type : " + type)

        // TODO TYPE DE TACHE
        if (!t->getPrecedentes().empty())
        {
            ui->precedenteName->setText("Précédente : " + t->getPrecedentes()[0].getNom());
        }
        else
        {
            ui->precedenteName->setText("Précédente : / ");
        }

        if (!t->getPrecedentes().empty())
        {
            ui->suivanteName->setText("Suivante : " + t->getPrecedentes()[0].getNom());
        }
        else
        {
            ui->precedenteName->setText("Suivante : / ");
        }

        descriptionTask_ = t;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString newName = ui->nameString->text();
    int newDuree = ui->time->value();
    double newCompletion = ui->completion->value();

    descriptionTask_->SetNom(newName);
    descriptionTask_->setDuree(newDuree);
    descriptionTask_->setCompletion(newCompletion);

    UpdateTableView();
    UpdateTableView();
}

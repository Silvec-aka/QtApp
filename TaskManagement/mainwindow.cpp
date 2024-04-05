/**
 * @file mainwindow.cpp
*/

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



/**

 * @brief Constructeur de la classe MainWindow
 *
 * @param parent Le widget parent de la fenêtre principale
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    taches = new QList<Tache*>();

    idsComposite = new QList<int>();
}

/**
 * @brief Destructeur de la classe MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * @brief Méthode on_actionSauvegarder_triggered qui gère le signal de sauvegarde
 *
 * -> slot
 */
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


/**
 * @brief Méthode on_actionAjouter_triggered qui gère le signal d'ajout de tâche
 *
 * -> slot
 */
void MainWindow::on_actionAjouter_triggered()
{
    addTasks* dialogBox = new addTasks();
    QList<QString> l;
    Q_FOREACH(const Tache &t , *taches)
    {
        if (idsComposite->contains(t.getId()) || (!(t.getNum().contains(".")) && t.getSuivantes().length()==0))
        {
            l.append(t.getNom());
        }
    }

    dialogBox->fillAllTaks(l);
    dialogBox->exec();

    if (!dialogBox->getAccepted()) return;
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


/**
 * @brief Méthode on_actionOuvrir_triggered qui gère le signal d'ouverture d'un fichier .json à charger
 *
 * -> slot
 */
void MainWindow::on_actionOuvrir_triggered()
{

    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath(), "Text Files (*.json)");
    if(!loadFromJson(filename))
    {
        QMessageBox::warning(this, tr("Erreur"), tr("Impossible de charger le fichier"));
    }

    UpdateTreeView();
    UpdateTableView();
}


/**
 * @brief Méthode onTableViewElementSelected qui gère le signal de sélection d'une tâche dans le TableView
 *
 * -> slot
 */
void MainWindow::onTableViewElementSelected(const QModelIndex &index)
{
    if (index.isValid())
    {
        QVariant data = ui->tableView->model()->data(index);

        Tache* t = findTacheByName(data.toString());
        if (t==NULL) return;

        ui->DescriptionLabel->setText("<center><b>" + tr("Tache n°") + t->getNum() + "</b></center>");
        ui->nameString->setText(t->getNom());
        ui->time->setValue(t->getDuree());
        ui->completion->setValue(t->getCompletion());

        QString type = tr("tâche terminale");
        if (idsComposite->contains(t->getId())) type = tr("tâche composite");

        ui->taskType->setText(tr("Type : ") + type);


        if (!t->getPrecedentes().empty())
        {
            ui->precedenteName->setText(tr("Précédente : ") + t->getPrecedentes()[0].getNom());
        }
        else
        {
            ui->precedenteName->setText(tr("Précédente : / "));
        }

        if (!t->getSuivantes().empty())
        {
            ui->suivanteName->setText(tr("Suivante : ") + t->getSuivantes()[0].getNom());
        }
        else
        {
            ui->suivanteName->setText(tr("Suivante : / "));
        }

        descriptionTask_ = t;
    }
}


/**
 * @brief Méthode on_pushButton_clicked qui gère le signal de modification d'une tâche (boutton Appliquer séléctionné)
 *
 * -> slot
 */
void MainWindow::on_pushButton_clicked()
{
    QString newName = ui->nameString->text();
    int newDuree = ui->time->value();
    double newCompletion = ui->completion->value();

    descriptionTask_->SetNom(newName);
    descriptionTask_->setDuree(newDuree);
    descriptionTask_->setCompletion(newCompletion);

    UpdateTreeView();
    UpdateTableView();
}


/**
 * @brief Méthode on_actionSupprimer_triggered qui gère le signal de suppression d'une tâche
 *
 * -> slot
 */
void MainWindow::on_actionSupprimer_triggered()
{
    // delete descriptionTask_;

    // UpdateTableView();
    // UpdateTableView();
}


/**
 * @brief Méthode writeToJson qui écrit les tâches dans un fichier JSON
 *
 * @param filename Le nom du fichier JSON à écrire
 *
 * @return True si l'écriture a réussi, false sinon
 */
bool MainWindow::writeToJson(const QString & filename)
{
    QJsonArray taskArray;

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
        return true;
    }

    return false;
}

/**
 * @brief Méthode loadFromJson qui charge les tâches depuis un fichier JSON
 *
 * @param filename Le nom du fichier JSON à charger
 *
 * @return True si le chargement a réussi, false sinon
 */
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
            if (taskObject.contains("suivantes"))
            {
                QJsonArray suivantesArray = taskObject["suivantes"].toArray();

                QList<int> suivantesId;
                for (const QJsonValue& suivanteValue : suivantesArray)
                {
                    suivantesId.append(suivanteValue.toInt());
                }
                suivantesMap->insert(id, suivantesId);
            }

            if (taskObject.contains("precedentes"))
            {
                QJsonArray precedentesArray = taskObject["precedentes"].toArray();

                QList<int> precedentesId;
                for (const QJsonValue& precedenteValue : precedentesArray)
                {
                    precedentesId.append(precedenteValue.toInt());
                }
                precedentesMap->insert(id, precedentesId);
            }

            // Gestion du cas où l'on à une classe composite
            if (taskObject.contains("composants"))
            {
                QJsonArray composantsArray = taskObject["composants"].toArray();

                QList<int> composantsId;
                for (const QJsonValue& composantValue : composantsArray)
                {
                    composantsId.append(composantValue.toInt());
                }
                composantsMap->insert(id, composantsId);

                TacheComposite* t = new TacheComposite(id, num, nom, duree, completion);
                // On ajoute la tâche à notre liste de tâche
                taches->append(t);
            }
            else
            {
                TacheTerminale* t = new TacheTerminale(id, num, nom, duree, completion);
                // On ajoute la tâche à notre liste de tâche
                taches->append(t);
            }
        }
    }
    else
    {
        return false;
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
        Tache* t = findTache(keys[i]);
        values = suivantesMap->value(keys[i]);

        for (int j=0; j < values.count(); j++)
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

        for (int j=0; j < values.count(); j++)
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

        for (int j=0; j < values.count(); j++)
        {
            // On lui ajoute sa suivante
            t.ajouterComposant(*findTache(values[j]));
        }
    }

    return true;
}


/**
 * @brief Méthode saveFile qui enregistre les tâches du projet sous le format .json
 *
 * @param filename Le nom du fichier à enregistrer
 */
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


/**
 * @brief Recherche une tâche par son identifiant
 *
 * @param id L'identifiant de la tâche à rechercher
 * @return Un pointeur vers la tâche trouvée, ou NULL si aucune tâche correspondante n'est trouvée
 */
Tache* MainWindow::findTache(int id) const
{
    for (Tache* t : *taches)
    {
        if (t->getId() == id) return t;
    }
    return NULL;
}


/**
 * @brief Recherche une tâche par son nom
 *
 * @param name Le nom de la tâche à rechercher
 * @return Un pointeur vers la tâche trouvée, ou NULL si aucune tâche correspondante n'est trouvée
 */
Tache* MainWindow::findTacheByName(QString name) const
{
    for (Tache* t : *taches)
    {
        if (t->getNom() == name) return t;
    }
    return NULL;
}


/**
 * @brief Recherche une tâche composite par son identifiant
 *
 * @param id L'identifiant de la tâche composite à rechercher
 * @return Un pointeur vers la tâche composite trouvée, ou NULL si aucune tâche composite correspondante n'est trouvée
 */
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


/**
 * @brief Ajoute une tâche terminale
 *
 * @param nom Le nom de la tâche
 * @param duree La durée de la tâche
 * @param dependance La dépendance de la tâche
 * @param isPrincipale Booléen indiquant si la tâche est terminale
 */
void MainWindow::AddTaskTerminal(const QString nom, int duree, const QString dependance, bool isPrincipale)
{
    Tache* t = new TacheTerminale(GenerateId(), "0", nom, duree, 0.0);
    if (dependance != "")
    {
        Tache* tacheDependance = findTacheByName(dependance);
        QString numDep = tacheDependance->getNum();

        if (!idsComposite->contains(tacheDependance->getId()) || (isPrincipale && tacheDependance->getSuivantes().length() == 0))
        {
            int newNum = numDep.toInt() +1 ;
            t->setNum(QString::number(newNum));

            t->ajouterPrecedente(tacheDependance);
            tacheDependance->ajouterSuivante(*t);
        }
        else
        {
            TacheComposite* tacheDepComp = findTacheComposite(tacheDependance->getId());

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


/**
 * @brief Ajoute une tâche composite
 *
 * @param nom Le nom de la tâche
 * @param duree La durée de la tâche
 * @param dependance La dépendance de la tâche
 * @param isPrincipale Booléen indiquant si la tâche appartient à la branche principale
 */
void MainWindow::AddTaskComposite(const QString nom, int duree, const QString dependance, bool isPrincipale)
{
    Tache* t = new TacheComposite(GenerateId(), "0", nom, duree, 0.0);
    idsComposite->append(t->getId());

    if (dependance != "")
    {
        Tache* tacheDependance = findTacheByName(dependance);
        QString numDep = tacheDependance->getNum();

        if ( !idsComposite->contains(tacheDependance->getId()) || (isPrincipale && tacheDependance->getSuivantes().length() == 0))
        {
            int newNum = numDep.toInt() +1 ;
            t->setNum(QString::number(newNum));

            t->ajouterPrecedente(tacheDependance);
            tacheDependance->ajouterSuivante(*t);
        }
        else
        {
            TacheComposite* tacheDepComp = findTacheComposite(tacheDependance->getId());

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


/**
 * @brief Met à jour la vue arborescente des tâches
 */
void MainWindow::UpdateTreeView()
{
    // Création du modèle
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(1); // 1 colonne : le nom de la tâche

    // Création des lignes du modèle : une ligne pour chaque tâche
    QStandardItem* row;

    Q_FOREACH(const Tache &t , *taches)
    {
        if (!t.getNum().contains("."))
        {
            row = t.addToTree();
            model->appendRow(row);

            if (idsComposite->contains(t.getId()))
            {
                TacheComposite* tacheDepComp = findTacheComposite(t.getId());
                QStandardItem* rowChild;
                for (int i=0; i < tacheDepComp->getComposante().length(); i++)
                {
                    rowChild = tacheDepComp->getComposante()[i].addToTree();
                    row->appendRow(rowChild);
                }
            }
        }
    }

    ui->treeView->setModel(model);
    ui->treeView->expandAll();
    ui->treeView->show();
}



/**
 * @brief Met à jour la vue en tableau des tâches
 */
void MainWindow::UpdateTableView()
{
    // Création du modèle
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(2); // 2 colonnes : num, nom

    // Modification des labels des colonnes
    QStringList columnLabels;
    columnLabels << tr("Num") << tr("Nom");
    model->setHorizontalHeaderLabels(columnLabels);


    // Création des lignes du modèle : une ligne pour chaque tâche
    QList<QStandardItem*> row;

    for (const Tache& tache : *taches)
    {
        row = tache.addToTable();
        model->appendRow(row);
        row.clear();
    }

    ui->tableView->setModel(model);
    ui->tableView->show();

    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableViewElementSelected(const QModelIndex &)));
}


/**
 * @brief Génère un nouvel identifiant unique pour une tâche
 *
 * @return Le nouvel identifiant généré
 */
int MainWindow::GenerateId()
{
    return id_ += 1;
}

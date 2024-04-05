/**
 * @file addtasks.cpp
*/

#include "addtasks.h"
#include "ui_addtasks.h"



/**
 * @brief Constructeur de la classe addTasks
 *
 * @param parent Le widget parent de la boîte de dialogue
 */
addTasks::addTasks(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addTasks)
{
    ui->setupUi(this);
}

/**
 * @brief Destructeur de la classe addTasks.
 */
addTasks::~addTasks()
{
    delete ui;
}


/**
* @brief Getter de l'attribut nameString
*
* @return Le nom de la tâche sous forme de QString
*/
QString addTasks::getNameString()
{
    return nameString;
}

/**
 * @brief Getter de l'attribut durationInt
 *
 * @return La durée de la tâche sous forme d'entier
 */
int addTasks::getDurationInt()
{
    return durationInt;
}


/**
 * @brief Getter de l'attribut dependanceString
 *
 * @return La dépendance de la tâche sous forme de QString
 */
QString addTasks::getDependanceString()
{
    return dependanceString;
}


/**
 * @brief Getter du booléen isTerminal
 *
 * @return True si la tâche est terminale, false sinon
 */
bool addTasks::getIsTerminal() const
{
    return isTerminal;
}

/**
 * @brief Getter du booléen isPrincipale
 *
 * @return True si la tâche est principale, false sinon
 */
bool addTasks::getIsPrincipale() const
{
    return isPrincipale;
}

/**
 * @brief Getter du booléen isAccepted
 *
 * @return True si la boîte de dialogue a été acceptée (OK), false sinon
 */
bool addTasks::getAccepted() const
{
    return accepted;
}


/**
 * @brief Méthode fillAllTaks qui remplit la boîte combo dependanceBox avec la liste de tâches L
 *
 * @param L La liste de tâches à ajouter à la boîte combo dependanceBox
 */
void addTasks::fillAllTaks(QList<QString> L)
{
    Q_FOREACH(const QString s , L)
    {
        ui->dependanceBox->addItem(s);
    }
}


/**
 * @brief Méthode on_buttonBox_accepted qui gère le signal accepté de la boîte de boutons
 *
 * Définit les variables nameString, durationInt, dependanceString, isTerminal et isPrincipale en fonction de l'entrée utilisateur
 * Définit la variable accepted à true et accepte la boîte de dialogue
 * -> slot
 */
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


/**
 * @brief Méthode on_buttonBox_rejected qui gère le signal rejeté (annuler) de la boîte de boutons
 *
 * Définit la variable accepted à false et rejette la boîte de dialogue
 * -> slot
 */
void addTasks::on_buttonBox_rejected()
{
    accepted = false;
    reject();
}

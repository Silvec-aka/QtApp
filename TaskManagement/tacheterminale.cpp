/**
 * @file tacheterminale.cpp
*/

#include "tacheterminale.h"
#include <QJsonArray>



/**
 * @brief Constructeur valué et par défaut de la classe TacheTerminale
 *
 * @param id L'identifiant de la tâche
 * @param num Le numéro de la tâche
 * @param nom Le nom de la tâche
 * @param duree La durée de la tâche
 * @param completion Le taux de complétion de la tâche
 * @param suivantes La liste des tâches suivantes
 * @param precedentes La liste des tâches précédentes
 *
 * Utilise le constructeur de la classe mère
 */
TacheTerminale::TacheTerminale(int id, QString num, QString nom, int duree, double completion, QList<Tache> suivantes, QList<Tache> precedentes) :
    Tache(id, num, nom, duree, completion, suivantes, precedentes)
{
}


/**
 * @brief Constructeur de copie de la classe TacheTerminale
 *
 * @param t Un pointeur vers l'objet Tache à partir duquel créer la tâche terminale
 */
TacheTerminale::TacheTerminale(Tache *t)
{
    id_ = t->getId();
    nom_ = t->getNom();
    num_ = t->getNum();
    duree_ = t->getDuree();
    completion_ = t->getDuree();
    suivantes_ = t->getSuivantes();
    precedentes_ = t->getPrecedentes();
}


/**
 * @brief Getter du taux de completion de la tâche : Obtient la durée effective de la tâche terminale
 *
 * @return La durée effective de la tâche terminale
 */
int TacheTerminale::getDuree() const
{
    return duree_;
}


/**
 * @brief Getter du taux de completion de la tâche : Obtient le taux d'achèvement de la tâche terminale
 *
 * @return Le taux de complétion de la tâche terminale
 */
double TacheTerminale::getCompletion() const
{
    // On initialise la complétion comme ayant la valeur de la tâche actuelle
    double completion = 0.0;
    double nbTask = 1;

    // On y ajoute la complétions des tâches précèdentes
    for (int i = 0; i < precedentes_.count(); i++)
    {
        completion += precedentes_[i].getCompletion();
        nbTask ++;
    }

    return completion / nbTask;
}


/**
 * @brief Setter de la durée de la tâche (en jours) : Définit la durée de la tâche
 *
 * @param duree La nouvelle durée de la tâche
 */
void TacheTerminale::setDuree(int duree)
{
    duree_ = duree;
}


/**
 * @brief Setter de la completion de la tâche : Définit le taux d'achèvement de la tâche
 *
 * @param completion Le nouveau taux de complétion de la tâche
 */
void TacheTerminale::setCompletion(double completion)
{
    completion_ = completion;
}

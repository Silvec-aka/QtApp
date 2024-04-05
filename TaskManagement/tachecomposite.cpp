/**
 * @file tachecomposite.cpp
*/

#include "tachecomposite.h"



/**
 * @brief Constructeur valué et par défaut de la classe TacheComposite
 *
 * @param id L'identifiant de la tâche
 * @param num Le numéro de la tâche
 * @param nom Le nom de la tâche
 * @param duree La durée de la tâche
 * @param completion Le taux de complétion de la tâche
 * @param suivantes La liste des tâches suivantes
 * @param precedentes La liste des tâches précédentes
 * @param composants La liste des composants de la tâche composite
 *
 * Utilise le constructeur de la classe mère
 *
 */
TacheComposite::TacheComposite(int id, QString num, QString nom, int duree, double completion, QList<Tache> suivantes, QList<Tache> precedentes, QList<Tache> composants) :
    Tache(id, num, nom, duree, completion, suivantes, precedentes)
{
    composants_ = composants;
}


/**
 * @brief Constructeur de copie de la classe TacheComposite
 *
 * @param t Un pointeur vers l'objet Tache à partir duquel créer la tâche composite
 * @param composants La liste des composants de la tâche composite
 */
TacheComposite::TacheComposite(Tache *t, QList<Tache> composants)
{
    id_ = t->getId();
    nom_ = t->getNom();
    num_ = t->getNum();
    duree_ = t->getDuree();
    completion_ = t->getDuree();
    suivantes_ = t->getSuivantes();
    precedentes_ = t->getPrecedentes();
    composants_ = composants;
}


/**
 * @brief Getter du taux de completion de la tâche : Obtient la durée effective de la tâche composite en tenant compte de ses composants
 *
 * @return La durée effective de la tâche composite
 */
int TacheComposite::getDuree() const
{
    // On initialise la durée à la durée de cette tâche
    int duree = duree_;

    // On y ajoute la durée de toutes les tâches précèdentes non complétées
    for (int i = 0; i < composants_.count(); i++)
    {
        if (composants_[i].getCompletion() != 1.0f) duree += composants_[i].getDuree();
    }

    return duree;
}


/**
 * @brief Getter du taux de completion de la tâche : Obtient le taux d'achèvement de la tâche composite en tenant compte de ses composants
 *
 * @return Le taux de complétion de la tâche composite
 */
double TacheComposite::getCompletion() const
{
    // On initialise la complétion comme ayant la valeur de la tâche actuelle
    double completion = completion_;
    double nbTask = 1;

    // On y ajoute la complétions des tâches précèdentes
    for (int i = 0; i < composants_.count(); i++)
    {
        completion += composants_[i].getCompletion();
        nbTask ++;
    }

    return completion / nbTask;
}


/**
 * @brief Convertit l'objet TacheComposite en objet pouvant être écrit dans un fichier .json
 *
 * @return L'objet TacheComposite au format .json
 */
QJsonObject TacheComposite::toJson() const
{
    QJsonObject tObj;
    tObj["id"] = id_;
    tObj["num"] = num_;
    tObj["nom"] = nom_;
    tObj["duree"] = duree_;
    tObj["completion"] = completion_;

    if (suivantes_.size() > 0)
    {
        QJsonArray suivantesArray;
        for (const Tache& suivante : suivantes_)
        {
            suivantesArray.append(suivante.getId());
        }
        tObj["suivantes"] = suivantesArray;
    }

    if (precedentes_.size() > 0)
    {
        QJsonArray precedentesArray;

        for (const Tache& precedente : precedentes_)
        {
            precedentesArray.append(precedente.getId());
        }
        tObj["precedentes"] = precedentesArray;
    }

    if (composants_.size() > 0)
    {
        QJsonArray composantsArray;

        for (const Tache& composant : composants_)
        {
            composantsArray.append(composant.getId());
        }
        tObj["composants"] = composantsArray;
    }

    return tObj;
}


/**
 * @brief Setter de la durée de la tâche (en jours) : Définit la durée de la tâche
 *
 * @param duree La nouvelle durée de la tâche
 */
void TacheComposite::setDuree(int duree)
{
    duree_ = duree;
}


/**
 * @brief Setter de la completion de la tâche : Définit le taux d'achèvement de la tâche
 *
 * @param completion Le nouveau taux de complétion de la tâche
 */
void TacheComposite::setCompletion(double completion)
{
    completion_ = completion;
}


/**
 * @brief Ajoute une tâche composante à la tâche
 *
 * @param t La tâche à ajouter aux tâches composantes
 */
void TacheComposite::ajouterComposant(const Tache t)
{
    composants_.append(t);
}


/**
 * @brief Retire une tâche composante à la liste des tâches composantes
 *
 * @param id L'identifiant de la tâche à retirer
 * @return La tâche retirée, ou NULL si aucune tâche correspondante n'est trouvée
 */
Tache TacheComposite::supprimerComposant(int id)
{
    for (int i = 0; i < composants_.count(); i++)
    {
        if (composants_[i].getId() == id)
        {
            Tache t = composants_[i];
            composants_.removeAt(i);
            return t;
        }
    }
    return NULL;
}


/**
 * @brief Obtient la liste des composants de la tâche composite
 *
 * @return La liste des composants de la tâche composite
 */
QList<Tache> TacheComposite::getComposante() const
{
    return composants_;
}

/**
 * @file tache.cpp
*/

#include "tache.h"
#include <QMap>



/**
 * @brief Constructeur valué et par défaut de la classe Tache
 *
 * @param id L'identifiant de la tâche
 * @param num Le numéro de la tâche
 * @param nom Le nom de la tâche
 * @param duree La durée de la tâche
 * @param completion Le taux de complétion de la tâche
 * @param suivantes La liste des tâches suivantes
 * @param precedentes La liste des tâches précédentes
 */
Tache::Tache(int id, QString num, QString nom, int duree, double completion, QList<Tache> suivantes, QList<Tache> precedentes)
{
    id_ = id;
    num_ = num;
    nom_ = nom;
    duree_ = duree;
    completion_ = completion;
    suivantes_ = suivantes;
    precedentes_ = precedentes;
}


/**
 * @brief Constructeur de copie de la classe Tache
 *
 * @param tache Un pointeur vers l'objet Tache à copier
 */
Tache::Tache(Tache *tache)
{
    id_ = tache->id_;
    nom_ = tache->nom_;
    num_ = tache->num_;
    duree_ = tache->duree_;
    completion_ = tache->completion_;
    suivantes_ = tache->suivantes_;
    precedentes_ = tache->precedentes_;
}


/**
 * @brief Getter de la durée de la tâche : Obtient la durée effective de la tâche en tenant compte de ses précédentes non complétées
 *
 * @return La durée effective de la tâche
 */
int Tache::getDuree() const
{
    // On initialise la durée à la durée de cette tâche
    int duree = duree_;

    // On y ajoute la durée de toutes les tâches précèdentes non complétées
    for (int i = 0; i < precedentes_.count(); i++)
    {
        if (precedentes_[i].getCompletion() != 1.0f) duree += precedentes_[i].getDuree();
    }

    return duree;
}


/**
 * @brief Getter du pourcentage de completion de la tâche : Obtient le taux d'achèvement de la tâche en tenant compte de ses précédentes
 *
 * @return Le taux d'achèvement de la tâche
 */
double Tache::getCompletion() const
{
    // On initialise la complétion comme ayant la valeur de la tâche actuelle
    double completion = completion_;
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
 * @brief Convertit l'objet Tache en objet pouvant être ajouter dans un fichier .json
 *
 * @return L'objet Tache adapté pour le format .json
 */
QJsonObject Tache::toJson() const
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

    return tObj;
}


/**
 * @brief Getter de l'identifiant (unique) de la tâche
 *
 * @return L'id de la tache
 */
int Tache::getId() const
{
    return id_;
}


/**
 * @brief Getter du numéro de la tâche
 *
 * @return Le numéro de la tâche
 */
QString Tache::getNum() const
{
    return num_;
}


/**
 * @brief Setter du numéro de la tâche : Définit le numéro de la tâche
 *
 * @param num Le nouveau numéro de la tâche
 */
void Tache::setNum(QString num)
{
    num_ = num;
}


/**
 * @brief Getter du nom de la tâche : Obtient le nom de la tâche
 *
 * @return Le nom de la tâche
 */
QString Tache::getNom() const
{
    return nom_;
}


/**
 * @brief Setter du nom de la tâche : Définit le nom de la tâche
 *
 * @param nom Le nouveau nom de la tâche
 */
void Tache::SetNom(QString nom)
{
    nom_ = nom;
}


/**
 * @brief Setter de la durée de la tâche (en jours) : Définit la durée de la tâche
 *
 * @param duree La nouvelle durée de la tâche
 */
void Tache::setDuree(int duree)
{
    duree_ = duree;
}


/**
 * @brief Setter de la completion de la tâche : Définit le taux d'achèvement de la tâche
 *
 * @param completion Le nouveau taux de complétion de la tâche
 */
void Tache::setCompletion(double completion)
{
    completion_ = completion;
}


/**
 * @brief Getter de la liste des tâches suivantes : Obtient la liste des tâches suivantes
 *
 * @return La liste des tâches suivantes
 */
QList<Tache> Tache::getSuivantes() const
{
    return suivantes_;
}


/**
 * @brief Getter de la liste des tâches précédentes : Obtient la liste des tâches précédentes
 *
 * @return La liste des tâches précédentes
 */
QList<Tache> Tache::getPrecedentes() const
{
    return precedentes_;
}


/**
 * @brief Ajoute une tâche suivante à la tâche
 *
 * @param tache La tâche à ajouter aux tâches suivantes
 */
void Tache::ajouterSuivante(const Tache& tache)
{
    suivantes_.append(tache);
}


/**
 * @brief Ajoute une tâche précédente à la tâche
 *
 * @param tache La tâche à ajouter aux tâches précédentes
 */
void Tache::ajouterPrecedente(const Tache& tache)
{
    precedentes_.append(tache);
}


/**
 * @brief Retire une tâche suivante à la liste des tâches suivantes
 *
 * @param id L'identifiant de la tâche à retirer
 * @return La tâche retirée, ou NULL si aucune tâche correspondante n'est trouvée
 */
Tache Tache::retirerSuivante(int id)
{
    for (int i = 0; i < suivantes_.count(); i++)
    {
        if (suivantes_[i].getId() == id)
        {
            Tache t = suivantes_[i];
            suivantes_.removeAt(i);
            return t;
        }
    }
    return NULL;
}


/**
 * @brief Retire une tâche précédente à la liste des tâches précédentes
 *
 * @param id L'identifiant de la tâche à retirer
 * @return La tâche retirée, ou NULL si aucune tâche correspondante n'est trouvée
 */
Tache Tache::retirerPrecedente(int id)
{
    for (int i = 0; i < precedentes_.count(); i++)
    {
        if (precedentes_[i].getId() == id)
        {
            Tache t = precedentes_[i];
            precedentes_.removeAt(i);
            return t;
        }
    }
    return NULL;
}


/**
 * @brief Ajoute la tâche à un modèle d'arbre pour l'affichage
 *
 * @return L'élément correspondant à la tâche dans l'arbre
 */
QStandardItem* Tache::addToTree() const
{    
    QStandardItem* nom = new QStandardItem(nom_);
    nom->setFlags(Qt::ItemIsEnabled);

    return nom;
}


/**
 * @brief Ajoute la tâche à une liste pour l'affichage en tableau
 *
 * @return La liste d'éléments correspondant à la tâche dans le tableau
 */
QList<QStandardItem*> Tache::addToTable() const
{
    QStandardItem* nom = new QStandardItem(nom_);
    nom->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QStandardItem* num = new QStandardItem(num_);
    num->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QList<QStandardItem*> childCol;
    childCol << num << nom;

    return childCol;
}

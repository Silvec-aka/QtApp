#include "tachecomposite.h"

TacheComposite::TacheComposite(int id, double num, QString nom, int duree, double completion, QList<Tache> suivantes, QList<Tache> precedentes, QList<Tache> composants) :
    Tache(id, num, nom, duree, completion, suivantes, precedentes)
{
    // if (suivantes_.isEmpty()) throw std::exception("Cette tâche est terminale et non composite.");

    composants_ = composants;
}

TacheComposite::TacheComposite(Tache *t, QList<Tache> composants)
{
    // TODO : pb car get Duree renvoie la durée totale et pas juste celle de la tâche

    id_ = t->getId();
    nom_ = t->getNom();
    duree_ = t->getDuree();
    completion_ = t->getDuree();
    suivantes_ = t->getSuivantes();
    precedentes_ = t->getPrecedentes();
    composants_ = composants;
}


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

void TacheComposite::setNum()
{
    // TODO


    //variable ext
    double nb = 0.0;

    double com = 0.1;

    for (int i=0; i < composants_.count(); i++)
    {
        composants_[i].setNum(nb+com);
        com += 0.1;
    }

    nb += 1.0;
}



void TacheComposite::setDuree(int duree)
{
    duree_ = duree;
}

void TacheComposite::setCompletion(double completion)
{
    completion_ = completion;
}


void TacheComposite::ajouterComposant(const Tache &t)
{
    composants_.append(t);
}

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

    // throw std::exception("Il n'y a pas de tâche correspondante");
}

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

#include "tache.h"

Tache::Tache(QString nom, QList<Tache> suivantes, QList<Tache> precedentes)
{
    // TODO id
    nom_ = nom;
    suivantes_ = suivantes;
    precedentes_ = precedentes;
}


Tache::Tache(Tache *tache)
{
    id_ = tache->id_;
    nom_ = tache->nom_;
    suivantes_ = tache->suivantes_;
    precedentes_ = tache->precedentes_;
}


int Tache::getId()
{
    return id_;
}


QString Tache::getNom()
{
    return nom_;
}


void Tache::SetNom(QString nom)
{
    nom_ = nom;
}


QList<Tache> Tache::getSuivantes()
{
    return suivantes_;
}


QList<Tache> Tache::getPrecedentes()
{
    return precedentes_;
}


void Tache::ajouterSuivante(Tache tache)
{
    suivantes_.append(tache);
}


void Tache::ajouterPrecedente(Tache tache)
{
    precedentes_.append(tache);
}


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

    // TODO Throw exception
}


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

    // TODO Throw exception
}

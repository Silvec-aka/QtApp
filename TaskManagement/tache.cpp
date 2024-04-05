#include "tache.h"
#include <QMap>

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


int Tache::getId() const
{
    return id_;
}


QString Tache::getNum() const
{
    return num_;
}

void Tache::setNum(QString num)
{
    num_ = num;
}


QString Tache::getNom() const
{
    return nom_;
}


void Tache::SetNom(QString nom)
{
    nom_ = nom;
}

void Tache::setDuree(int duree)
{
    duree_ = duree;
}

void Tache::setCompletion(double completion)
{
    completion_ = completion;
}

QList<Tache> Tache::getSuivantes() const
{
    return suivantes_;
}


QList<Tache> Tache::getPrecedentes() const
{
    return precedentes_;
}


void Tache::ajouterSuivante(const Tache& tache)
{
    suivantes_.append(tache);
}


void Tache::ajouterPrecedente(const Tache& tache)
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
    return NULL;
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
    return NULL;
}


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

QStandardItem* Tache::addToTree() const
{    
    QStandardItem* nom = new QStandardItem(nom_);
    nom->setFlags(Qt::ItemIsEnabled);

    return nom;
}


QList<QStandardItem*> Tache::addToList() const
{
    QStandardItem* nom = new QStandardItem(nom_);
    nom->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QStandardItem* num = new QStandardItem(num_);
    num->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QList<QStandardItem*> childCol;
    childCol << num << nom;

    return childCol;
}

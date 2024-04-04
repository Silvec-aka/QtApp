#include "tache.h"
#include <QMap>

Tache::Tache(int id, double num, QString nom, int duree, double completion, QList<Tache> suivantes, QList<Tache> precedentes)
{
    id_ = id;
    num_ = num;
    nom_ = nom;
    duree_ = duree;
    completion_ = completion;
    suivantes_ = suivantes;
    precedentes_ = precedentes;

    setNum();
}


Tache::Tache(Tache *tache)
{
    id_ = tache->id_;
    nom_ = tache->nom_;
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


void Tache::setNum()
{
    // TODO

    // if(precedentes_.isEmpty()) num_ = 1.0;
    // else if (precedentes_.count() > 1) num_ = precedentes_[0].getNum() + 0.1; // TODO
    // else
    // {
    //     num_ = precedentes_[0].getNum() + 1.0;
    // }
}


void Tache::setNum(double num)
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

    // throw std::exception("Il n'existe pas de tâche suivante correspondante.");
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

    // throw std::exception("Il n'existe pas de tâche suivante correspondante.");
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
    /*QJsonObject json;

    json["id"] = id_;
    json["num"] = num_;
    json["nom"] = nom_;
    json["duree"] = duree_;
    json["completion"] = completion_;

    QJsonArray suivantes;
    QJsonArray precedentes;
    for (int i=0; i<suivantes_.count(); i++)
    {
        suivantes.append((suivantes_[i]).id_);
    }
    json["suivantes"] = suivantes;
    for (int i=0; i<precedentes_.count(); i++)
    {
        suivantes.append((precedentes_[i]).id_);
    }
    json["precedentes"] = precedentes;

    return json;*/
}

QList<QStandardItem*> Tache::addToTree() const
{
    QStandardItem* nom = new QStandardItem(nom_);
    nom->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QStandardItem* duree = new QStandardItem(duree_);
    duree->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QStandardItem* num = new QStandardItem(num_);
    num->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QList<QStandardItem*> childCol;
    childCol << num << nom << duree;

    return childCol;
}

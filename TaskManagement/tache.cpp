#include "tache.h"
#include <QJsonObject>
#include <QJsonArray>
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


int Tache::getDuree()
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


double Tache::getCompletion()
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


int Tache::getId()
{
    return id_;
}


void Tache::setNum()
{
    // TODO

    if(precedentes_.isEmpty()) num_ = 1.0;
    else if (precedentes_.count() > 1) num_ = precedentes_[0].getNum() + 0.1; // TODO
    else
    {
        num_ = precedentes_[0].getNum() + 1.0;
    }
}


void Tache::setNum(double num)
{
    num_ = num;
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

    throw std::exception("Il n'existe pas de tâche suivante correspondante.");
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

    throw std::exception("Il n'existe pas de tâche suivante correspondante.");
}

QJsonObject Tache::toJson() const
{
    QJsonObject json;

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

    return json;
}


Tache Tache::fromJson(const QJsonObject & obj, QMap<int, QList<int>> &mapSuivantes, QMap<int, QList<int>> &mapPrecedentes)
{
    // On vérifie que la tâche est terminale
    if (obj.contains("composants"))
    {
        throw std::exception("Chargement d'une tâche non terminale");
    }

    const int id = obj["id"].toInt();

    //On détermine les tâches suivantes et précèdentes de nos listes que l'on stocke dans des dictionnaires
    QList<int> suivantes = QList<int>();
    for(const QJsonValue &s : obj["suivantes"].toArray())
    {
        suivantes.append(s.toInt());
    }
    mapSuivantes.insert(id, suivantes);

    QList<int> precedentes = QList<int>();
    for(const QJsonValue &s : obj["precedentes"].toArray())
    {
        precedentes.append(s.toInt());
    }
    mapPrecedentes.insert(id, precedentes);


    // On affecte toute les valeurs à notre tâche créée
    Tache t(
        obj["id"].toInt(),
        obj["num"].toDouble(),
        obj["nom"].toString(),
        obj["duree"].toDouble(),
        obj["completion"].toDouble()
        );

    return t;
}

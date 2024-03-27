#include "tachecomposite.h"

TacheComposite::TacheComposite(QString nom, int duree, double completion, QList<Tache> suivantes, QList<Tache> precedentes, QList<Tache> composants) :
    Tache(nom, duree, completion, suivantes, precedentes)
{
    if (suivantes_.isEmpty()) throw std::exception("Cette tâche est terminale et non composite.");

    composants_ = composants;
}


int TacheComposite::getDuree()
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


double TacheComposite::getCompletion()
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


void TacheComposite::ajouterComposant(Tache t)
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

    throw std::exception("Il n'y a pas de tâche correspondante");
}

QJsonObject Tache::toJson() const
{
    QJsonObject json;

    // On n'enregistre pas l'id est le num car ils seront déterminé par le code une fois la tâche créée
    json["nom"] = nom_;
    json["duree"] = duree_;
    json["completion"] = completion_;

    QJsonArray suivantes;
    QJsonArray precedentes;
    for (int i=0; i<suivantes_.count(); i++)
    {
        suivantes.append((suivantes_[i]).toJson());
    }
    json["suivantes"] = suivantes;
    for (int i=0; i<precedentes_.count(); i++)
    {
        suivantes.append((precedentes_[i]).toJson());
    }
    json["precedentes"] = precedentes;

    return json;
}


TacheComposite TacheComposite::fromJson(const QJsonObject & obj, QMap<int, QList<int>> mapSuivantes, QMap<int, QList<int>> mapPrecedentes, QMap<int, QList<int>> mapComposites)
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

#include "tacheterminale.h"
#include <QJsonArray>


TacheTerminale::TacheTerminale(int id, double num, QString nom, int duree, double completion, QList<Tache> suivantes, QList<Tache> precedentes) :
    Tache(id, num, nom, duree, completion, suivantes, precedentes)
{
    // if (!suivantes_.isEmpty()) throw std::exception("Cette tâche n'est pas terminale.");
}

TacheTerminale::TacheTerminale(Tache *t)
{
    // TODO : pb car get Duree renvoie la durée totale et pas juste celle de la tâche

    id_ = t->getId();
    nom_ = t->getNom();
    duree_ = t->getDuree();
    completion_ = t->getDuree();
    suivantes_ = t->getSuivantes();
    precedentes_ = t->getPrecedentes();
}



int TacheTerminale::getDuree() const
{
    return duree_;
}


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

void TacheTerminale::setNum()
{
    //variable ext
    double nb = 0.0;

    num_ = nb;
    nb += 1.0;
}



void TacheTerminale::setDuree(int duree)
{
    duree_ = duree;
}

void TacheTerminale::setCompletion(double completion)
{
    completion_ = completion;
}

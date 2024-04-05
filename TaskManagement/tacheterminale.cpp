#include "tacheterminale.h"
#include <QJsonArray>


TacheTerminale::TacheTerminale(int id, QString num, QString nom, int duree, double completion, QList<Tache> suivantes, QList<Tache> precedentes) :
    Tache(id, num, nom, duree, completion, suivantes, precedentes)
{
}

TacheTerminale::TacheTerminale(Tache *t)
{
    // TODO : pb car get Duree renvoie la durée totale et pas juste celle de la tâche

    id_ = t->getId();
    nom_ = t->getNom();
    num_ = t->getNum();
    duree_ = t->getDuree();
    completion_ = t->getDuree();
    suivantes_ = t->getSuivantes();
    precedentes_ = t->getPrecedentes();
}

// TacheTerminale::~TacheTerminale()
// {
//     qDeleteAll(suivantes_.begin(), suivantes_.end());
//     qDeleteAll(precedentes_);
// }


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


void TacheTerminale::setDuree(int duree)
{
    duree_ = duree;
}

void TacheTerminale::setCompletion(double completion)
{
    completion_ = completion;
}

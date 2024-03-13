#include "tacheterminale.h"

TacheTerminale::TacheTerminale(QString nom, int duree, double completion, QList<Tache> suivantes, QList<Tache> precedentes, QList<Tache> composants) :
    Tache(nom, duree, completion, suivantes, precedentes)
{
    composants_ = composants;
}



int TacheTerminale::getDuree()
{
    return duree_;
}

int TacheTerminale::getNum()
{
    // TODO
    return 0;
}

double TacheTerminale::getCompletion()
{
    // TODO
    return 0.0;
}



void TacheTerminale::ajouterComposant(Tache t)
{
    composants_.append(t);
}

Tache TacheTerminale::supprimerComposant(int id)
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

    // TODO Throw exception
}

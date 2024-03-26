#include "tacheterminale.h"

TacheTerminale::TacheTerminale(QString nom, int duree, double completion, QList<Tache> suivantes, QList<Tache> precedentes, QList<Tache> composants) :
    Tache(nom, duree, completion, suivantes, precedentes)
{
    if (!suivantes_.isEmpty()) throw std::exception("Cette tâche n'est pas terminale.");

    composants_ = composants;
}



int TacheTerminale::getDuree()
{
    return duree_;
}

double TacheTerminale::getNum()
{
    // TODO
    return 0;
}

double TacheTerminale::getCompletion()
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

    throw std::exception("Il n'y a pas de tâche correspondante");
}

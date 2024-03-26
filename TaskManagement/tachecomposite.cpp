#include "tachecomposite.h"
#include "qforeach.h"

TacheComposite::TacheComposite(QString nom, int duree, double completion, QList<Tache> suivantes, QList<Tache> precedentes) :
    Tache(nom, duree, completion, suivantes, precedentes)
{

}



int TacheComposite::getDuree()
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

double TacheComposite::getNum()
{
    // TODO
    return 0;
}

double TacheComposite::getCompletion()
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



void TacheComposite::setDuree(int duree)
{
    duree_ = duree;
}

void TacheComposite::setCompletion(double completion)
{
    completion_ = completion;
}

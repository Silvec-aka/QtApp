#include "tachecomposite.h"

TacheComposite::TacheComposite(QString nom, int duree, double completion, QList<Tache> suivantes, QList<Tache> precedentes) :
    Tache(nom, duree, completion, suivantes, precedentes)
{

}



int TacheComposite::getDuree()
{
    // TODO
    return duree_;
}

int TacheComposite::getNum()
{
    // TODO
    return 0;
}

double TacheComposite::getCompletion()
{
    // TODO
    return completion_;
}



void TacheComposite::setDuree(int duree)
{
    duree_ = duree;
}

void TacheComposite::setCompletion(double completion)
{
    completion_ = completion;
}

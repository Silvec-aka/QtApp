#ifndef TACHECOMPOSITE_H
#define TACHECOMPOSITE_H

#include "tache.h"

class TacheComposite : public Tache
{

    public:
        TacheComposite(QString nom = "nouvelle tache", int duree = 0, double completion = 0.0, QList<Tache> suivantes = QList<Tache>(), QList<Tache> precedentes = QList<Tache>());

        int getDuree();
        int getNum();
        double getCompletion();

        void setDuree(int duree);
        void setCompletion(double completion);
};

#endif // TACHECOMPOSITE_H

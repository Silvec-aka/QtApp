#ifndef TACHETERMINALE_H
#define TACHETERMINALE_H

#include "tache.h"

class TacheTerminale : public Tache
{
    private:
        QList<Tache> composants_;

    public:
        TacheTerminale(QString nom = "nouvelle tache", int duree = 0, double completion = 0.0, QList<Tache> suivantes = QList<Tache>(), QList<Tache> precedentes = QList<Tache>(), QList<Tache> composants = QList<Tache>());

        int getDuree();
        double getNum();
        double getCompletion();

        void ajouterComposant(Tache t);
        Tache supprimerComposant(int id);
};

#endif // TACHETERMINALE_H

#ifndef TACHETERMINALE_H
#define TACHETERMINALE_H

#include "tache.h"
#include <QJsonObject>

class TacheTerminale : public Tache
{
    public:
        TacheTerminale(int id, double num, QString nom = "nouvelle tache", int duree = 0, double completion = 0.0, QList<Tache> suivantes = QList<Tache>(), QList<Tache> precedentes = QList<Tache>());
        TacheTerminale(Tache* t);

        int getDuree() const override;
        double getCompletion() const override;

        void setDuree(int duree);
        void setCompletion(double completion);
};

#endif // TACHETERMINALE_H

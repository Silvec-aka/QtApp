#ifndef TACHETERMINALE_H
#define TACHETERMINALE_H

#include "tache.h"
#include <QJsonObject>

class TacheTerminale : public Tache
{
    public:
        TacheTerminale(QString nom = "nouvelle tache", int duree = 0, double completion = 0.0, QList<Tache> suivantes = QList<Tache>(), QList<Tache> precedentes = QList<Tache>());

        int getDuree() override;
        double getCompletion() override;
        void setNum() override;

        void setDuree(int duree);
        void setCompletion(double completion);
};

#endif // TACHETERMINALE_H

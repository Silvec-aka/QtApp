#ifndef TACHECOMPOSITE_H
#define TACHECOMPOSITE_H

#include "tache.h"

class TacheComposite : public Tache
{
    private:
    QList<Tache> composants_;

    public:
        TacheComposite(int id = 0, QString num = "0", QString nom = "nouvelle tache", int duree = 0, double completion = 0.0, QList<Tache> suivantes = QList<Tache>(), QList<Tache> precedentes = QList<Tache>(), QList<Tache> composants = QList<Tache>());
        TacheComposite(Tache* t, QList<Tache> composants = QList<Tache>());

        int getDuree() const override;
        double getCompletion() const override;

        void setDuree(int duree);
        void setCompletion(double completion);

        void ajouterComposant(const Tache t);
        Tache supprimerComposant(int id);

        QList<Tache> getComposante() const;

        QJsonObject toJson() const override;
};

#endif // TACHECOMPOSITE_H

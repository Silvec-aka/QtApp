#ifndef TACHECOMPOSITE_H
#define TACHECOMPOSITE_H

#include "tache.h"

class TacheComposite : public Tache
{
    private:
    QList<Tache> composants_;

    public:
        TacheComposite(int id, double num, QString nom = "nouvelle tache", int duree = 0, double completion = 0.0, QList<Tache> suivantes = QList<Tache>(), QList<Tache> precedentes = QList<Tache>(), QList<Tache> composants = QList<Tache>());

        int getDuree() const override;
        double getCompletion() const override;
        void setNum() override;

        void setDuree(int duree);
        void setCompletion(double completion);

        void ajouterComposant(const Tache &t);
        Tache supprimerComposant(int id);

        QJsonObject toJson() const override;
};

#endif // TACHECOMPOSITE_H

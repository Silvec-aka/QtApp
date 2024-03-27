#ifndef TACHECOMPOSITE_H
#define TACHECOMPOSITE_H

#include "tache.h"

class TacheComposite : public Tache
{
    private:
    QList<Tache> composants_;

    public:
        TacheComposite(QString nom = "nouvelle tache", int duree = 0, double completion = 0.0, QList<Tache> suivantes = QList<Tache>(), QList<Tache> precedentes = QList<Tache>(), QList<Tache> composants = QList<Tache>());

        int getDuree() override;
        double getCompletion() override;
        void setNum() override;

        void setDuree(int duree);
        void setCompletion(double completion);

        void ajouterComposant(Tache t);
        Tache supprimerComposant(int id);

        QJsonObject toJson() const override;
        static TacheComposite fromJson(const QJsonObject & obj, QMap<int, QList<int>> &mapSuivantes, QMap<int, QList<int>> &mapPrecedentes, QMap<int, QList<int>> &mapComposites);
};

#endif // TACHECOMPOSITE_H

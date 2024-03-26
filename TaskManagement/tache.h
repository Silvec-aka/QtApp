#ifndef TACHE_H
#define TACHE_H

#include <QObject>

class Tache
{

    protected:
        int id_;
        double num_;
        QString nom_;
        int duree_;
        double completion_;
        QList<Tache> suivantes_;
        QList<Tache> precedentes_;


    public:
        explicit Tache(QString nom = "nouvelle tache", int duree = 0, double completion = 0.0, QList<Tache> suivantes = QList<Tache>(), QList<Tache> precedentes = QList<Tache>());
        Tache(Tache *tache);
        virtual int getDuree();
        virtual double getCompletion();
        virtual void setNum();

        int getId();

        double getNum();
        void setNum(double num);

        QString getNom();
        void SetNom(QString nom) ;

        QList<Tache> getSuivantes();
        QList<Tache> getPrecedentes();

        void ajouterSuivante(Tache tache);
        void ajouterPrecedente(Tache tache);

        Tache retirerSuivante(int id);
        Tache retirerPrecedente(int id);

    signals:
};

#endif // TACHE_H

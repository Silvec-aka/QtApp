#ifndef TACHE_H
#define TACHE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItem>

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
        explicit Tache(int id = 0, double num = 0.0, QString nom = "nouvelle tache", int duree = 0, double completion = 0.0, QList<Tache> suivantes = QList<Tache>(), QList<Tache> precedentes = QList<Tache>());
        Tache(Tache *tache);
        virtual int getDuree() const;
        virtual double getCompletion() const;
        virtual void setNum();

        int getId() const;

        double getNum() const;
        void setNum(double num);

        QString getNom() const;
        void SetNom(QString nom) ;

        QList<Tache> getSuivantes() const;
        QList<Tache> getPrecedentes() const;

        void ajouterSuivante(const Tache& tache);
        void ajouterPrecedente(const Tache& tache);

        Tache retirerSuivante(int id);
        Tache retirerPrecedente(int id);

        virtual QJsonObject toJson() const;
        QList<QStandardItem*> addToTree() const;
        QList<QStandardItem*> addToList() const;


    signals:
};

#endif // TACHE_H

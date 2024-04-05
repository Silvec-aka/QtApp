#ifndef TACHETERMINALE_H
#define TACHETERMINALE_H

#include "tache.h"
#include <QJsonObject>

class TacheTerminale : public Tache
{
    public:
        /**
         * @brief Constructeur valué et par défaut de la classe TacheTerminale
         *
         * @param id L'identifiant de la tâche
         * @param num Le numéro de la tâche
         * @param nom Le nom de la tâche
         * @param duree La durée de la tâche
         * @param completion Le taux de complétion de la tâche
         * @param suivantes La liste des tâches suivantes
         * @param precedentes La liste des tâches précédentes
         */
        TacheTerminale(int id, QString num, QString nom = "nouvelle tache", int duree = 0, double completion = 0.0, QList<Tache> suivantes = QList<Tache>(), QList<Tache> precedentes = QList<Tache>());
        /**
         * @brief Constructeur de copie de la classe TacheTerminale
         *
         * @param t Un pointeur vers l'objet Tache à partir duquel créer la tâche terminale
         */
        TacheTerminale(Tache* t);


        /**
         * @brief Getter du taux de completion de la tâche : Obtient la durée effective de la tâche terminale
         *
         * @return La durée effective de la tâche terminale
         */
        int getDuree() const override;
        /**
         * @brief Getter du taux de completion de la tâche : Obtient le taux d'achèvement de la tâche terminale
         *
         * @return Le taux de complétion de la tâche terminale
         */
        double getCompletion() const override;


        /**
         * @brief Setter de la durée de la tâche (en jours) : Définit la durée de la tâche
         *
         * @param duree La nouvelle durée de la tâche
         */
        void setDuree(int duree);
        /**
         * @brief Setter de la completion de la tâche : Définit le taux d'achèvement de la tâche
         *
         * @param completion Le nouveau taux de complétion de la tâche
         */
        void setCompletion(double completion);
};

#endif // TACHETERMINALE_H

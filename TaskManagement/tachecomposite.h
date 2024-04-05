#ifndef TACHECOMPOSITE_H
#define TACHECOMPOSITE_H

#include "tache.h"

class TacheComposite : public Tache
{
    private:
    QList<Tache> composants_;


    public:
        /**
         * @brief Constructeur valué et par défaut de la classe TacheComposite
         *
         * @param id L'identifiant de la tâche
         * @param num Le numéro de la tâche
         * @param nom Le nom de la tâche
         * @param duree La durée de la tâche
         * @param completion Le taux de complétion de la tâche
         * @param suivantes La liste des tâches suivantes
         * @param precedentes La liste des tâches précédentes
         * @param composants La liste des composants de la tâche composite
         */
        TacheComposite(int id = 0, QString num = "0", QString nom = "nouvelle tache", int duree = 0, double completion = 0.0, QList<Tache> suivantes = QList<Tache>(), QList<Tache> precedentes = QList<Tache>(), QList<Tache> composants = QList<Tache>());
        /**
         * @brief Constructeur de copie de la classe TacheComposite
         *
         * @param t Un pointeur vers l'objet Tache à partir duquel créer la tâche composite
         * @param composants La liste des composants de la tâche composite
         */
        TacheComposite(Tache* t, QList<Tache> composants = QList<Tache>());


        /**
         * @brief Getter du la durée de la tâche : Obtient la durée effective de la tâche composite en tenant compte de ses composants
         *
         * @return La durée effective de la tâche composite
         */
        int getDuree() const override;
        /**
         * @brief Getter du taux de completion de la tâche : Obtient le taux d'achèvement de la tâche composite en tenant compte de ses composants
         *
         * @return Le taux de complétion de la tâche composite
         */
        double getCompletion() const override;
        /**
         * @brief Convertit l'objet TacheComposite en objet pouvant être écrit dans un fichier .json
         *
         * @return L'objet TacheComposite au format .json
         */
        QJsonObject toJson() const override;


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


        /**
         * @brief Ajoute une tâche composante à la tâche
         *
         * @param t La tâche à ajouter aux tâches composantes
         */
        void ajouterComposant(const Tache t);
        /**
         * @brief Retire une tâche composante à la liste des tâches composantes
         *
         * @param id L'identifiant de la tâche à retirer
         * @return La tâche retirée, ou NULL si aucune tâche correspondante n'est trouvée
         */
        Tache supprimerComposant(int id);


        /**
         * @brief Obtient la liste des composants de la tâche composite
         *
         * @return La liste des composants de la tâche composite
         */
        QList<Tache> getComposante() const;
};

#endif // TACHECOMPOSITE_H

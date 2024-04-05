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
        QString num_;
        QString nom_;
        int duree_;
        double completion_;
        QList<Tache> suivantes_;
        QList<Tache> precedentes_;


    public:
        /**
         * @brief Constructeur valué et par défaut de la classe Tache
         *
         * @param id L'identifiant de la tâche
         * @param num Le numéro de la tâche
         * @param nom Le nom de la tâche
         * @param duree La durée de la tâche
         * @param completion Le taux de complétion de la tâche
         * @param suivantes La liste des tâches suivantes
         * @param precedentes La liste des tâches précédentes
         */
        explicit Tache(int id = 0, QString num = "0", QString nom = "nouvelle tache", int duree = 0, double completion = 0.0, QList<Tache> suivantes = QList<Tache>(), QList<Tache> precedentes = QList<Tache>());
        /**
         * @brief Constructeur de copie de la classe Tache
         *
         * @param tache Un pointeur vers l'objet Tache à copier
         */
        Tache(Tache *tache);

        /**
         * @brief Getter de la durée de la tâche : Obtient la durée effective de la tâche en tenant compte de ses précédentes non complétées
         *
         * @return La durée effective de la tâche
         */
        virtual int getDuree() const;
        /**
         * @brief Getter du pourcentage de completion de la tâche : Obtient le taux d'achèvement de la tâche en tenant compte de ses précédentes
         *
         * @return Le taux d'achèvement de la tâche
         */
        virtual double getCompletion() const;
        /**
         * @brief Convertit l'objet Tache en objet pouvant être ajouter dans un fichier .json
         *
         * @return L'objet Tache adapté pour le format .json
         */
        virtual QJsonObject toJson() const;


        /**
         * @brief Getter de l'identifiant (unique) de la tâche
         *
         * @return L'id de la tache
         */
        int getId() const;


        /**
         * @brief Getter du numéro de la tâche
         *
         * @return Le numéro de la tâche
         */
        QString getNum() const;
        /**
         * @brief Setter du numéro de la tâche : Définit le numéro de la tâche
         *
         * @param num Le nouveau numéro de la tâche
         */
        void setNum(QString num);


        /**
         * @brief Getter du nom de la tâche : Obtient le nom de la tâche
         *
         * @return Le nom de la tâche
         */
        QString getNom() const;
        /**
         * @brief Setter du nom de la tâche : Définit le nom de la tâche
         *
         * @param nom Le nouveau nom de la tâche
         */
        void SetNom(QString nom) ;


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
         * @brief Getter de la liste des tâches suivantes : Obtient la liste des tâches suivantes
         *
         * @return La liste des tâches suivantes
         */
        QList<Tache> getSuivantes() const;
        /**
         * @brief Getter de la liste des tâches précédentes : Obtient la liste des tâches précédentes
         *
         * @return La liste des tâches précédentes
         */
        QList<Tache> getPrecedentes() const;


        /**
         * @brief Ajoute une tâche suivante à la tâche
         *
         * @param tache La tâche à ajouter aux tâches suivantes
         */
        void ajouterSuivante(const Tache& tache);
        /**
         * @brief Ajoute une tâche précédente à la tâche
         *
         * @param tache La tâche à ajouter aux tâches précédentes
         */
        void ajouterPrecedente(const Tache& tache);


        /**
         * @brief Retire une tâche suivante à la liste des tâches suivantes
         *
         * @param id L'identifiant de la tâche à retirer
         * @return La tâche retirée, ou NULL si aucune tâche correspondante n'est trouvée
         */
        Tache retirerSuivante(int id);
        /**
         * @brief Retire une tâche précédente à la liste des tâches précédentes
         *
         * @param id L'identifiant de la tâche à retirer
         * @return La tâche retirée, ou NULL si aucune tâche correspondante n'est trouvée
         */
        Tache retirerPrecedente(int id);


        /**
         * @brief Ajoute la tâche à un modèle d'arbre pour l'affichage
         *
         * @return L'élément correspondant à la tâche dans l'arbre
         */
        QStandardItem* addToTree() const;
        /**
         * @brief Ajoute la tâche à une liste pour l'affichage en tableau
         *
         * @return La liste d'éléments correspondant à la tâche dans le tableau
         */
        QList<QStandardItem*> addToTable() const;


    signals:
};

#endif // TACHE_H

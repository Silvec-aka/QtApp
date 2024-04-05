/**
 * @file addtasks.h
*/

#ifndef ADDTASKS_H
#define ADDTASKS_H

#include <QDialog>



namespace Ui {
class addTasks;
}


class addTasks : public QDialog
{
    Q_OBJECT

    public:
        /**
         * @brief Constructeur de la classe addTasks
         *
         * @param parent Le widget parent de la boîte de dialogue
         */
        explicit addTasks(QWidget *parent = nullptr);
        /**
         * @brief Destructeur de la classe addTasks.
         */
        ~addTasks();


        /**
        * @brief Getter de l'attribut nameString
        *
        * @return Le nom de la tâche sous forme de QString
        */
        QString getNameString();
        /**
         * @brief Getter de l'attribut durationInt
         *
         * @return La durée de la tâche sous forme d'entier
         */
        int getDurationInt();
        /**
         * @brief Getter de l'attribut dependanceString
         *
         * @return La dépendance de la tâche sous forme de QString
         */
        QString getDependanceString();
        /**
         * @brief Getter du booléen isTerminal
         *
         * @return True si la tâche est terminale, false sinon
         */
        bool getIsTerminal() const;
        /**
         * @brief Getter du booléen isPrincipale
         *
         * @return True si la tâche est principale, false sinon
         */
        bool getIsPrincipale() const;
        /**
         * @brief Getter du booléen isAccepted
         *
         * @return True si la boîte de dialogue a été acceptée (OK), false sinon
         */
        bool getAccepted() const;


        /**
         * @brief Méthode fillAllTaks qui remplit la boîte combo dependanceBox avec la liste de tâches L
         *
         * @param L La liste de tâches à ajouter à la boîte combo dependanceBox
         */
        void fillAllTaks(QList<QString>);


    private slots:
        /**
         * @brief Méthode on_buttonBox_accepted qui gère le signal accepté de la boîte de boutons
         *
         * Définit les variables nameString, durationInt, dependanceString, isTerminal et isPrincipale en fonction de l'entrée utilisateur
         * Définit la variable accepted à true et accepte la boîte de dialogue
         */
        void on_buttonBox_accepted();
        /**
         * @brief Méthode on_buttonBox_rejected qui gère le signal rejeté (annuler) de la boîte de boutons
         *
         * Définit la variable accepted à false et rejette la boîte de dialogue
         */
        void on_buttonBox_rejected();

    private:
        Ui::addTasks *ui;
        QString nameString;
        int durationInt;
        QString dependanceString;

        bool isTerminal;
        bool isPrincipale;

        bool accepted;

        QList<QString> allTasks;
};

#endif // ADDTASKS_H

/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/06/2016
 */
#ifndef SELECTINLISTBOX_H
#define SELECTINLISTBOX_H

#include <QPushButton>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QMap>
#include <QMessageBox>
#include <QPair>
#include <QVBoxLayout>
#include <QWidget>

/*! \ingroup groupeWidget
 * \brief Widget permettant de répartir des éléments entre deux listes, avec une possible mise en ordre.
 */
class SelectInListBox : public QWidget
{
    Q_OBJECT
protected:
    bool m_ordre;                       //! Permet ou non d'ordonner la liste.
    bool m_repetition;                  //! Permet ou non les répétitions.

    // Widget
    QPushButton *m_buttonDeplaceDroite;
    QPushButton *m_buttonDeplaceGauche;
    QPushButton *m_boutonDernierDroite;
    QPushButton *m_boutonDernierGauche;
    QPushButton *m_boutonPrecedentDroite;
    QPushButton *m_boutonPrecedentGauche;
    QPushButton *m_boutonPremierDroite;
    QPushButton *m_boutonPremierGauche;
    QPushButton *m_boutonSuivantDroite;
    QPushButton *m_boutonSuivantGauche;
    QLabel *m_labelTitreDroite;
    QLabel *m_labelTitreGauche;
    QListWidget *m_listeDroite;
    QListWidget *m_listeGauche;

    // Layout
    QVBoxLayout *m_layoutCentre;
    QVBoxLayout *m_layoutListeDroite;
    QVBoxLayout *m_layoutListeGauche;
    QHBoxLayout *m_layoutMain;
    QVBoxLayout *m_layoutOrdreDroite;
    QVBoxLayout *m_layoutOrdreGauche;

public:
    //!Constructeur
    explicit SelectInListBox(const QString & titreGauche = QString(), const QString & titreDroite = QString(), bool ordre = false, bool repetition = false, const QMap<int,QString> & choicesGauche = QMap<int,QString>(), const QMap<int,QString> & choicesDroite = QMap<int,QString>(), QWidget *parent = 0);

    //! Ajoute un élément dans la liste de droite
    void append(int n, const QString & txt, bool droite = true);

    //! Supprime les éléments des liste de valeur n et texte txt
    void remove(int n, const QString & txt);

    //! Retourne une paire contenant deux listes d'entiters, la liste des données (Qt::UserData) de la liste gauche et celles de la liste de droite.
    QPair<QVector<int>,QVector<int>> value() const;
signals:

public slots:
    //! Déplace les éléments selectionner dans liste de gauche vers la liste de droite
    void moveSelectedADroite();

    //! Déplace les éléments selectionner dans liste de droite vers la liste de gauche
    void moveSelectedAGauche();

protected:
    /*! \brief Ajoute item a liste.
     *
     * Ajoute item a liste, si
     * - m_ordre = false, l'élément est placer suivant l'ordre croissant dans liste.
     * - m_ordre = true et end = false, l'élément est placé juste après l'élement selectionné dans la liste s'il y en a un, à la fin sinon.
     * - m_ordre = true et end = true, l'élément et placé à la fin de la liste.
     */
    void append(QListWidget * list, QListWidgetItem * item, bool end = false);

    //! Place l'élément selectionner à la fin de list.
    void dernier(QListWidget * list);

    //! Place l'élément selectionner à la ligne précédente de list.
    void precedent(QListWidget * list);

    //! Place l'élément selectionner au début de list.
    void premier(QListWidget * list);

    //! Place l'élément selectionner à la ligne suivante de list.
    void suivant(QListWidget * list);
};

#endif // SELECTINLISTBOX_H

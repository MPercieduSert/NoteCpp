/*Auteur: PERCIE DU SERT Maxime
 *Date: 09/03/2017
 */
#ifndef SPINBOXDECIMALE_H
#define SPINBOXDECIMALE_H

#include <array>
#include <QAbstractSpinBox>
#include <QLineEdit>

#include "../Entities/AttributSimple.h"

/*! \ingroup groupeWidget
 * \brief Spin Box pour les décimales d'une épreuve.
 */
class SpinBoxDecimale : public QAbstractSpinBox
{
    Q_OBJECT
protected:
    int m_indice;       //! Indice de la valeur courante.
public:

    //! Constructeur.
    SpinBoxDecimale(int value = 1, QWidget * parent = nullptr);

    //! Renvoie l'indice de la valeur et -1 si elle n'est pas dans la liste.
    int find(int n) const;

    //! Retourne la valeur sélectionnée.
    int value() const         {return AttributDecimale::Decimale[m_indice];}

    //! Modifie la valeur (qui doit être dans la m_valeur).
    void setValue(int value);

    //! incrémente de steps fois la valeur.
    void stepBy(int steps);

protected:
    QAbstractSpinBox::StepEnabled stepEnabled() const;
public slots:
    //! Affiche la valeur.
    void printValue();
};

#endif // SPINBOXDECIMALE_H

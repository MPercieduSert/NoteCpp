/*Auteur: PERCIE DU SERT Maxime
 *Date: 28/03/2017
 */
#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H

#include "BarPlotter.h"

/*! \ingroup groupeWidget
 * \brief Classe mère des graphiques.
 */
class Histogramme : public BarPlotter
{
    Q_OBJECT
protected:
    QVector<QBrush> m_aspet;        //!< Aspet des barres.
    QVector<double> m_donnee;       //!< Série statistique continue brute.
    QVector<double> m_intervalle;   //!< Extrémintées des intervalles.

public:
    //! Constructeur.
    Histogramme(QWidget * parent = 0)
        : BarPlotter(parent) {}

    //! Constructeur.
    Histogramme(const QVector<double> & intervalle, const QVector<QBrush> & aspet, const QVector<double> donnee, QWidget * parent = 0);

    //! Accesseur des données.
    const QVector<double> & donnee() const
        {return m_donnee;}
    //! Accesseur des intervalles.
    const QVector<double> & intervalle() const
        {return m_intervalle;}
    //! Mutateurs des données.
    void setDonnee(const QVector<double> & donnee)
    {
        m_donnee = donnee;
        std::sort(m_donnee.begin(),m_donnee.end());
        calculBar();
    }
    //! Mutateurs des intervalles.
    void setIntervalle(const QVector<double> & intervalle)
        {m_intervalle = intervalle;}

protected:
    //! Détermine les barres en fonction des données et des intervalles
    void calculBar();
};

#endif // HISTOGRAMME_H

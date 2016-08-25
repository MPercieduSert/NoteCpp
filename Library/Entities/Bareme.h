/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/03/2016
 */

#ifndef BAREME_H
#define BAREME_H

#include "Entity.h"

/*! \ingroup groupeEntity
 * \brief Représentation de l'entité Bareme.
 */

class Bareme : public Entity
{
protected:
    int m_idEx;         //!< Clé: Clé vers la partie d'exercice sur laquelle porte le barème.
    int m_fraction;     //!< Attribut: nombre de fraction possible des points associés au barème.
    QString m_nom;      //!< Attribut: nom de la ligne de barème.
    int m_num;          //!< Attribut: numéro du barème pour la partie d'exercice.
    int m_valeur;       //!< Attribut: valeur de la partie d'exercice.


public:
    NOM_POS_5_ATT(IdEx,Fraction,Nom,Num,Valeur)
    INCLUCE_METHODE(Bareme)

    //! Constructeur à partir des valeurs attributs.
    Bareme(int idEx, int fraction, const QString &nom, int num, int valeur, int id = 0) : Entity(id)
    {
        setIdEx(idEx);
        setFraction(fraction);
        setNom(nom);
        setNum(num);
        setValeur(valeur);
    }

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    Bareme(int idEx, int num)
    {
        setIdEx(idEx);
        setNum(num);
    }

    GET_SET_ID(Ex)
    GET_SET_INT_SUP(fraction,Fraction,1)
    GET_SET_TEXTE_NOT_NULL(nom,Nom)
    GET_SET_INT_SUP(num,Num,0)
    GET_SET_INT_SUP(valeur,Valeur,0)

    //! Teste si l'entité est valide.
    bool isValid() const
    {
        return Entity::isValid() && (m_idEx > 0) && (m_fraction > 0) && (!m_nom.isEmpty()) && (m_num >= 0) && (m_valeur >= 0);
    }

protected:
    //! Test d'égalité entre cette entité et celle transmise en argument.
    bool egal(const Bareme & entity) const
    {
        return Entity::egal(entity)
                &&(m_idEx == entity.m_idEx)
                &&(m_fraction == entity.m_fraction)
                &&(m_nom == entity.m_nom)
                &&(m_num == entity.m_num)
                &&(m_valeur == entity.m_valeur);
    }

    //! Remplace les attributs de l'entité par celle de l'entité transmise, sauf l'identifiant.
    void set(const Bareme & entity)
    {
        setIdEx(entity.m_idEx);
        setFraction(entity.m_fraction);
        setNom(entity.m_nom);
        setNum(entity.m_num);
        setValeur(entity.m_valeur);

    }
};

#endif // BAREME_H

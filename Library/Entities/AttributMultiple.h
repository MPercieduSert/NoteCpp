/*Auteur: PERCIE DU SERT Maxime
 *Date: 17/02/2017
 */
#ifndef ATTRIBUTMULTIPLE_H
#define ATTRIBUTMULTIPLE_H

#include "AttributSimple.h"


/*! \ingroup groupeBaseAttributEntity
 * \brief Classe template des attributs multiple.
 */
template<class... Attribut> class Attributs;

template<class Attribut> class Attributs<Attribut> : public Attribut
{
public:
//    using AttPre = Attribut;
//    using AttSuiv = NoAttribut;
    using Attribut::Attribut;
    CONSTR_DESTR_DEFAUT(Attributs)
};

template <class AttributPremier, class... AttributSuivant> class Attributs<AttributPremier, AttributSuivant...>
        : public AttributPremier, public Attributs<AttributSuivant...>
{
public:
    //! Nombre d'attribut de l'entité.
    enum {NbrAtt = AttributPremier::NbrAtt + Attributs<AttributSuivant...>::NbrAtt};
    using AttPre = AttributPremier;
    using AttSuiv = Attributs<AttributSuivant...>;
    CONSTR_DESTR_DEFAUT(Attributs)

    //! Constructeur
    Attributs<AttributPremier, AttributSuivant...>(const AttPre & attPremier)
        : AttPre(attPremier) {}

    //! Constructeur
    Attributs<AttributPremier, AttributSuivant...>(const AttSuiv & attSuivant)
        : AttSuiv(attSuivant) {}

    //! Renvoie une chaine de caractère contenant les noms, les validités et les valeurs des attributs.
    virtual QString affiche() const
        {return AttributPremier::affiche().append("\n").append(Attributs<AttributSuivant...>::affiche());}

    //! Retourne un QVariant contenant la donnée souhaitée ou un QVariant nulle si num est invalide.
    QVariant data(int num) const
        {return num < NbrAtt ? dataP(num) : QVariant();}

    //! Teste si l'entité est valide.
    bool isValid() const
        {return AttributPremier::isValid() && Attributs<AttributSuivant...>::isValid();}

    //! Retourne le nom de l'attribut donnée souhaitée, pos doit être valide.
    static QString nomAttribut(int pos)
        {return pos < AttributPremier::NbrAtt ? AttributPremier::nomAttribut(pos)
                                              : Attributs<AttributSuivant...>::nomAttribut(pos - AttributPremier::NbrAtt);}

    //! Renvoie une chaine de caractère contenant les valeurs de l'attribut.
    QString toString() const
        {return AttributPremier::toString().append(",").append(Attributs<AttributSuivant...>::toString());}

    //! Test d'égalité entre cette entité et celle transmise en argument.
    bool operator ==(const Attributs<AttributPremier, AttributSuivant...> & entity) const
        {return AttributPremier::operator ==(entity) && Attributs<AttributSuivant...>::operator ==(entity);}

protected:
    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
    QVariant dataP(int pos) const
        {return pos < AttributPremier::NbrAtt ? AttributPremier::dataP(pos)
                                              : Attributs<AttributSuivant...>::dataP(pos - AttributPremier::NbrAtt);}

    //! Remplace les attributs de l'entité par celle de l'entité transmise.
    void set(const  Attributs<AttributPremier, AttributSuivant...> & entity)
    {
        AttributPremier::set(entity);
        Attributs<AttributSuivant...>::set(entity);
    }
};

template<class AttFound, class Att, class AttPre, class AttSuiv, int Pos> struct PosisionEnumTemp
{
    enum {Position = PosisionEnumTemp<AttFound,AttPre,typename AttPre::AttPre,typename AttPre::AttSuiv,Pos>::Position
          + PosisionEnumTemp<AttFound,AttSuiv,typename AttSuiv::AttPre,typename AttSuiv::AttSuiv,AttPre::NbrAtt + Pos>::Position};
};

template<class AttFound, class Att, int Pos> struct PosisionEnumTemp<AttFound,Att,NoAttribut,NoAttribut,Pos>
{enum {Position = 0};};

template<class AttFound, int Pos> struct PosisionEnumTemp<AttFound,AttFound,NoAttribut,NoAttribut,Pos>
{enum {Position = Pos};};

template<class AttFound, int Pos> struct PosisionEnumTemp<AttFound,Attributs<AttFound>,NoAttribut,NoAttribut,Pos>
{enum {Position = Pos};};

template<class AttFound, class Ent> using PositionEnum = PosisionEnumTemp<AttFound,Ent,typename Ent::AttPre,typename Ent::AttSuiv,0>;

using CibleAttributs = Attributs<IdCibleAttribut, CibleAttribut>;
using CibleDateTimeAttribut = Attributs<CibleAttributs, DateTimeValideAttribut>;
using CibleDateTimeNumAttribut = Attributs<CibleDateTimeAttribut, NumAttribut>;
using OrigineAttributs = Attributs<IdOrigineAttribut, OrigineAttribut>;
template<class ValeurAttribut> using CibleDateTimeNumValeurAttribut = Attributs<CibleDateTimeNumAttribut, ValeurAttribut>;
using RelationAttribut = Attributs<Id1Attribut, Id2Attribut>;
using RelationDateTimeAttribut = Attributs<RelationAttribut, DateTimeValideAttribut>;
using RelationNumAttribut = Attributs<RelationAttribut, NumAttribut>;
using IdNumAttribut = Attributs<Id1Attribut, NumAttribut>;
using IdProgNomAttribut = Attributs<IdProgAttribut,NomAttribut>;
template<class ValeurAttribut> using IdNumValeurAttribut = Attributs<IdNumAttribut, ValeurAttribut>;
using NomTypeAttribut = Attributs<NomAttribut, TypeAttribut>;
using NcNomAttribut = Attributs<NcAttribut, NomAttribut>;
using NcNomTypeAttribut = Attributs<NcNomAttribut, TypeAttribut>;
using TexteAttributs = Attributs<CreationAttribut,ModificationAttribut,TexteAttribut>;

/*! \ingroup groupeBaseAttributEntity
 * \brief Attributs contenant deux identifiant dont exactement un est non nul pour être valide.
 */
class RelationExactOneNotNullAttribut : public Attributs<Id1NullAttribut,Id2NullAttribut>
{
public:
    using Attributs<Id1NullAttribut,Id2NullAttribut>::Attributs;
    CONSTR_DESTR_DEFAUT(RelationExactOneNotNullAttribut)

    //! Teste si l'entité est valide.
    bool isValid() const
        {return (Id1NullAttribut::isValid() && id2() == 0) || (id1() == 0 && Id2NullAttribut::isValid());}
};


#endif // ENTITYATTRIBUT_H

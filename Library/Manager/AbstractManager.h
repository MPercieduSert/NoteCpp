/*Auteur: PERCIE DU SERT Maxime
 *Date: 22/02/2016
 */
#ifndef ABSTRACTMANAGER_H
#define ABSTRACTMANAGER_H

/*! \defgroup groupeManager Manageurs
 * \brief Ensemble de classes représentant les managers des entités de la base de donnée.
 */

/*! \defgroup groupeBaseManager Classe de bases des manageurs
 * \ingroup groupeManager
 * \brief Ensemble de classes de base des manageurs.
 */

#include <exception>
#include <QSqlError>

#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include "../Entities/Entity.h"
#include "../Div/ListPtr.h"
#include "../Div/MapPtr.h"
#include "../Div/Tree.h"
#include "../Div/VectorPtr.h"

/*! \ingroup groupeBaseManager
 * \brief Espace de nom pour la base de donnée.
 */
namespace bdd {
    //! Autorisation de modification d'une entité.
    enum Autorisation{Modif = 1,
                  Suppr = 2};
    //! Identifiant pour les fonctions d'agrega.
    enum Agrega {Max,
                Min,
                Moyenne,
                Nombre,
                Somme,
                NbrAgrega};

    //! Les différents types de conditions
    enum Condition {Egal,
                   Is,
                   Inf,
                   Sup,
                   InfEgal,
                   SupEgal,
                   Different,
                   IsNot};

    /*! \brief Les différents cas des résultats des tests d'existence d'unicité. 
     * (concordance d'indentifiant: ou bien l'identifiant en base de donnée et le même celui de l'entité testée ou bien ce dernier est nul.)
     * 
     */ 
    enum ExisteUni {Aucun = 0,  //!< Aucun des ensembles d'unicité n'existent en base de donnée.
            Tous = 1,   //!< Tous les ensembles d'unicité existent en base de donnée dans la même entité avec concordance d'identifiant.
            Meme = 2,   //!< Certains mais pas tous les ensembles d'unicité existent en base de donnée dans la même entité avec concordance d'identifiant.
            Autre = 3,  //!< Certains des ensembles d'unicité existent en base de donnée dans la même entité avec un autre identifiant.
            Conflit = 4 //!< Certains des ensembles d'unicité existent en base de donnée dans des entités différentes.
            };

    //! Enumeration des différent type de sauvegarde d'un arbre.
    enum TreeSave {EntityOnly,
                   AddLeaf,
                   WithoutDelete,
                   InternalChange,
                   ExternalChange,
                   EntityOnlyWhitoutRoot,
                   AddLeafWhitoutRoot,
                   WithoutDeleteWhitoutRoot,
                   InternalChangeWhitoutRoot,
                   ExternalChangeWhitoutRoot};
}

/*! \ingroup groupeBaseManager
 * \brief Classe abstraite de base des manageurs.
 *
 * Classe abstraite de base des manageurs.
 * Un manageur est un gestionnaire associé à un type d'entité,
 * permettant de faire le lien entre ce type d'entité et la table correspondante en base donnée.
 * Cette classe joue le rôle d'interface pour les différents manageurs.
 */
class AbstractManager
{
public:   
    //! Constructeur.
    AbstractManager() = default;

    //! Destructeur.
    ~AbstractManager() = default;

    //! Crée dans la base de donnée la table associée à l'entité du manageur.
    virtual void creer() = 0;

    //! Supprime de la table en base de donnée l'entité entity.
    virtual bool del(Entity & entity)
    {
        bool bb = del(entity.id());
        entity.setId(0);
        return bb;
    }

    //! Supprime de la table en base de donnée l'entité entity.
    virtual bool del(const Entity & entity)
        {return del(entity.id());}

    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    virtual bool exists(const Entity & entity) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée.
    //! Si le test est positif, l'identitfiant de l'entité entity est remplacé par celui de l'entité en base de donnée
    //! ayant les mêmes valeurs d'attributs uniques.
    virtual bdd::ExisteUni existsUnique(Entity & entity) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée.
    virtual bdd::ExisteUni existsUnique(const Entity & entity) = 0;

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant le même identifiant que entity.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool get(Entity & entity) = 0;

    /*//! Renvoie la liste des entités de la table ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Entity> getList(int ordre) = 0;

    //! Renvoie la liste des entités de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Entity> getList(int cle, const QVariant & value, int ordre) = 0;

    //! Renvoie la liste des entités de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1 puis ordre2.
    virtual ListPtr<Entity> getList(int cle, const QVariant & value, int ordre1, int ordre2) = 0;

    //! Renvoie la liste des entités de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Entity> getList(int cle1, int cle2, const QVariant & value1, const QVariant & value2, int ordre) = 0;*/

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant les mêmes valeurs pour les attributs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool getUnique(Entity & entity) = 0;

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes valeurs d'attributs (identifiant compris).
    virtual bool sameInBdd(const Entity & entity) = 0;

    //! Enregistre l'entité entity en base de donnée et assigne l'identifiant de l'entité insérée en base de donnée à entity.
    virtual void save(Entity & entity) = 0;

    //! Enregistre l'entité entity en base de donnée.
    virtual void save(const Entity & entity) = 0;

protected:
    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    virtual bool del(int id) = 0;

    //! Message d'erreurs si l'entité entity n'est pas valide.
    virtual QString messageErreurs(const Entity & entity) const = 0;

    //! Message d'erreurs s'il existe déjà en base de donnée une entité ayant les mêmes valeurs d'attributs uniques que entity.
    virtual QString messageErreursUnique(const Entity & entity) const = 0;
};

/*! \ingroup groupeBaseManager
 * \brief Classe abstraite de base des manageurs associée à l'entité Ent. Regroupe les méthodes virtuelles nécessaire à la base de donnée propre à l'entité Ent.
 */
template<class Ent> class AbstractManagerTemp : public AbstractManager
{
public:
    //! Constructeur.
    AbstractManagerTemp() = default;

    //! Destructeur.
    ~AbstractManagerTemp() = default;

    // Méthode d'existence
    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    virtual bool exists(const Ent & entity) = 0;

    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    bool exists(const Entity & entity)
        {return exists(Ent::convert(entity));}

    //! Teste s'il existe une entité vérifiant une condition en base de donnée.
    virtual bool exists(typename Ent::Position cle, const QVariant & value, bdd::Condition cond = bdd::Condition::Egal) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques qu'entity en base de donnée. De plus, si l'identifiant de entity est nul et qu'il existe en base de donnée exactement une entité possédant des ensembles d'attributs uniques avec les mêmes valeurs qu'entity, alors l'identifiant d'entity est remplacé par l'identifiant de cette entité.
    virtual bdd::ExisteUni existsUnique(Ent & entity) = 0;

    //! Idem bdd::ExisteUni existsUnique(Ent & entity) avec une conversion de référence.
    bdd::ExisteUni existsUnique(Entity & entity)
        {return existsUnique(Ent::convert(entity));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques qu'entity en base de donnée.
    virtual bdd::ExisteUni existsUnique(const Ent & entity) = 0;

    //! Idem bdd::ExisteUni existsUnique(const Ent & entity) avec une conversion de référence.
    bdd::ExisteUni existsUnique(const Entity & entity)
        {return existsUnique(Ent::convert(entity));}

    // Méthode d'aggrégation
    //! Fonction d'agrega de valeur de type T sur l'attribut att appliquée à toutes les entités de la table.
    template<class T> T fonctionAgrega(bdd::Agrega /*fonc*/, typename Ent::Position /*att*/)
        {throw std::invalid_argument(QString("La méthode 'fonctionAgregation' n'est pas définie pour le manager des : ").append(Ent::Name()).append(".").toStdString());}

    //! Fonction d'agrega de valeur de type T sur l'attribut att appliquée à toutes les entités vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    template<class T> T fonctionAgrega(bdd::Agrega /*fonc*/, typename Ent::Position /*att*/, typename Ent::Position /*cle*/,
                                       const QVariant & /*value*/, bdd::Condition /*cond*/ = bdd::Condition::Egal)
        {throw std::invalid_argument(QString("La méthode 'fonctionAgregation' n'est pas définie pour le manager des : ").append(Ent::Name()).append(".").toStdString());}

    //! Fonction d'agrega de valeur de type T sur l'attribut att appliquée à toutes les entités vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2.
    template<class T> T fonctionAgrega(bdd::Agrega /*fonc*/, typename Ent::Position /*att*/, typename Ent::Position /*cle1*/,
                                       const QVariant & /*value1*/, typename Ent::Position /*cle2*/,  const QVariant & /*value2*/,
                                       bdd::Condition /*cond1*/ = bdd::Condition::Egal,
                                       bdd::Condition /*cond2*/ = bdd::Condition::Egal)
        {throw std::invalid_argument(QString("La méthode 'fonctionAgregation' n'est pas définie pour le manager des : ").append(Ent::Name()).append(".").toStdString());}

    // Méthodes de demande
    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant le même identifiant que entity.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool get(Ent & entity) = 0;

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant le même identifiant que entity.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool get(Entity & entity)
        {return get(Ent::convert(entity));}

    //! Renvoie l'autorisation de modification de l'entité donnée en argument.
    virtual bool getAutorisation(const Ent & /*entity*/, bdd::Autorisation /*autorisation*/)
        {return true;}

    // Obtention d'un arbre.
    //! Renvoie l'arbre de toutes les entités.
    virtual Tree<Ent> getArbre()
        {throw std::invalid_argument(QString("La méthode 'getArbre' n'est pas définie pour le manager des : ").append(Ent::Name()).append(".").toStdString());}

    //! Renvoie l'arbre de racine d'identifiant id pour une entité de type arbre.
    virtual Tree<Ent> getArbre(int /*id*/)
        {throw std::invalid_argument(QString("La méthode 'getArbre' n'est pas définie pour le manager des : ").append(Ent::Name()).append(".").toStdString());}

    // Obtention d'une liste.
    //! Renvoie la liste des entités de la table des entités Ent ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position ordre = Ent::Id, bool croissant = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position cle, int value, typename Ent::Position ordre = Ent::Id, bdd::Condition cond = bdd::Condition::Egal, bool crois = true)
        {return getList(cle,QVariant(value),ordre,cond,crois);}

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position cle, const QVariant & value, typename Ent::Position ordre = Ent::Id, bdd::Condition cond = bdd::Condition::Egal, bool crois = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1 puis ordre2.
    virtual ListPtr<Ent> getList(typename Ent::Position cle, const QVariant & value, typename Ent::Position order1, typename Ent::Position order2, bdd::Condition cond = bdd::Condition::Egal, bool croissant1 = true, bool croissant2 = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1, ordre2 puis ordre3.
    virtual ListPtr<Ent> getList(typename Ent::Position cle, const QVariant & value,
                                 typename Ent::Position ordre1, typename Ent::Position ordre2, typename Ent::Position ordre3,
                                 bdd::Condition cond = bdd::Condition::Egal,
                                 bool crois1 = true, bool crois2 = true, bool crois3 = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position cle1, const QVariant & value1,
                                             typename Ent::Position cle2, const QVariant & value2,
                                             typename Ent::Position ordre = Ent::Id,
                                             bdd::Condition cond1 = bdd::Condition::Egal, bdd::Condition cond2 = bdd::Condition::Egal,
                                             bool crois = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2,
    //! et valeur de la colonne d'identifiant cle3 = value3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position cle1, const QVariant & value1,
                                             typename Ent::Position cle2, const QVariant & value2,
                                             typename Ent::Position cle3, const QVariant & value3,
                                             typename Ent::Position ordre = Ent::Id,
                                             bdd::Condition cond1 = bdd::Condition::Egal, bdd::Condition cond2 = bdd::Condition::Egal,
                                             bdd::Condition cond3 = bdd::Condition::Egal,
                                             bool crois = true) = 0;

    //! Renvoie le liste des descendant direct d'entity.
    virtual ListPtr<Ent> getListChilds(const Ent & /*entity*/)
        {throw std::invalid_argument(QString("La méthode 'ListPtr<Ent> getListChilds(const Ent & entity)' n'est pas définie pour le manager des : ").append(Ent::Name()).append(".").toStdString());}

    // Liste de Jointure
    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (colonneTable = colonneJoin),
    //! valeur des colonnes de la table Ent d'identifiant key = value de QMap whereMapTable,
    //! valeur des colonnes de la table Join key = value de QMap whereMapJoin,
    //! ordonnée suivant les colonnes de la table Ent d'identifiants key et d'ordre value de QMap orderMapTable (true -> croissant, false -> décroissant).
    virtual ListPtr<Ent> getListJoin(const QString & tableJoin, int colonneTable,
                                     const QString & colonneJoin,
                                     const QMap<int,QVariant> & whereMapTable,
                                     const QMap<QString,QVariant> & whereMapJoin,
                                     const QList<QPair<int,bool>> & orderMapTable) = 0;

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (table.ID = join.colonneJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = valueWhere,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    virtual ListPtr<Ent> getListJoin(const QString & tableJoin,
                                     const QString & colonneJoin,
                                     const QString & whereJoin,
                                     const QVariant & valueWhere,
                                     typename Ent::Position ordre = Ent::Id, bdd::Condition cond = bdd::Condition::Egal, bool crois = true) = 0;

    // Map
    //! Renvoie la map des entités de la table des entités Ent.
    virtual MapPtr<Ent> getMap(typename Ent::Position cleMap = Ent::Id) = 0;

    //! Renvoie la map des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    virtual MapPtr<Ent> getMap(typename Ent::Position cle, const QVariant & value,
                               typename Ent::Position cleMap = Ent::Id, bdd::Condition cond = bdd::Condition::Egal) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2.
    virtual MapPtr<Ent> getMap(typename Ent::Position cle1, const QVariant & value1,
                               typename Ent::Position cle2, const QVariant & value2,
                               typename Ent::Position cleMap = Ent::Id,
                               bdd::Condition cond1 = bdd::Condition::Egal, bdd::Condition cond2 = bdd::Condition::Egal) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2,
    //! et valeur de la colonne d'identifiant cle3 = value3.
    virtual MapPtr<Ent> getMap(typename Ent::Position cle1, const QVariant & value1,
                               typename Ent::Position cle2, const QVariant & value2,
                               typename Ent::Position cle3, const QVariant & value3,
                               typename Ent::Position cleMap = Ent::Id,
                               bdd::Condition cond1 = bdd::Condition::Egal, bdd::Condition cond2 = bdd::Condition::Egal,
                               bdd::Condition cond3 = bdd::Condition::Egal) = 0;

    // Map de Jointure
    //! Renvoie la map des entités de la table vérifiant une condition sur une jointure (colonneTable = colonneJoin),
    //! valeur des colonnes de la table Ent d'identifiant key = value de QMap whereMapTable,
    //! valeur des colonnes de la table Join key = value de QMap whereMapJoin.
    virtual MapPtr<Ent> getMapJoin(const QString & tableJoin, int colonneTable,
                                   const QString & colonneJoin,
                                   const QMap<int,QVariant> & whereMapTable,
                                   const QMap<QString,QVariant> & whereMapJoin,
                                   typename Ent::Position cleMap = Ent::Id) = 0;

    //! Renvoie la map des entités de la table vérifiant une condition sur une jointure (table.ID = join.colonneJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = valueWhere.
    virtual MapPtr<Ent> getMapJoin(const QString & tableJoin,
                                   const QString & colonneJoin,
                                   const QString & whereJoin,
                                   const QVariant & valueWhere,
                                   typename Ent::Position cleMap = Ent::Id,
                                   bdd::Condition cond = bdd::Condition::Egal) = 0;

    //! Renvoie la liste des restrictions de modification de l'entité donnée en argument.
    virtual QList<int> getRestriction(const Ent & /*entity*/)
        {return QList<int>();}

    //! Renvoie le vecteur des descendant direct d'entity.
    VectorPtr<Ent> getVectorChilds(const Ent & /*entity*/)
        {throw std::invalid_argument(QString("La méthode 'ListPtr<Ent> getListChilds(const Ent & entity)' n'est pas définie pour le manager des : ").append(Ent::Name()).append(".").toStdString());}

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant les mêmes valeurs pour au moins un ensemble des attributs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool getUnique(Ent & entity) = 0;

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant les mêmes valeurs pour les attributs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool getUnique(Entity & entity)
        {return getUnique(Ent::convert(entity));}

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    virtual bool sameInBdd(const Ent & entity) = 0;

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    bool sameInBdd(const Entity &entity)
        {return sameInBdd(Ent::convert(entity));}

    // Save
    //! Enregistre l'entity dans la base de donnée.
    virtual void save(Ent & entity) = 0;

    //! Enregistre l'entity dans la base de donnée.
    virtual void save(const Ent & entity) = 0;

    //! Enregistre l'entité entity en base de donnée et assigne l'identifiant de l'entité insérée en base de donnée à entity.
    void save(Entity & entity)
        {save(Ent::convert(entity));}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Entity & entity)
        {save(Ent::convert(entity));}

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification.
    virtual void save(Ent & entity, bdd::Autorisation autorisation, bool bb = false)
    {
        save(entity);
        setAutorisation(entity, autorisation, bb);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification.
    virtual void save(const Ent & entity, bdd::Autorisation autorisation, bool bb = false)
    {
        save(entity);
        setAutorisation(entity, autorisation, bb);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisations de modification.
    virtual void save(Ent & entity, const QMap<bdd::Autorisation,bool> & autorisations)
    {
        save(entity);
        setAutorisation(entity, autorisations);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisations de modification.
    virtual void save(const Ent & entity, const QMap<bdd::Autorisation,bool> & autorisations)
    {
        save(entity);
        setAutorisation(entity, autorisations);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles restriction de modification.
    virtual void save(Ent & entity, const QList<bdd::Autorisation> restriction)
    {
        save(entity);
        setRestriction(entity, restriction);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelle restriction de modification.
    virtual void save(const Ent & entity, const QList<bdd::Autorisation> restriction)
    {
        save(entity);
        setRestriction(entity, restriction);
    }

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void save(Ent & /*entity*/, const Ent & /*parent*/, int /*num*/ = 0)
        {throw std::invalid_argument(QString("La méthode 'save(Ent & entity, const Ent & parent, int num)' n'est pas définie pour le manager des : ").append(Ent::Name()).append(".").toStdString());}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void save(const Ent & /*entity*/, const Ent & /*parent*/, int /*num*/ = 0)
    {throw std::invalid_argument(QString("La méthode 'save(const Ent & entity, const Ent & parent, int num)' n'est pas définie pour le manager des : ").append(Ent::Name()).append(".").toStdString());}

    //! Enregistre l'arbre d'entités dans la base de donnée pour les entités de type arbre.
    virtual void save(Tree<Ent> & /*arbre*/, bdd::TreeSave /*n*/ = bdd::TreeSave::ExternalChange)
        {throw std::invalid_argument(QString("La méthode 'save(Tree<Ent>,...)' n'est pas définie pour le manager des : ").append(Ent::Name()).append(".").toStdString());}

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est mise à jour
    //! et l'identifiant de entity devient idU,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    virtual void saveUnique(Ent & entity) = 0;

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    virtual void saveUnique(const Ent & entity) = 0;

    //! Modifie une autorisation de modification pour une entité donnée.
    virtual void setAutorisation(const Ent & entity, bdd::Autorisation /*autorisation*/, bool bb = false)
    {
        if(!bb)
            throw std::invalid_argument(QString("Les entités du type de l'entité suivante ne peuvent avoir de restriction de modification.\n").append(entity.affiche()).append(".").toStdString());
    }

    //! Modifie les autorisations de modification pour une entité donnée.
    virtual void setAutorisation(const Ent & entity, const QMap<bdd::Autorisation,bool> & autorisations)
    {
        QMap<bdd::Autorisation,bool>::const_iterator i = autorisations.cbegin();
        while(i != autorisations.cend() && i.value()) i++;
        if(i != autorisations.cend())
            throw std::invalid_argument(QString("Les entités du type de l'entité suivante ne peuvent avoir de restriction de modification.\n").append(entity.affiche()).append(".").toStdString());
    }

    //! Ajoute des restrictions de modification pour une entité donnée.
    virtual void setRestriction(const Ent & entity, const QList<bdd::Autorisation> restriction)
    {
        if(!restriction.isEmpty())
            throw std::invalid_argument(QString("Les entités du type de l'entité suivante ne peuvent avoir de restriction de modification.\n").append(entity.affiche()).append(".").toStdString());
    }
};

#endif // ABSTRACTMANAGER_H

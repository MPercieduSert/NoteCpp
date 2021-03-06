/*Auteur: PERCIE DU SERT Maxime
 *Date: 15/08/2017
 */
#ifndef MANAGEROFMODIFCONTROLE_H
#define MANAGEROFMODIFCONTROLE_H

#include "AbstractGestionAutorisation.h"
#include "ManagerSql.h"

//! \ingroup groupeBaseManager
//! Début du coprs des deux methodes save.
#define SAVE_MODIF_DEBUT if(entity.isValid()) \
    {if(entity.isNew()) \
        {if(existsUnique(entity) == bdd::Aucun) \
            {Ent ent(entity); \
            add(ent);

//! \ingroup groupeBaseManager
//! Milieu du coprs des deux methodes save.
#define SAVE_MODIF_MILIEU }else \
            throw std::invalid_argument(messageErreursUnique(entity).append("\n Erreur d'unicité : il existe déjà dans la base de donnée une entité ayant les mêmes valeurs d'attributs unique que cette nouvelle entité.").toStdString());} \
    else \
        {if(!sameInBdd(entity)) \
            {if(existsUnique(entity) <= bdd::Meme) \
                modify(entity); \
            else \
                throw std::invalid_argument(messageErreursUnique(entity).append("\n Erreur d'unicité :  il existe déjà dans la base de donnée une entité ayant les mêmes valeurs d'attributs unique que l'entité modifiée.").toStdString());}

            //! \ingroup groupeBaseManager
            //! Milieu du coprs des deux methodes save.
#define SAVE_MODIF_FIN }} else \
    throw std::invalid_argument(messageErreurs(entity).append("\n Erreur de validité").toStdString());

/*! \ingroup groupeBaseManager
 * \brief Classe template mère des différents manageurs à modification controlée.
 */
template<class Ent> class BaseManagerModifControle
{
protected:
    bool m_bypass = false;  //!< Booléen permettant de courtcircuité le gestionnaire d'autorisation.
    AbstractGestionAutorisation<Ent> * m_gestionAutorisation;   //!< Gestionnaire d'autorisation
public:
    //! Constructeur.
    BaseManagerModifControle(AbstractGestionAutorisation<Ent> * gestionAutorisation = nullptr)
        : m_gestionAutorisation(gestionAutorisation)
    {}

    //! Accesseur du gestionnaire d'autorisations.
    AbstractGestionAutorisation<Ent> * gestionAutorisation() const
        {return m_gestionAutorisation;}

    //! Demande l'autorisation de modification pour une entité donnée.
    virtual bool getAutorisation(const Ent & entity, bdd::Autorisation autorisation)
        {return m_gestionAutorisation->getAutorisation(entity, autorisation) || m_bypass;}

    //! Demande l'autorisation de modification pour une entité d'identifiant id avec les valeurs de entity.
    virtual bool getAutorisation(const Ent & entity, int id, bdd::Autorisation autorisation)
        {return m_gestionAutorisation->getAutorisation(entity, id, autorisation) || m_bypass;}

    //! Demande la liste des restrictions de modification pour une entité donnée.
    virtual QList<int> getRestriction(const Ent & entity)
        {return m_gestionAutorisation->getRestriction(entity);}

    //! Modifie une autorisation de modification pour une entité donnée.
    virtual void setAutorisation(const Ent & entity, bdd::Autorisation autorisation, bool bb)
        {m_gestionAutorisation->setAutorisation(entity, autorisation, bb);}

    //! Modifie les autorisations de modification pour une entité donnée.
    virtual void setAutorisation(const Ent & entity, QMap<bdd::Autorisation,bool> autorisations)
        {m_gestionAutorisation->setAutorisation(entity, autorisations);}

    //! Mutateur du gestionnaire d'autorisations.
    void setGestionAutorisation(AbstractGestionAutorisation<Ent> * gestionAutorisation)
        {m_gestionAutorisation = gestionAutorisation;}

    //! Ajoute les restrictions de modification pour une entité donnée.
    virtual void setRestriction(const Ent & entity, QList<bdd::Autorisation> restrictions)
        {m_gestionAutorisation->setRestriction(entity, restrictions);}

protected:
    //! Met en place un court circuit du gestionnaire d'autorisations.
    void ouvrir()
        {m_bypass = true;}

    //! Met fin au court circuit du gestionnaire d'autorisations.
    void fermer()
        {m_bypass = false;}
};

/*! \ingroup groupeBaseManager
 * \brief Classe template manageurs à modification controlée.
 */
template<class Ent, class Unique> class ManagerOfModifControle : public virtual ManagerSql<Ent,Unique>, public BaseManagerModifControle<Ent>
{
protected:
    using ManagerSqlEnt = ManagerSql<Ent,Unique>;
    using ManagerBMC = BaseManagerModifControle<Ent>;

    using ManagerSqlEnt::add;
    using ManagerSqlEnt::messageErreurs;
    using ManagerSqlEnt::messageErreursUnique;
    using ManagerSqlEnt::table;
    using ManagerBMC::ouvrir;
    using ManagerBMC::fermer;

public:
    using ManagerSqlEnt::existsUnique;
    using ManagerSqlEnt::exists;
    using ManagerSqlEnt::del;
    using ManagerSqlEnt::get;
    using ManagerSqlEnt::sameInBdd;
    using ManagerSqlEnt::save;
    using ManagerBMC::getAutorisation;
    using ManagerBMC::getRestriction;
    using ManagerBMC::setAutorisation;
    using ManagerBMC::setRestriction;

    //! Constructeur
    ManagerOfModifControle (const QString & table, const QMap<int,QString> & att, const QVector<QMap<int,int>> & attUnique)
        : ManagerSql<Ent,Unique>(table, att, attUnique)
    {}

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification..
    void save(Ent & entity, bdd::Autorisation autorisation, bool bb = false)
    {
        ouvrir();
        save(entity);
        fermer();
        setAutorisation(entity, autorisation, bb);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification.
    void save(const Ent & entity, bdd::Autorisation autorisation, bool bb = false)
    {
        SAVE_MODIF_DEBUT
        setAutorisation(ent, autorisation, bb);
        SAVE_MODIF_MILIEU
        setAutorisation(entity, autorisation, bb);
        SAVE_MODIF_FIN
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisation de modification..
    void save(Ent & entity, const QMap<bdd::Autorisation,bool> & autorisations)
    {
        ouvrir();
        save(entity);
        fermer();
        setAutorisation(entity, autorisations);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisations de modification.
    void save(const Ent & entity, const QMap<bdd::Autorisation,bool> & autorisations)
    {
        SAVE_MODIF_DEBUT
        setAutorisation(ent, autorisations);
        SAVE_MODIF_MILIEU
        setAutorisation(entity, autorisations);
        SAVE_MODIF_FIN
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles restriction de modification.
    void save(Ent & entity, const QList<bdd::Autorisation> restriction)
    {
        ouvrir();
        save(entity);
        fermer();
        setRestriction(entity, restriction);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles restrictions de modification..
    void save(const Ent & entity, const QList<bdd::Autorisation> restriction)
    {
        SAVE_MODIF_DEBUT
        setRestriction(ent, restriction);
        SAVE_MODIF_MILIEU
        setRestriction(entity, restriction);
        SAVE_MODIF_FIN
    }

protected:
    //! Constructeur.
    ManagerOfModifControle() = default;

    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(int id)
    {
        if(getAutorisation(Ent(id), bdd::Suppr))
            return ManagerSqlEnt::del(id);
        else
            return false;
        /*{
            Ent entity(id);
            get(entity);
            throw std::invalid_argument(QString("Erreur d'autorisation de modification' : "
                                                "l'entité suivante n'est pas suprimable:\n").append(entity.affiche()).toStdString());
        }*/
    }

    //! Met à jour l'entité entity en base de donnée.
    void modify(const Ent & entity)
    {
        if(getAutorisation(entity, bdd::Autorisation::Modif))
            ManagerSqlEnt::modify(entity);
        else
            throw std::invalid_argument(QString("Erreur d'autorisation de modification' : "
                                                "l'entité suivante n'est pas modifiable:\n")
                                        .append(entity.affiche()).toStdString());
    }

    //! Met à jour l'entité entity en base de donnée d'identifiant id avec les valeurs d'entity.
    void modify(const Ent & entity, int id)
    {
        if(getAutorisation(entity, id, bdd::Autorisation::Modif))
            ManagerSqlEnt::modify(entity,id);
        else
            throw std::invalid_argument(QString("Erreur d'autorisation de modification' : "
                                                "l'entité suivante n'est pas modifiable:\n")
                                        .append(entity.affiche()).toStdString());
    }
};
#endif // MANAGEROFMODIFCONTROLE_H

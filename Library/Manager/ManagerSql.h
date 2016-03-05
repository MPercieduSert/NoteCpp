/*Auteur: PERCIE DU SERT Maxime
 *Date: 26/02/2016
 */

#ifndef MANAGERSQL_H
#define MANAGERSQL_H

#include "Manager.h"

/*! \ingroup groupeManager
 * \brief Classe template permettant d'instancier les différents manageurs.
 *
 * Classe template permettant d'instancier les différents manageurs.
 * Cette classe implémente les méthodes virtuelles pures de sa classe mère.
 * La classe Ent est la classe d'entitée de programmation
 * et la classe Lbdd est le lien avec les entitées en base de donnée.
 */

template<class Lbdd> class ManagerSql : public Manager
{
    typedef typename Lbdd::Ent Ent;

protected:
    QSqlQuery & m_requete;        //!< Objet executant les requétes sql du manageur.
    Lbdd m_linkBdd;             //!< Lien.
    const QString m_sqlAdd;     //!< Requéte sql d'insertion d'une ligne.
    const QString m_sqlDelete;  //!< Requéte sql de suppression d'une ligne
    const QString m_sqlExists;  //!< Requéte sql d'existence d'une ligne d'identifiant donné.
    const QString m_sqlGet;     //!< Requéte sql de lecture d'une ligne d'identifiant donné.
    const QString m_sqlGetList; //!< Requéte sql de lecture de toutes les lignes de la table triées suivant une colonne.
    const QString m_sqlGetList1Where1;  //!< Requéte sql de lecture des lignes de la table vérifiant une condition triées suivant une colonne.
    const QString m_sqlGetList1Where2;  //!< Requéte sql de lecture des lignes de la table vérifiant deux conditions triées suivant une colonne.
    const QString m_sqlGetList2Where1;  //!< Requéte sql de lecture des lignes de la table vérifiant une condition triées suivant deux colonnes.
    const QString m_sqllastId;  //!< Requéte sql de lecture de l'identifiant de la dernière ligne insérée dans la table.
    const QString m_sqlModify;  //!< Requéte sql de mis à jour d'une ligne de la table.


public:
    //! Contructeur, transmettre en argument la connexion ouverte à le base de donnée.
    ManagerSql(QSqlQuery & req);

    //! Destructeur.
    ~ManagerSql()  {}

    //! Crée dans la base de donnée la table associée l'entitée du manageur.
    void creer();

    //! Teste s'il existe une entitée de même identifiant que entity en base de donnée.
    bool exists(const Entity & entity)
    {
        if(Ent::verifEntity(entity))
        {
            m_requete.prepare(m_sqlExists);
            m_requete.bindValue(0,entity.id());
            exec();
            bool bb = m_requete.next();
            m_requete.finish();
            return bb;
        }
        else
        {
            throw std::runtime_error("Mauvaise correspondance des Entity");
        }
    }

    //! Teste s'il existe une entitée ayant les mêmes valeurs d'attributs uniques que l'entitée entity en base de donnée.
    //! Si le test est positif, l'identitfiant de l'entitée entity est remplacé par celui l'entitée en base de donnée
    //! ayant les mêmes valeurs d'attributs uniques.
    Manager::ExisteUni existsUnique(Entity & entity)
    {
        return m_linkBdd.existsUnique(Ent::convert(entity));
    }

    //! Teste s'il existe une entitée ayant les mêmes valeurs d'attributs uniques que entity en base de donnée.
    Manager::ExisteUni existsUnique(const Entity & entity)
    {
        return m_linkBdd.existsUnique(Ent::convert(entity));
    }

    //! Hydrate l'entitée entity avec les valeurs des attributs de l'entitée enregistrée en base de donnée
    //! ayant le même identifiant que entity.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool get(Entity & entity)
    {
        m_requete.prepare(m_sqlGet);
        m_requete.bindValue(0,entity.id());
        exec();
        if(m_requete.next())
        {
            m_linkBdd.fromRequete(Ent::convert(entity));
            m_requete.finish();
            return true;
        }
        else
        {
            m_requete.finish();
            return false;
        }
    }

    //! Renvoie la liste des entitées de la table ordonnée suivant la colonne d'identifiant ordre.
    ListEntities<Entity> getList(int ordre)
    {
        m_requete.prepare(m_sqlGetList.arg(Lbdd::Att[ordre]));
        return listFormRequete();
    }

    //! Renvoie la liste des entitées de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    ListEntities<Entity> getList(int cle, const QVariant & value, int ordre)
    {
        m_requete.prepare(m_sqlGetList1Where1.arg(Lbdd::Att[cle], Lbdd::Att[ordre]));
        m_requete.bindValue(0,value);
        return listFormRequete();
    }

    //! Renvoie la liste des entitées de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1 puis ordre2.
    ListEntities<Entity> getList(int cle, const QVariant & value, int ordre1, int ordre2)
    {
        m_requete.prepare(m_sqlGetList1Where2.arg(Lbdd::Att[cle], Lbdd::Att[ordre1], Lbdd::Att[ordre2]));
        m_requete.bindValue(0,value);
        return listFormRequete();
    }

    //! Renvoie la liste des entitées de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    ListEntities<Entity> getList(int cle1, int cle2, const QVariant & value1, const QVariant & value2, int ordre)
    {
        m_requete.prepare(m_sqlGetList2Where1.arg(Lbdd::Att[cle1], Lbdd::Att[cle2], Lbdd::Att[ordre]));
        m_requete.bindValue(0,value1);
        m_requete.bindValue(1,value2);
        return listFormRequete();
    }

    //! Teste s'il y a dans la base de donnée une entitée ayant exactement les mêmes attributs (identifiant compris).
    bool sameInBdd(const Entity &entity)
    {
        Ent entityT(entity.id());
        if(get(entityT))
        {
            return entityT == entity;
        }
        else
        {
            return false;
        }
    }

protected:
    //! Insert la nouvelle entitée entity dans la base de donnée
    //! et assigne l'identifiant de l'entitée insérée en base de donnée à entity.
    void add(Entity & entity)
    {
        m_requete.prepare(m_sqlAdd);
        m_linkBdd.bindValues(Ent::convert(entity));
        exec();
        exec(m_sqllastId);
        entity.setId(m_requete.value(0).toInt());
        m_requete.finish();
    }

    //! Insert la nouvelle entitée entity dans la base de donnée.
    void add(const Entity & entity)
    {
        m_requete.prepare(m_sqlAdd);
        m_linkBdd.bindValues(Ent::convert(entity));
        exec();
        m_requete.finish();
    }

    //! Lance une execption contenant le message d'erreur de la derniere requète SQL exéctuter.
    void affError() const;

    //! Supprime de la table en base de donnée l'entitée d'identifiant id.
    void del(int id)
    {
        m_requete.prepare(m_sqlDelete);
        m_requete.bindValue(0,id);
        exec();
        m_requete.finish();
    }

    //! Exécute la dernière requète préparée et lance une execption si celle-ci n'est pas valide.
    void exec()
    {
        if(m_requete.exec())
        {
            affError();
        }
    }

    //! Exécute la requéte SQL donnée en argument et lance une execption si celle-ci n'est pas valide.
    void exec(const QString & requete)
    {
        if(m_requete.exec(requete))
        {
            affError();
        }
    }

    //! Construit la liste des entitées correspondant une requète de type sqlGetList.
    ListEntities<Entity> listFormRequete()
    {
        exec();
        ListEntities<Entity> liste;
        while(m_requete.next())
        {
            liste.append(m_linkBdd.newFromRequete());
        }
        m_requete.finish();
        return liste;
    }

    //! Message d'erreurs si l'entitée entity n'est pas valide.
    QString messageErreurs(const Entity & entity) const;

    //! Message d'erreurs s'il existe déjà en base de donnée une entitée ayant les mêmes valeurs d'attributs uniques que entity.
    QString messageErreursUnique(const Entity & entity) const;

    //! Met à jour l'entitée entity en base de donnée.
    void modify(const Entity & entity)
    {
        m_requete.prepare(m_sqlModify);
        m_linkBdd.bindValues(Ent::convert(entity));
        m_requete.bindValue(Ent::IdPos,entity.id());
        exec();
        m_requete.finish();
    }
/*
    //! Modifie la base de donnée sur laquelle les requètes sont effectuées
    void setBdd(QSqlDatabase & bdd)
    {
        m_requete = QSqlQuery(bdd);
    }
*/
    //! Renvoie la chaine des noms des attributs séparés par une virgule sans l'identifiant.
    const QString writeColonne() const;

    //! Renvoie la chaine correspondant à la requète d'insertion d'une ligne.
    const QString writeStringAddQuery() const;

    //! Renvoie la chaine correspondant à la requète de suppression d'une ligne.
    const QString writeStringDeleteQuery() const;

    //! Renvoie la chaine correspondant à la requète d'existence d'une ligne.
    const QString writeStringExistsQuery() const;

    //! Renvoie la chaine correspondant à la requète de lecture d'une ligne.
    const QString writeStringGetQuery() const;

    //! Renvoie la chaine correspondant à la requète de lecture d'une table.
    const QString writeStringGetListQuery() const;

    //! Renvoie la chaine correspondant à la requète de lecture d'une liste vérifiant une condition.
    const QString writeStringGetList1Where1Query() const;

    //! Renvoie la chaine correspondant à la requète de lecture d'une liste
    //! vérifiant une condition ordonnée suivant les valeurs de deux attributs.
    const QString writeStringGetList1Where2Query() const;

    //! Renvoie la chaine correspondant à la requète de lecture d'une liste vérifiant deux conditions.
    const QString writeStringGetList2Where1Query() const;

    //! Renvoie la chaine correspondant à la requète demandant l'identifiant de la dernière ligne insérée dans la table.
    const QString writeStringLastIdQuery() const;

    //! Renvoie la chaine correspondant à la requète de mis à jour d'une ligne.
    const QString writeStringModifyQuery() const;
};

template<class Lbdd> ManagerSql<Lbdd>::ManagerSql(QSqlQuery & req)
: m_requete(req),
  m_linkBdd(m_requete),
  m_sqlAdd(writeStringAddQuery()),
  m_sqlExists(writeStringExistsQuery()),
  m_sqlGet(writeStringGetQuery()),
  m_sqlGetList(writeStringGetListQuery()),
  m_sqlGetList1Where1(writeStringGetList1Where1Query()),
  m_sqlGetList1Where2(writeStringGetList1Where2Query()),
  m_sqlGetList2Where1(writeStringGetList2Where1Query()),
  m_sqllastId(writeStringLastIdQuery()),
  m_sqlModify(writeStringModifyQuery())
{
    m_requete.setForwardOnly(true);
}

template<class Lbdd> void ManagerSql<Lbdd>::affError() const
{
    QSqlError err(m_requete.lastError());
    throw std::invalid_argument(err.text().append("\n"+err.nativeErrorCode()+"\n"+m_requete.lastQuery()).toStdString());
}

template<class Lbdd> void ManagerSql<Lbdd>::creer()
{
    m_linkBdd.creer();
}

template<class Lbdd> QString ManagerSql<Lbdd>::messageErreurs(const Entity & entity) const
{
    QString message("Entitée invalide:");
    message.append(Ent::Name).append(" d'identifiant:").append(QString::number(entity.idEntity())).append("\n");
    if(!entity.erreurs().isEmpty())
    {
        message.append("\n Erreurs :\n");
        QPair<int,int> erreur;
        foreach (erreur, entity.erreurs())
        {
            message.append(QString::number(erreur.second)+" :"+Ent::Att.at(erreur.second)+"\n");
        }
    }
    else
    {
        message.append("/n pas d'erreurs");
    }
    return message;
}

template<class Lbdd> QString ManagerSql<Lbdd>::messageErreursUnique(const Entity & entity) const
{
    QString message("Entitée ayant les même valeurs d'attributs unique déjà présente dans la base de donnée.\n");
    message.append(Ent::Name).append(" d'identifiant:").append(QString::number(entity.idEntity())).append("\n");
    //message.append("Valeurs Uniques:\n");
    //for(int i = 0; i != Ent::NbrAttUn; ++i)
    //    message.append(Ent::Att[Ent::AttUn[i]]).append("\n");
    return message;
}

template<class Lbdd> const QString ManagerSql<Lbdd>::writeColonne() const
{
    QString colonnes;
    for(int i = 1; i != Lbdd::NbrAtt-1; ++i) colonnes.append(Lbdd::Att[i]).append(",");
    colonnes.chop(1);
    return colonnes;
}

template<class Lbdd> const QString ManagerSql<Lbdd>::writeStringAddQuery() const
{
    QString sql("INSERT INTO ");
    sql.append(Lbdd::Name).append(" (").append(writeColonne()).append(") VALUES (");
    for(int i = 0; i != Lbdd::NbrAtt; ++i) sql.append("?,");
    sql.chop(1);
    sql.append(")");
    sql.squeeze();
    return sql;
}

template<class Lbdd> const QString ManagerSql<Lbdd>::writeStringDeleteQuery() const
{
    QString sql("DELETE FROM ");
    sql.append(Lbdd::Name).append(" WHERE ID=?");
    sql.squeeze();
    return sql;
}


template<class Lbdd> const QString ManagerSql<Lbdd>::writeStringExistsQuery() const
{
    QString sql("SELECT 1 FROM ");
    sql.append(Lbdd::Name).append(" WHERE ID=? LIMIT 1");
    sql.squeeze();
    return sql;
}

template<class Lbdd> const QString ManagerSql<Lbdd>::writeStringGetQuery() const
{
    QString sql("SELECT ");
    sql.append(writeColonne()).append(" FROM ").append(Lbdd::Name).append(" WHERE ID=?");
    sql.squeeze();
    return sql;
}

template<class Lbdd> const QString ManagerSql<Lbdd>::writeStringGetListQuery() const
{
    QString sql("SELECT ");
    sql.append(writeColonne()).append(",ID FROM ").append(Lbdd::Name).append(" ORDER BY %1");
    sql.squeeze();
    return sql;
}

template<class Lbdd> const QString ManagerSql<Lbdd>::writeStringGetList1Where1Query() const
{
    QString sql("SELECT ");
    sql.append(writeColonne()).append(",ID FROM ").append(Lbdd::Name).append(" WHERE %1=? ORDER BY %2");
    sql.squeeze();
    return sql;
}

template<class Lbdd> const QString ManagerSql<Lbdd>::writeStringGetList1Where2Query() const
{
    QString sql("SELECT ");
    sql.append(writeColonne()).append(",ID FROM ").append(Lbdd::Name).append(" WHERE %1=? ORDER BY %2,%3");
    sql.squeeze();
    return sql;
}

template<class Lbdd> const QString ManagerSql<Lbdd>::writeStringGetList2Where1Query() const
{
    QString sql("SELECT ");
    sql.append(writeColonne()).append(",ID FROM ").append(Lbdd::Name).append(" WHERE %1=?,%2=? ORDER BY %3");
    sql.squeeze();
    return sql;
}

template<class Lbdd> const QString ManagerSql<Lbdd>::writeStringLastIdQuery() const
{
    QString sql("SELECT last_insert_rowid() FROM ");
    sql.append(Lbdd::Name);
    sql.squeeze();
    return sql;
}

template<class Lbdd> const QString ManagerSql<Lbdd>::writeStringModifyQuery() const
{
    QString sql("UPDATE ");
    sql.append(Lbdd::Name).append(" SET ");
    for(int i = 0; i != Lbdd::NbrAtt; ++i) sql.append(Lbdd::Att[i]).append("=?,");
    sql.chop(1);
    sql.append("WHERE ID=?");
    sql.squeeze();
    return sql;
}

/*
        const QString m_sqlExistsUnique;
        m_sqlExistsUnique(writeStringExistsUniqueQuery()),
    //! Renvoie la chaine correspondant à la requète d'existence d'une ligne avec les attributs unique.
    const QString writeStringExistsUniqueQuery() const;

template<class Lbdd> const QString ManagerSql<Lbdd>::writeStringExistsUniqueQuery() const
{
    QString sql("SELECT 1 FROM ");
    sql.append(Lbdd::Name).append(" WHERE ");
    for(int i = 0; i != Lbdd::NbrAttUni; ++i)
        sql.append(Lbdd::Att[Lbdd::AttUni[i]]).append("=? AND ");
    sql.chop(5);
    sql.append(" LIMIT 1");
    sql.squeeze();
    return sql;
}
*/
/*
template<class Lbdd> void ManagerSql<Lbdd>::writeStringQuery()
{
    QString colonnes;
    for(int i = 0; i != nbrColonne(); ++i) colonnes.append(m_colonne.at(i)).append(",");
    colonnes.chop(1);

    //SQL Add
    m_sqlAdd = QString("INSERT INTO ");
    m_sqlAdd.append(m_tableName).append(" (").append(colonnes).append(") VALUES (");
    for(int i = 0; i != nbrColonne(); ++i) m_sqlAdd.append("?,");
    m_sqlAdd.chop(1);
    m_sqlAdd.append(")");

    //SQL Exists
    m_sqlExists = QString("SELECT 1 FROM ");
    m_sqlExists.append(m_tableName).append(" WHERE ID=? LIMIT 1");

    //SQL ExistsUnique
    m_sqlExistsUnique = QString("SELECT 1 FROM ");
    m_sqlExistsUnique.append(m_tableName).append(" WHERE ");
    for(int i = 0; i != m_colonneUnique.size(); ++i)
        m_sqlExistsUnique.append(m_colonne.at(m_colonneUnique.at(i))).append("=? AND ");
    m_sqlExistsUnique.chop(5);
    m_sqlExistsUnique.append(" LIMIT 1");

    //SQL Get
    m_sqlGet = QString("SELECT ");
    m_sqlGet.append(colonnes).append(" FROM ").append(m_tableName).append(" WHERE ID=?");

    //SQL GetList
    m_sqlGetList = QString("SELECT ID,");
    m_sqlGetList.append(colonnes).append(" FROM ").append(m_tableName).append(" ORDER BY %1");

    //SQL GetListWhere
    m_sqlGetListWhere = QString("SELECT ID,");
    m_sqlGetListWhere.append(colonnes).append(" FROM ").append(m_tableName).append(" WHERE %1=? ORDER BY %2");

    //SQL Modify
    m_sqlModify = QString("UPDATE "),
    m_sqlModify.append(m_tableName).append(" SET ");
    for(int i = 0; i != m_colonne.size(); ++i) m_sqlModify.append(m_colonne.at(i)).append("=?,");
    m_sqlModify.chop(1);
    m_sqlModify.append("WHERE ID=?");
}
*/
/*ListEntities<Entity> getList()
{
    m_requete.prepare(m_sqlGetList+"id");
    return listFormRequete();
}*/
/*ListEntities<Entity> getList(int cle, const QVariant & value)
{
    m_requete.prepare(m_sqlGetListWhere.arg(m_colonne.at(cle))+" ORDER BY ID");
    m_requete.bindValue(0,value);
    return listFormRequete();
}*/
/*ListEntities<Entity> getList(int cle1, int cle2, const QVariant & value1, const QVariant & value2)
{
    m_requete.prepare(m_sqlGetListWhere.arg(m_colonne.at(cle1))+QString("AND ").append(m_colonne.at(cle2)).append("=? ORDER BY ID"));
    m_requete.bindValue(0,value1);
    m_requete.bindValue(1,value2);
    return listFormRequete();
}*/
/*bool verifEntity(const Entity & entity) const {return Ent::IdEntity == entity.idEntity();}
T & verif(const Entity & entity) const
{
    if(verifEntity(entity))
    {
        return *((T *) &entity);
    }
    else
    {
        throw std::runtime_error("Mauvaise correspondance des Entity");
    }
}*/
#endif // MANAGERSQL_H

/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/03/2016
 */

#ifndef ELEVEMANAGER_H
#define ELEVEMANAGER_H

#include "ManagerSql.h"
#include "DonneeLinkSql.h"

/*! \ingroup groupeManager
 * \brief Manager de l'entitée Eleve.
 */

class EleveManager : public ManagerSql<DonneeLinkSql>
{
public:
    //! Constructeur, transmettre en argument une référence sur une objet de requete.
    EleveManager(QSqlQuery & req);
};

#endif // ELEVEMANAGER_H

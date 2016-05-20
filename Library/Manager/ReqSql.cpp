#include "ReqSql.h"

void ReqSql::affError() const
{
    QSqlError err(m_requete->lastError());
    throw std::invalid_argument(err.text().append("\n"+err.nativeErrorCode()+"\n"+m_requete->lastQuery()).toStdString());
}
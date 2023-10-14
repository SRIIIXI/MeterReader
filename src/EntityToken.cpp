#include "EntityToken.hpp"

EntityToken* tokenEntityPtr = nullptr;

EntityToken::EntityToken(QSqlDatabase *db, QString tabname) : DataEntity(db, tabname)
{
    tokenEntityPtr = this;
}

EntityToken::~EntityToken()
{
}

bool EntityToken::createTable()
{
    QString sqlString = "CREATE TABLE token( \
                        token_id Text NOT NULL PRIMARY KEY, \
                        serial_no Text NOT NULL, \
                        token_value Real NOT NULL, \
                        applied Integer NOT NULL, \
                        applied_date Integer NOT NULL, \
                        token_status Integer NOT NULL, \
                        token_status_text Text NOT NULL, \
                        row_index NOT NULL, \
                        CONSTRAINT unique_token_id UNIQUE ( token_id ))";

    QSqlQuery qryresult;

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(sqlString);

        _Database->commit();
    }
    catch(QException e)
    {
       _Database->rollback();
       QString errString = _Database->lastError().text();
       return false;
    }

    return true;

}

bool EntityToken::createToken(const Token &rec)
{
    QSqlQuery qryresult;
    QString err;

    QString sql;

    sql = "insert into token (token_id, serial_no, token_value, applied, applied_date, token_status, token_status_text, row_index) values('token_id__', 'serial_no__', token_value__, applied__, applied_date__, token_status__, 'token_status_text__', row_index__)";

    sql = sql.replace("token_id__", rec.TokenId_);
    sql = sql.replace("token_value__", QVariant(rec.TokenValue_).toString());
    sql = sql.replace("applied__", QVariant(rec.IsApplied_).toString());
    sql = sql.replace("serial_no__", rec.SerialNo_);
    sql = sql.replace("applied_date__", QVariant(rec.AppliedDate_).toString());
    sql = sql.replace("token_status__", QVariant(rec.TokenStatus_).toString());
    sql = sql.replace("token_status_text__", rec.TokenStatusText_);
    sql = sql.replace("row_index__", QVariant(rec.RowIndex_).toString());

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(sql);

        _Database->commit();
    }
    catch(QException e)
    {
       _Database->rollback();
       err = _Database->lastError().text();
       return false;
    }

    return true;
}

bool EntityToken::allTokens(QList<Token> &list)
{
    QString str;
    QList<QSqlRecord> *reclist = getAllRecords(str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        Token struc;

        struc.TokenId_ = rec.value(0).toString();
        struc.SerialNo_ = rec.value(1).toString();
        struc.TokenValue_ = rec.value(2).toReal();
        struc.IsApplied_ = rec.value(3).toInt();
        struc.AppliedDate_ = rec.value(4).toInt();
        struc.TokenStatus_ = rec.value(5).toInt();
        struc.TokenStatusText_ = rec.value(6).toString();
        struc.AppliedDateStr_ = QDateTime::fromSecsSinceEpoch(struc.AppliedDate_).toString("MMMM dd yyyy");

        if(struc.IsApplied_ && struc.TokenValue_ == 0)
        {
            struc.IsInValid_ = true;
        }
        else
        {
            struc.IsInValid_ = false;
        }

        list.append(struc);
    }

    return true;
}

bool EntityToken::selectedTokens(QList<Token> &list, QString keyname, QVariant value)
{
    QString str;
    QList<QSqlRecord> *reclist = getSelectedRecords(keyname, value, str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        Token struc;

        struc.TokenId_ = rec.value(0).toString();
        struc.SerialNo_ = rec.value(1).toString();
        struc.TokenValue_ = rec.value(2).toReal();
        struc.IsApplied_ = rec.value(3).toInt();
        struc.AppliedDate_ = rec.value(4).toInt();
        struc.TokenStatus_ = rec.value(5).toInt();
        struc.TokenStatusText_ = rec.value(6).toString();
        struc.AppliedDateStr_ = QDateTime::fromSecsSinceEpoch(struc.AppliedDate_).toString("MMMM dd yyyy");

        if(struc.IsApplied_ && struc.TokenValue_ == 0)
        {
            struc.IsInValid_ = true;
        }
        else
        {
            struc.IsInValid_ = false;
        }

        list.append(struc);
    }

    return true;
}

bool EntityToken::getRanges(QString serial_no, uint32_t &startRowIndex, uint32_t &endRowIndex)
{
    QList<QSqlRecord> *reclist = nullptr;
    QString sqlstart = "SELECT row_index FROM token where serial_no = '_serial_no_'  and row_index > 0 ORDER BY row_index ASC LIMIT 1";
    QString sqlend = "SELECT row_index FROM token where serial_no = '_serial_no_'  and row_index > 0 ORDER BY row_index DESC LIMIT 1";

    sqlstart = sqlstart.replace("_serial_no_", serial_no);
    sqlend = sqlend.replace("_serial_no_", serial_no);

    reclist= getCustomRecords(sqlstart);

    if(!reclist)
    {
        startRowIndex = 0;
    }
    else
    {
        if(reclist->count() < 1)
        {
            startRowIndex = 0;
        }
        else
        {
            startRowIndex = reclist->at(0).value(0).toUInt();
        }
    }

    reclist= getCustomRecords(sqlend);

    if(!reclist)
    {
        endRowIndex = 0;
    }
    else
    {
        if(reclist->count() < 1)
        {
            endRowIndex = 0;
        }
        else
        {
            endRowIndex = reclist->at(0).value(0).toUInt();
        }
    }

    return true;
}

bool EntityToken::getStartIndex(QString serial_no, uint32_t &startRowIndex)
{
    QList<QSqlRecord> *reclist = nullptr;
    QString sql = "SELECT row_index FROM token where serial_no = '_serial_no_' and row_index > 0 ORDER BY record_timestamp DESC LIMIT 1";

    sql = sql.replace("_serial_no_", serial_no);

    reclist= getCustomRecords(sql);

    if(!reclist)
    {
        startRowIndex = 0;
    }
    else
    {
        if(reclist->count() < 1)
        {
            startRowIndex = 0;
        }
        else
        {
            startRowIndex = reclist->at(0).value(0).toUInt();
        }
    }

    return true;
}



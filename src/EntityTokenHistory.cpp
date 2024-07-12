#include "EntityTokenHistory.hpp"

EntityTokenHistory* tokenHistoryEntityPtr = nullptr;

EntityTokenHistory::EntityTokenHistory(QSqlDatabase *db, QString tabname) : DataEntity(db, tabname)
{
    tokenHistoryEntityPtr = this;
}

EntityTokenHistory::~EntityTokenHistory()
{
}

bool EntityTokenHistory::createTable()
{
    QString sqlString;

    sqlString = "CREATE TABLE token_history( \
                        token_id Text NOT NULL PRIMARY KEY, \
                        serial_no Text NOT NULL, \
                        token_value Real NOT NULL, \
                        applied_date Integer NOT NULL, \
                        row_index NOT NULL, \
                        CONSTRAINT unique_token_id UNIQUE ( token_id ))";

    return executeQuery(sqlString);
}

bool EntityTokenHistory::createTokenHistory(const TokenHistory &rec)
{
    QString sqlString;

    sqlString = "insert into token_history (token_id, serial_no, token_value, applied_date, row_index) values('token_id__', 'serial_no__', token_value__, applied_date__, row_index__)";

    sqlString = sqlString.replace("token_id__", rec.TokenId_);
    sqlString = sqlString.replace("serial_no__", rec.SerialNo_);
    sqlString = sqlString.replace("token_value__", QVariant(rec.TokenValue_).toString());
    sqlString = sqlString.replace("applied_date__", QVariant(rec.AppliedDate_).toString());
    sqlString = sqlString.replace("row_index__", QVariant(rec.RowIndex_).toString());

    return executeQuery(sqlString);
}

bool EntityTokenHistory::selectedTokenHistories(QList<TokenHistory> &list, QString keyname, QVariant value)
{
    QString str;

    QList<QSqlRecord> *reclist = getSelectedRecords(keyname, value, str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        TokenHistory struc;

        struc.TokenId_ = rec.value(0).toString();
        struc.SerialNo_ = rec.value(1).toString();
        struc.TokenValue_ = rec.value(2).toReal();
        struc.AppliedDate_ = rec.value(3).toInt();
        struc.AppliedDateStr_ = QDateTime::fromSecsSinceEpoch(struc.AppliedDate_).toString("MMMM dd yyyy");
        struc.RowIndex_ = rec.value(4).toInt();

        list.append(struc);
    }

    return true;
}

bool EntityTokenHistory::getTokenHistoryRanges(QString serial_no, uint32_t &startRowIndex, uint32_t &endRowIndex)
{
    QList<QSqlRecord> *reclist = nullptr;
    QString sqlstart = "SELECT row_index FROM token_history where serial_no = '_serial_no_'  and row_index > 0 ORDER BY row_index ASC LIMIT 1";
    QString sqlend = "SELECT row_index FROM token_history where serial_no = '_serial_no_'  and row_index > 0 ORDER BY row_index DESC LIMIT 1";

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

bool EntityTokenHistory::getTokenHistoryStartIndex(QString serial_no, uint32_t &startRowIndex)
{
    QList<QSqlRecord> *reclist = nullptr;
    QString sql = "SELECT row_index FROM token_history where serial_no = '_serial_no_' and row_index > 0 ORDER BY record_timestamp DESC LIMIT 1";

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



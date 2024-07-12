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
    QString sqlString;

    sqlString = "CREATE TABLE token( \
                        token_id Text NOT NULL PRIMARY KEY, \
                        serial_no Text NOT NULL, \
                        token_value Real NOT NULL, \
                        is_token_new Integer NOT NULL, \
                        token_status_description Text NOT NULL, \
                        CONSTRAINT unique_token_id UNIQUE ( token_id ))";

    return executeQuery(sqlString);
}

bool EntityToken::createToken(const Token &rec)
{
    QString sqlString;

    sqlString = "insert into token (token_id, serial_no, token_value, is_token_new, token_status_description) values('token_id__', 'serial_no__', token_value__, is_token_new__, 'token_status_description__')";

    sqlString = sqlString.replace("token_id__", rec.TokenId_);
    sqlString = sqlString.replace("serial_no__", rec.SerialNo_);
    sqlString = sqlString.replace("token_value__", QVariant(rec.TokenValue_).toString());
    sqlString = sqlString.replace("is_token_new__", QVariant(rec.IsTokenNew_).toString());
    sqlString = sqlString.replace("token_status_description__", rec.TokenStatusStr_);

    return executeQuery(sqlString);
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
        struc.IsTokenNew_ = rec.value(3).toInt();
        struc.TokenStatusStr_ = rec.value(4).toString();

        list.append(struc);
    }

    return true;
}


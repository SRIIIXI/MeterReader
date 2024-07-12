#include "EntityUserInfo.hpp"

EntityUserInfo* userinfoEntityPtr = nullptr;

EntityUserInfo::EntityUserInfo(QSqlDatabase *db, QString tabname) : DataEntity(db, tabname)
{
    userinfoEntityPtr = this;
}

EntityUserInfo::~EntityUserInfo()
{
}

bool EntityUserInfo::createTable()
{
    QString sqlString = "CREATE TABLE user_info( \
                        user_id Text NOT NULL PRIMARY KEY, \
                        consumer_id Text NOT NULL, \
                        passcode1 Text NOT NULL, \
                        passcode2 Text NOT NULL, \
                        first_login Integer NOT NULL, \
                        CONSTRAINT unique_user_id UNIQUE ( user_id ), \
                        CONSTRAINT unique_consumer_id UNIQUE ( consumer_id ) )";

    return executeQuery(sqlString);
}

bool EntityUserInfo::createUserInfo(const UserInfo &rec)
{
    QString sqlString = "insert into user_info (user_id, consumer_id, passcode1, passcode2, first_login) \
                    values('user_id__', 'consumer_id__', 'passcode1__', 'passcode2__', first_login__)";

    sqlString = sqlString.replace("user_id__", rec.UserId_);
    sqlString = sqlString.replace("consumer_id__", rec.ConsumerId_);
    sqlString = sqlString.replace("passcode1__", rec.PassCode1_);
    sqlString = sqlString.replace("passcode2__", rec.PassCode2_);
    sqlString = sqlString.replace("first_login__", QVariant(rec.IsFirstLogin_).toString());

    return executeQuery(sqlString);
}

bool EntityUserInfo::allUserInfo(QList<UserInfo> &list)
{
    QString str;
    QList<QSqlRecord> *reclist = getAllRecords(str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        UserInfo struc;

        struc.UserId_ = rec.value(0).toString();
        struc.ConsumerId_ = rec.value(1).toString();
        struc.PassCode1_ = rec.value(2).toString();
        struc.PassCode2_ = rec.value(3).toString();
        struc.IsFirstLogin_ = rec.value(4).toInt();

        list.append(struc);
    }

    return true;
}

bool EntityUserInfo::selectedUserInfo(QList<UserInfo> &list, QString keyname, QVariant value)
{
    QString str;
    QList<QSqlRecord> *reclist = getSelectedRecords(keyname, value, str);

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        UserInfo struc;

        struc.UserId_ = rec.value(0).toString();
        struc.ConsumerId_ = rec.value(1).toString();
        struc.PassCode1_ = rec.value(2).toString();
        struc.PassCode2_ = rec.value(3).toString();
        struc.IsFirstLogin_ = rec.value(4).toInt();

        list.append(struc);
    }

    return true;
}

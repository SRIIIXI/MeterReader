#ifndef ENTITY_USER_INFO
#define ENTITY_USER_INFO

#include <QObject>
#include <QtSql>
#include "DataEntity.hpp"


class UserInfo
{
public:
    Q_GADGET

public:
    bool IsFirstLogin_;
public:
    QString UserId_;
public:
    QString ConsumerId_;
public:
    QString PassCode1_;
public:
    QString PassCode2_;
public:
    Q_PROPERTY(bool IsFirstLogin MEMBER IsFirstLogin_)
    Q_PROPERTY(QString UserId MEMBER UserId_)
    Q_PROPERTY(QString ConsumerId MEMBER ConsumerId_)
    Q_PROPERTY(QString PassCode1 MEMBER PassCode1_)
    Q_PROPERTY(QString PassCode2 MEMBER PassCode2_)

public:
    ~UserInfo()
    {

    }

    UserInfo()
    {

    }

    UserInfo(bool iIsFirstLogin, QString iUserId, QString iConsumerId, QString iPassCode1, QString iPassCode2)
    {
        IsFirstLogin_ = iIsFirstLogin;
        UserId_ = iUserId;
        ConsumerId_ = iConsumerId;
        PassCode1_ = iPassCode1;
        PassCode2_ = iPassCode2;
    }

    UserInfo(const UserInfo& other)
    {
        IsFirstLogin_ = other.IsFirstLogin_;
        UserId_ = other.UserId_;
        ConsumerId_ = other.ConsumerId_;
        PassCode1_ = other.PassCode1_;
        PassCode2_ = other.PassCode2_;
    }

    UserInfo& operator = (const UserInfo& other)
    {
        IsFirstLogin_ = other.IsFirstLogin_;
        UserId_ = other.UserId_;
        ConsumerId_ = other.ConsumerId_;
        PassCode1_ = other.PassCode1_;
        PassCode2_ = other.PassCode2_;
        return *this;
    }
};

class EntityUserInfo : public DataEntity
{
public:
    EntityUserInfo(QSqlDatabase* db, QString tabname);
    virtual ~EntityUserInfo();
    bool createTable();
    bool createUserInfo(const UserInfo &rec);
    bool allUserInfo(QList<UserInfo> &list);
    bool selectedUserInfo(QList<UserInfo> &list, QString keyname, QVariant value);
};

extern EntityUserInfo* userinfoEntityPtr;

#endif

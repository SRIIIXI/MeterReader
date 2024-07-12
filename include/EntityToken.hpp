#ifndef ENTITY_TOKEN
#define ENTITY_TOKEN

#include <QObject>
#include <QtSql>
#include "DataEntity.hpp"

class Token
{
public:
    Q_GADGET

public:
    QString SerialNo_;
public:
    QString TokenId_;
public:
    float TokenValue_;
public:
    bool IsTokenNew_;
public:
    QString TokenStatusStr_;

public:
    Q_PROPERTY(QString SerialNo MEMBER SerialNo_)
    Q_PROPERTY(QString TokenId MEMBER TokenId_)
    Q_PROPERTY(float TokenValue MEMBER TokenValue_)
    Q_PROPERTY(bool IsTokenNew MEMBER IsTokenNew_)
    Q_PROPERTY(QString TokenStatusStr MEMBER TokenStatusStr_)

public:
    ~Token()
    {

    }

    Token()
    {

    }

    Token(QString iSerialNo, QString iTokenId, float iTokenValue, bool iIsTokenNew, QString iStatus)
    {
        SerialNo_ = iSerialNo;
        TokenId_ = iTokenId;
        TokenValue_ = iTokenValue;
        IsTokenNew_ = iIsTokenNew;
        TokenStatusStr_ = iStatus;
    }

    Token(const Token& other)
    {
        SerialNo_ = other.SerialNo_;
        TokenId_ = other.TokenId_;
        TokenValue_ = other.TokenValue_;
        IsTokenNew_ = other.IsTokenNew_;
        TokenStatusStr_ = other.TokenStatusStr_;
    }

    Token& operator = (const Token& other)
    {
        SerialNo_ = other.SerialNo_;
        TokenId_ = other.TokenId_;
        TokenValue_ = other.TokenValue_;
        IsTokenNew_ = other.IsTokenNew_;
        TokenStatusStr_ = other.TokenStatusStr_;
        return *this;
    }
};

class EntityToken : public DataEntity
{
public:
    EntityToken(QSqlDatabase* db, QString tabname);
    virtual ~EntityToken();
    bool createTable();
    bool createToken(const Token &rec);
    bool selectedTokens(QList<Token> &list, QString keyname, QVariant value);
};

extern EntityToken* tokenEntityPtr;

#endif

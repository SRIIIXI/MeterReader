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
    bool IsApplied_;
public:
    QString SerialNo_;
public:
    uint32_t AppliedDate_;
public:
    QString AppliedDateStr_;
public:
    QString TokenId_;
public:
    float TokenValue_;
public:
    bool IsInValid_;
public:
    uint32_t TokenStatus_;
public:
    QString TokenStatusText_;
public:
    uint32_t RowIndex_;

public:
    Q_PROPERTY(bool IsApplied MEMBER IsApplied_)
    Q_PROPERTY(QString SerialNo MEMBER SerialNo_)
    Q_PROPERTY(uint32_t AppliedDate MEMBER AppliedDate_)
    Q_PROPERTY(QString AppliedDateStr MEMBER AppliedDateStr_)
    Q_PROPERTY(QString TokenId MEMBER TokenId_)
    Q_PROPERTY(float TokenValue MEMBER TokenValue_)
    Q_PROPERTY(bool IsInValid MEMBER IsInValid_)
    Q_PROPERTY(uint32_t TokenStatus MEMBER TokenStatus_)
    Q_PROPERTY(QString TokenStatusText MEMBER TokenStatusText_)
    Q_PROPERTY(uint32_t  RowIndex MEMBER RowIndex_)

public:
    ~Token()
    {

    }

    Token()
    {

    }

    Token(bool iIsApplied, QString iSerialNo, uint32_t iAppliedDate, QString iTokenId, float iTokenValue,  uint32_t iTokenStatus, QString iTokenStatusText, uint32_t iRowIndex)
    {
        SerialNo_ = iSerialNo;
        IsApplied_ = iIsApplied;
        AppliedDate_ = iAppliedDate;
        TokenId_ = iTokenId;
        TokenValue_ = iTokenValue;
        AppliedDateStr_ = QDateTime::fromSecsSinceEpoch(AppliedDate_).toString("MMMM dd yyyy");
        RowIndex_ = iRowIndex;
        TokenStatus_ = iTokenStatus;

        if(IsApplied_ && TokenValue_ == 0)
        {
            IsInValid_ = true;
        }
        else
        {
            IsInValid_ = false;
        }

        TokenStatusText_ = iTokenStatusText;
    }

    Token(const Token& other)
    {
        SerialNo_ = other.SerialNo_;
        IsApplied_ = other.IsApplied_;
        AppliedDate_ = other.AppliedDate_;
        TokenId_ = other.TokenId_;
        TokenValue_ = other.TokenValue_;
        AppliedDateStr_ = QDateTime::fromSecsSinceEpoch(AppliedDate_).toString("MMMM dd yyyy");
        RowIndex_ = other.RowIndex_;

        if(IsApplied_ && TokenValue_ == 0)
        {
            IsInValid_ = true;
        }
        else
        {
            IsInValid_ = false;
        }

        TokenStatusText_ = other.TokenStatusText_;
        TokenStatus_ = other.TokenStatus_;
    }

    Token& operator = (const Token& other)
    {
        SerialNo_ = other.SerialNo_;
        IsApplied_ = other.IsApplied_;
        AppliedDate_ = other.AppliedDate_;
        TokenId_ = other.TokenId_;
        TokenValue_ = other.TokenValue_;
        AppliedDateStr_ = QDateTime::fromSecsSinceEpoch(AppliedDate_).toString("MMMM dd yyyy");
        RowIndex_ = other.RowIndex_;

        if(IsApplied_ && TokenValue_ == 0)
        {
            IsInValid_ = true;
        }
        else
        {
            IsInValid_ = false;
        }

        TokenStatusText_ = other.TokenStatusText_;
        TokenStatus_ = other.TokenStatus_;

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
    bool allTokens(QList<Token> &list);
    bool selectedTokens(QList<Token> &list, QString keyname, QVariant value);
    bool getRanges(QString serial_no,uint32_t &startRowIndex, uint32_t &endRowIndex);
    bool getStartIndex(QString serial_no,uint32_t &startRowIndex);
};

extern EntityToken* tokenEntityPtr;

#endif

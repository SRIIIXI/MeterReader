#ifndef ENTITY_TOKEN_HISTORY
#define ENTITY_TOKEN_HISTORY

#include <QObject>
#include <QtSql>
#include "DataEntity.hpp"

class TokenHistory
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
    uint32_t AppliedDate_;
public:
    QString AppliedDateStr_;
public:
    int32_t RowIndex_;

public:
    Q_PROPERTY(QString SerialNo MEMBER SerialNo_)
    Q_PROPERTY(QString TokenId MEMBER TokenId_)
    Q_PROPERTY(float TokenValue MEMBER TokenValue_)
    Q_PROPERTY(uint32_t AppliedDate MEMBER AppliedDate_)
    Q_PROPERTY(QString AppliedDateStr MEMBER AppliedDateStr_)
    Q_PROPERTY(int32_t  RowIndex MEMBER RowIndex_)

public:
    ~TokenHistory()
    {

    }

    TokenHistory()
    {

    }

    TokenHistory(QString iSerialNo, QString iTokenId, float iTokenValue, uint32_t iAppliedDate, int32_t iRowIndex)
    {
        SerialNo_ = iSerialNo;
        TokenId_ = iTokenId;
        TokenValue_ = iTokenValue;
        AppliedDate_ = iAppliedDate;
        AppliedDateStr_ = QDateTime::fromSecsSinceEpoch(AppliedDate_).toString("MMMM dd yyyy");
        RowIndex_ = iRowIndex;
    }

    TokenHistory(const TokenHistory& other)
    {
        SerialNo_ = other.SerialNo_;
        AppliedDate_ = other.AppliedDate_;
        TokenId_ = other.TokenId_;
        TokenValue_ = other.TokenValue_;
        AppliedDateStr_ = QDateTime::fromSecsSinceEpoch(AppliedDate_).toString("MMMM dd yyyy");
        RowIndex_ = other.RowIndex_;
    }

    TokenHistory& operator = (const TokenHistory& other)
    {
        SerialNo_ = other.SerialNo_;
        AppliedDate_ = other.AppliedDate_;
        TokenId_ = other.TokenId_;
        TokenValue_ = other.TokenValue_;
        AppliedDateStr_ = QDateTime::fromSecsSinceEpoch(AppliedDate_).toString("MMMM dd yyyy");
        RowIndex_ = other.RowIndex_;
        return *this;
    }
};

class EntityTokenHistory : public DataEntity
{
public:
    EntityTokenHistory(QSqlDatabase* db, QString tabname);
    virtual ~EntityTokenHistory();
    bool createTable();
    bool createTokenHistory(const TokenHistory &rec);
    bool selectedTokenHistories(QList<TokenHistory> &list, QString keyname, QVariant value);
    bool getTokenHistoryRanges(QString serial_no,uint32_t &startRowIndex, uint32_t &endRowIndex);
    bool getTokenHistoryStartIndex(QString serial_no,uint32_t &startRowIndex);
};

extern EntityTokenHistory* tokenHistoryEntityPtr;

#endif

#ifndef I_WORKFLOW
#define I_WORKFLOW

#include <QObject>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QByteArray>
#include "DlmsClient.hpp"
#include "Logger.hpp"

typedef enum ActiveWorkflow
{
    AddMeter,
    ConnectMeter,
    DisconnectMeter,
    KeepAlive,
    TokenTransfer,
    Idle
}ActiveWorkflow;

class IWorkFlow : public QObject
{
    Q_OBJECT
public:
    IWorkFlow(QObject* parent=nullptr) : QObject(parent)
    {
        protocolState = 0;
        clientPublic = nullptr;
        clientPairing = nullptr;
        clientSecured = nullptr;
        secured_client_index = 103;

        old_deliveredEnergy_ = 0;
        old_instantaneousPower_ = 0;
        old_usedCredit_ = 0;
        old_grossCredit_ = 0;
        wkf_currency_scalar = 0;
    }

    virtual void StartWorkFlow(QList<QVariant> startParamters) = 0;
    virtual void ProcessSendingFrame() = 0;
    virtual void ProcessReceivedFrame(const QByteArray &buffer) = 0;

    void ProcessRetrySendingFrame()
    {
        protocolState--;
        ProcessSendingFrame();
    }

    void AddToProfileGenericColumns(QString colnames)
    {
        profileGenericColums.clear();
        colnames = colnames.remove('[');
        colnames = colnames.remove(']');
        QList<QString> templist = colnames.split(',', Qt::SkipEmptyParts);

        foreach(QString str, templist)
        {
            profileGenericColums.append(str.trimmed());
        }
    }

    int32_t FindProfileGenericColumn(QString colname)
    {
        if(profileGenericColums.contains(colname))
        {
            return profileGenericColums.indexOf(colname);
        }

        return -1;
    }

    QList<int32_t> FindProfileGenericColumns(QString colname)
    {
        QList<int32_t> ret;

        for(int32_t ctr = 0; ctr < profileGenericColums.count(); ctr++)
        {
            if(colname.trimmed() == profileGenericColums.at(ctr).trimmed())
            {
                ret.append(ctr);
            }
        }

        return ret;
    }

    void TraceLastTx()
    {
        Logger::GetInstance()->write(lastTx, LogInfo);
    }

    void TraceLastRx()
    {
        Logger::GetInstance()->write(lastRx, LogInfo);
    }

    void SaveRx(const QByteArray& buffer)
    {
        lastRx = "RX: " + Helpers::BytesToHex((unsigned char*)buffer.data(), buffer.length());
    }

    void SaveTx(const QByteArray& buffer)
    {
        lastTx = "TX: " + Helpers::BytesToHex((unsigned char*)buffer.data(), buffer.length());
    }

    static long secured_client_index;

protected:
    unsigned int protocolState;
    DlmsClient* clientPublic;
    DlmsClient* clientPairing;
    DlmsClient* clientSecured;
    ByteBuffer authKey;
    ByteBuffer cipherKey;
    QString metername;
    ByteBuffer systemtitle;
    QList<QString> profileGenericColums;

    std::string lastTx;
    std::string lastRx;

    float old_deliveredEnergy_;
    float old_instantaneousPower_;
    float old_usedCredit_;
    float old_grossCredit_;
    float old_remainingCredit_;
    int wkf_currency_scalar;

};


#endif

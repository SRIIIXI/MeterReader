#ifndef I_WORKFLOW
#define I_WORKFLOW

#include <QObject>
#include <QList>
#include <QVariant>
#include <QByteArray>
#include <QDebug>
#include "CommunicationClient.hpp"
#include "BleCentral.hpp"

typedef enum WorkflowName
{
    Addmeter,
    ConnectMeter,
    DisconnectMeter,
    TokenTransfer,
    KeepAlive
}WorkflowName;

class IWorkFlow : public QObject
{
    Q_OBJECT
public:
    IWorkFlow(QObject* parent=nullptr) : QObject(parent)
    {
        protocolState = 0;
        centralPtr = nullptr;
        clientPublic = nullptr;
        clientPairing = nullptr;
        clientSecured = nullptr;
    }

    virtual void StartWorkFlow(QList<QVariant> startParamters) = 0;
    virtual void ProcessSendingFrame() = 0;
    virtual void ProcessReceivedFrame(const QByteArray &buffer) = 0;

    void SetCentral(BleCentral* ctrlref)
    {
        centralPtr = ctrlref;
    }

signals:
    void Completed(QList<QVariant> completionParamters, WorkflowName wfn, bool status);

protected:
    unsigned int protocolState;
    BleCentral* centralPtr;
    CommunicationClient* clientPublic;
    CommunicationClient* clientPairing;
    CommunicationClient* clientSecured;
    QList<QVariant> completionParamters;
    ByteBuffer authKey;
    ByteBuffer cipherKey;
    QString serialno;
    QString metername;
    ByteBuffer systemtitle;
    bool is_disconnect_request;
};


#endif

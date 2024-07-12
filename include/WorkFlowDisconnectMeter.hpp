#ifndef WORKFLOW_DISCONNECT_METER
#define WORKFLOW_DISCONNECT_METER

#include "IWorkFlow.hpp"

class WorkFlowDisconnectMeter : public IWorkFlow
{
    Q_OBJECT
public:
    WorkFlowDisconnectMeter(QObject* parent = nullptr);
    void StartWorkFlow(QList<QVariant> startParamters);
    void ProcessSendingFrame();
    void ProcessReceivedFrame(const QByteArray &buffer);
signals:
    void DisconnectCompleted(QString &serialNo, bool status);
private:
    long CM_FRAME_COUNTER_SECURED;
    std::string temp;
    QString keys;
    char auth[48];
    char ciph[48];
    int scalar_temp;
    bool has_error;
};

#endif

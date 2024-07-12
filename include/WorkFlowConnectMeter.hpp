#ifndef WORKFLOW_CONNECT_METER
#define WORKFLOW_CONNECT_METER

#include "IWorkFlow.hpp"

class WorkFlowConnectMeter : public IWorkFlow
{
    Q_OBJECT
public:
    WorkFlowConnectMeter(QObject* parent = nullptr);
    void StartWorkFlow(QList<QVariant> startParamters);
    void ProcessSendingFrame();
    void ProcessReceivedFrame(const QByteArray &buffer);
signals:
    void ConnectCompleted(QString &serialNo, bool status);
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

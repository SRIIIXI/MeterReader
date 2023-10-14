#ifndef WORKFLOW_CONNECT_METER
#define WORKFLOW_CONNECT_METER

#include "IWorkFlow.hpp"

class WorkFlowConnectMeter : public IWorkFlow
{
public:
    WorkFlowConnectMeter(QObject* parent = nullptr);
    void StartWorkFlow(QList<QVariant> startParamters);
    void ProcessSendingFrame();
    void ProcessReceivedFrame(const QByteArray &buffer);
private:
    long CM_FRAME_COUNTER_SECURED_103;
    std::string temp;
    QString keys;
    char auth[48];
    char ciph[48];
    int scalar_temp;
    long secured_client_index;
    bool has_error;
};

#endif

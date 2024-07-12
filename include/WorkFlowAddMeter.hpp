#ifndef WORKFLOW_ADD_METER
#define WORKFLOW_ADD_METER

#include "IWorkFlow.hpp"

class WorkFlowAddMeter : public IWorkFlow
{
    Q_OBJECT
public:
    WorkFlowAddMeter(QObject* parent = nullptr);
    void StartWorkFlow(QList<QVariant> startParamters);
    void ProcessSendingFrame();
    void ProcessReceivedFrame(const QByteArray &buffer);
signals:
    void AddMeterCompleted(QString &serialNo, bool status);
private:
    long AM_FRAME_COUNTER_PAIRING;
    long AM_FRAME_COUNTER_SECURED_103;
    long AM_FRAME_COUNTER_SECURED_104;
    long AM_FRAME_COUNTER_SECURED_105;
    long secured_client_index;
    bool has_error;
};

#endif

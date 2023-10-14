#ifndef WORKFLOW_TOKEN_TRANSFER
#define WORKFLOW_TOKEN_TRANSFER

#include "IWorkFlow.hpp"

class WorkFlowTokenTransfer : public IWorkFlow
{
public:
    WorkFlowTokenTransfer(QObject* parent = nullptr);
    void StartWorkFlow(QList<QVariant> startParamters);
    void ProcessSendingFrame();
    void ProcessReceivedFrame(const QByteArray &buffer);
private:
    QString tokenStr;
    uint32_t currentIndex;
    long capturedTokenColumnCount;
    int token_status;
};

#endif

#ifndef WORKFLOW_KEEP_ALIVE
#define WORKFLOW_KEEP_ALIVE

#include "IWorkFlow.hpp"

class WorkFlowKeepAlive : public IWorkFlow
{
public:
    WorkFlowKeepAlive(QObject* parent = nullptr);
    void StartWorkFlow(QList<QVariant> startParamters);
    void ProcessSendingFrame();
    void ProcessReceivedFrame(const QByteArray &buffer);
private:
    char* int2bin(int a, char* buffer, int buf_size);
    char* strreverse(char* ptr);
    QString alarmString;
    uint32_t currentIndex;
    long capturedTokenColumnCount;
};

#endif

#include "WorkFlowDisconnectMeter.hpp"
#include "Controller.hpp"
#include "Mk7Mi.hpp"

const int DM_SECURED_REL_SEND = 1;
const int DM_SECURED_REL_RECV = 2;
const int DM_SECURED_DISCN_SEND = 3;
const int DM_SECURED_DISCN_RECV = 4;

const int DM_COMPLETE = 5;

///////////////////////////////////////

QString serial_no;

WorkFlowDisconnectMeter::WorkFlowDisconnectMeter(QObject* parent): IWorkFlow(parent)
{
    CM_FRAME_COUNTER_SECURED = 0;
}

void WorkFlowDisconnectMeter::StartWorkFlow(QList<QVariant> startParamters)
{
    has_error = false;
    scalar_temp = 0;
    if(startParamters.count() < 1)
    {
        return;
    }
    else
    {
        serial_no = startParamters.at(0).toString();
    }

    //Retreive the secured client 103 and the public client
    clientSecured = DlmsClient::GetClient(secured_client_index);

    protocolState = DM_SECURED_REL_SEND;

    emit controllerInstance->progressIndicatorsOn();

    ProcessSendingFrame();
}

void WorkFlowDisconnectMeter::ProcessSendingFrame()
{
    QByteArray buffer;

    switch(protocolState)
    {
        case DM_SECURED_REL_SEND:
        {
            clientSecured->CreateReleaseFrame(buffer);
            emit controllerInstance->workflowProgress(30);
            break;
        }

        case DM_SECURED_DISCN_SEND:
        {
            clientSecured->CreateDisconnectFrame(buffer);
            emit controllerInstance->workflowProgress(65);
            break;
        }

        case DM_COMPLETE:
        {
            emit controllerInstance->workflowProgress(100);
            break;
        }

        default:
        {
            controllerInstance->Trace("Tx Disconnect Meter: Unhandled Protocol State : " + QVariant((uint32_t)protocolState).toString());
            break;
        }
    }

    if(protocolState == DM_COMPLETE)
    {
        emit DisconnectCompleted(serial_no, !has_error);
        return;
    }

    if(buffer.length() > 0)
    {
        btInterfacePtr->Send(buffer);
        protocolState++;
    }
    else
    {
        controllerInstance->Trace("Tx Disconnect Meter: Protocol Buffer Not Ready : " + QVariant((uint32_t)protocolState).toString());
    }
}

void WorkFlowDisconnectMeter::ProcessReceivedFrame(const QByteArray &buffer)
{
    bool ispartial = false;
    bool islast = false;
    float responseObject = -1;
    DLMSAccount accountObject;
    bool isfound = false;

    switch(protocolState)
    {
        case DM_SECURED_REL_RECV:
        {
            if(!clientSecured->CheckReleaseResponse(buffer))
            {
                has_error = true;
            }
            else
            {
                protocolState = DM_SECURED_DISCN_SEND - 1;
            }
            break;
        }

        case DM_SECURED_DISCN_RECV:
        {
            protocolState = DM_COMPLETE - 1;
            break;
        }

        default:
        {
            controllerInstance->Trace("Rx Disconnect Meter: Unhandled Protocol State : " + QVariant((uint32_t)protocolState).toString());
            break;
        }
    }

    if(protocolState == DM_COMPLETE)
    {
        return;
    }

    if(!has_error)
    {
        protocolState++;
    }
    else
    {
        controllerInstance->Trace(clientPublic->ErrorMessage() + QString(" ") + QVariant((uint32_t)protocolState).toString());
        protocolState = DM_COMPLETE;
    }

    ProcessSendingFrame();
}

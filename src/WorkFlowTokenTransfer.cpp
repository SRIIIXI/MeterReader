#include "WorkFlowTokenTransfer.hpp"
#include "EntityMeterSettings.hpp"
#include "Controller.hpp"
#include "Mk7Mi.hpp"

const int TT_TRANSFER_TOKEN_SEND = 1;
const int TT_TRANSFER_TOKEN_RECV = 2;

const int TT_TOKEN_STATUS_SEND = 3;
const int TT_TOKEN_STATUS_RECV = 4;

const int TT_COMPLETE = 5;

///////////////////////////////////////

int tt_currency_scalar = 0;

const char* token_status_messages[] =
{
    "Format Ok",
    "Authentication Ok",
    "Validation Ok",
    "Execution Ok",
    "Format Failure",
    "Authentication Failure",
    "Validation Result Failure",
    "Execution Result Failure",
    "Received",
    "Not applied",
    "Transaction Failed",
    "Status Awaited"
};

QString token_statusStr;

WorkFlowTokenTransfer::WorkFlowTokenTransfer(QObject* parent): IWorkFlow(parent)
{
}

void WorkFlowTokenTransfer::StartWorkFlow(QList<QVariant> startParamters)
{
    currentIndex = 0;
    capturedTokenColumnCount = 0;

    tokenStr = startParamters.at(0).toString();
    token_status = 0;

    clientSecured = DlmsClient::GetClient(secured_client_index);

    if(clientSecured == NULL)
    {
        controllerInstance->Trace("Client connection is NULL");
        token_statusStr = "Transaction Failed";
        emit TokenTransferCompleted(controllerInstance->currentMeterSerialNo_, tokenStr, token_statusStr, false);
        return;
    }

    char str[256] = {0};
    snprintf(str, 255, "STS Transfer using client ID %d", clientSecured->GetClientId());
    controllerInstance->Trace(str);

    protocolState = TT_TRANSFER_TOKEN_SEND;

    QList<MeterSettings> msettingslist;
    meterSettingsEntityPtr->selectedMetersSettings(msettingslist, "serial_no", controllerInstance->currentMeterSerialNo_);

    for(int i = 0; i < msettingslist.count(); i++)
    {
        switch(msettingslist.at(i).Key_)
        {

        case 3:
        {
            tt_currency_scalar = msettingslist.at(i).Value_.toInt();
            break;
        }

        default:
            break;
        }
    }

    ProcessSendingFrame();
}

void WorkFlowTokenTransfer::ProcessSendingFrame()
{
    QByteArray buffer;

    switch(protocolState)
    {
        case TT_TRANSFER_TOKEN_SEND:
        {
            controllerInstance->Trace("Applying token " + tokenStr);
            tokenEntityPtr->update("token_id", tokenStr, "is_token_new", false);
            clientSecured->CreateTokenTransferRequestFrame(buffer, OBIS_TOKEN_GATEWAY, tokenStr);
            emit controllerInstance->workflowProgress(30);
            break;
        }

        case TT_TOKEN_STATUS_SEND:
        {
            clientSecured->CreateTokenTransferStatusRequestFrame(buffer, OBIS_TOKEN_GATEWAY);
            emit controllerInstance->workflowProgress(40);
            controllerInstance->Trace("Querying token status");
            break;
        }

        case TT_COMPLETE:
        {
            emit controllerInstance->workflowProgress(100);
            break;
        }

        default:
        {
            controllerInstance->Trace("Token Transfer: Unhandled Protocol State : " + QVariant((uint32_t)protocolState).toString());
            break;
        }
    }

    if(protocolState == TT_COMPLETE)
    {
        token_statusStr = token_status_messages[token_status];
        if(token_status > 3 && token_status != 8)
        {
            //Token was transferred but the token was not accepted
            emit TokenTransferCompleted(controllerInstance->currentMeterSerialNo_, tokenStr, token_statusStr, false);
        }
        else
        {
            emit TokenTransferCompleted(controllerInstance->currentMeterSerialNo_, tokenStr, token_statusStr, true);
        }
        return;
    }

    if(buffer.length() > 0)
    {
        btInterfacePtr->Send(buffer);
        protocolState++;
    }
    else
    {
        controllerInstance->Trace("Token Transfer: Protocol Buffer Not Ready : " + QVariant((uint32_t)protocolState).toString());
    }
}

void WorkFlowTokenTransfer::ProcessReceivedFrame(const QByteArray &buffer)
{
    bool has_error = false;

    DLMSVariant responseObject;
    bool ispartial = false;
    bool islast = false;

    switch(protocolState)
    {
        case TT_TRANSFER_TOKEN_RECV:
        {
            bool istransferred = false;
            DLMSTokenGateway gatewayObject;

            if(!clientSecured->CheckTokenTransferResponseFrame(buffer, OBIS_TOKEN_GATEWAY, gatewayObject, istransferred))
            {
                has_error = true;
                token_status = 9;
                tokenEntityPtr->update("token_id", tokenStr, "token_status_description", "Transaction failed");
            }
            else
            {
                token_status = 11;
                controllerInstance->Trace("Token applied");
            }

            emit controllerInstance->tokenTransferState(!has_error, QString(token_status_messages[token_status]));

            break;
        }

        case TT_TOKEN_STATUS_RECV:
        {
            bool isfound = false;
            DLMSTokenGateway gatewayObject;

            if(!clientSecured->CheckTokenTransferStatusResponseFrame(buffer, OBIS_TOKEN_GATEWAY, gatewayObject))
            {
                //Ignore - This could be a transient problem
                token_status = 9;
                tokenEntityPtr->update("token_id", tokenStr, "token_status_description", "Status Awaited");
            }
            else
            {
                token_status = gatewayObject.GetStatusCode();
                controllerInstance->Trace("Token status received : " + QString(token_status_messages[token_status]));

                if(token_status > 3)
                {
                    tokenEntityPtr->remove("token_id", tokenStr);
                }
                else
                {
                    tokenEntityPtr->update("token_id", tokenStr, "token_status_description", QString(token_status_messages[token_status]));
                }
            }

            emit controllerInstance->tokenTransferState(!has_error, QString(token_status_messages[token_status]));

            protocolState =  TT_COMPLETE - 1;

            break;
        }

        default:
            controllerInstance->Trace("Token Transfer: Unhandled Protocol State : " + QVariant((uint32_t)protocolState).toString());
            break;
    }

    if(protocolState == TT_COMPLETE)
    {
        return;
    }

    if(!has_error)
    {
        protocolState++;
        ProcessSendingFrame();
    }
    else
    {
        controllerInstance->Trace(clientSecured->ErrorMessage() + QString(" ") + QVariant((uint32_t)protocolState).toString());
        protocolState = TT_COMPLETE;
        token_statusStr = token_status_messages[token_status];
        emit TokenTransferCompleted(controllerInstance->currentMeterSerialNo_, tokenStr, token_statusStr, true);
    }
}

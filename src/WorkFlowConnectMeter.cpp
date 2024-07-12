#include "WorkFlowConnectMeter.hpp"
#include "Controller.hpp"
#include "Mk7Mi.hpp"

const int CM_PUBLIC_SNRM_SEND = 1;
const int CM_PUBLIC_UARS_RECV = 2;
const int CM_PUBLIC_AARQ_SEND = 3;
const int CM_PUBLIC_AARE_RECV = 4;
const int CM_PUBLIC_FCTR_SECURED103_SEND = 5;
const int CM_PUBLIC_FCTR_SECURED103_RECV = 6;
const int CM_PUBLIC_REL_SEND = 7;
const int CM_PUBLIC_REL_RECV = 8;
const int CM_PUBLIC_DISCN_SEND = 9;
const int CM_PUBLIC_DISCN_RECV = 10;

const int CM_SECURED_SNRM_SEND = 11;
const int CM_SECURED_UARS_RECV = 12;
const int CM_SECURED_AARQ_SEND = 13;
const int CM_SECURED_AARE_RECV = 14;
const int CM_SECURED_AUTH_SEND = 15;
const int CM_SECURED_AUTH_RECV = 16;

const int CM_DELIVERED_ACTIVE_ENERGY_SEND = 17;
const int CM_DELIVERED_ACTIVE_ENERGY_RECV = 18;

const int CM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SEND = 19;
const int CM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_RECV = 20;

const int CM_ACCOUNT_USED_CREDIT_SEND = 21;
const int CM_ACCOUNT_USED_CREDIT_RECV = 22;

const int CM_ACCOUNT_TOTAL_CREDIT_SEND = 23;
const int CM_ACCOUNT_TOTAL_CREDIT_RECV = 24;

const int CM_ACCOUNT_REMAINING_CREDIT_SEND = 25;
const int CM_ACCOUNT_REMAINING_CREDIT_RECV = 26;

const int CM_COMPLETE = 27;

///////////////////////////////////////

QString connect_meter_serial_no = "";

WorkFlowConnectMeter::WorkFlowConnectMeter(QObject* parent): IWorkFlow(parent)
{
    CM_FRAME_COUNTER_SECURED = 0;
    memset(&auth[0], 0, 48);
    memset(&ciph[0], 0, 48);
    scalar_temp = 0;
    systemtitle.Clear();
}

void WorkFlowConnectMeter::StartWorkFlow(QList<QVariant> startParamters)
{
    has_error = false;
    scalar_temp = 0;

    if(startParamters.count() < 1)
    {
        return;
    }
    else
    {
        systemtitle.Set(startParamters.at(0).toByteArray().data(), startParamters.at(0).toByteArray().size());
    }

    keys = startParamters.at(2).toString();

    memcpy(auth, &keys.toStdString().c_str()[0], 47);
    memcpy(ciph, &keys.toStdString().c_str()[48], 47);

    temp= auth;
    authKey.Clear();
    Helpers::HexToBytes(temp, authKey);

    temp= ciph;
    cipherKey.Clear();
    Helpers::HexToBytes(temp, cipherKey);

    //Retreive the secured client 103 and the public client
    
    clientPublic = DlmsClient::GetClient(CLIENT_PUBLIC);
    secured_client_index = CLIENT_SECURED_103;
    clientSecured = DlmsClient::GetClient(secured_client_index);

    clientPublic->SetSystemTitle(systemtitle.GetData(), 8);
    clientPublic->EnableCustomChallenge(false);
    clientPublic->InitializeClient();

    connect_meter_serial_no = startParamters.at(4).toString();
    protocolState = CM_PUBLIC_SNRM_SEND;

    emit controllerInstance->progressIndicatorsOn();

    old_deliveredEnergy_ = controllerInstance->deliveredEnergy_;
    old_instantaneousPower_ = controllerInstance->instantaneousPower_;
    old_usedCredit_ = controllerInstance->usedCredit_;
    old_grossCredit_ = controllerInstance->grossCredit_;
    old_remainingCredit_ = controllerInstance->remainingCredit_;

    ProcessSendingFrame();
}

void WorkFlowConnectMeter::ProcessSendingFrame()
{
    QByteArray buffer;

    switch(protocolState)
    {
        case CM_PUBLIC_SNRM_SEND:
        {
            clientPublic->CreateSNRMFrame(buffer);
        emit controllerInstance->workflowProgress(5);
            break;
        }

        case CM_PUBLIC_AARQ_SEND:
        {
            clientPublic->CreateAARQFrame(buffer);
            emit controllerInstance->workflowProgress(10);
            break;
        }

        case CM_PUBLIC_FCTR_SECURED103_SEND:
        {
            clientPublic->CreateFrameCounterFrame(buffer, OBIS_FRAME_COUNTER_103);
            emit controllerInstance->workflowProgress(20);
            break;
        }

        case CM_PUBLIC_REL_SEND:
        {
            clientPublic->CreateReleaseFrame(buffer);
            emit controllerInstance->workflowProgress(25);
            break;
        }

        case CM_PUBLIC_DISCN_SEND:
        {
            clientPublic->CreateDisconnectFrame(buffer);
            emit controllerInstance->workflowProgress(30);
            break;
        }

        case CM_SECURED_SNRM_SEND:
        {           
            controllerInstance->Trace("Starting with client " + QVariant((uint32_t)secured_client_index).toString());
            clientSecured->CreateSNRMFrame(buffer);
            emit controllerInstance->workflowProgress(35);
            break;
        }

        case CM_SECURED_AARQ_SEND:
        {
            clientSecured->CreateAARQFrame(buffer);
            emit controllerInstance->workflowProgress(40);
            break;
        }

        case CM_SECURED_AUTH_SEND:
        {
            clientSecured->CreateAuthenticationFrame(buffer);
            emit controllerInstance->workflowProgress(45);
            break;
        }

        case CM_DELIVERED_ACTIVE_ENERGY_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_DELIVERED_ACTIVE_ENERGY);
            emit controllerInstance->workflowProgress(60);
            break;
        }

        case CM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER);
            emit controllerInstance->workflowProgress(65);
            break;
        }

        case CM_ACCOUNT_USED_CREDIT_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_TOTAL_CREDIT_USED);
            emit controllerInstance->workflowProgress(75);
            break;
        }

        case CM_ACCOUNT_TOTAL_CREDIT_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_TOTAL_CREDIT_PURCHASED);
            emit controllerInstance->workflowProgress(80);
            break;
        }

        case CM_ACCOUNT_REMAINING_CREDIT_SEND:
        {
            clientSecured->CreateAccountStatusRequestFrame(buffer, OBIS_ACCOUNT_STATUS, 5);
            emit controllerInstance->workflowProgress(80);
            break;
        }

        case CM_COMPLETE:
        {
            emit controllerInstance->workflowProgress(100);
            break;
        }

        default:
        {
            controllerInstance->Trace("Connect Meter: Unhandled Protocol State : " + QVariant((uint32_t)protocolState).toString());
            break;
        }
    }

    if(protocolState == CM_COMPLETE)
    {
        emit ConnectCompleted(connect_meter_serial_no, !has_error);
        return;
    }

    if(buffer.length() > 0)
    {
        btInterfacePtr->Send(buffer);
        protocolState++;
    }
    else
    {
        controllerInstance->Trace("Connect Meter: Protocol Buffer Not Ready : " + QVariant((uint32_t)protocolState).toString());
    }
}

void WorkFlowConnectMeter::ProcessReceivedFrame(const QByteArray &buffer)
{
    bool ispartial = false;
    bool islast = false;
    float responseObject = -1;
    DLMSAccount accountObject;
    bool isfound = false;

    switch(protocolState)
    {
        case CM_PUBLIC_UARS_RECV:
        {
            if(!clientPublic->CheckUARFrame(buffer))
            {
                has_error = true;
                controllerInstance->Trace(clientPublic->ErrorMessage() + QString(" ") + QVariant((uint32_t)protocolState).toString());
            }

            break;
        }

        case CM_PUBLIC_AARE_RECV:
        {
            if(!clientPublic->CheckAAREFrame(buffer))
            {
                has_error = true;
                controllerInstance->Trace(clientPublic->ErrorMessage() + QString(" ") + QVariant((uint32_t)protocolState).toString());
            }
            break;
        }

        case CM_PUBLIC_FCTR_SECURED103_RECV:
        {
            if(!clientPublic->CheckFrameCounterFrame(buffer, CM_FRAME_COUNTER_SECURED))
            {
                has_error = true;
                controllerInstance->Trace(clientPublic->ErrorMessage() + QString(" ") + QVariant((uint32_t)protocolState).toString());
            }
            break;
        }

        case CM_PUBLIC_REL_RECV:
        {
            if(!clientPublic->CheckReleaseResponse(buffer))
            {
                has_error = true;
                controllerInstance->Trace(clientPublic->ErrorMessage() + QString(" ") + QVariant((uint32_t)protocolState).toString());
            }
            break;
        }

        case CM_PUBLIC_DISCN_RECV:
        {
            if(!clientPublic->CheckDisconnectResponse(buffer))
            {
                controllerInstance->Trace(clientPublic->ErrorMessage() + QString(" ") + QVariant((uint32_t)protocolState).toString());
                has_error = true;
            }
            else
            {
                clientSecured->EnableCustomChallenge(false);
                clientSecured->SetSystemTitle(systemtitle.GetData(), 8);
                clientSecured->SetAuthenticationKey(authKey.GetData(), authKey.GetSize());
                clientSecured->SetCipheringKey(cipherKey.GetData(), cipherKey.GetSize());
                clientSecured->SetInvocationCounter(CM_FRAME_COUNTER_SECURED);
                clientSecured->InitializeClient();
            }
            break;
        }

        case CM_SECURED_UARS_RECV:
        {
            if(!clientSecured->CheckUARFrame(buffer))
            {
                controllerInstance->Trace("SECURED CLIENT SNRM <-> UARS FAILED " + QVariant((uint32_t)secured_client_index).toString());

                secured_client_index++;
                if(secured_client_index > CLIENT_SECURED_105)
                {
                    has_error = true;
                }
                else
                {
                    has_error = false;
                    protocolState = CM_SECURED_SNRM_SEND - 1;
                }
            }
            break;
        }

        case CM_SECURED_AARE_RECV:
        {
            if(!clientSecured->CheckAAREFrame(buffer))
            {
                has_error = true;
                controllerInstance->Trace("SECURED CLIENT AARQ <-> AARE FAILED " + QVariant((uint32_t)secured_client_index).toString());

                secured_client_index++;
                if(secured_client_index > CLIENT_SECURED_105)
                {
                    has_error = true;
                }
                else
                {
                    has_error = false;
                    protocolState = CM_SECURED_SNRM_SEND - 1;
                }
            }
            else
            {                
                protocolState = CM_SECURED_AUTH_SEND - 1;
            }
            break;
        }

        case CM_SECURED_AUTH_RECV:
        {
            if(!clientSecured->CheckAuthenticationResponse(buffer))
            {
                has_error = true;
                controllerInstance->Trace("SECURED CLIENT AUTHENTICATION FAILED " + QVariant((uint32_t)secured_client_index).toString());

                secured_client_index++;
                if(secured_client_index > CLIENT_SECURED_105)
                {
                    has_error = true;
                }
                else
                {
                    has_error = false;
                    protocolState = CM_DELIVERED_ACTIVE_ENERGY_RECV - 1;
                }
            }
            break;
        }

        case CM_DELIVERED_ACTIVE_ENERGY_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_DELIVERED_ACTIVE_ENERGY, responseObject))
            {
                has_error = true;
                controllerInstance->deliveredEnergy_ = old_deliveredEnergy_;
            }
            else
            {
                controllerInstance->deliveredEnergy_ = responseObject;
                protocolState =  CM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SEND - 1;
            }
            break;
        }

        case CM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER, responseObject))
            {
                has_error = true;
                controllerInstance->instantaneousPower_ = old_instantaneousPower_;
            }
            else
            {
                controllerInstance->instantaneousPower_ = responseObject;
                protocolState =  CM_ACCOUNT_USED_CREDIT_SEND - 1;
            }
            break;
        }

        case CM_ACCOUNT_USED_CREDIT_RECV:
        {
            DLMSVariant cosemObject;
            if(!clientSecured->CheckObjectRetreiveResponseFrame(buffer, cosemObject, OBIS_TOTAL_CREDIT_USED))
            {
                has_error = true;
                controllerInstance->usedCredit_ = old_usedCredit_;
            }
            else
            {
                long val;
                clientSecured->ExtractIntegerData(cosemObject, val);
                controllerInstance->usedCredit_ = val;
                protocolState =  CM_ACCOUNT_TOTAL_CREDIT_SEND - 1;
            }

            break;
        }

        case CM_ACCOUNT_TOTAL_CREDIT_RECV:
        {
            DLMSVariant cosemObject;
            if(!clientSecured->CheckObjectRetreiveResponseFrame(buffer, cosemObject, OBIS_TOTAL_CREDIT_PURCHASED))
            {
                has_error = true;
                controllerInstance->grossCredit_ = old_grossCredit_;
            }
            else
            {
                long val;
                clientSecured->ExtractIntegerData(cosemObject, val);
                controllerInstance->grossCredit_ = val;
                protocolState =  CM_ACCOUNT_REMAINING_CREDIT_SEND - 1;
            }

            break;
        }

        case CM_ACCOUNT_REMAINING_CREDIT_RECV:
        {
            if(!clientSecured->CheckAccountStatusResponseFrame(buffer, OBIS_ACCOUNT_STATUS, accountObject, isfound, 5))
            {
                has_error = true;
                controllerInstance->remainingCredit_ = old_remainingCredit_;
            }
            else
            {
                controllerInstance->remainingCredit_ = accountObject.GetAvailableCredit();
                protocolState =  CM_COMPLETE - 1;
            }

            controllerInstance->remainingCreditPercent_ = (((float)controllerInstance->remainingCredit_/(float)controllerInstance->grossCredit_)*100);

            break;
        }

        default:
        {
            controllerInstance->Trace("Connect Meter: Unhandled Protocol State : " + QVariant((uint32_t)protocolState).toString());
            break;
        }
    }

    if(protocolState == CM_COMPLETE)
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
        protocolState = CM_COMPLETE;
    }

    ProcessSendingFrame();
}

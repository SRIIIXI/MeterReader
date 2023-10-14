#include "WorkFlowConnectMeter.hpp"
#include "UICommandHandler.hpp"
#include "Mk7Mi.hpp"
#include <QThread>

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

const int CM_ACCOUNT_TOTAL_CREDIT_SEND = 17;
const int CM_ACCOUNT_TOTAL_CREDIT_RECV = 18;

const int CM_ACCOUNT_USED_CREDIT_SEND = 19;
const int CM_ACCOUNT_USED_CREDIT_RECV = 20;

const int CM_DELIVERED_ACTIVE_ENERGY_SEND = 21;
const int CM_DELIVERED_ACTIVE_ENERGY_RECV = 22;

const int CM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SEND = 23;
const int CM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_RECV = 24;

const int CM_SECURED_REL_SEND = 33;
const int CM_SECURED_REL_RECV = 34;
const int CM_SECURED_DISCN_SEND = 35;
const int CM_SECURED_DISCN_RECV = 36;

const int CM_COMPLETE = 37;

///////////////////////////////////////

WorkFlowConnectMeter::WorkFlowConnectMeter(QObject* parent): IWorkFlow(parent)
{
    CM_FRAME_COUNTER_SECURED_103 = 0;
    memset(&auth[0], 0, 48);
    memset(&ciph[0], 0, 48);
    scalar_temp = 0;
    systemtitle.Clear();
    is_disconnect_request = false;
}

void WorkFlowConnectMeter::StartWorkFlow(QList<QVariant> startParamters)
{
    has_error = false;
    scalar_temp = 0;
    if(startParamters.count() < 2)
    {
        return;
    }
    else
    {
        systemtitle.Set(startParamters.at(0).toByteArray().data(), startParamters.at(0).toByteArray().size());
    }

    is_disconnect_request = startParamters.at(1).toBool();

    if(!is_disconnect_request)
    {
        keys = startParamters.at(2).toString();
        serialno = startParamters.at(3).toString();

        memcpy(auth, &keys.toStdString().c_str()[0], 47);
        memcpy(ciph, &keys.toStdString().c_str()[48], 47);

        temp= auth;
        authKey.Clear();
        Helpers::HexToBytes(temp, authKey);

        temp= ciph;
        cipherKey.Clear();
        Helpers::HexToBytes(temp, cipherKey);
    }

    //Retreive the secured client 103 and the public client
    clientPublic = CommunicationClient::GetClient(CLIENT_PUBLIC);

    secured_client_index = CLIENT_SECURED_103;
    clientSecured = CommunicationClient::GetClient(secured_client_index);

    clientPublic->SetSystemTitle(systemtitle.GetData(), 8);
    clientPublic->EnableCustomChallenge(false);
    clientPublic->InitializeClient();

    if(is_disconnect_request)
    {
        protocolState = CM_SECURED_REL_SEND;
    }
    else
    {
        protocolState = CM_PUBLIC_SNRM_SEND;
    }

    completionParamters.append(QVariant(serialno));
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
            emit globalApplicationData->connectionProgress("5%");
            break;
        }

        case CM_PUBLIC_AARQ_SEND:
        {
            clientPublic->CreateAARQFrame(buffer);
            emit globalApplicationData->connectionProgress("10%");
            break;
        }

        case CM_PUBLIC_FCTR_SECURED103_SEND:
        {
            clientPublic->CreateFrameCounterFrame(buffer, OBIS_FRAME_COUNTER_103);
            emit globalApplicationData->connectionProgress("20%");
            break;
        }

        case CM_PUBLIC_REL_SEND:
        {
            clientPublic->CreateReleaseFrame(buffer);
            emit globalApplicationData->connectionProgress("25%");
            break;
        }

        case CM_PUBLIC_DISCN_SEND:
        {
            clientPublic->CreateDisconnectFrame(buffer);
            emit globalApplicationData->connectionProgress("30%");
            break;
        }

        case CM_SECURED_SNRM_SEND:
        {
            clientSecured->CreateSNRMFrame(buffer);
            emit globalApplicationData->connectionProgress("35%");
            break;
        }

        case CM_SECURED_AARQ_SEND:
        {
            clientSecured->CreateAARQFrame(buffer);
            emit globalApplicationData->connectionProgress("40%");
            break;
        }

        case CM_SECURED_AUTH_SEND:
        {
            clientSecured->CreateAuthenticationFrame(buffer);
            emit globalApplicationData->connectionProgress("45%");
            break;
        }

        case CM_ACCOUNT_TOTAL_CREDIT_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_TOTAL_CREDIT_PURCHASED);
            emit globalApplicationData->connectionProgress("50%");
            break;
        }

        case CM_ACCOUNT_USED_CREDIT_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_TOTAL_CREDIT_USED);
            emit globalApplicationData->connectionProgress("55%");
            break;
        }

        case CM_DELIVERED_ACTIVE_ENERGY_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_DELIVERED_ACTIVE_ENERGY);
            emit globalApplicationData->connectionProgress("60%");
            break;
        }

        case CM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER);
            emit globalApplicationData->connectionProgress("60%");
            break;
        }

        case CM_SECURED_REL_SEND:
        {
            clientSecured->CreateReleaseFrame(buffer);
            if(is_disconnect_request)
            {
                emit globalApplicationData->connectionProgress("30%");
            }
            else
            {
                emit globalApplicationData->connectionProgress("90%");
            }
            break;
        }

        case CM_SECURED_DISCN_SEND:
        {
            clientSecured->CreateDisconnectFrame(buffer);
            if(is_disconnect_request)
            {
                emit globalApplicationData->connectionProgress("65%");
            }
            else
            {
                emit globalApplicationData->connectionProgress("95%");
            }
            break;
        }

        case CM_COMPLETE:
        {
            emit globalApplicationData->connectionProgress("100%");
            break;
        }

        default:
        {
            qDebug() << "ERROR: UNHANDLED PROTOCOL STATE : " << protocolState;
            break;
        }
    }

    if(protocolState == CM_COMPLETE)
    {
        if(is_disconnect_request)
        {
            completionParamters.clear();
            emit Completed(completionParamters, DisconnectMeter, !has_error);
        }
        else
        {
            emit Completed(completionParamters, ConnectMeter, !has_error);
        }

        return;
    }

    if(buffer.length() > 0)
    {
        //std::string str = Helpers::BytesToHex((unsigned char*)buffer.data(), buffer.length());
        //std::stringstream ss;
        //ss << protocolState << " TX [" << buffer.length() << "] " << str.c_str();
        //qDebug() << ss.str().c_str();
        QThread::currentThread()->msleep(100);

        centralPtr->SendData(buffer);
        protocolState++;
    }
    else
    {
        qDebug() << "PROTOCOL SEND BUFFER NOT READY -> STEP -> " << protocolState;
    }
}

void WorkFlowConnectMeter::ProcessReceivedFrame(const QByteArray &buffer)
{
    //std::string str = Helpers::BytesToHex((unsigned char*)buffer.data(), buffer.length());
    //std::stringstream ss;
    //ss << protocolState << " RX [" << buffer.length() << "] " << str.c_str();
    //qDebug() << ss.str().c_str();

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
                //qDebug() << clientPublic->ErrorMessage() << protocolState;
            }
            else
            {
                //qDebug() << "PUBLIC CLIENT SNRM <-> UARS OKAY";
            }
            break;
        }

        case CM_PUBLIC_AARE_RECV:
        {
            if(!clientPublic->CheckAAREFrame(buffer))
            {
                has_error = true;
                //qDebug() << clientPublic->ErrorMessage() << protocolState;
            }
            else
            {
                //qDebug() << "PUBLIC CLIENT AARQ <-> AARE OKAY";
            }
            break;
        }

        case CM_PUBLIC_FCTR_SECURED103_RECV:
        {
            if(!clientPublic->CheckFrameCounterFrame(buffer, CM_FRAME_COUNTER_SECURED_103))
            {
                has_error = true;
                //qDebug() << clientPublic->ErrorMessage() << protocolState;
            }
            else
            {
                //qDebug() << "SECURED CLIENT 103 FRAME COUNTER RECEIVED : " << CM_FRAME_COUNTER_SECURED_103;
            }
            break;
        }

        case CM_PUBLIC_REL_RECV:
        {
            if(!clientPublic->CheckReleaseResponse(buffer))
            {
                has_error = true;
                //qDebug() << clientPublic->ErrorMessage() << protocolState;
            }
            else
            {
                //qDebug() << "PUBLIC CLIENT RELEASED";
            }
            break;
        }

        case CM_PUBLIC_DISCN_RECV:
        {
            if(!clientPublic->CheckDisconnectResponse(buffer))
            {
                //qDebug() << clientPublic->ErrorMessage() << protocolState;
                has_error = true;
            }
            else
            {
                //qDebug() << "PUBLIC CLIENT DISCONNECTED";
                clientSecured->EnableCustomChallenge(false);
                clientSecured->SetSystemTitle(systemtitle.GetData(), 8);
                clientSecured->SetAuthenticationKey(authKey.GetData(), authKey.GetSize());
                clientSecured->SetCipheringKey(cipherKey.GetData(), cipherKey.GetSize());
                clientSecured->SetInvocationCounter(CM_FRAME_COUNTER_SECURED_103);
                clientSecured->InitializeClient();
            }
            break;
        }

        case CM_SECURED_UARS_RECV:
        {
            if(!clientSecured->CheckUARFrame(buffer))
            {
                //qDebug() << "SECURED CLIENT SNRM <-> UARS FAILED";
                //qDebug() << clientSecured->ErrorMessage() << protocolState;
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
                //qDebug() << "SECURED CLIENT SNRM <-> UARS OKAY";
            }
            break;
        }

        case CM_SECURED_AARE_RECV:
        {
            if(!clientSecured->CheckAAREFrame(buffer))
            {
                has_error = true;
                //qDebug() << "SECURED CLIENT AARQ <-> AARE FAILED";
                //qDebug() << clientSecured->ErrorMessage() << protocolState;
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
                //qDebug() << "SECURED CLIENT AARQ <-> AARE OKAY";
            }
            break;
        }

        case CM_SECURED_AUTH_RECV:
        {
            if(!clientSecured->CheckAuthenticationResponse(buffer))
            {
                has_error = true;
                //qDebug() << "SECURED CLIENT AUTHENTICATION FAILED" << protocolState;
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
                //qDebug() << "SECURED CLIENT AUTHENTICATED";
            }
            break;
        }

        case CM_ACCOUNT_TOTAL_CREDIT_RECV:
        {
            DLMSVariant cosemObject;
            if(!clientSecured->CheckObjectRetreiveResponseFrame(buffer, cosemObject, OBIS_TOTAL_CREDIT_PURCHASED))
            {
                has_error = true;
                //qDebug() << "TOTAL CREDIT RETRIEVAL FAILED";
            }
            else
            {
                long val;
                clientSecured->ExtractIntegerData(cosemObject, val);
                //qDebug() << "TOTAL CREDIT RETRIEVED" << val;
                completionParamters.append(QVariant((uint32_t)val));
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
                //qDebug() << "USED CREDIT RETRIEVAL FAILED";
            }
            else
            {
                long val;
                clientSecured->ExtractIntegerData(cosemObject, val);
                //qDebug() << "USED CREDIT RETRIEVED" << val;
                completionParamters.append(QVariant((uint32_t)val));
                protocolState =  CM_DELIVERED_ACTIVE_ENERGY_SEND - 1;
            }

            break;
        }

        case CM_DELIVERED_ACTIVE_ENERGY_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_DELIVERED_ACTIVE_ENERGY, responseObject))
            {
                //qDebug() << clientSecured->ErrorMessage();
                has_error = true;
            }
            else
            {
                //qDebug() << "DELIVERED ACTIVE ENERGY" << responseObject;
                completionParamters.append(QVariant((unsigned int)responseObject));
                protocolState =  CM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SEND - 1;
            }
            break;
        }

        case CM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER, responseObject))
            {
                //qDebug() << clientSecured->ErrorMessage();
                has_error = true;
            }
            else
            {
                //qDebug() << "INSTANTENOUS ACTIVE POWER" << responseObject;
                completionParamters.append(QVariant((unsigned int)responseObject));

                if(is_disconnect_request)
                {
                    protocolState =  CM_SECURED_REL_SEND - 1;
                }
                else
                {
                    protocolState =  CM_COMPLETE - 1;
                }
            }
            break;
        }

        case CM_SECURED_REL_RECV:
        {
            if(!clientPublic->CheckReleaseResponse(buffer))
            {
                has_error = true;
                //qDebug() << clientSecured->ErrorMessage() << protocolState;
            }
            else
            {
                //qDebug() << "SECURED CLIENT RELEASED";
                protocolState = CM_SECURED_DISCN_SEND - 1;
            }
            break;
        }

        case CM_SECURED_DISCN_RECV:
        {
            //qDebug() << "SECURED CLIENT DISCONNECTED";
            break;
        }

        default:
        {
            //qDebug() << "ERROR: UNHANDLED PROTOCOL STATE : " << protocolState;
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
        protocolState = CM_COMPLETE;
    }

    ProcessSendingFrame();
}

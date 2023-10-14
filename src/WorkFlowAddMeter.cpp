#include "WorkFlowAddMeter.hpp"
#include "UICommandHandler.hpp"
#include "Mk7Mi.hpp"
#include <QThread>

const int AM_PUBLIC_SNRM_SEND = 1;
const int AM_PUBLIC_UARS_RECV = 2;
const int AM_PUBLIC_AARQ_SEND = 3;
const int AM_PUBLIC_AARE_RECV = 4;
const int AM_PUBLIC_FCTR_PAIRING_SEND = 5;
const int AM_PUBLIC_FCTR_PAIRING_RECV = 6;
const int AM_PUBLIC_FCTR_SECURED103_SEND = 7;
const int AM_PUBLIC_FCTR_SECURED103_RECV = 8;
const int AM_PUBLIC_FCTR_SECURED104_SEND = 9;
const int AM_PUBLIC_FCTR_SECURED104_RECV = 10;
const int AM_PUBLIC_FCTR_SECURED105_SEND = 11;
const int AM_PUBLIC_FCTR_SECURED105_RECV = 12;
const int AM_PUBLIC_REL_SEND = 13;
const int AM_PUBLIC_REL_RECV = 14;
const int AM_PUBLIC_DISCN_SEND = 15;
const int AM_PUBLIC_DISCN_RECV = 16;

const int AM_PAIRING_SNRM_SEND = 17;
const int AM_PAIRING_UARS_RECV = 18;
const int AM_PAIRING_AARQ_SEND = 19;
const int AM_PAIRING_AARE_RECV = 20;
const int AM_PAIRING_AUTH_SEND = 21;
const int AM_PAIRING_AUTH_RECV = 22;
const int AM_PAIRING_CIUR_SEND = 23;
const int AM_PAIRING_CIUR_RECV = 24;
const int AM_PAIRING_REL_SEND = 25;
const int AM_PAIRING_REL_RECV = 26;
const int AM_PAIRING_DISCN_SEND = 27;
const int AM_PAIRING_DISCN_RECV = 28;

const int AM_SECURED_SNRM_SEND = 29;
const int AM_SECURED_UARS_RECV = 30;
const int AM_SECURED_AARQ_SEND = 31;
const int AM_SECURED_AARE_RECV = 32;
const int AM_SECURED_AUTH_SEND = 33;
const int AM_SECURED_AUTH_RECV = 34;

const int AM_SECURED_SERNO_SEND = 35;
const int AM_SECURED_SERNO_RECV = 36;
const int AM_SECURED_MNAME_SEND = 37;
const int AM_SECURED_MNAME_RECV = 38;

const int AM_H_CAPTURED_OBJECTS_SEND = 39;
const int AM_H_CAPTURED_OBJECTS_RECV = 40;
const int AM_H_CAPTURED_OBJECTS_RECEIVER_READY_SEND = 41;
const int AM_H_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV = 42;

const int AM_H_FIRST_RECORD_SEND = 43;
const int AM_H_FIRST_RECORD_RECV = 44;
const int AM_H_FIRST_RECORD_RECEIVER_READY_SEND = 45;
const int AM_H_FIRST_RECORD_PARTIAL_FRAME_RECV = 46;

const int AM_D_CAPTURED_OBJECTS_SEND = 47;
const int AM_D_CAPTURED_OBJECTS_RECV = 48;
const int AM_D_CAPTURED_OBJECTS_RECEIVER_READY_SEND = 49;
const int AM_D_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV = 50;

const int AM_D_FIRST_RECORD_SEND = 51;
const int AM_D_FIRST_RECORD_RECV = 52;
const int AM_D_FIRST_RECORD_RECEIVER_READY_SEND = 53;
const int AM_D_FIRST_RECORD_PARTIAL_FRAME_RECV = 54;

const int AM_ACCOUNT_TYPE_SEND = 55;
const int AM_ACCOUNT_TYPE_RECV = 56;
const int AM_ACCOUNT_CURRENCY_AND_SCALAR_SEND = 57;
const int AM_ACCOUNT_CURRENCY_AND_SCALAR_RECV = 58;

const int AM_DELIVERED_ACTIVE_ENERGY_SCALAR_SEND = 59;
const int AM_DELIVERED_ACTIVE_ENERGY_SCALAR_RECV = 60;
const int AM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SCALAR_SEND = 61;
const int AM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SCALAR_RECV = 62;
const int AM_ACTIVE_IMPORT_SCALAR_SEND = 63;
const int AM_ACTIVE_IMPORT_SCALAR_RECV = 64;
const int AM_ACTIVE_EXPORT_SCALAR_SEND = 65;
const int AM_ACTIVE_EXPORT_SCALAR_RECV = 66;
const int AM_REACTIVE_IMPORT_SCALAR_SEND = 67;
const int AM_REACTIVE_IMPORT_SCALAR_RECV = 68;
const int AM_REACTIVE_EXPORT_SCALAR_SEND = 69;
const int AM_REACTIVE_EXPORT_SCALAR_RECV = 70;

const int AM_SECURED_REL_SEND = 71;
const int AM_SECURED_REL_RECV = 72;
const int AM_SECURED_DISCN_SEND = 73;
const int AM_SECURED_DISCN_RECV = 74;

const int AM_COMPLETE = 75;

////////////////////////////////////

WorkFlowAddMeter::WorkFlowAddMeter(QObject* parent): IWorkFlow(parent)
{
    AM_FRAME_COUNTER_PAIRING = 0;
    AM_FRAME_COUNTER_SECURED_103 = 0;
    AM_FRAME_COUNTER_SECURED_104 = 0;
    AM_FRAME_COUNTER_SECURED_105 = 0;

    systemtitle.Clear();
}

void WorkFlowAddMeter::StartWorkFlow(QList<QVariant> startParamters)
{
    has_error = false;
    if(startParamters.count() < 1)
    {
        return;
    }
    else
    {
        systemtitle.Set(startParamters.at(0).toByteArray().data(), startParamters.at(0).toByteArray().size());
    }

    completionParamters.clear();

    // We expect the system title and network identifier
    std::string str;

    str = Helpers::BytesToHex((unsigned char*)startParamters.at(0).toByteArray().data(), startParamters.at(0).toByteArray().size());
    completionParamters.append(QVariant(QString(str.c_str())));

    str = Helpers::BytesToHex((unsigned char*)startParamters.at(1).toByteArray().data(), startParamters.at(1).toByteArray().size());
    completionParamters.append(QVariant(QString(str.c_str())));

    //Always retreive all the clients
    clientPublic = CommunicationClient::GetClient(CLIENT_PUBLIC);
    clientPairing = CommunicationClient::GetClient(CLIENT_PAIRING);

    secured_client_index = CLIENT_SECURED_103;
    clientSecured = CommunicationClient::GetClient(secured_client_index);

    clientPublic->SetSystemTitle(systemtitle.GetData(), 8);
    clientPublic->EnableCustomChallenge(false);
    clientPublic->InitializeClient();

    protocolState = AM_PUBLIC_SNRM_SEND;
    ProcessSendingFrame();
}

void WorkFlowAddMeter::ProcessSendingFrame()
{
    QByteArray buffer;
    switch(protocolState)
    {
        case AM_PUBLIC_SNRM_SEND:
        {
            clientPublic->CreateSNRMFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("1%");
            break;
        }
        case AM_PUBLIC_AARQ_SEND:
        {
            clientPublic->CreateAARQFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("4%");
            break;
        }
        case AM_PUBLIC_FCTR_PAIRING_SEND:
        {
            clientPublic->CreateFrameCounterFrame(buffer, OBIS_FRAME_COUNTER_PAIRING);
            emit globalApplicationData->meterAdditionProgress("7%");
            break;
        }
        case AM_PUBLIC_FCTR_SECURED103_SEND:
        {
            clientPublic->CreateFrameCounterFrame(buffer, OBIS_FRAME_COUNTER_103);
            emit globalApplicationData->meterAdditionProgress("11%");
            break;
        }
        case AM_PUBLIC_FCTR_SECURED104_SEND:
        {
            clientPublic->CreateFrameCounterFrame(buffer, OBIS_FRAME_COUNTER_104);
            emit globalApplicationData->meterAdditionProgress("14%");
            break;
        }
        case AM_PUBLIC_FCTR_SECURED105_SEND:
        {
            clientPublic->CreateFrameCounterFrame(buffer, OBIS_FRAME_COUNTER_105);
            emit globalApplicationData->meterAdditionProgress("18%");
            break;
        }
        case AM_PUBLIC_REL_SEND:
        {
            clientPublic->CreateReleaseFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("21%");
            break;
        }
        case AM_PUBLIC_DISCN_SEND:
        {
            clientPublic->CreateDisconnectFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("24%");
            break;
        }

        case AM_PAIRING_SNRM_SEND:
        {
            clientPairing->CreateSNRMFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("27%");
            break;
        }
        case AM_PAIRING_AARQ_SEND:
        {
            clientPairing->CreateAARQFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("30%");
            break;
        }
        case AM_PAIRING_AUTH_SEND:
        {
            clientPairing->CreateAuthenticationFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("34%");
            break;
        }
        case AM_PAIRING_CIUR_SEND:
        {
            clientPairing->CreateObjectRetreiveRequestFrame(buffer, OBIS_CIU_KEYS);
            emit globalApplicationData->meterAdditionProgress("37%");
            break;
        }
        case AM_PAIRING_REL_SEND:
        {
            clientPairing->CreateReleaseFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("40%");
            break;
        }
        case AM_PAIRING_DISCN_SEND:
        {
            clientPairing->CreateDisconnectFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("43%");
            break;
        }

        case AM_SECURED_SNRM_SEND:
        {
            clientSecured->CreateSNRMFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("47%");
            break;
        }
        case AM_SECURED_AARQ_SEND:
        {
            clientSecured->CreateAARQFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("51%");
            break;
        }
        case AM_SECURED_AUTH_SEND:
        {
            clientSecured->CreateAuthenticationFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("54%");
            break;
        }
        case AM_SECURED_SERNO_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_METER_SERIAL_NO);
            emit globalApplicationData->meterAdditionProgress("57%");
            break;
        }
        case AM_SECURED_MNAME_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_LOGICAL_DEVICE_NAME);
            emit globalApplicationData->meterAdditionProgress("60%");
            break;
        }
        case AM_H_CAPTURED_OBJECTS_SEND:
        {
            clientSecured->StartProfileGenericDialog("1.0.99.1.0.255");
            clientSecured->StartBatch();
            clientSecured->CreateProfileGenericCapturedObjectsRequestFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("64%");
            break;
        }
        case AM_H_CAPTURED_OBJECTS_RECEIVER_READY_SEND:
        {
            clientSecured->CreateReceiverReadyFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("67%");
            break;
        }
        case AM_H_FIRST_RECORD_SEND:
        {
            clientSecured->StartBatch();
            clientSecured->CreateProfileGenericBatchRecordsRequestFrame(buffer, 1, 1);
            emit globalApplicationData->meterAdditionProgress("71%");
            break;
        }
        case AM_H_FIRST_RECORD_RECEIVER_READY_SEND:
        {
            clientSecured->CreateReceiverReadyFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("74%");
            break;
        }
        case AM_D_CAPTURED_OBJECTS_SEND:
        {
            clientSecured->StartProfileGenericDialog("1.0.99.2.0.255");
            clientSecured->StartBatch();
            clientSecured->CreateProfileGenericCapturedObjectsRequestFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("77%");
            break;
        }
        case AM_D_CAPTURED_OBJECTS_RECEIVER_READY_SEND:
        {
            clientSecured->CreateReceiverReadyFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("81%");
            break;
        }
        case AM_D_FIRST_RECORD_SEND:
        {
            clientSecured->StartBatch();
            clientSecured->CreateProfileGenericBatchRecordsRequestFrame(buffer, 1, 1);
            emit globalApplicationData->meterAdditionProgress("84%");
            break;
        }
        case AM_D_FIRST_RECORD_RECEIVER_READY_SEND:
        {
            clientSecured->CreateReceiverReadyFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("87%");
            break;
        }
        case AM_ACCOUNT_TYPE_SEND:
        {
            clientSecured->CreatePrePaidRequestFrame(buffer, OBIS_ACCOUNT_STATUS, 2);
            emit globalApplicationData->meterAdditionProgress("80%");
            break;
        }
        case AM_ACCOUNT_CURRENCY_AND_SCALAR_SEND:
        {
            clientSecured->CreatePrePaidRequestFrame(buffer, OBIS_ACCOUNT_STATUS, 15);
            emit globalApplicationData->meterAdditionProgress("89%");
            break;
        }
        case AM_DELIVERED_ACTIVE_ENERGY_SCALAR_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER);
            emit globalApplicationData->meterAdditionProgress("90%");
            break;
        }
        case AM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SCALAR_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER, 3);
            emit globalApplicationData->meterAdditionProgress("91%");
            break;
        }
        case AM_ACTIVE_IMPORT_SCALAR_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_ACTIVE_ENERGY_IMPORT_TARIFF1, 3);
            emit globalApplicationData->meterAdditionProgress("92%");
            break;
        }
        case AM_ACTIVE_EXPORT_SCALAR_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_ACTIVE_ENERGY_EXPORT_TARIFF1, 3);
            emit globalApplicationData->meterAdditionProgress("93%");
            break;
        }
        case AM_REACTIVE_IMPORT_SCALAR_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_REACTIVE_ENERGY_IMPORT_TARIFF1, 3);
            emit globalApplicationData->meterAdditionProgress("94%");
            break;
        }
        case AM_REACTIVE_EXPORT_SCALAR_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_REACTIVE_ENERGY_EXPORT_TARIFF1, 3);
            emit globalApplicationData->meterAdditionProgress("95%");
            break;
        }
        case AM_SECURED_REL_SEND:
        {
            clientSecured->CreateReleaseFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("97%");
            break;
        }
        case AM_SECURED_DISCN_SEND:
        {
            clientSecured->CreateDisconnectFrame(buffer);
            emit globalApplicationData->meterAdditionProgress("100%");
            break;
        }

        case AM_COMPLETE:
        {
            break;
        }
        default:
        {
            qDebug() << "ERROR: UNHANDLED PROTOCOL STATE : " << protocolState;
            break;
        }
    }

    if(protocolState == AM_COMPLETE)
    {
        qDebug() << "Completed  adding a new meter";
        emit Completed(completionParamters, Addmeter, !has_error);
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
        //qDebug() << "SENT" << protocolState;
    }
    else
    {
        qDebug() << "PROTOCOL SEND BUFFER NOT READY -> STEP -> " << protocolState;
    }
}

void WorkFlowAddMeter::ProcessReceivedFrame(const QByteArray &buffer)
{
    //qDebug() << "RECEIVED" << protocolState;
    //std::string str = Helpers::BytesToHex((unsigned char*)buffer.data(), buffer.length());
    //std::stringstream ss;
    //ss << protocolState << " RX [" << buffer.length() << "] " << str.c_str();
    //qDebug() << ss.str().c_str();

    bool ispartial = false;
    bool islast = false;
    float responseObject = -1;
    DLMSAccount accountObject;
    bool isfound = false;
    has_error = false;

    switch(protocolState)
    {
        case AM_PUBLIC_UARS_RECV:
        {
            if(!clientPublic->CheckUARFrame(buffer))
            {
                has_error = true;
            }
            else
            {
                //qDebug() << "PUBLIC CLIENT SNRM <-> UARS OKAY";
            }
            break;
        }
        case AM_PUBLIC_AARE_RECV:
        {
            if(!clientPublic->CheckAAREFrame(buffer))
            {
                has_error = true;
            }
            else
            {
                //qDebug() << "PUBLIC CLIENT AARQ <-> AARE OKAY";
            }
            break;
        }
        case AM_PUBLIC_FCTR_PAIRING_RECV:
        {
            if(!clientPublic->CheckFrameCounterFrame(buffer, AM_FRAME_COUNTER_PAIRING))
            {
                has_error = true;
            }
            else
            {
                //qDebug() << "PAIRING CLIENT FRAME COUNTER RECEIVED : " << AM_FRAME_COUNTER_PAIRING;
            }
            break;
        }
        case AM_PUBLIC_FCTR_SECURED103_RECV:
        {
            if(!clientPublic->CheckFrameCounterFrame(buffer, AM_FRAME_COUNTER_SECURED_103))
            {
                has_error = true;
            }
            else
            {
                //qDebug() << "SECURED CLIENT 103 FRAME COUNTER RECEIVED : " << AM_FRAME_COUNTER_SECURED_103;
            }
            break;
        }
        case AM_PUBLIC_FCTR_SECURED104_RECV:
        {
            if(!clientPublic->CheckFrameCounterFrame(buffer, AM_FRAME_COUNTER_SECURED_104))
            {
                has_error = true;
            }
            else
            {
                //qDebug() << "SECURED CLIENT 104 FRAME COUNTER RECEIVED : " << AM_FRAME_COUNTER_SECURED_104;
            }
            break;
        }
        case AM_PUBLIC_FCTR_SECURED105_RECV:
        {
            if(!clientPublic->CheckFrameCounterFrame(buffer, AM_FRAME_COUNTER_SECURED_105))
            {
                has_error = true;
            }
            else
            {
                //qDebug() << "SECURED CLIENT 105 FRAME COUNTER RECEIVED : " << AM_FRAME_COUNTER_SECURED_105;
            }
            break;
        }
        case AM_PUBLIC_REL_RECV:
        {
            if(!clientPublic->CheckReleaseResponse(buffer))
            {
                has_error = true;
            }
            else
            {
                //qDebug() << "PUBLIC CLIENT RELEASED";
            }
            break;
        }
        case AM_PUBLIC_DISCN_RECV:
        {
            if(!clientPublic->CheckDisconnectResponse(buffer))
            {
                has_error = true;
            }
            else
            {
                //qDebug() << "PUBLIC CLIENT DISCONNECTED";
                clientPairing->EnableCustomChallenge(true);
                clientPairing->SetSystemTitle(systemtitle.GetData(), 8);
                clientPairing->SetInvocationCounter(AM_FRAME_COUNTER_PAIRING);
                clientPairing->InitializeClient();
            }
            break;
        }

        case AM_PAIRING_UARS_RECV:
        {
            if(!clientPairing->CheckUARFrame(buffer))
            {
                has_error = true;
            }
            else
            {
                //qDebug() << "PAIRING CLIENT SNRM <-> UARS OKAY";
            }
            break;
        }

        case AM_PAIRING_AARE_RECV:
        {
            if(!clientPairing->CheckAAREFrame(buffer))
            {
                has_error = true;
                //qDebug() << "PAIRING CLIENT AARQ <-> AARE FAILED";
                //qDebug() << clientPairing->ErrorMessage();
            }
            else
            {
                //qDebug() << "PAIRING CLIENT AARQ <-> AARE OKAY";
            }
            break;
        }

        case AM_PAIRING_AUTH_RECV:
        {
            if(!clientPairing->CheckAuthenticationResponse(buffer))
            {
                has_error = true;
                //qDebug() << "PAIRING CLIENT AUTHENTICATION FAILED";
            }
            else
            {
                //qDebug() << "PAIRING CLIENT AUTHENTICATED";
            }
            break;
        }

        case AM_PAIRING_CIUR_RECV:
        {
            DLMSVariant cosemObject;
            if(!clientPairing->CheckObjectRetreiveResponseFrame(buffer, cosemObject, OBIS_CIU_KEYS))
            {
                has_error = true;
                //qDebug() << "CIU KEY RETRIEVAL FAILED";
            }
            else
            {
                //qDebug() << "RECEIVED CIU KEYS";

                CommunicationClient* clientSecured103 = CommunicationClient::GetClient(CLIENT_SECURED_103);
                CommunicationClient* clientSecured104 = CommunicationClient::GetClient(CLIENT_SECURED_104);
                CommunicationClient* clientSecured105 = CommunicationClient::GetClient(CLIENT_SECURED_105);

                CommunicationClient::ExtractKeys(cosemObject, clientSecured103, clientSecured104, clientSecured105);
                //qDebug() << "Extracted keys";

                unsigned char key_buffer[32];
                std::string str;

                memset(key_buffer, 0, 32);
                memcpy(&key_buffer[0], clientSecured103->GetAuthenticationKey().GetData(), 16);
                memcpy(&key_buffer[16], clientSecured103->GetCipheringKey().GetData(), 16);

                str = Helpers::BytesToHex(key_buffer, 32);
                completionParamters.append(QVariant(QString(str.c_str())));

                memset(key_buffer, 0, 32);
                memcpy(&key_buffer[0], clientSecured104->GetAuthenticationKey().GetData(), 16);
                memcpy(&key_buffer[16], clientSecured104->GetCipheringKey().GetData(), 16);

                str = Helpers::BytesToHex(key_buffer, 32);
                completionParamters.append(QVariant(QString(str.c_str())));

                memset(key_buffer, 0, 32);
                memcpy(&key_buffer[0], clientSecured105->GetAuthenticationKey().GetData(), 16);
                memcpy(&key_buffer[16], clientSecured105->GetCipheringKey().GetData(), 16);

                str = Helpers::BytesToHex(key_buffer, 32);
                completionParamters.append(QVariant(QString(str.c_str())));
                //qDebug() << "Stored keys for completion parameters";
            }
            break;
        }

        case AM_PAIRING_REL_RECV:
        {
            if(!clientPairing->CheckReleaseResponse(buffer))
            {
                has_error = true;
            }
            else
            {
                //qDebug() << "PAIRING CLIENT RELEASED";
            }
            break;
        }

        case AM_PAIRING_DISCN_RECV:
        {
            if(!clientPairing->CheckDisconnectResponse(buffer))
            {
                has_error = true;
            }
            else
            {
                //qDebug() << "PAIRING CLIENT DISCONNECTED";
                clientSecured->EnableCustomChallenge(false);
                clientSecured->SetSystemTitle(systemtitle.GetData(), 8);
                clientSecured->SetInvocationCounter(AM_FRAME_COUNTER_SECURED_103);
                clientSecured->InitializeClient();
            }
            break;
        }

        case AM_SECURED_UARS_RECV:
        {
            if(!clientSecured->CheckUARFrame(buffer))
            {
                has_error = true;
                //qDebug() << "SECURED CLIENT SNRM <-> UARS FAILED";
                //qDebug() << clientSecured->ErrorMessage();
                secured_client_index++;
                if(secured_client_index > CLIENT_SECURED_105)
                {
                    has_error = true;
                }
                else
                {
                    has_error = false;
                    protocolState = AM_SECURED_SNRM_SEND - 1;
                }
            }
            else
            {
                //qDebug() << "SECURED CLIENT SNRM <-> UARS OKAY";
            }
            break;
        }

        case AM_SECURED_AARE_RECV:
        {
            if(!clientSecured->CheckAAREFrame(buffer))
            {
                has_error = true;
                //qDebug() << "SECURED CLIENT AARQ <-> AARE FAILED";
                //qDebug() << clientSecured->ErrorMessage();
                secured_client_index++;
                if(secured_client_index > CLIENT_SECURED_105)
                {
                    has_error = true;
                }
                else
                {
                    has_error = false;
                    protocolState = AM_SECURED_SNRM_SEND - 1;
                }
            }
            else
            {
                //qDebug() << "SECURED CLIENT AARQ <-> AARE OKAY";

            }
            break;
        }

        case AM_SECURED_AUTH_RECV:
        {
            if(!clientSecured->CheckAuthenticationResponse(buffer))
            {
                has_error = true;
                //qDebug() << "SECURED CLIENT AUTHENTICATION FAILED";
                secured_client_index++;
                if(secured_client_index > CLIENT_SECURED_105)
                {
                    has_error = true;
                }
                else
                {
                    has_error = false;
                    protocolState = AM_SECURED_SNRM_SEND - 1;
                }
            }
            else
            {
                //qDebug() << "SECURED CLIENT AUTHENTICATED";
            }
            break;
        }

        case AM_SECURED_SERNO_RECV:
        {
            DLMSVariant cosemObject;
            if(!clientSecured->CheckObjectRetreiveResponseFrame(buffer, cosemObject, OBIS_METER_SERIAL_NO))
            {
                has_error = true;
                //qDebug() << "SERIAL NUMBER RETRIEVAL FAILED";
            }
            else
            {
                clientSecured->ExtractStringData(cosemObject, serialno);
                //qDebug() << "SERIAL NUMBER RETRIEVED" << serialno;
                completionParamters.append(QVariant(serialno));
            }
            break;
        }

        case AM_SECURED_MNAME_RECV:
        {
            DLMSVariant cosemObject;
            if(!clientSecured->CheckObjectRetreiveResponseFrame(buffer, cosemObject, OBIS_LOGICAL_DEVICE_NAME))
            {
                has_error = true;
                //qDebug() << "METER NAME RETRIEVAL FAILED";
            }
            else
            {
                clientSecured->ExtractStringData(cosemObject, metername);
                //qDebug() << "METER NAME RETRIEVED" << metername;
                completionParamters.append(QVariant(metername));
            }
            break;
        }

        case AM_H_CAPTURED_OBJECTS_RECV:
        case AM_H_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV:
        {
            QString colnames;
            long capturedColumnCount = 0;
            if(!clientSecured->CheckBatchFrame(buffer, ispartial, islast))
            {
                has_error = true;
                //qDebug() << clientSecured->ErrorMessage();
                break;
            }

            if(ispartial)
            {
                // If not a complete frame then, keep the buffer in accumulator and set next stage
                protocolState = AM_H_CAPTURED_OBJECTS_RECEIVER_READY_SEND - 1;
            }
            else
            {
                if(islast)
                {
                    //Now assemble the complete frame
                    if(!clientSecured->CheckProfileGenericCapturedObjectsResponseFrame(colnames, capturedColumnCount))
                    {
                        //qDebug() << clientSecured->ErrorMessage();
                        has_error = true;
                    }

                    protocolState = AM_H_FIRST_RECORD_SEND - 1;
                }
                else
                {
                    protocolState = AM_H_CAPTURED_OBJECTS_RECEIVER_READY_SEND - 1;
                }
            }
            break;
        }

        case AM_H_FIRST_RECORD_RECV:
        case AM_H_FIRST_RECORD_PARTIAL_FRAME_RECV:
        {
            if(!clientSecured->CheckBatchFrame(buffer, ispartial, islast))
            {
                has_error = true;
                //qDebug() << clientSecured->ErrorMessage();
                break;
            }

            if(ispartial)
            {
                // If not a complete frame then, keep the buffer in accumulator and set next stage
                protocolState = AM_H_FIRST_RECORD_RECEIVER_READY_SEND - 1;
            }
            else
            {
                if(islast)
                {
                    //Now assemble the complete frame
                    std::vector<std::vector<DLMSVariant>> buffer;
                    if(!clientSecured->CheckProfileGenericBatchRecordsResponseFrame(buffer))
                    {
                        //qDebug() << clientSecured->ErrorMessage();
                        has_error = true;
                    }
                    else
                    {                       
                        completionParamters.append(QVariant((uint32_t)buffer[0][1].dateTime.ToUnixTime()));
                        completionParamters.append(QVariant((float)buffer[0][3].ToInteger()));
                        completionParamters.append(QVariant((float)buffer[0][4].ToInteger()));
                        completionParamters.append(QVariant((float)buffer[0][5].ToInteger()));
                        completionParamters.append(QVariant((float)buffer[0][6].ToInteger()));

                        //qDebug() << "FIRST RECORD LP1 RECEIVED";
                        protocolState = AM_D_CAPTURED_OBJECTS_SEND - 1;
                    }
                }
                else
                {
                    protocolState = AM_H_FIRST_RECORD_RECEIVER_READY_SEND - 1;
                }
            }
            break;
        }

        case AM_D_CAPTURED_OBJECTS_RECV:
        case AM_D_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV:
        {
            QString colnames;
            long capturedColumnCount = 0;
            if(!clientSecured->CheckBatchFrame(buffer, ispartial, islast))
            {
                has_error = true;
                //qDebug() << clientSecured->ErrorMessage();
                break;
            }

            if(ispartial)
            {
                // If not a complete frame then, keep the buffer in accumulator and set next stage
                protocolState = AM_D_CAPTURED_OBJECTS_RECEIVER_READY_SEND - 1;
            }
            else
            {
                if(islast)
                {
                    //Now assemble the complete frame
                    if(!clientSecured->CheckProfileGenericCapturedObjectsResponseFrame(colnames, capturedColumnCount))
                    {
                        //qDebug() << clientSecured->ErrorMessage();
                        has_error = true;
                    }

                    protocolState = AM_D_FIRST_RECORD_SEND - 1;
                }
                else
                {
                    protocolState = AM_D_CAPTURED_OBJECTS_RECEIVER_READY_SEND - 1;
                }
            }
            break;
        }

        case AM_D_FIRST_RECORD_RECV:
        case AM_D_FIRST_RECORD_PARTIAL_FRAME_RECV:
        {
            if(!clientSecured->CheckBatchFrame(buffer, ispartial, islast))
            {
                has_error = true;
                //qDebug() << clientSecured->ErrorMessage();
                break;
            }

            if(ispartial)
            {
                // If not a complete frame then, keep the buffer in accumulator and set next stage
                protocolState = AM_D_FIRST_RECORD_RECEIVER_READY_SEND - 1;
            }
            else
            {
                if(islast)
                {
                    //Now assemble the complete frame
                    std::vector<std::vector<DLMSVariant>> buffer;
                    if(!clientSecured->CheckProfileGenericBatchRecordsResponseFrame(buffer))
                    {
                        //qDebug() << clientSecured->ErrorMessage();
                        has_error = true;
                    }
                    else
                    {
                        completionParamters.append(QVariant((uint32_t)buffer[0][1].dateTime.ToUnixTime()));
                        completionParamters.append(QVariant((float)buffer[0][3].ToInteger()));
                        completionParamters.append(QVariant((float)buffer[0][4].ToInteger()));
                        completionParamters.append(QVariant((float)buffer[0][5].ToInteger()));
                        completionParamters.append(QVariant((float)buffer[0][6].ToInteger()));

                        //qDebug() << "FIRST RECORD LP2 RECEIVED";
                        protocolState = AM_ACCOUNT_TYPE_SEND - 1;
                    }
                }
                else
                {
                    protocolState = AM_D_FIRST_RECORD_RECEIVER_READY_SEND - 1;
                }
            }
            break;
        }

        case AM_ACCOUNT_TYPE_RECV:
        {
            if(!clientSecured->CheckPrePaidResponseFrame(buffer, OBIS_ACCOUNT_STATUS, accountObject, isfound, 2))
            {
                //qDebug() << clientSecured->ErrorMessage() << protocolState;
                has_error = true;
            }
            else
            {
                int ac_type = (int)accountObject.GetPaymentMode();
                completionParamters.append(QVariant((int)ac_type));
                //qDebug() << "ACCOUNT TYPE" << ac_type;
            }
            break;
        }

        case AM_ACCOUNT_CURRENCY_AND_SCALAR_RECV:
        {
            if(!clientSecured->CheckPrePaidResponseFrame(buffer, OBIS_ACCOUNT_STATUS, accountObject, isfound, 15))
            {
                //qDebug() << clientSecured->ErrorMessage() << protocolState;
                has_error = true;
            }
            else
            {
                char s = accountObject.GetCurrency().GetScale();
                int scale_s = 0;

                if(s > 127)
                {
                    scale_s = s - 256;
                }
                else
                {
                    scale_s = s;
                }

                QString currency_name = accountObject.GetCurrency().GetName().c_str();

                completionParamters.append(QVariant(currency_name));
                completionParamters.append(QVariant(scale_s));
                //qDebug() << "AVAILABLE CREDIT UNIT " << currency_name << scale_s;
            }

            break;
        }

        case AM_DELIVERED_ACTIVE_ENERGY_SCALAR_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_DELIVERED_ACTIVE_ENERGY, responseObject))
            {
                //qDebug() << clientSecured->ErrorMessage() << protocolState;
                has_error = true;
            }
            else
            {
                completionParamters.append(QVariant((unsigned int)responseObject));
                //qDebug() << "DELIVERED ACTIVE ENERGY SCALAR" << responseObject;
            }
            break;
        }

        case AM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SCALAR_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_DELIVERED_ACTIVE_ENERGY, responseObject, 3))
            {
                //qDebug() << clientSecured->ErrorMessage() << protocolState;
                has_error = true;
            }
            else
            {
                completionParamters.append(QVariant((int)responseObject));
                //qDebug() << "INSTANTENOUS POWER SCALAR" << responseObject;
            }
            break;
        }

        case AM_ACTIVE_IMPORT_SCALAR_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_ACTIVE_ENERGY_IMPORT_TARIFF1, responseObject, 3))
            {
                //qDebug() << clientSecured->ErrorMessage() << protocolState;
                has_error = true;
            }
            else
            {
                completionParamters.append(QVariant((int)responseObject));
                //qDebug() << "ACTIVE IMPORT ENERGY SCALAR" << responseObject;
            }
            break;
        }

        case AM_ACTIVE_EXPORT_SCALAR_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_ACTIVE_ENERGY_EXPORT_TARIFF1, responseObject, 3))
            {
                //qDebug() << clientSecured->ErrorMessage() << protocolState;
                has_error = true;
            }
            else
            {
                completionParamters.append(QVariant((int)responseObject));
                //qDebug() << "ACTIVE EXPORT ENERGY SCALAR" << responseObject;
            }
            break;
        }

        case AM_REACTIVE_IMPORT_SCALAR_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_REACTIVE_ENERGY_IMPORT_TARIFF1, responseObject, 3))
            {
                //qDebug() << clientSecured->ErrorMessage() << protocolState;
                has_error = true;
            }
            else
            {
                completionParamters.append(QVariant((int)responseObject));
                //qDebug() << "REACTIVE IMPORT ENERGY SCALAR" << responseObject;
            }
            break;
        }

        case AM_REACTIVE_EXPORT_SCALAR_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_REACTIVE_ENERGY_EXPORT_TARIFF1, responseObject, 3))
            {
                //qDebug() << clientSecured->ErrorMessage() << protocolState;
                has_error = true;
            }
            else
            {
                completionParamters.append(QVariant((int)responseObject));
                //qDebug() << "REACTIVE EXPORT ENERGY SCALAR" << responseObject;
            }
            break;
        }

        case AM_SECURED_REL_RECV:
        {
            if(!clientSecured->CheckReleaseResponse(buffer))
            {
                has_error = true;
            }
            else
            {
                //qDebug() << "SECURED CLIENT RELEASED";
            }
            break;
        }

        case AM_SECURED_DISCN_RECV:
        {
            if(!clientSecured->CheckDisconnectResponse(buffer))
            {
                has_error = true;
            }
            else
            {
                //qDebug() << "SECURED CLIENT DFISCONNECTED";
            }
            break;
        }

        default:
            qDebug() << "ERROR: UNHANDLED PROTOCOL STATE : " << protocolState;
            break;
    }

    if(protocolState == AM_COMPLETE)
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
        protocolState = AM_COMPLETE;
    }
}


#include "WorkFlowAddMeter.hpp"
#include "Controller.hpp"
#include "EntityMeterSettings.hpp"
#include "Mk7Mi.hpp"

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

const int AM_ACCOUNT_TYPE_SEND = 39;
const int AM_ACCOUNT_TYPE_RECV = 40;

const int AM_ACCOUNT_CURRENCY_AND_SCALAR_SEND = 41;
const int AM_ACCOUNT_CURRENCY_AND_SCALAR_RECV = 42;

const int AM_DELIVERED_ACTIVE_ENERGY_SCALAR_SEND = 43;
const int AM_DELIVERED_ACTIVE_ENERGY_SCALAR_RECV = 44;

const int AM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SCALAR_SEND = 45;
const int AM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SCALAR_RECV = 46;

const int AM_ACTIVE_IMPORT_SCALAR_SEND = 47;
const int AM_ACTIVE_IMPORT_SCALAR_RECV = 48;

const int AM_ACTIVE_EXPORT_SCALAR_SEND = 49;
const int AM_ACTIVE_EXPORT_SCALAR_RECV = 50;

const int AM_REACTIVE_IMPORT_SCALAR_SEND = 51;
const int AM_REACTIVE_IMPORT_SCALAR_RECV = 52;

const int AM_REACTIVE_EXPORT_SCALAR_SEND = 53;
const int AM_REACTIVE_EXPORT_SCALAR_RECV = 54;

const int AM_DELIVERED_ACTIVE_ENERGY_SEND = 55;
const int AM_DELIVERED_ACTIVE_ENERGY_RECV = 56;

const int AM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SEND = 57;
const int AM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_RECV = 58;

const int AM_ACCOUNT_USED_CREDIT_SEND = 59;
const int AM_ACCOUNT_USED_CREDIT_RECV = 60;

const int AM_ACCOUNT_TOTAL_CREDIT_SEND = 61;
const int AM_ACCOUNT_TOTAL_CREDIT_RECV = 62;

const int AM_ACCOUNT_REMAINING_CREDIT_SEND = 63;
const int AM_ACCOUNT_REMAINING_CREDIT_RECV = 64;

const int AM_COMPLETE = 65;

////////////////////////////////////

Meter newMeter;
MeterSettings ms0;
MeterSettings ms1;
MeterSettings ms2;
MeterSettings ms3;
MeterSettings ms4;
MeterSettings ms5;
MeterSettings ms6;
MeterSettings ms7;
MeterSettings ms8;

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
    newMeter.Clear();

    ms0.Clear();
    ms1.Clear();
    ms2.Clear();
    ms3.Clear();
    ms4.Clear();
    ms5.Clear();
    ms6.Clear();
    ms7.Clear();
    ms8.Clear();

    has_error = false;
    if(startParamters.count() < 1)
    {
        return;
    }
    else
    {
        systemtitle.Set(startParamters.at(0).toByteArray().data(), startParamters.at(0).toByteArray().size());
    }

    // We expect the system title and network identifier
    std::string str;
    str = Helpers::BytesToHex((unsigned char*)startParamters.at(0).toByteArray().data(), startParamters.at(0).toByteArray().size());
    newMeter.SystemTitle_ = str.c_str();
    newMeter.CustomerPIN_ = startParamters.at(1).toString();
    newMeter.PairingPIN_ = startParamters.at(2).toString();
    newMeter.RSSIStr_ = startParamters.at(3).toString();;

    //Always retreive all the clients
    clientPublic = DlmsClient::GetClient(CLIENT_PUBLIC);
    clientPairing = DlmsClient::GetClient(CLIENT_PAIRING);

    secured_client_index = CLIENT_SECURED_103;
    clientSecured = DlmsClient::GetClient(secured_client_index);

    clientPublic->SetSystemTitle(systemtitle.GetData(), 8);
    clientPublic->EnableCustomChallenge(false);
    clientPublic->InitializeClient();

    protocolState = AM_PUBLIC_SNRM_SEND;

    old_deliveredEnergy_ = 0;
    old_instantaneousPower_ = 0;
    old_usedCredit_ = 0;
    old_grossCredit_ = 0;
    old_remainingCredit_ = 0;

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
            emit controllerInstance->workflowProgress(1);
            break;
        }
        case AM_PUBLIC_AARQ_SEND:
        {
            clientPublic->CreateAARQFrame(buffer);
            emit controllerInstance->workflowProgress(4);
            break;
        }
        case AM_PUBLIC_FCTR_PAIRING_SEND:
        {
            clientPublic->CreateFrameCounterFrame(buffer, OBIS_FRAME_COUNTER_PAIRING);
            emit controllerInstance->workflowProgress(7);
            break;
        }
        case AM_PUBLIC_FCTR_SECURED103_SEND:
        {
            clientPublic->CreateFrameCounterFrame(buffer, OBIS_FRAME_COUNTER_103);
            emit controllerInstance->workflowProgress(11);
            break;
        }
        case AM_PUBLIC_FCTR_SECURED104_SEND:
        {
            clientPublic->CreateFrameCounterFrame(buffer, OBIS_FRAME_COUNTER_104);
            emit controllerInstance->workflowProgress(14);
            break;
        }
        case AM_PUBLIC_FCTR_SECURED105_SEND:
        {
            clientPublic->CreateFrameCounterFrame(buffer, OBIS_FRAME_COUNTER_105);
            emit controllerInstance->workflowProgress(18);
            break;
        }
        case AM_PUBLIC_REL_SEND:
        {
            clientPublic->CreateReleaseFrame(buffer);
            emit controllerInstance->workflowProgress(21);
            break;
        }
        case AM_PUBLIC_DISCN_SEND:
        {
            clientPublic->CreateDisconnectFrame(buffer);
            emit controllerInstance->workflowProgress(24);
            break;
        }

        case AM_PAIRING_SNRM_SEND:
        {
            clientPairing->CreateSNRMFrame(buffer);
            emit controllerInstance->workflowProgress(27);
            break;
        }
        case AM_PAIRING_AARQ_SEND:
        {
            clientPairing->CreateAARQFrame(buffer);
            emit controllerInstance->workflowProgress(30);
            break;
        }
        case AM_PAIRING_AUTH_SEND:
        {
            clientPairing->CreateAuthenticationFrame(buffer);
            emit controllerInstance->workflowProgress(34);
            break;
        }
        case AM_PAIRING_CIUR_SEND:
        {
            clientPairing->CreateObjectRetreiveRequestFrame(buffer, OBIS_CIU_KEYS);
            emit controllerInstance->workflowProgress(37);
            break;
        }
        case AM_PAIRING_REL_SEND:
        {
            clientPairing->CreateReleaseFrame(buffer);
            emit controllerInstance->workflowProgress(40);
            break;
        }
        case AM_PAIRING_DISCN_SEND:
        {
            clientPairing->CreateDisconnectFrame(buffer);
            emit controllerInstance->workflowProgress(43);
            break;
        }

        case AM_SECURED_SNRM_SEND:
        {
            clientSecured->CreateSNRMFrame(buffer);
            emit controllerInstance->workflowProgress(47);
            break;
        }
        case AM_SECURED_AARQ_SEND:
        {
            clientSecured->CreateAARQFrame(buffer);
            emit controllerInstance->workflowProgress(51);
            break;
        }
        case AM_SECURED_AUTH_SEND:
        {
            clientSecured->CreateAuthenticationFrame(buffer);
            emit controllerInstance->workflowProgress(54);
            break;
        }
        case AM_SECURED_SERNO_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_METER_SERIAL_NO);
            emit controllerInstance->workflowProgress(57);
            break;
        }
        case AM_SECURED_MNAME_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_LOGICAL_DEVICE_NAME);
            emit controllerInstance->workflowProgress(60);
            break;
        }
        case AM_ACCOUNT_TYPE_SEND:
        {
            clientSecured->CreateAccountStatusRequestFrame(buffer, OBIS_ACCOUNT_STATUS, 2);
            emit controllerInstance->workflowProgress(80);
            break;
        }
        case AM_ACCOUNT_CURRENCY_AND_SCALAR_SEND:
        {
            clientSecured->CreateAccountStatusRequestFrame(buffer, OBIS_ACCOUNT_STATUS, 15);
            emit controllerInstance->workflowProgress(84);
            break;
        }
        case AM_DELIVERED_ACTIVE_ENERGY_SCALAR_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER);
            emit controllerInstance->workflowProgress(88);
            break;
        }
        case AM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SCALAR_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER, 3);
            emit controllerInstance->workflowProgress(90);
            break;
        }
        case AM_ACTIVE_IMPORT_SCALAR_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_ACTIVE_ENERGY_IMPORT_TARIFF1, 3);
            emit controllerInstance->workflowProgress(91);
            break;
        }
        case AM_ACTIVE_EXPORT_SCALAR_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_ACTIVE_ENERGY_EXPORT_TARIFF1, 3);
            emit controllerInstance->workflowProgress(92);
            break;
        }
        case AM_REACTIVE_IMPORT_SCALAR_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_REACTIVE_ENERGY_IMPORT_TARIFF1, 3);
            emit controllerInstance->workflowProgress(93);
            break;
        }
        case AM_REACTIVE_EXPORT_SCALAR_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_REACTIVE_ENERGY_EXPORT_TARIFF1, 3);
            emit controllerInstance->workflowProgress(94);
            break;
        }


        case AM_DELIVERED_ACTIVE_ENERGY_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_DELIVERED_ACTIVE_ENERGY);
            emit controllerInstance->workflowProgress(95);
            break;
        }

        case AM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER);
            emit controllerInstance->workflowProgress(96);
            break;
        }

        case AM_ACCOUNT_USED_CREDIT_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_TOTAL_CREDIT_USED);
            emit controllerInstance->workflowProgress(97);
            break;
        }

        case AM_ACCOUNT_TOTAL_CREDIT_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_TOTAL_CREDIT_PURCHASED);
            emit controllerInstance->workflowProgress(98);
            break;
        }

        case AM_ACCOUNT_REMAINING_CREDIT_SEND:
        {
            clientSecured->CreateAccountStatusRequestFrame(buffer, OBIS_ACCOUNT_STATUS, 5);
            emit controllerInstance->workflowProgress(98);
            break;
        }

        case AM_COMPLETE:
        {
            break;
        }
        default:
        {
            controllerInstance->Trace("Add Meter: Unhandled Protocol State : " + QVariant((uint32_t)protocolState).toString());
            break;
        }
    }

    if(protocolState == AM_COMPLETE)
    {
        if(!has_error)
        {
            newMeter.ModelNo_ = "Mk7Mi";
            newMeter.LastSync_ = QDateTime::currentDateTime().toSecsSinceEpoch();
            newMeter.StatusStr_ = "1";
            newMeter.IsSelected_ = 1;
            newMeter.LastSyncStr_ = QDateTime::fromSecsSinceEpoch(newMeter.LastSync_).toString("dd MMM yy hh:mm");

            controllerInstance->global_scalar_ = QVariant(ms2.Value_).toInt();
            controllerInstance->deliveredEnergy_scalar = QVariant(ms3.Value_).toInt();
            controllerInstance->instantaneousPower_scalar = QVariant(ms4.Value_).toInt();

            // Set all connected meters to disconnected mode
            meterEntityPtr->allMeters(controllerInstance->meterList_, controllerInstance->isDemoMode_);
            for(int x = 0; x < controllerInstance->meterList_.count(); x++)
            {
                if(controllerInstance->meterList_.at(x).SerialNo_ == controllerInstance->currentMeterSerialNo_)
                {
                    meterEntityPtr->update("serial_no", controllerInstance->meterList_.at(x).SerialNo_, "meter_status", "0");
                }
            }

            meterEntityPtr->createMeter(newMeter);
            meterSettingsEntityPtr->createMeterSettings(ms0);
            meterSettingsEntityPtr->createMeterSettings(ms1);
            meterSettingsEntityPtr->createMeterSettings(ms2);
            meterSettingsEntityPtr->createMeterSettings(ms3);
            meterSettingsEntityPtr->createMeterSettings(ms4);
            meterSettingsEntityPtr->createMeterSettings(ms5);
            meterSettingsEntityPtr->createMeterSettings(ms6);
            meterSettingsEntityPtr->createMeterSettings(ms7);
            meterSettingsEntityPtr->createMeterSettings(ms8);
        }
        else
        {
            controllerInstance->Trace("No meter added, there wad workflow errors");
        }

        emit AddMeterCompleted(newMeter.SerialNo_, !has_error);
        return;
    }

    if(buffer.length() > 0)
    {
        btInterfacePtr->Send(buffer);
        protocolState++;
    }
    else
    {
        controllerInstance->Trace("Add Meter: Protocol Buffer Not Ready : " + QVariant((uint32_t)protocolState).toString());
    }
}

void WorkFlowAddMeter::ProcessReceivedFrame(const QByteArray &buffer)
{
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
            break;
        }
        case AM_PUBLIC_AARE_RECV:
        {
            if(!clientPublic->CheckAAREFrame(buffer))
            {
                has_error = true;
            }
            break;
        }
        case AM_PUBLIC_FCTR_PAIRING_RECV:
        {
            if(!clientPublic->CheckFrameCounterFrame(buffer, AM_FRAME_COUNTER_PAIRING))
            {
                has_error = true;
            }
            break;
        }
        case AM_PUBLIC_FCTR_SECURED103_RECV:
        {
            if(!clientPublic->CheckFrameCounterFrame(buffer, AM_FRAME_COUNTER_SECURED_103))
            {
                has_error = true;
            }
            break;
        }
        case AM_PUBLIC_FCTR_SECURED104_RECV:
        {
            if(!clientPublic->CheckFrameCounterFrame(buffer, AM_FRAME_COUNTER_SECURED_104))
            {
                has_error = true;
            }
            break;
        }
        case AM_PUBLIC_FCTR_SECURED105_RECV:
        {
            if(!clientPublic->CheckFrameCounterFrame(buffer, AM_FRAME_COUNTER_SECURED_105))
            {
                has_error = true;
            }
            break;
        }
        case AM_PUBLIC_REL_RECV:
        {
            if(!clientPublic->CheckReleaseResponse(buffer))
            {
                has_error = true;
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
            break;
        }

        case AM_PAIRING_AARE_RECV:
        {
            if(!clientPairing->CheckAAREFrame(buffer))
            {
                has_error = true;
            }
            break;
        }

        case AM_PAIRING_AUTH_RECV:
        {
            if(!clientPairing->CheckAuthenticationResponse(buffer))
            {
                has_error = true;
            }
            break;
        }

        case AM_PAIRING_CIUR_RECV:
        {
            DLMSVariant cosemObject;
            if(!clientPairing->CheckObjectRetreiveResponseFrame(buffer, cosemObject, OBIS_CIU_KEYS))
            {
                has_error = true;
            }
            else
            {              
                DlmsClient* clientSecured103 = DlmsClient::GetClient(CLIENT_SECURED_103);
                DlmsClient* clientSecured104 = DlmsClient::GetClient(CLIENT_SECURED_104);
                DlmsClient* clientSecured105 = DlmsClient::GetClient(CLIENT_SECURED_105);
                
                DlmsClient::ExtractKeys(cosemObject, clientSecured103, clientSecured104, clientSecured105);

                unsigned char key_buffer[32];
                std::string str;

                memset(key_buffer, 0, 32);
                memcpy(&key_buffer[0], clientSecured103->GetAuthenticationKey().GetData(), 16);
                memcpy(&key_buffer[16], clientSecured103->GetCipheringKey().GetData(), 16);
                str = Helpers::BytesToHex(key_buffer, 32);
                newMeter.Keys103_ = str.c_str();

                memset(key_buffer, 0, 32);
                memcpy(&key_buffer[0], clientSecured104->GetAuthenticationKey().GetData(), 16);
                memcpy(&key_buffer[16], clientSecured104->GetCipheringKey().GetData(), 16);
                str = Helpers::BytesToHex(key_buffer, 32);
                newMeter.Keys104_ = str.c_str();

                memset(key_buffer, 0, 32);
                memcpy(&key_buffer[0], clientSecured105->GetAuthenticationKey().GetData(), 16);
                memcpy(&key_buffer[16], clientSecured105->GetCipheringKey().GetData(), 16);
                str = Helpers::BytesToHex(key_buffer, 32);
                newMeter.Keys105_ = str.c_str();
            }
            break;
        }

        case AM_PAIRING_REL_RECV:
        {
            if(!clientPairing->CheckReleaseResponse(buffer))
            {
                has_error = true;
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
            break;
        }

        case AM_SECURED_AARE_RECV:
        {
            if(!clientSecured->CheckAAREFrame(buffer))
            {
                has_error = true;
                secured_client_index++;
                controllerInstance->Trace("Incrementing client id");
                if(secured_client_index > CLIENT_SECURED_105)
                {
                    controllerInstance->Trace("Error in the workflow, tries all client IDs");
                    has_error = true;
                }
                else
                {
                    has_error = false;
                    protocolState = AM_SECURED_SNRM_SEND - 1;
                }
            }
            break;
        }

        case AM_SECURED_AUTH_RECV:
        {
            if(!clientSecured->CheckAuthenticationResponse(buffer))
            {
                has_error = true;
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
            break;
        }

        case AM_SECURED_SERNO_RECV:
        {
            DLMSVariant cosemObject;
            if(!clientSecured->CheckObjectRetreiveResponseFrame(buffer, cosemObject, OBIS_METER_SERIAL_NO))
            {
                has_error = true;
            }
            else
            {
                QString serialno;
                clientSecured->ExtractStringData(cosemObject, serialno);
                newMeter.SerialNo_ = serialno;
            }
            break;
        }

        case AM_SECURED_MNAME_RECV:
        {
            DLMSVariant cosemObject;
            if(!clientSecured->CheckObjectRetreiveResponseFrame(buffer, cosemObject, OBIS_LOGICAL_DEVICE_NAME))
            {
                has_error = true;
            }
            else
            {
                clientSecured->ExtractStringData(cosemObject, metername);
                newMeter.MeterName_ = metername;
            }
            break;
        }

        case AM_ACCOUNT_TYPE_RECV:
        {
            if(!clientSecured->CheckAccountStatusResponseFrame(buffer, OBIS_ACCOUNT_STATUS, accountObject, isfound, 2))
            {
                has_error = true;
            }
            else
            {
                int ac_type = (int)accountObject.GetPaymentMode();
                ms0.SerialNo_ = newMeter.SerialNo_;
                ms0.Key_ = ACCOUNT_TYPE;
                ms0.Value_ = QVariant(ac_type).toString();
            }
            break;
        }

        case AM_ACCOUNT_CURRENCY_AND_SCALAR_RECV:
        {
            if(!clientSecured->CheckAccountStatusResponseFrame(buffer, OBIS_ACCOUNT_STATUS, accountObject, isfound, 15))
            {
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

                ms1.SerialNo_ = newMeter.SerialNo_;
                ms1.Key_ = ACCOUNT_CURRENCY;
                ms1.Value_ = currency_name;

                ms2.SerialNo_ = newMeter.SerialNo_;
                ms2.Key_ = ACCOUNT_CURRENCY_SCALAR;
                ms2.Value_ = QVariant(scale_s).toString();
             }

            break;
        }

        case AM_DELIVERED_ACTIVE_ENERGY_SCALAR_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_DELIVERED_ACTIVE_ENERGY, responseObject))
            {
                has_error = true;
            }
            else
            {
                ms3.SerialNo_ = newMeter.SerialNo_;
                ms3.Key_ = ACTIVE_ENERGY_SCALAR;
                ms3.Value_ = QVariant(responseObject).toString();
            }
            break;
        }

        case AM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SCALAR_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_DELIVERED_ACTIVE_ENERGY, responseObject, 3))
            {
                has_error = true;
            }
            else
            {
                ms4.SerialNo_ = newMeter.SerialNo_;
                ms4.Key_ = INSTANTENOUS_POWER_SCALAR;
                ms4.Value_ = QVariant(responseObject).toString();
            }
            break;
        }

        case AM_ACTIVE_IMPORT_SCALAR_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_ACTIVE_ENERGY_IMPORT_TARIFF1, responseObject, 3))
            {
                has_error = true;
            }
            else
            {
                ms5.SerialNo_ = newMeter.SerialNo_;
                ms5.Key_ = ACTIVE_IMPORT_SCALAR;
                ms5.Value_ = QVariant(responseObject).toString();
            }
            break;
        }

        case AM_ACTIVE_EXPORT_SCALAR_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_ACTIVE_ENERGY_EXPORT_TARIFF1, responseObject, 3))
            {
                has_error = true;
            }
            else
            {
                ms6.SerialNo_ = newMeter.SerialNo_;
                ms6.Key_ = ACTIVE_EXPORT_SCALAR;
                ms6.Value_ = QVariant(responseObject).toString();
            }
            break;
        }

        case AM_REACTIVE_IMPORT_SCALAR_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_REACTIVE_ENERGY_IMPORT_TARIFF1, responseObject, 3))
            {
                has_error = true;
            }
            else
            {
                ms7.SerialNo_ = newMeter.SerialNo_;
                ms7.Key_ = REACTIVE_EXPORT_SCALAR;
                ms7.Value_ = QVariant(responseObject).toString();
            }
            break;
        }

        case AM_REACTIVE_EXPORT_SCALAR_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_REACTIVE_ENERGY_EXPORT_TARIFF1, responseObject, 3))
            {
                has_error = true;
            }
            else
            {
                ms8.SerialNo_ = newMeter.SerialNo_;
                ms8.Key_ = REACTIVE_IMPORT_SCALAR;
                ms8.Value_ = QVariant(responseObject).toString();
            }
            break;
        }

        case AM_DELIVERED_ACTIVE_ENERGY_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_DELIVERED_ACTIVE_ENERGY, responseObject))
            {
                has_error = true;
                controllerInstance->deliveredEnergy_ = 0;
            }
            else
            {
                controllerInstance->deliveredEnergy_ = responseObject;
                protocolState =  AM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SEND - 1;
            }
            break;
        }

        case AM_TOTAL_INSTANTANEOUS_ACTIVE_POWER_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER, responseObject))
            {
                has_error = true;
                controllerInstance->instantaneousPower_ = 0;
            }
            else
            {
                controllerInstance->instantaneousPower_ = responseObject;
                protocolState =  AM_ACCOUNT_USED_CREDIT_SEND - 1;
            }
            break;
        }

        case AM_ACCOUNT_USED_CREDIT_RECV:
        {
            DLMSVariant cosemObject;
            if(!clientSecured->CheckObjectRetreiveResponseFrame(buffer, cosemObject, OBIS_TOTAL_CREDIT_USED))
            {
                has_error = true;
                controllerInstance->usedCredit_ = 0;
            }
            else
            {
                long val;
                clientSecured->ExtractIntegerData(cosemObject, val);
                controllerInstance->usedCredit_ = val;
                protocolState =  AM_ACCOUNT_TOTAL_CREDIT_SEND - 1;
            }

            break;
        }

        case AM_ACCOUNT_TOTAL_CREDIT_RECV:
        {
            DLMSVariant cosemObject;
            if(!clientSecured->CheckObjectRetreiveResponseFrame(buffer, cosemObject, OBIS_TOTAL_CREDIT_PURCHASED))
            {
                has_error = true;
                controllerInstance->grossCredit_ = 0;
            }
            else
            {
                long val;
                clientSecured->ExtractIntegerData(cosemObject, val);
                controllerInstance->grossCredit_ = val;
                protocolState =  AM_ACCOUNT_REMAINING_CREDIT_SEND - 1;
            }

            break;
        }

        case AM_ACCOUNT_REMAINING_CREDIT_RECV:
        {
            DLMSVariant cosemObject;
            if(!clientSecured->CheckAccountStatusResponseFrame(buffer, OBIS_ACCOUNT_STATUS, accountObject, isfound, 5))
            {
                has_error = true;
                controllerInstance->remainingCredit_ = old_remainingCredit_;
            }
            else
            {
                controllerInstance->remainingCredit_ = accountObject.GetAvailableCredit();
                protocolState =  AM_COMPLETE - 1;
            }

            controllerInstance->remainingCreditPercent_ = (((float)controllerInstance->remainingCredit_/(float)controllerInstance->grossCredit_)*100);

            break;
        }

        default:
            controllerInstance->Trace("Add Meter: Unhandled Protocol State : " + QVariant((uint32_t)protocolState).toString());
            break;
    }

    if(protocolState == AM_COMPLETE)
    {
        return;
    }

    if(!has_error)
    {
        protocolState++;
    }
    else
    {
        if(protocolState <= AM_PUBLIC_DISCN_RECV)
        {
            controllerInstance->Trace(clientPublic->ErrorMessage() + QString(" ") + QVariant((uint32_t)protocolState).toString());
        }

        if(protocolState <= AM_PAIRING_DISCN_RECV && protocolState > AM_PUBLIC_DISCN_RECV)
        {
            controllerInstance->Trace(clientPairing->ErrorMessage() + QString(" ") + QVariant((uint32_t)protocolState).toString());
        }

        if(protocolState <= AM_ACCOUNT_TOTAL_CREDIT_RECV && protocolState > AM_PAIRING_DISCN_RECV)
        {
            controllerInstance->Trace(clientSecured->ErrorMessage() + QString(" ") + QVariant((uint32_t)protocolState).toString());
        }

        protocolState = AM_COMPLETE;
    }

    ProcessSendingFrame();
}


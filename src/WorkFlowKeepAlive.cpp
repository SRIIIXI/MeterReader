#include "WorkFlowKeepAlive.hpp"
#include "EntityStatisticsEnergy.hpp"
#include "EntityTokenHistory.hpp"
#include "EntityMeterSettings.hpp"
#include "Controller.hpp"
#include "Mk7Mi.hpp"

const int KA_DELIVERED_ACTIVE_ENERGY_SEND = 1;
const int KA_DELIVERED_ACTIVE_ENERGY_RECV = 2;
const int KA_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SEND = 3;
const int KA_TOTAL_INSTANTANEOUS_ACTIVE_POWER_RECV = 4;
const int KA_ACCOUNT_USED_CREDIT_SEND = 5;
const int KA_ACCOUNT_USED_CREDIT_RECV = 6;
const int KA_ACCOUNT_TOTAL_CREDIT_SEND = 7;
const int KA_ACCOUNT_TOTAL_CREDIT_RECV = 8;
const int KA_ACCOUNT_REMAINING_CREDIT_SEND = 9;
const int KA_ACCOUNT_REMAINING_CREDIT_RECV = 10;

const int KA_SECURED_ALARMS_REG1_SEND = 11;
const int KA_SECURED_ALARMS_REG1_RECV = 12;
const int KA_SECURED_ALARMS_REG2_SEND = 13;
const int KA_SECURED_ALARMS_REG2_RECV = 14;
const int KA_SECURED_ALARMS_REG3_SEND = 15;
const int KA_SECURED_ALARMS_REG3_RECV = 16;

const int KA_HOURLY_CAPTURED_OBJECTS_SEND = 17;
const int KA_HOURLY_CAPTURED_OBJECTS_RECV = 18;
const int KA_HOURLY_CAPTURED_OBJECTS_RECEIVER_READY_SEND = 19;
const int KA_HOURLY_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV = 20;

const int KA_HOURLY_RECORDS_SEND = 21;
const int KA_HOURLY_RECORDS_RECV = 22;
const int KA_HOURLY_RECORDS_RECEIVER_READY_SEND = 23;
const int KA_HOURLY_RECORDS_PARTIAL_FRAME_RECV = 24;

const int KA_DAILY_CAPTURED_OBJECTS_SEND = 25;
const int KA_DAILY_CAPTURED_OBJECTS_RECV = 26;
const int KA_DAILY_CAPTURED_OBJECTS_RECEIVER_READY_SEND = 27;
const int KA_DAILY_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV = 28;

const int KA_DAILY_RECORDS_SEND = 29;
const int KA_DAILY_RECORDS_RECV = 30;
const int KA_DAILY_RECORDS_RECEIVER_READY_SEND = 31;
const int KA_DAILY_RECORDS_PARTIAL_FRAME_RECV = 32;

const int KA_TOKENS_CAPTURED_OBJECTS_SEND = 33;
const int KA_TOKENS_CAPTURED_OBJECTS_RECV = 34;
const int KA_TOKENS_CAPTURED_OBJECTS_RECEIVER_READY_SEND = 35;
const int KA_TOKENS_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV = 36;

const int KA_TOKENS_RECORDS_SEND = 37;
const int KA_TOKENS_RECORDS_RECV = 38;
const int KA_TOKENS_RECORDS_RECEIVER_READY_SEND = 39;
const int KA_TOKENS_RECORDS_PARTIAL_FRAME_RECV = 40;

const int KA_COMPLETE = 41;

///////////////////////////////////////

WorkFlowKeepAlive::WorkFlowKeepAlive(QObject* parent): IWorkFlow(parent)
{
}

void WorkFlowKeepAlive::StartWorkFlow(QList<QVariant> startParamters)
{   
    clientSecured = DlmsClient::GetClient(secured_client_index);
    protocolState = KA_DELIVERED_ACTIVE_ENERGY_SEND;

    old_deliveredEnergy_ = controllerInstance->deliveredEnergy_;
    old_instantaneousPower_ = controllerInstance->instantaneousPower_;
    old_usedCredit_ = controllerInstance->usedCredit_;
    old_grossCredit_ = controllerInstance->grossCredit_;
    old_remainingCredit_ = controllerInstance->remainingCredit_;
    alarmString.clear();

    QList<MeterSettings> msettingslist;
    meterSettingsEntityPtr->selectedMetersSettings(msettingslist, "serial_no", controllerInstance->currentMeterSerialNo_);

    for(int i = 0; i < msettingslist.count(); i++)
    {
        switch(msettingslist.at(i).Key_)
        {

        case 3:
        {
            wkf_currency_scalar = msettingslist.at(i).Value_.toInt();
            break;
        }

        default:
            break;
        }
    }

    ProcessSendingFrame();
}

void WorkFlowKeepAlive::ProcessSendingFrame()
{
    QByteArray buffer;

    switch(protocolState)
    {
        case KA_DELIVERED_ACTIVE_ENERGY_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_DELIVERED_ACTIVE_ENERGY);
            emit controllerInstance->workflowProgress(1);
            break;
        }

        case KA_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER);
            emit controllerInstance->workflowProgress(5);
            break;
        }

        case KA_ACCOUNT_USED_CREDIT_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_TOTAL_CREDIT_USED);
            emit controllerInstance->workflowProgress(15);
            break;
        }

        case KA_ACCOUNT_TOTAL_CREDIT_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_TOTAL_CREDIT_PURCHASED);
            emit controllerInstance->workflowProgress(10);
            break;
        }

        case KA_ACCOUNT_REMAINING_CREDIT_SEND:
        {
            clientSecured->CreateAccountStatusRequestFrame(buffer, OBIS_ACCOUNT_STATUS, 5);
            emit controllerInstance->workflowProgress(10);
            break;
        }

        case KA_SECURED_ALARMS_REG1_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_ALARMS_REGISTER1);
            emit controllerInstance->workflowProgress(20);
            break;
        }

        case KA_SECURED_ALARMS_REG2_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_ALARMS_REGISTER2);
            emit controllerInstance->workflowProgress(25);
            break;
        }

        case KA_SECURED_ALARMS_REG3_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_ALARMS_REGISTER3);
            emit controllerInstance->workflowProgress(30);
            break;
        }

        case KA_HOURLY_CAPTURED_OBJECTS_SEND:
        {
            emit controllerInstance->workflowProgress(35);
            clientSecured->StartProfileGenericDialog(OBIS_PROFILE_HOURLY_LOAD);
            clientSecured->StartBatch();
            clientSecured->CreateProfileGenericCapturedObjectsRequestFrame(buffer);
            break;
        }

        case KA_HOURLY_CAPTURED_OBJECTS_RECEIVER_READY_SEND:
        {
            emit controllerInstance->workflowProgress(40);
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case KA_HOURLY_RECORDS_SEND:
        {
            emit controllerInstance->workflowProgress(45);
            uint32_t startidx;
            statisticsEnergyEntityPtr->getIndexRanges(controllerInstance->currentMeterSerialNo_, startidx, currentIndex, 0);
            clientSecured->StartBatch();

            if(currentIndex == 0 || startidx == 0)
            {
                currentIndex = 1;
            }

            clientSecured->CreateProfileGenericBatchRecordsRequestFrame(buffer, currentIndex, 5);
            break;
        }

        case KA_HOURLY_RECORDS_RECEIVER_READY_SEND:
        {
            emit controllerInstance->workflowProgress(50);
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case KA_DAILY_CAPTURED_OBJECTS_SEND:
        {
            emit controllerInstance->workflowProgress(55);
            clientSecured->StartProfileGenericDialog(OBIS_PROFILE_DAILY_LOAD);
            clientSecured->StartBatch();
            clientSecured->CreateProfileGenericCapturedObjectsRequestFrame(buffer);
            break;
        }

        case KA_DAILY_CAPTURED_OBJECTS_RECEIVER_READY_SEND:
        {
            emit controllerInstance->workflowProgress(60);
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case KA_DAILY_RECORDS_SEND:
        {
            emit controllerInstance->workflowProgress(65);
            uint32_t startidx;
            statisticsEnergyEntityPtr->getIndexRanges(controllerInstance->currentMeterSerialNo_, startidx, currentIndex, 1);
            clientSecured->StartBatch();

            if(currentIndex == 0 || startidx == 0)
            {
                currentIndex = 1;
            }

            clientSecured->CreateProfileGenericBatchRecordsRequestFrame(buffer, currentIndex, 5);
            break;
        }

        case KA_DAILY_RECORDS_RECEIVER_READY_SEND:
        {
            emit controllerInstance->workflowProgress(70);
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case KA_TOKENS_CAPTURED_OBJECTS_SEND:
        {
            emit controllerInstance->workflowProgress(75);
            capturedTokenColumnCount = 0;
            clientSecured->StartProfileGenericDialog(OBIS_PROFILE_TOKEN_HISTORY);
            clientSecured->StartBatch();
            clientSecured->CreateProfileGenericCapturedObjectsRequestFrame(buffer);
            break;
        }

        case KA_TOKENS_CAPTURED_OBJECTS_RECEIVER_READY_SEND:
        {
            emit controllerInstance->workflowProgress(80);
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case KA_TOKENS_RECORDS_SEND:
        {
            emit controllerInstance->workflowProgress(85);
            uint32_t startidx;
            tokenHistoryEntityPtr->getTokenHistoryRanges(controllerInstance->currentMeterSerialNo_, startidx, currentIndex);
            clientSecured->StartBatch();

            if(currentIndex == 0 || startidx == 0)
            {
                currentIndex = 1;
            }

            clientSecured->CreateProfileGenericBatchRecordsRequestFrame(buffer, currentIndex, 5);
            break;
        }

        case KA_TOKENS_RECORDS_RECEIVER_READY_SEND:
        {
            emit controllerInstance->workflowProgress(90);
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case KA_COMPLETE:
        {
            break;
        }
        default:
        {
            controllerInstance->Trace("Keep Alive: Unhandled Protocol State : " + QVariant((uint32_t)protocolState).toString());
            break;
        }
    }

    if(protocolState == KA_COMPLETE)
    {
        emit KeepAliveCompleted(true);
        return;
    }

    if(buffer.length() > 0)
    {
        btInterfacePtr->Send(buffer);
        protocolState++;
    }
    else
    {
        controllerInstance->Trace("Keep Alive: Protocol Buffer Not Ready : " + QVariant((uint32_t)protocolState).toString());
    }
}

void WorkFlowKeepAlive::ProcessReceivedFrame(const QByteArray &buffer)
{
    bool has_error = false;
    bool ispartial = false;
    bool islast = false;
    float responseObject = -1;
    DLMSVariant alarmObject;

    switch(protocolState)
    {
        case KA_DELIVERED_ACTIVE_ENERGY_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_DELIVERED_ACTIVE_ENERGY, responseObject))
            {
                has_error = true;
                controllerInstance->deliveredEnergy_ = old_deliveredEnergy_;
            }
            else
            {
                controllerInstance->deliveredEnergy_ = responseObject;
            }
            break;
        }

        case KA_TOTAL_INSTANTANEOUS_ACTIVE_POWER_RECV:
        {
            if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER, responseObject))
            {
                has_error = true;
                controllerInstance->instantaneousPower_ = old_instantaneousPower_;
            }
            else
            {
                controllerInstance->instantaneousPower_ = responseObject;
            }
            break;
        }

        case KA_ACCOUNT_USED_CREDIT_RECV:
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
            }
            break;
        }

        case KA_ACCOUNT_TOTAL_CREDIT_RECV:
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
            }

            break;
        }

        case KA_ACCOUNT_REMAINING_CREDIT_RECV:
        {
            DLMSAccount accountObject;
            bool isfound = false;
            if(!clientSecured->CheckAccountStatusResponseFrame(buffer, OBIS_ACCOUNT_STATUS, accountObject, isfound, 5))
            {
                has_error = true;
                controllerInstance->remainingCredit_ = old_remainingCredit_;
            }
            else
            {
                controllerInstance->remainingCredit_ = accountObject.GetAvailableCredit();
            }

            controllerInstance->remainingCreditPercent_ = (((float)controllerInstance->remainingCredit_/(float)controllerInstance->grossCredit_)*100);

            break;
        }

        case KA_SECURED_ALARMS_REG1_RECV:
        {
            if(!clientSecured->CheckObjectRetreiveResponseFrame(buffer, alarmObject, OBIS_ALARMS_REGISTER1))
            {
                has_error = true;
                char bitstring[33] = { '0' };
                alarmString += bitstring;
            }
            else
            {
                int alarmstr;
                alarmstr = alarmObject.ToInteger();
                char bitstring[33] = { 0 };
                char* ptr = nullptr;
                ptr = int2bin(alarmstr, &bitstring[0], 32);
                ptr = strreverse(&bitstring[0]);
                alarmString += bitstring;
            }
            break;
        }

        case KA_SECURED_ALARMS_REG2_RECV:
        {
            if(!clientSecured->CheckObjectRetreiveResponseFrame(buffer, alarmObject, OBIS_ALARMS_REGISTER2))
            {
                has_error = true;
                char bitstring[33] = { '0' };
                alarmString += bitstring;
            }
            else
            {
                int alarmstr;
                alarmstr = alarmObject.ToInteger();
                char bitstring[33] = { 0 };
                char* ptr = nullptr;
                ptr = int2bin(alarmstr, &bitstring[0], 32);
                ptr = strreverse(&bitstring[0]);
                alarmString += bitstring;
            }
            break;
        }

        case KA_SECURED_ALARMS_REG3_RECV:
        {
            if(!clientSecured->CheckObjectRetreiveResponseFrame(buffer, alarmObject, OBIS_ALARMS_REGISTER3))
            {
                has_error = true;
                char bitstring[33] = { '0' };
                alarmString += bitstring;
            }
            else
            {
                int alarmstr;
                alarmstr = alarmObject.ToInteger();
                char bitstring[33] = { 0 };
                char* ptr = nullptr;
                ptr = int2bin(alarmstr, &bitstring[0], 32);
                ptr = strreverse(&bitstring[0]);
                alarmString += bitstring;

                char* alarmptr = (char*)calloc(1, alarmString.length()+1);
                memcpy(alarmptr, alarmString.toStdString().c_str(), alarmString.length());

                controllerInstance->activeAlarms_.clear();

                for(int x = 0; x < controllerInstance->alarmFilter_.count(); x++)
                {
                    if(ptr[x] == '1' && controllerInstance->alarmFilter_.at(x).IsSet_)
                    {
                        controllerInstance->activeAlarms_.append(Alarm(controllerInstance->alarmFilter_[x].AlarmName_, controllerInstance->currentMeterSerialNo_, x, QDateTime::currentDateTime().currentSecsSinceEpoch()));
                    }
                }

                free(alarmptr);
                alarmEntityPtr->remove("serial_no", controllerInstance->currentMeterSerialNo_);

                for(int x = 0; x < controllerInstance->activeAlarms_.count(); x++)
                {
                    Alarm a1;

                    a1.AlarmId_ = controllerInstance->activeAlarms_[x].AlarmId_;
                    a1.AlarmName_ = controllerInstance->activeAlarms_[x].AlarmName_;
                    a1.AlarmTimestamp_ = controllerInstance->activeAlarms_[x].AlarmTimestamp_;
                    a1.SerialNo_ = controllerInstance->currentMeterSerialNo_;
                    alarmEntityPtr->createAlarm(a1);
                }

            }
            break;
        }

        case KA_HOURLY_CAPTURED_OBJECTS_RECV:
        case KA_HOURLY_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV:
        {
            QString colnames;
            long capturedColumnCount = 0;
            if(!clientSecured->CheckBatchFrame(buffer, ispartial, islast))
            {
                has_error = true;
                break;
            }

            if(ispartial)
            {
                // If not a complete frame then, keep the buffer in accumulator and set next stage
                protocolState = KA_HOURLY_CAPTURED_OBJECTS_RECEIVER_READY_SEND - 1;
            }
            else
            {
                if(islast)
                {
                    //Now assemble the complete frame
                    if(!clientSecured->CheckProfileGenericCapturedObjectsResponseFrame(colnames, capturedColumnCount))
                    {
                        has_error = true;
                    }
                    else
                    {
                        AddToProfileGenericColumns(colnames);
                    }

                    protocolState = KA_HOURLY_RECORDS_SEND - 1;
                }
                else
                {
                    protocolState = KA_HOURLY_CAPTURED_OBJECTS_RECEIVER_READY_SEND - 1;
                }
            }
            break;
        }
        case KA_HOURLY_RECORDS_RECV:
        case KA_HOURLY_RECORDS_PARTIAL_FRAME_RECV:
        {
            if(!clientSecured->CheckBatchFrame(buffer, ispartial, islast))
            {
                has_error = true;
                break;
            }

            if(ispartial)
            {
                // If not a complete frame then, keep the buffer in accumulator and set next stage
                protocolState = KA_HOURLY_RECORDS_RECEIVER_READY_SEND - 1;
            }
            else
            {
                if(islast)
                {
                    //Now assemble the complete frame
                    std::vector<std::vector<DLMSVariant>> iobuffer;
                    if(!clientSecured->CheckProfileGenericBatchRecordsResponseFrame(iobuffer))
                    {
                        has_error = true;
                    }
                    else
                    {
                        long recTimeStamp = 0;
                        long rows = 0;

                        if(iobuffer.size() > 0)
                        {
                            rows = iobuffer.size();
                        }

                        int clock_pos = -1;

                        int active_imp_pos_tr1 = -1;
                        int active_exp_pos_tr1 = -1;
                        int reactive_imp_pos_tr1 = -1;
                        int reactive_exp_pos_tr1 = -1;

                        int active_imp_pos_tr2 = -1;
                        int active_exp_pos_tr2 = -1;
                        int reactive_imp_pos_tr2 = -1;
                        int reactive_exp_pos_tr2 = -1;

                        clock_pos = FindProfileGenericColumn(OBIS_CLOCK);

                        active_imp_pos_tr1 = FindProfileGenericColumn(OBIS_H_ACTIVE_ENERGY_IMPORT_TARIFF1);
                        active_exp_pos_tr1 = FindProfileGenericColumn(OBIS_H_ACTIVE_ENERGY_EXPORT_TARIFF1);
                        reactive_imp_pos_tr1 = FindProfileGenericColumn(OBIS_H_REACTIVE_ENERGY_IMPORT_TARIFF1);
                        reactive_exp_pos_tr1 = FindProfileGenericColumn(OBIS_H_REACTIVE_ENERGY_EXPORT_TARIFF1);

                        active_imp_pos_tr2 = FindProfileGenericColumn(OBIS_H_ACTIVE_ENERGY_IMPORT_TARIFF2);
                        active_exp_pos_tr2 = FindProfileGenericColumn(OBIS_H_ACTIVE_ENERGY_EXPORT_TARIFF2);
                        reactive_imp_pos_tr2 = FindProfileGenericColumn(OBIS_H_REACTIVE_ENERGY_IMPORT_TARIFF2);
                        reactive_exp_pos_tr2 = FindProfileGenericColumn(OBIS_H_REACTIVE_ENERGY_EXPORT_TARIFF2);

                        for(int x = 0; x < rows; x++)
                        {
                            StatisticsEnergy egr;

                            if(clock_pos > -1)  egr.RecordTimestamp_ =  iobuffer[x][clock_pos].dateTime.ToUnixTime();

                            if(active_imp_pos_tr1 > -1)  egr.ActiveImportTariff1_ = iobuffer[x][active_imp_pos_tr1].ToInteger();
                            if(active_exp_pos_tr1 > -1)  egr.ActiveExportTariff1_ = iobuffer[x][active_exp_pos_tr1].ToInteger();
                            if(reactive_imp_pos_tr1 > -1)  egr.ReactiveImportTariff1_ = iobuffer[x][reactive_imp_pos_tr1].ToInteger();
                            if(reactive_exp_pos_tr1 > -1)  egr.ReactiveExportTariff1_ = iobuffer[x][reactive_exp_pos_tr1].ToInteger();
                            if(active_imp_pos_tr2 > -1)  egr.ActiveImportTariff2_ = iobuffer[x][active_imp_pos_tr2].ToInteger();
                            if(active_exp_pos_tr2 > -1)  egr.ActiveExportTariff2_ = iobuffer[x][active_exp_pos_tr2].ToInteger();
                            if(reactive_imp_pos_tr2 > -1)  egr.ReactiveImportTariff2_ = iobuffer[x][reactive_imp_pos_tr2].ToInteger();
                            if(reactive_exp_pos_tr2 > -1)  egr.ReactiveExportTariff2_ = iobuffer[x][reactive_exp_pos_tr2].ToInteger();

                            egr.RowIndex_ = currentIndex + x;
                            egr.LoadProfileType_ = 0;
                            egr.SerialNo_ = controllerInstance->currentMeterSerialNo_;

                            statisticsEnergyEntityPtr->createStatisticsEnergy(egr);
                        }
                    }

                    protocolState = KA_DAILY_CAPTURED_OBJECTS_SEND - 1;
                }
                else
                {
                    protocolState = KA_HOURLY_RECORDS_RECEIVER_READY_SEND - 1;
                }
            }
            break;
        }
        case KA_DAILY_CAPTURED_OBJECTS_RECV:
        case KA_DAILY_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV:
        {
            QString colnames;
            long capturedColumnCount = 0;
            if(!clientSecured->CheckBatchFrame(buffer, ispartial, islast))
            {
                has_error = true;
                break;
            }

            if(ispartial)
            {
                // If not a complete frame then, keep the buffer in accumulator and set next stage
                protocolState = KA_DAILY_CAPTURED_OBJECTS_RECEIVER_READY_SEND - 1;
            }
            else
            {
                if(islast)
                {
                    //Now assemble the complete frame
                    if(!clientSecured->CheckProfileGenericCapturedObjectsResponseFrame(colnames, capturedColumnCount))
                    {
                        has_error = true;
                    }
                    else
                    {
                        AddToProfileGenericColumns(colnames);
                    }

                    protocolState = KA_DAILY_RECORDS_SEND - 1;
                }
                else
                {
                    protocolState = KA_DAILY_CAPTURED_OBJECTS_RECEIVER_READY_SEND - 1;
                }
            }
            break;
        }
        case KA_DAILY_RECORDS_RECV:
        case KA_DAILY_RECORDS_PARTIAL_FRAME_RECV:
        {
            if(!clientSecured->CheckBatchFrame(buffer, ispartial, islast))
            {
                has_error = true;
                break;
            }

            if(ispartial)
            {
                // If not a complete frame then, keep the buffer in accumulator and set next stage
                protocolState = KA_DAILY_RECORDS_RECEIVER_READY_SEND - 1;
            }
            else
            {
                if(islast)
                {
                    //Now assemble the complete frame
                    std::vector<std::vector<DLMSVariant>> iobuffer;
                    if(!clientSecured->CheckProfileGenericBatchRecordsResponseFrame(iobuffer))
                    {
                        has_error = true;
                    }
                    else
                    {
                        long recTimeStamp = 0;
                        long rows = 0;

                        if(iobuffer.size() > 0)
                        {
                            rows = iobuffer.size();
                        }

                        int clock_pos = -1;

                        int active_imp_pos_tr1 = -1;
                        int active_exp_pos_tr1 = -1;
                        int reactive_imp_pos_tr1 = -1;
                        int reactive_exp_pos_tr1 = -1;

                        int active_imp_pos_tr2 = -1;
                        int active_exp_pos_tr2 = -1;
                        int reactive_imp_pos_tr2 = -1;
                        int reactive_exp_pos_tr2 = -1;

                        clock_pos = FindProfileGenericColumn(OBIS_CLOCK);

                        active_imp_pos_tr1 = FindProfileGenericColumn(OBIS_D_ACTIVE_ENERGY_IMPORT_TARIFF1);
                        active_exp_pos_tr1 = FindProfileGenericColumn(OBIS_D_ACTIVE_ENERGY_EXPORT_TARIFF1);
                        reactive_imp_pos_tr1 = FindProfileGenericColumn(OBIS_D_REACTIVE_ENERGY_IMPORT_TARIFF1);
                        reactive_exp_pos_tr1 = FindProfileGenericColumn(OBIS_D_REACTIVE_ENERGY_EXPORT_TARIFF1);

                        active_imp_pos_tr2 = FindProfileGenericColumn(OBIS_D_ACTIVE_ENERGY_IMPORT_TARIFF2);
                        active_exp_pos_tr2 = FindProfileGenericColumn(OBIS_D_ACTIVE_ENERGY_EXPORT_TARIFF2);
                        reactive_imp_pos_tr2 = FindProfileGenericColumn(OBIS_D_REACTIVE_ENERGY_IMPORT_TARIFF2);
                        reactive_exp_pos_tr2 = FindProfileGenericColumn(OBIS_D_REACTIVE_ENERGY_EXPORT_TARIFF2);

                        for(int x = 0; x < rows; x++)
                        {
                            StatisticsEnergy egr;

                            if(clock_pos > -1)  egr.RecordTimestamp_ =  iobuffer[x][clock_pos].dateTime.ToUnixTime();

                            if(active_imp_pos_tr1 > -1)  egr.ActiveImportTariff1_ = iobuffer[x][active_imp_pos_tr1].ToInteger();
                            if(active_exp_pos_tr1 > -1)  egr.ActiveExportTariff1_ = iobuffer[x][active_exp_pos_tr1].ToInteger();
                            if(reactive_imp_pos_tr1 > -1)  egr.ReactiveImportTariff1_ = iobuffer[x][reactive_imp_pos_tr1].ToInteger();
                            if(reactive_exp_pos_tr1 > -1)  egr.ReactiveExportTariff1_ = iobuffer[x][reactive_exp_pos_tr1].ToInteger();
                            if(active_imp_pos_tr2 > -1)  egr.ActiveImportTariff2_ = iobuffer[x][active_imp_pos_tr2].ToInteger();
                            if(active_exp_pos_tr2 > -1)  egr.ActiveExportTariff2_ = iobuffer[x][active_exp_pos_tr2].ToInteger();
                            if(reactive_imp_pos_tr2 > -1)  egr.ReactiveImportTariff2_ = iobuffer[x][reactive_imp_pos_tr2].ToInteger();
                            if(reactive_exp_pos_tr2 > -1)  egr.ReactiveExportTariff2_ = iobuffer[x][reactive_exp_pos_tr2].ToInteger();

                            egr.RowIndex_ = currentIndex + x;
                            egr.LoadProfileType_ = 1;
                            egr.SerialNo_ = controllerInstance->currentMeterSerialNo_;

                            statisticsEnergyEntityPtr->createStatisticsEnergy(egr);
                        }
                    }

                    protocolState = KA_TOKENS_CAPTURED_OBJECTS_SEND - 1;
                }
                else
                {
                    protocolState = KA_DAILY_RECORDS_RECEIVER_READY_SEND - 1;
                }
            }
            break;
        }

        case KA_TOKENS_CAPTURED_OBJECTS_RECV:
        case KA_TOKENS_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV:
        {
            QString colnames;
            if(!clientSecured->CheckBatchFrame(buffer, ispartial, islast))
            {
                has_error = true;
                break;
            }

            if(ispartial)
            {
                // If not a complete frame then, keep the buffer in accumulator and set next stage
                protocolState = KA_TOKENS_CAPTURED_OBJECTS_RECEIVER_READY_SEND - 1;
            }
            else
            {
                if(islast)
                {
                    //Now assemble the complete frame
                    if(!clientSecured->CheckProfileGenericCapturedObjectsResponseFrame(colnames, capturedTokenColumnCount))
                    {
                        has_error = true;
                    }
                    else
                    {
                        AddToProfileGenericColumns(colnames);
                    }

                    protocolState =  KA_TOKENS_RECORDS_SEND - 1;
                }
                else
                {
                    protocolState =  KA_TOKENS_CAPTURED_OBJECTS_RECEIVER_READY_SEND - 1;
                }
            }
            break;
        }

        case KA_TOKENS_RECORDS_RECV:
        case KA_TOKENS_RECORDS_PARTIAL_FRAME_RECV:
        {
            if(!clientSecured->CheckBatchFrame(buffer, ispartial, islast))
            {
                has_error = true;
                break;
            }

            if(ispartial)
            {
                // If not a complete frame then, keep the buffer in accumulator and set next stage
                protocolState =  KA_TOKENS_RECORDS_RECEIVER_READY_SEND - 1;
            }
            else
            {
                if(islast)
                {
                    //Now assemble the complete frame
                    std::vector<std::vector<DLMSVariant>> iobuffer;
                    if(!clientSecured->CheckProfileGenericBatchRecordsResponseFrame(iobuffer))
                    {
                        has_error = true;
                    }
                    else
                    {
                        long recTimeStamp = 0;
                        long rows = 0;

                        if(iobuffer.size() > 0)
                        {
                            rows = iobuffer.size();
                        }

                        int clock_pos = -1;
                        int token_pos = -1;
                        int value_pos = -1;

                        QList<int32_t> positions = FindProfileGenericColumns(OBIS_TOKEN_HISTORY_CLOCK);

                        if(positions.count() > 0)
                        {
                            clock_pos = positions.at(0);
                        }

                        if(positions.count() > 1)
                        {
                            token_pos = positions.at(1);
                        }

                        value_pos = FindProfileGenericColumn(OBIS_TOKEN_HISTORY_VALUE);

                        for(int x = 0; x < rows; x++)
                        {
                            TokenHistory newToken;
                            ByteBuffer buff, timestampbuff;
                            std::string syst_hex;

                            newToken.SerialNo_ = controllerInstance->currentMeterSerialNo_;

                            syst_hex = iobuffer[x][token_pos].ToString();
                            Helpers::HexToBytes(syst_hex, buff);
                            char temp_token[21] = {0};
                            memcpy(&temp_token[0], buff.GetData(), 20);
                            newToken.TokenId_ = temp_token;

                            newToken.TokenValue_ = iobuffer[x][value_pos].ToInteger()*pow(10, wkf_currency_scalar);
                            newToken.RowIndex_ = currentIndex + x;

                            DLMSVariant clockValue;
                            iobuffer[x][clock_pos].GetBytes(timestampbuff);
                            DLMSClient::ChangeType(timestampbuff, DLMS_DATA_TYPE_DATETIME, clockValue);
                            newToken.AppliedDate_ = clockValue.dateTime.ToUnixTime();
                            newToken.AppliedDateStr_ = QDateTime::fromSecsSinceEpoch(newToken.AppliedDate_).toString("MMMM dd yyyy");

                            // New token from token transfer log
                            if(controllerInstance->IsTokenHistoryExisting(newToken.TokenId_) == false)
                            {
                                tokenHistoryEntityPtr->createTokenHistory(newToken);

                                // Delete from local database
                                tokenEntityPtr->remove("token_id", newToken.TokenId_);
                            }     
                        }

                        controllerInstance->tokenList_.clear();
                        tokenEntityPtr->selectedTokens(controllerInstance->tokenList_, "serial_no", controllerInstance->currentMeterSerialNo_);
                        controllerInstance->tokenHistoryList_.clear();
                        tokenHistoryEntityPtr->selectedTokenHistories(controllerInstance->tokenHistoryList_, "serial_no", controllerInstance->currentMeterSerialNo_);

                        for(int x = 0; x < controllerInstance->tokenHistoryList_.count(); x++)
                        {
                            QString hist_token = controllerInstance->tokenHistoryList_.at(x).TokenId_;

                            if(controllerInstance->IsTokenHistoryExisting(hist_token) == true)
                            {
                                tokenEntityPtr->remove("token_id", hist_token);
                            }
                        }

                        controllerInstance->tokenList_.clear();
                        tokenEntityPtr->selectedTokens(controllerInstance->tokenList_, "serial_no", controllerInstance->currentMeterSerialNo_);

                        emit controllerInstance->tokenAction();
                        protocolState = KA_COMPLETE -1;
                    }
                }
                else
                {
                    protocolState =  KA_TOKENS_RECORDS_RECEIVER_READY_SEND - 1;
                }
            }
            break;
        }

        default:
            controllerInstance->Trace("Keep Alive: Unhandled Protocol State : " + QVariant((uint32_t)protocolState).toString());
            break;
    }

    if(protocolState == KA_COMPLETE)
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
        controllerInstance->Trace("Please check last Tx and Rx");
        protocolState = KA_COMPLETE;
        emit KeepAliveCompleted(false);
    }
}

char* WorkFlowKeepAlive::int2bin(int a, char* buffer, int buf_size)
{
    buffer += (buf_size - 1);

    for (int i = 31; i >= 0; i--)
    {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }

    return buffer;
}

char* WorkFlowKeepAlive::strreverse(char* ptr)
{
    size_t start = 0;

    size_t term = strlen(ptr) - 1;

    while (start < term)
    {
        char temp = ptr[start];
        ptr[start] = ptr[term];
        ptr[term] = temp;
        start++;
        term--;
    }

    return ptr;
}



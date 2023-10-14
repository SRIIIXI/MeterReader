#include "WorkFlowKeepAlive.hpp"
#include "EntityStatisticsEnergy.hpp"
#include "UICommandHandler.hpp"
#include "Mk7Mi.hpp"
#include <QThread>

const int KA_DELIVERED_ACTIVE_ENERGY_SEND = 1;
const int KA_DELIVERED_ACTIVE_ENERGY_RECV = 2;
const int KA_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SEND = 3;
const int KA_TOTAL_INSTANTANEOUS_ACTIVE_POWER_RECV = 4;
const int KA_ACCOUNT_USED_CREDIT_SEND = 5;
const int KA_ACCOUNT_USED_CREDIT_RECV = 6;
const int KA_ACCOUNT_TOTAL_CREDIT_SEND = 7;
const int KA_ACCOUNT_TOTAL_CREDIT_RECV = 8;

const int KA_SECURED_ALARMS_REG1_SEND = 9;
const int KA_SECURED_ALARMS_REG1_RECV = 10;
const int KA_SECURED_ALARMS_REG2_SEND = 11;
const int KA_SECURED_ALARMS_REG2_RECV = 12;
const int KA_SECURED_ALARMS_REG3_SEND = 13;
const int KA_SECURED_ALARMS_REG3_RECV = 14;

const int KA_HOURLY_CAPTURED_OBJECTS_SEND = 15;
const int KA_HOURLY_CAPTURED_OBJECTS_RECV = 16;
const int KA_HOURLY_CAPTURED_OBJECTS_RECEIVER_READY_SEND = 17;
const int KA_HOURLY_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV = 18;

const int KA_HOURLY_RECORDS_SEND = 19;
const int KA_HOURLY_RECORDS_RECV = 20;
const int KA_HOURLY_RECORDS_RECEIVER_READY_SEND = 21;
const int KA_HOURLY_RECORDS_PARTIAL_FRAME_RECV = 22;

const int KA_DAILY_CAPTURED_OBJECTS_SEND = 23;
const int KA_DAILY_CAPTURED_OBJECTS_RECV = 24;
const int KA_DAILY_CAPTURED_OBJECTS_RECEIVER_READY_SEND = 25;
const int KA_DAILY_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV = 26;

const int KA_DAILY_RECORDS_SEND = 27;
const int KA_DAILY_RECORDS_RECV = 28;
const int KA_DAILY_RECORDS_RECEIVER_READY_SEND = 29;
const int KA_DAILY_RECORDS_PARTIAL_FRAME_RECV = 30;

const int KA_TOKENS_CAPTURED_OBJECTS_SEND = 31;
const int KA_TOKENS_CAPTURED_OBJECTS_RECV = 32;
const int KA_TOKENS_CAPTURED_OBJECTS_RECEIVER_READY_SEND = 33;
const int KA_TOKENS_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV = 34;

const int KA_TOKENS_RECORDS_SEND = 35;
const int KA_TOKENS_RECORDS_RECV = 36;
const int KA_TOKENS_RECORDS_RECEIVER_READY_SEND = 37;
const int KA_TOKENS_RECORDS_PARTIAL_FRAME_RECV = 38;

const int KA_COMPLETE = 39;

///////////////////////////////////////

WorkFlowKeepAlive::WorkFlowKeepAlive(QObject* parent): IWorkFlow(parent)
{
}

void WorkFlowKeepAlive::StartWorkFlow(QList<QVariant> startParamters)
{
    clientSecured = CommunicationClient::GetClient(CLIENT_SECURED_103);
    protocolState = KA_DELIVERED_ACTIVE_ENERGY_SEND;
    serialno = globalApplicationData->getCurrentMeter().SerialNo_;
    completionParamters.clear();
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
            break;
        }

        case KA_TOTAL_INSTANTANEOUS_ACTIVE_POWER_SEND:
        {
            clientSecured->CreateRegisterParamRequestFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER);
            break;
        }

        case KA_ACCOUNT_TOTAL_CREDIT_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_TOTAL_CREDIT_PURCHASED);
            break;
        }

        case KA_ACCOUNT_USED_CREDIT_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_TOTAL_CREDIT_USED);
            break;
        }

        case KA_SECURED_ALARMS_REG1_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_ALARMS_REGISTER1);
            break;
        }

        case KA_SECURED_ALARMS_REG2_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_ALARMS_REGISTER2);
            break;
        }

        case KA_SECURED_ALARMS_REG3_SEND:
        {
            clientSecured->CreateObjectRetreiveRequestFrame(buffer, OBIS_ALARMS_REGISTER3);
            break;
        }

        case KA_HOURLY_CAPTURED_OBJECTS_SEND:
        {
            clientSecured->StartProfileGenericDialog("1.0.99.1.0.255");
            clientSecured->StartBatch();
            clientSecured->CreateProfileGenericCapturedObjectsRequestFrame(buffer);
            break;
        }

        case KA_HOURLY_CAPTURED_OBJECTS_RECEIVER_READY_SEND:
        {
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case KA_HOURLY_RECORDS_SEND:
        {
            uint32_t minlp1, maxlp1, startidx;
            statisticsEnergyEntityPtr->getRanges(serialno, startidx, currentIndex, minlp1, maxlp1, 0);
            clientSecured->StartBatch();
            clientSecured->CreateProfileGenericBatchRecordsRequestFrame(buffer, currentIndex, 10);
            break;
        }

        case KA_HOURLY_RECORDS_RECEIVER_READY_SEND:
        {
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case KA_DAILY_CAPTURED_OBJECTS_SEND:
        {
            clientSecured->StartProfileGenericDialog("1.0.99.2.0.255");
            clientSecured->StartBatch();
            clientSecured->CreateProfileGenericCapturedObjectsRequestFrame(buffer);
            break;
        }

        case KA_DAILY_CAPTURED_OBJECTS_RECEIVER_READY_SEND:
        {
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case KA_DAILY_RECORDS_SEND:
        {
            uint32_t minlp1, maxlp1, startidx;
            statisticsEnergyEntityPtr->getRanges(serialno, startidx, currentIndex, minlp1, maxlp1, 1);
            clientSecured->StartBatch();
            clientSecured->CreateProfileGenericBatchRecordsRequestFrame(buffer, currentIndex, 10);
            break;
        }

        case KA_DAILY_RECORDS_RECEIVER_READY_SEND:
        {
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case KA_TOKENS_CAPTURED_OBJECTS_SEND:
        {
            capturedTokenColumnCount = 0;
            clientSecured->StartProfileGenericDialog("0.0.99.15.0.255");
            clientSecured->StartBatch();
            clientSecured->CreateProfileGenericCapturedObjectsRequestFrame(buffer);
            break;
        }

        case KA_TOKENS_CAPTURED_OBJECTS_RECEIVER_READY_SEND:
        {
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case KA_TOKENS_RECORDS_SEND:
        {
            uint32_t startidx;
            tokenEntityPtr->getRanges(serialno, startidx, currentIndex);
            clientSecured->StartBatch();

            if(currentIndex == 0 || startidx == 0)
            {
                currentIndex = 1;
            }

            clientSecured->CreateProfileGenericBatchRecordsRequestFrame(buffer, currentIndex, 10);
            break;
        }

        case KA_TOKENS_RECORDS_RECEIVER_READY_SEND:
        {
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case KA_COMPLETE:
        {
            break;
        }
        default:
        {
            qDebug() << "ERROR: UNHANDLED PROTOCOL STATE : " << protocolState;
            break;
        }
    }

    if(protocolState == KA_COMPLETE)
    {
        completionParamters.append(alarmString);
        emit Completed(completionParamters, KeepAlive, true);
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

void WorkFlowKeepAlive::ProcessReceivedFrame(const QByteArray &buffer)
{
    //std::string str = Helpers::BytesToHex((unsigned char*)buffer.data(), buffer.length());
    //std::stringstream ss;
    //ss << protocolState << " RX [" << buffer.length() << "] " << str.c_str();
    //qDebug() << ss.str().c_str();

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
            //qDebug() << clientSecured->ErrorMessage();
            has_error = true;
        }
        else
        {
            completionParamters.append(QVariant((unsigned int)responseObject));
        }
        break;
    }

    case KA_TOTAL_INSTANTANEOUS_ACTIVE_POWER_RECV:
    {
        if(!clientSecured->CheckRegisterParamResponseFrame(buffer, OBIS_TOTAL_INSTANTANEOUS_ACTIVE_POWER, responseObject))
        {
            //qDebug() << clientSecured->ErrorMessage();
            has_error = true;
        }
        else
        {
            completionParamters.append(QVariant((unsigned int)responseObject));
        }
        break;
    }

    case KA_ACCOUNT_TOTAL_CREDIT_RECV:
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
            completionParamters.append(QVariant((uint32_t)val));
        }
        break;
    }

    case KA_ACCOUNT_USED_CREDIT_RECV:
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
            completionParamters.append(QVariant((uint32_t)val));
        }
        break;
    }

    case KA_SECURED_ALARMS_REG1_RECV:
    {
        if(!clientSecured->CheckObjectRetreiveResponseFrame(buffer, alarmObject, OBIS_ALARMS_REGISTER1))
        {
            //qDebug() << clientSecured->ErrorMessage();
            has_error = true;
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
            //qDebug() << clientSecured->ErrorMessage();
            has_error = true;
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
            //qDebug() << clientSecured->ErrorMessage();
            has_error = true;
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

    case KA_HOURLY_CAPTURED_OBJECTS_RECV:
    case KA_HOURLY_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV:
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
            protocolState = KA_HOURLY_CAPTURED_OBJECTS_RECEIVER_READY_SEND - 1;
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
            //qDebug() << clientSecured->ErrorMessage();
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
                std::vector<std::vector<DLMSVariant>> buffer;
                if(!clientSecured->CheckProfileGenericBatchRecordsResponseFrame(buffer))
                {
                    //qDebug() << clientSecured->ErrorMessage();
                    has_error = true;
                }
                else
                {
                    long recTimeStamp = 0;
                    long rows = 0;

                    if(buffer.size() > 0)
                    {
                        rows = buffer.size();
                    }

                    for(int x = 0; x < rows; x++)
                    {
                        StatisticsEnergy egr;
                        egr.ActiveImportTariff1_ = buffer[x][3].ToInteger();
                        egr.ActiveExportTariff1_ = buffer[x][4].ToInteger();
                        egr.ReactiveImportTariff1_ = buffer[x][5].ToInteger();
                        egr.ReactiveExportTariff1_ = buffer[x][6].ToInteger();
                        egr.ActiveImportTariff2_ = 0;
                        egr.ActiveExportTariff2_ = 0;
                        egr.ReactiveImportTariff2_ = 0;
                        egr.ReactiveExportTariff2_ = 0;
                        egr.RowIndex_ = currentIndex + x;
                        egr.RecordTimestamp_ =  buffer[x][1].dateTime.ToUnixTime();
                        egr.LoadProfileType_ = 0;
                        egr.SerialNo_ = serialno;
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
            //qDebug() << clientSecured->ErrorMessage();
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
                    //qDebug() << clientSecured->ErrorMessage();
                    has_error = true;
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
            //qDebug() << clientSecured->ErrorMessage();
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
                std::vector<std::vector<DLMSVariant>> buffer;
                if(!clientSecured->CheckProfileGenericBatchRecordsResponseFrame(buffer))
                {
                    //qDebug() << clientSecured->ErrorMessage();
                    has_error = true;
                }
                else
                {
                    long recTimeStamp = 0;
                    long rows = 0;

                    if(buffer.size() > 0)
                    {
                        rows = buffer.size();
                    }

                    for(int x = 0; x < rows; x++)
                    {
                        StatisticsEnergy egr;
                        egr.ActiveImportTariff1_ = buffer[x][3].ToInteger();
                        egr.ActiveExportTariff1_ = buffer[x][4].ToInteger();
                        egr.ReactiveImportTariff1_ = buffer[x][5].ToInteger();
                        egr.ReactiveExportTariff1_ = buffer[x][6].ToInteger();
                        egr.ActiveImportTariff2_ = 0;
                        egr.ActiveExportTariff2_ = 0;
                        egr.ReactiveImportTariff2_ = 0;
                        egr.ReactiveExportTariff2_ = 0;
                        egr.RowIndex_ = currentIndex + x;
                        egr.RecordTimestamp_ =  buffer[x][1].dateTime.ToUnixTime();
                        egr.LoadProfileType_ = 1;
                        egr.SerialNo_ = serialno;
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
            //qDebug() << clientSecured->ErrorMessage() << protocolState;
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
                    qDebug() << clientSecured->ErrorMessage() << protocolState;
                    has_error = true;
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
            //qDebug() << clientSecured->ErrorMessage() << protocolState;
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
                std::vector<std::vector<DLMSVariant>> buffer;
                if(!clientSecured->CheckProfileGenericBatchRecordsResponseFrame(buffer))
                {
                    //qDebug() << clientSecured->ErrorMessage() << protocolState;
                    has_error = true;
                }
                else
                {
                    long recTimeStamp = 0;
                    long rows = 0;

                    if(buffer.size() > 0)
                    {
                        rows = buffer.size();
                    }

                    for(int x = 0; x < rows; x++)
                    {
                        Token t1;
                        t1.TokenStatusText_ = "OK";
                        t1.IsApplied_ = true;
                        t1.SerialNo_ = serialno;
                        t1.AppliedDate_ = buffer[x][1].dateTime.ToUnixTime();
                        t1.AppliedDateStr_ = QDateTime::fromSecsSinceEpoch(t1.AppliedDate_).toString("MMMM dd yyyy");
                        ByteBuffer buff;
                        std::string syst_hex = buffer[x][2].ToString();
                        Helpers::HexToBytes(syst_hex, buff);
                        t1.TokenId_ = (char*)buff.GetData();

                        if(capturedTokenColumnCount == 4)
                        {
                            t1.TokenValue_ = buffer[x][3].ToInteger();
                        }
                        else
                        {
                            t1.TokenValue_ = 0;
                        }

                        QString tempts;

                        clientSecured->ExtractStringData(buffer[x][1], tempts);

                        tokenEntityPtr->createToken(t1);
                    }

                    protocolState =  KA_COMPLETE - 1;
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
        qDebug() << "ERROR: UNHANDLED PROTOCOL STATE : " << protocolState;
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
        //emit Completed(completionParamters, KeepAlive, false);
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



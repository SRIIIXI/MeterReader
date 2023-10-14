#include "WorkFlowTokenTransfer.hpp"
#include "UICommandHandler.hpp"
#include "Mk7Mi.hpp"

const int TT_TRANSFER_TOKEN_SEND = 1;
const int TT_TRANSFER_TOKEN_RECV = 2;

const int TT_TOKEN_STATUS_SEND = 3;
const int TT_TOKEN_STATUS_RECV = 4;

const int TT_TOKENS_CAPTURED_OBJECTS_SEND = 5;
const int TT_TOKENS_CAPTURED_OBJECTS_RECV = 6;
const int TT_TOKENS_CAPTURED_OBJECTS_RECEIVER_READY_SEND = 7;
const int TT_TOKENS_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV = 8;

const int TT_TOKENS_RECORDS_SEND = 9;
const int TT_TOKENS_RECORDS_RECV = 10;
const int TT_TOKENS_RECORDS_RECEIVER_READY_SEND = 11;
const int TT_TOKENS_RECORDS_PARTIAL_FRAME_RECV = 12;


const int TT_COMPLETE = 13;

///////////////////////////////////////

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
    "Transfer Status Awaited"
};

WorkFlowTokenTransfer::WorkFlowTokenTransfer(QObject* parent): IWorkFlow(parent)
{
}

void WorkFlowTokenTransfer::StartWorkFlow(QList<QVariant> startParamters)
{
    currentIndex = 0;
    capturedTokenColumnCount = 0;

    tokenStr = startParamters.at(0).toString();
    clientSecured = CommunicationClient::GetClient(CLIENT_SECURED_103);
    protocolState = TT_TRANSFER_TOKEN_SEND;
    completionParamters.clear();
    ProcessSendingFrame();
}

void WorkFlowTokenTransfer::ProcessSendingFrame()
{
    QByteArray buffer;

    switch(protocolState)
    {
        case TT_TRANSFER_TOKEN_SEND:
        {
            qDebug() << "Applying token " << tokenStr;
            clientSecured->CreateTokenTransferRequestFrame(buffer, OBIS_TOKEN_GATEWAY, tokenStr);
            //emit globalApplicationData->tokenTransferProgress("Transfering token");
            emit globalApplicationData->tokenTransferProgress("40%");
            break;
        }

        case TT_TOKEN_STATUS_SEND:
        {
            clientSecured->CreateTokenTransferStatusRequestFrame(buffer, OBIS_TOKEN_GATEWAY);
            emit globalApplicationData->tokenTransferProgress("Querying token status");
            qDebug() << "Querying token status";
            break;
        }

        case TT_TOKENS_CAPTURED_OBJECTS_SEND:
        {
            capturedTokenColumnCount = 0;
            clientSecured->StartProfileGenericDialog("0.0.99.15.0.255");
            clientSecured->StartBatch();
            clientSecured->CreateProfileGenericCapturedObjectsRequestFrame(buffer);
            break;
        }

        case TT_TOKENS_CAPTURED_OBJECTS_RECEIVER_READY_SEND:
        {
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case TT_TOKENS_RECORDS_SEND:
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

        case TT_TOKENS_RECORDS_RECEIVER_READY_SEND:
        {
            clientSecured->CreateReceiverReadyFrame(buffer);
            break;
        }

        case TT_COMPLETE:
        {
            emit globalApplicationData->tokenTransferProgress("100%");
            break;
        }

        default:
        {
            qDebug() << "ERROR: UNHANDLED PROTOCOL STATE : " << protocolState;
            break;
        }
    }

    if(protocolState == TT_COMPLETE)
    {
        completionParamters.clear();
        completionParamters.append(tokenStr);
        completionParamters.append(token_status);
        completionParamters.append(token_status_messages[token_status]);

        if(token_status > 3 && token_status != 8)
        {
            //Token was transferred but the token was not accepted
            emit Completed(completionParamters, TokenTransfer, false);
        }
        else
        {
            emit Completed(completionParamters, TokenTransfer, true);
        }
        return;
    }

    if(buffer.length() > 0)
    {
        //std::string str = Helpers::BytesToHex((unsigned char*)buffer.data(), buffer.length());
        //std::stringstream ss;
        //ss << protocolState << " TX [" << buffer.length() << "] " << str.c_str();
        //qDebug() << ss.str().c_str();

        centralPtr->SendData(buffer);
        protocolState++;
    }
    else
    {
        qDebug() << "PROTOCOL SEND BUFFER NOT READY -> STEP -> " << protocolState;
    }
}

void WorkFlowTokenTransfer::ProcessReceivedFrame(const QByteArray &buffer)
{
    //std::string str = Helpers::BytesToHex((unsigned char*)buffer.data(), buffer.length());
    //std::stringstream ss;
    //ss << protocolState << " RX [" << buffer.length() << "] " << str.c_str();
    //qDebug() << ss.str().c_str();

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
            }
            else
            {
                qDebug() << "Token applied";
                emit globalApplicationData->tokenTransferProgress("Token transferred");
            }

            break;
        }

        case TT_TOKEN_STATUS_RECV:
        {
            bool isfound = false;
            DLMSTokenGateway gatewayObject;

            qDebug() << "Token status buffer received";

            if(!clientSecured->CheckTokenTransferStatusResponseFrame(buffer, OBIS_TOKEN_GATEWAY, gatewayObject))
            {
                //Ignore - This could be a transient problem
                token_status = 10;
            }
            else
            {
                token_status = gatewayObject.GetStatusCode();
                qDebug() << "Token status received" << token_status << token_status_messages[token_status];
            }

            break;
        }

        case TT_TOKENS_CAPTURED_OBJECTS_RECV:
        case TT_TOKENS_CAPTURED_OBJECTS_PARTIAL_FRAME_RECV:
        {
            QString colnames;
            if(!clientSecured->CheckBatchFrame(buffer, ispartial, islast))
            {
                //Ignore and proceed to completion state
                protocolState =  TT_COMPLETE - 1;
                break;
            }

            if(ispartial)
            {
                // If not a complete frame then, keep the buffer in accumulator and set next stage
                protocolState = TT_TOKENS_CAPTURED_OBJECTS_RECEIVER_READY_SEND - 1;
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

                    protocolState =  TT_TOKENS_RECORDS_SEND - 1;
                }
                else
                {
                    protocolState =  TT_TOKENS_CAPTURED_OBJECTS_RECEIVER_READY_SEND - 1;
                }
            }
            break;
        }

        case TT_TOKENS_RECORDS_RECV:
        case TT_TOKENS_RECORDS_PARTIAL_FRAME_RECV:
        {
            if(!clientSecured->CheckBatchFrame(buffer, ispartial, islast))
            {
                //Ignore and proceed to completion state
                protocolState =  TT_COMPLETE - 1;
                break;
            }

            if(ispartial)
            {
                // If not a complete frame then, keep the buffer in accumulator and set next stage
                protocolState =  TT_TOKENS_RECORDS_RECEIVER_READY_SEND - 1;
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

                        protocolState =  TT_COMPLETE - 1;
                    }
                }
                else
                {
                    protocolState =  TT_TOKENS_RECORDS_RECEIVER_READY_SEND - 1;
                }
            }
            break;
        }

        default:
            qDebug() << "ERROR: UNHANDLED PROTOCOL STATE : " << protocolState;
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
        protocolState = TT_COMPLETE;
        emit Completed(completionParamters, TokenTransfer, false);
    }
}

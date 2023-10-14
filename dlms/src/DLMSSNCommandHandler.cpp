#include "DLMSServer.h"
#include "DLMSSNCommandHandler.h"
#include "DLMSValueEventCollection.h"
#include "DLMSSNParameters.h"
#include "DLMS.h"
#include "DLMSClient.h"

int DLMSSNCommandHandler::HandleReadRequest(
    DLMSSettings& settings,
    DLMSServer* server,
    ByteBuffer& data,
    ByteBuffer* replyData,
    DLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    ByteBuffer bb;
    int ret;
    unsigned char ch;
    unsigned long cnt = 0xFF;
    DLMS_VARIABLE_ACCESS_SPECIFICATION type;
    DLMSValueEventCollection list;
    std::vector<DLMSValueEventArg*> reads;
    std::vector<DLMSValueEventArg*> actions;
    // If get next frame.
    if (xml == NULL && data.GetSize() == 0)
    {
        if (server != NULL && server->m_Transaction != NULL)
        {
            return 0;
        }
        if (replyData != NULL)
        {
            bb.Set(replyData);
            replyData->Clear();
        }
        for (std::vector<DLMSValueEventArg*>::iterator it = server->m_Transaction->GetTargets().begin();
            it != server->m_Transaction->GetTargets().end(); ++it)
        {
            list.push_back(*it);
        }
    }
    else
    {
        std::string str;
        if ((ret = Helpers::GetObjectCount(data, cnt)) != 0)
        {
            return ret;
        }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        if (xml != NULL)
        {
            xml->IntegerToHex(cnt, 2, str);
            xml->AppendStartTag(DLMS_COMMAND_READ_REQUEST, "Qty", str);
        }
#endif //DLMS_IGNORE_XML_TRANSLATOR
        SNInfo info;
        for (unsigned long pos = 0; pos != cnt; ++pos)
        {
            if ((ret = data.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            type = (DLMS_VARIABLE_ACCESS_SPECIFICATION)ch;
            switch (type)
            {
            case DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME:
            case DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS:
                ret = HandleRead(settings, server, type, data, list, reads, actions, replyData, xml);
                break;
            case DLMS_VARIABLE_ACCESS_SPECIFICATION_BLOCK_NUMBER_ACCESS:
                return HandleReadBlockNumberAccess(settings, server, data, replyData, xml);
            case DLMS_VARIABLE_ACCESS_SPECIFICATION_READ_DATA_BLOCK_ACCESS:
                return HandleReadDataBlockAccess(settings, server, DLMS_COMMAND_READ_RESPONSE, data, cnt, replyData, xml, cipheredCommand);
            default:
                return ReturnSNError(settings, server, DLMS_COMMAND_READ_RESPONSE, DLMS_ERROR_CODE_READ_WRITE_DENIED, replyData);
            }
        }
        if (reads.size() != 0)
        {
            server->PreRead(reads);
        }
        if (actions.size() != 0)
        {
            server->PreAction(actions);
        }
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_COMMAND_READ_REQUEST);
        return 0;
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    DLMS_SINGLE_READ_RESPONSE requestType;
    ret = GetReadData(settings, list, bb, requestType);
    if (reads.size() != 0)
    {
        server->PostRead(reads);
    }
    if (actions.size() != 0)
    {
        server->PostAction(actions);
    }
    DLMSSNParameters p(&settings, DLMS_COMMAND_READ_RESPONSE, cnt,
        requestType, NULL, &bb);
    DLMS::GetSNPdu(p, *replyData);
    if (server->m_Transaction == NULL && (bb.GetSize() != bb.GetPosition()
        || settings.GetCount() != settings.GetIndex()))
    {
        server->m_Transaction = new DLMSLongTransaction(list, DLMS_COMMAND_READ_REQUEST, bb);
    }
    else if (server->m_Transaction != NULL)
    {
        replyData->Set(&bb);
    }
    return 0;
}

int DLMSSNCommandHandler::HandleWriteRequest(
    DLMSSettings& settings,
    DLMSServer* server,
    ByteBuffer& data,
    ByteBuffer* replyData,
    DLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    // Return error if connection is not established.
    if (xml == NULL && (settings.GetConnected() & DLMS_CONNECTION_STATE_DLMS) == 0)
    {
        server->GenerateConfirmedServiceError(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR,
            DLMS_SERVICE_ERROR_SERVICE,
            DLMS_SERVICE_UNSUPPORTED, *replyData);
        return 0;
    }
    std::string str;
    int ret;
    unsigned char ch;
    unsigned short sn;
    unsigned long cnt, pos;
    DLMS_VARIABLE_ACCESS_SPECIFICATION type;
    DLMSVariant value;
    std::vector<SNInfo> targets;
    // Get object count.
    if ((ret = Helpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        xml->AppendStartTag(DLMS_COMMAND_WRITE_REQUEST);
        xml->IntegerToHex(cnt, 2, str);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_LIST_OF_VARIABLE_ACCESS_SPECIFICATION, "Qty", str);
        if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_VARIABLE_ACCESS_SPECIFICATION);
        }
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    ByteBuffer results(cnt);
    for (pos = 0; pos != cnt; ++pos)
    {
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        type = (DLMS_VARIABLE_ACCESS_SPECIFICATION)ch;
        SNInfo i;
        switch (type)
        {
        case DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME:
            if ((ret = data.GetUInt16(&sn)) != 0)
            {
                return ret;
            }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
            if (xml != NULL)
            {
                xml->IntegerToHex((long)sn, 4, str);
                xml->AppendLine(DLMS_COMMAND_WRITE_REQUEST << 8 |
                    DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME,
                    "", str);
            }
            else
#endif //DLMS_IGNORE_XML_TRANSLATOR
            {
                if ((ret = FindSNObject(server, sn, i)) != 0)
                {
                    return ret;
                }
                targets.push_back(i);
                // If target is unknown.
                if (i.GetItem() == NULL)
                {
                    // Device reports a undefined object.
                    results.SetUInt8(DLMS_ERROR_CODE_UNDEFINED_OBJECT);
                }
                else
                {
                    results.SetUInt8(DLMS_ERROR_CODE_OK);
                }
            }
            break;
        case DLMS_VARIABLE_ACCESS_SPECIFICATION_WRITE_DATA_BLOCK_ACCESS:
            ret = HandleReadDataBlockAccess(settings, server, DLMS_COMMAND_WRITE_RESPONSE, data, cnt, replyData, xml, cipheredCommand);
            if (xml == NULL)
            {
                return ret;
            }
        default:
            // Device reports a HW error.
            results.SetUInt8(DLMS_ERROR_CODE_HARDWARE_FAULT);
        }
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_VARIABLE_ACCESS_SPECIFICATION);
        }
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_LIST_OF_VARIABLE_ACCESS_SPECIFICATION);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    // Get data count.
    if ((ret = Helpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    DataInfo di;
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        di.SetXml(xml);
        xml->IntegerToHex(cnt, 2, str);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_LIST_OF_DATA, "Qty", str);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    for (pos = 0; pos != cnt; ++pos)
    {
        di.Clear();
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        if (xml != NULL)
        {
            if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
            {
                xml->AppendStartTag(DLMS_COMMAND_WRITE_REQUEST, (unsigned long)DLMS_SINGLE_READ_RESPONSE_DATA);
            }
            if ((ret = Helpers::GetData(&settings, data, di, value)) != 0)
            {
                return ret;
            }
            if (!di.IsComplete())
            {
                str = data.ToHexString(data.GetPosition(), data.Available(), false);
                xml->AppendLine(xml->GetDataType(di.GetType()), "", str);
            }
            if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
            {
                xml->AppendEndTag(DLMS_COMMAND_WRITE_REQUEST, (unsigned long)DLMS_SINGLE_READ_RESPONSE_DATA);
            }
        }
        else
#endif //DLMS_IGNORE_XML_TRANSLATOR
        {
            if ((ret = results.GetUInt8(pos, &ch)) != 0)
            {
                return ret;
            }
            if (ch == 0)
            {
                // If object has found.
                SNInfo target = *(targets.begin() + pos);
                ret = Helpers::GetData(&settings, data, di, value);
                if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
                {
                    DLMS_DATA_TYPE dt;
                    ret = target.GetItem()->GetDataType(target.GetIndex(), dt);
                    if (dt != DLMS_DATA_TYPE_NONE && dt != DLMS_DATA_TYPE_OCTET_STRING)
                    {
                        ByteBuffer bb;
                        bb.Set(value.byteArr, value.GetSize());
                        value.Clear();
                        if ((ret = DLMSClient::ChangeType(bb, dt, value)) != 0)
                        {
                            return ret;
                        }
                    }
                }
                DLMSValueEventArg* e = new DLMSValueEventArg(server, target.GetItem(), target.GetIndex());
                e->SetValue(value);
                DLMSValueEventCollection arr;
                arr.push_back(e);
                server->PreWrite(arr);
                DLMS_ACCESS_MODE am = server->GetAttributeAccess(e);
                // If write is denied.
                if (am != DLMS_ACCESS_MODE_WRITE && am != DLMS_ACCESS_MODE_READ_WRITE)
                {
                    results.SetUInt8(pos, DLMS_ERROR_CODE_READ_WRITE_DENIED);
                }
                else
                {
                    if (e->GetError() != 0)
                    {
                        results.SetUInt8(pos, e->GetError());
                    }
                    else if (!e->GetHandled())
                    {
                        target.GetItem()->SetValue(settings, *e);
                        server->PostWrite(arr);
                    }
                }
            }
        }
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_LIST_OF_DATA);
        xml->AppendEndTag(DLMS_COMMAND_WRITE_REQUEST);
        return 0;
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    ByteBuffer bb((2 * cnt));
    for (pos = 0; pos != cnt; ++pos)
    {
        if ((ret = results.GetUInt8(pos, &ch)) != 0)
        {
            return ret;
        }
        // If meter returns error.
        if (ch != 0)
        {
            bb.SetUInt8(1);
        }
        bb.SetUInt8(ch);
    }
    DLMSSNParameters p(&settings, DLMS_COMMAND_WRITE_RESPONSE, cnt, 0xFF, NULL, &bb);
    return DLMS::GetSNPdu(p, *replyData);
}


/**
 * Find Short Name object.
 *
 * @param sn
 */
int DLMSSNCommandHandler::FindSNObject(DLMSServer* server, int sn, SNInfo& i)
{
    unsigned char offset, count;
    for (DLMSObjectCollection::iterator it = server->GetItems().begin(); it != server->GetItems().end(); ++it)
    {
        if (sn >= (*it)->GetShortName())
        {
            // If attribute is accessed.
            if (sn < (*it)->GetShortName() + (*it)->GetAttributeCount() * 8)
            {
                i.SetAction(false);
                i.SetItem(*it);
                i.SetIndex(((sn - (*it)->GetShortName()) / 8) + 1);
                break;
            }
            else
            {
                // If method is accessed.
                DLMS::GetActionInfo((*it)->GetObjectType(), offset, count);
                if (sn < (*it)->GetShortName() + offset + (8 * count))
                {
                    i.SetItem(*it);
                    i.SetAction(true);
                    i.SetIndex((sn - (*it)->GetShortName() - offset) / 8 + 1);
                    break;
                }
            }
        }
    }
    if (i.GetItem() == NULL)
    {
        std::string ln;
        i.SetItem(server->FindObject(DLMS_OBJECT_TYPE_NONE, sn, ln));
    }
    return 0;
}

/**
* Get data for Read command.
*
* @param settings
*            DLMS settings.
* @param list
*            received objects.
* @param data
*            Data as byte array.
* @param type
*            Response type.
*/
int DLMSSNCommandHandler::GetReadData(DLMSSettings& settings,
    std::vector<DLMSValueEventArg*>& list,
    ByteBuffer& data,
    DLMS_SINGLE_READ_RESPONSE& type)
{
    int ret = 0;
    unsigned char first = 1;
    type = DLMS_SINGLE_READ_RESPONSE_DATA;
    for (std::vector<DLMSValueEventArg*>::iterator e = list.begin(); e != list.end(); ++e)
    {
        if (!(*e)->GetHandled())
        {
            // If action.
            if ((*e)->IsAction())
            {
                ret = (*e)->GetTarget()->Invoke(settings, *(*e));
            }
            else
            {
                ret = (*e)->GetTarget()->GetValue(settings, *(*e));
            }
        }
        if (ret != 0)
        {
            return ret;
        }
        DLMSVariant& value = (*e)->GetValue();
        if ((*e)->GetError() == DLMS_ERROR_CODE_OK)
        {
            if (!first && list.size() != 1)
            {
                data.SetUInt8(DLMS_SINGLE_READ_RESPONSE_DATA);
            }
            if ((*e)->IsByteArray())
            {
                data.Set(value.byteArr, value.GetSize());
            }
            else
            {
                ret = DLMS::AppendData(&settings, (*e)->GetTarget(), (*e)->GetIndex(), data, value);
            }
        }
        else
        {
            if (!first && list.size() != 1)
            {
                data.SetUInt8(DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR);
            }
            data.SetUInt8((*e)->GetError());
            type = DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR;
        }
        first = false;
    }
    return ret;
}

int DLMSSNCommandHandler::HandleRead(
    DLMSSettings& settings,
    DLMSServer* server,
    DLMS_VARIABLE_ACCESS_SPECIFICATION type,
    ByteBuffer& data,
    DLMSValueEventCollection& list,
    std::vector<DLMSValueEventArg*>& reads,
    std::vector<DLMSValueEventArg*>& actions,
    ByteBuffer* replyData,
    DLMSTranslatorStructure* xml)
{
    SNInfo i;
    int ret;
    unsigned char ch;
    unsigned short sn;
    if ((ret = data.GetUInt16(&sn)) != 0)
    {
        return ret;
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        std::string str;
        if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_VARIABLE_ACCESS_SPECIFICATION);
        }
        if (type == DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS)
        {
            xml->AppendStartTag(DLMS_COMMAND_READ_REQUEST,
                DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS);
            xml->IntegerToHex((long)sn, 4, str);
            xml->AppendLine(
                DLMS_COMMAND_READ_REQUEST << 8 | DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME,
                "", str);
            if ((ret = data.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            xml->IntegerToHex((long)ch, 2, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_SELECTOR, "", str);
            DataInfo di;
            di.SetXml(xml);
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_PARAMETER);
            DLMSVariant value;
            if ((ret = Helpers::GetData(&settings, data, di, value)) != 0)
            {
                return ret;
            }
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_PARAMETER);
            xml->AppendEndTag(DLMS_COMMAND_READ_REQUEST, (unsigned long)DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS);
        }
        else
        {
            xml->IntegerToHex((long)sn, 4, str);
            xml->AppendLine(
                (int)DLMS_COMMAND_READ_REQUEST << 8
                | (int)DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME,
                "", str);
        }
        if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_VARIABLE_ACCESS_SPECIFICATION);
        }
        return 0;
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    if ((ret = FindSNObject(server, sn, i)) != 0)
    {
        return ret;
    }
    DLMSValueEventArg* e = new DLMSValueEventArg(server, i.GetItem(), i.GetIndex());
    e->SetAction(i.IsAction());
    list.push_back(e);
    if (type == DLMS_VARIABLE_ACCESS_SPECIFICATION_PARAMETERISED_ACCESS)
    {
        DLMSVariant params;
        DataInfo di;
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        e->SetSelector(ch);
        if ((ret = Helpers::GetData(&settings, data, di, params)) != 0)
        {
            return ret;
        }
        e->SetParameters(params);
    }
    // Return error if connection is not established.
    if (settings.GetConnected() == DLMS_CONNECTION_STATE_NONE &&
        (!e->IsAction() || e->GetTarget()->GetShortName() != 0xFA00 || e->GetIndex() != 8))
    {
        server->GenerateConfirmedServiceError(
            DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR, DLMS_SERVICE_ERROR_SERVICE,
            DLMS_SERVICE_UNSUPPORTED, *replyData);
        return 0;
    }

    if (server->GetAttributeAccess(e) == DLMS_ACCESS_MODE_NONE)
    {
        e->SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    else
    {
        if (e->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC && e->GetIndex() == 2)
        {
            e->SetRowToPdu(server->GetRowsToPdu((DLMSProfileGeneric*)e->GetTarget()));
        }
        if (e->IsAction())
        {
            actions.push_back(e);
        }
        else
        {
            reads.push_back(e);
        }
    }
    return ret;
}

int DLMSSNCommandHandler::HandleReadBlockNumberAccess(
    DLMSSettings& settings,
    DLMSServer* server,
    ByteBuffer& data,
    ByteBuffer* replyData,
    DLMSTranslatorStructure* xml)
{
    unsigned short blockNumber;
    int ret;
    if ((ret = data.GetUInt16(&blockNumber)) != 0)
    {
        return ret;
    }
    if (blockNumber != settings.GetBlockIndex())
    {
        ByteBuffer bb;
        bb.SetUInt8(DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID);
        DLMSSNParameters p(&settings,
            DLMS_COMMAND_READ_RESPONSE, 1,
            DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR, &bb, NULL);
        ret = DLMS::GetSNPdu(p, *replyData);
        settings.ResetBlockIndex();
        return ret;
    }
    if (settings.GetIndex() != settings.GetCount()
        && server->m_Transaction->GetData().GetSize() < settings.GetMaxPduSize())
    {
        std::vector<DLMSValueEventArg*> reads;
        std::vector<DLMSValueEventArg*> actions;

        for (std::vector<DLMSValueEventArg*>::iterator it = server->m_Transaction->GetTargets().begin();
            it != server->m_Transaction->GetTargets().end(); ++it)
        {
            if ((*it)->IsAction())
            {
                actions.push_back(*it);
            }
            else
            {
                reads.push_back(*it);
            }
        }
        if (reads.size() != 0)
        {
            server->PreRead(reads);
        }

        if (actions.size() != 0)
        {
            server->PreAction(actions);
        }
        DLMS_SINGLE_READ_RESPONSE requestType;
        std::vector<DLMSValueEventArg*>& list = server->m_Transaction->GetTargets();
        ByteBuffer& data2 = server->m_Transaction->GetData();
        ret = GetReadData(settings, list, data2, requestType);
        if (reads.size() != 0)
        {
            server->PostRead(reads);
        }
        if (actions.size() != 0)
        {
            server->PostAction(actions);
        }
    }
    settings.IncreaseBlockIndex();
    ByteBuffer& tmp = server->m_Transaction->GetData();
    DLMSSNParameters p(&settings, DLMS_COMMAND_READ_RESPONSE, 1,
        DLMS_SINGLE_READ_RESPONSE_DATA_BLOCK_RESULT, NULL, &tmp);
    p.SetMultipleBlocks(true);
    ret = DLMS::GetSNPdu(p, *replyData);
    // If all data is sent.
    if (server->m_Transaction->GetData().GetSize() == server->m_Transaction->GetData().GetPosition())
    {
        delete server->m_Transaction;
        server->m_Transaction = NULL;
        settings.ResetBlockIndex();
    }
    else
    {
        server->m_Transaction->GetData().Trim();
    }
    return ret;
}

int DLMSSNCommandHandler::HandleReadDataBlockAccess(
    DLMSSettings& settings,
    DLMSServer* server,
    DLMS_COMMAND command,
    ByteBuffer& data,
    int cnt,
    ByteBuffer* replyData,
    DLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    int ret;
    unsigned long size;
    unsigned short blockNumber;
    unsigned char lastBlock;
    if ((ret = data.GetUInt8(&lastBlock)) != 0)
    {
        return ret;
    }
    if ((ret = data.GetUInt16(&blockNumber)) != 0)
    {
        return ret;
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        std::string str;
        if (command == DLMS_COMMAND_WRITE_RESPONSE)
        {
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_WRITE_DATA_BLOCK_ACCESS);
        }
        else
        {
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_READ_DATA_BLOCK_ACCESS);
        }
        xml->IntegerToHex((long)lastBlock, 2, str);
        xml->AppendLine("<LastBlock Value=\"" + str + "\" />");
        xml->IntegerToHex((long)blockNumber, 4, str);
        xml->AppendLine("<BlockNumber Value=\"" + str + "\" />");
        if (command == DLMS_COMMAND_WRITE_RESPONSE)
        {
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_WRITE_DATA_BLOCK_ACCESS);
        }
        else
        {
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_READ_DATA_BLOCK_ACCESS);
        }
        return 0;
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    if (blockNumber != settings.GetBlockIndex())
    {
        ByteBuffer bb;
        bb.SetUInt8(DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID);
        DLMSSNParameters p(&settings, command, 1, DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR, &bb, NULL);
        ret = DLMS::GetSNPdu(p, *replyData);
        settings.ResetBlockIndex();
        return ret;
    }
    unsigned char count = 1, type = DLMS_DATA_TYPE_OCTET_STRING;
    if (command == DLMS_COMMAND_WRITE_RESPONSE)
    {
        if ((ret = data.GetUInt8(&count)) != 0 ||
            (ret = data.GetUInt8(&type)) != 0)
        {
            return ret;
        }
    }
    if ((ret = Helpers::GetObjectCount(data, size)) != 0)
    {
        return ret;
    }
    unsigned long realSize = data.GetSize() - data.GetPosition();
    if (count != 1 || type != DLMS_DATA_TYPE_OCTET_STRING || size != realSize)
    {
        ByteBuffer bb;
        bb.SetUInt8(DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE);
        DLMSSNParameters p(&settings, command, cnt,
            DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR, &bb, NULL);
        ret = DLMS::GetSNPdu(p, *replyData);
        settings.ResetBlockIndex();
        return ret;
    }
    if (server->m_Transaction == NULL)
    {
        DLMSValueEventCollection tmp;
        server->m_Transaction = new DLMSLongTransaction(tmp, command, data);
    }
    else
    {
        server->m_Transaction->GetData().Set(&data, data.GetPosition());
    }
    if (lastBlock == 0)
    {
        ByteBuffer bb;
        bb.SetUInt16(blockNumber);
        settings.IncreaseBlockIndex();
        if (command == DLMS_COMMAND_READ_RESPONSE)
        {
            type = DLMS_SINGLE_READ_RESPONSE_BLOCK_NUMBER;
        }
        else
        {
            type = DLMS_SINGLE_WRITE_RESPONSE_BLOCK_NUMBER;
        }
        DLMSSNParameters p(&settings, command, cnt, type, NULL, &bb);
        ret = DLMS::GetSNPdu(p, *replyData);
    }
    else
    {
        if (server->m_Transaction != NULL)
        {
            data.SetSize(0);
            data.Set(&server->m_Transaction->GetData());
            delete server->m_Transaction;
            server->m_Transaction = NULL;
        }
        if (command == DLMS_COMMAND_READ_RESPONSE)
        {
            ret = DLMSSNCommandHandler::HandleReadRequest(settings, server, data, replyData, NULL, cipheredCommand);
        }
        else
        {
            ret = DLMSSNCommandHandler::HandleWriteRequest(settings, server, data, replyData, NULL, cipheredCommand);
        }
        settings.ResetBlockIndex();
    }
    return ret;
}

int DLMSSNCommandHandler::ReturnSNError(
    DLMSSettings& settings,
    DLMSServer* server,
    DLMS_COMMAND cmd, DLMS_ERROR_CODE error,
    ByteBuffer* replyData)
{
    int ret;
    ByteBuffer bb;
    bb.SetUInt8(error);
    DLMSSNParameters p(&settings, cmd, 1,
        DLMS_SINGLE_READ_RESPONSE_DATA_ACCESS_ERROR, &bb, NULL);
    ret = DLMS::GetSNPdu(p, *replyData);
    settings.ResetBlockIndex();
    return ret;
}


/// Handle Information Report.
int DLMSSNCommandHandler::HandleInformationReport(
    DLMSSettings& settings,
    ReplyData& reply)
{
    unsigned long count;
    std::string str;
    int ret;
    unsigned char ch;
    reply.SetTime(0);
    if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }

    ByteBuffer tmp;
    // If date time is given.
    if (ch != 0)
    {
        tmp.Set(&reply.GetData(), reply.GetData().GetPosition(), ch);
        DLMSVariant value;
        if ((ret = DLMSClient::ChangeType(tmp, DLMS_DATA_TYPE_DATETIME, value)) != 0)
        {
            return ret;
        }
        reply.SetTime(&value.dateTime.GetValue());
    }
    if ((ret = Helpers::GetObjectCount(reply.GetData(), count)) != 0)
    {
        return ret;
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (reply.GetXml() != NULL)
    {
        reply.GetXml()->AppendStartTag(DLMS_COMMAND_INFORMATION_REPORT);
        if (reply.GetTime() != NULL)
        {
            DateTime dt(reply.GetTime());
            reply.GetXml()->AppendComment(dt.ToString());
            if (reply.GetXml()->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
            {
                str = tmp.ToHexString(false);
                reply.GetXml()->AppendLine(DLMS_TRANSLATOR_TAGS_CURRENT_TIME, "", str);
            }
            else
            {
                str = Helpers::GeneralizedTime(reply.GetTime());
                reply.GetXml()->AppendLine(DLMS_TRANSLATOR_TAGS_CURRENT_TIME, "", str);
            }
        }
        reply.GetXml()->IntegerToHex(count, 2, str);
        reply.GetXml()->AppendStartTag(DLMS_TRANSLATOR_TAGS_LIST_OF_VARIABLE_ACCESS_SPECIFICATION, "Qty", str);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    for (unsigned long pos = 0; pos != count; ++pos)
    {
        if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (ch == DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME)
        {
            unsigned short sn;
            if ((ret = reply.GetData().GetUInt16(&sn)) != 0)
            {
                return ret;
            }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
            if (reply.GetXml() != NULL)
            {
                reply.GetXml()->IntegerToHex((long)sn, 4, str);
                reply.GetXml()->AppendLine(
                    DLMS_COMMAND_WRITE_REQUEST << 8 | DLMS_VARIABLE_ACCESS_SPECIFICATION_VARIABLE_NAME,
                    "", str);
            }
#endif //DLMS_IGNORE_XML_TRANSLATOR
        }
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (reply.GetXml() != NULL)
    {
        reply.GetXml()->IntegerToHex(count, 2, str);
        reply.GetXml()->AppendEndTag(DLMS_TRANSLATOR_TAGS_LIST_OF_VARIABLE_ACCESS_SPECIFICATION);
        reply.GetXml()->AppendStartTag(DLMS_TRANSLATOR_TAGS_LIST_OF_DATA, "Qty", str);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    //Get values.
    if ((ret = Helpers::GetObjectCount(reply.GetData(), count)) != 0)
    {
        return ret;
    }
    DataInfo di;
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    di.SetXml(reply.GetXml());
#endif //DLMS_IGNORE_XML_TRANSLATOR
    DLMSVariant value;
    for (unsigned long pos = 0; pos != count; ++pos)
    {
        di.Clear();
        if (reply.GetXml() != NULL)
        {
            if ((ret = Helpers::GetData(&settings, reply.GetData(), di, value)) != 0)
            {
                return ret;
            }
        }
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (reply.GetXml() != NULL)
    {
        reply.GetXml()->AppendEndTag(DLMS_TRANSLATOR_TAGS_LIST_OF_DATA);
        reply.GetXml()->AppendEndTag(DLMS_COMMAND_INFORMATION_REPORT);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    return 0;
}

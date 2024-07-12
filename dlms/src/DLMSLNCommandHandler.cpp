#include "DLMSServer.h"
#include "DLMSLNCommandHandler.h"
#include "DLMS.h"
#include "DLMSValueEventCollection.h"
#include "DLMSClient.h"
#include "DLMSSecuritySetup.h"
#include "DLMSAccessItem.h"

#ifndef DLMS_IGNORE_XML_TRANSLATOR
void AppendAttributeDescriptor(DLMSTranslatorStructure* xml, int ci, unsigned char* ln, unsigned char attributeIndex)
{
    xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR);
    if (xml->GetComments())
    {
        xml->AppendComment(DLMSConverter::ToString((DLMS_OBJECT_TYPE)ci));
    }
    std::string tmp;
    xml->IntegerToHex((long)ci, 4, tmp);
    xml->AppendLine(DLMS_TRANSLATOR_TAGS_CLASS_ID, "", tmp);
    Helpers::GetLogicalName(ln, tmp);
    xml->AppendComment(tmp);
    tmp = Helpers::BytesToHex(ln, 6, false);
    xml->AppendLine(DLMS_TRANSLATOR_TAGS_INSTANCE_ID, "", tmp);
    xml->IntegerToHex((long)attributeIndex, 2, tmp);
    xml->AppendLine(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_ID, "", tmp);
    xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR);
}

void AppendMethodDescriptor(DLMSTranslatorStructure* xml, int ci, unsigned char* ln, unsigned char attributeIndex)
{
    xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_METHOD_DESCRIPTOR);
    if (xml->GetComments())
    {
        xml->AppendComment(DLMSConverter::ToString((DLMS_OBJECT_TYPE)ci));
    }
    std::string tmp;
    xml->IntegerToHex((long)ci, 4, tmp);
    xml->AppendLine(DLMS_TRANSLATOR_TAGS_CLASS_ID, "", tmp);
    Helpers::GetLogicalName(ln, tmp);
    xml->AppendComment(tmp);
    tmp = Helpers::BytesToHex(ln, 6, false);
    xml->AppendLine(DLMS_TRANSLATOR_TAGS_INSTANCE_ID, "", tmp);
    xml->IntegerToHex((long)attributeIndex, 2, tmp);
    xml->AppendLine(DLMS_TRANSLATOR_TAGS_METHOD_ID, "", tmp);
    xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_METHOD_DESCRIPTOR);
}
#endif //DLMS_IGNORE_XML_TRANSLATOR

int DLMSLNCommandHandler::GetRequestNormal(
    DLMSSettings& settings,
    unsigned char invokeID,
    DLMSServer* server,
    ByteBuffer& data,
    ByteBuffer* replyData,
    DLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    int ret;
    ByteBuffer bb;
    DLMS_ERROR_CODE status = DLMS_ERROR_CODE_OK;
    settings.SetCount(0);
    settings.SetIndex(0);
    settings.ResetBlockIndex();
    DLMSValueEventCollection arr;
    unsigned char attributeIndex;
    unsigned char* ln;
    // CI
    unsigned short tmp;
    if ((ret = data.GetUInt16(&tmp)) != 0)
    {
        return ret;
    }
    DLMS_OBJECT_TYPE ci = (DLMS_OBJECT_TYPE)tmp;
    ln = data.GetData() + data.GetPosition();
    data.SetPosition(data.GetPosition() + 6);
    // Attribute Id
    if ((ret = data.GetUInt8(&attributeIndex)) != 0)
    {
        return ret;
    }

    // Access selection
    unsigned char selection, selector = 0;
    if ((ret = data.GetUInt8(&selection)) != 0)
    {
        return ret;
    }
    if (selection != 0)
    {
        if ((ret = data.GetUInt8(&selector)) != 0)
        {
            return ret;
        }
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        AppendAttributeDescriptor(xml, (int)ci, ln, attributeIndex);
        if (selection != 0)
        {
            std::string tmp;
            xml->IntegerToHex((long)selector, 2, tmp);
            DataInfo info;
            DLMSVariant value;
            info.SetXml(xml);
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ACCESS_SELECTION);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_ACCESS_SELECTOR, "", tmp);
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ACCESS_PARAMETERS);
            if ((ret = Helpers::GetData(&settings, data, info, value)) != 0)
            {
                return ret;
            }
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ACCESS_PARAMETERS);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ACCESS_SELECTION);
        }
        return 0;
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    DLMSVariant parameters;
    if (selection != 0)
    {
        DataInfo i;
        if ((ret = Helpers::GetData(&settings, data, i, parameters)) != 0)
        {
            return ret;
        }
    }
    DLMSObject* obj = settings.GetObjects().FindByLN(ci, ln);
    if (obj == NULL)
    {
        std::string name;
        Helpers::GetLogicalName(ln, name);
        obj = server->FindObject(ci, 0, name);
    }
    DLMSValueEventArg* e = new DLMSValueEventArg(server, obj, attributeIndex, selector, parameters);
    e->SetInvokeId(invokeID);
    arr.push_back(e);
    if (obj == NULL)
    {
        // "Access Error : Device reports a undefined object."
        status = DLMS_ERROR_CODE_UNDEFINED_OBJECT;
    }
    else
    {
        if (server->GetAttributeAccess(e) == DLMS_ACCESS_MODE_NONE)
        {
            // Read Write denied.
            status = DLMS_ERROR_CODE_READ_WRITE_DENIED;
        }
        else
        {
            if (obj->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC && attributeIndex == 2)
            {
                e->SetRowToPdu(server->GetRowsToPdu((DLMSProfileGeneric*)obj));
            }
            server->PreRead(arr);
            if (!e->GetHandled())
            {
                settings.SetCount(e->GetRowEndIndex() - e->GetRowBeginIndex());
                if ((ret = obj->GetValue(settings, *e)) != 0)
                {
                    status = DLMS_ERROR_CODE_HARDWARE_FAULT;
                }
                server->PostRead(arr);
            }
            if (status == 0)
            {
                status = e->GetError();
            }
            DLMSVariant& value = e->GetValue();
            if (e->IsByteArray() && value.vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                // If byte array is added do not add type.
                bb.Set(value.byteArr, value.GetSize());
            }
            else if ((ret = DLMS::AppendData(&settings, obj, attributeIndex, bb, value)) != 0)
            {
                status = DLMS_ERROR_CODE_HARDWARE_FAULT;
            }
        }
    }
    DLMSLNParameters p(&settings, invokeID, DLMS_COMMAND_GET_RESPONSE, 1, NULL, &bb, status, cipheredCommand);
    ret = DLMS::GetLNPdu(p, *replyData);
    if (settings.GetCount() != settings.GetIndex()
        || bb.GetSize() != bb.GetPosition())
    {
        if (server->m_Transaction != NULL)
        {
            delete server->m_Transaction;
            server->m_Transaction = NULL;
        }
        server->m_Transaction = new DLMSLongTransaction(arr, DLMS_COMMAND_GET_REQUEST, bb);
    }
    return ret;
}

int DLMSLNCommandHandler::GetRequestNextDataBlock(
    DLMSSettings& settings,
    unsigned char invokeID,
    DLMSServer* server,
    ByteBuffer& data,
    ByteBuffer* replyData,
    DLMSTranslatorStructure* xml,
    bool streaming,
    unsigned char cipheredCommand)
{
    ByteBuffer bb;
    int ret;
    if (!streaming)
    {
        unsigned long index;
        // Get block index.
        if ((ret = data.GetUInt32(&index)) != 0)
        {
            return ret;
        }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        if (xml != NULL)
        {
            std::string tmp;
            xml->IntegerToHex(index, 8, tmp);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_BLOCK_NUMBER, "", tmp);
            return 0;
        }
#endif //DLMS_IGNORE_XML_TRANSLATOR
        if (index != settings.GetBlockIndex())
        {
            DLMSLNParameters p(&settings, invokeID, DLMS_COMMAND_GET_RESPONSE, 2,
                NULL, &bb,
                DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID, cipheredCommand);
            return DLMS::GetLNPdu(p, *replyData);
        }
    }
    settings.IncreaseBlockIndex();
    DLMSLNParameters p(&settings, invokeID,
        streaming ? DLMS_COMMAND_GENERAL_BLOCK_TRANSFER : DLMS_COMMAND_GET_RESPONSE,
        2, NULL, &bb, DLMS_ERROR_CODE_OK, cipheredCommand);
    p.SetStreaming(streaming);
    p.SetWindowSize(settings.GetGbtWindowSize());
    // If m_Transaction is not in progress.
    if (server->m_Transaction == NULL)
    {
        p.SetStatus(DLMS_ERROR_CODE_NO_LONG_GET_OR_READ_IN_PROGRESS);
    }
    else
    {
        bb.Set(&server->m_Transaction->GetData());
        unsigned char moreData = settings.GetIndex() != settings.GetCount();
        if (moreData)
        {
            // If there is multiple blocks on the buffer.
            // This might happen when Max PDU size is very small.
            if (bb.GetSize() < settings.GetMaxPduSize())
            {
                DLMSVariant value;
                for (std::vector<DLMSValueEventArg*>::iterator arg = server->m_Transaction->GetTargets().begin();
                    arg != server->m_Transaction->GetTargets().end(); ++arg)
                {
                    server->PreRead(server->m_Transaction->GetTargets());
                    if (!(*arg)->GetHandled())
                    {
                        if ((ret = (*arg)->GetTarget()->GetValue(settings, *(*arg))) != 0)
                        {
                            return ret;
                        }
                        std::vector<DLMSValueEventArg*> arr;
                        arr.push_back(*arg);
                        server->PostRead(arr);
                    }
                    value = (*arg)->GetValue();
                    // Add data.
                    if ((*arg)->IsByteArray() && value.vt == DLMS_DATA_TYPE_OCTET_STRING)
                    {
                        // If byte array is added do not add type.
                        bb.Set(value.byteArr, value.GetSize());
                    }
                    else if ((ret = DLMS::AppendData(&settings, (*arg)->GetTarget(), (*arg)->GetIndex(), bb, value)) != 0)
                    {
                        return DLMS_ERROR_CODE_HARDWARE_FAULT;
                    }
                }
            }
        }
        p.SetMultipleBlocks(true);
        ret = DLMS::GetLNPdu(p, *replyData);
        moreData = settings.GetIndex() != settings.GetCount();
        if (moreData || bb.GetSize() - bb.GetPosition() != 0)
        {
            server->m_Transaction->SetData(bb);
        }
        else
        {
            delete server->m_Transaction;
            server->m_Transaction = NULL;
            settings.ResetBlockIndex();
        }
    }
    return ret;
}

int DLMSLNCommandHandler::GetRequestWithList(
    DLMSSettings& settings,
    unsigned char invokeID,
    DLMSServer* server,
    ByteBuffer& data,
    ByteBuffer* replyData,
    DLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    DLMSValueEventCollection list;
    ByteBuffer bb;
    int ret;
    unsigned char attributeIndex;
    unsigned short id;
    unsigned long pos, cnt;
    if ((ret = Helpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    Helpers::SetObjectCount(cnt, bb);
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        std::string tmp;
        xml->IntegerToHex(cnt, 2, tmp);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_LIST, "Qty", tmp);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    for (pos = 0; pos != cnt; ++pos)
    {
        if ((ret = data.GetUInt16(&id)) != 0)
        {
            return ret;
        }
        DLMS_OBJECT_TYPE ci = (DLMS_OBJECT_TYPE)id;
        unsigned char* ln;
        ln = data.GetData() + data.GetPosition();
        data.SetPosition(data.GetPosition() + 6);
        if ((ret = data.GetUInt8(&attributeIndex)) != 0)
        {
            return ret;
        }

        // AccessSelection
        unsigned char selection, selector = 0;
        DLMSVariant parameters;
        if ((ret = data.GetUInt8(&selection)) != 0)
        {
            return ret;
        }
        if (selection != 0)
        {
            if ((ret = data.GetUInt8(&selector)) != 0)
            {
                return ret;
            }
            DataInfo i;
            if ((ret = Helpers::GetData(&settings, data, i, parameters)) != 0)
            {
                return ret;
            }
        }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        if (xml != NULL)
        {
            std::string tmp;
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_WITH_SELECTION);
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR);
            xml->AppendComment(DLMSConverter::ToString(ci));
            xml->IntegerToHex((long)ci, 4, tmp);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_CLASS_ID, "", tmp);
            Helpers::GetLogicalName(ln, tmp);
            xml->AppendComment(tmp);
            tmp = Helpers::BytesToHex(ln, 6, false);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_INSTANCE_ID, "", tmp);
            xml->IntegerToHex((long)attributeIndex, 2, tmp);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_ID, "", tmp);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_WITH_SELECTION);
        }
        else
#endif //DLMS_IGNORE_XML_TRANSLATOR
        {
            DLMSObject* obj = settings.GetObjects().FindByLN(ci, ln);
            if (obj == NULL)
            {
                std::string name;
                Helpers::GetLogicalName(ln, name);
                obj = server->FindObject(ci, 0, name);
            }
            if (obj == NULL)
            {
                // Access Error : Device reports a undefined object.
                DLMSValueEventArg* e = new DLMSValueEventArg(server, obj, attributeIndex);
                e->SetError(DLMS_ERROR_CODE_UNDEFINED_OBJECT);
                list.push_back(e);
            }
            else
            {
                DLMSValueEventArg* arg = new DLMSValueEventArg(server, obj, attributeIndex, selector, parameters);
                list.push_back(arg);
                if (server->GetAttributeAccess(arg) == DLMS_ACCESS_MODE_NONE)
                {
                    // Read Write denied.
                    arg->SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
                }
            }
        }
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_LIST);
        return 0;
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    server->PreRead(list);
    pos = 0;
    for (std::vector<DLMSValueEventArg*>::iterator it = list.begin(); it != list.end(); ++it)
    {
        if (!(*it)->GetHandled())
        {
            ret = (*it)->GetTarget()->GetValue(settings, *(*it));
        }
        DLMSVariant& value = (*it)->GetValue();
        bb.SetUInt8((*it)->GetError());
        if ((*it)->IsByteArray() && value.vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            // If byte array is added do not add type.
            bb.Set(value.byteArr, value.GetSize());
        }
        else if ((ret = DLMS::AppendData(&settings, (*it)->GetTarget(), (*it)->GetIndex(), bb, value)) != 0)
        {
            return DLMS_ERROR_CODE_HARDWARE_FAULT;
        }
        if (settings.GetIndex() != settings.GetCount())
        {
            if (server->m_Transaction != NULL)
            {
                delete server->m_Transaction;
                server->m_Transaction = NULL;
            }
            ByteBuffer empty;
            server->m_Transaction = new DLMSLongTransaction(list, DLMS_COMMAND_GET_REQUEST, empty);
        }
        ++pos;
    }
    server->PostRead(list);
    DLMSLNParameters p(&settings, invokeID, DLMS_COMMAND_GET_RESPONSE, 3, NULL, &bb, 0xFF, cipheredCommand);
    return DLMS::GetLNPdu(p, *replyData);
}

int DLMSLNCommandHandler::HandleGetRequest(
    DLMSSettings& settings,
    DLMSServer* server,
    ByteBuffer& data,
    ByteBuffer* replyData,
    DLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    // Return error if connection is not established.
    if (xml == NULL && (settings.GetConnected() & DLMS_CONNECTION_STATE_DLMS) == 0 &&
        cipheredCommand == DLMS_COMMAND_NONE)
    {
        server->GenerateConfirmedServiceError(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR,
            DLMS_SERVICE_ERROR_SERVICE, DLMS_SERVICE_UNSUPPORTED,
            *replyData);
        return 0;
    }
    int ret;
    unsigned char ch, invokeID;
    DLMS_GET_COMMAND_TYPE type = DLMS_GET_COMMAND_TYPE_NEXT_DATA_BLOCK;
    //If GBT is used data is empty.
    if (data.GetSize() != 0)
    {
        // Get type.
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        type = (DLMS_GET_COMMAND_TYPE)ch;
        // Get invoke ID and priority.
        if ((ret = data.GetUInt8(&invokeID)) != 0)
        {
            return ret;
        }
        settings.UpdateInvokeId(invokeID);
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        DLMS::AddInvokeId(xml, DLMS_COMMAND_GET_REQUEST, type, invokeID);
#endif //DLMS_IGNORE_XML_TRANSLATOR
    }
    // GetRequest normal
    if (type == DLMS_GET_COMMAND_TYPE_NORMAL)
    {
        ret = GetRequestNormal(settings, invokeID, server, data, replyData, xml, cipheredCommand);
    }
    else if (type == DLMS_GET_COMMAND_TYPE_NEXT_DATA_BLOCK)
    {
        // Get request for next data block
        ret = GetRequestNextDataBlock(settings, invokeID, server, data, replyData, xml, false, cipheredCommand);
    }
    else if (type == DLMS_GET_COMMAND_TYPE_WITH_LIST)
    {
        // Get request with a list.
        ret = GetRequestWithList(settings, invokeID, server, data, replyData, xml, cipheredCommand);
    }
    else
    {
        ByteBuffer bb;
        settings.ResetBlockIndex();
        // Access Error : Device reports a hardware fault.
        bb.SetUInt8(DLMS_ERROR_CODE_HARDWARE_FAULT);
        DLMSLNParameters p(&settings, invokeID, DLMS_COMMAND_GET_RESPONSE,
            type, NULL, &bb, DLMS_ERROR_CODE_OK, cipheredCommand);
        ret = DLMS::GetLNPdu(p, *replyData);
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_COMMAND_GET_REQUEST, (unsigned long)type);
        xml->AppendEndTag(DLMS_COMMAND_GET_REQUEST);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    return ret;
}

int DLMSLNCommandHandler::HandleSetRequestNormal(
    DLMSSettings& settings,
    DLMSServer* server,
    ByteBuffer& data,
    short type,
    DLMSLNParameters& p,
    DLMSTranslatorStructure* xml)
{
    DataInfo i;
    DLMSVariant value;
    std::string str;
    int ret;
    unsigned char index, ch, lastBlock;
    unsigned short tmp;
    // Get CI.
    if ((ret = data.GetUInt16(&tmp)) != 0)
    {
        return ret;
    }
    DLMS_OBJECT_TYPE ci = (DLMS_OBJECT_TYPE)tmp;
    unsigned char* ln;
    ln = data.GetData() + data.GetPosition();
    data.SetPosition(data.GetPosition() + 6);
    // Attribute index.
    if ((ret = data.GetUInt8(&index)) != 0)
    {
        return ret;
    }
    // Get Access Selection.
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (type == 2)
    {
        unsigned long size, blockNumber;
        if ((ret = data.GetUInt8(&lastBlock)) != 0)
        {
            return ret;
        }
        p.SetMultipleBlocks(lastBlock == 0);
        ret = data.GetUInt32(&blockNumber);
        if (ret != 0)
        {
            return ret;
        }
        if (blockNumber != settings.GetBlockIndex())
        {
            p.SetStatus(DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID);
            return 0;
        }
        settings.IncreaseBlockIndex();
        ret = Helpers::GetObjectCount(data, size);
        if (ret != 0)
        {
            return ret;
        }
        unsigned long realSize = data.GetSize() - data.GetPosition();
        if (size != realSize)
        {
            p.SetStatus(DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE);
            return 0;
        }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        if (xml != NULL)
        {
            AppendAttributeDescriptor(xml, ci, ln, index);
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_DATA_BLOCK);
            xml->IntegerToHex((long)lastBlock, 2, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_LAST_BLOCK, "", str);
            xml->IntegerToHex(blockNumber, 8, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_BLOCK_NUMBER, "", str);
            str = data.ToHexString(data.GetPosition(), data.Available(), false);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_RAW_DATA, "", str);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_DATA_BLOCK);
        }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        AppendAttributeDescriptor(xml, (int)ci, ln, index);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_VALUE);
        DLMSVariant value;
        DataInfo di;
        di.SetXml(xml);
        if ((ret = Helpers::GetData(&settings, data, di, value)) != 0)
        {
            return ret;
        }
        if (!di.IsComplete())
        {
            value = data.ToHexString(data.GetPosition(), data.Available(), false);
        }
        else if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            value = Helpers::BytesToHex(value.byteArr, value.GetSize(), false);
        }
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_VALUE);
        return 0;
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    if (!p.IsMultipleBlocks())
    {
        settings.ResetBlockIndex();
        ret = Helpers::GetData(&settings, data, i, value);
        if (ret != 0)
        {
            return ret;
        }
    }

    DLMSObject* obj = settings.GetObjects().FindByLN(ci, ln);
    if (obj == NULL)
    {
        std::string name;
        Helpers::GetLogicalName(ln, name);
        obj = server->FindObject(ci, 0, name);
    }
    // If target is unknown.
    if (obj == NULL)
    {
        // Device reports a undefined object.
        p.SetStatus(DLMS_ERROR_CODE_UNAVAILABLE_OBJECT);
    }
    else
    {
        DLMSValueEventArg* e = new DLMSValueEventArg(server, obj, index);
        e->SetValue(value);
        DLMSValueEventCollection list;
        list.push_back(e);
        DLMS_ACCESS_MODE am = server->GetAttributeAccess(e);
        // If write is denied.
        if (am != DLMS_ACCESS_MODE_WRITE && am != DLMS_ACCESS_MODE_READ_WRITE)
        {
            //Read Write denied.
            p.SetStatus(DLMS_ERROR_CODE_READ_WRITE_DENIED);
        }
        else
        {
            if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                DLMS_DATA_TYPE dt;
                ret = obj->GetDataType(index, dt);
                if (ret != 0)
                {
                    return ret;
                }
                if (dt != DLMS_DATA_TYPE_NONE && dt != DLMS_DATA_TYPE_OCTET_STRING)
                {
                    ByteBuffer tmp;
                    tmp.Set(value.byteArr, value.GetSize());
                    value.Clear();
                    if ((ret = DLMSClient::ChangeType(tmp, dt, value)) != 0)
                    {
                        return ret;
                    }
                }
            }
            if (p.IsMultipleBlocks())
            {
                server->m_Transaction = new DLMSLongTransaction(list, DLMS_COMMAND_GET_REQUEST, data);
            }
            server->PreWrite(list);
            if (e->GetError() != 0)
            {
                p.SetStatus(e->GetError());
            }
            else if (!e->GetHandled() && !p.IsMultipleBlocks())
            {
                obj->SetValue(settings, *e);
                server->PostWrite(list);
            }
        }
    }
    return ret;
}

int DLMSLNCommandHandler::HanleSetRequestWithDataBlock(
    DLMSSettings& settings,
    DLMSServer* server,
    ByteBuffer& data,
    DLMSLNParameters& p,
    DLMSTranslatorStructure* xml)
{
    DataInfo reply;
    int ret;
    unsigned long blockNumber, size;
    unsigned char lastBlock;
    if ((ret = data.GetUInt8(&lastBlock)) != 0)
    {
        return ret;
    }
    p.SetMultipleBlocks(lastBlock == 0);
    if ((ret = data.GetUInt32(&blockNumber)) != 0)
    {
        return ret;
    }
    if (xml == NULL && blockNumber != settings.GetBlockIndex())
    {
        p.SetStatus(DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID);
    }
    else
    {
        settings.IncreaseBlockIndex();
        if ((ret = Helpers::GetObjectCount(data, size)) != 0)
        {
            return ret;
        }
        unsigned long realSize = data.GetSize() - data.GetPosition();
        if (size != realSize)
        {
            p.SetStatus(DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE);
        }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        if (xml != NULL)
        {
            std::string str;
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_DATA_BLOCK);
            xml->IntegerToHex((long)lastBlock, 2, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_LAST_BLOCK, "", str);
            xml->IntegerToHex(blockNumber, 8, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_BLOCK_NUMBER, "", str);
            str = data.ToHexString(data.GetPosition(), data.Available(), false);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_RAW_DATA, "", str);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_DATA_BLOCK);
            return 0;
        }
#endif //DLMS_IGNORE_XML_TRANSLATOR
        server->m_Transaction->GetData().Set(&data, data.GetPosition());
        // If all data is received.
        if (!p.IsMultipleBlocks())
        {
            DLMSVariant value;
            if ((ret != Helpers::GetData(&settings, server->m_Transaction->GetData(), reply, value)) != 0)
            {
                return ret;
            }
            DLMSValueEventArg* target = *server->m_Transaction->GetTargets().begin();
            if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                DLMS_DATA_TYPE dt;
                ret = target->GetTarget()->GetDataType(target->GetIndex(), dt);
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
            target->SetValue(value);
            server->PreWrite(server->m_Transaction->GetTargets());
            if (!target->GetHandled() && !p.IsMultipleBlocks())
            {
                target->GetTarget()->SetValue(settings, *target);
                server->PostWrite(server->m_Transaction->GetTargets());
            }
            if (server->m_Transaction != NULL)
            {
                delete server->m_Transaction;
                server->m_Transaction = NULL;
            }
            settings.ResetBlockIndex();
        }
    }
    p.SetMultipleBlocks(true);
    return 0;
}

int DLMSLNCommandHandler::HanleSetRequestWithList(
    DLMSSettings& settings,
    unsigned char invoke,
    DLMSServer* server,
    ByteBuffer& data,
    DLMSLNParameters& p,
    DLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    std::string str;
    int ret;
    unsigned long cnt;
    unsigned char attributeIndex, selection, selector;
    DLMS_OBJECT_TYPE ci;
    unsigned short tmp;
    unsigned char* ln;
    DLMSVariant parameters;
    if ((ret = Helpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    std::map<unsigned short, unsigned char> status;
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        xml->IntegerToHex(cnt, 2, str);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_LIST, "Qty", str);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    for (unsigned short pos = 0; pos != cnt; ++pos)
    {
        status[pos] = 0;
        if ((ret = data.GetUInt16(&tmp)) != 0)
        {
            return ret;
        }
        ci = (DLMS_OBJECT_TYPE)tmp;
        ln = data.GetData() + data.GetPosition();
        data.SetPosition(data.GetPosition() + 6);
        // Attribute Id
        if ((ret = data.GetUInt8(&attributeIndex)) != 0)
        {
            return ret;
        }

        // AccessSelection
        if ((ret = data.GetUInt8(&selection)) != 0)
        {
            return ret;
        }
        if (selection != 0)
        {
            if ((ret = data.GetUInt8(&selector)) != 0)
            {
                return ret;
            }
            DataInfo info;
            if ((ret = Helpers::GetData(&settings, data, info, parameters)) != 0)
            {
                return ret;
            }
        }
        else
        {
            selector = 0;
        }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        if (xml != NULL)
        {
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_WITH_SELECTION);
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR);
            xml->AppendComment(DLMSConverter::ToString(ci));
            xml->IntegerToHex((long)ci, 4, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_CLASS_ID, "", str);
            Helpers::GetLogicalName(ln, str);
            xml->AppendComment(str);
            str = Helpers::BytesToHex(ln, 6, false);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_INSTANCE_ID, "", str);
            xml->IntegerToHex((long)attributeIndex, 2, str);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_ID, "", str);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_WITH_SELECTION);
        }
        else
#endif //DLMS_IGNORE_XML_TRANSLATOR
        {
            DLMSObject* obj = settings.GetObjects().FindByLN(ci, ln);
            if (obj == NULL)
            {
                std::string name;
                Helpers::GetLogicalName(ln, name);
                obj = server->FindObject(ci, 0, name);
            }
            if (obj == NULL)
            {
                // Access Error : Device reports a undefined object.
                status[pos] = DLMS_ERROR_CODE_UNDEFINED_OBJECT;
            }
            else
            {
                DLMSValueEventArg e(server, obj, attributeIndex, selector, parameters);
                if ((server->GetAttributeAccess(&e) & DLMS_ACCESS_MODE_WRITE) == 0)
                {
                    status[pos] = DLMS_ERROR_CODE_READ_WRITE_DENIED;
                }
            }
        }
    }
    if ((ret = Helpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_DESCRIPTOR_LIST);
        xml->IntegerToHex(cnt, 2, str);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_VALUE_LIST, "Qty", str);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    for (unsigned short pos = 0; pos != cnt; ++pos)
    {
        DLMSVariant value;
        DataInfo di;
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        di.SetXml(xml);
        if (xml != NULL && xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendStartTag(DLMS_COMMAND_WRITE_REQUEST, DLMS_SINGLE_READ_RESPONSE_DATA);
        }
#endif //DLMS_IGNORE_XML_TRANSLATOR
        if ((ret = Helpers::GetData(&settings, data, di, value)) != 0)
        {
            status[pos] = DLMS_ERROR_CODE_READ_WRITE_DENIED;
        }
        if (ret == 0)
        {
            if (!di.IsComplete())
            {
                value = data.ToHexString(data.GetPosition(), data.Available(), false);
            }
            else if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                value = Helpers::BytesToHex(value.byteArr, value.GetSize(), false);
            }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
            if (xml != NULL && xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
            {
                xml->AppendEndTag(DLMS_COMMAND_WRITE_REQUEST, (unsigned long)DLMS_SINGLE_READ_RESPONSE_DATA);
            }
#endif //DLMS_IGNORE_XML_TRANSLATOR
        }
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_VALUE_LIST);
    }
    else
#endif //DLMS_IGNORE_XML_TRANSLATOR
    {
        p.SetStatus(0xFF);
        Helpers::SetObjectCount((unsigned long)status.size(), *p.GetAttributeDescriptor());
        for (std::map<unsigned short, unsigned char >::iterator it = status.begin(); it != status.end(); ++it)
        {
            p.GetAttributeDescriptor()->SetUInt8(it->second);
        }
        p.SetRequestType(DLMS_SET_RESPONSE_TYPE_WITH_LIST);
    }
    return 0;
}

int DLMSLNCommandHandler::HandleSetRequest(
    DLMSSettings& settings,
    DLMSServer* server,
    ByteBuffer& data,
    ByteBuffer* replyData,
    DLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    DLMSVariant value;
    unsigned char type, invoke;
    int ret;
    DataInfo i;
    ByteBuffer bb;
    // Return error if connection is not established.
    if (xml == NULL && (settings.GetConnected() & DLMS_CONNECTION_STATE_DLMS) == 0)
    {
        server->GenerateConfirmedServiceError(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR,
            DLMS_SERVICE_ERROR_SERVICE, DLMS_SERVICE_UNSUPPORTED,
            *replyData);
        return 0;
    }
    // Get type.
    if ((ret = data.GetUInt8(&type)) != 0)
    {
        return ret;
    }
    // Get invoke ID and priority.
    if ((ret = data.GetUInt8(&invoke)) != 0)
    {
        return ret;
    }
    settings.UpdateInvokeId(invoke);
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        xml->AppendStartTag(DLMS_COMMAND_SET_REQUEST);
        xml->AppendStartTag(DLMS_COMMAND_SET_REQUEST, type);
        // InvokeIdAndPriority
        std::string str;
        xml->IntegerToHex((long)invoke, 2, str);
        xml->AppendLine(DLMS_TRANSLATOR_TAGS_INVOKE_ID, "", str);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR

    ByteBuffer attributeDescriptor;
    DLMSLNParameters p(&settings, invoke, DLMS_COMMAND_SET_RESPONSE, type, &attributeDescriptor, NULL, 0xFF, cipheredCommand);
    if (type == DLMS_SET_COMMAND_TYPE_NORMAL || type == DLMS_SET_COMMAND_TYPE_FIRST_DATABLOCK)
    {
        if (type == DLMS_SET_COMMAND_TYPE_NORMAL)
        {
            p.SetStatus(0);
        }
        ret = HandleSetRequestNormal(settings, server, data, type, p, xml);
    }
    else if (type == DLMS_SET_COMMAND_TYPE_WITH_DATABLOCK)
    {
        // Set Request With Data Block
        ret = HanleSetRequestWithDataBlock(settings, server, data, p, xml);
    }
    else if (type == DLMS_SET_COMMAND_TYPE_WITH_LIST)
    {
        // Set Request With Data Block
        ret = HanleSetRequestWithList(settings, invoke, server, data, p, xml, cipheredCommand);
    }
    else
    {
        settings.ResetBlockIndex();
        p.SetStatus(DLMS_ERROR_CODE_HARDWARE_FAULT);
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_COMMAND_SET_REQUEST, (unsigned long)type);
        xml->AppendEndTag(DLMS_COMMAND_SET_REQUEST);
        return 0;
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    return DLMS::GetLNPdu(p, *replyData);
}

int DLMSLNCommandHandler::MethodRequestNormal(
    DLMSSettings& settings,
    uint8_t invokeId,
    DLMSServer* server,
    ByteBuffer& data,
    DLMSConnectionEventArgs* connectionInfo,
    ByteBuffer* replyData,
    DLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    int ret;
    DLMS_ERROR_CODE error = DLMS_ERROR_CODE_OK;
    unsigned char id, selection;
    unsigned short tmp;
    DLMSVariant parameters;
    DLMSValueEventArg* e = NULL;
    ByteBuffer bb;
    // CI
    if ((ret = data.GetUInt16(&tmp)) != 0)
    {
        return ret;
    }
    DLMS_OBJECT_TYPE ci = (DLMS_OBJECT_TYPE)tmp;
    unsigned char* ln;
    ln = data.GetData() + data.GetPosition();
    data.SetPosition(data.GetPosition() + 6);
    // Attribute ID
    if ((ret = data.GetUInt8(&id)) != 0)
    {
        return ret;
    }
    if ((ret = data.GetUInt8(&selection)) != 0)
    {
        return ret;
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        xml->AppendStartTag(DLMS_COMMAND_METHOD_REQUEST);
        std::string str;
        xml->AppendStartTag(DLMS_COMMAND_METHOD_REQUEST, DLMS_ACTION_REQUEST_TYPE_NORMAL);
        xml->IntegerToHex((unsigned long)invokeId, 2, str);
        xml->AppendLine(DLMS_TRANSLATOR_TAGS_INVOKE_ID, "", str);
        AppendMethodDescriptor(xml, (int)ci, ln, id);
        if (selection != 0)
        {
            DLMSVariant value;
            //MethodInvocationParameters
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_METHOD_INVOCATION_PARAMETERS);
            DataInfo di;
            di.SetXml(xml);
            if ((ret = Helpers::GetData(&settings, data, di, value)) != 0)
            {
                return ret;
            }
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_METHOD_INVOCATION_PARAMETERS);
        }
        xml->AppendEndTag(DLMS_COMMAND_METHOD_REQUEST, (unsigned long)DLMS_ACTION_REQUEST_TYPE_NORMAL);
        xml->AppendEndTag(DLMS_COMMAND_METHOD_REQUEST);
        return 0;
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    // Get parameters.
    if (selection != 0)
    {
        DataInfo i;
        if ((ret = Helpers::GetData(&settings, data, i, parameters)) != 0)
        {
            return ret;
        }
    }
    DLMSObject* obj = settings.GetObjects().FindByLN(ci, ln);
    if (obj == NULL)
    {
        std::string name;
        Helpers::GetLogicalName(ln, name);
        obj = server->FindObject(ci, 0, name);
    }
    if (obj == NULL)
    {
        error = DLMS_ERROR_CODE_UNDEFINED_OBJECT;
    }
    else
    {
        e = new DLMSValueEventArg(server, obj, id, 0, parameters);
        DLMSValueEventCollection arr;
        arr.push_back(e);
        if (server->GetMethodAccess(e) == DLMS_METHOD_ACCESS_MODE_NONE)
        {
            error = DLMS_ERROR_CODE_READ_WRITE_DENIED;
        }
        else
        {
            server->PreAction(arr);
            if (!e->GetHandled())
            {
                if ((ret = obj->Invoke(settings, *e)) != 0)
                {
                    return ret;
                }
                server->PostAction(arr);
            }
            DLMSVariant& actionReply = e->GetValue();
            // Set default action reply if not given.
            if (actionReply.vt != DLMS_DATA_TYPE_NONE && e->GetError() == DLMS_ERROR_CODE_OK)
            {
                // Add return parameters
                bb.SetUInt8(1);
                //Add parameters error code.
                bb.SetUInt8(0);
                Helpers::SetData(&settings, bb, actionReply.vt, actionReply);
            }
            else
            {
                // Add parameters error code.
                error = e->GetError();
                bb.SetUInt8(0);
            }
        }
    }
    DLMSLNParameters p(&settings, invokeId, DLMS_COMMAND_METHOD_RESPONSE, 1, NULL, &bb, error, cipheredCommand);
    ret = DLMS::GetLNPdu(p, *replyData);
#ifndef DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
    if (error == 0 && obj->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME && id == 1)
    {
        if (((DLMSAssociationLogicalName*)obj)->GetAssociationStatus() == DLMS_ASSOCIATION_STATUS_ASSOCIATED)
        {
            server->Connected(*connectionInfo);
            settings.SetConnected((DLMS_CONNECTION_STATE)(settings.GetConnected() | DLMS_CONNECTION_STATE_DLMS));
        }
        else
        {
            // If High level authentication fails.
            server->InvalidConnection(*connectionInfo);
            settings.SetConnected((DLMS_CONNECTION_STATE)(settings.GetConnected() & ~DLMS_CONNECTION_STATE_DLMS));
        }
    }
#endif //DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
#ifndef DLMS_IGNORE_SECURITY_SETUP
    if (e != NULL && error == 0 && obj->GetObjectType() == DLMS_OBJECT_TYPE_SECURITY_SETUP && id == 2)
    {
        ((DLMSSecuritySetup*)obj)->ApplyKeys(settings, *e);
    }
#endif //DLMS_IGNORE_SECURITY_SETUP
    return 0;
}


int DLMSLNCommandHandler::MethodRequestNextBlock(
    DLMSSettings& settings,
    uint8_t invokeId,
    DLMSServer* server,
    ByteBuffer& data,
    DLMSConnectionEventArgs* connectionInfo,
    ByteBuffer* replyData,
    DLMSTranslatorStructure* xml,
    bool streaming,
    unsigned char cipheredCommand)
{
    int ret = 0;
    ByteBuffer bb;
    if (!streaming)
    {
        unsigned long index;
        // Get block index.
        if ((ret = data.GetUInt32(&index)) != 0)
        {
            return ret;
        }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        if (xml != NULL)
        {
            std::string tmp2;
            xml->IntegerToHex(index, 8, tmp2);
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_BLOCK_NUMBER, "", tmp2);
            return 0;
        }
#endif //DLMS_IGNORE_XML_TRANSLATOR
        if (index != settings.GetBlockIndex())
        {
            DLMSLNParameters p(&settings, invokeId, DLMS_COMMAND_METHOD_RESPONSE, DLMS_ACTION_REQUEST_TYPE_NEXT_BLOCK,
                NULL, NULL, DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID, cipheredCommand);
            return DLMS::GetLNPdu(p, *replyData);
        }
    }
    settings.IncreaseBlockIndex();
    DLMSLNParameters p(&settings, 
        invokeId, 
        streaming ? DLMS_COMMAND_GENERAL_BLOCK_TRANSFER : DLMS_COMMAND_METHOD_RESPONSE, 
        DLMS_ACTION_REQUEST_TYPE_NEXT_BLOCK,
        NULL, &bb, DLMS_ERROR_CODE_OK, cipheredCommand);

    p.SetStreaming(streaming);
    p.SetWindowSize(settings.GetGbtWindowSize());

    bool is_null_transaction = true;

    if(server != nullptr)
    {
        if(server->m_Transaction != nullptr)
        {
            is_null_transaction = false;
        }
    }

    //If transaction is not in progress.
    //if (&server->m_Transaction == NULL)
    if (is_null_transaction)
    {
        p.SetStatus(DLMS_ERROR_CODE_NO_LONG_GET_OR_READ_IN_PROGRESS);
    }
    else
    {
        bb.Set(&server->m_Transaction->GetData());
        bool moreData = settings.GetIndex() != settings.GetCount();
        if (moreData)
        {
            //If there is multiple blocks on the buffer.
            //This might happen when Max PDU size is very small.
            if (bb.GetSize() < settings.GetMaxPduSize())
            {
                DLMSVariant value;
                for (std::vector<DLMSValueEventArg*>::iterator arg = server->m_Transaction->GetTargets().begin();
                    arg != server->m_Transaction->GetTargets().end(); ++arg)
                {
                    server->PreAction(server->m_Transaction->GetTargets());
                    if (!(*arg)->GetHandled())
                    {
                        if ((ret = (*arg)->GetTarget()->Invoke(settings, *(*arg))) != 0)
                        {
                            return ret;
                        }
                    }
                    std::vector<DLMSValueEventArg*> arr;
                    arr.push_back(*arg);
                    server->PostAction(arr);
                    value = (*arg)->GetValue();
                    //Add data.
                    // Add data.
                    if ((*arg)->IsByteArray() && value.vt == DLMS_DATA_TYPE_OCTET_STRING)
                    {
                        // If byte array is added do not add type.
                        bb.Set(value.byteArr, value.GetSize());
                    }
                    else if ((ret = DLMS::AppendData(&settings, (*arg)->GetTarget(), (*arg)->GetIndex(), bb, value)) != 0)
                    {
                        return DLMS_ERROR_CODE_HARDWARE_FAULT;
                    }
                }
                moreData = settings.GetIndex() != settings.GetCount();
            }
        }
        p.SetMultipleBlocks(true);
        ret = DLMS::GetLNPdu(p, *replyData);
        moreData = settings.GetIndex() != settings.GetCount();
        if (moreData || bb.GetSize() - bb.GetPosition() != 0)
        {
            server->m_Transaction->SetData(bb);
        }
        else
        {
            delete server->m_Transaction;
            server->m_Transaction = NULL;
            settings.ResetBlockIndex();
        }
    }
    return ret;
}

/**
 * Handle action request.
 *
 * @param reply
 *            Received data from the client.
 * @return Reply.
 */
int DLMSLNCommandHandler::HandleMethodRequest(
    DLMSSettings& settings,
    DLMSServer* server,
    ByteBuffer& data,
    ByteBuffer* replyData,
    DLMSConnectionEventArgs* connectionInfo,
    DLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    int ret;
    unsigned char invokeId, type;
    // Get type.
    if ((ret = data.GetUInt8(&type)) != 0)
    {
        return ret;
    }
    // Get invoke ID and priority.
    if ((ret = data.GetUInt8(&invokeId)) != 0)
    {
        return ret;
    }
    settings.UpdateInvokeId(invokeId);
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    DLMS::AddInvokeId(xml, DLMS_COMMAND_METHOD_REQUEST, type, invokeId);
#endif //DLMS_IGNORE_XML_TRANSLATOR

    switch (type)
    {
    case DLMS_ACTION_REQUEST_TYPE_NORMAL:
        ret = MethodRequestNormal(settings, invokeId, server, data, connectionInfo, replyData, xml, cipheredCommand);
        break;
    case DLMS_ACTION_REQUEST_TYPE_NEXT_BLOCK:
        ret = MethodRequestNextBlock(settings, invokeId, server, data, connectionInfo, replyData, xml, false, cipheredCommand);
        break;
    case DLMS_ACTION_REQUEST_TYPE_WITH_FIRST_BLOCK:
        break;
    case DLMS_ACTION_REQUEST_TYPE_WITH_BLOCK:
        break;
    default:
        if (xml == NULL)
        {
            settings.ResetBlockIndex();
            type = DLMS_ACTION_REQUEST_TYPE_NORMAL;
            data.Clear();
            DLMSLNParameters p(&settings, invokeId, DLMS_COMMAND_METHOD_RESPONSE, DLMS_ACTION_REQUEST_TYPE_NORMAL,
                NULL, NULL, DLMS_ERROR_CODE_READ_WRITE_DENIED, cipheredCommand);
            ret = DLMS::GetLNPdu(p, *replyData);
        }
        break;
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        if (type <= DLMS_ACTION_REQUEST_TYPE_WITH_BLOCK)
        {
            xml->AppendEndTag(DLMS_COMMAND_METHOD_REQUEST, (unsigned long)type);
        }
        xml->AppendEndTag(DLMS_COMMAND_METHOD_REQUEST);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    return ret;
}

int DLMSLNCommandHandler::HandleAccessRequest(
    DLMSSettings& settings,
    DLMSServer* server,
    ByteBuffer& data,
    ByteBuffer* replyData,
    DLMSTranslatorStructure* xml,
    unsigned char cipheredCommand)
{
    //Return error if connection is not established.
    if (xml == NULL && settings.GetConnected() == DLMS_CONNECTION_STATE_NONE)
    {
        server->GenerateConfirmedServiceError(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR,
            DLMS_SERVICE_ERROR_SERVICE, DLMS_SERVICE_UNSUPPORTED,
            *replyData);
        return 0;
    }
    int ret;
    unsigned char ch;
    //Get long invoke id and priority.
    unsigned long invokeId;
    if ((ret = data.GetUInt32(&invokeId)) != 0)
    {
        return ret;
    }
    settings.SetLongInvokeID(invokeId);
    unsigned long len, cnt;
    if ((ret = Helpers::GetObjectCount(data, len)) != 0)
    {
        return ret;
    }

    ByteBuffer tmp;
    // If date time is given.
    if (len != 0)
    {
        tmp.Set(&data, data.GetPosition(), len);
        if (xml == NULL)
        {
            DLMS_DATA_TYPE dt = DLMS_DATA_TYPE_DATETIME;
            if (len == 4)
            {
                dt = DLMS_DATA_TYPE_TIME;
            }
            else if (len == 5)
            {
                dt = DLMS_DATA_TYPE_DATE;
            }
            DataInfo info;
            DLMSVariant value;
            info.SetType(dt);
            if ((ret = Helpers::GetData(&settings, data, info, value)) != 0)
            {
                return ret;
            }
        }
    }
    // Get object count.
    if ((ret = Helpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        std::string tmp2;
        xml->IntegerToHex(invokeId, 2, tmp2);
        xml->AppendStartTag(DLMS_COMMAND_ACCESS_REQUEST);
        xml->AppendLine(DLMS_TRANSLATOR_TAGS_LONG_INVOKE_ID, "Value", tmp2);
        tmp2 = tmp.ToHexString(false);
        xml->AppendLine(DLMS_TRANSLATOR_TAGS_DATE_TIME, "Value", tmp2);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_BODY);
        xml->IntegerToHex(cnt, 2, tmp2);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_LIST_OF_ACCESS_REQUEST_SPECIFICATION, "Qty", tmp2);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    DLMS_ACCESS_SERVICE_COMMAND_TYPE type;
    unsigned short id;
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    unsigned char* ln;
#endif //DLMS_IGNORE_XML_TRANSLATOR
    unsigned char attributeIndex;
    std::vector<DLMSAccessItem> list;
    for (unsigned long pos = 0; pos != cnt; ++pos)
    {
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        type = (DLMS_ACCESS_SERVICE_COMMAND_TYPE)ch;
        if (!(type == DLMS_ACCESS_SERVICE_COMMAND_TYPE_GET ||
            type == DLMS_ACCESS_SERVICE_COMMAND_TYPE_SET ||
            type == DLMS_ACCESS_SERVICE_COMMAND_TYPE_ACTION))
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        // CI
        if ((ret = data.GetUInt16(&id)) != 0)
        {
            return ret;
        }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        ln = data.GetData() + data.GetPosition();
#endif //DLMS_IGNORE_XML_TRANSLATOR
        data.SetPosition(data.GetPosition() + 6);
        // Attribute Id
        if ((ret = data.GetUInt8(&attributeIndex)) != 0)
        {
            return ret;
        }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        if (xml != NULL)
        {
            xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_SPECIFICATION);
            xml->AppendStartTag(DLMS_COMMAND_ACCESS_REQUEST, type);
            AppendAttributeDescriptor(xml, id, ln, attributeIndex);
            xml->AppendEndTag(DLMS_COMMAND_ACCESS_REQUEST, (unsigned long)type);
            xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_SPECIFICATION);
        }
        else
        {
            list.push_back(DLMSAccessItem(type, settings.GetObjects().FindByLN((DLMS_OBJECT_TYPE)id, ln), attributeIndex));
        }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        std::string tmp2;
        xml->IntegerToHex(cnt, 2, tmp2);
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_LIST_OF_ACCESS_REQUEST_SPECIFICATION);
        xml->AppendStartTag(DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_LIST_OF_DATA, "Qty", tmp2);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    // Get data count.
    if ((ret = Helpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    ByteBuffer bb;
    // access-request-specification.
    bb.SetUInt8(0);
    Helpers::SetObjectCount(cnt, bb);
    ByteBuffer results;
    Helpers::SetObjectCount(cnt, results);
    DLMSValueEventCollection args;
    for (unsigned long pos = 0; pos != cnt; ++pos)
    {
        DLMSValueEventArg* e = new DLMSValueEventArg(server, NULL, 0);
        args.push_back(e);
        DLMSVariant value;
        DataInfo di;
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        di.SetXml(xml);
        if (xml != NULL && xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendStartTag(DLMS_COMMAND_WRITE_REQUEST, DLMS_SINGLE_READ_RESPONSE_DATA);
        }
#endif //DLMS_IGNORE_XML_TRANSLATOR
        if ((ret = Helpers::GetData(&settings, data, di, value)) != 0)
        {
            return ret;
        }
        if (!di.IsComplete())
        {
            value = data.ToHexString(data.GetPosition(), data.Available(), false);
        }
        else if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            value = Helpers::BytesToHex(value.byteArr, value.GetSize());
        }
        if (xml == NULL)
        {
            DLMSAccessItem& it = list.at(pos);
            results.SetUInt8(it.GetCommand());
            if (it.GetTarget() == NULL)
            {
                //If target is unknown.
                bb.SetUInt8(0);
                results.SetUInt8(DLMS_ERROR_CODE_UNAVAILABLE_OBJECT);
            }
            else
            {
                e->SetTarget(it.GetTarget());
                e->SetIndex(it.GetIndex());
                e->SetValue(value);
                if (it.GetCommand() == DLMS_ACCESS_SERVICE_COMMAND_TYPE_GET)
                {
                    if ((server->GetAttributeAccess(e) & DLMS_ACCESS_MODE_READ) == 0)
                    {
                        //Read Write denied.
                        results.SetUInt8(DLMS_ERROR_CODE_READ_WRITE_DENIED);
                    }
                    else
                    {
                        server->PreRead(args);
                        if (e->GetHandled())
                        {
                            value = e->GetValue();
                        }
                        else
                        {
                            if ((ret = it.GetTarget()->GetValue(settings, *e)) == 0)
                            {
                                value = e->GetValue();
                            }
                        }
                        //If all data is not fit to PDU and GBT is not used.
                        if (settings.GetIndex() != settings.GetCount())
                        {
                            settings.SetCount(0);
                            settings.SetIndex(0);
                            bb.SetUInt8(0);
                            results.SetUInt8(DLMS_ERROR_CODE_READ_WRITE_DENIED);
                        }
                        else
                        {
                            if (e->IsByteArray())
                            {
                                // If byte array is added do not add type.
                                bb.Set(value.byteArr, value.GetSize());
                            }
                            else if ((ret = DLMS::AppendData(&settings, it.GetTarget(), it.GetIndex(), bb, value)) != 0)
                            {
                                results.SetUInt8(DLMS_ERROR_CODE_HARDWARE_FAULT);
                            }
                            server->PostRead(args);
                            if (ret == 0)
                            {
                                results.SetUInt8(DLMS_ERROR_CODE_OK);
                            }
                        }
                    }
                }
                else if (it.GetCommand() == DLMS_ACCESS_SERVICE_COMMAND_TYPE_SET)
                {
                    results.SetUInt8(DLMS_ERROR_CODE_OK);
                }
                else
                {
                    results.SetUInt8(DLMS_ERROR_CODE_OK);
                }
            }
        }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        if (xml != NULL && xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
        {
            xml->AppendEndTag(DLMS_COMMAND_WRITE_REQUEST, (unsigned long)DLMS_SINGLE_READ_RESPONSE_DATA);
        }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (xml != NULL)
    {
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_LIST_OF_DATA);
        xml->AppendEndTag(DLMS_TRANSLATOR_TAGS_ACCESS_REQUEST_BODY);
        xml->AppendEndTag(DLMS_COMMAND_ACCESS_REQUEST);
    }
    else
#endif //DLMS_IGNORE_XML_TRANSLATOR
    {
        // Append status codes.
        bb.Set(&results);
        DLMSLNParameters p(&settings, invokeId, DLMS_COMMAND_ACCESS_RESPONSE, 0xFF, NULL, &bb, 0xFF, cipheredCommand);
        return DLMS::GetLNPdu(p, *replyData);
    }
    return 0;
}

// Handle Event Notification.
int DLMSLNCommandHandler::HandleEventNotification(
    DLMSSettings& settings,
    ReplyData& reply)
{
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
        if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        tmp.Set(&reply.GetData(), reply.GetData().GetPosition(), ch);
        DLMSVariant value;
        if ((ret = DLMSClient::ChangeType(tmp, DLMS_DATA_TYPE_DATETIME, value)) != 0)
        {
            return ret;
        }
        reply.SetTime(&value.dateTime.GetValue());
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (reply.GetXml() != NULL)
    {
        reply.GetXml()->AppendStartTag(DLMS_COMMAND_EVENT_NOTIFICATION);
        if (reply.GetTime() != 0)
        {
            DateTime dt(reply.GetTime());
            reply.GetXml()->AppendComment(dt.ToString());
            std::string str = tmp.ToHexString(false);
            reply.GetXml()->AppendLine(DLMS_TRANSLATOR_TAGS_TIME, "", str);
        }
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    unsigned short ci;
    if ((ret = reply.GetData().GetUInt16(&ci)) != 0)
    {
        return ret;
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    unsigned char* ln = reply.GetData().GetData() + reply.GetData().GetPosition();
#endif //DLMS_IGNORE_XML_TRANSLATOR
    reply.GetData().SetPosition(reply.GetData().GetPosition() + 6);
    if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (reply.GetXml() != NULL)
    {
        AppendAttributeDescriptor(reply.GetXml(), ci, ln, ch);
        reply.GetXml()->AppendStartTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_VALUE);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    DataInfo di;
    DLMSVariant value;
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    di.SetXml(reply.GetXml());
#endif //DLMS_IGNORE_XML_TRANSLATOR
    if ((ret = Helpers::GetData(&settings, reply.GetData(), di, value)) != 0)
    {
        return ret;
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    if (reply.GetXml() != NULL)
    {
        reply.GetXml()->AppendEndTag(DLMS_TRANSLATOR_TAGS_ATTRIBUTE_VALUE);
        reply.GetXml()->AppendEndTag(DLMS_COMMAND_EVENT_NOTIFICATION);
    }
#endif //DLMS_IGNORE_XML_TRANSLATOR
    return 0;
}

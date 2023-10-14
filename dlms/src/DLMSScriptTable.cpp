#include "DLMSScriptTable.h"
#include "DLMSClient.h"
#include <sstream>
#ifndef DLMS_IGNORE_SCRIPT_TABLE
//Constructor.
DLMSScriptTable::DLMSScriptTable() : DLMSScriptTable("0.0.10.0.100.255", 0)
{
}

//SN Constructor.
DLMSScriptTable::DLMSScriptTable(std::string ln, unsigned short sn) : DLMSObject(DLMS_OBJECT_TYPE_SCRIPT_TABLE, ln, sn)
{

}

//LN Constructor.
DLMSScriptTable::DLMSScriptTable(std::string ln) : DLMSScriptTable(ln, 0)
{

}

//Destructor.
DLMSScriptTable::~DLMSScriptTable()
{
    for (std::vector<DLMSScript*>::iterator it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
    {
        delete *it;
    }
    m_Scripts.clear();
}


std::vector<DLMSScript*>& DLMSScriptTable::GetScripts()
{
    return m_Scripts;
}

int DLMSScriptTable::Execute(DLMSClient* client, DLMSScript* script, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data(script->GetID());
    data.vt = DLMS_DATA_TYPE_UINT16;
    return client->Method(this, 1, data, reply);
}

int DLMSScriptTable::Execute(DLMSClient* client, unsigned short scriptId, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data(scriptId);
    data.vt = DLMS_DATA_TYPE_UINT16;
    return client->Method(this, 1, data, reply);
}


// Returns amount of attributes.
int DLMSScriptTable::GetAttributeCount()
{
    return 2;
}

// Returns amount of methods.
int DLMSScriptTable::GetMethodCount()
{
    return 1;
}

void DLMSScriptTable::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);

    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<DLMSScript*>::iterator it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        sb << (*it)->GetID();
        for (std::vector<DLMSScriptAction*>::iterator a = (*it)->GetActions().begin(); a != (*it)->GetActions().end(); ++a)
        {
            sb << " ";
            std::string str = (*a)->ToString();
            sb.write(str.c_str(), str.size());
        }
    }
    sb << ']';
    values.push_back(sb.str());
}

void DLMSScriptTable::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Scripts
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
}

int DLMSScriptTable::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    //Scripts
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Returns value of given attribute.
int DLMSScriptTable::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        int ret;
        DLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 2)
    {
        e.SetByteArray(true);
        ByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        Helpers::SetObjectCount((unsigned long)m_Scripts.size(), data);
        int ret;
        DLMSVariant id, type, oType, ln, index;
        for (std::vector<DLMSScript*>::iterator it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            //Count
            data.SetUInt8(2);
            //Script_identifier:
            id = (*it)->GetID();
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, id)) != 0)
            {
                return ret;
            }
            data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
            //Count
            Helpers::SetObjectCount((unsigned long)(*it)->GetActions().size(), data);
            for (std::vector<DLMSScriptAction*>::iterator a = (*it)->GetActions().begin(); a != (*it)->GetActions().end(); ++a)
            {
                Helpers::SetLogicalName((*a)->GetLogicalName().c_str(), ln);
                data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
                data.SetUInt8(5);
                type = (*a)->GetType();
                oType = (*a)->GetObjectType();
                index = (*a)->GetIndex();
                DLMSVariant param = (*a)->GetParameter();
                if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_ENUM, type)) != 0 || //service_id
                    (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, oType)) != 0 || //class_id
                    (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != 0 || //logical_name
                    (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_INT8, index)) != 0 || //index
                    (ret = Helpers::SetData(&settings, data, (*a)->GetParameter().vt, param)) != 0) //parameter
                {
                    return ret;
                }
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int DLMSScriptTable::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        for (std::vector<DLMSScript*>::iterator it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
        {
            delete *it;
        }
        m_Scripts.clear();
        //Fix Xemex bug here.
        //Xemex meters do not return array as they shoul be according standard.
        if (e.GetValue().Arr.size() != 0)
        {
            if (e.GetValue().Arr[0].vt == DLMS_DATA_TYPE_STRUCTURE)
            {
                std::string ln;
                for (std::vector<DLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
                {
                    DLMSScript* script = new DLMSScript();
                    script->SetID((*item).Arr[0].ToInteger());
                    m_Scripts.push_back(script);
                    for (std::vector<DLMSVariant>::iterator arr = (*item).Arr[1].Arr.begin(); arr != (*item).Arr[1].Arr.end(); ++arr)
                    {
                        DLMSScriptAction* it = new DLMSScriptAction();
                        SCRIPT_ACTION_TYPE type = (SCRIPT_ACTION_TYPE)(*arr).Arr[0].ToInteger();
                        it->SetType(type);
                        DLMS_OBJECT_TYPE ot = (DLMS_OBJECT_TYPE)(*arr).Arr[1].ToInteger();
                        it->SetObjectType(ot);
                        ln.clear();
                        Helpers::GetLogicalName((*arr).Arr[2].byteArr, ln);
                        it->SetLogicalName(ln);
                        it->SetIndex((*arr).Arr[3].ToInteger());
                        it->SetParameter((*arr).Arr[4]);
                        script->GetActions().push_back(it);
                    }
                }
            }
            else //Read Xemex meter here.
            {
                DLMSScript* script = new DLMSScript();
                script->SetID(e.GetValue().Arr[0].ToInteger());
                m_Scripts.push_back(script);
                DLMSScriptAction *it = new DLMSScriptAction();
                SCRIPT_ACTION_TYPE type = (SCRIPT_ACTION_TYPE)e.GetValue().Arr[1].Arr[0].ToInteger();
                it->SetType(type);
                DLMS_OBJECT_TYPE ot = (DLMS_OBJECT_TYPE)e.GetValue().Arr[1].Arr[1].ToInteger();
                it->SetObjectType(ot);
                std::string ln;
                Helpers::GetLogicalName(e.GetValue().Arr[1].Arr[2].byteArr, ln);
                it->SetLogicalName(ln);
                it->SetIndex(e.GetValue().Arr[1].Arr[3].ToInteger());
                it->SetParameter(e.GetValue().Arr[1].Arr[4]);
                script->GetActions().push_back(it);
            }
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_SCRIPT_TABLE

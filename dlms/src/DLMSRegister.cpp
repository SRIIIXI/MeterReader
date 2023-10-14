#include <cmath>
#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSRegister.h"
#include "DLMSConverter.h"

#ifndef DLMS_IGNORE_REGISTER
bool DLMSRegister::IsRead(int index)
{
    if (index == 3)
    {
        return m_Unit != 0;
    }
    return DLMSObject::IsRead(index);
}

//Constructor.
DLMSRegister::DLMSRegister() :
    DLMSRegister("", 0)
{
}

DLMSRegister::DLMSRegister(DLMS_OBJECT_TYPE type, std::string ln, unsigned short sn) :
    DLMSObject(type, ln, sn)
{
    m_Unit = 0;
    m_Scaler = 0;
}

//SN Constructor.
DLMSRegister::DLMSRegister(std::string ln, unsigned short sn) :
    DLMSRegister(DLMS_OBJECT_TYPE_REGISTER, ln, sn)
{
}

//LN Constructor.
DLMSRegister::DLMSRegister(std::string ln) :
    DLMSRegister(ln, 0)
{
}

/// <summary>
/// Get value of COSEM Data object.
/// </summary>
DLMSVariant& DLMSRegister::GetValue()
{
    return m_Value;
}

/// <summary>
/// Set value of COSEM Data object.
/// </summary>
void DLMSRegister::SetValue(DLMSVariant& value)
{
    m_Value = value;
}

// Scaler of COSEM Register object.
double DLMSRegister::GetScaler()
{
    return pow((float)10, m_Scaler);
}

void DLMSRegister::SetScaler(double value)
{
    m_Scaler = (signed char)log10(value);
}

// Unit of COSEM Register object.
int DLMSRegister::GetUnit()
{
    return m_Unit;
}

void DLMSRegister::SetUnit(unsigned char value)
{
    m_Unit = value;
}

/*
 * Reset value.
 */
void DLMSRegister::Reset()
{
    m_Value.Clear();
}

// Returns amount of attributes.
int DLMSRegister::GetAttributeCount()
{
    return 3;
}

// Returns amount of methods.
int DLMSRegister::GetMethodCount()
{
    return 1;
}

int DLMSRegister::Reset(
    DLMSClient* client,
    std::vector<ByteBuffer>& reply)
{
    DLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int DLMSRegister::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_Value.Clear();
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

void DLMSRegister::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_Value.ToString());
    std::string str = "Scaler: ";
    double s = GetScaler();
    //if there is no fractal part.
    if (s - (long)s == 0)
    {
        str += DLMSVariant((long)s).ToString();
    }
    else
    {
        str += DLMSVariant(s).ToString();
    }
    str += " Unit: ";
    str += DLMSConverter::GetUnitAsString(m_Unit);
    values.push_back(str);
}

void DLMSRegister::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //ScalerUnit
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //Value
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
}

int DLMSRegister::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        return DLMSObject::GetDataType(index, type);
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int DLMSRegister::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret;
    DLMSVariant tmp;
    if (e.GetIndex() == 1)
    {
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 2)
    {
        DLMS_DATA_TYPE dt;
        if ((ret = DLMSObject::GetDataType(2, dt)) != 0)
        {
            return ret;
        }
        tmp = m_Value;
        if (m_Scaler != 0)
        {
            if (dt == DLMS_DATA_TYPE_NONE)
            {
                dt = m_Value.vt;
            }
            if ((ret = tmp.ChangeType(DLMS_DATA_TYPE_FLOAT64)) != 0)
            {
                return ret;
            }
            tmp = m_Value.dblVal / GetScaler();
            if ((ret = tmp.ChangeType(dt)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        ByteBuffer buff;
        buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        buff.SetUInt8(2);
        buff.SetUInt8(DLMS_DATA_TYPE_INT8);
        buff.SetUInt8(m_Scaler);
        buff.SetUInt8(DLMS_DATA_TYPE_ENUM);
        buff.SetUInt8(m_Unit);
        e.SetValue(buff);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int DLMSRegister::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        if (m_Scaler != 0 && e.GetValue().IsNumber())
        {
            double val = GetScaler();
            val *= e.GetValue().ToDouble();
            DLMSVariant tmp(val);
            SetValue(tmp);
        }
        else
        {
            SetValue(e.GetValue());
        }
    }
    else if (e.GetIndex() == 3 && e.GetValue().vt == DLMS_DATA_TYPE_STRUCTURE)
    {
        m_Scaler = e.GetValue().Arr[0].ToInteger();
        m_Unit = e.GetValue().Arr[1].ToInteger();
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_REGISTER

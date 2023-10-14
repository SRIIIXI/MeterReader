#include <cmath>
#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSConverter.h"
#include "DLMSDemandRegister.h"

#ifndef DLMS_IGNORE_DEMAND_REGISTER
bool DLMSDemandRegister::IsRead(int index)
{
    if (index == 4)
    {
        return m_Unit != 0;
    }
    return DLMSObject::IsRead(index);
}

DLMSDemandRegister::DLMSDemandRegister() :
    DLMSDemandRegister("", 0)
{
}

DLMSDemandRegister::DLMSDemandRegister(std::string ln) :
    DLMSDemandRegister(ln, 0)
{
}

DLMSDemandRegister::DLMSDemandRegister(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_DEMAND_REGISTER, ln, sn)
{
    m_Period = m_NumberOfPeriods = m_Unit = m_Scaler = 0;
}

DLMSVariant& DLMSDemandRegister::GetCurrentAverageValue()
{
    return m_CurrentAverageValue;
}
void DLMSDemandRegister::SetCurrentAverageValue(DLMSVariant& value)
{
    m_CurrentAverageValue = value;
}

void DLMSDemandRegister::SetCurrentAverageValue(DLMSVariant value)
{
    m_CurrentAverageValue = value;
}

DLMSVariant& DLMSDemandRegister::GetLastAverageValue()
{
    return m_LastAverageValue;
}
void DLMSDemandRegister::SetLastAverageValue(DLMSVariant& value)
{
    m_LastAverageValue = value;
}
void DLMSDemandRegister::SetLastAverageValue(DLMSVariant value)
{
    m_LastAverageValue = value;
}

double DLMSDemandRegister::GetScaler()
{
    return pow((float)10, m_Scaler);
}

void DLMSDemandRegister::SetScaler(double value)
{
    m_Scaler = (signed char)log10(value);
}

int DLMSDemandRegister::GetUnit()
{
    return m_Unit;
}

void DLMSDemandRegister::SetUnit(unsigned char value)
{
    m_Unit = value;
}

DLMSVariant& DLMSDemandRegister::GetStatus()
{
    return m_Status;
}
void DLMSDemandRegister::SetStatus(DLMSVariant& value)
{
    m_Status = value;
}

void DLMSDemandRegister::SetStatus(DLMSVariant value)
{
    m_Status = value;
}

DateTime& DLMSDemandRegister::GetCaptureTime()
{
    return m_CaptureTime;
}
void DLMSDemandRegister::SetCaptureTime(DateTime value)
{
    m_CaptureTime = value;
}

DateTime& DLMSDemandRegister::GetStartTimeCurrent()
{
    return m_StartTimeCurrent;
}
void DLMSDemandRegister::SetStartTimeCurrent(DateTime& value)
{
    m_StartTimeCurrent = value;
}

unsigned long DLMSDemandRegister::GetPeriod()
{
    return m_Period;
}

void DLMSDemandRegister::SetPeriod(unsigned long value)
{
    m_Period = value;
}

int DLMSDemandRegister::GetNumberOfPeriods()
{
    return m_NumberOfPeriods;
}

void DLMSDemandRegister::SetNumberOfPeriods(int value)
{
    m_NumberOfPeriods = value;
}

int DLMSDemandRegister::Reset(DLMSClient* client,
    std::vector<ByteBuffer>& reply)
{
    DLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int DLMSDemandRegister::NextPeriod(DLMSClient* client,
    std::vector<ByteBuffer>& reply)
{
    DLMSVariant data((char)0);
    return client->Method(this, 2, data, reply);
}

void DLMSDemandRegister::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_CurrentAverageValue.ToString());
    values.push_back(m_LastAverageValue.ToString());
    std::string str = "Scaler: ";
    //if there is no fractal part.
    double s = GetScaler();
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
    values.push_back(m_Status.ToString());
    values.push_back(m_CaptureTime.ToString());
    values.push_back(m_StartTimeCurrent.ToString());
    values.push_back(DLMSVariant(m_Period).ToString());
    values.push_back(DLMSVariant(m_NumberOfPeriods).ToString());
}

void DLMSDemandRegister::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //ScalerUnit
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //CurrentAverageValue
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //LastAverageValue
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //Status
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    //CaptureTime
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    //StartTimeCurrent
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
    //Period
    if (all || CanRead(8))
    {
        attributes.push_back(8);
    }
    //NumberOfPeriods
    if (all || CanRead(9))
    {
        attributes.push_back(9);
    }
}

int DLMSDemandRegister::GetAttributeCount()
{
    return 9;
}

int DLMSDemandRegister::GetMethodCount()
{
    return 2;
}

int DLMSDemandRegister::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_CurrentAverageValue.Clear();
        m_LastAverageValue.Clear();
        m_CaptureTime = DateTime(DateTime::Now());
        m_StartTimeCurrent = DateTime(DateTime::Now());
    }
    else if (e.GetIndex() == 2)
    {
        m_LastAverageValue = m_CurrentAverageValue;
        m_CurrentAverageValue.Clear();
        m_CaptureTime = DateTime(DateTime::Now());
        m_StartTimeCurrent = DateTime(DateTime::Now());
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

int DLMSDemandRegister::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        return DLMSObject::GetDataType(index, type);
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 5)
    {
        type = m_Status.vt;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 6)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 7)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 8)
    {
        type = DLMS_DATA_TYPE_UINT32;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 9)
    {
        type = DLMS_DATA_TYPE_UINT16;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int DLMSDemandRegister::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        tmp = m_CurrentAverageValue;
        if (m_Scaler != 0)
        {
            DLMS_DATA_TYPE dt;
            if ((ret = DLMSObject::GetDataType(2, dt)) != 0)
            {
                return ret;
            }
            if (dt == DLMS_DATA_TYPE_NONE)
            {
                dt = m_CurrentAverageValue.vt;
            }
            if ((ret = tmp.ChangeType(DLMS_DATA_TYPE_FLOAT64)) != 0)
            {
                return ret;
            }
            tmp = m_CurrentAverageValue.dblVal / GetScaler();
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
        tmp = m_LastAverageValue;
        if (m_Scaler != 0)
        {
            DLMS_DATA_TYPE dt;
            if ((ret = DLMSObject::GetDataType(2, dt)) != 0)
            {
                return ret;
            }
            if (dt == DLMS_DATA_TYPE_NONE)
            {
                dt = m_LastAverageValue.vt;
            }
            if ((ret = tmp.ChangeType(DLMS_DATA_TYPE_FLOAT64)) != 0)
            {
                return ret;
            }
            tmp = m_LastAverageValue.dblVal / GetScaler();
            if ((ret = tmp.ChangeType(dt)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.GetValue().Clear();
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
    if (e.GetIndex() == 5)
    {
        e.SetValue(m_Status);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        e.SetValue(m_CaptureTime);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 7)
    {
        e.SetValue(m_StartTimeCurrent);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 8)
    {
        e.SetValue(m_Period);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 9)
    {
        e.SetValue(m_NumberOfPeriods);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int DLMSDemandRegister::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        if (m_Scaler != 0 && e.GetValue().IsNumber())
        {
            m_CurrentAverageValue = e.GetValue().ToDouble() * GetScaler();
        }
        else
        {
            m_CurrentAverageValue = e.GetValue();
        }
    }
    else if (e.GetIndex() == 3)
    {
        if (m_Scaler != 0 && e.GetValue().IsNumber())
        {
            m_LastAverageValue = e.GetValue().ToDouble() * GetScaler();
        }
        else
        {
            m_LastAverageValue = e.GetValue();
        }
    }
    else if (e.GetIndex() == 4 && e.GetValue().vt == DLMS_DATA_TYPE_STRUCTURE)
    {
        m_Scaler = e.GetValue().Arr[0].ToInteger();
        m_Unit = e.GetValue().Arr[1].ToInteger();
    }
    else if (e.GetIndex() == 5)
    {
        m_Status = e.GetValue();
    }
    else if (e.GetIndex() == 6)
    {
        DLMSVariant tmp;
        DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_DATETIME, tmp);
        SetCaptureTime(tmp.dateTime);
    }
    else if (e.GetIndex() == 7)
    {
        DLMSVariant tmp;
        DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_DATETIME, tmp);
        SetStartTimeCurrent(tmp.dateTime);
    }
    else if (e.GetIndex() == 8)
    {
        SetPeriod(e.GetValue().ulVal);
    }
    else if (e.GetIndex() == 9)
    {
        SetNumberOfPeriods(e.GetValue().ToInteger());
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_DEMAND_REGISTER

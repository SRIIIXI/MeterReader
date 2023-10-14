#include <assert.h>
#include <string.h>
#include "DLMSVariant.h"
#include "ErrorCodes.h"
#include "Helpers.h"
#include "BitString.h"

int DLMSVariant::Convert(DLMSVariant* item, DLMS_DATA_TYPE type)
{
    if (item->vt == type)
    {
        return DLMS_ERROR_CODE_OK;
    }
    DLMSVariant tmp(item);
    if (type == DLMS_DATA_TYPE_STRING)
    {
        item->Clear();
        if (tmp.vt == DLMS_DATA_TYPE_ARRAY || tmp.vt == DLMS_DATA_TYPE_STRUCTURE)
        {
            bool empty = true;
            std::stringstream sb;
            sb << "{";
            for (std::vector<DLMSVariant>::iterator it = tmp.Arr.begin(); it != tmp.Arr.end(); ++it)
            {
                if (!empty)
                {
                    sb << ", ";
                }
                else
                {
                    empty = false;
                }
                sb << it->ToString();
            }
            sb << "}";
            item->strVal = sb.str();
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (tmp.vt == DLMS_DATA_TYPE_BOOLEAN)
        {
            if (tmp.boolVal == 0)
            {
                item->strVal = "False";
            }
            else
            {
                item->strVal = "True";
            }
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        char buff[250];
        if (tmp.vt == DLMS_DATA_TYPE_INT32)
        {
#if _MSC_VER > 1000
            sprintf_s(buff, 250, "%ld", tmp.lVal);
#else
            sprintf(buff, "%ld", tmp.lVal);
#endif
            item->strVal = buff;
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (tmp.vt == DLMS_DATA_TYPE_UINT32)
        {
#if _MSC_VER > 1000
            sprintf_s(buff, 250, "%lu", tmp.ulVal);
#else
            sprintf(buff, "%lu", tmp.ulVal);
#endif
            item->strVal = buff;
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (tmp.vt == DLMS_DATA_TYPE_INT8)
        {
#if _MSC_VER > 1000
            sprintf_s(buff, 250, "%d", tmp.cVal);
#else
            sprintf(buff, "%d", tmp.cVal);
#endif
            item->strVal = buff;
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (tmp.vt == DLMS_DATA_TYPE_INT16)
        {
#if _MSC_VER > 1000
            sprintf_s(buff, 250, "%d", tmp.iVal);
#else
            sprintf(buff, "%d", tmp.iVal);
#endif
            item->strVal = buff;
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (tmp.vt == DLMS_DATA_TYPE_UINT8)
        {
#if _MSC_VER > 1000
            sprintf_s(buff, 250, "%d", tmp.bVal);
#else
            sprintf(buff, "%d", tmp.bVal);
#endif
            item->strVal = buff;
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (tmp.vt == DLMS_DATA_TYPE_UINT16)
        {
#if _MSC_VER > 1000
            sprintf_s(buff, 250, "%d", tmp.uiVal);
#else
            sprintf(buff, "%d", tmp.uiVal);
#endif
            item->strVal = buff;
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (tmp.vt == DLMS_DATA_TYPE_INT64)
        {
#if defined(_WIN32) || defined(_WIN64)//Windows
#if _MSC_VER > 1000
            sprintf_s(buff, 250, "%lld", tmp.llVal);
#else
            sprintf(buff, "%I64d", tmp.llVal);
#endif

#else
            sprintf(buff, "%lld", tmp.llVal);
#endif
            item->strVal = buff;
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (tmp.vt == DLMS_DATA_TYPE_UINT64)
        {
#if defined(_WIN32) || defined(_WIN64)//Windows
#if _MSC_VER > 1000
            sprintf_s(buff, 250, "%llu", tmp.ullVal);
#else
            sprintf(buff, "%I64u", tmp.llVal);
#endif
#else
            sprintf(buff, "%llu", tmp.ullVal);
#endif
            item->strVal = buff;
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (tmp.vt == DLMS_DATA_TYPE_ENUM)
        {
#if _MSC_VER > 1000
            sprintf_s(buff, 250, "%d", tmp.bVal);
#else
            sprintf(buff, "%d", tmp.bVal);
#endif
            item->strVal = buff;
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (tmp.vt == DLMS_DATA_TYPE_FLOAT32 ||
            tmp.vt == DLMS_DATA_TYPE_FLOAT64)
        {
            int ret = snprintf(buff, 250, tmp.vt == DLMS_DATA_TYPE_FLOAT32 ? "%f" : "%lf", tmp.vt == DLMS_DATA_TYPE_FLOAT32 ? tmp.fltVal : tmp.dblVal);
            //Remove trailing zeroes.
            while (ret > 0 && buff[ret - 1] == '0')
            {
                --ret;
            }
            if (ret > 0)
            {
                if (buff[ret - 1] == ',' || buff[ret - 1] == '.')
                {
                    --ret;
                    buff[ret] = 0;
                }
            }
            item->strVal = buff;
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (tmp.vt == DLMS_DATA_TYPE_BIT_STRING)
        {
            item->strVal = tmp.strVal;
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (tmp.vt == DLMS_DATA_TYPE_DATETIME)
        {
            item->strVal = tmp.dateTime.ToString();
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (tmp.vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            item->strVal = Helpers::BytesToHex(tmp.byteArr, tmp.GetSize());
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
    }
    if (item->vt == DLMS_DATA_TYPE_STRING)
    {
        item->Clear();
        if (type == DLMS_DATA_TYPE_BOOLEAN)
        {
            item->boolVal = tmp.strVal.compare("False") == 0 ? 0 : 1;
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (type == DLMS_DATA_TYPE_INT32)
        {
#if _MSC_VER > 1000
            sscanf_s(tmp.strVal.c_str(), "%ld", &item->lVal);
#else
            sscanf(tmp.strVal.c_str(), "%ld", &item->lVal);
#endif
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (type == DLMS_DATA_TYPE_UINT32)
        {
#if _MSC_VER > 1000
            sscanf_s(tmp.strVal.c_str(), "%lu", &item->ulVal);
#else
            sscanf(tmp.strVal.c_str(), "%lu", &item->ulVal);
#endif
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (type == DLMS_DATA_TYPE_INT8)
        {
#if _MSC_VER > 1000
            sscanf_s(tmp.strVal.c_str(), "%c", &item->cVal, 1);
#else
            sscanf(tmp.strVal.c_str(), "%c", &item->cVal);
#endif
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (type == DLMS_DATA_TYPE_INT16)
        {
#if _MSC_VER > 1000
            sscanf_s(tmp.strVal.c_str(), "%hd", &item->iVal);
#else
            sscanf(tmp.strVal.c_str(), "%hd", &item->iVal);
#endif
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (type == DLMS_DATA_TYPE_UINT8)
        {
#if _MSC_VER > 1000
            sscanf_s(tmp.strVal.c_str(), "%hhu", &item->bVal);
#else
            int value;
            sscanf(tmp.strVal.c_str(), "%d", &value);
            item->bVal = value & 0xFF;
#endif
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (type == DLMS_DATA_TYPE_UINT16)
        {
#if _MSC_VER > 1000
            sscanf_s(tmp.strVal.c_str(), "%hu", &item->uiVal);
#else
            int value;
            sscanf(tmp.strVal.c_str(), "%d", &value);
            item->uiVal = value & 0xFFFF;
#endif
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (type == DLMS_DATA_TYPE_INT64)
        {
#if defined(_WIN32) || defined(_WIN64)//Windows
#if _MSC_VER > 1000
            sscanf_s(tmp.strVal.c_str(), "%lld", &item->llVal);
#else
            sscanf(tmp.strVal.c_str(), "%I64d", &item->llVal);
#endif
#else
            sscanf(tmp.strVal.c_str(), "%lld", &item->llVal);
#endif
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (type == DLMS_DATA_TYPE_UINT64)
        {
#if defined(_WIN32) || defined(_WIN64)//Windows
#if _MSC_VER > 1000
            sscanf_s(tmp.strVal.c_str(), "%llu", &item->ullVal);
#else
            sscanf(tmp.strVal.c_str(), "%I64u", &item->ullVal);
#endif
#else
            sscanf(tmp.strVal.c_str(), "%llu", &item->ullVal);
#endif

#if _MSC_VER > 1000
#else
#endif
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (type == DLMS_DATA_TYPE_ENUM)
        {
#if _MSC_VER > 1000
            sscanf_s(tmp.strVal.c_str(), "%hhu", &item->bVal);
#else
            int value;
            sscanf(tmp.strVal.c_str(), "%d", &value);
            item->bVal = value & 0xFF;
#endif
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (type == DLMS_DATA_TYPE_FLOAT32)
        {
#if _MSC_VER > 1000
            sscanf_s(tmp.strVal.c_str(), "%f", &item->fltVal);
#else
            sscanf(tmp.strVal.c_str(), "%f", &item->fltVal);
#endif
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (type == DLMS_DATA_TYPE_FLOAT64)
        {
#if _MSC_VER > 1000
            sscanf_s(tmp.strVal.c_str(), "%lf", &item->dblVal);
#else
            sscanf(tmp.strVal.c_str(), "%lf", &item->dblVal);
#endif
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        if (type == DLMS_DATA_TYPE_OCTET_STRING)
        {
            ByteBuffer tmp2;
            Helpers::HexToBytes(tmp.strVal, tmp2);
            item->Add(tmp2.GetData(), tmp2.GetSize());
            item->vt = type;
            return DLMS_ERROR_CODE_OK;
        }
        return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
    }
    int fromSize = tmp.GetSize();
    int toSize = GetSize(type);
    //If we try to change bigger value to smaller check that value is not too big.
    //Example Int16 to Int8.
    if (fromSize > toSize&& tmp.vt != DLMS_DATA_TYPE_FLOAT32 && tmp.vt != DLMS_DATA_TYPE_FLOAT64)
    {
        unsigned char* pValue = &tmp.bVal;
        for (int pos = toSize; pos != fromSize; ++pos)
        {
            if (pValue[pos] != 0)
            {
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
        }
    }
    item->Clear();
    if (tmp.vt == DLMS_DATA_TYPE_FLOAT32)
    {
        long long value = (long long)tmp.fltVal;
        memcpy(&item->bVal, &value, toSize);
    }
    else if (tmp.vt == DLMS_DATA_TYPE_FLOAT64)
    {
        long long value = (long long)tmp.dblVal;
        memcpy(&item->bVal, &value, toSize);
    }
    else
    {
        if (fromSize > toSize)
        {
            memcpy(&item->bVal, &tmp.bVal, toSize);
        }
        else
        {
            memset(&item->bVal, 0, toSize);
            memcpy(&item->bVal, &tmp.bVal, fromSize);
        }
    }
    item->vt = type;
    return DLMS_ERROR_CODE_OK;
}

void DLMSVariant::Clear()
{
    Arr.clear();
    strVal.clear();
    if (vt == DLMS_DATA_TYPE_OCTET_STRING && byteArr != NULL)
    {
        if (size == 0)
        {
            assert(size != 0);
        }
        free(byteArr);
        byteArr = NULL;
        size = 0;
    }
    vt = DLMS_DATA_TYPE_NONE;
    this->byteArr = NULL;
    size = 0;
}

DLMSVariant::DLMSVariant()
{
    vt = DLMS_DATA_TYPE_NONE;
    size = 0;
    byteArr = NULL;
    Clear();
}

DLMSVariant::~DLMSVariant()
{
    Clear();
}
DLMSVariant::DLMSVariant(DLMSVariant* value)
{
    byteArr = NULL;
    vt = value->vt;
    if (vt == DLMS_DATA_TYPE_NONE)
    {
        return;
    }
    else if (vt == DLMS_DATA_TYPE_STRING ||
        vt == DLMS_DATA_TYPE_BIT_STRING)
    {
        this->strVal.append(value->strVal);
    }
    else if (vt == DLMS_DATA_TYPE_DATETIME ||
        vt == DLMS_DATA_TYPE_DATE ||
        vt == DLMS_DATA_TYPE_TIME)
    {
        this->dateTime = value->dateTime;
    }
    else if (vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        size = value->size;
        if (size != 0)
        {
            byteArr = (unsigned char*)malloc(size);
            memcpy(byteArr, value->byteArr, size);
        }
    }
    else if (vt == DLMS_DATA_TYPE_ARRAY || vt == DLMS_DATA_TYPE_STRUCTURE)
    {
        for (std::vector<DLMSVariant>::iterator it = value->Arr.begin(); it != value->Arr.end(); ++it)
        {
            Arr.push_back(*it);
        }
    }
    else
    {
        int size = value->GetSize();
        if (size > 0)
        {
            memcpy(&bVal, &value->bVal, size);
        }
        else
        {
            assert(0);
        }
    }
}

DLMSVariant::DLMSVariant(float value)
{
    vt = DLMS_DATA_TYPE_FLOAT32;
    fltVal = value;
}

DLMSVariant::DLMSVariant(double value)
{
    vt = DLMS_DATA_TYPE_FLOAT64;
    dblVal = value;
}


DLMSVariant::DLMSVariant(unsigned long long value)
{
    vt = DLMS_DATA_TYPE_UINT64;
    ullVal = value;
}

DLMSVariant::DLMSVariant(long long value)
{
    vt = DLMS_DATA_TYPE_INT64;
    llVal = value;
}

DLMSVariant::DLMSVariant(bool value)
{
    vt = DLMS_DATA_TYPE_BOOLEAN;
    boolVal = value;
}


DLMSVariant::DLMSVariant(char value)
{
    vt = DLMS_DATA_TYPE_INT8;
    cVal = value;
}

DLMSVariant::DLMSVariant(short value)
{
    vt = DLMS_DATA_TYPE_INT16;
    iVal = value;
}


DLMSVariant::DLMSVariant(int value)
{
    vt = DLMS_DATA_TYPE_INT32;
    lVal = value;
}

DLMSVariant::DLMSVariant(long value)
{
    vt = DLMS_DATA_TYPE_INT32;
    lVal = value;
}

DLMSVariant::DLMSVariant(struct tm value)
{
    vt = DLMS_DATA_TYPE_DATETIME;
    dateTime.SetValue(value);
}

DLMSVariant::DLMSVariant(ByteBuffer& value)
{
    vt = DLMS_DATA_TYPE_OCTET_STRING;
    size = (unsigned short)value.GetSize();
    if (size != 0)
    {
        byteArr = (unsigned char*)malloc(size);
        memcpy(byteArr, value.GetData(), size);
    }
    else
    {
        byteArr = NULL;
    }
}

DLMSVariant::DLMSVariant(Date& value)
{
    vt = DLMS_DATA_TYPE_DATE;
    dateTime = value;
}

DLMSVariant::DLMSVariant(Time& value)
{
    vt = DLMS_DATA_TYPE_TIME;
    dateTime = value;
}

DLMSVariant::DLMSVariant(DateTime& value)
{
    vt = DLMS_DATA_TYPE_DATETIME;
    dateTime = value;
}

DLMSVariant::DLMSVariant(unsigned char value)
{
    vt = DLMS_DATA_TYPE_UINT8;
    bVal = value;
}

DLMSVariant::DLMSVariant(unsigned short value)
{
    vt = DLMS_DATA_TYPE_UINT16;
    uiVal = value;
}

DLMSVariant::DLMSVariant(unsigned int value)
{
    vt = DLMS_DATA_TYPE_UINT32;
    ulVal = value;
}

DLMSVariant::DLMSVariant(unsigned long value)
{
    vt = DLMS_DATA_TYPE_UINT32;
    ulVal = value;
}

DLMSVariant::DLMSVariant(std::string value)
{
    vt = DLMS_DATA_TYPE_STRING;
    strVal = value;
}

DLMSVariant::DLMSVariant(unsigned char* value, int count)
{
    vt = DLMS_DATA_TYPE_OCTET_STRING;
    size = count;
    if (size != 0)
    {
        byteArr = (unsigned char*)malloc(size);
        memcpy(byteArr, value, size);
    }
    else
    {
        byteArr = NULL;
    }
}

DLMSVariant::DLMSVariant(const DLMSVariant& value)
{
    //If Octect String, copy byte buffer.
    if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        vt = value.vt;
        size = value.size;
        if (size != 0)
        {
            byteArr = (unsigned char*)malloc(size);
            memcpy(byteArr, value.byteArr, size);
        }
        else
        {
            byteArr = NULL;
        }
    }
    else
    {
        *((dlmsVARIANT*)this) = value;
    }
}

DLMSVariant& DLMSVariant::operator=(const DLMSVariant& value)
{
    Clear();
    //If Octect String, copy byte buffer.
    if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        vt = value.vt;
        size = value.size;
        if (size != 0)
        {
            byteArr = (unsigned char*)malloc(size);
            memcpy(byteArr, value.byteArr, size);
        }
    }
    else
    {
        *((dlmsVARIANT*)this) = value;
    }
    return *this;
}

DLMSVariant& DLMSVariant::operator=(ByteBuffer& value)
{
    Clear();
    vt = DLMS_DATA_TYPE_OCTET_STRING;
    size = (unsigned short)value.GetSize();
    if (size != 0)
    {
        byteArr = (unsigned char*)malloc(size);
        memcpy(byteArr, value.GetData(), size);
    }
    return *this;
}

DLMSVariant::DLMSVariant(const char* value)
{
    vt = DLMS_DATA_TYPE_STRING;
    strVal.append(value);
}

DLMSVariant::DLMSVariant(unsigned char* pValue, int count, DLMS_DATA_TYPE type)
{
    assert(count > -1);
    byteArr = NULL;
    size = count;
    if (size != 0)
    {
        byteArr = (unsigned char*)malloc(count);
        memcpy(byteArr, pValue, count);
    }
    vt = type;
}

DLMSVariant& DLMSVariant::operator=(std::string value)
{
    Clear();
    vt = DLMS_DATA_TYPE_STRING;
    strVal.append(value);
    return *this;
}

DLMSVariant& DLMSVariant::operator=(const char* value)
{
    Clear();
    vt = DLMS_DATA_TYPE_STRING;
    strVal.append(value);
    return *this;
}

DLMSVariant& DLMSVariant::operator=(float value)
{
    Clear();
    vt = DLMS_DATA_TYPE_FLOAT32;
    fltVal = value;
    return *this;
}

DLMSVariant& DLMSVariant::operator=(double value)
{
    Clear();
    vt = DLMS_DATA_TYPE_FLOAT64;
    dblVal = value;
    return *this;
}

DLMSVariant& DLMSVariant::operator=(unsigned long long value)
{
    Clear();
    vt = DLMS_DATA_TYPE_UINT64;
    ullVal = value;
    return *this;
}

DLMSVariant& DLMSVariant::operator=(long long value)
{
    Clear();
    vt = DLMS_DATA_TYPE_INT64;
    llVal = value;
    return *this;
}

DLMSVariant& DLMSVariant::operator=(const bool value)
{
    Clear();
    vt = DLMS_DATA_TYPE_BOOLEAN;
    boolVal = value;
    return *this;
}


DLMSVariant& DLMSVariant::operator=(char value)
{
    Clear();
    vt = DLMS_DATA_TYPE_INT8;
    cVal = value;
    return *this;
}

DLMSVariant& DLMSVariant::operator=(short value)
{
    Clear();
    vt = DLMS_DATA_TYPE_INT16;
    iVal = value;
    return *this;
}

DLMSVariant& DLMSVariant::operator=(int value)
{
    Clear();
    vt = DLMS_DATA_TYPE_INT32;
    lVal = value;
    return *this;
}

DLMSVariant& DLMSVariant::operator=(long value)
{
    Clear();
    vt = DLMS_DATA_TYPE_INT32;
    lVal = value;
    return *this;
}


DLMSVariant& DLMSVariant::operator=(unsigned char value)
{
    Clear();
    vt = DLMS_DATA_TYPE_UINT8;
    bVal = value;
    return *this;
}

DLMSVariant& DLMSVariant::operator=(unsigned short value)
{
    Clear();
    vt = DLMS_DATA_TYPE_UINT16;
    uiVal = value;
    return *this;
}

DLMSVariant& DLMSVariant::operator=(unsigned long value)
{
    Clear();
    vt = DLMS_DATA_TYPE_UINT32;
    ulVal = value;
    return *this;
}

DLMSVariant& DLMSVariant::operator=(struct tm value)
{
    Clear();
    vt = DLMS_DATA_TYPE_DATETIME;
    dateTime.SetValue(value);
    return *this;
}

DLMSVariant& DLMSVariant::operator=(Date& value)
{
    Clear();
    vt = DLMS_DATA_TYPE_DATE;
    dateTime = value;
    return *this;
}

DLMSVariant& DLMSVariant::operator=(Time& value)
{
    Clear();
    vt = DLMS_DATA_TYPE_TIME;
    dateTime = value;
    return *this;
}

DLMSVariant& DLMSVariant::operator=(DateTime& value)
{
    Clear();
    vt = DLMS_DATA_TYPE_DATETIME;
    dateTime = value;
    return *this;
}

DLMSVariant& DLMSVariant::operator=(unsigned int value)
{
    Clear();
    vt = DLMS_DATA_TYPE_UINT32;
    ulVal = value;
    return *this;
}

void DLMSVariant::Add(const unsigned char* pValue, int count)
{
    vt = DLMS_DATA_TYPE_OCTET_STRING;
    if (count != 0)
    {
        byteArr = (unsigned char*)realloc(byteArr, size + count);
        memcpy(byteArr + size, pValue, count);
        size += count;
    }
}

void DLMSVariant::Add(const char* pValue, int count)
{
    vt = DLMS_DATA_TYPE_OCTET_STRING;
    if (count != 0)
    {
        byteArr = (unsigned char*)realloc(byteArr, size + count);
        memcpy(byteArr + size, pValue, count);
        size += count;
    }
}

void DLMSVariant::Add(std::string value)
{
    vt = DLMS_DATA_TYPE_OCTET_STRING;
    if (value.size() != 0)
    {
        byteArr = (unsigned char*)realloc(byteArr, size + value.size());
        memcpy(byteArr + size, value.c_str(), value.size());
        size += (unsigned short)value.size();
    }
}

bool DLMSVariant::Equals(DLMSVariant& item)
{
    if (vt != item.vt)
    {
        return false;
    }
    int size = GetSize();
    if (size == -1 || size != item.GetSize())
    {
        return false;
    }
    if (size != 0)
    {
        return memcmp(&this->bVal, &item.bVal, size) == 0;
    }
    return true;
}

int DLMSVariant::ChangeType(DLMS_DATA_TYPE newType)
{
    if (newType == vt)
    {
        return DLMS_ERROR_CODE_OK;
    }

    if (newType == DLMS_DATA_TYPE_NONE)
    {
        Clear();
        return DLMS_ERROR_CODE_OK;
    }
    if (vt == DLMS_DATA_TYPE_ARRAY && newType == DLMS_DATA_TYPE_OCTET_STRING)
    {
        return DLMS_ERROR_CODE_OK;
    }
    if (vt == DLMS_DATA_TYPE_STRING)
    {
        return Convert(this, newType);
    }
    switch (newType)
    {
    case DLMS_DATA_TYPE_STRING:
    case DLMS_DATA_TYPE_BOOLEAN:
    case DLMS_DATA_TYPE_INT32:
    case DLMS_DATA_TYPE_UINT32:
    case DLMS_DATA_TYPE_INT8:
    case DLMS_DATA_TYPE_INT16:
    case DLMS_DATA_TYPE_UINT8:
    case DLMS_DATA_TYPE_UINT16:
    case DLMS_DATA_TYPE_INT64:
    case DLMS_DATA_TYPE_UINT64:
    case DLMS_DATA_TYPE_ENUM:
    case DLMS_DATA_TYPE_FLOAT32:
    case DLMS_DATA_TYPE_FLOAT64:
        return Convert(this, newType);
        break;
    default:
        //Handled later.
        break;
    }
    DLMSVariant tmp(this);
    switch (vt)
    {
    case DLMS_DATA_TYPE_BOOLEAN:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    case DLMS_DATA_TYPE_BIT_STRING:
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    case DLMS_DATA_TYPE_INT32:
        switch (newType)
        {
        case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_UINT32:
        switch (newType)
        {
        case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_OCTET_STRING:
        switch (newType)
        {
        case DLMS_DATA_TYPE_DATETIME:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        case DLMS_DATA_TYPE_DATE:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        case DLMS_DATA_TYPE_TIME:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
        switch (newType)
        {
        case DLMS_DATA_TYPE_INT32:
            break;
        case DLMS_DATA_TYPE_UINT32:
            break;
        case DLMS_DATA_TYPE_STRING:
            break;
        case DLMS_DATA_TYPE_INT8:
            break;
        case DLMS_DATA_TYPE_INT16:
            break;
        case DLMS_DATA_TYPE_UINT8:
            break;
        case DLMS_DATA_TYPE_UINT16:
            break;
        case DLMS_DATA_TYPE_INT64:
            break;
        case DLMS_DATA_TYPE_UINT64:
            break;
        case DLMS_DATA_TYPE_ENUM:
            break;
        case DLMS_DATA_TYPE_FLOAT32:
            break;
        case DLMS_DATA_TYPE_FLOAT64:
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_INT8:
        switch (newType)
        {
        case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_INT16:
        switch (newType)
        {
        case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_UINT8:
        switch (newType)
        {
        case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_UINT16:
        switch (newType)
        {
        case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_INT64:
        switch (newType)
        {
        case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_UINT64:
        switch (newType)
        {
        case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_ENUM:
        switch (newType)
        {
        case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_FLOAT32:
        switch (newType)
        {
        case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_FLOAT64:
        switch (newType)
        {
        case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
            return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_DATETIME:
        switch (newType)
        {
        case DLMS_DATA_TYPE_OCTET_STRING:
            break;
        case DLMS_DATA_TYPE_STRING:
            break;
        case DLMS_DATA_TYPE_DATE:
            break;
        case DLMS_DATA_TYPE_TIME:
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_DATE:
        switch (newType)
        {
        case DLMS_DATA_TYPE_OCTET_STRING:
            break;
        case DLMS_DATA_TYPE_STRING:
            break;
        case DLMS_DATA_TYPE_DATETIME:
            break;
        case DLMS_DATA_TYPE_DATE:
            break;
        case DLMS_DATA_TYPE_TIME:
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_TIME:
        switch (newType)
        {
        case DLMS_DATA_TYPE_OCTET_STRING:
            break;
        case DLMS_DATA_TYPE_STRING:
            break;
        case DLMS_DATA_TYPE_DATETIME:
            break;
        case DLMS_DATA_TYPE_DATE:
            break;
        case DLMS_DATA_TYPE_TIME:
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        break;
    case DLMS_DATA_TYPE_ARRAY:
    case DLMS_DATA_TYPE_STRUCTURE:
    case DLMS_DATA_TYPE_COMPACT_ARRAY:
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

//Get size in bytes.
int DLMSVariant::GetSize()
{
    if (this->vt == DLMS_DATA_TYPE_STRING ||
        this->vt == DLMS_DATA_TYPE_BIT_STRING)
    {
        return (int)strVal.size();
    }
    if (this->vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        return size;
    }
    return GetSize(this->vt);
}

//Get size in bytes.
int DLMSVariant::GetSize(DLMS_DATA_TYPE vt)
{
    int nSize = -1;
    switch (vt)
    {
    case DLMS_DATA_TYPE_NONE:
        nSize = 0;
        break;
    case DLMS_DATA_TYPE_BOOLEAN:
    case DLMS_DATA_TYPE_INT8:
    case DLMS_DATA_TYPE_UINT8:
    case DLMS_DATA_TYPE_ENUM:
        nSize = 1;
        break;
    case DLMS_DATA_TYPE_INT16:
    case DLMS_DATA_TYPE_UINT16:
        nSize = 2;
        break;
    case DLMS_DATA_TYPE_INT32:
    case DLMS_DATA_TYPE_UINT32:
    case DLMS_DATA_TYPE_FLOAT32:
        nSize = 4;
        break;
    case DLMS_DATA_TYPE_INT64:
    case DLMS_DATA_TYPE_UINT64:
    case DLMS_DATA_TYPE_FLOAT64:
        nSize = 8;
        break;
    case DLMS_DATA_TYPE_BIT_STRING:
        break;
    case DLMS_DATA_TYPE_OCTET_STRING:
        break;
    case DLMS_DATA_TYPE_STRING:
    case DLMS_DATA_TYPE_STRING_UTF8:
        nSize = -1;
        break;
    case DLMS_DATA_TYPE_BINARY_CODED_DESIMAL:
        break;
    case DLMS_DATA_TYPE_DATETIME:
        nSize = 12;
        break;
    case DLMS_DATA_TYPE_DATE:
        break;
    case DLMS_DATA_TYPE_TIME:
        break;
    case DLMS_DATA_TYPE_ARRAY:
        break;
    case DLMS_DATA_TYPE_STRUCTURE:
        break;
    case DLMS_DATA_TYPE_COMPACT_ARRAY:
        break;
    }
    return nSize;
}

std::string DLMSVariant::ToString()
{
    DLMSVariant tmp(this);
    tmp.ChangeType(DLMS_DATA_TYPE_STRING);
    if (tmp.strVal.length() == 0)
    {
        return "";
    }
    return tmp.strVal;
}

int DLMSVariant::ToInteger()
{
    if (vt == DLMS_DATA_TYPE_NONE)
    {
        return 0;
    }

    if (vt == DLMS_DATA_TYPE_BOOLEAN)
    {
        return boolVal ? 1 : 0;
    }
    if (vt == DLMS_DATA_TYPE_INT32)
    {
        return lVal;
    }
    if (vt == DLMS_DATA_TYPE_UINT32)
    {
        return ulVal;
    }
    if (vt == DLMS_DATA_TYPE_BINARY_CODED_DESIMAL)
    {
        assert(0);
    }
    if (vt == DLMS_DATA_TYPE_STRING_UTF8)
    {
        //TODO:
        assert(0);
    }
    if (vt == DLMS_DATA_TYPE_INT8)
    {
        return cVal;
    }

    if (vt == DLMS_DATA_TYPE_INT16)
    {
        return iVal;
    }
    if (vt == DLMS_DATA_TYPE_UINT8)
    {
        return bVal;
    }
    if (vt == DLMS_DATA_TYPE_UINT16)
    {
        return uiVal;
    }
    if (vt == DLMS_DATA_TYPE_INT64)
    {
        //TODO:
        assert(0);
    }
    if (vt == DLMS_DATA_TYPE_UINT64)
    {
        //TODO:
        assert(0);
    }
    if (vt == DLMS_DATA_TYPE_ENUM)
    {
        return bVal;
    }
    if (vt == DLMS_DATA_TYPE_FLOAT32)
    {
        return (int)fltVal;
    }
    if (vt == DLMS_DATA_TYPE_FLOAT64)
    {
        return (int)dblVal;
    }
    if (vt == DLMS_DATA_TYPE_STRING)
    {
        int val = 0;
#if _MSC_VER > 1000
        if (sscanf_s(strVal.c_str(), "%d", &val) == -1)
#else
        if (sscanf(strVal.c_str(), "%d", &val) == -1)
#endif
        {
            assert(0);
        }
        return val;
    }
    if (vt == DLMS_DATA_TYPE_BIT_STRING)
    {
        int val;
        BitString bs(strVal);
        bs.ToInteger(val);
        return val;
    }
    assert(0);
    return 0;
}

double DLMSVariant::ToDouble()
{
    if (vt == DLMS_DATA_TYPE_NONE)
    {
        return 0;
    }

    if (vt == DLMS_DATA_TYPE_BOOLEAN)
    {
        return boolVal ? 1 : 0;
    }
    if (vt == DLMS_DATA_TYPE_INT32)
    {
        return lVal;
    }
    if (vt == DLMS_DATA_TYPE_UINT32)
    {
        return ulVal;
    }
    if (vt == DLMS_DATA_TYPE_BINARY_CODED_DESIMAL)
    {
        assert(0);
    }
    if (vt == DLMS_DATA_TYPE_STRING_UTF8)
    {
        assert(0);
    }
    if (vt == DLMS_DATA_TYPE_INT8)
    {
        return cVal;
    }

    if (vt == DLMS_DATA_TYPE_INT16)
    {
        return iVal;
    }
    if (vt == DLMS_DATA_TYPE_UINT8)
    {
        return bVal;
    }
    if (vt == DLMS_DATA_TYPE_UINT16)
    {
        return uiVal;
    }
    if (vt == DLMS_DATA_TYPE_INT64)
    {
        return (double)llVal;
    }
    if (vt == DLMS_DATA_TYPE_UINT64)
    {
        return (double)ullVal;
    }
    if (vt == DLMS_DATA_TYPE_ENUM)
    {
        return bVal;
    }
    if (vt == DLMS_DATA_TYPE_FLOAT32)
    {
        return fltVal;
    }
    if (vt == DLMS_DATA_TYPE_FLOAT64)
    {
        return dblVal;
    }
    assert(0);
    return 0;
}

/**
* Add new object to the byte buffer.
*
* @param value
*            Value to add.
*/
int DLMSVariant::GetBytes(ByteBuffer& value)
{
    if (vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        value.Set(byteArr, size);
    }
    else if (vt == DLMS_DATA_TYPE_UINT8)
    {
        value.SetUInt8(cVal);
    }
    else if (vt == DLMS_DATA_TYPE_UINT16)
    {
        value.SetUInt16(uiVal);
    }
    else if (vt == DLMS_DATA_TYPE_UINT32)
    {
        value.SetUInt32(ulVal);
    }
    else if (vt == DLMS_DATA_TYPE_UINT64)
    {
        value.SetUInt64(ullVal);
    }
    else if (vt == DLMS_DATA_TYPE_INT8)
    {
        value.SetInt8(bVal);
    }
    else if (vt == DLMS_DATA_TYPE_INT16)
    {
        value.SetInt16(iVal);
    }
    else if (vt == DLMS_DATA_TYPE_INT32)
    {
        value.SetInt32(lVal);
    }
    else if (vt == DLMS_DATA_TYPE_INT64)
    {
        value.SetInt64(llVal);
    }
    else if (vt == DLMS_DATA_TYPE_STRING)
    {
        value.AddString(strVal.c_str());
    }
    else if (vt == DLMS_DATA_TYPE_FLOAT32)
    {
        value.SetFloat(fltVal);
    }
    else if (vt == DLMS_DATA_TYPE_FLOAT64)
    {
        value.SetDouble(dblVal);
    }
    else
    {
        //Invalid object type.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return 0;
}

bool DLMSVariant::IsNumber()
{
    switch (vt)
    {
    case DLMS_DATA_TYPE_UINT8:
    case DLMS_DATA_TYPE_UINT16:
    case DLMS_DATA_TYPE_UINT32:
    case DLMS_DATA_TYPE_UINT64:
    case DLMS_DATA_TYPE_INT8:
    case DLMS_DATA_TYPE_INT16:
    case DLMS_DATA_TYPE_INT32:
    case DLMS_DATA_TYPE_INT64:
    case DLMS_DATA_TYPE_FLOAT32:
    case DLMS_DATA_TYPE_FLOAT64:
        return true;
    default:
        break;
    }
    return false;
}

#ifndef DLMS_VARIANT_H
#define DLMS_VARIANT_H

#include <vector>
#include "Date.h"
#include "DLMSTime.h"
#include "DateTime.h"
#include "ByteBuffer.h"

#define __tagVARIANT
#define __VARIANT_NAME_1
#define __VARIANT_NAME_2
#define __VARIANT_NAME_3

class DLMSVariant;

struct dlmsVARIANT
{
    union
    {
        struct __tagVARIANT
        {
            DLMS_DATA_TYPE vt;
            union
            {
                unsigned char bVal;
                char cVal;
                short iVal;
                long lVal;
                long long llVal;
                float fltVal;
                double dblVal;
                bool boolVal;
                unsigned short uiVal;
                unsigned long ulVal;
                unsigned long long ullVal;
                unsigned char *byteArr;
            } 	__VARIANT_NAME_3;
        } 	__VARIANT_NAME_2;
    } 	__VARIANT_NAME_1;
    DateTime dateTime;
    //Size of byte array.
    unsigned short size;
    std::string strVal;
    std::vector<DLMSVariant> Arr;
};

class DLMSVariant : public dlmsVARIANT
{
    static int Convert(DLMSVariant* item, DLMS_DATA_TYPE type);
public:
    void Clear();
    DLMSVariant();

    //Copy constructor.
    DLMSVariant(const DLMSVariant& value);

    DLMSVariant(float value);
    DLMSVariant(double value);

    DLMSVariant(unsigned long long value);
    DLMSVariant(long long value);
    DLMSVariant(bool value);
    DLMSVariant(char value);
    DLMSVariant(short value);
    DLMSVariant(int value);
    DLMSVariant(long value);
    DLMSVariant(struct tm value);
    DLMSVariant(unsigned char* value, int count);
    DLMSVariant(Date& value);
    DLMSVariant(Time& value);
    DLMSVariant(DateTime& value);
    DLMSVariant(DLMSVariant* value);
    DLMSVariant(unsigned char* pValue, int count, DLMS_DATA_TYPE type);
    DLMSVariant(unsigned char value);
    DLMSVariant(unsigned short value);
    DLMSVariant(unsigned long value);
    DLMSVariant(std::string value);
    DLMSVariant(ByteBuffer& value);
    DLMSVariant(const char* value);
    DLMSVariant(unsigned int value);
    //Destructor.
    ~DLMSVariant();


    DLMSVariant& operator=(const DLMSVariant& value);

    DLMSVariant& operator=(std::string value);
    DLMSVariant& operator=(const char* value);
    DLMSVariant& operator=(ByteBuffer& value);
    DLMSVariant& operator=(float value);
    DLMSVariant& operator=(double value);
    DLMSVariant& operator=(unsigned long long value);
    DLMSVariant& operator=(long long value);
    DLMSVariant& operator=(const bool value);
    DLMSVariant& operator=(char value);
    DLMSVariant& operator=(short value);
    DLMSVariant& operator=(int value);
    DLMSVariant& operator=(long value);
    DLMSVariant& operator=(unsigned char value);
    DLMSVariant& operator=(unsigned short value);
    DLMSVariant& operator=(unsigned int value);
    DLMSVariant& operator=(unsigned long value);
    DLMSVariant& operator=(struct tm value);
    DLMSVariant& operator=(Date& value);
    DLMSVariant& operator=(Time& value);
    DLMSVariant& operator=(DateTime& value);
    void Add(const unsigned char*, int count);
    void Add(const char*, int count);
    void Add(std::string value);
    bool Equals(DLMSVariant& item);
    int ChangeType(DLMS_DATA_TYPE newType);
    //Get size in bytes.
    int GetSize();
    //Get size in bytes.
    static int GetSize(DLMS_DATA_TYPE vt);
    std::string ToString();
    int ToInteger();
    double ToDouble();
    int GetBytes(ByteBuffer& value);

    //Returns true if value is number.
    bool IsNumber();
};
#endif

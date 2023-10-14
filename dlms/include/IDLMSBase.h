#ifndef IDLMSBASE_H
#define IDLMSBASE_H

#include "DLMSVariant.h"
#include "DLMSValueEventArg.h"

class DLMSSettings;

struct IDLMSBase
{
public:
    // Returns amount of attributes.
    virtual int GetAttributeCount() = 0;

    // Returns amount of methods.
    virtual int GetMethodCount() = 0;

    // Returns value of given attribute.
    virtual int GetValue(DLMSSettings& settings, DLMSValueEventArg& e) = 0;

    // Set value of given attribute.
    virtual int SetValue(DLMSSettings& settings, DLMSValueEventArg& e) = 0;

    // Invokes method.
    virtual int Invoke(DLMSSettings& settings, DLMSValueEventArg& e) = 0;

//	virtual DLMS_DATA_TYPE GetUIDataType(int index) = 0;
};
#endif

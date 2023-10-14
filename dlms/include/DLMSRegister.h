#ifndef DLMS_REGISTER_H
#define DLMS_REGISTER_H

#include "Ignore.h"
#include "DLMSObject.h"
#ifndef DLMS_IGNORE_REGISTER

class DLMSRegister : public DLMSObject
{
protected:
    DLMSVariant m_Value;
    signed char m_Scaler;
    unsigned char m_Unit;

    //SN Constructor.
    DLMSRegister(DLMS_OBJECT_TYPE type, std::string ln, unsigned short sn);

    bool IsRead(int index);

public:
    //Constructor.
    DLMSRegister(void);

    //SN Constructor.
    DLMSRegister(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSRegister(std::string ln);

    /// <summary>
    /// Get value of COSEM Data object.
    /// </summary>
    DLMSVariant& GetValue();

    /// <summary>
    /// Set value of COSEM Data object.
    /// </summary>
    void SetValue(DLMSVariant& value);

    // Scaler of COSEM Register object.
    double GetScaler();

    virtual void SetScaler(double value);

    // Unit of COSEM Register object.
    int GetUnit();

    void SetUnit(unsigned char value);

    /*
     * Reset value.
     */
    void Reset();

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    /////////////////////////////////////////////////////////////////////////
    //Reset the register value.
    int Reset(
        DLMSClient* client,
        std::vector<ByteBuffer>& reply);

    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);

    //Get attribute values of object.
    void GetValues(std::vector<std::string>& values);

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);
};
#endif
#endif

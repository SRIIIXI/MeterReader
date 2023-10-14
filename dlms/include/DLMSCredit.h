#ifndef CREDIT_H
#define CREDIT_H


#ifndef DLMS_IGNORE_CREDIT
#include "DLMSObject.h"

class DLMSCredit : public DLMSObject
{
    long m_CurrentCreditAmount;
    DLMS_CREDIT_TYPE m_Type;
    unsigned char m_Priority;
    long m_WarningThreshold;
    long m_Limit;
    DLMS_CREDIT_CONFIGURATION m_CreditConfiguration;
    DLMS_CREDIT_STATUS m_Status;
    long m_PresetCreditAmount;
    long m_CreditAvailableThreshold;
    DateTime m_Period;
public:
    //Constructor.
    DLMSCredit();

    //SN Constructor.
    DLMSCredit(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSCredit(std::string ln);

    /**
    * @return Current credit amount.
    */
    long GetCurrentCreditAmount()
    {
        return m_CurrentCreditAmount;
    }

    /**
     * @param value
     *            Current credit amount.
     */
    void SetCurrentCreditAmount(long value)
    {
        m_CurrentCreditAmount = value;
    }

    /**
     * @return Type.
     */
    DLMS_CREDIT_TYPE GetType()
    {
        return m_Type;
    }

    /**
     * @param value
     *            Type.
     */
    void SetType(DLMS_CREDIT_TYPE value)
    {
        m_Type = value;
    }

    /**
     * @return Priority.
     */
    unsigned char GetPriority()
    {
        return m_Priority;
    }

    /**
     * @param value
     *            Priority.
     */
    void SetPriority(unsigned char value)
    {
        m_Priority = value;
    }

    /**
     * @return Warning threshold.
     */
    long GetWarningThreshold()
    {
        return m_WarningThreshold;
    }

    /**
     * @param value
     *            Warning threshold.
     */
    void SetWarningThreshold(long value)
    {
        m_WarningThreshold = value;
    }

    /**
     * @return Limit.
     */
    long GetLimit()
    {
        return m_Limit;
    }

    /**
     * @param value
     *            Limit.
     */
    void SetLimit(long value)
    {
        m_Limit = value;
    }

    /**
     * @return Credit configuration.
     */
    DLMS_CREDIT_CONFIGURATION GetCreditConfiguration()
    {
        return m_CreditConfiguration;
    }

    /**
     * @param value
     *            Credit configuration.
     */
    void SetCreditConfiguration(DLMS_CREDIT_CONFIGURATION value)
    {
        m_CreditConfiguration = value;
    }

    /**
     * @return Status.
     */
    DLMS_CREDIT_STATUS GetStatus()
    {
        return m_Status;
    }

    /**
     * @param value
     *            Status.
     */
    void SetStatus(DLMS_CREDIT_STATUS value)
    {
        m_Status = value;
    }

    /**
     * @return Preset credit amount.
     */
    long GetPresetCreditAmount()
    {
        return m_PresetCreditAmount;
    }

    /**
     * @param value
     *            Preset credit amount.
     */
    void SetPresetCreditAmount(long value)
    {
        m_PresetCreditAmount = value;
    }

    /**
     * @return Credit available threshold.
     */
    long GetCreditAvailableThreshold()
    {
        return m_CreditAvailableThreshold;
    }

    /**
     * @param value
     *            Credit available threshold.
     */
    void SetCreditAvailableThreshold(long value)
    {
        m_CreditAvailableThreshold = value;
    }

    DateTime& GetPeriod()
    {
        return m_Period;
    }

    void setPeriod(DateTime& value)
    {
        m_Period = value;
    }

    //Adjusts the value of the current credit amount attribute.
    int UpdateAmount(DLMSClient* client, uint32_t value, std::vector<ByteBuffer>& reply);

    //Sets the value of the current credit amount attribute.
    int SetAmountToValue(DLMSClient* client, uint32_t value, std::vector<ByteBuffer>& reply);

    //Sets the value of the current credit amount attribute.
    int InvokeCredit(DLMSClient* client, DLMS_CREDIT_STATUS value, std::vector<ByteBuffer>& reply);

    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

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

    // Returns value of given attribute.
    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);
};
#endif
#endif

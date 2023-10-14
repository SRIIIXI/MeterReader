#ifndef CHARGE_H
#define CHARGE_H

#ifndef DLMS_IGNORE_CHARGE
#include "DLMSObject.h"
#include "UnitCharge.h"

class DLMSCharge : public DLMSObject
{
    long m_TotalAmountPaid;
    DLMS_CHARGE_TYPE m_ChargeType;
    unsigned char m_Priority;
    UnitCharge m_UnitChargeActive;
    UnitCharge m_UnitChargePassive;
    DateTime m_UnitChargeActivationTime;
    unsigned long m_Period;
    DLMS_CHARGE_CONFIGURATION m_ChargeConfiguration;
    DateTime m_LastCollectionTime;
    long m_LastCollectionAmount;
    long m_TotalAmountRemaining;
    unsigned short m_Proportion;

    static int GetUnitCharge(UnitCharge& charge, ByteBuffer& bb);
    static int SetUnitCharge(DLMSSettings& settings, UnitCharge& charge, DLMSValueEventArg& e);
public:
    //Constructor.
    DLMSCharge();

    //SN Constructor.
    DLMSCharge(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSCharge(std::string ln);

    /**
    * Online help:<br>
    * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
    *
    * @return Total amount paid.
    */
    long GetTotalAmountPaid()
    {
        return m_TotalAmountPaid;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Total amount paid.
     */
    void SetTotalAmountPaid(long value)
    {
        m_TotalAmountPaid = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Charge type.
     */
    DLMS_CHARGE_TYPE GetChargeType()
    {
        return m_ChargeType;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Charge type.
     */
    void SetChargeType(DLMS_CHARGE_TYPE value)
    {
        m_ChargeType = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Priority
     */
    unsigned char GetPriority()
    {
        return m_Priority;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Priority
     */
    void SetPriority(unsigned char  value)
    {
        m_Priority = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Unit charge active.
     */
    UnitCharge& GetUnitChargeActive()
    {
        return m_UnitChargeActive;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Unit charge passive.
     */
    UnitCharge& GetUnitChargePassive()
    {
        return m_UnitChargePassive;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Unit charge activation time.
     */
    DateTime& GetUnitChargeActivationTime()
    {
        return m_UnitChargeActivationTime;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Unit charge activation time.
     */
    void SetUnitChargeActivationTime(DateTime& value)
    {
        m_UnitChargeActivationTime = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Period.
     */
    unsigned long GetPeriod()
    {
        return m_Period;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Period.
     */
    void SetPeriod(unsigned long value)
    {
        m_Period = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Charge configuration.
     */
    DLMS_CHARGE_CONFIGURATION GetChargeConfiguration()
    {
        return m_ChargeConfiguration;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Charge configuration.
     */
    void SetChargeConfiguration(DLMS_CHARGE_CONFIGURATION value)
    {
        m_ChargeConfiguration = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Last collection time.
     */
    DateTime& GetLastCollectionTime()
    {
        return m_LastCollectionTime;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Last collection time.
     */
    void SetLastCollectionTime(DateTime& value)
    {
        m_LastCollectionTime = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Last collection amount.
     */
    long GetLastCollectionAmount()
    {
        return m_LastCollectionAmount;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Last collection amount.
     */
    void SetLastCollectionAmount(long value)
    {
        m_LastCollectionAmount = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Total amount remaining
     */
    long GetTotalAmountRemaining()
    {
        return m_TotalAmountRemaining;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Total amount remaining
     */
    void SetTotalAmountRemaining(long value)
    {
        m_TotalAmountRemaining = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Proportion
     */
    unsigned short GetProportion()
    {
        return m_Proportion;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @param value
     *            Proportion
     */
    void SetProportion(unsigned short value)
    {
        m_Proportion = value;
    }

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
    int GetUIDataType(int index, DLMS_DATA_TYPE& type);

    // Returns value of given attribute.
    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);
};
#endif
#endif

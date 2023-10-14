#ifndef DLMS_ACCOUNT_H
#define DLMS_ACCOUNT_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_ACCOUNT
#include "DLMSObject.h"
#include "CreditChargeConfiguration.h"
#include "TokenGatewayConfiguration.h"
#include "Currency.h"

class DLMSAccount : public DLMSObject
{
    DLMS_ACCOUNT_PAYMENT_MODE m_PaymentMode;
    DLMS_ACCOUNT_STATUS m_AccountStatus;
    unsigned char m_CurrentCreditInUse;
    DLMS_ACCOUNT_CREDIT_STATUS m_CurrentCreditStatus;
    int m_AvailableCredit;   
    int m_AmountToClear;
    int m_ClearanceThreshold;
    int m_AggregatedDebt;
    std::vector<std::string> m_CreditReferences;
    std::vector<std::string> m_ChargeReferences;
    /**
     * Credit charge configurations.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     */
    std::vector<CreditChargeConfiguration> m_CreditChargeConfigurations;

    /**
     * Token gateway configurations.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     */
    std::vector<TokenGatewayConfiguration> m_TokenGatewayConfigurations;
    /**
     * Account activation time.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     */
    DateTime m_AccountActivationTime;

    /**
     * Account closure time.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     */
    DateTime m_AccountClosureTime;

    /**
     * Currency.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     */
    Currency m_Currency;
    /**
     * Low credit threshold.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     */
    int m_LowCreditThreshold;
    /**
     * Next credit available threshold.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     */
    int m_NextCreditAvailableThreshold;
    /**
     * Max Provision.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     */
    unsigned short m_MaxProvision;

    /**
     * Max provision period.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     */
    int m_MaxProvisionPeriod;

    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);
public:
    //Constructor.
    DLMSAccount();

    //SN Constructor.
    DLMSAccount(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSAccount(std::string ln);

    /**
        * Online help:<br>
        * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
        *
        * @return Payment mode.
        */
    DLMS_ACCOUNT_PAYMENT_MODE GetPaymentMode()
    {
        return m_PaymentMode;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Payment mode.
     */
    void SetPaymentMode(DLMS_ACCOUNT_PAYMENT_MODE value)
    {
        m_PaymentMode = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Account status.
     */
    DLMS_ACCOUNT_STATUS GetAccountStatus()
    {
        return m_AccountStatus;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Account status.
     */
    void SetAccountStatus(DLMS_ACCOUNT_STATUS value)
    {
        m_AccountStatus = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Credit In Use.
     */
    unsigned char GetCurrentCreditInUse()
    {
        return m_CurrentCreditInUse;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Credit In Use.
     */
    void SetCurrentCreditInUse(unsigned char value)
    {
        m_CurrentCreditInUse = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Credit status.
     */
    DLMS_ACCOUNT_CREDIT_STATUS GetCurrentCreditStatus()
    {
        return m_CurrentCreditStatus;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Credit status.
     */
    void SetCurrentCreditStatus(DLMS_ACCOUNT_CREDIT_STATUS value)
    {
        m_CurrentCreditStatus = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return AvailableCredit
     */
    int GetAvailableCredit()
    {
        return m_AvailableCredit;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            AvailableCredit
     */
    void SetAvailableCredit(int value)
    {
        m_AvailableCredit = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Amount to clear.
     */
    int GetAmountToClear()
    {
        return m_AmountToClear;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Amount to clear.
     */
    void SetAmountToClear(int value)
    {
        m_AmountToClear = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Clearance threshold.
     */
    int GetClearanceThreshold()
    {
        return m_ClearanceThreshold;
    }

    /**
     * @param value
     *            Clearance threshold.
     */
    void SetClearanceThreshold(int value)
    {
        m_ClearanceThreshold = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Aggregated debt.
     */
    int GetAggregatedDebt()
    {
        return m_AggregatedDebt;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Aggregated debt.
     */
    void SetAggregatedDebt(int value)
    {
        m_AggregatedDebt = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Credit references.
     */
    std::vector<std::string>& GetCreditReferences()
    {
        return m_CreditReferences;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Credit references.
     */
    void SetCreditReferences(std::vector<std::string>& value)
    {
        m_CreditReferences = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Charge references.
     */
    std::vector<std::string>& GetChargeReferences()
    {
        return m_ChargeReferences;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Charge references.
     */
    void SetChargeReferences(std::vector<std::string>& value)
    {
        m_ChargeReferences = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Credit charge configurations.
     */
    std::vector<CreditChargeConfiguration>& GetCreditChargeConfigurations()
    {
        return m_CreditChargeConfigurations;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Credit charge configurations.
     */
    void SetCreditChargeConfigurations(std::vector<CreditChargeConfiguration>& value)
    {
        m_CreditChargeConfigurations = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Token gateway configurations.
     */
    std::vector<TokenGatewayConfiguration>& GetTokenGatewayConfigurations()
    {
        return m_TokenGatewayConfigurations;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Token gateway configurations.
     */
    void SetTokenGatewayConfigurations(std::vector<TokenGatewayConfiguration>& value)
    {
        m_TokenGatewayConfigurations = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Account activation time.
     */
    DateTime& GetAccountActivationTime()
    {
        return m_AccountActivationTime;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Account activation time.
     */
    void SetAccountActivationTime(DateTime& value)
    {
        m_AccountActivationTime = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Account closure time.
     */
    DateTime& GetAccountClosureTime()
    {
        return m_AccountClosureTime;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Account closure time.
     */
    void SetAccountClosureTime(DateTime& value)
    {
        m_AccountClosureTime = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Currency.
     */
    Currency& GetCurrency()
    {
        return m_Currency;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Low credit threshold.
     */
    int GetLowCreditThreshold()
    {
        return m_LowCreditThreshold;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Low credit threshold.
     */
    void SetLowCreditThreshold(int value)
    {
        m_LowCreditThreshold = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Next credit available threshold.
     */
    int GetNextCreditAvailableThreshold()
    {
        return m_NextCreditAvailableThreshold;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Next credit available threshold.
     */
    void SetNextCreditAvailableThreshold(int value)
    {
        m_NextCreditAvailableThreshold = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Max Provision.
     */
    int GetMaxProvision()
    {
        return m_MaxProvision;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Max Provision.
     */
    void SetMaxProvision(int value)
    {
        m_MaxProvision = value;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @return Max provision period.
     */
    int GetMaxProvisionPeriod()
    {
        return m_MaxProvisionPeriod;
    }

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAccount
     *
     * @param value
     *            Max provision period.
     */
    void SetMaxProvisionPeriod(int value) {
        m_MaxProvisionPeriod = value;
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

    // Returns value of given attribute.
    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    /////////////////////////////////////////////////////////////////////////
    //Activate the account value.
    int Activate(
        DLMSClient* client,
        std::vector<ByteBuffer>& reply);

    /////////////////////////////////////////////////////////////////////////
    //Close the account value.
    int Close(
        DLMSClient* client,
        std::vector<ByteBuffer>& reply);

    /////////////////////////////////////////////////////////////////////////
    //Reset the account value.
    int Reset(
        DLMSClient* client,
        std::vector<ByteBuffer>& reply);
};
#endif
#endif

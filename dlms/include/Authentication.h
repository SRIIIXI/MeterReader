#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include "DLMSVariant.h"

class Authentication
{
    DLMSVariant m_ClientID;
    DLMS_AUTHENTICATION m_Type;
    std::string m_Password;
public:
    //Constructor.
    Authentication(void)
    {
    }

    //Constructor.
    Authentication(DLMS_AUTHENTICATION type, std::string password, DLMSVariant clientID)
    {
        m_Type = type;
        m_ClientID = clientID;
    }


    /// <summary>
    /// Authentication type
    /// </summary>
    DLMS_AUTHENTICATION GetType()
    {
        return m_Type;
    }

    /// <summary>
    /// Authentication type
    /// </summary>
    void SetType(DLMS_AUTHENTICATION value)
    {
        m_Type = value;
    }

    /// <summary>
    /// Client address.
    /// </summary>
    DLMSVariant GetClientID()
    {
        return m_ClientID;
    }

    /// <summary>
    /// Client address.
    /// </summary>
    void SetClientID(DLMSVariant value)
    {
        m_ClientID = value;
    }

    /// <summary>
    /// Get Used password.
    /// </summary>
    std::string GetPassword()
    {
        return m_Password;
    }

    /// <summary>
    /// Set Used password.
    /// </summary>
    void SetPassword(std::string value)
    {
        m_Password = value;
    }
};
#endif

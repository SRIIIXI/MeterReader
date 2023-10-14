#ifndef DLMS_PPP_SETUP_IPCP_OPTION_H
#define DLMS_PPP_SETUP_IPCP_OPTION_H

#include "IDLMSBase.h"
#include "DLMSObject.h"
#include "Helpers.h"

enum PPP_SETUP_IPCP_OPTION_TYPE
{
    PPP_SETUP_IPCP_OPTION_TYPE_IPCOMPRESSIONPROTOCOL = 2,
    PPP_SETUP_IPCP_OPTION_TYPE_PREFLOCALIP = 3,
    PPP_SETUP_IPCP_OPTION_TYPE_PREFPEERIP = 20,
    PPP_SETUP_IPCP_OPTION_TYPE_GAO = 21,
    PPP_SETUP_IPCP_OPTION_TYPE_USIP = 22
};

class DLMSPppSetupIPCPOption
{
    friend class DLMSPppSetup;
    PPP_SETUP_IPCP_OPTION_TYPE m_Type;
    int m_Length;
    DLMSVariant m_Data;
public:

    PPP_SETUP_IPCP_OPTION_TYPE GetType();
    void SetType(PPP_SETUP_IPCP_OPTION_TYPE value);

    int GetLength();
    void SetLength(int value);

    DLMSVariant& GetData();
    void SetData(DLMSVariant& value);

    std::string ToString();
};
#endif

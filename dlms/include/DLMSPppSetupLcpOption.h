#ifndef DLMS_PPP_SETUP_LCP_OPTION_H
#define DLMS_PPP_SETUP_LCP_OPTION_H

#include "IDLMSBase.h"
#include "DLMSObject.h"
#include "Helpers.h"

typedef enum
{
    PPP_SETUP_LCP_OPTION_TYPE_MAX_REC_UNIT = 1,
    PPP_SETUP_LCP_OPTION_TYPE_ASYNC_CONTROL_CHAR_MAP = 2,
    PPP_SETUP_LCP_OPTION_TYPE_AUTH_PROTOCOL = 3,
    PPP_SETUP_LCP_OPTION_TYPE_MAGIC_NUMBER = 5,
    PPP_SETUP_LCP_OPTION_TYPE_PROTOCOL_FIELD_COMPRESSION = 7,
    PPP_SETUP_LCP_OPTION_TYPE_ADDRESS_AND_CTR_COMPRESSION = 8,
    PPP_SETUP_LCP_OPTION_TYPE_FCS_ALTERNATIVES = 9,
    PPP_SETUP_LCP_OPTION_TYPE_CALLBACK = 13
} PPP_SETUP_LCP_OPTION_TYPE;

class DLMSPppSetupLcpOption
{
    PPP_SETUP_LCP_OPTION_TYPE m_Type;
    DLMSVariant m_Data;
    int m_Length;
public:

    PPP_SETUP_LCP_OPTION_TYPE GetType();
    void SetType(PPP_SETUP_LCP_OPTION_TYPE value);

    int GetLength();
    void SetLength(int value);

    DLMSVariant& GetData();
    void SetData(DLMSVariant& value);

    std::string ToString();
};
#endif

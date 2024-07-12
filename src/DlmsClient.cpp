#include <string.h>
#include <QByteArray>
#include "DlmsClient.hpp"
#include "DLMSData.h"
#include "Mk7Mi.hpp"
#include "Enums.h"

class CIUKeys
{
public:
    ByteBuffer RandomString;
    ByteBuffer Client103WrappedKeys;
    ByteBuffer Client104WrappedKeys;
    ByteBuffer Client105WrappedKeys;

    unsigned char Client103AuthenticationKey[16];
    unsigned char Client103CipherKey[16];
    unsigned char Client104AuthenticationKey[16];
    unsigned char Client104CipherKey[16];
    unsigned char Client105AuthenticationKey[16];
    unsigned char Client105CipherKey[16];
};

static std::map<int, DlmsClient*> clientMap;
static CIUKeys EDMI_CIU_KEYS;

// DLMS Configuration

bool USE_LN_REFERENCING = true;
int SERVER_ADDRESS = 1;
int SERVER_PHYSICAL_ADDRESS = 17;
int SERVER_ADDRESS_SIZE = 1;

DLMS_INTERFACE_TYPE APP_INTERFACE_TYPE = DLMS_INTERFACE_TYPE_HDLC;
DLMS_AUTHENTICATION APP_AUTHENTICATION_TYPE = DLMS_AUTHENTICATION_HIGH_GMAC;
DLMS_SECURITY APP_SECURITY_TYPE = DLMS_SECURITY_AUTHENTICATION_ENCRYPTION;

bool AUTO_INCREASE_INVOKE_ID = true;
unsigned char SYSTEMTITLE[9] = { 0 };
unsigned char GBT_WINDOW_SIZE = 1;
unsigned char WINDOW_SIZE = 1;
uint16_t MAXIMUM_INFORMATION = 128;

int SYSTLEN = 8;
int PASSLEN = 8;
int ATKYLEN = 16;
int BLCYLEN = 16;
int MSKYLEN = 16;
int MFGRLEN = 8;

const unsigned char MASTERKEY[] =
{
    0x31,  0x31,  0x31, 0x31,  0x31, 0x31,  0x31,  0x31,
    0x31,  0x31,  0x31, 0x31,  0x31, 0x31,  0x31,  0x31
};

const unsigned char BLOCKCIPHERKEY[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

const unsigned char AUTHENTICATIONKEY[] =
{
    0xD0,  0xD1,  0xD2, 0xD3,  0xD4,  0xD5,  0xD6,  0xD7,
    0xD8,  0xD9,  0xDA,  0xDB,  0xDC, 0xDD,  0xDE,  0xDF
};

const unsigned char MANUFACTURERID[] =
{
    'E', 'D', 'M'
};

unsigned char RUNTIME_CUSTOMER_PIN[9] =
{
    '0','0','0','0','0','0','0','0','\0'
};

unsigned char MANUFACTURER_PASSWORD[9] =
{
        '1','2','3','4','5','6','7','8','\0'
};

void DlmsClient::CreateClients()
{
    DlmsClient* publicClient = new DlmsClient(
        SYSTEMTITLE,
        SYSTLEN,
        USE_LN_REFERENCING,
        CLIENT_PUBLIC,
        SERVER_ADDRESS,
        SERVER_PHYSICAL_ADDRESS,
        SERVER_ADDRESS_SIZE,
        DLMS_AUTHENTICATION_NONE,
        RUNTIME_CUSTOMER_PIN,
        PASSLEN,
        APP_INTERFACE_TYPE,
        DLMS_SECURITY_NONE,
        AUTO_INCREASE_INVOKE_ID,
        OBIS_FRAME_COUNTER_PAIRING,
        AUTHENTICATIONKEY,
        ATKYLEN,
        BLOCKCIPHERKEY,
        BLCYLEN,
        MASTERKEY,
        MSKYLEN,
        MANUFACTURERID,
        MFGRLEN,
        GBT_WINDOW_SIZE,
        WINDOW_SIZE,
        MAXIMUM_INFORMATION,
        RUNTIME_CUSTOMER_PIN);

    clientMap[CLIENT_PUBLIC] = publicClient;
    publicClient->EnableCustomChallenge(false);
    
    DlmsClient* pairingClient = new DlmsClient(
        SYSTEMTITLE,
        SYSTLEN,
        USE_LN_REFERENCING,
        CLIENT_PAIRING,
        SERVER_ADDRESS,
        SERVER_PHYSICAL_ADDRESS,
        SERVER_ADDRESS_SIZE,
        DLMS_AUTHENTICATION_HIGH_GMAC,
        RUNTIME_CUSTOMER_PIN,
        PASSLEN,
        APP_INTERFACE_TYPE,
        DLMS_SECURITY_AUTHENTICATION_ENCRYPTION,
        AUTO_INCREASE_INVOKE_ID,
        OBIS_FRAME_COUNTER_PAIRING,
        AUTHENTICATIONKEY,
        ATKYLEN,
        BLOCKCIPHERKEY,
        BLCYLEN,
        nullptr,
        0,
        MANUFACTURERID,
        MFGRLEN,
        GBT_WINDOW_SIZE,
        WINDOW_SIZE,
        MAXIMUM_INFORMATION,
        RUNTIME_CUSTOMER_PIN);

    clientMap[CLIENT_PAIRING] = pairingClient;
    pairingClient->EnableCustomChallenge(true);
    
    DlmsClient* client103 = new DlmsClient(
        SYSTEMTITLE,
        SYSTLEN,
        USE_LN_REFERENCING,
        CLIENT_SECURED_103,
        SERVER_ADDRESS,
        SERVER_PHYSICAL_ADDRESS,
        SERVER_ADDRESS_SIZE,
        DLMS_AUTHENTICATION_HIGH_GMAC,
        RUNTIME_CUSTOMER_PIN,
        PASSLEN,
        APP_INTERFACE_TYPE,
        DLMS_SECURITY_AUTHENTICATION_ENCRYPTION,
        AUTO_INCREASE_INVOKE_ID,
        OBIS_FRAME_COUNTER_PAIRING,
        AUTHENTICATIONKEY,
        ATKYLEN,
        BLOCKCIPHERKEY,
        BLCYLEN,
        nullptr,
        0,
        MANUFACTURERID,
        MFGRLEN,
        GBT_WINDOW_SIZE,
        WINDOW_SIZE,
        MAXIMUM_INFORMATION,
        RUNTIME_CUSTOMER_PIN);

    clientMap[CLIENT_SECURED_103] = client103;
    
    DlmsClient* client104 = new DlmsClient(
        SYSTEMTITLE,
        SYSTLEN,
        USE_LN_REFERENCING,
        CLIENT_SECURED_104,
        SERVER_ADDRESS,
        SERVER_PHYSICAL_ADDRESS,
        SERVER_ADDRESS_SIZE,
        DLMS_AUTHENTICATION_HIGH_GMAC,
        RUNTIME_CUSTOMER_PIN,
        PASSLEN,
        APP_INTERFACE_TYPE,
        DLMS_SECURITY_AUTHENTICATION_ENCRYPTION,
        AUTO_INCREASE_INVOKE_ID,
        OBIS_FRAME_COUNTER_PAIRING,
        AUTHENTICATIONKEY,
        ATKYLEN,
        BLOCKCIPHERKEY,
        BLCYLEN,
        nullptr,
        0,
        MANUFACTURERID,
        MFGRLEN,
        GBT_WINDOW_SIZE,
        WINDOW_SIZE,
        MAXIMUM_INFORMATION,
        RUNTIME_CUSTOMER_PIN);

    clientMap[CLIENT_SECURED_104] = client104;
    
    DlmsClient* client105 = new DlmsClient(
        SYSTEMTITLE,
        SYSTLEN,
        USE_LN_REFERENCING,
        CLIENT_SECURED_105,
        SERVER_ADDRESS,
        SERVER_PHYSICAL_ADDRESS,
        SERVER_ADDRESS_SIZE,
        DLMS_AUTHENTICATION_HIGH_GMAC,
        RUNTIME_CUSTOMER_PIN,
        PASSLEN,
        APP_INTERFACE_TYPE,
        DLMS_SECURITY_AUTHENTICATION_ENCRYPTION,
        AUTO_INCREASE_INVOKE_ID,
        OBIS_FRAME_COUNTER_PAIRING,
        AUTHENTICATIONKEY,
        ATKYLEN,
        BLOCKCIPHERKEY,
        BLCYLEN,
        nullptr,
        0,
        MANUFACTURERID,
        MFGRLEN,
        GBT_WINDOW_SIZE,
        WINDOW_SIZE,
        MAXIMUM_INFORMATION,
        RUNTIME_CUSTOMER_PIN);

    clientMap[CLIENT_SECURED_105] = client105;
}

DlmsClient* DlmsClient::GetClient(const int clientid)
{
    return clientMap[clientid];
}

DlmsClient::DlmsClient()
{
    errorcode = DLMS_ERROR_CODE_OK;
    useLogicalNameReferencing = true;
    clientAddress = 16;
    serverAddress = 1;
    serverPhysicalAddress = 17;
    serverAddressSize = 4;
    authentication = DLMS_AUTHENTICATION_NONE;
    password = nullptr;
    interfaceType = DLMS_INTERFACE_TYPE_HDLC;
    security = DLMS_SECURITY_NONE;
    autoIncreaseInvokeID = false;
    invocationCounter = nullptr;
    systemTitle = nullptr;
    authenticationKey = nullptr;
    blockCipherKey = nullptr;
    dedicatedKey = nullptr;
    gbtWindowSize = 1;
    windowSize = 1;
    maxInfo = 128;
    manufacturerId = nullptr;
    client = nullptr;
    systlen = 8;
    passlen = 8;
    atkylen = 16;
    blcylen = 16;
    mskylen = 16;
    mfgrlen = 8;
}

DlmsClient::DlmsClient(const unsigned char* isystemTitle,
                       int isystemTitlelen,
                       bool iuseLogicalNameReferencing,
                       int iclientAddress,
                       int iServerAddress,
                       int iServerPhysicalAddress,
                       int iServerAddressSize,
                       DLMS_AUTHENTICATION iauthentication,
                       unsigned const char* ipassword,
                       int ipasswordlen,
                       DLMS_INTERFACE_TYPE iinterfaceType,
                       DLMS_SECURITY isecurity,
                       bool iautoIncreaseInvokeID,
                       const char* iinvocationCounter,
                       const unsigned char* iauthenticationKey,
                       int iauthenticationKeylen,
                       const unsigned char* iblockCipherKey,
                       int iblockCipherKeylen,
                       const unsigned char* idedicatedKey,
                       int idedicatedKeylen,
                       const unsigned char* imanufacturerId,
                       int imanufacturerIdlen,
                       unsigned char igbtWindowSize,
                       unsigned char iwindowSize,
                       uint16_t imaxInfo,
                       const unsigned char* customerpin)
{
    errorcode = DLMS_ERROR_CODE_OK;

    useLogicalNameReferencing = iuseLogicalNameReferencing;
    clientAddress = iclientAddress;
    serverAddress = iServerAddress;
    serverPhysicalAddress = iServerPhysicalAddress;
    serverAddressSize = iServerAddressSize;
    authentication = iauthentication;
    interfaceType = iinterfaceType;
    security = isecurity;
    autoIncreaseInvokeID = iautoIncreaseInvokeID;
    gbtWindowSize = igbtWindowSize;
    windowSize = iwindowSize;
    maxInfo = imaxInfo;

    systlen = isystemTitlelen;
    passlen = ipasswordlen;
    atkylen = iauthenticationKeylen;
    blcylen = iblockCipherKeylen;
    mskylen = idedicatedKeylen;
    mfgrlen = imanufacturerIdlen;

    if (ipassword)
    {
        password = new char[passlen + 1];
        memset(password, 0, passlen + 1);
        memcpy(password, ipassword, passlen);
    }
    else
    {
        password = nullptr;
    }

    if (iinvocationCounter)
    {
        invocationCounter = new char[strlen(iinvocationCounter) + 1];
        memset(invocationCounter, 0, strlen(iinvocationCounter) + 1);
        strcpy(invocationCounter, iinvocationCounter);
    }
    else
    {
        invocationCounter = nullptr;
    }

    if (isystemTitle)
    {
        systemTitle = new char[systlen +1];
        memset(systemTitle, 0, systlen + 1);
        memcpy(systemTitle, isystemTitle, systlen);
    }
    else
    {
        systemTitle = nullptr;
    }

    if (iauthenticationKey)
    {
        authenticationKey = new char[atkylen + 1];
        memset(authenticationKey, 0, atkylen + 1);
        memcpy(authenticationKey, iauthenticationKey, atkylen);
    }
    else
    {
        authenticationKey = nullptr;
    }

    if (iblockCipherKey)
    {
        blockCipherKey = new char[blcylen + 1];
        memset(blockCipherKey, 0, blcylen + 1);
        memcpy(blockCipherKey, iblockCipherKey, blcylen);
    }
    else
    {
        blockCipherKey = nullptr;
    }

    if (idedicatedKey)
    {
        dedicatedKey = new char[mskylen + 1];
        memset(dedicatedKey, 0, mskylen + 1);
        memcpy(dedicatedKey, idedicatedKey, mskylen);
    }
    else
    {
        dedicatedKey = nullptr;
    }

    if (imanufacturerId)
    {
        manufacturerId = new char[mfgrlen + 1];
        memset(manufacturerId, 0, mfgrlen + 1);
        memcpy(manufacturerId, imanufacturerId, mfgrlen);
    }
    else
    {
        manufacturerId = nullptr;
    }

    client = new DLMSSecureClient(useLogicalNameReferencing, clientAddress, serverAddress, authentication, password, interfaceType, systemTitle);

    client->SetCustomerPIN((unsigned char*)customerpin, 8);
}

void DlmsClient::SetSystemTitle(const unsigned char* isystemTitle, int isystemTitlelen)
{
    if(isystemTitle && isystemTitlelen == 8)
    {
        if(systemTitle)
        {
            delete systemTitle;
            systemTitle = nullptr;
        }

        systemTitle = new char[systlen +1];
        memset(systemTitle, 0, systlen + 1);
        memcpy(systemTitle, isystemTitle, systlen);

        ByteBuffer buffer;
        buffer.Set(systemTitle, isystemTitlelen);

        client->GetCiphering()->SetSystemTitle(buffer);
    }
}

void DlmsClient::SetAuthenticationKey(const unsigned char* iauthKey, int iKeylen)
{
    if(iauthKey && iKeylen == 16)
    {
        if(authenticationKey)
        {
            delete authenticationKey;
            authenticationKey = nullptr;
        }

        authenticationKey = new char[atkylen +1];
        memset(authenticationKey, 0, atkylen + 1);
        memcpy(authenticationKey, iauthKey, atkylen);

        ByteBuffer buffer;
        buffer.Set(authenticationKey, iKeylen);

        client->GetCiphering()->SetAuthenticationKey(buffer);
    }
}

void DlmsClient::SetCipheringKey(const unsigned char* icipherKey, int iKeylen)
{
    if(blockCipherKey && iKeylen == 16)
    {
        if(blockCipherKey)
        {
            delete blockCipherKey;
            blockCipherKey = nullptr;
        }

        blockCipherKey = new char[blcylen +1];
        memset(blockCipherKey, 0, blcylen + 1);
        memcpy(blockCipherKey, icipherKey, blcylen);

        ByteBuffer buffer;
        buffer.Set(blockCipherKey, iKeylen);

        client->GetCiphering()->SetBlockCipherKey(buffer);
    }
}

ByteBuffer& DlmsClient::GetSystemTitle()
{
    return client->GetCiphering()->GetSystemTitle();
}

ByteBuffer& DlmsClient::GetAuthenticationKey()
{
    return client->GetCiphering()->GetAuthenticationKey();
}

ByteBuffer& DlmsClient::GetCipheringKey()
{
    return client->GetCiphering()->GetBlockCipherKey();
}

void DlmsClient::UnwrapKeys(const unsigned char* random_octet, const int random_octet_len, const unsigned char* wrapped_key, const int wrapped_key_len, ByteBuffer& reply)
{
    return client->UnwrapKeys(random_octet, random_octet_len, wrapped_key, wrapped_key_len, reply);
}

DlmsClient::~DlmsClient()
{
}

bool DlmsClient::InitializeClient()
{
    client->GetCiphering()->SetSecurity(security);
    client->SetAutoIncreaseInvokeID(autoIncreaseInvokeID);
    client->SetGbtWindowSize(gbtWindowSize);
    client->GetHdlcSettings().SetWindowSizeRX(windowSize);
    client->GetHdlcSettings().SetWindowSizeTX(windowSize);
    client->GetHdlcSettings().SetMaxInfoRX(maxInfo);
    client->GetHdlcSettings().SetMaxInfoTX(maxInfo);
    client->GetHdlcSettings().SetMaxInfoTX(maxInfo);
    client->SetManufacturerId(manufacturerId);

    ByteBuffer bb;
    if (systemTitle != NULL)
    {
        bb.Clear();
        bb.Set(systemTitle, systlen);
        client->GetCiphering()->SetSystemTitle(bb);
    }
    if (authenticationKey != NULL)
    {
        bb.Clear();
        bb.Set(authenticationKey, atkylen);
        client->GetCiphering()->SetAuthenticationKey(bb);
    }
    if (blockCipherKey != NULL)
    {
        bb.Clear();
        bb.Set(blockCipherKey, blcylen);
        client->GetCiphering()->SetBlockCipherKey(bb);
    }
    if (dedicatedKey != NULL)
    {
        bb.Clear();
        bb.Set(dedicatedKey, mskylen);
        client->GetCiphering()->SetDedicatedKey(bb);
    }

    client->SetClientAddress(clientAddress);
    int svradd = DLMSClient::GetServerAddress(serverAddress, serverPhysicalAddress, serverAddressSize);
    client->SetServerAddress(svradd);
    client->GetHdlcSettings().SetMaxInfoRX(1280);
    client->GetHdlcSettings().SetMaxInfoTX(1280);
    client->GetHdlcSettings().SetWindowSizeRX(1);
    client->GetHdlcSettings().SetWindowSizeTX(1);

    return true;
}

void DlmsClient::SetInvocationCounter(unsigned long iFrameCounter)
{
    client->GetCiphering()->SetInvocationCounter(1 + iFrameCounter);
}

void DlmsClient::EnableCustomChallenge(bool fl)
{
    client->EnableCustomChallenge(fl);
}

const char* DlmsClient::ErrorMessage()
{
    return DLMSConverter::GetErrorMessage(errorcode);
}

int DlmsClient::ErrorCode()
{
    return errorcode;
}

unsigned long DlmsClient::GetInvocationCounter()
{
    return client->GetCiphering()->GetInvocationCounter();
}

void DlmsClient::CreateSNRMFrame(QByteArray& buffer)
{
    std::vector<ByteBuffer> data;
    int ret = 0;

    ret = client->SNRMRequest(data);
    if (ret != 0)
    {
        errorcode = ret;
        return;
    }

    buffer.clear();

    for(int x = 0; x < data.at(0).GetSize(); x++)
    {
        buffer.append(data.at(0).GetData()[x]);
    }
}

bool DlmsClient::CheckUARFrame(const QByteArray& buffer)
{
    ReplyData reply;
    ByteBuffer bytebuffer;
    int ret = 0;

    bytebuffer.Set(buffer.data(), buffer.length());

    ret = client->GetData(bytebuffer, reply, reply);

    if (ret != DLMS_ERROR_CODE_OK && ret != DLMS_ERROR_CODE_HARDWARE_FAULT)
    {
        return false;
    }

    ret = client->ParseUAResponse(reply.GetData());

    if (ret != 0)
    {
        errorcode = ret;
        return false;
    }

    return true;
}

void DlmsClient::CreateAARQFrame(QByteArray& buffer)
{
    std::vector<ByteBuffer> data;
    int ret = 0;

    ret = client->AARQRequest(data);
    if (ret != 0)
    {
        errorcode = ret;
        return;
    }

    buffer.clear();

    for(int x = 0; x < data.at(0).GetSize(); x++)
    {
        buffer.append(data.at(0).GetData()[x]);
    }

    //if (client->GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC)
    //{
    //    std::string chl;
    //    chl += "CtoS = ";
    //    chl += Helpers::BytesToHex(client->GetCtoSChallenge().GetData(), client->GetCtoSChallenge().GetSize());
    //    globalApplicationData->AppLogInformation(chl.c_str());
    //}
}

bool DlmsClient::CheckAAREFrame(const QByteArray& buffer)
{
    ReplyData reply;
    ByteBuffer bytebuffer;
    int ret = 0;

    bytebuffer.Set(buffer.data(), buffer.length());

    ret = client->GetData(bytebuffer, reply, reply);

    if (ret == DLMS_ERROR_CODE_OK || ret == DLMS_ERROR_CODE_HARDWARE_FAULT)
    {
        ret = client->ParseAAREResponse(reply.GetData());

        //if (client->GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC)
        //{
        //    if(client->GetStoCChallenge().GetSize() > 0)
        //    {
        //        std::string chl;
        //        chl += "StoC = ";
        //        chl += Helpers::BytesToHex(client->GetStoCChallenge().GetData(), client->GetStoCChallenge().GetSize());
        //        globalApplicationData->AppLogInformation(chl.c_str());
        //    }
        //}

        if (ret != 0)
        {
            errorcode = ret;
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

void DlmsClient::CreateFrameCounterFrame(QByteArray& buffer, QString obis)
{
    int ret = -1;

    if(invocationCounter != nullptr && client->GetCiphering() != nullptr)
    {
        client->SetProposedConformance((DLMS_CONFORMANCE)(client->GetProposedConformance() | DLMS_CONFORMANCE_GENERAL_PROTECTION));

        std::vector<ByteBuffer> data;
        data.clear();

        std::string ln;

        if(obis.length()> 0 && !obis.isEmpty())
        {
            ln.append(obis.toStdString().c_str());
        }
        else
        {
            ln.append(invocationCounter);
        }

        DLMSData d(ln);
        std::string str;

        ret = client->Read(&d, 2, data);

        if (ret != 0)
        {
            errorcode = ret;
            return;
        }

        buffer.clear();

        for(int x = 0; x < data.at(0).GetSize(); x++)
        {
            buffer.append(data.at(0).GetData()[x]);
        }
    }
    else
    {
        errorcode = DLMS_ERROR_CODE_OK;
    }

    return;
}

bool DlmsClient::CheckFrameCounterFrame(const QByteArray& buffer, long &ioFrameCounter)
{
    std::string ln;
    ln.append(invocationCounter);
    DLMSData d(ln);
    std::string str;
    ReplyData reply;
    ByteBuffer bytebuffer;
    int ret = 0;

    bytebuffer.Set(buffer.data(), buffer.length());

    ret = client->GetData(bytebuffer, reply, reply);

    if (ret != DLMS_ERROR_CODE_OK && ret != DLMS_ERROR_CODE_HARDWARE_FAULT)
    {
        errorcode = ret;
        return false;
    }

    ret = client->UpdateValue(d, 2, reply.GetValue());

    //Update data type.
    DLMS_DATA_TYPE type;
    if ((ret = d.GetDataType(2, type)) != 0)
    {
        return false;
    }

    if (type == DLMS_DATA_TYPE_NONE)
    {
        type = reply.GetValue().vt;
        if ((ret = d.SetDataType(2, type)) != 0)
        {
            return false;
        }
    }

    client->GetCiphering()->SetInvocationCounter(1 + d.GetValue().ToInteger());
    ioFrameCounter = 1 + d.GetValue().ToInteger();

    return true;
}

void DlmsClient::CreateAuthenticationFrame(QByteArray& buffer)
{
    int ret = 0;
    std::vector<ByteBuffer> data;

    if (client->GetAuthentication() > DLMS_AUTHENTICATION_LOW)
    {
        std::map<std::string, ByteBuffer> debuginfo;

        ret = client->GetReplytoEDMIStoC(data, debuginfo, false);

        if (ret != 0)
        {
            errorcode = ret;
            return;
        }

        std::map<std::string, ByteBuffer>::iterator it;

        buffer.clear();

        for(int x = 0; x < data.at(0).GetSize(); x++)
        {
            buffer.append(data.at(0).GetData()[x]);
        }
    }
}

bool DlmsClient::CheckAuthenticationResponse(const QByteArray &buffer)
{
    ReplyData reply;
    ByteBuffer bytebuffer;
    int ret = 0;

    bytebuffer.Set(buffer.data(), buffer.length());

    ret = client->GetData(bytebuffer, reply, reply);

    if (ret == DLMS_ERROR_CODE_OK || ret == DLMS_ERROR_CODE_HARDWARE_FAULT)
    {
        ret = client->ValidateEDMIAutheticationResponse(reply.GetData());
        if (ret == 0)
        {
            return true;
        }
        else
        {
            errorcode = ret;
        }
    }

    return false;
}

void DlmsClient::CreateObjectRetreiveRequestFrame(QByteArray& buffer, const QString &obiscode)
{
    int ret = 0;
    std::vector<ByteBuffer> data;
    data.clear();

    std::string ln;
    ln.append(obiscode.toStdString());
    DLMSData d(ln);

    ret = client->Read(&d, 2, data);

    if (ret != 0)
    {
        errorcode = ret;
        return;
    }

    buffer.clear();

    for(int x = 0; x < data.at(0).GetSize(); x++)
    {
        buffer.append(data.at(0).GetData()[x]);
    }
}

bool DlmsClient::CheckObjectRetreiveResponseFrame(const QByteArray& buffer, DLMSVariant &objectData, const QString &obiscode)
{
    std::string ln;
    ln.append(obiscode.toStdString());
    DLMSData d(ln);
    std::string str;
    ReplyData reply;
    ByteBuffer bytebuffer;
    int ret = 0;

    bytebuffer.Set(buffer.data(), buffer.length());

    ret = client->GetData(bytebuffer, reply, reply);

    if (ret != DLMS_ERROR_CODE_OK && ret != DLMS_ERROR_CODE_HARDWARE_FAULT)
    {
        errorcode = ret;
        return false;
    }

    ret = client->UpdateValue(d, 2, reply.GetValue());

    //Update data type.
    DLMS_DATA_TYPE type;
    if ((ret = d.GetDataType(2, type)) != 0)
    {
        return false;
    }

    if (type == DLMS_DATA_TYPE_NONE)
    {
        type = reply.GetValue().vt;
        if ((ret = d.SetDataType(2, type)) != 0)
        {
            return false;
        }
    }

    objectData = d.GetValue();

    return true;
}

void DlmsClient::CreateRegisterParamRequestFrame(QByteArray& buffer, const QString &obiscode, int index)
{
    int ret = 0;
    std::vector<ByteBuffer> data;
    data.clear();

    std::string ln;
    ln.append(obiscode.toStdString());
    DLMSRegister d(ln);

    ret = client->Read(&d, index, data);

    if (ret != 0)
    {
        errorcode = ret;
        return;
    }

    buffer.clear();

    for(int x = 0; x < data.at(0).GetSize(); x++)
    {
        buffer.append(data.at(0).GetData()[x]);
    }
}

bool DlmsClient::CheckRegisterParamResponseFrame(const QByteArray &buffer, const QString &obiscode, float &value, int index)
{
    std::string ln;
    ln.append(obiscode.toStdString());
    DLMSRegister d(ln);
    ReplyData reply;
    ByteBuffer bytebuffer;
    int ret = 0;

    bytebuffer.Set(buffer.data(), buffer.length());

    ret = client->GetData(bytebuffer, reply, reply);

    if (ret != DLMS_ERROR_CODE_OK && ret != DLMS_ERROR_CODE_HARDWARE_FAULT)
    {
        errorcode = ret;
        return false;
    }

    ret = client->UpdateValue(d, index, reply.GetValue());

    //Update data type.
    DLMS_DATA_TYPE type;
    if ((ret = d.GetDataType(index, type)) != 0)
    {
        return false;
    }

    if (type == DLMS_DATA_TYPE_NONE)
    {
        type = reply.GetValue().vt;
        if ((ret = d.SetDataType(index, type)) != 0)
        {
            return false;
        }
    }

    if(index == 2)
    {
        value = d.GetValue().ToInteger();
    }

    if(index == 3)
    {
        value = d.GetScaler();
    }

    return true;
}

void DlmsClient::CreateAccountStatusRequestFrame(QByteArray& buffer, const QString &obiscode, int index)
{
    int ret = 0;
    std::vector<ByteBuffer> data;
    data.clear();

    std::string ln;
    ln.append(obiscode.toStdString());
    DLMSAccount d(ln);

    ret = client->Read(&d, index, data);

    if (ret != 0)
    {
        errorcode = ret;
        return;
    }

    buffer.clear();

    for(int x = 0; x < data.at(0).GetSize(); x++)
    {
        buffer.append(data.at(0).GetData()[x]);
    }
}

bool DlmsClient::CheckAccountStatusResponseFrame(const QByteArray &buffer, const QString &obiscode, DLMSAccount &value, bool &isfound, int index)
{
    std::string ln;
    ln.append(obiscode.toStdString());
    DLMSAccount d(ln);
    ReplyData reply;
    ByteBuffer bytebuffer;
    int ret = 0;

    bytebuffer.Set(buffer.data(), buffer.length());

    ret = client->GetData(bytebuffer, reply, reply);

    if (ret != DLMS_ERROR_CODE_OK && ret != DLMS_ERROR_CODE_HARDWARE_FAULT)
    {
        errorcode = ret;
        return false;
    }

    ret = client->UpdateValue(d, index, reply.GetValue());

    //Update data type.
    DLMS_DATA_TYPE type;
    if ((ret = d.GetDataType(index, type)) != 0)
    {
        return false;
    }

    if (type == DLMS_DATA_TYPE_NONE)
    {
        type = reply.GetValue().vt;
        if ((ret = d.SetDataType(index, type)) != 0)
        {
            return false;
        }
    }

    value = d;

    isfound = true;

    return true;
}

void DlmsClient::CreateTokenTransferRequestFrame(QByteArray& buffer, const QString &obiscode, const QString &tokenstring)
{
    int ret = 0;
    std::vector<ByteBuffer> data;
    data.clear();

    std::string ln;
    ln.append(obiscode.toStdString());
    DLMSTokenGateway d(ln);
    tokenGateway = d;

    ByteBuffer tokenbuffer;
    std::string hexstring = Helpers::BytesToHex((unsigned char*)tokenstring.toStdString().c_str(), tokenstring.length());
    tokenbuffer.SetHexString(hexstring.c_str());
    tokenGateway.SetToken(tokenbuffer);
    tokenGateway.SetDeliveryMethod(DLMS_TOKEN_DELIVERY_LOCAL);
    tokenGateway.SetStatusCode(FORMAT_OK);
    DLMSVariant tokenvar(tokenbuffer);

    ret = client->Method(&tokenGateway, 1, tokenvar, data);

    if (ret != 0)
    {
        errorcode = ret;
        return;
    }

    buffer.clear();

    for(int x = 0; x < data.at(0).GetSize(); x++)
    {
        buffer.append(data.at(0).GetData()[x]);
    }
}

bool DlmsClient::CheckTokenTransferResponseFrame(const QByteArray &buffer, const QString &obiscode, DLMSTokenGateway &value, bool &istransferred)
{
    std::string ln;
    ln.append(obiscode.toStdString());
    ReplyData reply;
    ByteBuffer bytebuffer;
    int ret = 0;

    bytebuffer.Set(buffer.data(), buffer.length());

    ret = client->GetData(bytebuffer, reply, reply);

    if (ret != DLMS_ERROR_CODE_OK && ret != DLMS_ERROR_CODE_HARDWARE_FAULT)
    {
        errorcode = ret;
        return false;
    }

    ret = client->UpdateValue(tokenGateway, 2, reply.GetValue());

    //Update data type.
    DLMS_DATA_TYPE type;
    if ((ret = tokenGateway.GetDataType(1, type)) != 0)
    {
        return false;
    }

    if (type == DLMS_DATA_TYPE_NONE)
    {
        type = reply.GetValue().vt;
        if ((ret = tokenGateway.SetDataType(1, type)) != 0)
        {
            return false;
        }
    }

    value = tokenGateway;

    istransferred = true;

    return true;
}

void DlmsClient::CreateTokenTransferStatusRequestFrame(QByteArray& buffer, const QString &obiscode)
{
    int ret = 0;
    std::vector<ByteBuffer> data;
    data.clear();

    std::string ln;
    ln.append(obiscode.toStdString());
    DLMSTokenGateway d(ln);
    tokenGateway = d;
    
    ret = client->Read(&tokenGateway, 6, data);

    if (ret != 0)
    {
        errorcode = ret;
        return;
    }

    buffer.clear();

    for(int x = 0; x < data.at(0).GetSize(); x++)
    {
        buffer.append(data.at(0).GetData()[x]);
    }
}

bool DlmsClient::CheckTokenTransferStatusResponseFrame(const QByteArray &buffer, const QString &obiscode, DLMSTokenGateway &value)
{
    std::string ln;
    ln.append(obiscode.toStdString());
    ReplyData reply, temp;
    ByteBuffer bytebuffer;
    int ret = 0;

    bytebuffer.Set(buffer.data(), buffer.length());

    ret = client->GetData(bytebuffer, reply, temp);

    if (ret != DLMS_ERROR_CODE_OK && ret != DLMS_ERROR_CODE_HARDWARE_FAULT)
    {
        errorcode = ret;
        return false;
    }
    
    ret = client->UpdateValue(tokenGateway, 6, reply.GetValue());

    //Update data type.
    DLMS_DATA_TYPE type;
    if ((ret = tokenGateway.GetDataType(6, type)) != 0)
    {
        return false;
    }

    if (type == DLMS_DATA_TYPE_NONE)
    {
        type = reply.GetValue().vt;
        if ((ret = tokenGateway.SetDataType(6, type)) != 0)
        {
            return false;
        }
    }
    
    value = tokenGateway;

    return true;
}

void DlmsClient::CreateReleaseFrame(QByteArray &buffer)
{
    if (!client)
    {
        return;
    }

    int ret = -1;
    std::vector<ByteBuffer> data;

    buffer.clear();
    ret = client->ReleaseRequest(data);

    if (ret != 0)
    {
        errorcode = ret;
        return;
    }

    for(int x = 0; x < data.at(0).GetSize(); x++)
    {
        buffer.append(data.at(0).GetData()[x]);
    }
}

bool DlmsClient::CheckReleaseResponse(const QByteArray& buffer)
{
    return true;
}

void DlmsClient::CreateDisconnectFrame(QByteArray& buffer)
{
    if (!client)
    {
        return;
    }

    int ret = -1;
    std::vector<ByteBuffer> data;

    buffer.clear();
    ret = client->DisconnectRequest(data);

    if (ret != 0)
    {
        errorcode = ret;
        return;
    }

    for(int x = 0; x < data.at(0).GetSize(); x++)
    {
        buffer.append(data.at(0).GetData()[x]);
    }
}

bool DlmsClient::CheckDisconnectResponse(const QByteArray& buffer)
{
//    ReplyData reply;
//    ByteBuffer bytebuffer;
//    int ret = 0;

//    bytebuffer.Set(buffer.data(), buffer.length());

//    ret = client->GetData(bytebuffer, reply, reply);

//    if (ret != DLMS_ERROR_CODE_OK && ret != DLMS_ERROR_CODE_HARDWARE_FAULT)
//    {
//        return false;
//    }

//    if (ret != 0)
//    {
//        errorcode = ret;
//        return false;
//    }

    return true;
}

void DlmsClient::StartProfileGenericDialog(const QString &obiscode)
{
    DLMSProfileGeneric pg(obiscode.toStdString());
    profileGeneric = pg;
}

void DlmsClient::CreateProfileGenericNumValueRequestFrame(QByteArray& buffer, int index)
{
    int ret = 0;
    std::vector<ByteBuffer> pgData;
    ByteBuffer pgRequest;
    pgData.clear();
    ret = client->Read(&profileGeneric, index, pgData);

    if (ret != 0)
    {
        return;
    }

    pgRequest = pgData.at(0);

    for(int x = 0; x < pgData.at(0).GetSize(); x++)
    {
        buffer.append(pgData.at(0).GetData()[x]);
    }
}

bool DlmsClient::CheckProfileGenericNumValueResponseFrame(const QByteArray& buffer, int index, long &numvalue)
{
    int ret = 0;
    ReplyData reply;
    ByteBuffer bytebuffer;
    DLMS_DATA_TYPE type;

    bytebuffer.Set(buffer.data(), buffer.length());

    ret = client->GetData(bytebuffer, reply, reply);

    if (ret != DLMS_ERROR_CODE_OK && ret != DLMS_ERROR_CODE_HARDWARE_FAULT)
    {
        errorcode = ret;
        return false;
    }

    ret = client->UpdateValue(profileGeneric, index, reply.GetValue());

    if (ret != 0)
    {
        errorcode = ret;
        return false;
    }

    //Update data type.
    if ((ret = profileGeneric.GetDataType(index, type)) != 0)
    {
        errorcode = ret;
        return false;
    }

    if (type == DLMS_DATA_TYPE_NONE)
    {
        type = reply.GetValue().vt;
        if ((ret = profileGeneric.SetDataType(index, type)) != 0)
        {
            errorcode = ret;
            return false;
        }
    }

    numvalue = 0;

    if(index == 4)
    {
        numvalue = profileGeneric.GetCapturePeriod();
    }

    if(index == 8)
    {
        numvalue = profileGeneric.GetProfileEntries();
    }

    return true;
}

void DlmsClient::CreateProfileGenericCapturedObjectsRequestFrame(QByteArray& buffer)
{
    int ret = 0;
    std::vector<ByteBuffer> pgData;
    ByteBuffer pgRequest;
    pgData.clear();

    replyBlockBuffer.Clear();
    ret = client->Read(&profileGeneric, 3, pgData);

    if (ret != 0)
    {
        return;
    }

    pgRequest = pgData.at(0);

    for(int x = 0; x < pgData.at(0).GetSize(); x++)
    {
        buffer.append(pgData.at(0).GetData()[x]);
    }
}

bool DlmsClient::CheckProfileGenericCapturedObjectsResponseFrame(QString &colnames, long &colcount)
{
    int ret = 0;
    DLMS_DATA_TYPE type;
    int cols = 0;

    ret = client->UpdateValue(profileGeneric, 3, replyBlockBuffer.GetValue());

    if (ret != 0)
    {
        errorcode = ret;
        return false;
    }

    //Update data type.
    if ((ret = profileGeneric.GetDataType(3, type)) != 0)
    {
        errorcode = ret;
        return false;
    }

    if (type == DLMS_DATA_TYPE_NONE)
    {
        type = replyBlockBuffer.GetValue().vt;
        if ((ret = profileGeneric.SetDataType(3, type)) != 0)
        {
            errorcode = ret;
            return false;
        }
    }

    std::vector<std::string> capObjvalueList;
    profileGeneric.GetValues(capObjvalueList);
    std::string capObjvalue = capObjvalueList[2];
    cols = profileGeneric.GetCaptureObjects().size();
    colnames = capObjvalue.c_str();
    colcount = cols;

    return true;
}

void DlmsClient::CreateProfileGenericFirstRecordRequestFrame(QByteArray& buffer)
{
    int ret = 0;
    std::vector<ByteBuffer> pgData;
    ByteBuffer pgRequest;
    pgData.clear();
    ret = client->ReadRowsByEntry(&profileGeneric, 1, 1, pgData);

    if (ret != 0)
    {
        errorcode = ret;
        return;
    }

    pgRequest = pgData.at(0);

    for(int x = 0; x < pgData.at(0).GetSize(); x++)
    {
        buffer.append(pgData.at(0).GetData()[x]);
    }
}

bool DlmsClient::CheckProfileGenericFirstRecordResponseFrame(const QByteArray& buffer, long capcol, long &frowid, long &ftstamp)
{
    int ret = 0;
    int rows = 0;
    ReplyData reply;
    ByteBuffer bytebuffer;
    DLMS_DATA_TYPE type;

    bytebuffer.Set(buffer.data(), buffer.length());
    ret = client->GetData(bytebuffer, reply, reply);
    errorcode = ret;

    if (ret != 0)
    {
        return false;
    }

    ret = client->UpdateValue(profileGeneric, 2, reply.GetValue());

    if (ret != 0)
    {
        return false;
    }

    std::vector<std::vector<DLMSVariant>> databuffer;

    databuffer = profileGeneric.GetBuffer();

    rows = databuffer.size();

    if (rows > 0 && capcol >= 2)
    {
        frowid = databuffer[0][0].ToInteger();
        ftstamp = databuffer[0][1].dateTime.ToUnixTime();
    }

    return true;
}

void DlmsClient::CreateProfileGenericBatchRecordsRequestFrame(QByteArray& buffer, int start, int numofrecords)
{
    int ret = 0;
    std::vector<ByteBuffer> pgData;
    ByteBuffer pgRequest;
    pgData.clear();
    ret = client->ReadRowsByEntry(&profileGeneric, start, numofrecords, pgData);

    if (ret != 0)
    {
        errorcode = ret;
        return;
    }

    pgRequest = pgData.at(0);

    for(int x = 0; x < pgData.at(0).GetSize(); x++)
    {
        buffer.append(pgData.at(0).GetData()[x]);
    }
}

bool DlmsClient::CheckProfileGenericBatchRecordsResponseFrame(std::vector<std::vector<DLMSVariant>> &iobuffer)
{
    int ret = 0;
    int rows = 0;
    int cols = 0;
    ByteBuffer bytebuffer;
    DLMS_DATA_TYPE type;

    ret = client->UpdateValue(profileGeneric, 2, replyBlockBuffer.GetValue());

    if (ret != 0)
    {
        errorcode = ret;
        return false;
    }

    iobuffer = profileGeneric.GetBuffer();

    return true;
}

void DlmsClient::StartBatch()
{
    replyBlockBuffer.Clear();
}

void DlmsClient::CreateReceiverReadyFrame(QByteArray& buffer)
{
    int ret = -1;
    ByteBuffer request;

    ret = client->ReceiverReady(replyBlockBuffer.GetMoreData(), request);

    if (ret != 0)
    {
        errorcode = ret;
        return;
    }

    for(int x = 0; x < request.GetSize(); x++)
    {
        buffer.append(request.GetData()[x]);
    }
    return;
}

bool DlmsClient::CheckBatchFrame(const QByteArray &buffer, bool& ispartial, bool &islast)
{
    if (buffer.size() == 0)
    {
        return false;
    }

    //ReplyData reply;
    ByteBuffer bytebuffer;
    int ret = 0;
    ispartial = false;
    islast = false;

    bytebuffer.Set(buffer.data(), buffer.length());

    ret = client->GetData(bytebuffer, replyBlockBuffer);

    if (ret != DLMS_ERROR_CODE_OK && ret != DLMS_ERROR_CODE_HARDWARE_FAULT)
    {
        if(replyBlockBuffer.GetMoreData() != DLMS_DATA_REQUEST_TYPES_FRAME || replyBlockBuffer.GetMoreData() != DLMS_DATA_REQUEST_TYPES_NONE)
        {
            errorcode = ret;
            return false;
        }
    }

    if(replyBlockBuffer.IsMoreData())
    {
        ispartial = true;
    }
    else
    {
        islast = true;
    }

    return true;
}


bool DlmsClient::IsAuthenticationRequired()
{
    return client->IsAuthenticationRequired();
}

int DlmsClient::GetClientId()
{
    return clientAddress;
}

void DlmsClient::ExtractStringData(DLMSVariant &data, QString &strval)
{
    ByteBuffer buffer;
    buffer.Clear();
    std::string str = data.ToString();
    Helpers::HexToBytes(str, buffer);

    if(buffer.GetSize() > data.GetSize())
    {
        buffer.SetUInt8(data.GetSize(),0);
    }

    char* ptr = (char*)calloc(1, data.GetSize()+1);
    memcpy(ptr, (char*)buffer.GetData(), data.GetSize());
    strval = ptr;
    free(ptr);
}

void DlmsClient::ExtractIntegerData(DLMSVariant &data, long &numval)
{
    numval = data.ToInteger();
}

void DlmsClient::ExtractKeys(DLMSVariant& data, DlmsClient* client103, DlmsClient* client104, DlmsClient* client105)
{
    if(data.vt == DLMS_DATA_TYPE_STRUCTURE)
    {
        EDMI_CIU_KEYS.RandomString.Clear();
        EDMI_CIU_KEYS.Client103WrappedKeys.Clear();
        EDMI_CIU_KEYS.Client104WrappedKeys.Clear();
        EDMI_CIU_KEYS.Client105WrappedKeys.Clear();

        data.Arr[0].GetBytes(EDMI_CIU_KEYS.RandomString);
        data.Arr[1].GetBytes(EDMI_CIU_KEYS.Client103WrappedKeys);
        data.Arr[2].GetBytes(EDMI_CIU_KEYS.Client104WrappedKeys);
        data.Arr[3].GetBytes(EDMI_CIU_KEYS.Client105WrappedKeys);

        ByteBuffer unwrappedKey;

        unwrappedKey.Clear();
        client103->UnwrapKeys(EDMI_CIU_KEYS.RandomString.GetData(), EDMI_CIU_KEYS.RandomString.GetSize(), EDMI_CIU_KEYS.Client103WrappedKeys.GetData(), EDMI_CIU_KEYS.Client103WrappedKeys.GetSize(), unwrappedKey);
        memcpy(EDMI_CIU_KEYS.Client103CipherKey, &unwrappedKey.GetData()[0], 16);
        memcpy(EDMI_CIU_KEYS.Client103AuthenticationKey, &unwrappedKey.GetData()[16], 16);
        client103->SetAuthenticationKey(EDMI_CIU_KEYS.Client103AuthenticationKey, 16);
        client103->SetCipheringKey(EDMI_CIU_KEYS.Client103CipherKey, 16);

        unwrappedKey.Clear();
        client104->UnwrapKeys(EDMI_CIU_KEYS.RandomString.GetData(), EDMI_CIU_KEYS.RandomString.GetSize(), EDMI_CIU_KEYS.Client104WrappedKeys.GetData(), EDMI_CIU_KEYS.Client104WrappedKeys.GetSize(), unwrappedKey);
        memcpy(EDMI_CIU_KEYS.Client104CipherKey, &unwrappedKey.GetData()[0], 16);
        memcpy(EDMI_CIU_KEYS.Client104AuthenticationKey, &unwrappedKey.GetData()[16], 16);
        client104->SetAuthenticationKey(EDMI_CIU_KEYS.Client104AuthenticationKey, 16);
        client104->SetCipheringKey(EDMI_CIU_KEYS.Client104CipherKey, 16);

        unwrappedKey.Clear();
        client105->UnwrapKeys(EDMI_CIU_KEYS.RandomString.GetData(), EDMI_CIU_KEYS.RandomString.GetSize(), EDMI_CIU_KEYS.Client105WrappedKeys.GetData(), EDMI_CIU_KEYS.Client105WrappedKeys.GetSize(), unwrappedKey);
        memcpy(EDMI_CIU_KEYS.Client105CipherKey, &unwrappedKey.GetData()[0], 16);
        memcpy(EDMI_CIU_KEYS.Client105AuthenticationKey, &unwrappedKey.GetData()[16], 16);
        client105->SetAuthenticationKey(EDMI_CIU_KEYS.Client105AuthenticationKey, 16);
        client105->SetCipheringKey(EDMI_CIU_KEYS.Client105CipherKey, 16);
    }
}

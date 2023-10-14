#include "DLMSSecureServer.h"

DLMSSecureServer::DLMSSecureServer(bool UseLogicalNameReferencing,
    DLMS_INTERFACE_TYPE intefaceType) :
    DLMSServer(
        UseLogicalNameReferencing,
        intefaceType), m_Cipher("ABCDEFGH")
{
    m_Settings.SetCipher(&m_Cipher);
}

#ifndef DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
DLMSSecureServer::DLMSSecureServer(
    DLMSAssociationLogicalName* ln,
    DLMSIecHdlcSetup* hdlc) :
    DLMSServer(
        ln,
        hdlc), m_Cipher("ABCDEFGH")
{
    m_Settings.SetCipher(&m_Cipher);
}

DLMSSecureServer::DLMSSecureServer(
    DLMSAssociationLogicalName* ln,
    DLMSTcpUdpSetup* wrapper) :
    DLMSServer(
        ln,
        wrapper), m_Cipher("ABCDEFGH")
{
    m_Settings.SetCipher(&m_Cipher);
}
#endif //DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME

#ifndef DLMS_IGNORE_ASSOCIATION_SHORT_NAME
DLMSSecureServer::DLMSSecureServer(
    DLMSAssociationShortName* sn,
    DLMSIecHdlcSetup* hdlc) :
    DLMSServer(
        sn,
        hdlc), m_Cipher("ABCDEFGH")
{
    m_Settings.SetCipher(&m_Cipher);
}

DLMSSecureServer::DLMSSecureServer(
    DLMSAssociationShortName* sn,
    DLMSTcpUdpSetup* wrapper) :
    DLMSServer(
        sn,
        wrapper), m_Cipher("ABCDEFGH")
{
    m_Settings.SetCipher(&m_Cipher);
}

#endif //DLMS_IGNORE_ASSOCIATION_SHORT_NAME

DLMSSecureServer::~DLMSSecureServer()
{
}

Cipher* DLMSSecureServer::GetCiphering()
{
    return m_Settings.GetCipher();
}

ByteBuffer& DLMSSecureServer::GetKek() {
    return GetSettings().GetKek();
}

int DLMSSecureServer::SetKek(ByteBuffer& value) {
    GetSettings().SetKek(value);
    return 0;
}

#include "DLMSContextType.h"
#include "DLMSVariant.h"
#include "Helpers.h"

DLMSContextType::DLMSContextType()
{
    m_Conformance = (DLMS_CONFORMANCE)0;
    m_MaxReceivePduSize = 0;
    m_MaxSendPduSize = 0;
    m_DlmsVersionNumber = 6;
    m_QualityOfService = 0;
}

DLMS_CONFORMANCE DLMSContextType::GetConformance()
{
    return m_Conformance;
}
void DLMSContextType::SetConformance(DLMS_CONFORMANCE value)
{
    m_Conformance = value;
}

int DLMSContextType::GetMaxReceivePduSize()
{
    return m_MaxReceivePduSize;
}
void DLMSContextType::SetMaxReceivePduSize(int value)
{
    m_MaxReceivePduSize = value;
}

int DLMSContextType::GetMaxSendPduSize()
{
    return m_MaxSendPduSize;
}
void DLMSContextType::SetMaxSendPduSize(int value)
{
    m_MaxSendPduSize = value;
}

int DLMSContextType::GetDlmsVersionNumber()
{
    return m_DlmsVersionNumber;
}
void DLMSContextType::SetDlmsVersionNumber(int value)
{
    m_DlmsVersionNumber = value;
}
int DLMSContextType::GetQualityOfService()
{
    return m_QualityOfService;
}
void DLMSContextType::SetQualityOfService(int value)
{
    m_QualityOfService = value;
}

ByteBuffer& DLMSContextType::GetCypheringInfo()
{
    return m_CypheringInfo;
}

void DLMSContextType::SetCypheringInfo(ByteBuffer& value)
{
    m_CypheringInfo = value;
}

std::string DLMSContextType::ToString()
{
    ByteBuffer bb;
    bb.AddIntAsString(m_Conformance);
    bb.SetInt8(' ');
    bb.AddIntAsString(m_MaxReceivePduSize);
    bb.SetInt8(' ');
    bb.AddIntAsString(m_MaxSendPduSize);
    bb.SetInt8(' ');
    bb.AddIntAsString(m_DlmsVersionNumber);
    bb.SetInt8(' ');
    bb.AddIntAsString(m_QualityOfService);
    bb.SetInt8(' ');
    bb.Set(&m_CypheringInfo, 0, -1);
    return bb.ToString();
}

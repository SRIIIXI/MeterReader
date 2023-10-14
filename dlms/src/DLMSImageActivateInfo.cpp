#include "DLMSImageActivateInfo.h"
#include <sstream>

/**
 * Constructor.
 */
DLMSImageActivateInfo::DLMSImageActivateInfo()
{

}

/**
 * Constructor.
 */
DLMSImageActivateInfo::DLMSImageActivateInfo(long size, ByteBuffer& identification, ByteBuffer& signature)
{
    m_Size = size;
    m_Identification = identification;
    m_Signature = signature;
}

/**
 *  Image_size is the size of the Image(s) to be activated.
 *  Expressed in octets;
*/
long DLMSImageActivateInfo::GetSize()
{
    return m_Size;
}
void DLMSImageActivateInfo::SetSize(long value)
{
    m_Size = value;
}

/**
 * Image identification is the identification of the Image(s)
 * to be activated, and may contain information like
 * manufacturer, device type, version information, etc.
*/
ByteBuffer& DLMSImageActivateInfo::GetIdentification()
{
    return m_Identification;
}

void DLMSImageActivateInfo::SetIdentification(ByteBuffer& value)
{
    m_Identification.Clear();
    m_Identification.Set(&value);
}

/**
 * Image signature is the signature of the Image(s) to be activated.
*/
ByteBuffer& DLMSImageActivateInfo::GetSignature()
{
    return m_Signature;
}
void DLMSImageActivateInfo::SetSignature(ByteBuffer& value)
{
    m_Signature.Clear();
    m_Signature.Set(&value);
}

std::string DLMSImageActivateInfo::ToString()
{
    std::stringstream sb;
    sb << m_Identification.ToHexString();
    sb << " ";
    sb << m_Signature.ToHexString();
    sb << " ";
    sb << m_Size;
    return sb.str();
}

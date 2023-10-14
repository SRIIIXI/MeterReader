#ifndef DLMS_IMAGE_ACTIVATE_INFO_H
#define DLMS_IMAGE_ACTIVATE_INFO_H

#include "ByteBuffer.h"

class DLMSImageActivateInfo
{
    long m_Size;
    ByteBuffer m_Identification;
    ByteBuffer m_Signature;
public:
    /**
     * Constructor.
     */
    DLMSImageActivateInfo();

    /**
     * Constructor.
     */
    DLMSImageActivateInfo(long size, ByteBuffer& identification, ByteBuffer& signature);

    /**
     *  Image_size is the size of the Image(s) to be activated.
     *  Expressed in octets;
    */
    long GetSize();
    void SetSize(long value);

    /**
     * Image identification is the identification of the Image(s)
     * to be activated, and may contain information like
     * manufacturer, device type, version information, etc.
    */
    ByteBuffer& GetIdentification();
    void SetIdentification(ByteBuffer& value);

    /**
     * Image signature is the signature of the Image(s) to be activated.
    */
    ByteBuffer& GetSignature();
    void SetSignature(ByteBuffer& value);

    std::string ToString();
};
#endif

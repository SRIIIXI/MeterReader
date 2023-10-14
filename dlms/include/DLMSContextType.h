#ifndef DLMS_CONTEXT_TYPE_H
#define DLMS_CONTEXT_TYPE_H

#include <string>
#include "ByteBuffer.h"
#include "Enums.h"

class DLMSContextType
{
private:
    DLMS_CONFORMANCE m_Conformance;
    int m_MaxReceivePduSize;
    int m_MaxSendPduSize;
    int m_DlmsVersionNumber;
    int m_QualityOfService;
    ByteBuffer m_CypheringInfo;

public:
    /*Constructor.*/
    DLMSContextType();
    /**
    * Server will tell what functionality is available for the client.
    * @return Available functionality.
    */
    DLMS_CONFORMANCE GetConformance();

    /**
    * Server will tell what functionality is available for the client.
    *
    * @param value
    *            Available functionality.
    */
    void SetConformance(DLMS_CONFORMANCE value);

    int GetMaxReceivePduSize();
    void SetMaxReceivePduSize(int value);

    int GetMaxSendPduSize();
    void SetMaxSendPduSize(int value);

    int GetDlmsVersionNumber();
    void SetDlmsVersionNumber(int value);

    int GetQualityOfService();
    void SetQualityOfService(int value);

    ByteBuffer& GetCypheringInfo();
    void SetCypheringInfo(ByteBuffer& value);

    std::string ToString();
};
#endif

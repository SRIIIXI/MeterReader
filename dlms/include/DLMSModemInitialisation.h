#ifndef DLMS_MODEM_INITIALISATION_H
#define DLMS_MODEM_INITIALISATION_H

#include <string>

class DLMSModemInitialisation
{
    std::string m_Request;
    std::string m_Response;
    int m_Delay;

public:
    DLMSModemInitialisation();
    std::string GetRequest();
    void SetRequest(std::string value);
    std::string GetResponse();
    void SetResponse(std::string value);
    int GetDelay();
    void SetDelay(int value);

    std::string ToString();
};
#endif

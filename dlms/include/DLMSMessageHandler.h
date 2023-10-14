#ifndef DLMS_MESSAGE_HANDLER_H
#define DLMS_MESSAGE_HANDLER_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_MESSAGE_HANDLER

#include "DLMSObject.h"
#include "DateTime.h"
#include "DLMSScriptAction.h"

class DLMSMessageHandler : public DLMSObject
{
    std::vector<std::pair<DateTime, DateTime> > m_ListeningWindow;
    std::vector<std::string> m_AllowedSenders;
    std::vector<std::pair<std::string, std::pair<int, DLMSScriptAction> > > m_SendersAndActions;
public:
    //Constructor.
    DLMSMessageHandler();

    //SN Constructor.
    DLMSMessageHandler(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSMessageHandler(std::string ln);

    /**
    Listening Window.
    */
    std::vector<std::pair<DateTime, DateTime> >& GetListeningWindow();

    /**
     List of allowed Senders.
    */
    std::vector<std::string>& GetAllowedSenders();

    /**
     Contains the logical name of a "Script table" object and the script selector of the
     script to be executed if an empty message is received from a match-ing sender.
    */
    std::vector<std::pair<std::string, std::pair<int, DLMSScriptAction> > >& GetSendersAndActions();

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    //Get attribute values of object.
    void GetValues(std::vector<std::string>& values);

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    // Returns value of given attribute.
    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);
};
#endif
#endif

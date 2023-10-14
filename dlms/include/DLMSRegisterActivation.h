#ifndef DLMS_REGISTER_ACTIVATION_H
#define DLMS_REGISTER_ACTIVATION_H

#include "IDLMSBase.h"

#ifndef DLMS_IGNORE_REGISTER_ACTIVATION
#include "Ignore.h"
#include "DLMSObject.h"
#include "Helpers.h"
#include "DLMSObjectDefinition.h"

class DLMSRegisterActivation : public DLMSObject
{
    std::vector<DLMSObjectDefinition> m_RegisterAssignment;
    std::vector<std::pair<ByteBuffer, ByteBuffer > > m_MaskList;
    ByteBuffer m_ActiveMask;

public:
    //Constructor.
    DLMSRegisterActivation();

    //SN Constructor.
    DLMSRegisterActivation(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSRegisterActivation(std::string ln);

    std::vector<DLMSObjectDefinition>& GetRegisterAssignment();

    std::vector<std::pair<ByteBuffer, ByteBuffer > >& GetMaskList();

    ByteBuffer& GetActiveMask();

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    void GetValues(std::vector<std::string>& attributes);

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

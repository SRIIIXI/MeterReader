#ifndef DLMS_OBJECT_FACTORY_H
#define DLMS_OBJECT_FACTORY_H

#include "DLMSObject.h"

class DLMSObjectFactory
{
public:
    static DLMSObject* CreateObject(DLMS_OBJECT_TYPE type);
    static DLMSObject* CreateObject(DLMS_OBJECT_TYPE type, std::string ln);
};
#endif

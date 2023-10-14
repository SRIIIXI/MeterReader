#ifndef DLMS_VALUEEVENT_COLLECTION_H
#define DLMS_VALUEEVENT_COLLECTION_H

#include "DLMSValueEventArg.h"

class DLMSValueEventCollection : public std::vector<DLMSValueEventArg*>
{
public:
    /**
    * Destructor.
    */
    ~DLMSValueEventCollection()
    {
        for(std::vector<DLMSValueEventArg*>::iterator it = begin(); it != end(); ++it)
        {
            delete *it;
        }
        clear();
    }
};
#endif

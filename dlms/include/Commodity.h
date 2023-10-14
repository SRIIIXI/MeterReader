#ifndef COMMODITY_H
#define COMMODITY_H

#include <string>
#include "Enums.h"

class Commodity
{
    DLMS_OBJECT_TYPE m_Type;
    unsigned char m_LogicalName[6];
    unsigned char m_Index;
public:
    // Constructor.
    Commodity();

    /**
   * @return Object type.
   */
    DLMS_OBJECT_TYPE GetType();

    /**
     * @param value
     *            Object type.
     */
    void SetType(DLMS_OBJECT_TYPE value);

    /**
   * @return Attribute index.
   */
    unsigned char* GetLogicalName();

    /**
     * @param value
     *            Attribute index.
     */
    void SetLogicalName(unsigned char* value);

    /**
    * @return Attribute index.
    */
    unsigned char GetIndex();

    /**
     * @param value
     *            Attribute index.
     */
    void SetIndex(unsigned char value);

    std::string ToString();
};
#endif

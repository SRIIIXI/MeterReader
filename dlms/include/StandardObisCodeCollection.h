#ifndef STANDARD_OBIS_CODE_COLLECTION_H
#define STANDARD_OBIS_CODE_COLLECTION_H

#include "ErrorCodes.h"
#include "StandardObisCode.h"
#include "Helpers.h"

/////////////////////////////////////////////////////////////////////////////
// Obis Code Collection is used to save all default OBIS Codes.
/////////////////////////////////////////////////////////////////////////////
class StandardObisCodeCollection : public std::vector<StandardObisCode*>
{
    //Convert logican name std::string to bytes.
    static int GetBytes(std::string ln, unsigned char* bytes);

    // Check is interface included to standard.
    bool EqualsInterface(StandardObisCode item, int ic);

    // Check OBIS codes.
    static bool EqualsMask(std::string& obis, int ic);

    // Check OBIS code.
    static bool EqualsObisCode(std::vector< std::string >& obisMask, unsigned char* ic);

    // Get description.
    std::string GetDescription(std::string& str);

public:
    int Find(std::string ln, DLMS_OBJECT_TYPE objectType, std::vector<StandardObisCode*>& list);

    static bool EqualsMask(std::string& obisMask, std::string& ln);
    static bool EqualsMask2(std::string obisMask, std::string& ln);

    // Find Standard OBIS Code description.
    void Find(unsigned char* pObisCode, int IC, std::vector<StandardObisCode*>& list);
};
#endif

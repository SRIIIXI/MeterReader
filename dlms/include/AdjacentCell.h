#ifndef ADJACENTCELL_H
#define ADJACENTCELL_H

#include <string>

class AdjacentCell
{
private:
    /**
    * Four byte cell ID.
    */
    unsigned long m_CellId;

    /**
    * Signal quality.
    */
    unsigned char m_SignalQuality;
public:
    /**
    * @return Four byte cell ID.
    */
    unsigned long GetCellId();

    /**
    * @param value
    *            Four byte cell ID.
    */
    void SetCellId(unsigned long value);

    /**
    * @return Signal quality.
    */
    unsigned char GetSignalQuality();

    /**
    * @param value
    *            Signal quality.
    */
    void SetSignalQuality(unsigned char value);
};
#endif

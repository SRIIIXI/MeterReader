#ifndef MAC_AVAILABLE_SWITCH_H
#define MAC_AVAILABLES_WITCH_H

#include "ByteBuffer.h"

class MacAvailableSwitch
{
    /*
     * EUI-48 of the subnetwork.
     */
    ByteBuffer m_Sna;

    /*
     * SID of this switch.
     */
    short m_LsId;

    /*
     * Level of this switch in subnetwork hierarchy.
     */
    signed char m_Level;

    /*
     * The received signal level for this switch;
     */
    signed char m_RxLevel;
    /*
     * The signal to noise ratio for this switch.
     */
    signed char m_RxSnr;
public:
    /*
     * Returns EUI-48 of the subnetwork.
     */
    ByteBuffer& GetSna()
    {
        return m_Sna;
    }

    /*
     * value:
     *            EUI-48 of the subnetwork.
     */
    void SetSna(ByteBuffer& value)
    {
        m_Sna = value;
    }

    /*
     * Returns SID of this switch.
     */
    short GetLsId()
    {
        return m_LsId;
    }

    /*
     * value: SID of this switch.
     */
    void SetLsId(short value)
    {
        m_LsId = value;
    }

    /*
     * Returns Level of this switch in subnetwork hierarchy.
     */
    signed char GetLevel()
    {
        return m_Level;
    }

    /*
     * value: Level of this switch in subnetwork hierarchy.
     */
    void SetLevel(signed char value)
    {
        m_Level = value;
    }

    /*
     * Returns The received signal level for this switch;
     */
    signed char GetRxLevel()
    {
        return m_RxLevel;
    }

    /*
     * value: The received signal level for this switch;
     */
    void SetRxLevel(signed char value)
    {
        m_RxLevel = value;
    }

    /*
     * Returns The signal to noise ratio for this switch.
     */
    signed char GetRxSnr()
    {
        return m_RxSnr;
    }

    /*
     * value: The signal to noise ratio for this switch.
     */
    void SetRxSnr(signed char value)
    {
        m_RxSnr = value;
    }

    std::string ToString()
    {
        std::stringstream sb;
        sb << m_Sna.ToHexString();
        sb << ' ';
        sb << m_LsId;
        sb << ' ';
        sb << m_Level;
        sb << ' ';
        sb << m_RxLevel;
        sb << ' ';
        sb << m_RxSnr;
        return sb.str();
    }
};
#endif

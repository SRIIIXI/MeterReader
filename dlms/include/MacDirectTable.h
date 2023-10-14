#ifndef MAC_DIRECT_TABLE_H
#define MAC_DIRECT_TABLE_H

#include <sstream>
#include "ByteBuffer.h"

class MacDirectTable
{
    /*
    * SID of switch through which the source service node is connected.
    */
    short m_SourceSId;

    /*
     * NID allocated to the source service node.
     */
    short m_SourceLnId;
    /*
     * LCID allocated to this connection at the source.
     */
    short m_SourceLcId;

    /*
     * SID of the switch through which the destination service node is
     * connected.
     */
    short m_DestinationSId;
    /*
     * NID allocated to the destination service node.
     */
    short m_DestinationLnId;

    /*
     * LCID allocated to this connection at the destination.
     */
    short m_DestinationLcId;
    /*
     * Entry DID is the EUI-48 of the direct switch.
     */
    ByteBuffer m_Did;
public:
    /*
     * Returns SID of switch through which the source service node is connected.
     */
    short GetSourceSId()
    {
        return m_SourceSId;
    }

    /*
     * value: SID of switch through which the source service node is connected.
     */
    void SetSourceSId(short value)
    {
        m_SourceSId = value;
    }

    /*
     * Returns NID allocated to the source service node.
     */
    short GetSourceLnId()
    {
        return m_SourceLnId;
    }

    /*
     * value: NID allocated to the source service node.
     */
    void SetSourceLnId(short value)
    {
        m_SourceLnId = value;
    }

    /*
     * Returns LCID allocated to this connection at the source.
     */
    short GetSourceLcId()
    {
        return m_SourceLcId;
    }

    /*
     * value: LCID allocated to this connection at the source.
     */
    void SetSourceLcId(short value)
    {
        m_SourceLcId = value;
    }

    /*
     * Returns SID of the switch through which the destination service node is connected.
     */
    short GetDestinationSId()
    {
        return m_DestinationSId;
    }

    /*
     * value: SID of the switch through which the destination service node is connected.
     */
    void SetDestinationSId(short value)
    {
        m_DestinationSId = value;
    }

    /*
     * Returns NID allocated to the destination service node.
     */
    short GetDestinationLnId() {
        return m_DestinationLnId;
    }

    /*
     * value: NID allocated to the destination service node.
     */
    void SetDestinationLnId(short value)
    {
        m_DestinationLnId = value;
    }

    /*
     * Returns LCID allocated to this connection at the destination.
     */
    short GetDestinationLcId()
    {
        return m_DestinationLcId;
    }

    /*
     * value: LCID allocated to this connection at the destination.
     */
    void SetDestinationLcId(short value)
    {
        m_DestinationLcId = value;
    }

    /*
     * Returns Entry DID is the EUI-48 of the direct switch.
     */
    ByteBuffer& GetDid()
    {
        return m_Did;
    }

    /*
     * value: Entry DID is the EUI-48 of the direct switch.
     */
    void SetDid(ByteBuffer& value)
    {
        m_Did = value;
    }

    std::string ToString()
    {
        std::stringstream sb;
        sb << m_SourceSId;
        sb << ' ';
        sb << m_SourceLnId;
        sb << ' ';
        sb << m_SourceLcId;
        sb << ' ';
        sb << m_DestinationSId;
        sb << ' ';
        sb << m_DestinationLnId;
        sb << ' ';
        sb << m_DestinationLcId;
        sb << ' ';
        sb << m_Did.ToHexString();
        return sb.str();
    }
};
#endif

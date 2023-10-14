#ifndef MAC_MULTICAST_ENTRY_H
#define MAC_MULTICAST_ENTRY_H

#include <sstream>
class MacMulticastEntry
{
    /*
    * LCID of multicast group
    */
    char m_Id;
    /*
     * Number of child nodes.
     */
    short m_Members;
public:
    /*
     * Returns LCID of multicast group
     */
    char GetId()
    {
        return m_Id;
    }

    /*
     * value: LCID of multicast group
     */
    void SetId(char value)
    {
        m_Id = value;
    }

    /*
     * Returns Number of child nodes.
     */
    short GetMembers()
    {
        return m_Members;
    }

    /*
     * value: Number of child nodes.
     */
    void SetMembers(short value)
    {
        m_Members = value;
    }

    std::string ToString()
    {
        std::stringstream sb;
        sb << m_Id;
        sb << ' ';
        sb << m_Members;
        return sb.str();
    }

};
#endif

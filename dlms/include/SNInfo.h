#ifndef SN_INFO_H
#define SN_INFO_H

class DLMSObject;

class SNInfo
{
private:
    /**
     * Is attribute index or action index
     */
    bool m_Action;

    /**
     * Attribute index.
     */
    int m_Index;

    /**
     * COSEM object.
     */
    DLMSObject* m_Item;

public:
    /**
     * Constructor.
     */
    SNInfo();
    /**
     * @return The index
     */
    int GetIndex();

    /**
     * @param value
     *            The index to set
     */
    void SetIndex(int value);

    /**
     * @return Is action.
     */
    bool IsAction();

    /**
     * @param value
     *            Is action.
     */
    void SetAction(bool value);

    /**
     * @return The item
     */
    DLMSObject* GetItem();

    /**
     * @param value
     *            The item to set
     */
    void SetItem(DLMSObject* value);
};
#endif

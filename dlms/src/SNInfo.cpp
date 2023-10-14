#include "SNInfo.h"

SNInfo::SNInfo()
{
    m_Action = false;
    m_Index = 0;
    m_Item = 0;
}

int SNInfo::GetIndex()
{
    return m_Index;
}

void SNInfo::SetIndex(int value)
{
    m_Index = value;
}

bool SNInfo::IsAction()
{
    return m_Action;
}

void SNInfo::SetAction(bool value)
{
    m_Action = value;
}

DLMSObject* SNInfo::GetItem()
{
    return m_Item;
}

void SNInfo::SetItem(DLMSObject* value)
{
    m_Item = value;
}

#include "stdafx.h"
#include "WSTreeItem.h"


CWSTreeItem::CWSTreeItem(ETreeType eTreeType, CTime cTime, EClass eClass)
{
m_eTreeType=eTreeType;
m_cTime=cTime;
m_eClass=eClass;
}



CWSTreeItem::~CWSTreeItem(void)
{
}

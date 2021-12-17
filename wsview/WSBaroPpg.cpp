//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// WSBaroPpg.cpp : Implementation of the CWSBaroPropPage property page class.

#include "stdafx.h"
#include "WSView.h"
#include "WSBaroPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CWSBaroPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CWSBaroPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CWSBaroPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CWSBaroPropPage, "WSBARO.WSBaroPropPage.1",
	0x49134337, 0x1ce2, 0x11d4, 0xb1, 0xd2, 0x54, 0xf8, 0x5e, 0, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// CWSBaroPropPage::CWSBaroPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CWSBaroPropPage

BOOL CWSBaroPropPage::CWSBaroPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_WSBARO_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CWSBaroPropPage::CWSBaroPropPage - Constructor

CWSBaroPropPage::CWSBaroPropPage() :
	COlePropertyPage(IDD, IDS_WSBARO_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CWSBaroPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CWSBaroPropPage::DoDataExchange - Moves data between page and properties

void CWSBaroPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CWSBaroPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CWSBaroPropPage message handlers


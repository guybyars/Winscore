//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// GatePropPage.cpp : implementation file
//

#include "stdafx.h"
#include "wsview.h"
#include "GatePropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGatePropPage dialog

IMPLEMENT_DYNCREATE(CGatePropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CGatePropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CGatePropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

// {3AC29253-2234-4785-92D4-A90A9AE717AE}
IMPLEMENT_OLECREATE_EX(CGatePropPage, "WSView.CGatePropPage",
	0x3ac29253, 0x2234, 0x4785, 0x92, 0xd4, 0xa9, 0xa, 0x9a, 0xe7, 0x17, 0xae)


/////////////////////////////////////////////////////////////////////////////
// CGatePropPage::CGatePropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CGatePropPage

BOOL CGatePropPage::CGatePropPageFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Define string resource for page type; replace '0' below with ID.

	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_WSVIEW_GATE_PPG_CAPTION);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CGatePropPage::CGatePropPage - Constructor

// TODO: Define string resource for page caption; replace '0' below with ID.

CGatePropPage::CGatePropPage() : 
	COlePropertyPage(IDD, IDS_WSVIEW_GATE_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CGatePropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CGatePropPage::DoDataExchange - Moves data between page and properties

void CGatePropPage::DoDataExchange(CDataExchange* pDX)
{
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//{{AFX_DATA_MAP(CGatePropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CGatePropPage message handlers


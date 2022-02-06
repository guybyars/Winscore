//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// WinscoreDlgBar.cpp: implementation of the CWinscoreDlgBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "WinscoreDoc.h"
#include "WinscoreDlgBar.h"
#include "Global_Prototypes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinscoreDlgBar::CWinscoreDlgBar()
{

}

CWinscoreDlgBar::~CWinscoreDlgBar()
	{

	}

BOOL CWinscoreDlgBar::Create(CWnd* pParentWnd)
	{
	if (!CPaneDialog::Create (_T("DialogBar"), pParentWnd, FALSE, 
							 MAKEINTRESOURCE (IDD_CONTROL_BAR), 
							 WS_CHILD | WS_VISIBLE | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_TOP, 
							 ID_VIEW_DLGBAR))
	{
		TRACE0("Failed to create Dialog Bar\n");
		return FALSE;      // fail to create
	}

	CComboBox* pcClassComboBox=(CComboBox*)GetDlgItem(IDC_CONTROL_BAR_CLASS);
	CComboBox* pcDayComboBox=  (CComboBox*)GetDlgItem(IDC_CONTROL_BAR_DAY);
	CComboBox* pcViewComboBox= (CComboBox*)GetDlgItem(IDC_CONTROL_BAR_VIEW);

		//  Setup the view menu picks
	for( int i=0; i<NVIEWS; i++ ) 
		pcViewComboBox->AddString(_gszViewNames[i]);
	SetView(eContestantView);

	return TRUE;

	}

EViews CWinscoreDlgBar::GetView()
{	
	if( this->m_hWnd==NULL ) 
		{
		return eContestantView;
		}

	CComboBox* pcViewComboBox= (CComboBox*)GetDlgItem(IDC_CONTROL_BAR_VIEW);
	if( !pcViewComboBox ) return eContestantView;
	int iSel=pcViewComboBox->GetCurSel();

	return (EViews)iSel;
}

void CWinscoreDlgBar::SetView(EViews eView)
{
	if(!::IsWindow(m_hWnd)) return;

	CComboBox* pcViewComboBox= (CComboBox*)GetDlgItem(IDC_CONTROL_BAR_VIEW);
	CComboBox* pcDayComboBox=  (CComboBox*)GetDlgItem(IDC_CONTROL_BAR_DAY);
	CComboBox* pcClassComboBox=(CComboBox*)GetDlgItem(IDC_CONTROL_BAR_CLASS);

	CStatic* pcClassText=(CStatic*)GetDlgItem(IDC_CONTROL_BAR_CLASS_TEXT);
	CStatic* pcDayText=  (CStatic*)GetDlgItem(IDC_CONTROL_BAR_DAY_TEXT);
	CStatic* pcViewText= (CStatic*)GetDlgItem(IDC_CONTROL_BAR_VIEW_TEXT);

	pcViewComboBox->SetCurSel(eView);

	pcViewText->EnableWindow(TRUE);
	pcViewComboBox->EnableWindow(TRUE);
	pcDayText->EnableWindow(TRUE);
	pcDayComboBox->EnableWindow(TRUE);

	if( eView==eContestantView  ||
		eView==eTurnpointView	||
		eView==eTurnpointGridView			)
		{
		pcClassText->EnableWindow(FALSE);	
		pcClassComboBox->EnableWindow(FALSE);
		}
//	else if(eView==eTimesView )
//		{
//		pcClassText->EnableWindow(FALSE);	
//		pcClassComboBox->EnableWindow(FALSE);
//		}
	else if(eView==eFlightLogView )
		{
		pcClassText->EnableWindow(TRUE);	
		pcClassComboBox->EnableWindow(TRUE);
		}
	else if(eView==eTaskView )
		{
		pcClassText->EnableWindow(FALSE);	
		pcClassComboBox->EnableWindow(FALSE);
		}
	else if(/*eView==eLandingCardView  ||*/
			eView==eScoreView			)
		{
		pcClassText->EnableWindow(TRUE);	
		pcClassComboBox->EnableWindow(TRUE);
		}
	else if(eView==eTakeoffGridView  )
		{
		pcClassText->EnableWindow(TRUE);	
		pcClassComboBox->EnableWindow(TRUE);
		}
//	else if(eView==eInvalidLegView  )
//		{
//		pcClassText->EnableWindow(FALSE);	
//		pcClassComboBox->EnableWindow(FALSE);
//		}
	}

void CWinscoreDlgBar::LoadDayCombo(CWinscoreDoc * pDoc)
{
	if(!::IsWindow(m_hWnd))return;
	CComboBox* pcDayComboBox=  (CComboBox*)GetDlgItem(IDC_CONTROL_BAR_DAY);
	int iSel=pcDayComboBox->GetCurSel();
	FreeDateComboBox( *pcDayComboBox);
	pDoc->LoadDateComboBox( *pcDayComboBox );
	if( iSel>=0 ) pcDayComboBox->SetCurSel(iSel);

}

void CWinscoreDlgBar::LoadClassCombo(CWinscoreDoc * pDoc)
{
	if(!::IsWindow(m_hWnd))return;
	CComboBox* pcDayComboBox=  (CComboBox*)GetDlgItem(IDC_CONTROL_BAR_CLASS);
	pDoc->SetAvailableClasses( pcDayComboBox );
}

CTime CWinscoreDlgBar::GetDayCombo()
	{
	if(!::IsWindow(m_hWnd))return 0;
	CComboBox* pcDayComboBox=  (CComboBox*)GetDlgItem(IDC_CONTROL_BAR_DAY);

	int iSel=pcDayComboBox->GetCurSel();
	if( iSel== -1 ) return 0;

	CTime *pcTime=(CTime*)(pcDayComboBox->GetItemDataPtr(iSel));

	return *pcTime;

	}

EClass CWinscoreDlgBar::GetClassCombo()
	{
	if(!::IsWindow(m_hWnd))return eStandard;
	CComboBox* pcClassComboBox=  (CComboBox*)GetDlgItem(IDC_CONTROL_BAR_CLASS);

	int iSel=pcClassComboBox->GetCurSel();

	EClass eClass=(EClass)(pcClassComboBox->GetItemData(iSel));

	return eClass;

	}



void CWinscoreDlgBar::SetClass(EClass eClass)
{
	if(!::IsWindow(m_hWnd))return;
	CComboBox* pcClassComboBox=  (CComboBox*)GetDlgItem(IDC_CONTROL_BAR_CLASS);

	SetDefaultClass(eClass);
	int nClasses=pcClassComboBox->GetCount();
	for( int i=0; i<nClasses; i++ )
		{
		if( (EClass)pcClassComboBox->GetItemData(i)==eClass)
			{
			pcClassComboBox->SetCurSel(i);
			return;
			}
		}
		pcClassComboBox->SetCurSel(0);
}

void CWinscoreDlgBar::SetDate(CTime cDate)
	{
	if(!::IsWindow(m_hWnd))return;
	CTime *pcTime;
	CComboBox* pcDayComboBox=  (CComboBox*)GetDlgItem(IDC_CONTROL_BAR_DAY);

	int nDays=pcDayComboBox->GetCount();
	for( int i=0; i<nDays; i++ )
		{ 
		pcTime=(CTime*)(pcDayComboBox->GetItemDataPtr(i));

		if( DatesEqual(*pcTime,cDate) )
			{
			pcDayComboBox->SetCurSel(i);
			SetDefaultDay(cDate);
			return;
			}
		}
		pcDayComboBox->SetCurSel(0);
	}

void CWinscoreDlgBar::GreyAll()
{
	if(!::IsWindow(m_hWnd))return;
	CComboBox* pcViewComboBox= (CComboBox*)GetDlgItem(IDC_CONTROL_BAR_VIEW);
	CComboBox* pcDayComboBox=  (CComboBox*)GetDlgItem(IDC_CONTROL_BAR_DAY);
	CComboBox* pcClassComboBox=(CComboBox*)GetDlgItem(IDC_CONTROL_BAR_CLASS);

	CStatic* pcClassText=(CStatic*)GetDlgItem(IDC_CONTROL_BAR_CLASS_TEXT);
	CStatic* pcDayText=  (CStatic*)GetDlgItem(IDC_CONTROL_BAR_DAY_TEXT);
	CStatic* pcViewText= (CStatic*)GetDlgItem(IDC_CONTROL_BAR_VIEW_TEXT);

	pcViewText->EnableWindow(FALSE);
	pcViewComboBox->EnableWindow(FALSE);
	pcDayText->EnableWindow(FALSE);
	pcDayComboBox->EnableWindow(FALSE);
	pcClassText->EnableWindow(FALSE);	
	pcClassComboBox->EnableWindow(FALSE);

}


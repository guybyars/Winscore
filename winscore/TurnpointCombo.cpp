//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TurnpointCombo.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "TurnpointCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTurnpointCombo

CTurnpointCombo::CTurnpointCombo()
{
}

CTurnpointCombo::~CTurnpointCombo()
{
}


BEGIN_MESSAGE_MAP(CTurnpointCombo, CComboBox)
	//{{AFX_MSG_MAP(CTurnpointCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTurnpointCombo message handlers

CTurnpoint* CTurnpointCombo::GetCurSel()
{
	int iSel=CComboBox::GetCurSel();
	if( iSel<0 ) return NULL;
	return (CTurnpoint*)GetItemDataPtr( iSel);
}

void CTurnpointCombo::SetCurSel(int iID)
{
	if( iID==-1 ) 
		{
		CComboBox::SetCurSel(-1);
		CComboBox::Clear();
		return;
		}
	int nSel=GetCount();
	CTurnpoint* pcTurnpoint;
	for( int i=0; i<nSel; i++ )
		{
		pcTurnpoint = (CTurnpoint*)GetItemDataPtr( i);
		if( pcTurnpoint )
			{
			if( iID==pcTurnpoint->m_iID )
				{
				CComboBox::SetCurSel(i);
				return;
				}
			}

		}
		CComboBox::SetCurSel(0);

}

	int CTurnpointCombo::GetNumSel()
		{
		nSel=0;
		int iSel=CComboBox::GetCurSel();
		if( iSel>=0) 
			{
			nSel=1;
			return 1;
			}
		CString cText;
		GetWindowText(cText);
		int iLength=cText.GetLength();
		if( iLength==0 )
			{
			return 0;
			}
		for( int iChar=0; iChar<iLength; iChar++)
			{
			if( cText[iChar]==',' ) cText.SetAt(iChar,' ');
			}

		int aiSelIDs[12];
		int nScan=sscanf_s(cText,_T("%d%d%d%d%d%d%d%d%d%d%d%d"),
								&aiSelIDs[0], 
								&aiSelIDs[1],
								&aiSelIDs[2],
								&aiSelIDs[3],
								&aiSelIDs[4],
								&aiSelIDs[5],
								&aiSelIDs[6],
								&aiSelIDs[7],
								&aiSelIDs[8],
								&aiSelIDs[9],
								&aiSelIDs[10],
								&aiSelIDs[11]
								);
		if( nScan==0)
			{
			return nSel;
			}

		int nList=GetCount();
		CTurnpoint* pcTurnpoint;

		for( int iScan=0; iScan<nScan; iScan++)
			{
			for( int i=0; i<nList; i++ )
				{
				pcTurnpoint = (CTurnpoint*)GetItemDataPtr( i);
				if( pcTurnpoint &&  aiSelIDs[iScan]==pcTurnpoint->m_iID )
					{
					m_apSelTpts[nSel]=pcTurnpoint;
					nSel++;
					break;
					}
				}
			}
		return nSel;
		}

	CTurnpoint*  CTurnpointCombo::GetSelAt(int i)
		{
		int iSel=CComboBox::GetCurSel();
		if( iSel>=0 )
			{
			return (CTurnpoint*)GetItemDataPtr( iSel);
			}
		return m_apSelTpts[i];
		}





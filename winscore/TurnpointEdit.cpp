//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TurnpointEdit.cpp : implementation file
//

#include "stdafx.h"
//#include "winscore.h"
#include "TurnpointEdit.h"
#include <math.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTurnpointEdit

CTurnpointEdit::CTurnpointEdit()
{
}

CTurnpointEdit::~CTurnpointEdit()
{
}


int CTurnpointEdit::ParseTurnpoints( int MaxTurnpoints, int aiTurnpoints[] )
	{
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
	if( nScan==0) return 0;
	
	if( nScan>MaxTurnpoints ) 
		nScan=MaxTurnpoints;

	for( int iScan=0; iScan<nScan; iScan++)
			{
			aiTurnpoints[iScan]=aiSelIDs[iScan];
			}

	return nScan;
	
	}


int CTurnpointEdit::ParseTurnpointRadii( int MaxTurnpoints, double adTurnpoints[] )
	{
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

	float afSelIDs[12];
	int nScan=sscanf_s(cText,_T("%f%f%f%f%f%f%f%f%f%f%f%f"),
								&afSelIDs[0], 
								&afSelIDs[1],
								&afSelIDs[2],
								&afSelIDs[3],
								&afSelIDs[4],
								&afSelIDs[5],
								&afSelIDs[6],
								&afSelIDs[7],
								&afSelIDs[8],
								&afSelIDs[9],
								&afSelIDs[10],
								&afSelIDs[11]
								);
	if( nScan==0) return 0;
	
	if( nScan>MaxTurnpoints ) 
		nScan=MaxTurnpoints;

	for( int iScan=0; iScan<nScan; iScan++)
			{
			adTurnpoints[iScan]=afSelIDs[iScan];
			}

	return nScan;
	
	}

BEGIN_MESSAGE_MAP(CTurnpointEdit, CEdit)
	//{{AFX_MSG_MAP(CTurnpointEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTurnpointEdit message handlers

void CTurnpointEdit::AddTurnpoint(int iID)
{
	CString cText, cIDText;
	GetWindowText(cText);
	if( cText.GetLength()>0 )
		cIDText.Format(_T(",%d"), iID);
	else
		cIDText.Format(_T("%d"), iID);
	cText+=cIDText;
	SetWindowText(cText);
}

void CTurnpointEdit::AddTurnpoint(double dID)
{
	CString cText, cIDText;
	double dRemainder=dID-floor(dID);
	bool bDecmil= dRemainder<.9 && dRemainder>.1;

	GetWindowText(cText);
	if( cText.GetLength()>0 )
		!bDecmil?cIDText.Format(_T(",%3.0lf"), dID):cIDText.Format(_T(", %3.1lf"), dID);
	else
		!bDecmil?cIDText.Format(_T("%3.0lf"), dID):cIDText.Format(_T("%3.1lf"), dID);
	cText+=cIDText;
	SetWindowText(cText);
}
void CTurnpointEdit::Clear()
{
	SetWindowText(_T(""));
}


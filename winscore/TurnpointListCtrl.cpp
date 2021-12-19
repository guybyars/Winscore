//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TurnpointListCtrl.cpp: implementation of the CTurnpointListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "TurnpointListCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTurnpointListCtrl::CTurnpointListCtrl()
	{
	m_pcTurnpointArray=NULL;
	m_bShowFinish=true;
	m_bRadiusDisplay=false;
	m_bFixDisplay=false;
	for( int iFix=0; iFix<MAXTASKTPS; iFix++) m_iFixIDS[iFix]=0;

	}

CTurnpointListCtrl::~CTurnpointListCtrl()
{

}

int CTurnpointListCtrl::GetCurSel()
{
	return GetNextItem( -1, LVNI_SELECTED );
}

void CTurnpointListCtrl::AddTurnpoint(int iID, int iAttribute, double dRadius )
{
	CTurnpoint *pcTurnpoint=m_pcTurnpointArray->GetAt(iID-1);
	if( !pcTurnpoint ) return;

	if( pcTurnpoint->m_iAttribute & iAttribute )
		{
		InsertTurnpoint(pcTurnpoint, dRadius);
		}
}

void CTurnpointListCtrl::InsertTurnpoint(CTurnpoint * pcTurnpoint, double dRadius  )
{
	ASSERT( pcTurnpoint);
	if( !pcTurnpoint ) return;

	int iItem = GetCurSel();
	if( iItem <0 ) iItem=GetItemCount();

	LV_ITEM lvi;

	lvi.iSubItem = 0;
	lvi.mask = 0;

	lvi.iItem = iItem;
	int iNewItem=InsertItem(&lvi);

	if( m_bCheckBoxes )
		ListView_SetCheckState(m_hWnd, iNewItem, 1);

	int iPos=0;
	SetItemText(iItem,iPos++,_T(" "));
	SetItemText(iItem,iPos++,pcTurnpoint->GetNoNameText());
	if( m_bRadiusDisplay )
		{
		CString strText;
		if(dRadius>.0001 ) strText.Format(_T("%5.1lf"),ConvertDistance(dRadius, SYSTEMUNITS, m_eUnits) );
		SetItemText(iItem,iPos++,strText );
		}
	SetItemText(iItem,iPos++,_T("<error>"));
	SetItemText(iItem,iPos++,_T(""));

	SetItemData(iItem, (LPARAM)pcTurnpoint);
	UpdateDistance();

	EnsureVisible( iItem, FALSE);
}

void CTurnpointListCtrl::DeleteSelected()
{
  int iSel= GetCurSel();
  if( iSel>= 0 ) DeleteItem( iSel );
  UpdateDistance();
}


double CTurnpointListCtrl::Distance(EUnits eUnits)
{

	double  dDist=0.0;

	CTurnpoint *pcTurnpoint, *pcPrevTurnpoint;

	int nSel=GetItemCount();
	for( int i=1; i<nSel; i++ )
		{
		if( m_bCheckBoxes &&  !ListView_GetCheckState(m_hWnd, i) ) 
			break;
		
		pcPrevTurnpoint=(CTurnpoint*)GetItemData(i-1);
		pcTurnpoint=(CTurnpoint*)GetItemData(i);
		dDist+=pcPrevTurnpoint->DistanceTo(pcTurnpoint, eUnits);

		if(m_cStartGate.IsGPSCylinder() )
			{
			if( i==1 && pcPrevTurnpoint->m_iAttribute & TPT_START )
				dDist-=ConvertDistance( m_cStartGate.GetRadius(), SYSTEMUNITS, eUnits);
			}
		if(m_bShowFinish && m_cFinishGate.IsGPSCylinder() )
			{
			if( i==nSel-1 && pcTurnpoint->m_iAttribute & TPT_FINISH )
				dDist-=ConvertDistance( m_cFinishGate.GetRadius(), SYSTEMUNITS, eUnits);
			}


		}
	return dDist;		
}

void CTurnpointListCtrl::UpdateDistance()
{

	double  dDist=0.0;
	TCHAR	buf[10];
	CString cDistString, cFDRString, cNoString;
	CLocation *pcTurnpoint, *pcPrevTurnpoint;
	int i=0;

	int nSel=GetItemCount();
	for( i=0; i<nSel; i++ )
		{
		bool m_bStart= i==0;
		bool m_bFinish=m_bShowFinish && i==nSel-1;

		if( m_bCheckBoxes &&  !ListView_GetCheckState(m_hWnd, i) ) 
			cDistString=_T("");
		else
			{

			if( i>0 )
				{
				if( m_bFixDisplay )
					{
					if( i==1 )
						{
						pcPrevTurnpoint=(CLocation*)GetItemData(i-1);
						pcTurnpoint= &m_cFixLocations[i-1];
						}
					else if( m_bFinish )
						{
						pcPrevTurnpoint= &m_cFixLocations[i-2];
						pcTurnpoint=(CLocation*)GetItemData(i);
						}
					else
						{
						pcPrevTurnpoint= &m_cFixLocations[i-2];
						pcTurnpoint= &m_cFixLocations[i-1];
						}
					}
				else
					{
					pcPrevTurnpoint=(CLocation*)GetItemData(i-1);
					pcTurnpoint=(CLocation*)GetItemData(i);
					}
				dDist+=pcPrevTurnpoint->DistanceTo(pcTurnpoint, m_eUnits);
				cFDRString=_T("");

				if(m_cStartGate.IsGPSCylinder() && i==1 )
					{
					dDist-=ConvertDistance(m_cStartGate.GetRadius(), SYSTEMUNITS, m_eUnits);
					cFDRString=m_cStartGate.GetText(m_eUnits);
					}
				else  if(m_bFinish && m_cFinishGate.IsGPSCylinder() )
					{
					dDist-=ConvertDistance(m_cFinishGate.GetRadius(), SYSTEMUNITS, m_eUnits);
					cFDRString=m_cFinishGate.GetText(m_eUnits);
					}

				cDistString.Format(_T("%5.2lf %s"), max(dDist,0.0), cFDRString );


				}
			else
				cDistString.Format(_T("%5.2lf "), 0.00);
			}

		SetItemText( i, m_bRadiusDisplay?(3):(2), cDistString );
		if( m_bStart )
			cNoString=_T("Start");
		else if( m_bFinish )
			cNoString=_T("Finish");
		else
			cNoString.Format(_T("%d"),i);
		SetItemText( i, 0,  cNoString );

		if( !m_bStart && !m_bFinish && m_bFixDisplay && m_iFixIDS[i-1]>0 )
			{
			_itoa_s(m_iFixIDS[i-1],buf,10,10);
			SetItemText( i, 4, buf );
			}


		}		
}


int CTurnpointListCtrl::GetTurnpointID(int i)
{
	CTurnpoint *pcTurnpoint=(CTurnpoint*)GetItemData(i+1);
	if( !pcTurnpoint ) return 0;
	return pcTurnpoint->m_iID;	
}

float CTurnpointListCtrl::GetTurnpointRadius(int i)
{
	CString strRad=GetItemText(i+1, 2);
	return (float)atof(strRad);	
}

void CTurnpointListCtrl::GetTurnpointRadii(int aiTurnpointRadii[ ])
{
	int nItems=GetItemCount();
	for( int i=1; i<nItems-1; i++ )
		{
		aiTurnpointRadii[i]=(int)GetTurnpointRadius(i);
		}
}


void CTurnpointListCtrl::GetTurnpointIDs(int aiTurnpoints [ ])
{

	CTurnpoint *pcTurnpoint;

	int nItems=GetItemCount();
	for( int i=1; i<nItems-1; i++ )
		{
		pcTurnpoint=(CTurnpoint*)GetItemData(i);
		if( pcTurnpoint ) 
			aiTurnpoints[i]=pcTurnpoint->m_iID;
		else
			aiTurnpoints[i]=0;

		}
}





void CTurnpointListCtrl::ResetContent()
{
	DeleteAllItems( );
}


#define NUM_COLUMNS 3

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
{
	_T(" "), _T("Control Point"),_T("Distance")
};

static _TCHAR *_gszColumnLabelRadius[NUM_COLUMNS+1] =
{
	_T(" "), _T("Control Point"),_T("Radius"),_T("Distance")
};

static _TCHAR *_gszColumnLabelRadiusFix[NUM_COLUMNS+2] =
{
	_T(" "), _T("Control Point"),_T("Radius"),_T("Distance"),_T("TAT Fix")
};

static int _gnColumnWidth[NUM_COLUMNS] = 
{
	40, 150, 120
};

static int _gnColumnWidthRadius[NUM_COLUMNS+2] = 
{
	40, 150, 80, 100, 100
};



void CTurnpointListCtrl::Initalize(EUnits eUnits, CString cUnitsText, CTurnpointArray *pcTurnpointArray, bool bCheckBoxes, bool bRadiusDisplay, bool bFixDisplay)
{


	m_pcTurnpointArray=pcTurnpointArray;
	m_eUnits=eUnits;
	m_strUnits=cUnitsText;
	m_bCheckBoxes=bCheckBoxes;
	m_bRadiusDisplay=bRadiusDisplay;
	m_bFixDisplay=bFixDisplay;

	if( bCheckBoxes ) SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

	LV_COLUMN lvc;
	TCHAR cBuff[256];

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	int nColumns=NUM_COLUMNS;
	if( m_bRadiusDisplay ) nColumns++;
	if( m_bFixDisplay ) nColumns++;

	for(  int i = 0; i<nColumns; i++)
	{
		CString  cHeader;
		lvc.iSubItem = i;
		if( m_bFixDisplay )
			cHeader=_gszColumnLabelRadiusFix[i];
		else if( m_bRadiusDisplay )
			cHeader=_gszColumnLabelRadius[i];
		else
			cHeader=_gszColumnLabel[i];

		if( (m_bRadiusDisplay && (i==3||i==2)) ||
			i==2 )
			{
			cHeader+=_T(" (");
			cHeader+=cUnitsText;
			cHeader+=_T(")");
			}
		strncpy_s( cBuff, 256, cHeader, 255);
		lvc.pszText = cBuff;
		lvc.cx =  m_bRadiusDisplay?(_gnColumnWidthRadius[i]):(_gnColumnWidth[i]);;
		lvc.fmt =LVCFMT_LEFT;
		InsertColumn(i,&lvc);
	}

}

CTurnpoint* CTurnpointListCtrl::LastAcheived()
{
	int nSel=GetItemCount();
	if( nSel<=0 ) return NULL;
	if( !m_bCheckBoxes)
		{
		int iSel=nSel-1;
		CTurnpoint* pcTurnpoint=(CTurnpoint*)GetItemData(iSel);
		return pcTurnpoint;
		}
	else
		{
		for( int i=1; i<nSel; i++ )
			{
			if( !ListView_GetCheckState(m_hWnd, i) ) 
				{
				CTurnpoint* pcTurnpoint=(CTurnpoint*)GetItemData(i-1);
				return pcTurnpoint;
				}
			}
		}

	return NULL;
}


int CTurnpointListCtrl::GetNumTurnpoints()
	{
	int iHighestChecked=0;
	if( m_bCheckBoxes )
		{
		for( int i=1; i<GetItemCount()-1; i++ )
			{
			if( ListView_GetCheckState(m_hWnd, i) )
				{
				iHighestChecked=i;
				}
			else
				return iHighestChecked;
			}
 		return iHighestChecked;
		}
	else
		{
		if( m_bShowFinish )
			return max( GetItemCount()-2, 0 );
		else
			return max( GetItemCount()-1, 0);
		}
	return 0;

	}

void CTurnpointListCtrl::ShowFinish(bool bShow)
	{
	m_bShowFinish=bShow;
	}


void CTurnpointListCtrl::SetFix(int iTP, int iFixID, CLocation &cFixLocation)
{
	m_cFixLocations[iTP]=cFixLocation;
	m_iFixIDS[iTP]=iFixID;
}

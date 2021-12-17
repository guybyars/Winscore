//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TurnpointArray.cpp: implementation of the CTurnpointArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "TurnpointArray.h"
#include "WinscoreFonts.h"
#include "Global_Prototypes.h"
#include <fstream>
#include <atlbase.h>

#define PAGE_WIDTH    800
#define NUM_TURNPOINTS_PER_PAGE  43

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTurnpointArray::CTurnpointArray()
{
	for ( int i=0; i<MAX_TURNPOINTS; i++)
		{   
		this->SetAtGrow(i, NULL);
		}
}

CTurnpointArray::~CTurnpointArray()
	{
	Purge();
	}

CTurnpointArray& CTurnpointArray::operator =(CTurnpointArray &CInputTurnpointArray)
{
//  Clean out the destination  
	CTurnpoint* pTurnpointItem;
	int i;
	for ( i=0; i<GetSize(); i++)
		{   
		if( i>CInputTurnpointArray.GetSize()-1 ) break;

		RemoveAt(i);
		pTurnpointItem=CInputTurnpointArray[i];
		if( pTurnpointItem!=NULL)
			{
			CTurnpoint* pnewTurnpoint = 
				new CTurnpoint(	pTurnpointItem->m_iID,
								pTurnpointItem->m_strName,
								pTurnpointItem,
								pTurnpointItem->m_iAttribute,
								pTurnpointItem->m_iElevation);
			this->SetAt(i, pnewTurnpoint);
			}
		}

return *this;

}

void CTurnpointArray::SetAt(int i, CTurnpoint *pcTurnpoint)
{
CObArray::SetAt(i, pcTurnpoint);
}

CTurnpoint* CTurnpointArray::GetAt(int i)
{
	if( i<0 || i>GetSize()-1 ) return NULL;
	return (CTurnpoint*)CObArray::GetAt(i);
}

void CTurnpointArray::RemoveAt(int i)
	{
	//  Delete any entry at this position and ensure it contains
	//  a NULL pointer

	CTurnpoint* pcTurnpointItem = GetAt(i);
    if( pcTurnpointItem!=NULL) 
		{
		delete pcTurnpointItem;
		SetAt(i, NULL);
		}
	}

CTurnpoint* CTurnpointArray::operator [ ](int i)
	{

	if( i<0 || i>GetSize()-1 ) return NULL;
	return GetAt(i);
	}


void CTurnpointArray::LoadTurnpointList(CListCtrl& ListCtrl, ECoordFormat eCoordFormat, bool bControlPointsOnly)
{
	CTurnpoint* pcTurnpoint;
	int iItem=0;

	LV_ITEM lvi;

	lvi.iSubItem = 0;
	lvi.mask = 0;

	ListCtrl.DeleteAllItems( );

	for ( int i=0; i<GetSize(); i++)
		{ 
	    pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( pcTurnpoint==NULL) continue;
		if( bControlPointsOnly && !(pcTurnpoint->m_iAttribute & TPT_CONTROL) ) continue;
		lvi.iItem = iItem;
		lvi.lParam = (LPARAM)pcTurnpoint;
		ListCtrl.InsertItem(&lvi);
		pcTurnpoint->LoadListItem(ListCtrl, iItem, eCoordFormat );
		iItem++;
		}
	}

#define NUM_COLUMNS 6

static _TCHAR *_gszColumnHHMMSSLabel[NUM_COLUMNS] =
{
	_T("ID"), _T("Name"), _T("Lat (ddmmss)"), _T("Lon (ddmmss)"), _T("Attributes"), _T("Elevation") };

static _TCHAR *_gszColumnHHMMMMLabel[NUM_COLUMNS] =
{
	_T("ID"), _T("Name"), _T("Lat (ddmmmm)"), _T("Lon (ddmmmm)"), _T("Attributes"), _T("Elevation") };

static int _gnColumnFmt[NUM_COLUMNS] = 
{
	LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_LEFT
};

static int _gnColumnWidth[NUM_COLUMNS] = 
{
	35, 140, 130, 130, 75, 75 };

void CTurnpointArray::CreateControlColumns(CListCtrl& ListCtrl, ECoordFormat eCoordFormat)
{
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;



	for(  int i = 0; i<NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		if( eCoordFormat==eHHMMSS )
			lvc.pszText = _gszColumnHHMMSSLabel[i];
		else
			lvc.pszText = _gszColumnHHMMMMLabel[i];
		lvc.cx =  _gnColumnWidth[i];
		lvc.fmt =_gnColumnFmt[i];
		ListCtrl.InsertColumn(i,&lvc);
	}

}

void CTurnpointArray::Purge()
{
for ( int i=0; i<GetSize(); i++)  
	{
	RemoveAt(i);
	}
}

void CTurnpointArray::CreateGridControlColumns(CListCtrl & ListCtrl)
{
	LV_COLUMN lvc;

	TCHAR cTempBuf[100];

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	CTurnpoint* pcTurnpoint;

	lvc.iSubItem = 0;
	lvc.pszText=_T("Name");
	lvc.cx =  100;
	lvc.fmt = LVCFMT_LEFT;
	ListCtrl.InsertColumn(0,&lvc);

	for ( int i=0; i<GetSize(); i++)
		{ 
	    pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( pcTurnpoint==NULL) continue;
		if( !pcTurnpoint->IsControlPoint() ) continue;
		lvc.iSubItem = i+1;
		strcpy_s( cTempBuf, pcTurnpoint->GetNoNameText());
		lvc.pszText=cTempBuf;
		lvc.cx =  130;
		lvc.fmt = LVCFMT_LEFT;
		ListCtrl.InsertColumn(i+1,&lvc);
		}
}

void CTurnpointArray::LoadTurnpointGridList(CListCtrl & ListCtrl, EUnits eUnits)
{
	CTurnpoint	*pcTurnpointRow;
	CLocation   *pcLocation;
	int iItem=0,i=0;

	LV_ITEM lvi;
	double dDist, dCourse;

	lvi.iSubItem = 0;
	lvi.mask = 0;

	ListCtrl.DeleteAllItems( );

	for (  i=0; i<GetSize(); i++)
		{ 
	    pcTurnpointRow = (CTurnpoint*)GetAt(i);
		if( pcTurnpointRow==NULL) continue;
		if( !pcTurnpointRow->IsControlPoint() ) continue;
		lvi.iItem = iItem;
		ListCtrl.InsertItem(&lvi);
		ListCtrl.SetItemText(iItem,0,pcTurnpointRow->GetNoNameText());
		CString cTemp;
		int nOutput=0;
		for ( int j=0; j<GetSize(); j++)
			{ 
			CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(j);
			pcLocation = (CLocation*)GetAt(j);
			if( pcLocation==NULL) continue;
			if( pcTurnpoint==NULL) continue;
			if( !pcTurnpoint->IsControlPoint() ) continue;
			dDist=pcTurnpointRow->DistanceTo(pcLocation,eUnits);
			dCourse=pcTurnpointRow->CourseTo(*pcLocation);
//			if( j<=i )
			if(  pcTurnpoint->m_iID <=pcTurnpointRow->m_iID )
				cTemp.Format(_T(" %8.3f %s - %8.1f Degrees "), dDist, UnitsTextShort(eUnits), dCourse);
			else
				cTemp=_T(" ");

			ListCtrl.SetItemText(iItem,nOutput+1,cTemp);
			nOutput++;
			}
		iItem++;
		}
}

void CTurnpointArray::LoadTurnpointComboBox(CComboBox &cComboBox, bool bOnlyControl)
{
	cComboBox.ResetContent();
	for ( int i=0; i<GetSize(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( pcTurnpoint )  
			{
			if(  bOnlyControl &&
				 (!(pcTurnpoint->m_iAttribute & TPT_HOME)	&&
				  !(pcTurnpoint->m_iAttribute & TPT_START)  &&
				  !(pcTurnpoint->m_iAttribute & TPT_FINISH) &&
				  !(pcTurnpoint->m_iAttribute & TPT_CONTROL)	)  ) continue;

			int iItem=cComboBox.AddString( pcTurnpoint->GetNoNameText() );
			cComboBox.SetItemDataPtr( iItem, pcTurnpoint );
			}
	
	}
	cComboBox.SetCurSel(0);
}


int CTurnpointArray::LoadTurnpointComboBox(CComboBox &cComboBox, int iAttribute, int iDefaultID )
{
	int nEntries=0;
	int iDef=0;
	cComboBox.ResetContent();
	for ( int i=0; i<GetSize(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( pcTurnpoint )  
			{
			if( pcTurnpoint->m_iAttribute & iAttribute )
				{
				if( pcTurnpoint->GetID()==iDefaultID ) iDef=nEntries;
				int iItem=cComboBox.AddString( pcTurnpoint->GetNoNameText() );
				cComboBox.SetItemDataPtr( iItem, pcTurnpoint );
				nEntries++;
				}
			}
	
	}
	cComboBox.SetCurSel(iDef);
	return nEntries;
}


/*

void CTurnpointArray::LoadList(int nList, int aiList [ ], CTurnpointListCtrl& cTurnpointListCtrl)
{
	cTurnpointListCtrl.ResetContent();
	for ( int i=0; i<nList; i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(aiList[i]-1);
		if( pcTurnpoint )  
			{
			cTurnpointListCtrl.InsertTurnpoint( pcTurnpoint );
			}	
	}
}
*/


void CTurnpointArray::LoadTurnpointComboBox(CComboBox & cComboBox, 
											int nTurnpoints, 
											int aiTurnpoints[] )
{
	cComboBox.ResetContent();
	for ( int i=0; i<nTurnpoints; i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(aiTurnpoints[i]-1);
		ASSERT(pcTurnpoint);
		if( pcTurnpoint )  
			{
			int iItem=cComboBox.AddString( pcTurnpoint->GetNoNameText() );
			cComboBox.SetItemDataPtr( iItem, pcTurnpoint );
			}
	
	}
	cComboBox.SetCurSel(0);
}

int CTurnpointArray::NumberSummaryPages()
	{
	int nTurnpoints=0;
	for ( int i=0; i<GetSize(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( pcTurnpoint ) nTurnpoints++;
		}
	int nPages=nTurnpoints/NUM_TURNPOINTS_PER_PAGE;
	if( nPages*NUM_TURNPOINTS_PER_PAGE < nTurnpoints ) nPages++;

	return nPages;

	}


void CTurnpointArray::DrawTurnpointList(	CDC *pDC, 
											CString &cContestName,
											CString &cContestLocation,
											CTime	&cDate, 
											int		iPage,
											ECoordFormat eFormat)
	{

	CSize	 cTextSize;
	int iYpos=0;
	CString  cTitle=_T("Control Point List"), cText, cPage;
	int iStartTP=((iPage-1)*NUM_TURNPOINTS_PER_PAGE)+1;
	int iEndTP=iPage*NUM_TURNPOINTS_PER_PAGE;

	SelectFont( pDC, 30, FW_BOLD, FALSE);

    pDC->SetTextAlign( TA_TOP| TA_CENTER );
    pDC->TextOut( PAGE_WIDTH/2, 0, cTitle, cTitle.GetLength() );
	cTextSize=pDC->GetTextExtent(cTitle);
	iYpos-=cTextSize.cy;
	iYpos-=5;

	SelectFont( pDC, 20, FW_BOLD, FALSE);
	cPage.Format(_T("Page %d/%d"), iPage, NumberSummaryPages());
    pDC->SetTextAlign( TA_TOP| TA_RIGHT );
    pDC->TextOut( PAGE_WIDTH, 0, cPage, cPage.GetLength() );

	SelectFont( pDC, 25, FW_BOLD, FALSE);
    pDC->SetTextAlign( TA_TOP| TA_LEFT );

	pDC->TextOut( 0, iYpos, cContestName, cContestName.GetLength() );
	cTextSize=pDC->GetTextExtent(cContestName);
	iYpos-=cTextSize.cy;

	pDC->TextOut( 0, iYpos, cContestLocation, cContestLocation.GetLength() );
	cTextSize=pDC->GetTextExtent(cContestLocation);
	iYpos-=cTextSize.cy;

	CString strDate = cDate.Format( _T("%B %Y") );
	pDC->TextOut( 0, iYpos, strDate, strDate.GetLength() );
	cTextSize=pDC->GetTextExtent(strDate);
	iYpos-=cTextSize.cy;
	iYpos-=8;

//  Output Column Headers
	SelectFont( pDC, 20, FW_BOLD, TRUE); 

	cText=_T("ID");
	pDC->TextOut( 0 , iYpos, cText, cText.GetLength() );
	cTextSize=pDC->GetTextExtent(cText);

	cText=_T("Name");
	pDC->TextOut( 40 , iYpos, cText, cText.GetLength() );
	cTextSize=pDC->GetTextExtent(cText);

	cText=_T("Latitude");
	pDC->TextOut( 250 , iYpos, cText, cText.GetLength() );
	cTextSize=pDC->GetTextExtent(cText);

	cText=_T("Longitude");
	pDC->TextOut( 450 , iYpos, cText, cText.GetLength() );
	cTextSize=pDC->GetTextExtent(cText);

	cText=_T("Attributes");
	pDC->TextOut( 600 , iYpos, cText, cText.GetLength() );
	cTextSize=pDC->GetTextExtent(cText);

	cText=_T("Elevation");
	pDC->TextOut( 700 , iYpos, cText, cText.GetLength() );
	cTextSize=pDC->GetTextExtent(cText);

	iYpos-=cTextSize.cy;


	SelectFont( pDC, 20, FW_NORMAL, FALSE); 
	int nTurnpoints=0;
	for ( int i=0; i<GetSize(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( !pcTurnpoint ) continue;
		nTurnpoints++;
		if( nTurnpoints< iStartTP || nTurnpoints> iEndTP ) continue;

		cText=pcTurnpoint->IdText();
		pDC->TextOut( 0 , iYpos, cText, cText.GetLength() );
		cTextSize=pDC->GetTextExtent(cText);

		cText=pcTurnpoint->m_strName;
		pDC->TextOut( 40 , iYpos, cText, cText.GetLength() );
		cTextSize=pDC->GetTextExtent(cText);

		if( eFormat==eHHMMMM )
			{
			cText=pcTurnpoint->HHHMMMLatText();
			pDC->TextOut( 250 , iYpos, cText, cText.GetLength() );
			cTextSize=pDC->GetTextExtent(cText);

			cText=pcTurnpoint->HHHMMMLongText();
			pDC->TextOut( 450 , iYpos, cText, cText.GetLength() );
			cTextSize=pDC->GetTextExtent(cText);
			}
		else
			{
			cText=pcTurnpoint->HHMMSSLatText();
			pDC->TextOut( 250 , iYpos, cText, cText.GetLength() );
			cTextSize=pDC->GetTextExtent(cText);

			cText=pcTurnpoint->HHMMSSLongText();
			pDC->TextOut( 450 , iYpos, cText, cText.GetLength() );
			cTextSize=pDC->GetTextExtent(cText);
			}

		cText=pcTurnpoint->AttributeText();
		pDC->TextOut( 600 , iYpos, cText, cText.GetLength() );
		cTextSize=pDC->GetTextExtent(cText);

		pDC->SetTextAlign( TA_RIGHT );
		cText=pcTurnpoint->ElevationText();
		pDC->TextOut( 750 , iYpos, cText, cText.GetLength() );
		cTextSize=pDC->GetTextExtent(cText);
		pDC->SetTextAlign( TA_LEFT );

		iYpos-=cTextSize.cy;
		}


	pDC->SetTextAlign( TA_TOP| TA_CENTER );

	SelectFont( pDC, 16, FW_NORMAL, FALSE); 
	cText=_T("A-Airfield   F-Finish   H-Home   S-Start   T-Turnpoint");
	pDC->TextOut( PAGE_WIDTH/2 , -1010, cText, cText.GetLength() );



	DRAW_HORIZONTAL( -1030 );


	CTime cToday = CTime::GetCurrentTime();
	cText=cToday.Format(_T("Printed on %A %B %d, %Y - %H:%M by Winscore") );
	SelectFont( pDC, 13, FW_NORMAL, FALSE); 
	pDC->TextOut( PAGE_WIDTH/2, -1035 , cText, cText.GetLength() );

	return;
	}

int CTurnpointArray::ImportTurnpoints(CString cFileName, int iIndex)
	{

#define MAXLINELEN 256

	char  cBuff[MAXLINELEN], cBuf1[80];
	
	char cName[80];
	int nImported=0;
	ifstream	cIfstream;

	//  Use the ifstream class to do I/O on the ascii files.
	cIfstream.open(cFileName);
	if (cIfstream.is_open()==0)
		{
		return 0;
		}

	char *pChar;

	while( cIfstream.good() )
		{

		cIfstream.getline( cBuff, MAXLINELEN );
		if( cIfstream.fail() ) break;
        if( strncmp( cBuff, ("**"), 2 )==0 ) continue;

		pChar=cBuff;
		if( strctok( cBuf1, 80, &pChar, ',' )==0 ) continue;

//		Read the ID
		int iID;
		sscanf_s(cBuf1, ("%d"), &iID);
		if( iID<=0 || iID>MAX_TURNPOINTS )
			{
			CString cMess;
			cMess.Format("Turnpoint ID %i, is not a valid ID.  Valid range is 1 through %i.\nContinue reading control points?",iID,MAX_TURNPOINTS);
			int iAnswer=AfxMessageBox( cMess, MB_YESNOCANCEL | MB_ICONEXCLAMATION   );
			if( iAnswer!=IDYES ) break;
			continue;
			}

//		Get the Latitude Degrees
		int iLatDegrees;
		if( strctok( cBuf1, 80, &pChar, ':' )==0 ) continue;
		sscanf_s(cBuf1, ("%d"), &iLatDegrees);


//		Get the Latitude Minutes and or degrees
		double dLatMinutes;
		if( strctok( cBuf1, 80, &pChar, ',' )==0 ) continue;
		ELatDirection eLatDirection=eNORTH;
		char *pDir=strchr(cBuf1, 'N');
		if( !pDir )
			{
			pDir=strchr(cBuf1, 'S');
			if(!pDir) continue;
			eLatDirection=eSOUTH;
			}
		*pDir=' ';


	    if( strchr( cBuf1, '.' ) ) 
			// Decimal point present, read decimal minutes
			sscanf_s(cBuf1, ("%lf"), &dLatMinutes);
		else if( strchr( cBuf1, ':' ) ) 
			// Format in HHMMSS
			{
			int iLatMinutes, iLatSeconds;
			sscanf_s(cBuf1, ("%2d:%2d"), &iLatMinutes, &iLatSeconds);
			dLatMinutes=(double)iLatMinutes+((double)iLatSeconds/60.);
			}
		else
			continue;


//		Get the Long Degrees
		int iLongDegrees;
		if( strctok( cBuf1, 80, &pChar, ':' )==0 ) continue;
		sscanf_s(cBuf1, ("%d"), &iLongDegrees);


//		Get the Latitude Minutes
		double dLongMinutes;
		if( strctok( cBuf1, 80, &pChar, ',' )==0 ) continue;
		ELongDirection eLongDirection=eWEST;
		pDir=strchr(cBuf1, 'W');
		if( !pDir )
			{
			pDir=strchr(cBuf1, 'E');
			if(!pDir) continue;
			eLongDirection=eEAST;
			}
		*pDir=' ';

	    if( strchr( cBuf1, '.' ) ) 
			// Decimal point present, read decimal minutes
			sscanf_s(cBuf1, ("%lf"), &dLongMinutes);
		else if( strchr( cBuf1, ':' ) ) 
			// Format in HHMMSS
			{
			int iLongMinutes, iLongSeconds;
			sscanf_s(cBuf1, ("%2d:%2d"), &iLongMinutes, &iLongSeconds);
			dLongMinutes=(double)iLongMinutes+((double)iLongSeconds/60.);
			}
		else
			continue;

		// Read the Elevation
		int iElevation=0;
		if( strctok( cBuf1, 80, &pChar, ',' )>0 )
			{
			sscanf_s(cBuf1, ("%d"), &iElevation);
			}
		else
			continue;

		// Read Attributes and check for turnpoint, Start Finish and Home
		if( strctok( cBuf1, 80, &pChar, ',' )==0 ) continue;
//		if( strchr(cBuf1, 'T')==NULL &&
//			strchr(cBuf1, 'S')==NULL &&
//			strchr(cBuf1, 'F')==NULL &&
//			strchr(cBuf1, 'H')==NULL     ) continue;

		int iAttribute=0;
		if( strchr(cBuf1, 'A') ) iAttribute |=TPT_AIRFIELD;
		if( strchr(cBuf1, 'L') ) iAttribute |=TPT_AIRFIELD;
		if( strchr(cBuf1, 'S') ) iAttribute |=TPT_START;
		if( strchr(cBuf1, 'F') ) iAttribute |=TPT_FINISH;
		if( strchr(cBuf1, 'H') ) iAttribute |=TPT_HOME;
		if( strchr(cBuf1, 'T') ) iAttribute |=TPT_CONTROL;

		// Read the turnpoint name
		if( strctok( cName, 80, &pChar, ',' )==0 ) continue;

		CLocation cLocation( 	iLatDegrees,	dLatMinutes, 
								iLongDegrees,	dLongMinutes, 
								eLatDirection,  eLongDirection);

		CTurnpoint* pcTurnpoint=new CTurnpoint( iID, 
												cName, 
												&cLocation,
												iAttribute,
												iElevation );
		RemoveAt(iID-1);  // If one is there already, remove it
		SetAt(iID-1, pcTurnpoint); // Add to the list

		nImported++;
		if( nImported>MAX_TURNPOINTS ) break;

		}

	cIfstream.close();

	return nImported;
	}

int CTurnpointArray::GetValidTpts()
{
	int nTpts=0;
	for ( int i=0; i<GetSize(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( pcTurnpoint )  nTpts++;
		}
	return nTpts;
}

CTurnpoint*  CTurnpointArray::GetHomePoint(CTurnpoint* pStart)
	{
	CTurnpoint* pcClosest=NULL;
	double dDist=100000.;
	for ( int i=0; i<GetSize(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( pcTurnpoint )  
			{
			if( pcTurnpoint->m_iAttribute & TPT_HOME ) 
				{
				if( pStart ) 
					{
					double dCheck=pStart->DistanceTo(pcTurnpoint,eNautical);
					if( dCheck<dDist ) 
						{
						dDist=dCheck;
						pcClosest=pcTurnpoint;
						}
					}
				else
					return pcTurnpoint;
				}
			}
		}
	return pcClosest;
	}

CTurnpoint* CTurnpointArray::FindClosestAirport(CLocation cLoc, double dRadius)
	{
	CTurnpoint *pcClosestTurnpoint=NULL;
	double	dMinDist=1.e+30;;
	for ( int i=0; i<GetSize(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( pcTurnpoint )  
			{
			if( pcTurnpoint->m_iAttribute & TPT_AIRFIELD ) 
				{
				double dist=pcTurnpoint->DistanceTo( &cLoc, SYSTEMUNITS);
				if( dist<dMinDist )
					{
					dMinDist=dist;
					pcClosestTurnpoint=pcTurnpoint;
					}
				}
			}
		}
	return (dMinDist<=dRadius)?(pcClosestTurnpoint):(NULL);
}
CString CTurnpointArray::GetText(ECoordFormat eFormat)
{
	CString str;
	int nTpts=0;
	for ( int i=0; i<GetSize(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( !pcTurnpoint )  continue;
	
		str+=pcTurnpoint->IdText()+_T("\t");
		str+=pcTurnpoint->m_strName+_T("\t");
		if( eFormat==eHHMMSS)
			{
			str+=pcTurnpoint->HHMMSSLatText()+_T("\t");
			str+=pcTurnpoint->HHMMSSLongText()+_T("\t");
			}
		else
			{
			str+=pcTurnpoint->HHHMMMLatText()+_T("\t");
			str+=pcTurnpoint->HHHMMMLongText()+_T("\t");
			}
		str+=pcTurnpoint->AttributeText()+_T("\t");
		str+=pcTurnpoint->ElevationText()+_T("\t");
		str+=_T("\r\n");

		}
	return str;

}
CString CTurnpointArray::MatrixText(EUnits eUnits)
{
	CString str;

	CTurnpoint* pcTurnpoint;
	CTurnpoint	*pcTurnpointRow;
	CLocation   *pcLocation;
	double dDist;

	str+=_T("Name\t");
	for ( int i=0; i<GetSize(); i++)
		{ 
	    pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( pcTurnpoint==NULL) continue;
		if( !pcTurnpoint->IsControlPoint() ) continue;
		str+=pcTurnpoint->GetNoNameText()+_T("\t");
		}

	str+=_T("\n");

	for ( int i=0; i<GetSize(); i++)
		{ 
	    pcTurnpointRow = (CTurnpoint*)GetAt(i);
		if( pcTurnpointRow==NULL) continue;
		if( !pcTurnpointRow->IsControlPoint() ) continue;
		str+=pcTurnpointRow->GetNoNameText()+_T("\t");

		CString cTemp;
		int nOutput=0;
		for ( int j=0; j<GetSize(); j++)
			{ 
			CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(j);
			pcLocation = (CLocation*)GetAt(j);
			if( pcLocation==NULL) continue;
			if( pcTurnpoint==NULL) continue;
			if( !pcTurnpoint->IsControlPoint() ) continue;
			dDist=pcTurnpointRow->DistanceTo(pcLocation,eUnits);
			if(  pcTurnpoint->m_iID <=pcTurnpointRow->m_iID )
				{
				cTemp.Format(_T(" %8.3f"), dDist );
				str+=cTemp+_T("\t");
				}
			}
		str+=_T("\r\n");
		}
	return str;
}

void CTurnpointArray::AddToControl(CWSView &cWSView)
{

	for (  int i=0; i<GetSize(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( pcTurnpoint && pcTurnpoint->IsControlPoint())
			{
			cWSView.AddControlPoint(  
									   pcTurnpoint->m_iID, 
									   pcTurnpoint->m_strName, 
									   (float)pcTurnpoint->GetLat(), 
									   (float)pcTurnpoint->GetLong(), 
									   pcTurnpoint->m_iElevation,
									   pcTurnpoint->m_iAttribute  );
			}
		}


}


CTurnpoint* CTurnpointArray::GetFirstByAttribute(int iAttribute)
	{
	for ( int i=0; i<GetSize(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( pcTurnpoint )  
			{
			if( pcTurnpoint->m_iAttribute & iAttribute ) 
				{
				return pcTurnpoint;
				}
			}
		}
	return NULL;
	}

double CTurnpointArray::GetMaxDistanceFromHome()
{
	CTurnpoint* pHome=GetHomePoint();
	if(!pHome) return 0;

	double dDist=0;
	for ( int i=0; i<GetSize(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( pcTurnpoint )  
			{
			double dCheck=pcTurnpoint->DistanceTo(pHome,SYSTEMUNITS);
			if( dDist<dCheck ) dDist=dCheck;
			}
		}
	return dDist;
}

bool CTurnpointArray::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pParentNode)
	{

	MSXML2::IXMLDOMNodePtr pIDOMChildNode=NULL;
	cMgr.CreateChild( pParentNode, pIDOMChildNode, _T("Waypoints"));

	for ( int i=0; i<GetSize(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)GetAt(i);
		if( pcTurnpoint )  
			{
			pcTurnpoint->GetXML( cMgr, pIDOMChildNode );
			}
		}

	return true;
	}


int CTurnpointArray::ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pWinscore)
	{
	int nImported=0;

	MSXML2::IXMLDOMNodePtr pWaypoints=NULL;
	TESTHR(cMgr.SelectChild(_T("Waypoints"), pWinscore, pWaypoints));
	if( pWaypoints==NULL ) return 0;


	MSXML2::IXMLDOMNodeListPtr pWaypointLists=cMgr.GetChildList( pWaypoints );
	if( pWaypointLists==NULL ) return 0;

	long length=0;
	TESTHR(pWaypointLists->get_length(&length));
	if( length==0 ) return 0;

	for( int i=0; i<length; i++ )
		{
		MSXML2::IXMLDOMNodePtr pWaypoint=NULL;
		TESTHR(pWaypointLists->get_item(i,&pWaypoint));
		CTurnpoint* pcTurnpoint=NULL;
		try
			{
			pcTurnpoint=new CTurnpoint(cMgr, pWaypoint );
			}
		catch(...)
			{
			ASSERT(FALSE);
			if(pcTurnpoint) delete pcTurnpoint;
			continue;
			}

		if( pcTurnpoint->GetID()<=0 || pcTurnpoint->GetID()>MAX_TURNPOINTS )
			{
			ASSERT(FALSE);
			delete pcTurnpoint;
			continue;
			}

		RemoveAt(pcTurnpoint->GetID()-1);  // If one is there already, remove it
		SetAt(pcTurnpoint->GetID()-1, pcTurnpoint); // Add to the list

		nImported++;
		if( nImported>MAX_TURNPOINTS ) break;
		}


	return nImported;
	}



//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ContestantList.cpp: implementation of the CContestantList class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Winscore.h"
#include "ContestantList.h"
#include "WinscoreFonts.h"
#include "GliderInfoList.h"
#include "Global_Prototypes.h"
#include "mainfrm.h"
#include <atlbase.h>

#define PAGE_WIDTH  800


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static int iSort;

int compare_contestants( const void *arg1, const void *arg2 );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContestantList::CContestantList()
{

}

CContestantList::~CContestantList()
{
	if( IsEmpty() !=0  )  return;
	Purge();
	return;
}


double  CContestantList::GetLowestPerformanceHandicap(EClass eClass)
	{
		double dLowHandicap=0.0;
		POSITION thispos, pos = GetHeadPosition();
		while( pos!=NULL)
			{
			thispos=pos;
			CContestant* pcContestant=(CContestant*)GetNext(pos);
			if(pcContestant->IsGuest() || pcContestant->m_eClass!=eClass ) continue;
			dLowHandicap=max( pcContestant->m_fHandicap, dLowHandicap);
			}
		return dLowHandicap;
	}


CContestant* CContestantList::GetByContestNo(CString strContestNo)
	{
	CContestant* pcContestant;

	if( IsEmpty() !=0  )  return NULL;

	POSITION pos = GetHeadPosition();

	while(1)
		{
		pcContestant=(CContestant*)GetNext(pos);
		if( pcContestant->m_strContestNo==strContestNo ) return pcContestant;
		if( pos==NULL ) break;
		}
	return NULL;
	}


bool CContestantList::AddToList(CContestant * pcAddContestant)
	{
	//  If a contestant with this name is there already, remove it
	if( !IsEmpty() ) 
		{
		POSITION thispos, pos = GetHeadPosition();
		while( pos!=NULL)
			{
			thispos=pos;
			CContestant* pcContestant=(CContestant*)GetNext(pos);
			if( pcContestant->m_strContestNo==pcAddContestant->m_strContestNo ) 
				{
				delete pcContestant;
				SetAt( thispos, (CContestant*)pcAddContestant );
				return false;
				}
			}
		}
//  If we got here, it must be a new one
	AddTail( (CContestant*)pcAddContestant );
	return true;
	}


CContestantList& CContestantList::operator =(CContestantList& cInputContestantList)
{
	CContestant *pcNewContestant, *pcContestant;

	//  Clean out the destination  
	Purge();

	if( cInputContestantList.IsEmpty() !=0  )  return *this;

	POSITION pos = cInputContestantList.GetHeadPosition();

	while(1)
		{
		pcContestant=(CContestant*)cInputContestantList.GetNext(pos);
		if( pcContestant ) 
			{
			pcNewContestant=new CContestant(pcContestant);
			(CContestant*)AddTail(pcNewContestant);
			}
		if( pos==NULL ) break;
		}

	return *this;

}


void CContestantList::Purge()
	{

	if( IsEmpty() !=0  )  return;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CContestant* pcContestant=GetNext(pos);
		if( pcContestant ) delete pcContestant;
		if( pos==NULL ) break;
		}
	RemoveAll();
	}

CContestant* CContestantList::GetNext(POSITION& pos)
{
	return (CContestant*)CObList::GetNext(pos);
}



void CContestantList::RemoveByPointer(CContestant* pcRemContestant)
	{

	if( !pcRemContestant )  return ;
	CContestant *pcContestant;
	POSITION thispos, pos = GetHeadPosition();
	while( pos )
		{
		thispos=pos;
		pcContestant=(CContestant*)GetNext(pos);
		if( pcContestant->m_strContestNo==pcRemContestant->m_strContestNo) 
			{
			delete pcContestant;
			RemoveAt(thispos);
			return;
			}
		}
	}

#define NUM_COLUMNS 9

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
{
	_T("Name"), _T("ID"), _T("Glider"), _T("Class"), _T("Handicap"),
	 _T("SSA Number"), _T("Status"),  _T("Gate"), _T("Address"), 
};

static int _gnColumnFmt[NUM_COLUMNS] = 
{
	LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT,
		LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT
};

static int _gnColumnWidth[NUM_COLUMNS] = 
{
	150, 50, 80, 50, 50, 50, 50, 50, 300
};

void CContestantList::CreateControlColumns(CListCtrl& ListCtrl)
{
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for(  int i = 0; i<NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx =  _gnColumnWidth[i];
		lvc.fmt =_gnColumnFmt[i];
		ListCtrl.InsertColumn(i,&lvc);
	}

}

void CContestantList::LoadContestantList(CListCtrl& ListCtrl)
{
//	char buff[50];

	CContestant* pcContestant;

	LV_ITEM lvi;

	lvi.iSubItem = 0;
	lvi.mask = 0;

	ListCtrl.DeleteAllItems( );

	//  Load up the listbox
	int iItem=0;
	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		pcContestant=GetNext(pos);
		if( pcContestant )
			{
			lvi.iItem = iItem;
			lvi.lParam = (LPARAM)pcContestant;
			ListCtrl.InsertItem(&lvi);
			SetContestantItem(ListCtrl, iItem, pcContestant);
			iItem++;
			}
		}
	}

void CContestantList::SetContestantItem(CListCtrl& ListCtrl, int iItem, CContestant *pcContestant)
	{
	int iCol=0;
	CString cGlider=pcContestant->m_strGlider;
	if( pcContestant->IsMotorized() ) cGlider+=" (Motorized)";
	CString cName=pcContestant->NameText();
	ListCtrl.SetItemText(iItem,iCol++,cName);
	ListCtrl.SetItemText(iItem,iCol++,pcContestant->m_strContestNo);
	ListCtrl.SetItemText(iItem,iCol++,cGlider);
	ListCtrl.SetItemText(iItem,iCol++,GetClass(pcContestant->m_eClass).GetName(false));
	ListCtrl.SetItemText(iItem,iCol++,pcContestant->HandicapText());
	ListCtrl.SetItemText(iItem,iCol++,pcContestant->SSANumberText());
	ListCtrl.SetItemText(iItem,iCol++,pcContestant->CitizenText());

	if( GetWinscoreInt( ALLOWTWOGATES, 0 )==0 )
		ListCtrl.SetItemText(iItem,iCol++,_T(""));
	else
		ListCtrl.SetItemText(iItem,iCol++,pcContestant->GetGateText());

	ListCtrl.SetItemText(iItem,iCol++,pcContestant->AddressText());
	ListCtrl.SetItemData(iItem, (LPARAM)pcContestant);
	}



void CContestantList::RemoveByPosition(int iPos)
{
	if( iPos<0 )  return ;
	CContestant *pcContestant;
	int iItem=0;
	POSITION thispos, pos = GetHeadPosition();
	while( pos )
		{
		thispos=pos;
		pcContestant=(CContestant*)GetNext(pos);
		if( iItem==iPos) 
			{
			delete pcContestant;
			RemoveAt(thispos);
			return;
			}
		iItem++;
		}
}

void CContestantList::GetContestants(EClass eClass, CStringArray & cArray, bool bGuestsOnly)
	{
	if( IsEmpty() !=0  )  return;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CContestant* pcContestant=GetNext(pos);
		if( pcContestant->m_eClass != eClass ) continue;
		if( bGuestsOnly && pcContestant->IsGuest() ) continue;
			
		cArray.Add(pcContestant->m_strContestNo);
		}
	}


int CContestantList::NumberSummaryPages()
	{
	if( IsEmpty() )  return 1;

	int nContestants=GetCount();

	int nPages=nContestants/NUM_CONTESTANTS_PER_PAGE;
	if( nPages*NUM_CONTESTANTS_PER_PAGE < nContestants ) nPages++;

	return nPages;

	}


void CContestantList::DrawContestantList(	CDC		*pDC,
											CString &cContestName,
											CString &cContestLocation,
											CTime	&cDate, 
											int		iPage )
	{
	CSize	 cTextSize;
	int iYpos=0;

	BOOL m_bTwoGates=GetWinscoreInt( ALLOWTWOGATES, 0 );

	CString  cTitle=_T("Contestant List"), cText, cPage;
	int iStart=((iPage-1)*NUM_CONTESTANTS_PER_PAGE)+1;
	int iEnd=iPage*NUM_CONTESTANTS_PER_PAGE;

	int iTabPos=50;

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

	int nContestants=0;

	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		CContestant *pcContestant=GetNext(pos);
		if( pcContestant )
			{
			nContestants++;
			if( nContestants< iStart || nContestants> iEnd ) continue;

			DRAW_HORIZONTAL( iYpos );
			iYpos-=2;

			SelectFont( pDC, 15, FW_BOLD, FALSE); 

			cText=pcContestant->m_strContestNo;
			pDC->TextOut( 0 , iYpos, cText, cText.GetLength() );

			SelectFont( pDC, 15, FW_NORMAL, FALSE); 

			CString cHandicapText=_T("");
			if( GetClass(pcContestant->m_eClass).IsHandicapped() ) cHandicapText.Format(_T(",  Hdcp: %4.3lf"),pcContestant->m_fHandicap);
			CString cSSANo;
			cSSANo.Format(_T("SSA No: %d,   "),pcContestant->m_iSSANumber);
			CString cGlider=pcContestant->m_strGlider;
			if( pcContestant->IsMotorized() ) cGlider+=_T(" (Motorized)");

			CString strGate;
			if( m_bTwoGates )
				strGate=_T(", Gate ")+pcContestant->GetGateText();

			cText=	pcContestant->m_strLastName+_T(", ")+
					pcContestant->m_strFirstName+_T(" ")+
					pcContestant->m_strMiddleName+_T("  ")+
					cSSANo+
					_T("Glider: ")+cGlider+_T(",  ")+
					_T("Class: ")+GetClass(pcContestant->m_eClass).GetName(false)+
					cHandicapText+_T(",  ")+
					_T("Status: ")+pcContestant->CitizenText()+strGate;

			pDC->TextOut(iTabPos , iYpos, cText, cText.GetLength() );			
			cTextSize=pDC->GetTextExtent(cText);
			iYpos-=cTextSize.cy;

			if( pcContestant->m_strAddress1.GetLength()>0 ||
				pcContestant->m_strAddress2.GetLength()>0 ||
				pcContestant->m_strCity.GetLength()>0 ||
				pcContestant->m_strState.GetLength()>0    )
				{
				cText.Format( _T("%s;  %s;  %s, %s  %s - %s"),
							pcContestant->m_strAddress1,
							pcContestant->m_strAddress2,
							pcContestant->m_strCity,
							pcContestant->m_strState,
							pcContestant->m_strZipcode1,
							pcContestant->m_strZipcode2 );
				pDC->TextOut(iTabPos , iYpos, cText, cText.GetLength() );			
				cTextSize=pDC->GetTextExtent(cText);
				iYpos-=cTextSize.cy;
				}
			else
				iYpos-=cTextSize.cy;

			}
		}

	DrawStandardTrailer(pDC);

	return;
	}


ostream& operator <<(ostream &os, CContestantList& cList )
{
	if( !cList.IsEmpty() )
		{
		os << cList.GetCount() << endl;
		POSITION pos = cList.GetHeadPosition();
		while(pos!=NULL)
			{
			CContestant* pcContestant=cList.GetNext(pos);
			os << (*pcContestant);
			}
		}
return os;
}

istream& operator >>(istream &is, CContestantList& cList )
{
	int nCont=0;
	is >> nCont;
	if( nCont<0 || nCont>250 ) return is;
	for( int i=0; i<nCont; i++ )
		{	
		CContestant* pcContestant=new CContestant();
		is>> *pcContestant;
		cList.AddToList( pcContestant);

		if( is.eof() || is.bad() ) break;
		}

return is;
}

void CContestantList::Sort( int iSortColumn )
{
	int nContestants=GetCount();
	int i=0;
	CContestant** ppcContestants;

	ppcContestants=new CContestant*[nContestants];

	POSITION pos = GetHeadPosition();
	while(pos)
		{
		ppcContestants[i++]=GetNext(pos);
		}

	if( i>0 )
		{
		iSort=abs(iSortColumn)-1;
		qsort( (void*)ppcContestants, i, sizeof(CContestant*), compare_contestants );

		RemoveAll();

		for( int iCnt=0; iCnt<i; iCnt++ )
			{
			AddTail( (CContestant*)ppcContestants[iCnt] );
			}; 

		}
	delete ppcContestants;


}
int compare_contestants( const void *arg1, const void *arg2 )
{
	 if( iSort==0 )
		{
		if( (*(CContestant**)arg1)->m_strLastName<(*(CContestant**)arg2)->m_strLastName  )
			return  -1;
		else if( (*(CContestant**)arg1)->m_strLastName>(*(CContestant**)arg2)->m_strLastName  )
			return 1;
		else
			return 0;
		}
	 else if( iSort==1 )
		{
		// Sort By Contest No
		int iResult=strcmp(  (*(CContestant**)arg1)->m_strContestNo,
							 (*(CContestant**)arg2)->m_strContestNo );
		return iResult;
		}
	else if( iSort==2 )
		{
		// Sort By Glider
		CString str1=(*(CContestant**)arg1)->m_strGlider;
		CString str2=(*(CContestant**)arg2)->m_strGlider;
		return strcmp(  str1.MakeLower(),
						str2.MakeLower() );
		}
	else if( iSort==3 ) 
		{
		// Sort By Class

		if( (*(CContestant**)arg1)->m_eClass<(*(CContestant**)arg2)->m_eClass  )
			return  -1;
		else if( (*(CContestant**)arg1)->m_eClass>(*(CContestant**)arg2)->m_eClass  )
			return 1;
		else
			return strcmp(  (*(CContestant**)arg1)->m_strContestNo,
							 (*(CContestant**)arg2)->m_strContestNo );
		}
	else if( iSort==4 ) 
		{
		// Handicap

		if( (*(CContestant**)arg1)->m_fHandicap<(*(CContestant**)arg2)->m_fHandicap  )
			return  -1;
		else if( (*(CContestant**)arg1)->m_fHandicap>(*(CContestant**)arg2)->m_fHandicap  )
			return 1;
		}

	return 0;
}

CString CContestantList::GetText()
	{
	CString str;
	if( !IsEmpty() )
		{
		POSITION pos = GetHeadPosition();
		while(pos!=NULL)
			{
			CContestant* pcContestant=GetNext(pos);
			str+=pcContestant->GetText();
			str+=_T("\r\n");
			}
		}
	return str;
	}

CContestant* CContestantList::GetByPilotName(CString cName)
{
	CContestant* pcContestant;

	if( IsEmpty() !=0  )  return NULL;

	cName.MakeLower();

	POSITION pos = GetHeadPosition();

	while( pos!=NULL )
		{
		pcContestant=(CContestant*)GetNext(pos);
		CString strLast =pcContestant->m_strLastName;
		CString strFirst=pcContestant->m_strFirstName;
		strLast.MakeLower();
		strFirst.MakeLower();
		if( cName.Find(strLast) >=0  &&			
			cName.Find(strFirst)>=0    ) return pcContestant;
		}
	return NULL;
}


int CContestantList::GetXML(CXMLMgr &cMgr, IDispatch *pIDOMNode,bool bHandicappedOnly )
	{

	int iNum=0;
    MSXML2::IXMLDOMNodePtr		pParentNode(pIDOMNode);
  
	MSXML2::IXMLDOMNodePtr pIDOMChildNode;
	TESTHR(cMgr.CreateChild(pParentNode, pIDOMChildNode, _T("Contestants")));

	if( IsEmpty() !=0  )  return 0;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CContestant* pcContestant=GetNext(pos);
		if( pcContestant ) 
			{
			if( !bHandicappedOnly || GetClass(pcContestant->m_eClass).IsHandicapped() )
				{
				pcContestant->GetXML( cMgr, pIDOMChildNode );;
				iNum++;
				}
			}
		if( pos==NULL ) break;
		}

	return iNum;
	}


int CContestantList::ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pWinscore, int &nSkipped)
	{
	int nImported=0;

	MSXML2::IXMLDOMNodePtr pContestants;
	TESTHR(cMgr.SelectChild(_T("Contestants"), pWinscore, pContestants));
	if( pContestants==NULL ) 
		{
		TESTHR(cMgr.SelectChild(_T("contestants"), pWinscore, pContestants));
		if( pContestants==NULL ) return 0;
		}

	MSXML2::IXMLDOMNodeListPtr pContestantList=cMgr.GetChildList(pContestants);
	if( pContestantList==NULL ) return 0;

	long length=0;
	TESTHR(pContestantList->get_length(&length));
	if( length==0 ) return 0;

	for( int i=0; i<length; i++ )
		{
		MSXML2::IXMLDOMNodePtr pContestant;
		TESTHR(pContestantList->get_item(i,&pContestant));
		CContestant* pcContestant=NULL;
		try
			{
			pcContestant=new CContestant(cMgr, pContestant );
			}
		catch(...)
			{
			ASSERT(FALSE);
			if(pcContestant) delete pcContestant;
			continue;
			}

		if( AddToList(pcContestant) ) 
			nImported++;
		else
			nSkipped++;
		}

	return nImported;
	}

int CContestantList::ImportSSA(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pWinscore, CContestantList &cExistingContestantList, int &nSkipped)
	{
	int nImported=0;
	nSkipped=0;

	MSXML2::IXMLDOMNodePtr pContestants;
	TESTHR(cMgr.SelectChild(_T("Contestants"), pWinscore, pContestants));
	if( pContestants==NULL ) 
		{
		TESTHR(cMgr.SelectChild(_T("contestants"), pWinscore, pContestants));
		if( pContestants==NULL ) return 0;
		}

	MSXML2::IXMLDOMNodeListPtr pContestantList=cMgr.GetChildList(pContestants);
	if( pContestantList==NULL ) return 0;

	long length=0;
	TESTHR(pContestantList->get_length(&length));
	if( length==0 ) return 0;

	for( int i=0; i<length; i++ )
		{
		MSXML2::IXMLDOMNodePtr pContestant;
		TESTHR(pContestantList->get_item(i,&pContestant));
		CContestant* pcContestant=NULL;
		try
			{
			pcContestant=new CContestant(cMgr, pContestant, true );
			if( cExistingContestantList.GetByContestNo(pcContestant->m_strContestNo) )
				{
				delete pcContestant;
				nSkipped++;
				continue;
				}
			}
		catch(...)
			{
			ASSERT(FALSE);
			if(pcContestant) delete pcContestant;
			continue;
			}

		if( AddToList(pcContestant) ) 
			nImported++;
		else
			nSkipped++;
		}

	return nImported;
	}

int CContestantList::CheckClasses(EContest eContest)
{
	int nChanged=0;
	CContestant* pcContestant;
	if( IsEmpty() !=0  )  return 0;

	EClass eGoodClass=eStandard;

   	for( int j=0; j<NUMCLASSES; j++ )
    	{
		if( GetClass(j).m_eContest!=eContest ||
			!GetClass(j).IsActive()					) continue;
		eGoodClass=GetClass(j).GetType();
		break;
    	}

	POSITION pos = GetHeadPosition();
	while( pos!=NULL )
		{
		pcContestant=(CContestant*)GetNext(pos);
		CWSClass &cClass=GetClass(eContest, pcContestant->m_eClass);
		if( !cClass.IsActive() )
			{
//			CString strMess;
//			strMess.Format(_T("%s registered as %s, but your contest doesn't have that class.\n Changed to %s"),
//							pcContestant->m_strContestNo,
//							GetClass(pcContestant->m_eClass,false).GetName(),
//							GetClass(eGoodClass).GetName());
//			AfxMessageBox(strMess);
			nChanged++;
			pcContestant->m_eClass=eGoodClass;
			}
		}

	if( nChanged>0  )
		{
		CString strMess;
		strMess.Format(_T("Some contestants had their class changed to be consistant with your contest.\nCheck your contestants class assignment carefully."));
		AfxMessageBox(strMess);
		}
	return nChanged;
}


void CContestantList::UpdateHandicaps(EContest eContest, CGliderInfoList &cGIList)
{
	int nChanged=0;
	CContestant* pcContestant;
	if( IsEmpty() !=0  )  return;
	
	int iRead=cGIList.GetCount();
	if( iRead==0 ) 
		{
		//Cached copy empty, read from local file.
		CString strHCFile=GetHandicapFileName();
		iRead=cGIList.ImportXML(strHCFile,true);
		}
	if( iRead==0 ) return;

	POSITION pos = GetHeadPosition();
	while( pos!=NULL )
		{
		pcContestant=(CContestant*)GetNext(pos);
		CWSClass &cClass=GetClass(eContest, pcContestant->m_eClass);
		if( cClass.IsActive() &&  cClass.IsHandicapped() )
			{
			pcContestant->UpdateHandicap(cGIList);
			}
		}
	return;
}

int CContestantList::GetCountClass(EClass eClass)
{
	int iCount=0;
	if( IsEmpty() !=0  )  return 0;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CContestant* pcContestant=GetNext(pos);
		if( pcContestant ) 
			{
			if(  pcContestant->m_eClass==eClass && !pcContestant->HasWithdrawn() ) iCount++;
			}
		}
	return iCount;
}

#include "stdafx.h"
#include "FDRecorderList.h"
#include "global_prototypes.h"

CFDRecorderList::CFDRecorderList(void)
{
}


CFDRecorderList::~CFDRecorderList(void)
{
}

CFDRecorder* CFDRecorderList::GetNext(POSITION& pos)
{
	return (CFDRecorder*)CObList::GetNext(pos);
}

void CFDRecorderList::Purge()
	{

	if( IsEmpty() !=0  )  return;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CFDRecorder* pcRecorder=GetNext(pos);
		if( pcRecorder ) delete pcRecorder;
		if( pos==NULL ) break;
		}
	RemoveAll();
	}


void CFDRecorderList::SetRecorderItem(CListCtrl& ListCtrl, int iItem, CFDRecorder *pcRecorder)
	{
	int iCol=0;
	ListCtrl.SetItemText(iItem,iCol++,pcRecorder->GetFDRID());
	ListCtrl.SetItemText(iItem,iCol++,pcRecorder->GetDateText());
	ListCtrl.SetItemText(iItem,iCol++,pcRecorder->GetCID() );
	ListCtrl.SetItemText(iItem,iCol++,pcRecorder->GetManufacturer());
	ListCtrl.SetItemText(iItem,iCol++,pcRecorder->GetHardwareVersion());
	ListCtrl.SetItemText(iItem,iCol++,pcRecorder->GetFileName());
	ListCtrl.SetItemText(iItem,iCol++,pcRecorder->GetENLText());
	ListCtrl.SetItemText(iItem,iCol++,pcRecorder->GetMOPText());
	ListCtrl.SetItemData(iItem, (LPARAM)pcRecorder);
	}

#define NUM_COLUMNS 8

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
{
	_T("FDR ID"), _T("Baseline Date"), _T("CID"), _T("Manufacturer"), _T("Hardware Version"), _T("Baseline File Name"),
	 _T("ENL (min/max)"), _T("MOP (min/max") 
};

static int _gnColumnFmt[NUM_COLUMNS] = 
{
	LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT,
		LVCFMT_LEFT, LVCFMT_LEFT
};

static int _gnColumnWidth[NUM_COLUMNS] = 
{
	100, 150, 100, 100, 50, 250, 100, 100
};

void CFDRecorderList::CreateControlColumns(CListCtrl& ListCtrl)
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

void CFDRecorderList::LoadRecorderList(CListCtrl& ListCtrl)
{
//	char buff[50];

	CFDRecorder* pcFDRecorder;

	LV_ITEM lvi;

	lvi.iSubItem = 0;
	lvi.mask = 0;

	ListCtrl.DeleteAllItems( );

	//  Load up the listbox
	int iItem=0;
	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		pcFDRecorder=GetNext(pos);
		if( pcFDRecorder )
			{
			lvi.iItem = iItem;
			lvi.lParam = (LPARAM)pcFDRecorder;
			ListCtrl.InsertItem(&lvi);
			SetRecorderItem(ListCtrl, iItem, pcFDRecorder);
			iItem++;
			}
		}
	}


	CFDRecorder* CFDRecorderList::GetByRecorderID(CString strFDRID)
		{
		CFDRecorder* pcFDRecorder;

		if( IsEmpty() !=0  )  return NULL;

		POSITION pos = GetHeadPosition();

		while(1)
			{
			pcFDRecorder=GetNext(pos);
			if( pcFDRecorder->GetFDRID()==strFDRID ) return pcFDRecorder;
			if( pos==NULL ) break;
			}
		return NULL;
		}

bool CFDRecorderList::RemoveByPointer(CFDRecorder *pcRemRecorder)
	{
	if( !pcRemRecorder )  return false ;
	CFDRecorder *pcRecorder;
	POSITION thispos, pos = GetHeadPosition();
	while( pos )
		{
		thispos=pos;
		pcRecorder=GetNext(pos);
		if( pcRecorder->GetFDRID()==pcRemRecorder->GetFDRID()) 
			{
			delete pcRecorder;
			RemoveAt(thispos);
			return true;
			}
		}
	return false;
	}


int CFDRecorderList::GetXML(CXMLMgr &cMgr, IDispatch *pIDOMNode )
	{

	int iNum=0;
    MSXML2::IXMLDOMNodePtr		pParentNode(pIDOMNode);
  
	MSXML2::IXMLDOMNodePtr pIDOMChildNode;
	TESTHR(cMgr.CreateChild(pParentNode, pIDOMChildNode, _T("Recorders")));

	if( IsEmpty() !=0  )  return 0;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CFDRecorder* pcRecorder=GetNext(pos);
		if( pcRecorder ) 
			{
			pcRecorder->GetXML( cMgr, pIDOMChildNode );;
			iNum++;
			}
		if( pos==NULL ) break;
		}

	return iNum;
	}

int CFDRecorderList::ImportXML(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr pWinscore)
	{
	int nImported=0;

	MSXML2::IXMLDOMNodePtr pRecorders;
	TESTHR(cMgr.SelectChild(_T("Recorders"), pWinscore, pRecorders));
	if( pRecorders==NULL ) return 0;

	MSXML2::IXMLDOMNodeListPtr pContestantList=cMgr.GetChildList(pRecorders);
	if( pContestantList==NULL ) return 0;

	long length=0;
	TESTHR(pContestantList->get_length(&length));
	if( length==0 ) return 0;

	for( int i=0; i<length; i++ )
		{
		MSXML2::IXMLDOMNodePtr pRecorder;
		TESTHR(pContestantList->get_item(i,&pRecorder));
		CFDRecorder* pcRecorder=NULL;
		try
			{
			pcRecorder=new CFDRecorder(cMgr, pRecorder );
			}
		catch(...)
			{
			ASSERT(FALSE);
			if(pcRecorder) delete pcRecorder;
			continue;
			}

		if( AddToList(pcRecorder) ) 
			nImported++;
		}

	return nImported;
	}

bool CFDRecorderList::AddToList(CFDRecorder * pcAddRecorder)
	{
	//  If a recorder with this name is there already, remove it
	if( !IsEmpty() ) 
		{
		POSITION thispos, pos = GetHeadPosition();
		while( pos!=NULL)
			{
			thispos=pos;
			CFDRecorder* pcRecorder=GetNext(pos);
			if( pcRecorder->GetFDRID()==pcAddRecorder->GetFDRID() ) 
				{
				delete pcRecorder;
				SetAt( thispos, (CContestant*)pcAddRecorder );
				return false;
				}
			}
		}
//  If we got here, it must be a new one
	AddTail( (CContestant*)pcAddRecorder );
	return true;
	}


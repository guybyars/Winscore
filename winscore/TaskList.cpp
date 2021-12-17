//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TaskList.cpp: implementation of the CTaskList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "TaskList.h"
#include "wsclass.h"
#include "global_prototypes.h"
#include <atlbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static int iSort;
static int compare_tasks( const void *arg1, const void *arg2 );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CTaskList::CTaskList()
{

}

CTaskList::~CTaskList()
{
	Purge();
	return;
}


CTask* CTaskList::GetByDateClass(CTime cDate, EClass eClass)
	{
	CSingleLock cLock(&m_cSem_GetByDateClass);

	CTask* pcTask;

	if( IsEmpty() !=0  )  return NULL;

	POSITION pos = GetHeadPosition();

	while(pos!=NULL)
		{
		pcTask=GetNext(pos);
		if( pcTask->m_cDate.GetDay()	==cDate.GetDay()  &&
			pcTask->m_cDate.GetMonth()	==cDate.GetMonth()  &&
			pcTask->m_cDate.GetYear()	==cDate.GetYear()  &&
			pcTask->m_eClass==eClass ) return pcTask;
		}
	return NULL;
	}

CTask* CTaskList::GetNext(POSITION &pos, CTime cDate )
	{
	CTask* pcTask;

	if( IsEmpty() !=0  )  return NULL;

	while(pos!=NULL)
		{
		pcTask=GetNext(pos);
		if( pcTask && DatesEqual(pcTask->m_cDate,cDate) ) return pcTask;
		}
	return NULL;
	}
void CTaskList::AddToList(CTask * pcAddTask)
	{

	CTask* pcNewTask = new CTask(pcAddTask);

	//  If a Task with this date is there already, remove it
	if( !IsEmpty() ) 
		{
		POSITION thispos, pos = GetHeadPosition();
		while( pos!=NULL)
			{
			thispos=pos;
			CTask* pcTask=(CTask*)GetNext(pos);
			if( DatesEqual(pcTask->m_cDate, pcAddTask->m_cDate)  &&
				pcTask->m_eClass==pcAddTask->m_eClass	) 
				{
				delete pcTask;
				SetAt( thispos, (CTask*)pcNewTask );
				return;
				}
			}
		}
//  If we got here, it must be a new one

	AddTail( (CTask*)pcNewTask );
	}


CTaskList& CTaskList::operator =(CTaskList& cInputTaskList)
{
	CTask *pcNewTask, *pcTask;

	//  Clean out the destination  
	Purge();

	if( cInputTaskList.IsEmpty() !=0  )  return *this;

	POSITION pos = cInputTaskList.GetHeadPosition();

	while(1)
		{
		pcTask=(CTask*)cInputTaskList.GetNext(pos);
		if( pcTask ) 
			{
			pcNewTask=new CTask(pcTask);
			(CTask*)AddTail(pcNewTask);
			}
		if( pos==NULL ) break;
		}

	return *this;

}


void CTaskList::Purge()
	{
	if( IsEmpty() !=0  )  return;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CTask* pcTask=GetNext(pos);
		if( pcTask ) delete pcTask;
		}
	RemoveAll();
	}


CTask* CTaskList::GetNext(POSITION& pos)
{
	return (CTask*)CObList::GetNext(pos);
}


void CTaskList::LoadTaskList(CListCtrl& ListCtrl, EUnits eUnits, CTurnpointArray& cTurnpointArray)
{
//	char buff[50];

	CTask* pcTask;

	LV_ITEM lvi;

	lvi.iSubItem = 0;
	lvi.mask = 0;

	ListCtrl.DeleteAllItems( );

	//  Load up the listbox
	int iItem=0;
	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		pcTask=GetNext(pos);
		if( pcTask )
			{
			int i=0;
			lvi.iItem = iItem;
			lvi.lParam = (LPARAM)pcTask;
			ListCtrl.InsertItem(&lvi);
			ListCtrl.SetItemText(iItem,i++,pcTask->DateText());
			ListCtrl.SetItemText(iItem,i++,GetClass(pcTask->m_eClass).GetName());
			ListCtrl.SetItemText(iItem,i++,pcTask->GetStatusText());
			ListCtrl.SetItemText(iItem,i++,pcTask->TypeText());
			ListCtrl.SetItemText(iItem,i++,pcTask->DistanceText(cTurnpointArray, eUnits));
			ListCtrl.SetItemText(iItem,i++,pcTask->TurnpointText());
			ListCtrl.SetItemText(iItem,i++,pcTask->TaskOpenText());

			ListCtrl.SetItemText(iItem,i++,pcTask->MinTimeText());

			ListCtrl.SetItemText(iItem,i++,pcTask->StartGateText(cTurnpointArray));
			ListCtrl.SetItemText(iItem,i++,pcTask->StartGateBText(cTurnpointArray));

			ListCtrl.SetItemText(iItem,i++,pcTask->GPSStartText(eUnits));
			ListCtrl.SetItemText(iItem,i++,pcTask->GPSFinishText(eUnits));
			ListCtrl.SetItemText(iItem,i++,pcTask->GetStartCeilingText());
			ListCtrl.SetItemText(iItem,i++,pcTask->GetFinishBaseText());
			ListCtrl.SetItemData(iItem, (LPARAM)pcTask);
			iItem++;
			}
		}
	}


BOOL CTaskList::NoContestDay(CTime cDate, EClass eClass)
{
CTask* pcTask=GetByDateClass( cDate,  eClass);

if( pcTask==NULL ) return TRUE;

if( pcTask->m_eStatus==eNoContest ) return TRUE;

return FALSE;

}

void CTaskList::Merge(CTaskList &cInputTaskList,bool bAllowDuplicates)
{
	CTask *pcNewTask, *pcTask;

	if( cInputTaskList.IsEmpty() !=0  )  return;

	POSITION pos = cInputTaskList.GetHeadPosition();

	while(1)
		{
		pcTask=(CTask*)cInputTaskList.GetNext(pos);
		if( pcTask ) 
			{
			if( !bAllowDuplicates ) Purge( pcTask->m_eClass, pcTask->m_cDate);
			pcNewTask=new CTask(pcTask);
			(CTask*)AddTail(pcNewTask);
			}
		if( pos==NULL ) break;
		}

	return;
}

void CTaskList::Purge(EClass eClass, CTime cDate)
	{
	if( !IsEmpty() ) 
		{
		POSITION thispos, pos = GetHeadPosition();
		while( pos!=NULL)
			{
			thispos=pos;
			CTask* pcTask=(CTask*)CObList::GetNext(pos);
			if( DatesEqual(pcTask->m_cDate,cDate)		&&
				pcTask->m_eClass==eClass	) 
				{
				delete pcTask;
				CObList::RemoveAt( thispos );
				}
			}
		}
	}

void CTaskList::Remove(CTask* pcRemoveTask)
	{
	if( !IsEmpty() ) 
		{
		POSITION thispos, pos = GetHeadPosition();
		while( pos!=NULL)
			{
			thispos=pos;
			CTask* pcTask=(CTask*)CObList::GetNext(pos);
			if( pcTask==pcRemoveTask ) 
				{
				delete pcTask;
				CObList::RemoveAt( thispos );
				return;
				}
			}
		}
	}

#define NUM_COLUMNS 14

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
{
	_T("Date"),		 
	_T("Class"),
	_T("Status"),
	_T("Type"), 
	_T("Dist."),
	_T("Control. Pnts."), 
	_T("Task Open"),
	_T("Min Time"),
	_T("Start Gate"),	
	_T("Start Gate B"), 
	_T("GPS Start Radius"),	
	_T("GPS Finish Radius"), 
	_T("GPS Start Cylinder Ceiling"),	
	_T("GPS Finish Cylinder Base") 
							};
	
static int _gnColumnFmt[NUM_COLUMNS] = 
{
	LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT,
	LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT,
	LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT,
	LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT
};

static int _gnColumnWidth[NUM_COLUMNS] = 
{	100, 80,  90,  90,
	70,  100, 100, 100,
	100, 100, 100, 100, 
	100, 100};

void CTaskList::CreateControlColumns(CListCtrl& ListCtrl)
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

CTask* CTaskList::Get(CTime &cTime, int iItem)
{
	int nFound=0;
	if( IsEmpty() !=0  )  return NULL;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CTask* pcTask=GetNext(pos, cTime);
		if( nFound==iItem ) return pcTask;
		nFound++;
		}
	return NULL;

}

int CTaskList::GetNumTasks(CTime &cDate, int nClasses, EClass aeClasses[])
	{
	int nTasks=0;
	for( int i=0; i<nClasses; i++ )
		{
		CTask* pcTask=GetByDateClass( cDate, aeClasses[i]);
		if( pcTask!=NULL ) nTasks++;
		}
	return nTasks;
	}


CTask* CTaskList::ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pContestDay)
	{
	MSXML2::IXMLDOMNodePtr pTask;
	TESTHR(cMgr.SelectChild(_T("Task"),pContestDay,pTask));
	if( pTask==NULL ) return 0;
	
	try
		{
		CTask *pcTask=new CTask(cMgr, pTask);

		AddToList(pcTask);
		return pcTask;

		}
	catch(...)
		{
		return NULL;
		}

	return NULL;
	}


int CTaskList::ImportXML2(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pWinscore, CString strFilePathName)
	{
	CString strFileName;
	int nImported=0;

	MSXML2::IXMLDOMNodePtr pTasks;
	TESTHR(cMgr.SelectChild(_T("Tasks"),pWinscore,pTasks));
	if( pTasks==NULL ) return 0;

	MSXML2::IXMLDOMNodeListPtr pTaskList=cMgr.GetChildList(pTasks );
	if( pTaskList==NULL ) return 0;

	long length=0;
	TESTHR(pTaskList->get_length(&length));
	if( length==0 ) return 0;

	if( strFilePathName.GetLength()>0 )
		{
		int iFind=strFilePathName.ReverseFind('\\');
		if( iFind>0 ) strFileName=strFilePathName.Right(strFilePathName.GetLength()-iFind-1);
		}

	for( int i=0; i<length; i++ )
		{
		MSXML2::IXMLDOMNodePtr pTask;
		TESTHR(pTaskList->get_item(i,&pTask));
		CTask* pcTask=NULL;
		try
			{
			pcTask=new CTask( cMgr, pTask );
			}
		catch(...)
			{
			ASSERT(FALSE);
			if(pcTask) delete pcTask;
			continue;
			}
		if( strFileName.GetLength()>0 )
			{
			pcTask->m_strFileName=strFileName;
			}
		AddTail(pcTask);
		nImported++;
		}

	return nImported;
	}



void CTaskList::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode)
	{
	if( IsEmpty()!=0  )  return;
	
	MSXML2::IXMLDOMNodePtr pIDOMChildNode=NULL;
	cMgr.CreateChild( pParentNode, pIDOMChildNode, _T("Tasks"));


	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CTask* pcTask=GetNext(pos);
		if( pcTask ) pcTask->GetXML( cMgr, pIDOMChildNode );;
		if( pos==NULL ) break;
		}

	return;
	}


int CTaskList::LoadTaskLibrary(CString strLibraryPath)
	{
	CFileFind cFindFile;
	CString strSearchName;
	int nLoaded=0;
	bool bWinscoreFile;

	strSearchName.Format(_T("%s\\*.xml"),  strLibraryPath  );

	BOOL fMore=cFindFile.FindFile(strSearchName);
	while(fMore )
		{
		fMore=cFindFile.FindNextFile();
		nLoaded+=Import(cFindFile.GetFilePath(),bWinscoreFile);
		}

	return nLoaded;
	}

int CTaskList::Import(CString strXMLFileName,bool &bWinscoreFile)
	{
	try
		{
		CXMLMgr cMgr;
		cMgr.Initlize();

		if( !cMgr.Load(strXMLFileName) ) return 0;

		CString strRoot=cMgr.GetNodeName(cMgr.GetRoot());
		if(strRoot!=_T("Winscore") ) return 0;

		MSXML2::IXMLDOMNodePtr pRoot=cMgr.GetRoot();

		CString str;
		HRESULT hr=cMgr.SelectChildSTR(pRoot,"Name",str);
		bWinscoreFile=str.GetLength()>0;


		return ImportXML2(cMgr, pRoot, strXMLFileName);
		}
	catch (...)
		{
		return 0;
		}
	return 0;
	}

int CTaskList::Export(CString strXMLFileName)
	{
	try
		{
		CXMLMgr cMgr;
		cMgr.Initlize();
		cMgr.CreateXMLFileHeader();
		cMgr.CreateProcInstruction(_T("xml-stylesheet"),_T("type=\"text/xsl\" href=\"ws_scoresheet.xsl\""));
		cMgr.CreateRoot(_T("Winscore"));

		MSXML2::IXMLDOMNodePtr pIDOMRootNode=cMgr.GetRoot();
		GetXML(cMgr, pIDOMRootNode );

		TESTHR(cMgr.Save(strXMLFileName) );
		}
	catch(...)
		{
		return 0;
		}
	return GetCount();
	}


void CTaskList::Sort( int iSortColumn )
{
	int nTasks=GetCount();
	int i=0;
	CTask** ppcTasks;

	ppcTasks=new CTask*[nTasks];

	POSITION pos = GetHeadPosition();
	while(pos)
		{
		ppcTasks[i++]=GetNext(pos);
		}

	if( i>0 )
		{
		iSort=iSortColumn;
		qsort( (void*)ppcTasks, i, sizeof(CTask*), compare_tasks );

		RemoveAll();

		for( int iCnt=0; iCnt<i; iCnt++ )
			{
			AddTail( (CContestant*)ppcTasks[iCnt] );
			}; 

		}
	delete ppcTasks;

}
int compare_tasks( const void *arg1, const void *arg2 )
{
	 if( abs(iSort)==1 )
		{
		if( (*(CTask**)arg1)->m_strFileName<(*(CTask**)arg2)->m_strFileName  )
			return iSort<0?1:-1;
		else if( (*(CTask**)arg1)->m_strFileName>(*(CTask**)arg2)->m_strFileName  )
			return iSort<0?-1:1;
		}
	 else if( abs(iSort)==2 )
		{
		// Sort By type
		if( (*(CTask**)arg1)->m_eType<(*(CTask**)arg2)->m_eType  )
			return iSort<0?1:-1;
		else if( (*(CTask**)arg1)->m_eType>(*(CTask**)arg2)->m_eType  )
			return iSort<0?-1:1;
		else
			{// In a tie, subsort by distance
			if( (*(CTask**)arg1)->m_dColumnSortDistance[1]<(*(CTask**)arg2)->m_dColumnSortDistance[1]  )
				return 1;
			else if( (*(CTask**)arg1)->m_dColumnSortDistance[1]>(*(CTask**)arg2)->m_dColumnSortDistance[1]  )
				return -1;
			}
		}
	 else if( abs(iSort)==3 || abs(iSort)==4 || abs(iSort)==5 )
		{
		// Sort By Distance
		if( (*(CTask**)arg1)->m_dColumnSortDistance[abs(iSort)-3]<(*(CTask**)arg2)->m_dColumnSortDistance[abs(iSort)-3]  )
			return iSort<0?1:-1;
		else if( (*(CTask**)arg1)->m_dColumnSortDistance[abs(iSort)-3]>(*(CTask**)arg2)->m_dColumnSortDistance[abs(iSort)-3]  )
			return iSort<0?-1:1;
		}
	 else if( abs(iSort)==6 )
		{
		// Sort By Task Time
		if( (*(CTask**)arg1)->m_cPostTime<(*(CTask**)arg2)->m_cPostTime  )
			return iSort<0?1:-1;
		else if( (*(CTask**)arg1)->m_cPostTime>(*(CTask**)arg2)->m_cPostTime  )
			return iSort<0?-1:1;
		else
			{// In a tie, subsort by distance
			if( (*(CTask**)arg1)->m_dColumnSortDistance[1]<(*(CTask**)arg2)->m_dColumnSortDistance[1]  )
				return 1;
			else if( (*(CTask**)arg1)->m_dColumnSortDistance[1]>(*(CTask**)arg2)->m_dColumnSortDistance[1]  )
				return -1;
			}
		}
	 else if( abs(iSort)==7 )
		{
		// Sort By turnpoint
		if( (*(CTask**)arg1)->GetNumTurnpoints()<(*(CTask**)arg2)->GetNumTurnpoints()  )
			return iSort<0?1:-1;
		else if( (*(CTask**)arg1)->GetNumTurnpoints()>(*(CTask**)arg2)->GetNumTurnpoints()  )
			return iSort<0?-1:1;
		else
			{
			int iResult=strcmp(  (*(CTask**)arg1)->TurnpointText(),
								 (*(CTask**)arg2)->TurnpointText() );
			return 	iSort<0?-iResult:iResult;
			}
		}
	 else if( abs(iSort)==8 )
		{
		// Sort By start gate
			if( (*(CTask**)arg1)->m_cStartGate.GetPointID()<(*(CTask**)arg2)->m_cStartGate.GetPointID()  )
			return iSort<0?1:-1;
		else if( (*(CTask**)arg1)->m_cStartGate.GetPointID()>(*(CTask**)arg2)->m_cStartGate.GetPointID()  )
			return iSort<0?-1:1;
		}
	return 0;
}


int CTaskList::ExportToTaskLibrary()
{
	CString strFileName;
	bool		bWinscoreFile=false;
	char *pAlphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	for( int i=0; i<(int)strlen(pAlphabet); i++ )
		{
		strFileName.Format("%s\\Task_%c.xml",GetTaskLibraryPath(),pAlphabet[i]);
		CFileStatus rStatus;
		if( !CFile::GetStatus(strFileName, rStatus) ) break;
		}

	static char BASED_CODE szFilter[] = "XML Files (*.xml)|*.xml|All Files (*.*)|*.*||";
	CFileDialog  cFileDlg( false, "xml", strFileName, OFN_HIDEREADONLY, szFilter);

	CString strPath=AfxGetApp()->GetProfileString( REGSECTION, EXPORTPATH  );
	cFileDlg.m_ofn.lpstrInitialDir=strPath;

	if( cFileDlg.DoModal()==IDOK )
		{
		CString strCheck=cFileDlg.GetPathName();
		strCheck.MakeLower();
		if( strCheck.Find(".xml")<0 )
			{
			CString strWarn;
			strWarn.Format("%s is invalid for task export, it must have a file type of .xml",cFileDlg.GetFileName() );
			AfxMessageBox(strWarn,MB_ICONSTOP);
			return 0;
			}
		CFileStatus rStatus;
		if( !CFile::GetStatus(cFileDlg.GetPathName(), rStatus) )
			{
			//File doesn't exist, save it normally
			return Export(cFileDlg.GetPathName());
			}
		else
			{
			//File exists, merge iff it contains ONLY tasks
			CTaskList	cTaskList;
			int nTasks=cTaskList.Import(cFileDlg.GetPathName(),bWinscoreFile);
			if( bWinscoreFile )
				{
				CString strWarn;
				strWarn.Format("%s is a Winscore contest file, task export cannot overwrite it.",cFileDlg.GetFileName() );
				AfxMessageBox(strWarn,MB_ICONSTOP);
				return 0;
				}
			if( nTasks>0 )
				{// File has valid tasks, OK to merge
				CString strOverWrite;
				strOverWrite.Format("%s already exists and contains %d task(s), do you want to add your task(s) to this file?\n\nYES - Add task(s) to the selected file.\n\nNO - Overwite the selected file with this task(s).\n\nCANCEL - Do nothing right now.",cFileDlg.GetFileName(),nTasks);
				int iAnswer= AfxMessageBox( strOverWrite, MB_ICONQUESTION  | MB_YESNOCANCEL );
				if( iAnswer==IDYES )
					{
					Merge(cTaskList,true);
					if( Export(cFileDlg.GetPathName()) >0 )
						return GetCount();
					else
						return 0;
					}
				else if( iAnswer==IDNO )
					{
					return Export(cFileDlg.GetPathName());
					}
				else
					return 0;
				}
			else
				{// No valid tasks in file, overwrite it.
				CString strOverWrite;
				strOverWrite.Format("%s already exists, Overwrite?",cFileDlg.GetFileName());
				if(  AfxMessageBox( strOverWrite,MB_ICONEXCLAMATION | MB_YESNO )==IDYES )
					{
					return Export(cFileDlg.GetPathName());
					}
				}
			}
		}
return 0;
}
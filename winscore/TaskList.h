//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TaskList.h: interface for the CTaskList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKLIST_H__C923E5B2_4F74_11D1_ACE5_000000000000__INCLUDED_)
#define AFX_TASKLIST_H__C923E5B2_4F74_11D1_ACE5_000000000000__INCLUDED_


#include "Task.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTaskList : public CObList  
{
public:
	void GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode);
	CTask* ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pTask);
	int ImportXML2	(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pTask, CString strFileName="");

	int GetNumTasks(CTime &cDate, int nClasses, EClass aeClasses[] );
	CTask* Get(CTime& cTime, int iItem);
	void Merge(CTaskList &cInputTaskList,bool bAllowDuplicates=false);
	BOOL NoContestDay(CTime cTime, EClass eClass);
	void CTaskList::Remove(CTask* pcRemoveTask);

	void Sort(int iSortColumn );


	CSemaphore m_cSem_GetByDateClass;

	CTaskList();
	virtual ~CTaskList();

	void CreateControlColumns( CListCtrl& );
	void LoadTaskList(CListCtrl&, EUnits eUnits, CTurnpointArray& cTurnpointArray);
	void Purge(EClass eClass, CTime cDate);


	CTask* GetNext( POSITION& pos);
	CTask* CTaskList::GetNext(POSITION &pos, CTime cDate );

	void Purge();
	CTaskList& operator =(CTaskList&);
	void AddToList(CTask* pcTask);
	CTask* CTaskList::GetByDateClass(CTime cDate, EClass eClass);

	int LoadTaskLibrary(CString strLibraryPath);
	int Import(CString strXMLFileName,bool &bWinscoreFile);
	int Export(CString strXMLFileName);
	int ExportToTaskLibrary();
};

#endif // !defined(AFX_TASKLIST_H__C923E5B2_4F74_11D1_ACE5_000000000000__INCLUDED_)


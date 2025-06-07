#pragma once
#include "afxcoll.h"

#include "FDRecorder.h"

class CFDRecorderList :
	public CObList
{
public:
	CFDRecorderList(void);
	~CFDRecorderList(void);

	int ImportXML(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr pIDOMWinscoreNode );
	int GetXML	 (CXMLMgr &cMgr, IDispatch *pIDOMNode );

	CFDRecorder* GetByRecorderID(CString strFDRID);
	CFDRecorder* GetNext( POSITION& pos);
	void Purge();
	bool AddToList(CFDRecorder* pcRecorder);

	void CreateControlColumns( CListCtrl& );
	void LoadRecorderList(CListCtrl&);
	void SetRecorderItem(CListCtrl& ListCtrl, int iItem, CFDRecorder *pcRecorder);
	bool RemoveByPointer(CFDRecorder *pcRecorder);


};

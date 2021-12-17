// GliderInfoList.h: interface for the CGliderInfoList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLIDERINFOLIST_H__B2218A5F_B7E9_4BE6_9A40_AB1D01B58B85__INCLUDED_)
#define AFX_GLIDERINFOLIST_H__B2218A5F_B7E9_4BE6_9A40_AB1D01B58B85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gliderinfo.h"

class CGliderInfoList : public CObList    
{
public:
	CString m_strTitle;
	CStringArray m_strCodeIDs;
	CStringArray m_strCodeDescriptions;
//
	CGliderInfo* Get(CString strManufacturer, CString strModel,CString strNotes="");
	int GetModels(CString strManufacturer, CStringArray &strArray, CString strNotes="", float fMaxSpan=100);
	int GetModels( CString strManufacturer, CPtrArray &ptrArray);
	CGliderInfo* GetModel( CString strModel, BOOL bMotorPreference=false, float fSpan=0.0);
	int GetManufacturers(CStringArray &strArray, CString strNotes="", float fMaxSpan=100);
	int ReadFromFile(CString strFileName);
	int ImportXML(CString strFileName,bool bSilent=false);
	bool ExportXML(CString strFileName,bool bSilent=false);
	CGliderInfoList();
	virtual ~CGliderInfoList();
	bool IsManInList(CStringArray &strArray, CString strManu);
	void Purge(void);	 
	bool operator ==(CGliderInfoList &cInfoList);
	int GetAllModels(CStringArray &strArray);
};


#endif // !defined(AFX_GLIDERINFOLIST_H__B2218A5F_B7E9_4BE6_9A40_AB1D01B58B85__INCLUDED_)

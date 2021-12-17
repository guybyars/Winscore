// WSSingleDocTemplate.h: interface for the CWSSingleDocTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WSSINGLEDOCTEMPLATE_H__788C49A2_0FBA_4BBF_8064_042B9F6CD5D7__INCLUDED_)
#define AFX_WSSINGLEDOCTEMPLATE_H__788C49A2_0FBA_4BBF_8064_042B9F6CD5D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWSSingleDocTemplate : public CSingleDocTemplate  
{
public:
	virtual ~CWSSingleDocTemplate();

   CWSSingleDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass );


   BOOL CWSSingleDocTemplate::GetDocString(CString& rString, enum DocStringIndex i) const;


   CDocTemplate::Confidence CWSSingleDocTemplate::MatchDocType(const char* pszPathName, CDocument*& rpDocMatch);


};

#endif // !defined(AFX_WSSINGLEDOCTEMPLATE_H__788C49A2_0FBA_4BBF_8064_042B9F6CD5D7__INCLUDED_)

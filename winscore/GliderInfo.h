// GliderInfo.h: interface for the CGliderInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLIDERINFO_H__59F6B5F9_38EF_4215_9BF9_ACE2C350396F__INCLUDED_)
#define AFX_GLIDERINFO_H__59F6B5F9_38EF_4215_9BF9_ACE2C350396F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xmlmgr.h"

class CGliderInfo: public CObject
{

	DECLARE_SERIAL(CGliderInfo)

public:
	bool IsNull();
	CGliderInfo();

	CGliderInfo( CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pGlider);


	CGliderInfo(	CString strManufacturer,
					CString strModel,
					CString strNotes,
					float	fSpan,
					double	dHandicap,
					float	fWeight	);
	virtual ~CGliderInfo();
	virtual void Serialize(CArchive & ar);

	CGliderInfo& operator =(CGliderInfo &cInfo);
	bool operator ==(CGliderInfo &cInfo);
	bool GetXML(CXMLMgr &cMgr, IDispatch *pIdsp );


	CString m_strManufacturer;
	CString m_strModel;
	CString m_strNotes;
	CString m_strModified;
	float m_fSpan;
	double m_dHandicap;
	float m_fWeight;
	bool  m_bNull;
};

#endif // !defined(AFX_GLIDERINFO_H__59F6B5F9_38EF_4215_9BF9_ACE2C350396F__INCLUDED_)

//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Turnpoint.h: interface for the CTurnpoint class.
//
//////////////////////////////////////////////////////////////////////
#include "Location.h"
#include "xmlmgr.h"

#if !defined(AFX_TURNPOINT_H__7CBA71A1_3B0C_11D1_ACC4_000000000000__INCLUDED_)
#define AFX_TURNPOINT_H__7CBA71A1_3B0C_11D1_ACC4_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//  If these attributes are changed, need to change in Task OCX too.
#define TPT_HOME              0x0001
#define TPT_START             0x0002
#define TPT_FINISH            0x0004
#define TPT_AIRFIELD          0x0008
#define TPT_CONTROL           0x0010

class CTurnpoint : public CLocation  
{

protected:

    CTurnpoint();
	DECLARE_SERIAL(CTurnpoint)

public:
	CTurnpoint(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pWaypoint);
	int GetElevation();
	int GetID();
	BOOL HasAttribute( int iAtt );
	void LoadListItem( CListCtrl& ListCtrl, int iItem,  ECoordFormat eCoordFormat);
	bool IsControlPoint();
	bool IsTurnPoint();
	bool IsAirfield();
	CString AttributeText();
	CString ElevationText();
	CString GetNoNameText();

	CString m_strName;
	int m_iID;
	int m_iAttribute;
	int m_iElevation;

	CTurnpoint(int iID, CString str, CLocation* pcLocation, int iAttribute=0, int iElevation=0);
    CTurnpoint(CString strLCAMField);
	bool GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pParentNode  );

	virtual ~CTurnpoint();

    CString CTurnpoint::GetFormattedInfo(ECoordFormat);
	CString CTurnpoint::IdText();
	CString CTurnpoint::HHMMSSLatText();
	CString CTurnpoint::HHMMSSLongText();
	CString CTurnpoint::HHHMMMLatText();
	CString CTurnpoint::HHHMMMLongText();

};

int CALLBACK CompareTurnpoint(LPARAM lParam1, LPARAM lParam2, 
    LPARAM iColumn);

#endif // !defined(AFX_TURNPOINT_H__7CBA71A1_3B0C_11D1_ACC4_000000000000__INCLUDED_)


//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TurnpointArray.h: interface for the CTurnpointArray class.
//
//////////////////////////////////////////////////////////////////////
#include <afxcoll.h>
#include "Turnpoint.h"
#include "TurnpointListCtrl.h"
#include "wsview.h"
#include "xmlmgr.h"

#if !defined(AFX_TURNPOINTARRAY_H__BAD124F3_4095_11D1_ACCF_000000000000__INCLUDED_)
#define AFX_TURNPOINTARRAY_H__BAD124F3_4095_11D1_ACCF_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTurnpointArray : public CObArray  
{
public:

//	int ImportXML(IDispatch *pIDOMDocument);
	int ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pIDOMWinscoreNode);

	bool GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pIDOMNode );
	double GetMaxDistanceFromHome();
	CTurnpoint* GetFirstByAttribute(int iAttribute);
	void AddToControl( CWSView &cWSView);
	CString GetText( ECoordFormat eCoordFormat);
	CString MatrixText(EUnits eUnits);
	CTurnpoint* FindClosestAirport(CLocation cLoc, double dToler);
	int GetValidTpts();
	int ImportTurnpoints( CString cFileName, int iIndex );
//	void LoadList( int nList, int aiList[], CTurnpointListCtrl& cTurnpointListCtr );

	void LoadTurnpointComboBox(CComboBox &cComboBox, bool bOnlyControl=true);
	int  LoadTurnpointComboBox(CComboBox &cComboBox, int iAttribute, int iDefaultID );
	void LoadTurnpointComboBox(CComboBox &cComboBox, int nTurnpoints, int aiTurnpoints[]);

	void LoadTurnpointGridList(CListCtrl& ListCtrl, EUnits eUnits);
	void CreateGridControlColumns(CListCtrl& ListCtrl);
	void Purge();
	CTurnpoint* GetHomePoint(CTurnpoint* pClosest=NULL);
	void LoadTurnpointList(CListCtrl&, ECoordFormat eCoordFormat , bool bControlPointsOnly=FALSE);
	CTurnpoint* operator [](int i);
	void RemoveAt(int i);
	CTurnpoint* GetAt(int i);
	void SetAt( int i, CTurnpoint* );
	CTurnpointArray& operator=(CTurnpointArray&);
	CTurnpointArray();
	virtual ~CTurnpointArray();
	void CreateControlColumns(CListCtrl& ListCtrl, ECoordFormat eCoordFormat);

	int NumberSummaryPages();
	void DrawTurnpointList(	CDC *pDC, 
							CString &ContestName,
							CString &ContestLocation,
							CTime	&cDate, 
							int		iPageID,
							ECoordFormat eFormat);
};

#endif // !defined(AFX_TURNPOINTARRAY_H__BAD124F3_4095_11D1_ACCF_000000000000__INCLUDED_)


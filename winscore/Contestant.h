//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Contestant.h: interface for the CContestant class.
//
//////////////////////////////////////////////////////////////////////


using namespace std;

#include <afx.h>
#include "winscore.h"
#include "GliderInfo.h"
#include "GliderInfoList.h"
#include "xmlmgr.h"
#include <iostream>

#if !defined(AFX_CONTESTANT_H__23C6BD61_40EF_11D1_ACD0_000000000000__INCLUDED_)
#define AFX_CONTESTANT_H__23C6BD61_40EF_11D1_ACD0_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define	CURRENT_CONTESTANT_SCHEME	3

#define CON_EXCLUDED      0x00001
#define CON_MOTORIZED     0x00002
#define CON_WITHDRAWN	  0x00004
#define CON_IN_B_GATE	  0x00008
#define CON_FOREIGN 	  0x00010
#define CON_WINGLETS 	  0x00020
#define CON_TURBULATION	  0x00040
#define CON_ROOTFARINGS	  0x00080

class CContestant : public CObject  
{
	
private:
	CString m_strFDR_ID;

protected:	
	DECLARE_SERIAL(CContestant)

public:
	CContestant(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pContestant, bool bSSA=false);

	bool GetXML(CXMLMgr &cMgr, IDispatch *pIDOMDoc );
	bool GetSSAXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pIDOMNode );

	CString GetWingletText();
	CString GetSpanText();
	CString GetWeightText();
	CString GetGateText();
	bool IsGuest();

	CString GetFDRID();
	void SetFDRID(CString strFDRID );

	void ExcludeFromFlightFiles(BOOL b);
	bool IsExcludedFromFlightFiles() {return m_iOptions & CON_EXCLUDED;};

	BOOL HasWithdrawn() {return (m_iOptions & CON_WITHDRAWN)?(true):(false);};
	void Withdrew(BOOL b);


	BOOL IsForeign()  {return (m_iOptions & CON_FOREIGN)?(true):(false);};
	void Foreign(BOOL b);

	BOOL IsMotorized()  {return (m_iOptions & CON_MOTORIZED)?(true):(false);};
	void Motorized(BOOL b);

	BOOL IsInBGroup()  {return (m_iOptions & CON_IN_B_GATE)?(true):(false);};
	void SetBGroup(BOOL b);

	CContestant();
	CString  operator =(CContestant* pcContestant);
	CString HandicapText();
	CString AddressText();
	CString CitizenText();
	CString NameText();
	CString SSANumberText();
	CString	GetText();

	CString GetFormattedInfo();

	CString m_strContestNo;
	CString m_strLastName;
	CString m_strMiddleName;
	CString m_strFirstName;
	CString m_strGlider;
	CString m_strAddress1;
	CString m_strAddress2;
	CString m_strCity;
	CString m_strState;
	CString m_strZipcode1;
	CString m_strZipcode2;


	int		m_iSSANumber;
	double	m_fHandicap;
	EClass	m_eClass;
	bool	m_bGuest;

	// These values are the actual parameters of the contestant's glider
	// similar values stored in the giderinfo class are for a "stock" glider
	float	m_fSpan;
	double	m_fWinglet;
	float	m_fWeight;


	CGliderInfo m_cGliderInfo;

	int	  m_iOptions;

	//   Functions  -----------------------------------------
	CContestant(	CString &strContestNo,
					CString &strLastName,
					CString &strMiddleName,
					CString &strFirstName,
					CString &strGlider,
					CString &strAddress1,
					CString &strAddress2,
					CString &strCity,
					CString &strState,
					CString &strZipcode1,
					CString &strZipcode2,
					CString &strFDR_ID,
					int iSSANumber,
					double fHandicap,
					EClass eClass,
					BOOL	bGuest,
					BOOL	bMotorized,
					BOOL    bWithdrew,
					BOOL    bIsInBGroup,
					BOOL    bForeign);

	CContestant(CContestant*);
	virtual ~CContestant();

	BOOL CContestant::CheckOption(int iOption) { return m_iOptions & iOption; }
	void CContestant::SetOption(int iOption) {m_iOptions|=iOption;}
	void CContestant::ClearOption(int iOption) { if( m_iOptions|=iOption ) m_iOptions^=iOption; }
	void UpdateHandicap(CGliderInfoList &cGIList);
};

int CALLBACK CompareContestantName(LPARAM lParam1, LPARAM lParam2, 
    LPARAM lParamSort);

ostream &operator <<(ostream &os, CContestant& cCon);
istream &operator >>(istream &is, CContestant& cCon);
CString ExtractString(istream &is);
void PutString(ostream &os, const CString &str);

#endif // !defined(AFX_CONTESTANT_H__23C6BD61_40EF_11D1_ACD0_000000000000__INCLUDED_)


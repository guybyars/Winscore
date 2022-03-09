//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// WSClass.h: interface for the CWSClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WSCLASS_H__964659C6_A19C_11D3_B124_7030F7000000__INCLUDED_)
#define AFX_WSCLASS_H__964659C6_A19C_11D3_B124_7030F7000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "winscoredefs.h"
#include "summary.h"


#define WSC_HANDICAPPED		0x00001
#define WSC_SHORTTASKFACTOR	0x00002
#define WSC_COMPLETIONRATIO 0x00004
#define WSC_ACTIVE			0x00008
#define WSC_USINGWDSA		0x00010

class CWSClass : public CObject  
{
protected:

	DECLARE_SERIAL(CWSClass)

public:
	void ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pClass);
	void GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode);
	void SetMinTaskTime(double dHours);

	CWSClass& CWSClass::operator =(CWSClass &cClass);

	void Initialize( EContest eContest, EClass eClass );
//	CWSClass(CString strName, EContest eContest, EClass eClass );
	CWSClass();
	virtual ~CWSClass();

	double	GetMinTaskDistance(EUnits eUnits);
	void	SetMinTaskDistance(double dDistance)  { m_dSMTD=dDistance; }

	CTimeSpan	GetMinTaskTime();

	void		SetName(CString str)	 { m_strName=str; }
	CString		GetName(bool bWithClass=true);
	EClass		GetType()		  { return m_eClass; }
	EContest	GetContest()	  { return m_eContest; }

	bool		IsHandicapped()			{return (m_iOptions & WSC_HANDICAPPED)?(true):(false);};
	bool		IsActive()				{return (m_iOptions & WSC_ACTIVE)?(true):(false);};
	bool		IsUseShortTaskFactor()	{return (m_iOptions & WSC_SHORTTASKFACTOR)?(true):(false);};
	bool		IsUseScoredCompletionRatio()		  {return (m_iOptions & WSC_COMPLETIONRATIO)?(true):(false);};
	bool		IsUsingWDSA()			{return (m_iOptions & WSC_USINGWDSA)?(true):(false);};
	bool		IsAirfieldBonusActive() {return m_iAirfieldBonusPoints>0;};

	void		SetHandicapped(bool b);
	void		SetActive(bool b);
	void		SetUseShortTaskFactor(bool b);
	void		SetUseScoredCompletionRatio(bool b);
	void		SetUsingWDSA(bool b);
	
// Score Calculation methods

	double	GetMaximumSpeedPoints(ETaskType eTaskType, CSummary &cSummary);
	double	GetMaximumDistancePoints(ETaskType eTaskType, CSummary &cSummary);
	//double	GetNoFinisherPoints(ETaskType eTaskType);
	void	GetPoints( CTask *pcTask, CObject *pcScoreRecord,  CSummary &cSummary);

	double GetCompletionRatio(ETaskType eTaskType, CSummary &cSummary);
	double GetShortTaskFactor(CSummary &cSummary);
	double GetContestantFactor(CSummary &cSummary);

	CString m_strName;

	EContest	m_eContest;
	EClass		m_eClass;

	int		m_iAirfieldBonusPoints;
	double	m_dNoFinisherPoints_ASGNED;
	double	m_dMDP_M_ASGNED;
	double	m_dMDP_B_ASGNED;
	double	m_dMSP_M_ASGNED;
	double	m_dMSP_B_ASGNED;
	double	m_dNoFinisherPoints_POST;
	double	m_dMDP_M_POST;
	double	m_dMDP_B_POST;
	double	m_dMSP_M_POST;
	double	m_dMSP_B_POST;
	CTimeSpan m_cSMTT;
	double	m_dSMTD;
	double  m_dD1;
	double  m_dDm;


private:

	int m_iOptions;


};


/////////////  class globals ////////////////////////////

CWSClass& GetClass(int i);

CWSClass& GetClass(EClass eClass, bool bActiveOnly=true);

CWSClass& GetClass(EContest eContest, EClass eClass);

int GetClassPos(EContest eContest, EClass eClass);

bool IsClassValid(EClass eClass);

#endif // !defined(AFX_WSCLASS_H__964659C6_A19C_11D3_B124_7030F7000000__INCLUDED_)


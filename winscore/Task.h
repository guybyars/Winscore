//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Task.h: interface for the CTask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASK_H__C923E5B1_4F74_11D1_ACE5_000000000000__INCLUDED_)
#define AFX_TASK_H__C923E5B1_4F74_11D1_ACE5_000000000000__INCLUDED_

#include "TurnpointArray.h"
#include "gate.h"
#include "contestant.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTask : public CObject  
{

protected:	
	DECLARE_SERIAL(CTask);

public:
	void Initialize();
	CString GetText(CTurnpointArray &turnpointArray,  EUnits eUnits);

	bool		GetXML(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr &pParentNode);
	bool		GetSSAXML(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr &pParentNode,EUnits eUnits, CTurnpointArray  &cTurnpointArray, bool bDateOutput=false);
	ETaskType&	GetType() {return m_eType;}
	CGate&		GetFinishGate() {return m_cFinishGate;}
	CGate&		GetActiveStartGate(CContestant *pcContestant);
	void		GetTurnAreaDistances( CTurnpointArray &turnpointArray, double &dMin, double &dNominal, double &dMax );
	bool CheckTurnAreaDistances(	CTurnpointArray &cTurnpointArray);
	bool		LoadXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pTask, bool bFromLibrary);

	CTask & operator=(CTask&);
	static int GetCurrentSchema();
	void AddToControl( CTurnpointArray &turnpointArray, CWSView &cWSView, int iGateFlag = 0, CGate *pGate=NULL, bool bGatesOnly=false );
	BOOL GPSFinishCylinder();
	BOOL GPSStartCylinder();
	CString GetFinishBaseText();
	CString GetStartCeilingText();
	CString TurnpointText();
	CString TaskOpenText();
	double m_dSTIFactor;

	int		GetTurnpointID(int i) {return m_aiTurnpointIDs[i];}
	void	SetTurnpointID(int i, int iID) {m_aiTurnpointIDs[i]=iID;}
	float	GetTurnpointRadius(int i) {return m_afTurnpointRadius[i];}
	void	SetTurnpointRadius(int i, float fRad) {m_afTurnpointRadius[i]=fRad;}

	int		GetNumTurnpoints() {return m_nTurnpoints;}
	void	SetNumTurnpoints(int i) {m_nTurnpoints=i;}

	int		GetNumLegs();
	int		GetLegStart(int iLeg);
	int		GetLegFinish(int iLeg);

	bool	m_b2ndGateActive;
	CGate	m_cStartGate;
	CGate	m_cStartGate2;
	CGate	m_cFinishGate;

	int GetAssignedTurnpoints(CTurnpointArray& cTurnpointArray,  CString acStrings[]);
	CString GetStatusText();
	static CString GetStatusText(EStatus eStatus);

	double GetDistance(CTurnpointArray& cTurnpointArray, EUnits eUnits);

	CString StartGateText(CTurnpointArray& cTurnpointArray );
	CString FinishGateText(CTurnpointArray& cTurnpointArray );
	CString StartGateBText(CTurnpointArray& cTurnpointArray );

	CTask();
	virtual ~CTask();
	CTask(CTask *pcTask);
	CTask(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pTask);

	EStatus		m_eStatus;
	EClass		m_eClass;
	CTime		m_cDate;
	ETaskType	m_eType;

	CTime		m_cGateOpenTime;
	CTimeSpan	m_cPostTime;
	CTimeSpan	GetTaskTime() {return m_cPostTime;};
	
	CString     m_strFileName;
	double		m_dColumnSortDistance[3];
	
	virtual void Serialize(CArchive& ar);

    CString DateText();
    CString TypeText();
    CString MinTimeText();
    CString GPSStartText(EUnits eUnits);
    CString GPSFinishText(EUnits eUnits);
	CString DistanceText(CTurnpointArray&, EUnits);

	private:

	int			m_nTurnpoints;
	int			m_aiTurnpointIDs[MAXTASKTPS];
	float		m_afTurnpointRadius[MAXTASKTPS];

public:
	bool Export(CString strFileName);
	bool Import(CString strFileName);
	bool IsOfficial(void);
	bool IsAreaTask()		{ return m_eType==eTurnArea			|| m_eType==eFAIAssignedArea; }
	bool IsTimedTask()		{ return m_eType==eTurnArea			|| m_eType==eFAIAssignedArea || m_eType==eModifiedAssigned; }
	bool IsAssignedTask()	{ return m_eType==eAssigned			|| m_eType==eFAIRacing ; }
	bool IsFAITask()		{ return m_eType==eFAIAssignedArea	|| m_eType==eFAIRacing ; }
};

int CALLBACK CompareTasks(LPARAM lParam1, LPARAM lParam2, 
    LPARAM iColumn);

#endif // !defined(AFX_TASK_H__C923E5B1_4F74_11D1_ACE5_000000000000__INCLUDED_)


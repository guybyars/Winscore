//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Position.h: interface for the CPosition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POSITION_H__36BAB18F_D466_11D2_B0B9_0080C84B0161__INCLUDED_)
#define AFX_POSITION_H__36BAB18F_D466_11D2_B0B9_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winscoredefs.h"
#include "Location.h"

#define FAN_IN_START_CYL      0x00001
#define FAN_ABOVE_START_CYL   0x00002
#define FAN_IN_FINISH_CYL	  0x00004
#define FAN_BELOW_FINISH_CYL  0x00008
#define FAN_LANDED			  0x00010
#define FAN_MOTOR_ON		  0x00020
#define FAN_AT_TURNPOINT      0x00040
#define FAN_OUTER_TURNPOINT   0x00080
#define FAN_TURN_AREA_TURNPOINT 0x00100
#define FAN_START			  0x00200
#define FAN_FINISH			  0x00400
#define FAN_LATEST_START	  0x00800
#define FAN_ROLLING			  0x01000
#define FAN_GAP				  0x02000
#define FAN_AT_TASKPOINT	  0x04000
#define FAN_FURTHEST_PROGRESS 0x08000
#define FAN_OUT_TOP_START	  0x10000
#define FAN_PEV				  0x20000


class CPosition : public CLocation  
{
public:
	CString GetTimeString();
	int GetTurnpoint(int iPos);
	int GetNumTurnpoints();
	CString GetTurnpointDistText(EUnits eUnit);
	CString GetTurnpointText();
	bool CheckTurnpoint( int iTPID, bool bOnlyOne=false );
	bool CheckTurnpoint(int iPos, int iTPID);

	CString GetStatusText(bool bStartGateCyl=true, bool bFinishGateCyl=true);
	double	GetDistanceFromControlPoint( int iTaskPt );
	void	SetDistanceFromControlPoint(int iPnt, double dDist);
	void	SetDistanceFromFinish(double dDist);
	void	SetDistanceFromStart(double dDist);
	bool	CheckStatus(int iStatus);
	void	SetTurnpoint(int iID, int iPos);
	void	AddStatus(int iStatus);
	void	RemoveStatus(int iStatus);
	CPosition(int iYear, int iMonth, int iDay, int iDST, CString strBRecord, int iAccuracyStartPos, int iAccuracyEndPos, int iENLStartPos, int iENLEndPos, int iMOPStartPos, int iMOPEndPos, bool bBFI, bool m_bPEV);
	CPosition();
	virtual ~CPosition();


	double	m_dDistanceFromFinish;
	double	m_dDistanceFromStart;
	CTime	m_cTime;
	int		m_iPressureAltitude;
	int		m_iGPSAltitude;
	int		m_iCorrectedAltitude;
	int		m_iCorrectedFinishAltitude;
	int     m_iStatus;
	double	m_dAccuracy;
	int		m_iEngineNoiseLevel;
	int     m_iMOPLevel;
	int		m_iEngineNoiseLevelRAW;
	int     m_iMOPLevelRAW;
	double  m_dSpeed;
	float	m_fVSpeed;
	bool	m_bBFI;
	bool	m_bPEV;

private:
	double	m_adDistanceFromControlPoint[ALLOCTASKTPS];
	int		m_asTurnpoint[ALLOCTASKTPS];


};

#endif // !defined(AFX_POSITION_H__36BAB18F_D466_11D2_B0B9_0080C84B0161__INCLUDED_)



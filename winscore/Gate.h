//
// Copyrightę Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Gate.h: interface for the CGate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATE_H__A8001D04_50BB_11D3_8941_00C04F79ECB9__INCLUDED_)
#define AFX_GATE_H__A8001D04_50BB_11D3_8941_00C04F79ECB9__INCLUDED_

#include "Location.h"	// Added by ClassView
#include "winscore.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "position.h"

class CGate  : public CObject
{

public:
	bool FromXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pGate);

	bool GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode);
	bool GetSSAXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode, EUnits eUnits, void* pTPArray, bool bisFAI);

	CString GetIDString();
	bool GetCrossingLocation( CLocation &cP1, CLocation &cP2, CLocation &cLoc);
	bool IsPerpToCourse();

	CString GetDescription(EUnits eUnits);
	CString GetText(EUnits eUnits);


	CGate& operator=(CGate&);

//	bool IsGPS()			{return true;}
	bool IsGPSCylinder()	{return m_eGateType==eCylinder;}
	bool IsFirstLegAtExit() {return m_bFirstLegAtExit;}
	bool IsPEVStart()		{return m_bPEVStart;}

	int GetHeight()			{return m_iHeight;}
	int GetBase()			{return m_iBase;}
	double GetHeading()		{return m_dHeading;}
	double GetRadius()		{return m_dRadius;}
	double GetMaxSpeed()	{return m_dMaxGroundSpeed;}
	int GetPointID()		{return m_iGatePointID;}
	EGateType GetGateType()	{return m_eGateType;}

	void SetBase(int i)			 {m_iBase=i;}
	void SetHeight(int i)		 {m_iHeight=i;}
	void SetHeading(double d);
	void SetRadius(double d)	 {m_dRadius=d;}
	void SetMaxSpeed(double d)	{m_dMaxGroundSpeed=d;}
	void SetPointID(int i)		 {m_iGatePointID=i;}
	void SetGateType(EGateType e){m_eGateType=e;}
	void SetFirstLegAtExit(bool b) {m_bFirstLegAtExit=b;}
	void SetPerpToCourse(bool b) {m_bPerpToCourse=b;}
	void SetScoreOutTheTop(bool b) {m_bScoreOutTheTop=b;}
	void SetIsPreStartAltitude(bool b) {m_bPreStartAltitude=b;}
	void SetMaxAltitude(int i)		{m_iMaxStartAltitude=i;}
	void SetPreStartAltitude(int i) {m_iPreStartAltitude=i;}
	void SetPEVStart( bool b) {m_bPEVStart=b;}
	void SetPEVStartWindow(int i) {m_iPEVStartWindow=i;}
	void SetPEVWaitTime(int i) {m_iPEVWaitTime=i;}

	int GetMaxAltitude()		{return m_iMaxStartAltitude;}
	int GetPreStartAltitude() {return m_iPreStartAltitude;}
	int GetPEVStartWindow() {return m_iPEVStartWindow;}
	int GetPEVWaitTime() {return	m_iPEVWaitTime;}


	bool ScoreOutTheTop();
	bool IsPreStartAltitude();

	void GetEnds( float &fLat1, float &fLong1, float &fLat2, float &fLong2, double dCorr=1.0);

	CGate::CGate( );
	virtual ~CGate();
	BOOL CrossedLine( CLocation &start, CLocation &end, BOOL bAllowEitherDirection=TRUE);
	BOOL CrossingTime(CPosition &cP1, CPosition &cP2, CTime &cTime);
    int  CrossingAltitude(CPosition &cP1, CPosition &cP2 );
	void Initialize( CLocation &cCenterLoc );

private:

	CLocation	m_cCenter;
	CLocation	m_cEnd1;
	CLocation	m_cEnd2;
	CLocation	m_cIP;

	bool		m_bFirstLegAtExit;
	bool		m_bPerpToCourse;
	bool		m_bScoreOutTheTop;
	bool		m_bPreStartAltitude;
	int			m_iGatePointID;
	double		m_dOrientation;
	double		m_dRadius;
	EGateType	m_eGateType;
	int			m_iBase;
	int			m_iHeight;
	double		m_dHeading;
	double		m_dMaxGroundSpeed;
	int			m_iPreStartAltitude;
	int			m_iMaxStartAltitude;
	int			m_iPEVStartWindow;
	int			m_iPEVWaitTime;
	bool		m_bPEVStart;
};

#endif // !defined(AFX_GATE_H__A8001D04_50BB_11D3_8941_00C04F79ECB9__INCLUDED_)


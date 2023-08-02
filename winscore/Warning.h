#pragma once

#define NWARNINGTYPES		11
enum EWarningType {	eUnknownWarning	=0,
					eInformation	=1,
					eStartLatest	=2,
					eStart			=3,
					eTurnpoint		=4,
					eFinish			=5,
					eAirspaceMinor	=6,
					eAirspaceMajor	=7,
					eSecurity		=8,
					eMotorRunLandout=9,
					eMotorRunStart=10

					};
static CString strWarningTypes[NWARNINGTYPES][2] =
					{
					"UNKN","Unknown",
					"INFO","Information",
					"STLA","Later Start",
					"STRT","Start",
					"TUPT","Turnpoint",
					"FNSH","Finish",
					"ARMI","Airspace (Minor)",
					"ARMA","Airspace (Major)",
					"SECU","Log Security",
					"MRUN","Motor Run Landout",
					"MRST","Motor Run Before Start"
					};

class CWarning
{

public:
	EWarningType	m_eType;
	CString			m_strWarning;
	bool			m_bActive;
	int				m_iPenalty;

	CWarning(EWarningType eType, int iPenalty, CString strWarning);
	CWarning(CString strWarningText);

	CString			GetText();
	CString			GetTypeText();
	EWarningType	GetType()	{return m_eType;}
	bool			IsActive()	{return m_bActive;}
	void			Clear()		{m_bActive=false;}

	~CWarning(void);
};


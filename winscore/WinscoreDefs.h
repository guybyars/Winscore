//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(WINSCOREDEFSH)
#define WINSCOREDEFSH

#define  MAXTASKTPS			12
#define  ALLOCTASKTPS		MAXTASKTPS*2

#define   PI  3.1415926535
#define   TORAD 3.1415926535/180.
#define   TODEG 180./3.1415926535

enum	EGateType { eCylinder=0, eLine=1 };
enum	ETraceDisplayType { eByPoints=0, eByTime=1 };

enum	EDatum		{eWGS84=0, eFAISphere=1 };
#define DEFAULTDATUM   eFAISphere

enum	EUnits		{eStatute, eNautical, eKilometers };
enum	EAltUnits	{eFeet, eMeters };
#define  SYSTEMUNITS        eNautical
#define  SYSTEMALTUNITS     eFeet

#define SAVE_REMIND_HOURS  12
#define SAVE_AUTOSAVE_HOURS 1
enum	EAutoSaveOption	{eNone, eRemind, eAutoSave };

enum	EStatus {eOfficial, eUnofficial, eNoContest, ePreliminary, eFinal};
enum	ETaskType {eAssigned=0, eModifiedAssigned=2, eTurnArea=3, eFAIRacing=4, eFAIAssignedArea=5};
enum	EContest {eNational, eRegional };

#define  NUMCLASSES		    20

#define  AIRBONTOLER        1.00

enum ENudgeView	{	eNegXRotation, 
					ePosXRotation,
					eNegYRotation,
					ePosYRotation };


enum ECoordFormat { eHHMMSS, eHHMMMM } ;

enum EClass	{ eStandard	=0, 
			  e15Meter	=1, 
			  eSports	=2, 
			  eClub     =3,
			  eOpen		=4, 
			  e18Meter	=5,
			  eFAIHandicapped=6,
			  eUser1	=7,
			  eUser2	=8,
			  eUser3	=9 };

enum ELogTime	{ eSystemTime	=0,
				  eLogTime		=1, 
				  eUserTime		=2  };


#ifndef ListView_SetCheckState
   #define ListView_SetCheckState(hwndLV, i, fCheck) \
      ListView_SetItemState(hwndLV, i, \
      INDEXTOSTATEIMAGEMASK((fCheck)+1), LVIS_STATEIMAGEMASK)
#endif


//#define WSP_STI               0x0001
//#define WSP_STI2X             0x0002
#define WSP_MINDIST           0x0004
#define WSP_MINTIME           0x0008
#define WSP_NOSTART			  0x0010
#define WSP_NOFINISH		  0x0020
#define WSP_NOLANDINGCARD	  0x0040
#define WSP_NOSSANUMBER		  0x0080
#define WSP_NOLAUNCH		  0x0100
#define WSP_AIRFIELDBONUS	  0x0200
#define WSP_MINTIME15		  0x0400
#define WSP_WITHDREW		  0x0800
#define WSP_NOFLIGHTLOG		  0x1000
#define WSP_A_GATE			  0x2000
#define WSP_B_GATE			  0x4000
#define WSP_FOREIGN 		  0x8000

#define WSC_BACKGROUND_COLOR	0
#define WSC_TASK_COLOR			1
#define WSC_START_RADIUS_COLOR	2
#define WSC_FINISH_RADIUS_COLOR	3
#define WSC_CLIMB_COLOR			4
#define WSC_WAYPOINT_COLOR		5
#define WSC_AIRSPACE_COLOR		6

#define WSC_UNITS				 10
#define WSC_ALTUNITS			 11
#define WSC_FONT_NAME			 12

#define WSC_PRESS_ALT_DISPLAY	 13
#define WSC_GPS_ALT_DISPLAY		 14
#define WSC_ENL_DISPLAY			 15

#define WSC_BACKGROUND_COLOR_RGB	RGB(255,255,255)
#define WSC_TASK_COLOR_RGB			RGB(0,0,255)
#define WSC_AIRSPACE_COLOR_RGB		RGB(0,0,0)
#define WSC_START_RADIUS_COLOR_RGB	RGB(255,128,128)
#define WSC_FINISH_RADIUS_COLOR_RGB	RGB(128,255,128)
#define WSC_CLIMB_COLOR_RGB			RGB(0,128,0)
#define WSC_WAYPOINT_COLOR_RGB		RGB(255,255,128)


#define WSO_TIME_STEP_METHOD		0x0001
#define WSO_FULL_TRACE_DISPLAY		0x0002
#define WSO_DIFFERENT_CLIMB_COLOR	0x0004
#define WSO_SHOW_SCENERY			0x0008
#define WSO_DEMO_MODE				0x0010
#define WSO_OUTER_RADIUS			0x0020
#define WSO_INNER_RADIUS			0x0040

#define	HCFILE		"handicaplist.xml"
#define HC_URL  "https://members.ssa.org//cde//handicaps_xml.asp"

#define	GET_XML_BOOL( cMgr, pParent, element, bval, bdefault) \
	{\
	CString str;\
	GET_XML_STR( cMgr, pParent, element, str);\
	int ival=atoi(str);\
	str.GetLength()>0? bval=(ival==0?false:true):bval=bdefault;\
	}\

#define	GET_XML_INT( cMgr, pParent, element, cast, ival, idefault) \
	{\
	CString str;\
	GET_XML_STR( cMgr, pParent, element, str);\
	str.GetLength()>0? ival=(cast)atoi(str):ival=idefault;\
	}\

#define	GET_XML_DBL( cMgr, pParent, element, cast, dval, ddefault) \
	{\
	CString str;\
	GET_XML_STR( cMgr, pParent, element, str);\
	str.GetLength()>0? dval=(cast)strtod(str,NULL):dval=ddefault;\
	}\

#define	GET_XML_STR( cMgr, pParent, element, str) \
	{\
	TESTHR(cMgr.SelectChildSTR(pParent,element,str));\
	}\

#define OUTPUT_TPT_TEXT( pcTurnpoint, dDist, dRadius, cLine, bTabs, cNewLineChar, strOut  ) {\
				\
				if( dRadius>0 )\
				{\
  				cLine.Format(bTabs?(_T("\t%d\t%s\t%6.2lf\t%6.1lf\t%s")):(_T("%6d   %-20s %6.2lf           %6.1lf %s")),\
    								pcTurnpoint->m_iID,\
    								pcTurnpoint->m_strName,\
    								dDist,\
									dRadius,\
									cNewLineChar);\
				}\
				else\
				{\
  				cLine.Format(bTabs?(_T("\t%d\t%s\t%6.2lf\t%s")):(_T("%6d   %-20s %6.2lf%s")),\
    								pcTurnpoint->m_iID,\
    								pcTurnpoint->m_strName,\
    								dDist,\
									cNewLineChar);\
				}\
	  			strOut+=cLine; }\

#define OUTPUT_TPT_TASK_HTML( pcTurnpoint, dDist, dRadius, strAltitude, cLine, strOut  ) {\
				\
				if( dRadius>0 )\
				{\
  				cLine.Format(_T("<tr><td>%d</td><td>%s</td><td>%6.2lf</td><td>%6.1lf</td><td>%s</td></tr>"),\
    								pcTurnpoint->m_iID,\
    								pcTurnpoint->m_strName,\
    								dDist,\
									dRadius,\
									strAltitude);\
				}\
				else\
				{\
  				cLine.Format(_T("<tr><td>%d</td><td>%s</td><td>%6.2lf</td><td></td><td>%s</td></tr>"),\
    								pcTurnpoint->m_iID,\
    								pcTurnpoint->m_strName,\
    								dDist,\
									strAltitude);\
				}\
	  			strOut+=cLine; }\

#endif // !defined(WINSCOREDEFS.H)



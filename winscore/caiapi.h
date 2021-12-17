//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
//
//  Copyright 2000 Cambridge Aero Instruments//
//
//	Cambridge Aero Instrument Access DLL Include file
//
//	Author:  Guy Byars	June 4, 2000
//
//
//
//
//
#ifndef _CAIAPI_H_
#define _CAIAPI_H_

#define  SEC_KEY				3141516
#define  CAI_MAX_TASK_POINTS	9
#define  CAI_MAX_TASKS			10
#define  CAI_MAX_DECL_POINTS	15
#define  CAI_PATH_LENGTH		512
#define  MAX_WAYPOINTS			250
#define  MAX_WAYPOINT_ID		9999

#include "CAIWaypoint.h"


//
//	API return codes
//
//
#define  CER_OK								 0
#define  CER_INVALID_SECURITY_KEY			 1
#define  CER_INVALID_COM_PORT				 2
#define  CER_USER_CANCEL					 3
#define  CER_NOT_CONNECTED					 4
#define  CER_COMMUNICATION_ERROR			 5
#define  CER_NO_LOGS_AVAILABLE				 6
#define  CER_COULD_NOT_WRITE_FILE			 7
#define  CER_LOG_FAILED_SECURITY			 8
#define  CER_COULD_NOT_OPEN_FILE			 9
#define  CER_ERROR_READING_LOG_FILE			10
#define  CER_INVALID_TASK_ID				11
#define  CER_COULD_NOT_VERIFY_TASK			12
#define  CER_COULD_NOT_VERIFY_DECLARATION	13
#define  CER_COULD_NOT_VERIFY_WAYPOINTS		14


/*
	TCAIInput - Structure for input options.
*/

typedef struct 
	{
	int		 iSecurityKey;
	short	 sComPort;
	short	 sLanguageID;
	short	 sTimeout;
	bool	 bSilent;
	} TCAIInput;


//  TCAITransferedLogs - Structure for transfered logs.

typedef struct 
	{
	int		 nLogs;
	char	 cLogNames[10][256];
	} TCAITransferedLogs;


//  TCAIUTC, TCAIFileDesc, TCAIFileInfo - Structures for log info.

typedef struct 
	{
	BYTE mbYear;
	BYTE mbMonth;
	BYTE mbDay;
	BYTE mbHour;
	BYTE mbMinute;
	BYTE mbSecond;
	}  TCAIUTC;

typedef struct 
	{
	TCAIUTC		mStartTime;	
	TCAIUTC		mFinishTime;
	UINT		muiNRecs;
	}  TCAILogDesc;

typedef struct 
	{
	UINT			nLogs;				// Nuber of logs stored in recorder
	TCAILogDesc		LogDesc[16];		// Log Description structure, one for each log
	TCHAR			cLogInfo[16][255];	// Log discription.
	} TCAILogInfo;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Helpful Macros
//

#define  INITIALIZE_CAI_STRUCT( sCaiInput )	{\
			sCaiInput.iSecurityKey=SEC_KEY;	\
			sCaiInput.sComPort=1;				\
			sCaiInput.sLanguageID=0;			\
			sCaiInput.sTimeout=10;			\
			sCaiInput.bSilent=false;			\
			}\

#define  CAI_ERROR_TEXT( iErr, cErrString )	{\
			switch(iErr)\
			{	\
			case CER_OK:	\
				cErrString="OK";\
				break;\
			case CER_INVALID_SECURITY_KEY:\
				cErrString="Invalid security key supplied to CAI API";\
				break;\
			case CER_INVALID_COM_PORT:\
				cErrString="Invalid communications port ID";\
				break;\
			case CER_NOT_CONNECTED:\
				cErrString="No connection established to recorder";\
				break;\
			case CER_COMMUNICATION_ERROR:\
				cErrString="Communication error.";\
				break;\
			case CER_NO_LOGS_AVAILABLE:\
				cErrString="No Logs Available.";\
				break;\
			case CER_COULD_NOT_WRITE_FILE:\
				cErrString="Error writing flight log to disk.";\
				break;\
			case CER_LOG_FAILED_SECURITY:\
				cErrString="Flight failed security check.";\
				break;\
			case CER_COULD_NOT_OPEN_FILE:\
				cErrString="Could not open requested file.";\
				break;\
			case CER_ERROR_READING_LOG_FILE:\
				cErrString="An error occured while trying to read requested file.";\
				break;\
			case CER_INVALID_TASK_ID:\
				cErrString="Specified task id was invalid, must be 1-10.";\
				break;\
			case CER_COULD_NOT_VERIFY_TASK:\
				cErrString="Specified task could not be validated in the recorder.";\
				break;\
			case CER_COULD_NOT_VERIFY_DECLARATION:\
				cErrString="Specified declaration could not be validated in the recorder.";\
				break;\
			case CER_COULD_NOT_VERIFY_WAYPOINTS:\
				cErrString="Navpoint transfer could not be validated in the recorder.";\
				break;\
			}\
			}\

//			case CER_USER_CANCEL:
//				cErrString="User canceled the operation.";
//				break;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Access API prototypes
//
		
/*

  CAIInfo - Connects to the recorder and returns the following information:
	Pilot Name
	Recorder ID
	Contest Number
	Glider Number

*/	
extern "C" int PASCAL EXPORT CAIInfo(   const TCAIInput* psCaiInput, // Input
										char* pstrPilotName,		 // Output
										char* pstrRecorderID,		 // Output
										char* pstrContestID,		 // Output
										char* pstrGliderID			 // Output
										);
		


/*
    CAIGetLog - Connects to the recorder and get the specified log and display
	a status bar as log is transfered.  This function creates both an IGC file and
	a propriarity .CAI file.  The name and path of the IGC files are returned in the TCAITransferedLogs structure.

*/
extern "C" int PASCAL EXPORT CAIGetLog (	const TCAIInput*	pCaiInput,		// Input
											const char*			pstrLogDir,		// Input
											const int			iLog,			// Input 1-N
											TCAITransferedLogs*	pLog			// Output
											);


		
/*

/*
    CAIGetLogs - Connects to the recorder and displays a dialog box which controls the log transfer
	process and displays a status bar as logs are transfered.  This function creates both an IGC file and
	a propriarity .CAI file.  The name and path of the IGC files are returned in the TCAITransferedLogs structure.

*/
extern "C" int PASCAL EXPORT CAIGetLogs(	const TCAIInput*	pCaiInput,		// Input
											const char*			pstrLogDir,		// Input
											TCAITransferedLogs*	pLogs			// Output
											);


		
/*
	CAIEditConfig - Connects to the recorder and displays a dialog box which permits the user to view 
	and edit the recorder configuration.
*/
extern "C" int PASCAL EXPORT CAIEditConfig(	
										   const TCAIInput*		pCaiInput	// Input
										   );


	
	
/*
	CAIGetWaypoints - Connects to the recorder and transfers the waypoints from the recorder to the PC and
	returns them in the CCAIWaypoint structure array.
*/
extern "C" int PASCAL EXPORT CAIGetWaypoints(	const TCAIInput*	pCaiInput,			// Input
												const int			nMax,				// Input
												CCAIWaypoint		cCAIWaypoints[],	// Output
												int					*nReturned,			// Output
												char*				strSiteName );		// Output

	

extern "C" int PASCAL EXPORT CAISetWaypoints(	const TCAIInput*	pCaiInput,
												const int			nToTransfer,
												const char*			pstrSiteName,
												CCAIWaypoint		cCAIWaypoints[],
												int					*pnTransfered );


	
/*
	CAIGetSingleLog - Connects to the recorder and transfers the log with the specified date.  Optionally, the 
	caller can pass in "-1" for the day and month to retreive the latest log in the recorder.  
	No user interaction	is required, but a dialog box is displayed displaying the status of the transfer.
*/
extern "C" int PASCAL EXPORT CAIGetSingleLog(	const TCAIInput*	pCaiInput,			// Input
 												const char*			pstrLogDir,		    // Input
												const int			iDay,				// Input, 1-31  or -1 for most recent log
												const int			iMonth,				// Input, 1-12  or -1 for most recent log
												char*				pstrLogFileName		// Output
												);

	

	
/*
	CAIValidateFile - Reads the input file and performs the Cambridge propritary security check.  
	Returned Value:
		CER_OK -  Log passed security check.
		CER_LOG_FAILED_SECURITY -  Log failed security check.
*/
extern "C" int PASCAL EXPORT CAIValidateFile( 
											 const char*		pstrFileName		// Input
											 );


		
/*
CAIConvertFile - Converts a log file stored in Cambridge propritary format CAI format to IGC format.
The boolean value pointed to by "pbSecure" is set to true if the CAI file passed the security check, and is set 
to false if not.

	Returned Value:
		CER_COULD_NOT_OPEN_FILE;
		CER_ERROR_READING_LOG_FILE;
		CER_COULD_NOT_WRITE_FILE;
*/

extern "C" int PASCAL EXPORT CAIConvertFile( const char*		pstrSourceFileName,		// Input
											 const char*		pstrOutputPath,			// Input
											 char*				pstrCreatedFileName,	// Output
											 BOOL*				pbSecure				// Output
											);
		
/*
	CAIGetTasks - Get the all the tasks which have been created in the recorder.  They are returned in the iTaskPoints
	array.  The values returned in the iTaskPoints array are the ordnal value of each wayoint in the task.  
	Up to CAI_MAX_TASKS can be defined.  Each task can contain CAI_MAX_TASK_POINTS points. If the first point
	in the task definition is =0, then that task is undefined.
*/
extern "C" int PASCAL EXPORT CAIGetTasks(	const TCAIInput*	pCaiInput,											// Input
											int					iTaskPoints[CAI_MAX_TASKS][CAI_MAX_TASK_POINTS]);	// Output

		
/*
	CAISetTask - Set a single task in the recorder.
*/
extern "C" int PASCAL EXPORT CAISetTask(	const TCAIInput*	pCaiInput,			// Input
											const int			iTaskID,			// Input
											const int			nTaskPoints,		// Input
											int*				piTaskPoints);		// Input
		
/*
	CAIDeclareTask - Declares one of the previously set tasks.

*/
extern "C" int PASCAL EXPORT CAIDeclareTask(	const TCAIInput*		pCaiInput,		// Input
												const int				iTaskID	);		// Input
		
/*

CAIGetDeclaration - Returns the task declaration.

*/
extern "C" int PASCAL EXPORT CAIGetDeclaration(	const TCAIInput*	pCaiInput,						// Input
												char*				pstrDecTime,					// Output
												char*				pstrLaunchTime,					// Output
												int*				pnLegs,							// Output
												int					iPoints[CAI_MAX_DECL_POINTS]	// Output
												);
		
/*

CAIAbout - Puts up a dialog box displaying dll version information

*/
extern "C" int PASCAL EXPORT CAIAbout();


/*

CAIManageSites - Puts up a dialog box to perform site management

*/

extern "C" int PASCAL EXPORT  CAIManageSites(	const	TCAIInput* pCaiInput,
												const char*	pstrSiteDirectory);


/*

CAIGetLogInfo - Gets discription of all the logs in the recorder.

*/

extern "C" int PASCAL EXPORT CAIGetLogInfo(	const TCAIInput*	pCaiInput,
											TCAILogInfo*		pLogs	);


/*

CAIGetSiteName - Gets site name.

*/

extern "C" int PASCAL EXPORT CAIGetSiteName(	const TCAIInput*	pCaiInput,
												char*				pstrSiteName );


/*

CAIConnect - Checks if a recorder is connected to the serial port.

*/

extern "C" int PASCAL EXPORT CAIConnect(	const TCAIInput*	pCaiInput );

/*

CAIGetRevDate - Returns latest Revision Date of the DLL.

*/

extern "C" void PASCAL EXPORT CAIGetRevDate(	char* pcDate, int iSize );

/*

CAIGetLogFileTime - Get the star time of a CAI flight log.

*/

extern "C" int PASCAL EXPORT CAIGetLogFileTime( const char*		pstrFileName,
											    TCAIUTC*		pStartTime,
												int*			pnRecords );


/*

CAIEditWaypoints - Edits an array of waypoints.

*/

extern "C" int PASCAL EXPORT CAIEditWaypoints(	const TCAIInput*	pCaiInput,
												const char*			strSiteName,		// Input
											    const int			nWaypoints,			// Input
											    const int			nMax,				// Input
												CCAIWaypoint		cCAIWaypoints[],	// Input/Output
												int					*nReturned);		// Output

/*

CAIGPSOff - Turns off the recorder, if supported.

*/

extern "C" void PASCAL EXPORT CAIGPSOff();		// Output

/*

CAIValidateIGCFile - Validates the security of a CAI 302 generated IGC file.

*/

extern "C" int PASCAL EXPORT CAIValidateIGCFile( const char*	pstrFileName,
												 int			iResultLen,
												 char*			pstrResult);
/*

ValidateLog - For IGC compatability..

*/

extern "C" BOOL PASCAL EXPORT ValidateLog( const char*	pstrFileName );

#endif
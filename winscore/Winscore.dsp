# Microsoft Developer Studio Project File - Name="Winscore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Winscore - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Winscore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Winscore.mak" CFG="Winscore - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Winscore - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Winscore - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Winscore", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Winscore - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "../zip" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 CAItrans.lib ../zip/release/zip.lib Wininet.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /debug /nodefaultlib /force

!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../zip" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 mfc42d.lib caitrans.lib ../zip/debug/zip.lib crypt32.lib wininet.lib rpcrt4.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Desc=Making the Help
PreLink_Cmds=Makehelp.bat
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Winscore - Win32 Release"
# Name "Winscore - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ACComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\AddTurnpointDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AlternateStartDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoExport.cpp
# End Source File
# Begin Source File

SOURCE=.\Averager.cpp
# End Source File
# Begin Source File

SOURCE=.\bitmappicture.cpp
# End Source File
# Begin Source File

SOURCE=.\CalculateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeTurnpointLocDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassAssignedPointsPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassPostPointsPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassPropSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Contestant.cpp
# End Source File
# Begin Source File

SOURCE=.\ContestantAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\ContestantDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ContestantList.cpp
# End Source File
# Begin Source File

SOURCE=.\ContestantsFromSSA.cpp
# End Source File
# Begin Source File

SOURCE=.\ContestInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ContestSettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyScoresheetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DescriptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetailedScoreInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DirDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DontShowDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditFlightDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EventList.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcludeContestantsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportFlightFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportScoresheetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportSuccessDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FDRAnalysisSettingsPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\FDRecorder.cpp
# End Source File
# Begin Source File

SOURCE=.\FDRecorderInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\FDRSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\FDRSettingsPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\FDRSettingsPropSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\Fix.cpp
# End Source File
# Begin Source File

SOURCE=.\FixList.cpp
# End Source File
# Begin Source File

SOURCE=.\Flight.cpp
# End Source File
# Begin Source File

SOURCE=.\FlightAnalysisDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FlightDisplayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FlightList.cpp
# End Source File
# Begin Source File

SOURCE=.\FlightLogTransferSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\FlightSearchFoldersDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FlightStream.cpp
# End Source File
# Begin Source File

SOURCE=.\FTPClient.cpp
# End Source File
# Begin Source File

SOURCE=.\FTPUpload.cpp
# End Source File
# Begin Source File

SOURCE=.\Gate.cpp
# End Source File
# Begin Source File

SOURCE=.\GateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GliderInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\GliderInfoList.cpp
# End Source File
# Begin Source File

SOURCE=.\Global_Functions.cpp
# End Source File
# Begin Source File

SOURCE=.\GridPositionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HandicapCalcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HScrollListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\IGCFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportFlightsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportFlightSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\InvalidLeg.cpp
# End Source File
# Begin Source File

SOURCE=.\InvalidLegDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InvalidLegList.cpp
# End Source File
# Begin Source File

SOURCE=.\LandingCard.cpp
# End Source File
# Begin Source File

SOURCE=.\LandingCardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LandingCardList.cpp
# End Source File
# Begin Source File

SOURCE=.\LandingLocationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LandoutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ListVwEx.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalMachineRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\Location.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MAKEHELP.BAT
# End Source File
# Begin Source File

SOURCE=.\Median.cpp
# End Source File
# Begin Source File

SOURCE=.\Penality.cpp
# End Source File
# Begin Source File

SOURCE=.\PenalityDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PenalityList.cpp
# End Source File
# Begin Source File

SOURCE=.\PlannerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Position.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintGridDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RealtimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RealtimeInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RealTimePropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ScoreRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\ScoreRecordList.cpp
# End Source File
# Begin Source File

SOURCE=.\ScoreReportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScoreServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScoreSheetListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowWarningsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SortListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StringHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\SUA.cpp
# End Source File
# Begin Source File

SOURCE=.\SUAUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\Summary.cpp
# End Source File
# Begin Source File

SOURCE=.\Sxbutton.cpp
# End Source File
# Begin Source File

SOURCE=.\Task.cpp
# End Source File
# Begin Source File

SOURCE=.\taskcontrol.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskDisplayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskList.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskTimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskTurnpointsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Turnpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\TurnpointArray.cpp
# End Source File
# Begin Source File

SOURCE=.\TurnpointCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\TurnpointEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\TurnpointListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TurnpointPickerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WindDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Winscore.cpp
# End Source File
# Begin Source File

SOURCE=.\Winscore.rc
# End Source File
# Begin Source File

SOURCE=.\WinscoreDlgBar.cpp
# End Source File
# Begin Source File

SOURCE=.\WinscoreDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\WinscoreEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\WinscoreFonts.cpp
# End Source File
# Begin Source File

SOURCE=.\WinscoreListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\WinscoreView.cpp
# End Source File
# Begin Source File

SOURCE=.\wsbaro.cpp
# End Source File
# Begin Source File

SOURCE=.\WSClass.cpp
# End Source File
# Begin Source File

SOURCE=.\WSSingleDocTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\wsview.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLMgr.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ACComboBox.h
# End Source File
# Begin Source File

SOURCE=.\AddTurnpointDlg.h
# End Source File
# Begin Source File

SOURCE=.\AlternateStartDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoExport.h
# End Source File
# Begin Source File

SOURCE=.\Averager.h
# End Source File
# Begin Source File

SOURCE=.\bitmappicture.h
# End Source File
# Begin Source File

SOURCE=.\CalculateDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChangeTurnpointLocDlg.h
# End Source File
# Begin Source File

SOURCE=.\ClassAssignedPointsPropPage.h
# End Source File
# Begin Source File

SOURCE=.\ClassListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ClassPostPointsPropPage.h
# End Source File
# Begin Source File

SOURCE=.\ClassPropPage.h
# End Source File
# Begin Source File

SOURCE=.\ClassPropSheet.h
# End Source File
# Begin Source File

SOURCE=.\ClassTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Contestant.h
# End Source File
# Begin Source File

SOURCE=.\ContestantAddress.h
# End Source File
# Begin Source File

SOURCE=.\ContestantDlg.h
# End Source File
# Begin Source File

SOURCE=.\ContestantList.h
# End Source File
# Begin Source File

SOURCE=.\ContestantsFromSSA.h
# End Source File
# Begin Source File

SOURCE=.\ContestInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\ContestSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\CopyScoresheetDlg.h
# End Source File
# Begin Source File

SOURCE=.\DescriptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetailedScoreInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\DirDialog.h
# End Source File
# Begin Source File

SOURCE=.\DontShowDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditFlightDlg.h
# End Source File
# Begin Source File

SOURCE=.\EventList.h
# End Source File
# Begin Source File

SOURCE=.\ExcludeContestantsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExportFlightFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExportScoresheetDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExportSuccessDlg.h
# End Source File
# Begin Source File

SOURCE=.\FDRAnalysisSettingsPropPage.h
# End Source File
# Begin Source File

SOURCE=.\FDRecorder.h
# End Source File
# Begin Source File

SOURCE=.\FDRecorderInfo.h
# End Source File
# Begin Source File

SOURCE=.\FDRSettings.h
# End Source File
# Begin Source File

SOURCE=.\FDRSettingsPropPage.h
# End Source File
# Begin Source File

SOURCE=.\FDRSettingsPropSheet.h
# End Source File
# Begin Source File

SOURCE=.\Fix.h
# End Source File
# Begin Source File

SOURCE=.\FixList.h
# End Source File
# Begin Source File

SOURCE=.\Flight.h
# End Source File
# Begin Source File

SOURCE=.\FlightAnalysisDlg.h
# End Source File
# Begin Source File

SOURCE=.\FlightDisplayDlg.h
# End Source File
# Begin Source File

SOURCE=.\FlightList.h
# End Source File
# Begin Source File

SOURCE=.\FlightLogTransferSettings.h
# End Source File
# Begin Source File

SOURCE=.\FlightSearchFoldersDlg.h
# End Source File
# Begin Source File

SOURCE=.\FlightStream.h
# End Source File
# Begin Source File

SOURCE=.\FTPClient.h
# End Source File
# Begin Source File

SOURCE=.\FTPUpload.h
# End Source File
# Begin Source File

SOURCE=.\Gate.h
# End Source File
# Begin Source File

SOURCE=.\GateDlg.h
# End Source File
# Begin Source File

SOURCE=.\GliderInfo.h
# End Source File
# Begin Source File

SOURCE=.\GliderInfoList.h
# End Source File
# Begin Source File

SOURCE=.\Global_Prototypes.h
# End Source File
# Begin Source File

SOURCE=.\GridPositionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\HandicapCalcDlg.h
# End Source File
# Begin Source File

SOURCE=.\HScrollListBox.h
# End Source File
# Begin Source File

SOURCE=.\IGCFile.h
# End Source File
# Begin Source File

SOURCE=.\ImportDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImportFlightsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImportFlightSettings.h
# End Source File
# Begin Source File

SOURCE=.\InvalidLeg.h
# End Source File
# Begin Source File

SOURCE=.\InvalidLegDlg.h
# End Source File
# Begin Source File

SOURCE=.\InvalidLegList.h
# End Source File
# Begin Source File

SOURCE=.\LandingCard.h
# End Source File
# Begin Source File

SOURCE=.\LandingCardDlg.h
# End Source File
# Begin Source File

SOURCE=.\LandingCardList.h
# End Source File
# Begin Source File

SOURCE=.\LandingLocationDlg.h
# End Source File
# Begin Source File

SOURCE=.\LandoutDlg.h
# End Source File
# Begin Source File

SOURCE=.\ListVwEx.h
# End Source File
# Begin Source File

SOURCE=.\LocalMachineRegistry.h
# End Source File
# Begin Source File

SOURCE=.\Location.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Median.h
# End Source File
# Begin Source File

SOURCE=.\Penality.h
# End Source File
# Begin Source File

SOURCE=.\PenalityDlg.h
# End Source File
# Begin Source File

SOURCE=.\PenalityList.h
# End Source File
# Begin Source File

SOURCE=.\PlannerDlg.h
# End Source File
# Begin Source File

SOURCE=.\Position.h
# End Source File
# Begin Source File

SOURCE=.\PreviewDlg.h
# End Source File
# Begin Source File

SOURCE=.\PrintGridDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\RealtimeDlg.h
# End Source File
# Begin Source File

SOURCE=.\RealtimeInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\RealTimePropPage.h
# End Source File
# Begin Source File

SOURCE=.\ResizingDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScoreRecord.h
# End Source File
# Begin Source File

SOURCE=.\ScoreRecordList.h
# End Source File
# Begin Source File

SOURCE=.\ScoreReportDlg.h
# End Source File
# Begin Source File

SOURCE=.\ScoreServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\ScoreSheetListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ServerSettings.h
# End Source File
# Begin Source File

SOURCE=.\ShowWarningsDlg.h
# End Source File
# Begin Source File

SOURCE=.\SortListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StringHelper.h
# End Source File
# Begin Source File

SOURCE=.\SUA.h
# End Source File
# Begin Source File

SOURCE=.\suautil.h
# End Source File
# Begin Source File

SOURCE=.\Summary.h
# End Source File
# Begin Source File

SOURCE=.\Sxbutton.h
# End Source File
# Begin Source File

SOURCE=.\Task.h
# End Source File
# Begin Source File

SOURCE=.\taskcontrol.h
# End Source File
# Begin Source File

SOURCE=.\TaskDisplayDlg.h
# End Source File
# Begin Source File

SOURCE=.\TaskDlg.h
# End Source File
# Begin Source File

SOURCE=.\TaskInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\TaskList.h
# End Source File
# Begin Source File

SOURCE=.\TaskTimeDlg.h
# End Source File
# Begin Source File

SOURCE=.\TaskTurnpointsDlg.h
# End Source File
# Begin Source File

SOURCE=.\TimeDlg.h
# End Source File
# Begin Source File

SOURCE=.\Turnpoint.h
# End Source File
# Begin Source File

SOURCE=.\TurnpointArray.h
# End Source File
# Begin Source File

SOURCE=.\TurnpointCombo.h
# End Source File
# Begin Source File

SOURCE=.\TurnpointEdit.h
# End Source File
# Begin Source File

SOURCE=.\TurnpointListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TurnpointPickerDlg.h
# End Source File
# Begin Source File

SOURCE=.\WindDlg.h
# End Source File
# Begin Source File

SOURCE=.\Winscore.h
# End Source File
# Begin Source File

SOURCE=.\WinscoreDefs.h
# End Source File
# Begin Source File

SOURCE=.\WinscoreDlgBar.h
# End Source File
# Begin Source File

SOURCE=.\WinscoreDoc.h
# End Source File
# Begin Source File

SOURCE=.\WinscoreEvent.h
# End Source File
# Begin Source File

SOURCE=.\WinscoreFonts.h
# End Source File
# Begin Source File

SOURCE=.\WinscoreListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\WinscoreView.h
# End Source File
# Begin Source File

SOURCE=.\wsbaro.h
# End Source File
# Begin Source File

SOURCE=.\WSClass.h
# End Source File
# Begin Source File

SOURCE=.\WSSingleDocTemplate.h
# End Source File
# Begin Source File

SOURCE=.\wsview.h
# End Source File
# Begin Source File

SOURCE=.\XMLMgr.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\Logo2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SmallLogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Splash.bmp
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Winscore.ico
# End Source File
# Begin Source File

SOURCE=.\res\Winscore.rc2
# End Source File
# Begin Source File

SOURCE=.\res\WinscoreDoc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\inetdownload.avi
# End Source File
# Begin Source File

SOURCE=.\res\upload.avi
# End Source File
# Begin Source File

SOURCE=.\Chilkat\lib\ChilkatRelMt.lib
# End Source File
# Begin Source File

SOURCE=.\Chilkat\lib\CkBaseRelMt.lib
# End Source File
# End Target
# End Project
# Section Winscore : {6D74002B-7061-7000-0065-0000453A0000}
# 	1:17:ID_INDICATOR_TIME:106
# 	1:17:ID_INDICATOR_DATE:105
# 	2:2:BH:
# 	2:17:ID_INDICATOR_TIME:ID_INDICATOR_TIME
# 	2:17:ID_INDICATOR_DATE:ID_INDICATOR_DATE
# End Section
# Section Winscore : {C201F982-42E6-11D1-ACD3-000000000000}
# 	2:12:RListDoc.cpp:RListDoc.cpp
# 	2:10:RListDoc.h:RListDoc.h
# 	2:18:CLASS: CRowListDoc:CRowListDoc
# 	2:19:Application Include:Winscore.h
# End Section
# Section Winscore : {49134336-1CE2-11D4-B1D2-54F85E000000}
# 	2:21:DefaultSinkHeaderFile:wsbaro.h
# 	2:16:DefaultSinkClass:CWSBaro
# End Section
# Section Winscore : {C201F981-42E6-11D1-ACD3-000000000000}
# 	2:10:ListVwEx.h:ListVwEx.h
# 	2:12:ListVwEx.cpp:ListVwEx.cpp
# 	2:18:CLASS: CListViewEx:CListViewEx
# 	2:19:Application Include:Winscore.h
# End Section
# Section Winscore : {BAD124D2-4095-11D1-ACCF-000000000000}
# 	2:13:Turnpoint.cpp:Turnpoint.cpp
# 	2:11:Turnpoint.h:Turnpoint.h
# 	2:17:CLASS: CTurnpoint:CTurnpoint
# 	2:19:Application Include:Winscore.h
# End Section
# Section Winscore : {68982028-C595-11D2-B653-0080C84B0161}
# 	2:21:DefaultSinkHeaderFile:taskcontrol.h
# 	2:16:DefaultSinkClass:CTaskControl
# End Section
# Section Winscore : {E0748846-5814-11D3-BA96-0010A4043281}
# 	2:21:DefaultSinkHeaderFile:wsview.h
# 	2:16:DefaultSinkClass:CWSView
# End Section
# Section Winscore : {A82AB342-BB35-11CF-8771-00A0C9039735}
# 	1:10:IDB_SPLASH:104
# 	2:21:SplashScreenInsertKey:4.0
# End Section
# Section Winscore : {BAD124D1-4095-11D1-ACCF-000000000000}
# 	2:16:CLASS: CLocation:CLocation
# 	2:12:Location.cpp:Location.cpp
# 	2:10:Location.h:Location.h
# 	2:19:Application Include:Winscore.h
# End Section
# Section Winscore : {D35EE0D5-432A-11D1-ACD3-000000000000}
# 	2:9:RListVw.h:RListVw.h
# 	2:11:RListVw.cpp:RListVw.cpp
# 	2:19:CLASS: CRowListView:CRowListView
# 	2:19:Application Include:Winscore.h
# End Section
# Section Winscore : {49134334-1CE2-11D4-B1D2-54F85E000000}
# 	2:5:Class:CWSBaro
# 	2:10:HeaderFile:wsbaro.h
# 	2:8:ImplFile:wsbaro.cpp
# End Section
# Section Winscore : {68982026-C595-11D2-B653-0080C84B0161}
# 	2:5:Class:CTaskControl
# 	2:10:HeaderFile:taskcontrol.h
# 	2:8:ImplFile:taskcontrol.cpp
# End Section
# Section Winscore : {BAD124D0-4095-11D1-ACCF-000000000000}
# 	1:11:IDD_ADD_TPT:102
# 	2:16:Resource Include:resource.h
# 	2:17:AddTurnpointDlg.h:AddTurnpointDlg.h
# 	2:10:ENUM: enum:enum
# 	2:23:CLASS: CAddTurnpointDlg:CAddTurnpointDlg
# 	2:11:IDD_ADD_TPT:IDD_ADD_TPT
# 	2:19:Application Include:Winscore.h
# 	2:19:AddTurnpointDlg.cpp:AddTurnpointDlg.cpp
# End Section
# Section Winscore : {E0748844-5814-11D3-BA96-0010A4043281}
# 	2:5:Class:CWSView
# 	2:10:HeaderFile:wsview.h
# 	2:8:ImplFile:wsview.cpp
# End Section

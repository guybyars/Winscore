# Microsoft Developer Studio Generated NMAKE File, Based on Winscore.dsp
!IF "$(CFG)" == ""
CFG=Winscore - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Winscore - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Winscore - Win32 Release" && "$(CFG)" !=\
 "Winscore - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Winscore - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Winscore.exe"

!ELSE 

ALL : "$(OUTDIR)\Winscore.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AddTurnpointDlg.obj"
	-@erase "$(INTDIR)\CalculateDlg.obj"
	-@erase "$(INTDIR)\ClassTabCtrl.obj"
	-@erase "$(INTDIR)\Contestant.obj"
	-@erase "$(INTDIR)\ContestantAddress.obj"
	-@erase "$(INTDIR)\ContestantDlg.obj"
	-@erase "$(INTDIR)\ContestantList.obj"
	-@erase "$(INTDIR)\ContestInfoDlg.obj"
	-@erase "$(INTDIR)\EventList.obj"
	-@erase "$(INTDIR)\ExportScoresheetDlg.obj"
	-@erase "$(INTDIR)\FDRecorder.obj"
	-@erase "$(INTDIR)\FDRecorderInfo.obj"
	-@erase "$(INTDIR)\Global_Functions.obj"
	-@erase "$(INTDIR)\GridPositionsDlg.obj"
	-@erase "$(INTDIR)\InvalidLeg.obj"
	-@erase "$(INTDIR)\InvalidLegDlg.obj"
	-@erase "$(INTDIR)\InvalidLegList.obj"
	-@erase "$(INTDIR)\LandingCard.obj"
	-@erase "$(INTDIR)\LandingCardDlg.obj"
	-@erase "$(INTDIR)\LandingCardList.obj"
	-@erase "$(INTDIR)\LandoutDlg.obj"
	-@erase "$(INTDIR)\ListVwEx.obj"
	-@erase "$(INTDIR)\Location.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Penality.obj"
	-@erase "$(INTDIR)\PenalityDlg.obj"
	-@erase "$(INTDIR)\PenalityList.obj"
	-@erase "$(INTDIR)\ScoreRecord.obj"
	-@erase "$(INTDIR)\ScoreRecordList.obj"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Task.obj"
	-@erase "$(INTDIR)\TaskDlg.obj"
	-@erase "$(INTDIR)\TaskList.obj"
	-@erase "$(INTDIR)\TimeDlg.obj"
	-@erase "$(INTDIR)\Turnpoint.obj"
	-@erase "$(INTDIR)\TurnpointArray.obj"
	-@erase "$(INTDIR)\TurnpointCombo.obj"
	-@erase "$(INTDIR)\TurnpointListCtrl.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\WindDlg.obj"
	-@erase "$(INTDIR)\Winscore.obj"
	-@erase "$(INTDIR)\Winscore.pch"
	-@erase "$(INTDIR)\Winscore.res"
	-@erase "$(INTDIR)\WinscoreDlgBar.obj"
	-@erase "$(INTDIR)\WinscoreDoc.obj"
	-@erase "$(INTDIR)\WinscoreEvent.obj"
	-@erase "$(INTDIR)\WinscoreListCtrl.obj"
	-@erase "$(INTDIR)\WinscoreView.obj"
	-@erase "$(OUTDIR)\Winscore.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Winscore.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Winscore.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Winscore.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\Winscore.pdb" /machine:I386 /out:"$(OUTDIR)\Winscore.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AddTurnpointDlg.obj" \
	"$(INTDIR)\CalculateDlg.obj" \
	"$(INTDIR)\ClassTabCtrl.obj" \
	"$(INTDIR)\Contestant.obj" \
	"$(INTDIR)\ContestantAddress.obj" \
	"$(INTDIR)\ContestantDlg.obj" \
	"$(INTDIR)\ContestantList.obj" \
	"$(INTDIR)\ContestInfoDlg.obj" \
	"$(INTDIR)\EventList.obj" \
	"$(INTDIR)\ExportScoresheetDlg.obj" \
	"$(INTDIR)\FDRecorder.obj" \
	"$(INTDIR)\FDRecorderInfo.obj" \
	"$(INTDIR)\Global_Functions.obj" \
	"$(INTDIR)\GridPositionsDlg.obj" \
	"$(INTDIR)\InvalidLeg.obj" \
	"$(INTDIR)\InvalidLegDlg.obj" \
	"$(INTDIR)\InvalidLegList.obj" \
	"$(INTDIR)\LandingCard.obj" \
	"$(INTDIR)\LandingCardDlg.obj" \
	"$(INTDIR)\LandingCardList.obj" \
	"$(INTDIR)\LandoutDlg.obj" \
	"$(INTDIR)\ListVwEx.obj" \
	"$(INTDIR)\Location.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Penality.obj" \
	"$(INTDIR)\PenalityDlg.obj" \
	"$(INTDIR)\PenalityList.obj" \
	"$(INTDIR)\ScoreRecord.obj" \
	"$(INTDIR)\ScoreRecordList.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Task.obj" \
	"$(INTDIR)\TaskDlg.obj" \
	"$(INTDIR)\TaskList.obj" \
	"$(INTDIR)\TimeDlg.obj" \
	"$(INTDIR)\Turnpoint.obj" \
	"$(INTDIR)\TurnpointArray.obj" \
	"$(INTDIR)\TurnpointCombo.obj" \
	"$(INTDIR)\TurnpointListCtrl.obj" \
	"$(INTDIR)\WindDlg.obj" \
	"$(INTDIR)\Winscore.obj" \
	"$(INTDIR)\Winscore.res" \
	"$(INTDIR)\WinscoreDlgBar.obj" \
	"$(INTDIR)\WinscoreDoc.obj" \
	"$(INTDIR)\WinscoreEvent.obj" \
	"$(INTDIR)\WinscoreListCtrl.obj" \
	"$(INTDIR)\WinscoreView.obj"

"$(OUTDIR)\Winscore.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Winscore.exe" "$(OUTDIR)\Winscore.bsc"

!ELSE 

ALL : "$(OUTDIR)\Winscore.exe" "$(OUTDIR)\Winscore.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AddTurnpointDlg.obj"
	-@erase "$(INTDIR)\AddTurnpointDlg.sbr"
	-@erase "$(INTDIR)\CalculateDlg.obj"
	-@erase "$(INTDIR)\CalculateDlg.sbr"
	-@erase "$(INTDIR)\ClassTabCtrl.obj"
	-@erase "$(INTDIR)\ClassTabCtrl.sbr"
	-@erase "$(INTDIR)\Contestant.obj"
	-@erase "$(INTDIR)\Contestant.sbr"
	-@erase "$(INTDIR)\ContestantAddress.obj"
	-@erase "$(INTDIR)\ContestantAddress.sbr"
	-@erase "$(INTDIR)\ContestantDlg.obj"
	-@erase "$(INTDIR)\ContestantDlg.sbr"
	-@erase "$(INTDIR)\ContestantList.obj"
	-@erase "$(INTDIR)\ContestantList.sbr"
	-@erase "$(INTDIR)\ContestInfoDlg.obj"
	-@erase "$(INTDIR)\ContestInfoDlg.sbr"
	-@erase "$(INTDIR)\EventList.obj"
	-@erase "$(INTDIR)\EventList.sbr"
	-@erase "$(INTDIR)\ExportScoresheetDlg.obj"
	-@erase "$(INTDIR)\ExportScoresheetDlg.sbr"
	-@erase "$(INTDIR)\FDRecorder.obj"
	-@erase "$(INTDIR)\FDRecorder.sbr"
	-@erase "$(INTDIR)\FDRecorderInfo.obj"
	-@erase "$(INTDIR)\FDRecorderInfo.sbr"
	-@erase "$(INTDIR)\Global_Functions.obj"
	-@erase "$(INTDIR)\Global_Functions.sbr"
	-@erase "$(INTDIR)\GridPositionsDlg.obj"
	-@erase "$(INTDIR)\GridPositionsDlg.sbr"
	-@erase "$(INTDIR)\InvalidLeg.obj"
	-@erase "$(INTDIR)\InvalidLeg.sbr"
	-@erase "$(INTDIR)\InvalidLegDlg.obj"
	-@erase "$(INTDIR)\InvalidLegDlg.sbr"
	-@erase "$(INTDIR)\InvalidLegList.obj"
	-@erase "$(INTDIR)\InvalidLegList.sbr"
	-@erase "$(INTDIR)\LandingCard.obj"
	-@erase "$(INTDIR)\LandingCard.sbr"
	-@erase "$(INTDIR)\LandingCardDlg.obj"
	-@erase "$(INTDIR)\LandingCardDlg.sbr"
	-@erase "$(INTDIR)\LandingCardList.obj"
	-@erase "$(INTDIR)\LandingCardList.sbr"
	-@erase "$(INTDIR)\LandoutDlg.obj"
	-@erase "$(INTDIR)\LandoutDlg.sbr"
	-@erase "$(INTDIR)\ListVwEx.obj"
	-@erase "$(INTDIR)\ListVwEx.sbr"
	-@erase "$(INTDIR)\Location.obj"
	-@erase "$(INTDIR)\Location.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\Penality.obj"
	-@erase "$(INTDIR)\Penality.sbr"
	-@erase "$(INTDIR)\PenalityDlg.obj"
	-@erase "$(INTDIR)\PenalityDlg.sbr"
	-@erase "$(INTDIR)\PenalityList.obj"
	-@erase "$(INTDIR)\PenalityList.sbr"
	-@erase "$(INTDIR)\ScoreRecord.obj"
	-@erase "$(INTDIR)\ScoreRecord.sbr"
	-@erase "$(INTDIR)\ScoreRecordList.obj"
	-@erase "$(INTDIR)\ScoreRecordList.sbr"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\Splash.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Task.obj"
	-@erase "$(INTDIR)\Task.sbr"
	-@erase "$(INTDIR)\TaskDlg.obj"
	-@erase "$(INTDIR)\TaskDlg.sbr"
	-@erase "$(INTDIR)\TaskList.obj"
	-@erase "$(INTDIR)\TaskList.sbr"
	-@erase "$(INTDIR)\TimeDlg.obj"
	-@erase "$(INTDIR)\TimeDlg.sbr"
	-@erase "$(INTDIR)\Turnpoint.obj"
	-@erase "$(INTDIR)\Turnpoint.sbr"
	-@erase "$(INTDIR)\TurnpointArray.obj"
	-@erase "$(INTDIR)\TurnpointArray.sbr"
	-@erase "$(INTDIR)\TurnpointCombo.obj"
	-@erase "$(INTDIR)\TurnpointCombo.sbr"
	-@erase "$(INTDIR)\TurnpointListCtrl.obj"
	-@erase "$(INTDIR)\TurnpointListCtrl.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\WindDlg.obj"
	-@erase "$(INTDIR)\WindDlg.sbr"
	-@erase "$(INTDIR)\Winscore.obj"
	-@erase "$(INTDIR)\Winscore.pch"
	-@erase "$(INTDIR)\Winscore.res"
	-@erase "$(INTDIR)\Winscore.sbr"
	-@erase "$(INTDIR)\WinscoreDlgBar.obj"
	-@erase "$(INTDIR)\WinscoreDlgBar.sbr"
	-@erase "$(INTDIR)\WinscoreDoc.obj"
	-@erase "$(INTDIR)\WinscoreDoc.sbr"
	-@erase "$(INTDIR)\WinscoreEvent.obj"
	-@erase "$(INTDIR)\WinscoreEvent.sbr"
	-@erase "$(INTDIR)\WinscoreListCtrl.obj"
	-@erase "$(INTDIR)\WinscoreListCtrl.sbr"
	-@erase "$(INTDIR)\WinscoreView.obj"
	-@erase "$(INTDIR)\WinscoreView.sbr"
	-@erase "$(OUTDIR)\Winscore.bsc"
	-@erase "$(OUTDIR)\Winscore.exe"
	-@erase "$(OUTDIR)\Winscore.ilk"
	-@erase "$(OUTDIR)\Winscore.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Winscore.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Winscore.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Winscore.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AddTurnpointDlg.sbr" \
	"$(INTDIR)\CalculateDlg.sbr" \
	"$(INTDIR)\ClassTabCtrl.sbr" \
	"$(INTDIR)\Contestant.sbr" \
	"$(INTDIR)\ContestantAddress.sbr" \
	"$(INTDIR)\ContestantDlg.sbr" \
	"$(INTDIR)\ContestantList.sbr" \
	"$(INTDIR)\ContestInfoDlg.sbr" \
	"$(INTDIR)\EventList.sbr" \
	"$(INTDIR)\ExportScoresheetDlg.sbr" \
	"$(INTDIR)\FDRecorder.sbr" \
	"$(INTDIR)\FDRecorderInfo.sbr" \
	"$(INTDIR)\Global_Functions.sbr" \
	"$(INTDIR)\GridPositionsDlg.sbr" \
	"$(INTDIR)\InvalidLeg.sbr" \
	"$(INTDIR)\InvalidLegDlg.sbr" \
	"$(INTDIR)\InvalidLegList.sbr" \
	"$(INTDIR)\LandingCard.sbr" \
	"$(INTDIR)\LandingCardDlg.sbr" \
	"$(INTDIR)\LandingCardList.sbr" \
	"$(INTDIR)\LandoutDlg.sbr" \
	"$(INTDIR)\ListVwEx.sbr" \
	"$(INTDIR)\Location.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\Penality.sbr" \
	"$(INTDIR)\PenalityDlg.sbr" \
	"$(INTDIR)\PenalityList.sbr" \
	"$(INTDIR)\ScoreRecord.sbr" \
	"$(INTDIR)\ScoreRecordList.sbr" \
	"$(INTDIR)\Splash.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Task.sbr" \
	"$(INTDIR)\TaskDlg.sbr" \
	"$(INTDIR)\TaskList.sbr" \
	"$(INTDIR)\TimeDlg.sbr" \
	"$(INTDIR)\Turnpoint.sbr" \
	"$(INTDIR)\TurnpointArray.sbr" \
	"$(INTDIR)\TurnpointCombo.sbr" \
	"$(INTDIR)\TurnpointListCtrl.sbr" \
	"$(INTDIR)\WindDlg.sbr" \
	"$(INTDIR)\Winscore.sbr" \
	"$(INTDIR)\WinscoreDlgBar.sbr" \
	"$(INTDIR)\WinscoreDoc.sbr" \
	"$(INTDIR)\WinscoreEvent.sbr" \
	"$(INTDIR)\WinscoreListCtrl.sbr" \
	"$(INTDIR)\WinscoreView.sbr"

"$(OUTDIR)\Winscore.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\Winscore.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\Winscore.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AddTurnpointDlg.obj" \
	"$(INTDIR)\CalculateDlg.obj" \
	"$(INTDIR)\ClassTabCtrl.obj" \
	"$(INTDIR)\Contestant.obj" \
	"$(INTDIR)\ContestantAddress.obj" \
	"$(INTDIR)\ContestantDlg.obj" \
	"$(INTDIR)\ContestantList.obj" \
	"$(INTDIR)\ContestInfoDlg.obj" \
	"$(INTDIR)\EventList.obj" \
	"$(INTDIR)\ExportScoresheetDlg.obj" \
	"$(INTDIR)\FDRecorder.obj" \
	"$(INTDIR)\FDRecorderInfo.obj" \
	"$(INTDIR)\Global_Functions.obj" \
	"$(INTDIR)\GridPositionsDlg.obj" \
	"$(INTDIR)\InvalidLeg.obj" \
	"$(INTDIR)\InvalidLegDlg.obj" \
	"$(INTDIR)\InvalidLegList.obj" \
	"$(INTDIR)\LandingCard.obj" \
	"$(INTDIR)\LandingCardDlg.obj" \
	"$(INTDIR)\LandingCardList.obj" \
	"$(INTDIR)\LandoutDlg.obj" \
	"$(INTDIR)\ListVwEx.obj" \
	"$(INTDIR)\Location.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Penality.obj" \
	"$(INTDIR)\PenalityDlg.obj" \
	"$(INTDIR)\PenalityList.obj" \
	"$(INTDIR)\ScoreRecord.obj" \
	"$(INTDIR)\ScoreRecordList.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Task.obj" \
	"$(INTDIR)\TaskDlg.obj" \
	"$(INTDIR)\TaskList.obj" \
	"$(INTDIR)\TimeDlg.obj" \
	"$(INTDIR)\Turnpoint.obj" \
	"$(INTDIR)\TurnpointArray.obj" \
	"$(INTDIR)\TurnpointCombo.obj" \
	"$(INTDIR)\TurnpointListCtrl.obj" \
	"$(INTDIR)\WindDlg.obj" \
	"$(INTDIR)\Winscore.obj" \
	"$(INTDIR)\Winscore.res" \
	"$(INTDIR)\WinscoreDlgBar.obj" \
	"$(INTDIR)\WinscoreDoc.obj" \
	"$(INTDIR)\WinscoreEvent.obj" \
	"$(INTDIR)\WinscoreListCtrl.obj" \
	"$(INTDIR)\WinscoreView.obj"

"$(OUTDIR)\Winscore.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Winscore - Win32 Release" || "$(CFG)" ==\
 "Winscore - Win32 Debug"
SOURCE=.\AddTurnpointDlg.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_ADDTU=\
	".\AddTurnpointDlg.h"\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\AddTurnpointDlg.obj" : $(SOURCE) $(DEP_CPP_ADDTU) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_ADDTU=\
	".\AddTurnpointDlg.h"\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\AddTurnpointDlg.obj"	"$(INTDIR)\AddTurnpointDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_ADDTU) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\CalculateDlg.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_CALCU=\
	".\CalculateDlg.h"\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\CalculateDlg.obj" : $(SOURCE) $(DEP_CPP_CALCU) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_CALCU=\
	".\CalculateDlg.h"\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\CalculateDlg.obj"	"$(INTDIR)\CalculateDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_CALCU) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\ClassTabCtrl.cpp
DEP_CPP_CLASS=\
	".\ClassTabCtrl.h"\
	".\Winscore.h"\
	

!IF  "$(CFG)" == "Winscore - Win32 Release"


"$(INTDIR)\ClassTabCtrl.obj" : $(SOURCE) $(DEP_CPP_CLASS) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"


"$(INTDIR)\ClassTabCtrl.obj"	"$(INTDIR)\ClassTabCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_CLASS) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\Contestant.cpp
DEP_CPP_CONTE=\
	".\Contestant.h"\
	".\FDRecorder.h"\
	".\Winscore.h"\
	

!IF  "$(CFG)" == "Winscore - Win32 Release"


"$(INTDIR)\Contestant.obj" : $(SOURCE) $(DEP_CPP_CONTE) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"


"$(INTDIR)\Contestant.obj"	"$(INTDIR)\Contestant.sbr" : $(SOURCE)\
 $(DEP_CPP_CONTE) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\ContestantAddress.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_CONTES=\
	".\ContestantAddress.h"\
	".\Winscore.h"\
	

"$(INTDIR)\ContestantAddress.obj" : $(SOURCE) $(DEP_CPP_CONTES) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_CONTES=\
	".\ContestantAddress.h"\
	".\Winscore.h"\
	

"$(INTDIR)\ContestantAddress.obj"	"$(INTDIR)\ContestantAddress.sbr" : $(SOURCE)\
 $(DEP_CPP_CONTES) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\ContestantDlg.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_CONTEST=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantAddress.h"\
	".\ContestantDlg.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\ContestantDlg.obj" : $(SOURCE) $(DEP_CPP_CONTEST) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_CONTEST=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantAddress.h"\
	".\ContestantDlg.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\ContestantDlg.obj"	"$(INTDIR)\ContestantDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_CONTEST) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\ContestantList.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_CONTESTA=\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\FDRecorder.h"\
	".\Winscore.h"\
	

"$(INTDIR)\ContestantList.obj" : $(SOURCE) $(DEP_CPP_CONTESTA) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_CONTESTA=\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\FDRecorder.h"\
	".\Winscore.h"\
	

"$(INTDIR)\ContestantList.obj"	"$(INTDIR)\ContestantList.sbr" : $(SOURCE)\
 $(DEP_CPP_CONTESTA) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\ContestInfoDlg.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_CONTESTI=\
	".\ContestInfoDlg.h"\
	".\Winscore.h"\
	

"$(INTDIR)\ContestInfoDlg.obj" : $(SOURCE) $(DEP_CPP_CONTESTI) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_CONTESTI=\
	".\ContestInfoDlg.h"\
	".\Winscore.h"\
	

"$(INTDIR)\ContestInfoDlg.obj"	"$(INTDIR)\ContestInfoDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_CONTESTI) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\EventList.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_EVENT=\
	".\EventList.h"\
	".\Winscore.h"\
	".\WinscoreEvent.h"\
	

"$(INTDIR)\EventList.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_EVENT=\
	".\EventList.h"\
	".\Winscore.h"\
	".\WinscoreEvent.h"\
	

"$(INTDIR)\EventList.obj"	"$(INTDIR)\EventList.sbr" : $(SOURCE)\
 $(DEP_CPP_EVENT) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\ExportScoresheetDlg.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_EXPOR=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\ExportScoresheetDlg.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\ExportScoresheetDlg.obj" : $(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_EXPOR=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\ExportScoresheetDlg.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\ExportScoresheetDlg.obj"	"$(INTDIR)\ExportScoresheetDlg.sbr" : \
$(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\FDRecorder.cpp
DEP_CPP_FDREC=\
	".\FDRecorder.h"\
	".\Winscore.h"\
	

!IF  "$(CFG)" == "Winscore - Win32 Release"


"$(INTDIR)\FDRecorder.obj" : $(SOURCE) $(DEP_CPP_FDREC) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"


"$(INTDIR)\FDRecorder.obj"	"$(INTDIR)\FDRecorder.sbr" : $(SOURCE)\
 $(DEP_CPP_FDREC) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\FDRecorderInfo.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_FDRECO=\
	".\FDRecorderInfo.h"\
	".\Winscore.h"\
	

"$(INTDIR)\FDRecorderInfo.obj" : $(SOURCE) $(DEP_CPP_FDRECO) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_FDRECO=\
	".\FDRecorderInfo.h"\
	".\Winscore.h"\
	

"$(INTDIR)\FDRecorderInfo.obj"	"$(INTDIR)\FDRecorderInfo.sbr" : $(SOURCE)\
 $(DEP_CPP_FDRECO) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\Global_Functions.cpp
DEP_CPP_GLOBA=\
	".\Global_Prototypes.h"\
	".\Winscore.h"\
	

!IF  "$(CFG)" == "Winscore - Win32 Release"


"$(INTDIR)\Global_Functions.obj" : $(SOURCE) $(DEP_CPP_GLOBA) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"


"$(INTDIR)\Global_Functions.obj"	"$(INTDIR)\Global_Functions.sbr" : $(SOURCE)\
 $(DEP_CPP_GLOBA) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\GridPositionsDlg.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_GRIDP=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\GridPositionsDlg.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\GridPositionsDlg.obj" : $(SOURCE) $(DEP_CPP_GRIDP) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_GRIDP=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\GridPositionsDlg.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\GridPositionsDlg.obj"	"$(INTDIR)\GridPositionsDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_GRIDP) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\InvalidLeg.cpp
DEP_CPP_INVAL=\
	".\InvalidLeg.h"\
	".\Winscore.h"\
	

!IF  "$(CFG)" == "Winscore - Win32 Release"


"$(INTDIR)\InvalidLeg.obj" : $(SOURCE) $(DEP_CPP_INVAL) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"


"$(INTDIR)\InvalidLeg.obj"	"$(INTDIR)\InvalidLeg.sbr" : $(SOURCE)\
 $(DEP_CPP_INVAL) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\InvalidLegDlg.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_INVALI=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegDlg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointCombo.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\InvalidLegDlg.obj" : $(SOURCE) $(DEP_CPP_INVALI) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_INVALI=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegDlg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointCombo.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\InvalidLegDlg.obj"	"$(INTDIR)\InvalidLegDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_INVALI) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\InvalidLegList.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_INVALID=\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\Location.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\InvalidLegList.obj" : $(SOURCE) $(DEP_CPP_INVALID) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_INVALID=\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\Location.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\InvalidLegList.obj"	"$(INTDIR)\InvalidLegList.sbr" : $(SOURCE)\
 $(DEP_CPP_INVALID) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\LandingCard.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_LANDI=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\LandingCard.obj" : $(SOURCE) $(DEP_CPP_LANDI) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_LANDI=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\LandingCard.obj"	"$(INTDIR)\LandingCard.sbr" : $(SOURCE)\
 $(DEP_CPP_LANDI) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\LandingCardDlg.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_LANDIN=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardDlg.h"\
	".\LandingCardList.h"\
	".\LandoutDlg.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointCombo.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\LandingCardDlg.obj" : $(SOURCE) $(DEP_CPP_LANDIN) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_LANDIN=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardDlg.h"\
	".\LandingCardList.h"\
	".\LandoutDlg.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointCombo.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\LandingCardDlg.obj"	"$(INTDIR)\LandingCardDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_LANDIN) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\LandingCardList.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_LANDING=\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\FDRecorder.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Task.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\LandingCardList.obj" : $(SOURCE) $(DEP_CPP_LANDING) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_LANDING=\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\FDRecorder.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Task.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\LandingCardList.obj"	"$(INTDIR)\LandingCardList.sbr" : $(SOURCE)\
 $(DEP_CPP_LANDING) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\LandoutDlg.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_LANDO=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\LandoutDlg.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointCombo.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\LandoutDlg.obj" : $(SOURCE) $(DEP_CPP_LANDO) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_LANDO=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\LandoutDlg.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointCombo.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\LandoutDlg.obj"	"$(INTDIR)\LandoutDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_LANDO) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\ListVwEx.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_LISTV=\
	".\Contestant.h"\
	".\FDRecorder.h"\
	".\LandingCard.h"\
	".\ListVwEx.h"\
	".\Location.h"\
	".\Task.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\ListVwEx.obj" : $(SOURCE) $(DEP_CPP_LISTV) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_LISTV=\
	".\Contestant.h"\
	".\FDRecorder.h"\
	".\LandingCard.h"\
	".\ListVwEx.h"\
	".\Location.h"\
	".\Task.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\ListVwEx.obj"	"$(INTDIR)\ListVwEx.sbr" : $(SOURCE) $(DEP_CPP_LISTV)\
 "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\Location.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_LOCAT=\
	".\Location.h"\
	".\Winscore.h"\
	

"$(INTDIR)\Location.obj" : $(SOURCE) $(DEP_CPP_LOCAT) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_LOCAT=\
	".\Location.h"\
	".\Winscore.h"\
	

"$(INTDIR)\Location.obj"	"$(INTDIR)\Location.sbr" : $(SOURCE) $(DEP_CPP_LOCAT)\
 "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_MAINF=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\ListVwEx.h"\
	".\Location.h"\
	".\MainFrm.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Splash.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDlgBar.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	".\WinscoreView.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_MAINF=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\ListVwEx.h"\
	".\Location.h"\
	".\MainFrm.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Splash.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDlgBar.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	".\WinscoreView.h"\
	

"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) $(DEP_CPP_MAINF)\
 "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\Penality.cpp
DEP_CPP_PENAL=\
	".\Penality.h"\
	".\Winscore.h"\
	

!IF  "$(CFG)" == "Winscore - Win32 Release"


"$(INTDIR)\Penality.obj" : $(SOURCE) $(DEP_CPP_PENAL) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"


"$(INTDIR)\Penality.obj"	"$(INTDIR)\Penality.sbr" : $(SOURCE) $(DEP_CPP_PENAL)\
 "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\PenalityDlg.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_PENALI=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityDlg.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\PenalityDlg.obj" : $(SOURCE) $(DEP_CPP_PENALI) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_PENALI=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityDlg.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\PenalityDlg.obj"	"$(INTDIR)\PenalityDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_PENALI) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\PenalityList.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_PENALIT=\
	".\Penality.h"\
	".\PenalityList.h"\
	".\Winscore.h"\
	

"$(INTDIR)\PenalityList.obj" : $(SOURCE) $(DEP_CPP_PENALIT) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_PENALIT=\
	".\Penality.h"\
	".\PenalityList.h"\
	".\Winscore.h"\
	

"$(INTDIR)\PenalityList.obj"	"$(INTDIR)\PenalityList.sbr" : $(SOURCE)\
 $(DEP_CPP_PENALIT) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\ScoreRecord.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_SCORE=\
	".\Location.h"\
	".\Penality.h"\
	".\ScoreRecord.h"\
	".\Winscore.h"\
	

"$(INTDIR)\ScoreRecord.obj" : $(SOURCE) $(DEP_CPP_SCORE) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_SCORE=\
	".\Location.h"\
	".\Penality.h"\
	".\ScoreRecord.h"\
	".\Winscore.h"\
	

"$(INTDIR)\ScoreRecord.obj"	"$(INTDIR)\ScoreRecord.sbr" : $(SOURCE)\
 $(DEP_CPP_SCORE) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\ScoreRecordList.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_SCORER=\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\ScoreRecordList.obj" : $(SOURCE) $(DEP_CPP_SCORER) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_SCORER=\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\ScoreRecordList.obj"	"$(INTDIR)\ScoreRecordList.sbr" : $(SOURCE)\
 $(DEP_CPP_SCORER) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\Splash.cpp
DEP_CPP_SPLAS=\
	".\Splash.h"\
	

!IF  "$(CFG)" == "Winscore - Win32 Release"


"$(INTDIR)\Splash.obj" : $(SOURCE) $(DEP_CPP_SPLAS) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"


"$(INTDIR)\Splash.obj"	"$(INTDIR)\Splash.sbr" : $(SOURCE) $(DEP_CPP_SPLAS)\
 "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "Winscore - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Winscore.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Winscore.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Winscore.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Winscore.pch" : \
$(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Task.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_TASK_=\
	".\Location.h"\
	".\Task.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\Task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_TASK_=\
	".\Location.h"\
	".\Task.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\Task.obj"	"$(INTDIR)\Task.sbr" : $(SOURCE) $(DEP_CPP_TASK_)\
 "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\TaskDlg.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_TASKD=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskDlg.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointCombo.h"\
	".\TurnpointListCtrl.h"\
	".\WindDlg.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\TaskDlg.obj" : $(SOURCE) $(DEP_CPP_TASKD) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_TASKD=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskDlg.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointCombo.h"\
	".\TurnpointListCtrl.h"\
	".\WindDlg.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\TaskDlg.obj"	"$(INTDIR)\TaskDlg.sbr" : $(SOURCE) $(DEP_CPP_TASKD)\
 "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\TaskList.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_TASKL=\
	".\Location.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\TaskList.obj" : $(SOURCE) $(DEP_CPP_TASKL) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_TASKL=\
	".\Location.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\TaskList.obj"	"$(INTDIR)\TaskList.sbr" : $(SOURCE) $(DEP_CPP_TASKL)\
 "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\TimeDlg.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_TIMED=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\TimeDlg.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\TimeDlg.obj" : $(SOURCE) $(DEP_CPP_TIMED) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_TIMED=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\TimeDlg.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\TimeDlg.obj"	"$(INTDIR)\TimeDlg.sbr" : $(SOURCE) $(DEP_CPP_TIMED)\
 "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\Turnpoint.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_TURNP=\
	".\Location.h"\
	".\Turnpoint.h"\
	".\Winscore.h"\
	

"$(INTDIR)\Turnpoint.obj" : $(SOURCE) $(DEP_CPP_TURNP) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_TURNP=\
	".\Location.h"\
	".\Turnpoint.h"\
	".\Winscore.h"\
	

"$(INTDIR)\Turnpoint.obj"	"$(INTDIR)\Turnpoint.sbr" : $(SOURCE)\
 $(DEP_CPP_TURNP) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\TurnpointArray.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_TURNPO=\
	".\Global_Prototypes.h"\
	".\Location.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\TurnpointArray.obj" : $(SOURCE) $(DEP_CPP_TURNPO) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_TURNPO=\
	".\Global_Prototypes.h"\
	".\Location.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\TurnpointArray.obj"	"$(INTDIR)\TurnpointArray.sbr" : $(SOURCE)\
 $(DEP_CPP_TURNPO) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\TurnpointCombo.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_TURNPOI=\
	".\Location.h"\
	".\Turnpoint.h"\
	".\TurnpointCombo.h"\
	".\Winscore.h"\
	

"$(INTDIR)\TurnpointCombo.obj" : $(SOURCE) $(DEP_CPP_TURNPOI) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_TURNPOI=\
	".\Location.h"\
	".\Turnpoint.h"\
	".\TurnpointCombo.h"\
	".\Winscore.h"\
	

"$(INTDIR)\TurnpointCombo.obj"	"$(INTDIR)\TurnpointCombo.sbr" : $(SOURCE)\
 $(DEP_CPP_TURNPOI) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\TurnpointListCtrl.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_TURNPOIN=\
	".\Location.h"\
	".\Turnpoint.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\TurnpointListCtrl.obj" : $(SOURCE) $(DEP_CPP_TURNPOIN) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_TURNPOIN=\
	".\Location.h"\
	".\Turnpoint.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\TurnpointListCtrl.obj"	"$(INTDIR)\TurnpointListCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_TURNPOIN) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\WindDlg.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_WINDD=\
	".\Global_Prototypes.h"\
	".\WindDlg.h"\
	".\Winscore.h"\
	

"$(INTDIR)\WindDlg.obj" : $(SOURCE) $(DEP_CPP_WINDD) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_WINDD=\
	".\Global_Prototypes.h"\
	".\WindDlg.h"\
	".\Winscore.h"\
	

"$(INTDIR)\WindDlg.obj"	"$(INTDIR)\WindDlg.sbr" : $(SOURCE) $(DEP_CPP_WINDD)\
 "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\Winscore.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_WINSC=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\ListVwEx.h"\
	".\Location.h"\
	".\MainFrm.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Splash.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDlgBar.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	".\WinscoreView.h"\
	

"$(INTDIR)\Winscore.obj" : $(SOURCE) $(DEP_CPP_WINSC) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_WINSC=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\ListVwEx.h"\
	".\Location.h"\
	".\MainFrm.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Splash.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDlgBar.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	".\WinscoreView.h"\
	

"$(INTDIR)\Winscore.obj"	"$(INTDIR)\Winscore.sbr" : $(SOURCE) $(DEP_CPP_WINSC)\
 "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\Winscore.rc
DEP_RSC_WINSCO=\
	".\res\icon1.ico"\
	".\res\Logo2.bmp"\
	".\res\Toolbar.bmp"\
	".\res\Winscore.rc2"\
	".\res\WinscoreDoc.ico"\
	".\Splsh16.bmp"\
	

"$(INTDIR)\Winscore.res" : $(SOURCE) $(DEP_RSC_WINSCO) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\WinscoreDlgBar.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_WINSCOR=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDlgBar.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\WinscoreDlgBar.obj" : $(SOURCE) $(DEP_CPP_WINSCOR) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_WINSCOR=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDlgBar.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\WinscoreDlgBar.obj"	"$(INTDIR)\WinscoreDlgBar.sbr" : $(SOURCE)\
 $(DEP_CPP_WINSCOR) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\WinscoreDoc.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_WINSCORE=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\ContestInfoDlg.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\ListVwEx.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	".\WinscoreView.h"\
	

"$(INTDIR)\WinscoreDoc.obj" : $(SOURCE) $(DEP_CPP_WINSCORE) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_WINSCORE=\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantList.h"\
	".\ContestInfoDlg.h"\
	".\EventList.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\InvalidLeg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardList.h"\
	".\ListVwEx.h"\
	".\Location.h"\
	".\Penality.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskList.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	".\WinscoreView.h"\
	

"$(INTDIR)\WinscoreDoc.obj"	"$(INTDIR)\WinscoreDoc.sbr" : $(SOURCE)\
 $(DEP_CPP_WINSCORE) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\WinscoreEvent.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_WINSCOREE=\
	".\Winscore.h"\
	".\WinscoreEvent.h"\
	

"$(INTDIR)\WinscoreEvent.obj" : $(SOURCE) $(DEP_CPP_WINSCOREE) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_WINSCOREE=\
	".\Winscore.h"\
	".\WinscoreEvent.h"\
	

"$(INTDIR)\WinscoreEvent.obj"	"$(INTDIR)\WinscoreEvent.sbr" : $(SOURCE)\
 $(DEP_CPP_WINSCOREE) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\WinscoreListCtrl.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_WINSCOREL=\
	".\ListVwEx.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\WinscoreListCtrl.obj" : $(SOURCE) $(DEP_CPP_WINSCOREL) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_WINSCOREL=\
	".\ListVwEx.h"\
	".\Winscore.h"\
	".\WinscoreListCtrl.h"\
	

"$(INTDIR)\WinscoreListCtrl.obj"	"$(INTDIR)\WinscoreListCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_WINSCOREL) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 

SOURCE=.\WinscoreView.cpp

!IF  "$(CFG)" == "Winscore - Win32 Release"

DEP_CPP_WINSCOREV=\
	".\AddTurnpointDlg.h"\
	".\CalculateDlg.h"\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantDlg.h"\
	".\ContestantList.h"\
	".\ContestInfoDlg.h"\
	".\EventList.h"\
	".\ExportScoresheetDlg.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\GridPositionsDlg.h"\
	".\InvalidLeg.h"\
	".\InvalidLegDlg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardDlg.h"\
	".\LandingCardList.h"\
	".\ListVwEx.h"\
	".\Location.h"\
	".\MainFrm.h"\
	".\Penality.h"\
	".\PenalityDlg.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskDlg.h"\
	".\TaskList.h"\
	".\TimeDlg.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointCombo.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDlgBar.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	".\WinscoreView.h"\
	

"$(INTDIR)\WinscoreView.obj" : $(SOURCE) $(DEP_CPP_WINSCOREV) "$(INTDIR)"\
 "$(INTDIR)\Winscore.pch"


!ELSEIF  "$(CFG)" == "Winscore - Win32 Debug"

DEP_CPP_WINSCOREV=\
	".\AddTurnpointDlg.h"\
	".\CalculateDlg.h"\
	".\ClassTabCtrl.h"\
	".\Contestant.h"\
	".\ContestantDlg.h"\
	".\ContestantList.h"\
	".\ContestInfoDlg.h"\
	".\EventList.h"\
	".\ExportScoresheetDlg.h"\
	".\FDRecorder.h"\
	".\Global_Prototypes.h"\
	".\GridPositionsDlg.h"\
	".\InvalidLeg.h"\
	".\InvalidLegDlg.h"\
	".\InvalidLegList.h"\
	".\LandingCard.h"\
	".\LandingCardDlg.h"\
	".\LandingCardList.h"\
	".\ListVwEx.h"\
	".\Location.h"\
	".\MainFrm.h"\
	".\Penality.h"\
	".\PenalityDlg.h"\
	".\PenalityList.h"\
	".\ScoreRecord.h"\
	".\ScoreRecordList.h"\
	".\Task.h"\
	".\TaskDlg.h"\
	".\TaskList.h"\
	".\TimeDlg.h"\
	".\Turnpoint.h"\
	".\TurnpointArray.h"\
	".\TurnpointCombo.h"\
	".\TurnpointListCtrl.h"\
	".\Winscore.h"\
	".\WinscoreDlgBar.h"\
	".\WinscoreDoc.h"\
	".\WinscoreEvent.h"\
	".\WinscoreListCtrl.h"\
	".\WinscoreView.h"\
	

"$(INTDIR)\WinscoreView.obj"	"$(INTDIR)\WinscoreView.sbr" : $(SOURCE)\
 $(DEP_CPP_WINSCOREV) "$(INTDIR)" "$(INTDIR)\Winscore.pch"


!ENDIF 


!ENDIF 


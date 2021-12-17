# Microsoft Developer Studio Project File - Name="WSView" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=WSView - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WSView.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WSView.mak" CFG="WSView - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WSView - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WSView - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WSView - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WSView - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WSView - Win32 Release Min Dep" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/WSView", MPAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WSView - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 glu32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Release
TargetPath=.\Release\WSView.ocx
InputPath=.\Release\WSView.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "WSView - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=.\Debug\WSView.ocx
InputPath=.\Debug\WSView.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "WSView - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\DebugU
TargetPath=.\DebugU\WSView.ocx
InputPath=.\DebugU\WSView.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "WSView - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseU"
# PROP BASE Intermediate_Dir "ReleaseU"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\ReleaseU
TargetPath=.\ReleaseU\WSView.ocx
InputPath=.\ReleaseU\WSView.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "WSView - Win32 Release Min Dep"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WSView___Win32_Release_Min_Dep"
# PROP BASE Intermediate_Dir "WSView___Win32_Release_Min_Dep"
# PROP BASE Target_Ext "ocx"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WSView___Win32_Release_Min_Dep"
# PROP Intermediate_Dir "WSView___Win32_Release_Min_Dep"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib glu32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 glu32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\WSView___Win32_Release_Min_Dep
TargetPath=.\WSView___Win32_Release_Min_Dep\WSView.ocx
InputPath=.\WSView___Win32_Release_Min_Dep\WSView.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "WSView - Win32 Release"
# Name "WSView - Win32 Debug"
# Name "WSView - Win32 Unicode Debug"
# Name "WSView - Win32 Unicode Release"
# Name "WSView - Win32 Release Min Dep"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\2DTransform.cpp
# End Source File
# Begin Source File

SOURCE=.\3DTransformation.cpp
# End Source File
# Begin Source File

SOURCE=.\BaroMeasureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BaroPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\BaroTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\BaroTraceGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\BaroZoom.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Cursor.cpp
# End Source File
# Begin Source File

SOURCE=.\Fix.cpp
# End Source File
# Begin Source File

SOURCE=.\FontCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\HitList.cpp
# End Source File
# Begin Source File

SOURCE=.\Location.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasuringDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ModulVer.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SUA.cpp
# End Source File
# Begin Source File

SOURCE=.\TipWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Trace.cpp
# End Source File
# Begin Source File

SOURCE=.\TraceGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\TracesPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\WSBaroCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\WSBaroPpg.cpp
# End Source File
# Begin Source File

SOURCE=.\WSView.cpp
# End Source File
# Begin Source File

SOURCE=.\WSView.def
# End Source File
# Begin Source File

SOURCE=.\WSView.odl
# End Source File
# Begin Source File

SOURCE=.\WSView.rc
# End Source File
# Begin Source File

SOURCE=.\WSViewAboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WSViewCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\wsviewinterface.cpp
# End Source File
# Begin Source File

SOURCE=.\WSViewPpg.cpp
# End Source File
# Begin Source File

SOURCE=.\xyzoom.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\2DTransform.h
# End Source File
# Begin Source File

SOURCE=.\3DTransformation.h
# End Source File
# Begin Source File

SOURCE=.\BaroMeasureDlg.h
# End Source File
# Begin Source File

SOURCE=.\BaroPoint.h
# End Source File
# Begin Source File

SOURCE=.\BaroTrace.h
# End Source File
# Begin Source File

SOURCE=.\BaroTraceGroup.h
# End Source File
# Begin Source File

SOURCE=.\BaroZoom.h
# End Source File
# Begin Source File

SOURCE=.\ControlPoint.h
# End Source File
# Begin Source File

SOURCE=.\Cursor.h
# End Source File
# Begin Source File

SOURCE=.\Fix.h
# End Source File
# Begin Source File

SOURCE=.\FontCombo.h
# End Source File
# Begin Source File

SOURCE=.\gllistinfo.h
# End Source File
# Begin Source File

SOURCE=.\HitList.h
# End Source File
# Begin Source File

SOURCE=.\Location.h
# End Source File
# Begin Source File

SOURCE=.\MeasuringDlg.h
# End Source File
# Begin Source File

SOURCE=.\ModulVer.h
# End Source File
# Begin Source File

SOURCE=.\Position.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SUA.h
# End Source File
# Begin Source File

SOURCE=.\TipWnd.h
# End Source File
# Begin Source File

SOURCE=.\Trace.h
# End Source File
# Begin Source File

SOURCE=.\TraceGroup.h
# End Source File
# Begin Source File

SOURCE=.\TracesPropPage.h
# End Source File
# Begin Source File

SOURCE=.\Winscore.h
# End Source File
# Begin Source File

SOURCE=.\WinscoreDefs.h
# End Source File
# Begin Source File

SOURCE=.\WSBaroCtl.h
# End Source File
# Begin Source File

SOURCE=.\WSBaroPpg.h
# End Source File
# Begin Source File

SOURCE=.\WSView.h
# End Source File
# Begin Source File

SOURCE=.\WSViewAboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\WSViewCtl.h
# End Source File
# Begin Source File

SOURCE=.\wsviewinterface.h
# End Source File
# Begin Source File

SOURCE=.\WSViewPpg.h
# End Source File
# Begin Source File

SOURCE=.\xyzoom.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\printer.bmp
# End Source File
# Begin Source File

SOURCE=.\WSBaro.ico
# End Source File
# Begin Source File

SOURCE=.\WSBaroCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\wsview32.bmp
# End Source File
# Begin Source File

SOURCE=.\WSViewCtl.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

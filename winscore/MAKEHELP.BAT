@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by Winscore.HPJ. >"hlp\Winscore.hm"
echo. >>"hlp\Winscore.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\Winscore.hm"
"C:\Program Files\Microsoft Visual Studio\Common\Tools\makehm.exe" ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\Winscore.hm"
echo. >>"hlp\Winscore.hm"
echo // Prompts (IDP_*) >>"hlp\Winscore.hm"
"C:\Program Files\Microsoft Visual Studio\Common\Tools\makehm.exe" IDP_,HIDP_,0x30000 resource.h >>"hlp\Winscore.hm"
echo. >>"hlp\Winscore.hm"
echo // Resources (IDR_*) >>"hlp\Winscore.hm"
"C:\Program Files\Microsoft Visual Studio\Common\Tools\makehm.exe" IDR_,HIDR_,0x20000 resource.h >>"hlp\Winscore.hm"
echo. >>"hlp\Winscore.hm"
echo // Dialogs (IDD_*) >>"hlp\Winscore.hm"
C:\Progra~1\Micros~2\Common\Tools\makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\Winscore.hm"
echo. >>"hlp\Winscore.hm"
echo // Frame Controls (IDW_*) >>"hlp\Winscore.hm"
"C:\Program Files\Microsoft Visual Studio\Common\Tools\makehm.exe" IDW_,HIDW_,0x50000 resource.h >>"hlp\Winscore.hm"
REM -- Make help for Project Winscore

if "%1" == "?" goto :Error
if "%1" == "/?" goto :Error
if "%1" == "-?" goto :Error
if "%1" == "help" goto :Error
if "%1" == "-help" goto :Error
if "%1" == "/help" goto :Error

if "%1" == "MAC" goto Mac

:Intel
if not "%1" == "" goto :Error
if not "%2" == "" goto :Error

echo Building Win32 Help files
start /wait MAKEHM.EXE -x hlp\Winscore.hpj"
echo.
if exist Debug\nul if exist hlp\Winscore.hlp copy "hlp\Winscore.hlp" Debug
if exist Debug\nul if exist hlp\Winscore.cnt copy "hlp\Winscore.cnt" Debug
if exist Release\nul if exist hlp\Winscore.hlp copy "hlp\Winscore.hlp" Release
if exist Release\nul if exist hlp\Winscore.cnt copy "hlp\Winscore.cnt" Release
goto :done

:Mac
echo Building Macintosh Help files
call hc35 hlp\ScriMac.hpj

if %2x == x goto :done
echo Copying to remote machine
mfile copy -c MSH2 -t HELP "ScriMac.hlp" %2
goto :done

:Error
echo Usage MAKEHELP [MAC [macintosh-path]]
echo       Where macintosh-path is of the form:
echo       ":<MacintoshName>:...:<MacintoshHelpFile>"

:done
echo.


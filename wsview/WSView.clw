; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CBaroMeasureDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "wsview.h"
LastPage=0
CDK=Y

ClassCount=10
Class1=CGatePropPage
Class2=CWSBaroCtrl
Class3=CWSBaroPropPage
Class4=CWSViewAboutDlg
Class5=CWSViewCtrl
Class6=CWSViewPropPage

ResourceCount=11
Resource1=IDR_POPUP_MENU_DEMO
Resource2=IDD_ABOUTBOX_WSBARO
Resource3=IDR_POPUP_MENU
Resource4=IDD_BARO_MEASURE
Resource5=IDD_PROPPAGE_WSVIEW
Resource6=IDD_ABOUTBOX_WSVIEWCTL
Class7=CMeasuringDlg
Resource7=IDD_PROPPAGE_WSBARO
Class8=CTracePropPage
Class9=CTracesPropPage
Resource8=IDD_MEASURING
Class10=CBaroMeasureDlg
Resource9=IDR_BARO_POPUP
Resource10=IDD_OLE_PROPPAGE_TRACES
Resource11=IDR_BARO_POPUP_DEMO

[CLS:CGatePropPage]
Type=0
BaseClass=COlePropertyPage
HeaderFile=GatePropPage.h
ImplementationFile=GatePropPage.cpp
LastObject=CGatePropPage

[CLS:CWSBaroCtrl]
Type=0
BaseClass=COleControl
HeaderFile=WSBaroCtl.h
ImplementationFile=WSBaroCtl.cpp

[CLS:CWSBaroPropPage]
Type=0
BaseClass=COlePropertyPage
HeaderFile=WSBaroPpg.h
ImplementationFile=WSBaroPpg.cpp
LastObject=CWSBaroPropPage

[CLS:CWSViewAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=WSViewAboutDlg.h
ImplementationFile=WSViewAboutDlg.cpp
LastObject=CWSViewAboutDlg

[CLS:CWSViewCtrl]
Type=0
BaseClass=COleControl
HeaderFile=WSViewCtl.h
ImplementationFile=WSViewCtl.cpp
Filter=W
VirtualFilter=wWC
LastObject=ID_POPUP_ZOOMWINDOW

[CLS:CWSViewPropPage]
Type=0
BaseClass=COlePropertyPage
HeaderFile=WSViewPpg.h
ImplementationFile=WSViewPpg.cpp
LastObject=CWSViewPropPage
Filter=D
VirtualFilter=idWC

[DLG:IDD_OLE_PROPPAGE_GATE]
Type=1
Class=CGatePropPage

[DLG:IDD_PROPPAGE_WSBARO]
Type=1
Class=CWSBaroPropPage
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX_WSVIEWCTL]
Type=1
Class=CWSViewAboutDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342177294
Control3=IDC_VERSION_TEXT,static,1342308352
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_PROPPAGE_WSVIEW]
Type=1
Class=CWSViewPropPage
ControlCount=10
Control1=IDC_BACKGROUND_COLOR,static,1350570510
Control2=IDC_CHANGE_BACKGROUND_BUTTON,button,1342242816
Control3=IDC_RESETBUTTON,button,1342242816
Control4=IDC_ALTSLIDER,msctls_trackbar32,1342242842
Control5=IDC_COLOR_COMBO,combobox,1344339970
Control6=IDC_VERTSCALEGROUP,button,1342177287
Control7=IDC_SCENERY_CHECK,button,1342242819
Control8=IDC_STATIC,static,1342308352
Control9=IDC_FONT_COMBO,combobox,1344340754
Control10=IDC_STATIC,button,1342177287

[DLG:IDD_ABOUTBOX_WSBARO]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_POPUP_MENU]
Type=1
Class=?
Command1=ID_POPUP_ZOOMWINDOW
Command2=ID_ZOOMOUT
Command3=ID_POPUP_AUTOSCALE
Command4=ID_MEASURE
Command5=ID_POPUP_PROPERTIES
CommandCount=5

[DLG:IDD_MEASURING]
Type=1
Class=CMeasuringDlg
ControlCount=6
Control1=IDC_OUTPUT,static,1342308352
Control2=IDC_TIME,static,1342308352
Control3=IDC_SPEED,static,1342308352
Control4=IDC_ALTITUDE,static,1342308352
Control5=IDC_AVEROC,static,1342308352
Control6=IDC_TURNPOINT_TEXT,static,1342308352

[CLS:CMeasuringDlg]
Type=0
HeaderFile=MeasuringDlg.h
ImplementationFile=MeasuringDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_TURNPOINT_TEXT

[MNU:IDR_POPUP_MENU_DEMO]
Type=1
Class=?
Command1=ID_ZOOMOUT
Command2=ID_POPUP_ZOOMWINDOW
Command3=ID_POPUP_AUTOSCALE
Command4=ID_POPUP_PROPERTIES
CommandCount=4

[CLS:CTracePropPage]
Type=0
HeaderFile=TracePropPage.h
ImplementationFile=TracePropPage.cpp
BaseClass=COlePropertyPage
Filter=D
LastObject=CTracePropPage

[DLG:IDD_OLE_PROPPAGE_TRACES]
Type=1
Class=CTracesPropPage
ControlCount=6
Control1=IDC_DISPLAY_LENGTH_TEXT,static,1342308352
Control2=IDC_DISPLAY_LENGTH,edit,1350631552
Control3=IDC_MINUTES_TEXT,static,1342308352
Control4=IDC_FIX_POINTS_CHECK,button,1342242819
Control5=IDC_DIFFERENT_COLOR_CHECK,button,1342242819
Control6=IDC_FULL_TRACE_CHECK,button,1342242819

[CLS:CTracesPropPage]
Type=0
HeaderFile=TracesPropPage.h
ImplementationFile=TracesPropPage.cpp
BaseClass=COlePropertyPage
Filter=D
LastObject=CTracesPropPage
VirtualFilter=idWC

[DLG:IDD_BARO_MEASURE]
Type=1
Class=CBaroMeasureDlg
ControlCount=2
Control1=IDC_LIST1,SysListView32,1350647809
Control2=IDC_COPY,button,1342242816

[CLS:CBaroMeasureDlg]
Type=0
HeaderFile=BaroMeasureDlg.h
ImplementationFile=BaroMeasureDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CBaroMeasureDlg
VirtualFilter=dWC

[MNU:IDR_BARO_POPUP]
Type=1
Class=?
Command1=ID_BARO_MEASURE
Command2=ID_BARO_ZOOM
Command3=ID_BARO_RESET
CommandCount=3

[MNU:IDR_BARO_POPUP_DEMO]
Type=1
Class=?
Command1=ID_BARO_ZOOM
Command2=ID_BARO_RESET
CommandCount=2


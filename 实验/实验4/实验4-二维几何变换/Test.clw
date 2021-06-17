; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTestView
LastTemplate=CFormView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Test.h"
LastPage=0

ClassCount=6
Class1=CTestApp
Class2=CTestDoc
Class3=CTestView
Class4=CMainFrame

ResourceCount=3
Resource1=IDR_MAINFRAME
Class5=CAboutDlg
Resource2=IDD_ABOUTBOX
Class6=CLeftPortion
Resource3=IDD_DIALOG1

[CLS:CTestApp]
Type=0
HeaderFile=Test.h
ImplementationFile=Test.cpp
Filter=N

[CLS:CTestDoc]
Type=0
HeaderFile=TestDoc.h
ImplementationFile=TestDoc.cpp
Filter=N

[CLS:CTestView]
Type=0
HeaderFile=TestView.h
ImplementationFile=TestView.cpp
Filter=C
LastObject=CTestView
BaseClass=CView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_MDRAW
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=Test.cpp
ImplementationFile=Test.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=2
Control1=IDOK,button,1342373889
Control2=IDC_STATIC,static,1342177294

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_APP_EXIT
Command2=ID_MDRAW
Command3=ID_APP_ABOUT
CommandCount=3

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_APP_EXIT
Command2=ID_MDRAW
Command3=ID_APP_ABOUT
CommandCount=3

[DLG:IDD_DIALOG1]
Type=1
Class=CLeftPortion
ControlCount=16
Control1=IDC_STATIC,button,1342308359
Control2=IDC_SLIDER1,msctls_trackbar32,1342242825
Control3=IDC_SLIDER2,msctls_trackbar32,1342242825
Control4=IDC_STATIC,button,1342308359
Control5=IDC_SLIDER3,msctls_trackbar32,1342242825
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,button,1342308359
Control9=IDC_SLIDER4,msctls_trackbar32,1342242825
Control10=IDC_SLIDER5,msctls_trackbar32,1342242825
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,button,1342308359
Control14=IDC_CURFACE,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352

[CLS:CLeftPortion]
Type=0
HeaderFile=LeftPortion.h
ImplementationFile=LeftPortion.cpp
BaseClass=CFormView
Filter=D
LastObject=CLeftPortion
VirtualFilter=VWC


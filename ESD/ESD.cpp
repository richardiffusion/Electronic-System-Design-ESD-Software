// ESD.cpp : Defines the class behaviors for the application.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2007 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESD.h"
#include "io.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "system.h"

#include "Dlg_About.h"
#include "Dlg_FirstPage.h"
#include ".\esd.h"

#include "Dlg_File_Insert.h"
#include "Dlg_Print_Setup_SCH.h"
#include "Dlg_Print_Setup_PCB.h"
#include "Dlg_Perference.h"

#include "Dlg_LibManageA.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESDApp

BEGIN_MESSAGE_MAP(CESDApp, CWinApp)
	//{{AFX_MSG_MAP(CESDApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_WINDOW_CLOSE_ALL, OnWindowCloseAll)
	//}}AFX_MSG_MAP

	//MFC标准的文件打开模式
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	//自定义模式，打开open和新建new 命令
	ON_COMMAND(ID_FILE_NEW_PROJECT, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)

	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)

	ON_COMMAND(ID_PERFERENCE, OnPerferences)
	ON_COMMAND(ID_FILE_PRINT_PAGE_SETUP, OnFilePrintPageSetup)

	ON_COMMAND(ID_HELP_TIP, OnHelpTip)
	ON_COMMAND(ID_DOWNLOAD_ONLINE, OnDownloadOnline)
	//ON_COMMAND(33330, &CESDApp::On33330)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESDApp construction

CESDApp::CESDApp()
{
	EnableManifestEditor();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CESDApp object

CESDApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CESDApp initialization

BOOL CESDApp::InitInstance()
{
	//使用GDI+
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	AfxEnableControlContainer();
	CXTPWinDwmWrapper().SetProcessDPIAware();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	AfxOleInit();

#if _MSC_VER <= 1200 // MFC 6.0 or earlier
#ifdef _AFXDLL
	Enable3dControls();         // Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();   // Call this when linking to MFC statically
#endif
#endif // MFC 6.0 or earlier

	TCHAR szSystemPath[_MAX_PATH];

	VERIFY(::GetModuleFileName(
		AfxGetApp()->m_hInstance, szSystemPath, _MAX_PATH));		

	csEDA_System_Path = szSystemPath;
	int nIndex  = csEDA_System_Path.ReverseFind(_T('\\'));
	if (nIndex > 0) 	csEDA_System_Path = csEDA_System_Path.Left(nIndex);
	else 				csEDA_System_Path.Empty();


	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("ESD_V4"));

	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)


	csESDViewClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,\
										0,\
										NULL,\
										0);	

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_ESDTYPE,
		RUNTIME_CLASS(CESDDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CESDView));
	AddDocTemplate(pDocTemplate);



	Init_EDAElements();
	LoadSystemParam();

	CDlg_FirstPage* firstpage;
	firstpage = new CDlg_FirstPage();
	firstpage->Create(IDD_FIRSTPAGE,NULL);
	firstpage->ShowWindow(SW_HIDE);
	firstpage->CenterWindow(firstpage);

	/*RECT rect;
	firstpage->GetWindowRect(&rect);
	firstpage->MoveWindow( rect.left + (int)(((rect.right - rect.left) - 600)/2), \
						   rect.top  + (int)(((rect.bottom - rect.top) - 400)/2), \
						   rect.right - rect.left,    rect.bottom - rect.top );*/
	CRgn myrgn;
	myrgn.CreateRectRgn(0,0,2400,1600);
	firstpage->SetWindowRgn((HRGN)myrgn,TRUE); 
	firstpage->ShowWindow(SW_SHOWNORMAL);
	firstpage->SetWindowPos(&CWnd::wndTopMost, 0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	firstpage->RedrawWindow();
	Sleep(4000);

	char libstring[MAX_PATH];
	CString cstring;
	sprintf(libstring,"%s\\SCH_V4.dat",csEDA_System_Path);
	if(cfileEDA_SchLibrary.Open(libstring, CFile::modeRead) != TRUE)
	{
		AfxMessageBox(szEDA_ProgramAbort, MB_ICONWARNING);
		return FALSE;
	}
	else
	{	
		int file_length = cfileEDA_SchLibrary.GetLength();
		if( file_length != 884369318)  
		{
			AfxMessageBox("无法识别系统元件库文件格式, 程序终止.", MB_ICONWARNING);
			return FALSE;
		}
		else
		{
			for(int i=0; i<SCH_SYSTEM_LIB_NUMBER; i++)
			{
				int offset;
				offset = sch_library_offset[i];
				cfileEDA_SchLibrary.Seek(offset, CFile::begin);

				Struct_Library_Head lib_head;
				cfileEDA_SchLibrary.Read(&lib_head, 32);

				for(int j = 0; j < lib_head.totalcompnum; j++)
				{
					Struct_Comp_Head comp_head;
					cfileEDA_SchLibrary.Read(&comp_head, 320);

					Struct_CompList complist;
					strcpy(complist.compname, comp_head.name);
					complist.basiclibrary = TRUE;
					complist.liboffset = offset;
					complist.offset = comp_head.offset;
					complist.length = comp_head.length;
					arrayEDA_SchLibraryCompName[i].Add(complist);
				}
			}
		}
	}

	FILE *fp;
	strcpy(libstring, csEDA_System_Path);
	strcat(libstring, "\\mscvs.dll");
	char s[260];
	int  n = 200;
	if((fp = fopen(libstring, "r")) != NULL)
	{
		for(;;)
		{
			if(fgets(s,n,fp) == NULL) break;

			int i=strlen(s);
			s[i-1]='\0';
			CString cstring = s;
			arrayEDA_SchLibraryTreeStructure.Add(cstring);
		}
	
		fclose(fp);
	}

	WIN32_FIND_DATA ff;
	sprintf(libstring,"%s\\schlib\\*.lib",csEDA_System_Path);
	HANDLE hFile = FindFirstFile(libstring, &ff);
	if(hFile != INVALID_HANDLE_VALUE)
    { 
		cstring = ff.cFileName;
		for(;;)
		{
			sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, cstring);

			CFile cfileUserLibrary;
			if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE) continue;  

			CArchive ar(&cfileUserLibrary, CArchive::load);

			CString headformat;
			ar >> headformat;
			if(headformat == SCH_Library_Head)	arrayEDA_SchUserLibraryName.Add(cstring);
			ar.Close();
			cfileUserLibrary.Close();

			if(FindNextFile(hFile,&ff) == 0) break;
			cstring = ff.cFileName;
		}
    }
	
	sprintf(libstring,"%s\\PCB_V4.dat",csEDA_System_Path);
	if(cfileEDA_PcbLibrary.Open(libstring, CFile::modeRead) != TRUE)
	{
		AfxMessageBox(szEDA_ProgramAbort, MB_ICONWARNING);
		return FALSE;
	}
	else
	{	
			int file_length = cfileEDA_PcbLibrary.GetLength();
		if( file_length != 545058036)
		{
			AfxMessageBox("无法识别系统封装库文件格式, 程序终止.", MB_ICONWARNING);
			return FALSE;
		}
		else
		{
			for(int i=0; i<PCB_SYSTEM_LIB_NUMBER; i++)
			{
				int offset;
				offset = pcb_library_offset[i];
				cfileEDA_PcbLibrary.Seek(offset, CFile::begin);

				Struct_Library_Head lib_head;
				cfileEDA_PcbLibrary.Read(&lib_head, 32);

				for(int j = 0; j < lib_head.totalcompnum; j++)
				{
					Struct_Comp_Head comp_head;
					cfileEDA_PcbLibrary.Read(&comp_head, 320);

					Struct_CompList complist;
					strcpy(complist.compname, comp_head.name);
					complist.basiclibrary = TRUE;
					complist.liboffset = offset;
					complist.offset = comp_head.offset;
					complist.length = comp_head.length;
					arrayEDA_PcbLibraryCompName[i].Add(complist);
				}
			}
		}
	}

	strcpy(libstring, csEDA_System_Path);
	strcat(libstring, "\\mscvp.dll");
	n = 200;
	if((fp = fopen(libstring, "r")) != NULL)
	{
		for(;;)
		{
			if(fgets(s,n,fp) == NULL) break;

			int i=strlen(s);
			s[i-1]='\0';
			CString cstring = s;
			arrayEDA_PcbLibraryTreeStructure.Add(cstring);
		}
		fclose(fp);
	}
	
	sprintf(libstring,"%s\\pcblib\\*.lib",csEDA_System_Path);
	hFile = FindFirstFile(libstring, &ff);
	if(hFile != INVALID_HANDLE_VALUE)
    { 
		cstring = ff.cFileName;
		for(;;)
		{
			sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, cstring);

			CFile cfileUserLibrary;
			if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE) continue; 

			CArchive ar(&cfileUserLibrary, CArchive::load);

			CString headformat;
			ar >> headformat;
			if(headformat == PCB_Library_Head)	arrayEDA_PcbUserLibraryName.Add(cstring);
			ar.Close();
			cfileUserLibrary.Close();

			if(FindNextFile(hFile,&ff) == 0) break;
			cstring = ff.cFileName;
		}
    }

	//================ create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_SCH_MAINMENU))   //IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	int nCmdShow = m_nCmdShow;

	pMainFrame->InitialShowWindow(nCmdShow);
	pMainFrame->UpdateWindow();

	firstpage->DestroyWindow();
	delete firstpage;

	EnumFontFamilies(pMainFrame->GetDC()->GetSafeHdc(), (LPCTSTR)NULL, (FONTENUMPROCA)MyEnumFontFamProc,(LPARAM)NULL);
	int judge = 1;
	for(;;)
	{
		if(judge == 1) judge = 0;
		else if(judge == 0) break;
		for(int i=0; i<arrayEDA_SystemFont.GetCount()-1;i++)
		{
			CString str1 = arrayEDA_SystemFont.GetAt(i);
			CString str2 = arrayEDA_SystemFont.GetAt(i+1);
			if(str1 > str2)
			{
				judge=1;
				arrayEDA_SystemFont.SetAt(i, str2);
				arrayEDA_SystemFont.SetAt(i+1, str1);
			}
		}
	}
	
	CXTTipOfTheDay dlg;
	if (dlg.m_bStartup) 
	{
		OnTipoftheday();
	}
	return TRUE;
}


// App command to run the dialog
void CESDApp::OnAppAbout()
{
	CDlg_About dlgAbout;
	
	////////////////////////////////////////////////////////////////////////////////////
	long nRc;
	HKEY hReadKey;
	DWORD dwDataType;
	DWORD dwLength;
	char szUser[255];
	char szCompany[255];
	char szSerial[255];

	strcpy(szUser,"未知");
	strcpy(szCompany,"未知");
	strcpy(szSerial,"未知");

	//open the reg
	nRc = RegOpenKeyEx(HKEY_CLASSES_ROOT,"dgnFile\\info",0,KEY_READ,&hReadKey);
	if (nRc == ERROR_SUCCESS)
	{
		dwLength = sizeof(szUser);
		nRc = RegQueryValueEx(hReadKey,"user",NULL,&dwDataType,(LPBYTE)szUser,&dwLength);
		dwLength = sizeof(szSerial);
		nRc = RegQueryValueEx(hReadKey,"serial",NULL,&dwDataType,(LPBYTE)szSerial,&dwLength);
		dwLength = sizeof(szCompany);
		nRc = RegQueryValueEx(hReadKey,"company",NULL,&dwDataType,(LPBYTE)szCompany,&dwLength);
		nRc=RegCloseKey(hReadKey);
	}
	

	/////////////////////////////////////////////////////////////////////////////////////
	dlgAbout.m_szSerial = szSerial;
	dlgAbout.m_szUnit = szCompany;
	dlgAbout.m_szUser = szUser;

	dlgAbout.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CESDApp message handlers


void CESDApp::OnWindowCloseAll()
{
	CloseAllDocuments(FALSE);

}

void CESDApp::OnFileOpen()
{
	if(nEDA_System_State == SCH)
	{
		static char BASED_CODE szFileFilter[] = "所有设计文件(*.eda;*.dgn;*.schdoc;*.sch;*.asc;*.dxf)|*.eda;*.dgn;*.schdoc;*.sch;*.asc;*.dxf|\
												ESD_V4设计文件 (*.eda) |*.eda|\
												ESD_V1设计文件 (*.dgn) |*.dgn|\
												Altium Designer ASCII 5.0 SCH (*.schdoc) |*.schdoc|\
												Protel 99 Schematic ASCII (*.asc,*.sch) |*.asc;*.sch|\
												AutoCAD dxf (*.dxf) |*.dxf|\
												所有文件 (*.*) |*.*||";
		CFileDialog importdialog(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_ENABLESIZING,szFileFilter,NULL);  //OFN_ALLOWMULTISELECT

		char filename[MAX_PATH];
		int filetype = -1;
		if(importdialog.DoModal() == IDOK) 
		{
			CFile myfile;
			if(myfile.Open(importdialog.GetPathName(), CFile::modeRead) == TRUE)
			{
				CArchive ar(&myfile, CArchive::load);
				CString onelinestr;
				if(ar.ReadString(onelinestr) == TRUE)
				{
					if(onelinestr.Find(csESDFileHead_V1_1) >= 0) 																		filetype = DOC_ESD;
					else if(onelinestr.Find(csESDFileHead_V1_2) >= 0) 																	filetype = DOC_ESD;
					else if(onelinestr.Find(csESDFileHead_V1_0) >= 0) 																	filetype = DOC_ESD;
					else if(onelinestr.Find("|HEADER=Protel for Windows - Schematic Capture Ascii File Version 5.0") >= 0) 				filetype = DOC_AD6_SCH;
					else if(onelinestr.Find("Protel for Windows - Schematic Capture Ascii File Version") >= 0)							filetype = DOC_99_SCH;
					else if(IsAD6SchBinary(myfile, onelinestr) == TRUE)
					{
						AfxMessageBox("该文件是Altium SCH V5.0 二进制数据格式, 无法识别, 请保存为ASCII格式再读入。");
					}
				}

				ar.Close();
				myfile.Close();
			}

			if(filetype >= 0)
			{
				if(filetype == DOC_ESD)
				{
					strcpy(filename, importdialog.GetPathName());
					CString newName = filename;
					AfxGetApp()->OpenDocumentFile(newName);
				}
				else
				{
					CESDDoc* pDoc = (CESDDoc*) GetCurrentDoc();
					CESDView* pView = (CESDView*)GetCurrentView();

					if((pDoc == NULL)||(pView == NULL)||(pDoc->m_nDocumentType == DOC_AD6_SCH)||(pDoc->m_nDocumentType == DOC_99_SCH))
					{
						strcpy(filename, importdialog.GetPathName());
						CString newName = filename;
						AfxGetApp()->OpenDocumentFile(newName);						
					}
					else
					{
						if(pDoc->Get_SCH_Elements_Param(0, pView->m_nDiagram, 0) > 0)
						{
							strcpy(filename, importdialog.GetPathName());
							CString newName = filename;
							AfxGetApp()->OpenDocumentFile(newName);
						}
						else
						{
							CDlg_File_Insert dlg;
							dlg.m_nOpenMode = 0;
							dlg.DoModal();
							if(dlg.m_nOpenMode == 0)
							{
								strcpy(filename, importdialog.GetPathName());
								CString newName = filename;
								AfxGetApp()->OpenDocumentFile(newName);
							}
							else
							{
								CFile insertfile;
								if(insertfile.Open(importdialog.GetPathName(), CFile::modeRead) == TRUE)
								{
									CArchive insertar(&insertfile, CArchive::load);
									CString onelinestr;
									if(insertar.ReadString(onelinestr) == TRUE)
									{
										if(filetype == DOC_AD6_SCH)
										{
											int total_linenum;
											int pos = onelinestr.ReverseFind('=');
											if(pos >= 0)
											{
												pos = onelinestr.GetLength() - pos - 1;				
												onelinestr = onelinestr.Right(pos);
												total_linenum = atoi(onelinestr);  //ad6 ASCII文件总行数
											}
											else total_linenum = 0;
											if(total_linenum <= 0) 
											{
												AfxMessageBox("文件格式无法识别, 打开文件失败。", MB_ICONERROR);
												return;
											}
											char sss[100]; sprintf(sss, "%s%d", "插入AD6电路图到分页:", pView->m_nDiagram + 1);
											AfxMessageBox(sss);	

											pDoc->Open_AD6_SCH_ASCII_File(insertar, total_linenum, pView->m_nDiagram);
										}
										else
										{

											char sss[100]; sprintf(sss, "%s%d", "插入99电路图到分页:", pView->m_nDiagram + 1);
											AfxMessageBox(sss);	
											pDoc->Open_99_SCH_ASCII_File(insertar,pView->m_nDiagram);
										}
										pView->Invalidate();
									}
								}
								else
								{
									AfxMessageBox("文件无法打开。", MB_ICONERROR);
								}
							}
						}
					}

				}
			}
			else
			{
				AfxMessageBox("文件无法打开或未知的文件格式。", MB_ICONERROR);
			}
		}
	}
	else
	{
		static char BASED_CODE szFileFilter[] = "所有设计文件(*.eda;*.dgn;*.pcbdoc;*.pcb;*.dxf)|*.eda;*.dgn;*.pcbdoc;*.pcb;*.dxf|\
												EDA_V4 (*.eda) |*.eda|\
												EDA_V1 (*.dgn) |*.dgn|\
												Altium Designer ASCII 5.0 PCB (*.pcbdoc) |*.pcbdoc|\
												Protel 99 PCB ASCII 3.0 (*.pcb) |*.pcb|\
												AutoCAD dxf (*.dxf) |*.dxf|\
												所有文件 (*.*) |*.*||";
		CFileDialog importdialog(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_ENABLESIZING,szFileFilter,NULL);  //OFN_ALLOWMULTISELECT

		char filename[MAX_PATH];
		if(importdialog.DoModal() == IDOK) 
		{
			strcpy(filename, importdialog.GetPathName());
			CString newName = filename;
			AfxGetApp()->OpenDocumentFile(newName);
		}
	}
}

int CESDApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	GdiplusShutdown(m_gdiplusToken);
	
	if(cfileEDA_SchLibrary.m_hFile != (HANDLE)(-1)) cfileEDA_SchLibrary.Close();
	
	while (!listEDA_SchClipBoard.IsEmpty())
	{
		delete listEDA_SchClipBoard.RemoveHead();
	}

	while (!arrayEDA_PcbClipBoardArc.IsEmpty())
	{
		delete arrayEDA_PcbClipBoardArc.GetAt(0);
        arrayEDA_PcbClipBoardArc.RemoveAt(0);
	}
	while (!arrayEDA_PcbClipBoardPad.IsEmpty())
	{
		delete arrayEDA_PcbClipBoardPad.GetAt(0);
        arrayEDA_PcbClipBoardPad.RemoveAt(0);
	}
	while (!arrayEDA_PcbClipBoardVia.IsEmpty())
	{
		delete arrayEDA_PcbClipBoardVia.GetAt(0);
        arrayEDA_PcbClipBoardVia.RemoveAt(0);
	}
	while (!arrayEDA_PcbClipBoardTrack.IsEmpty())
	{
		delete arrayEDA_PcbClipBoardTrack.GetAt(0);
        arrayEDA_PcbClipBoardTrack.RemoveAt(0);
	}
	while (!arrayEDA_PcbClipBoardText.IsEmpty())
	{
		delete arrayEDA_PcbClipBoardText.GetAt(0);
        arrayEDA_PcbClipBoardText.RemoveAt(0);
	}
	while (!arrayEDA_PcbClipBoardFill.IsEmpty())
	{
		delete arrayEDA_PcbClipBoardFill.GetAt(0);
        arrayEDA_PcbClipBoardFill.RemoveAt(0);
	}
	while (!arrayEDA_PcbClipBoardRegion.IsEmpty())
	{
		delete arrayEDA_PcbClipBoardRegion.GetAt(0);
        arrayEDA_PcbClipBoardRegion.RemoveAt(0);
	}
	while (!arrayEDA_PcbClipBoardCoordinate.IsEmpty())
	{
		delete arrayEDA_PcbClipBoardCoordinate.GetAt(0);
        arrayEDA_PcbClipBoardCoordinate.RemoveAt(0);
	}
	while (!arrayEDA_PcbClipBoardDimension.IsEmpty())
	{
		delete arrayEDA_PcbClipBoardDimension.GetAt(0);
        arrayEDA_PcbClipBoardDimension.RemoveAt(0);
	}
	while (!arrayEDA_PcbClipBoardComp.IsEmpty())
	{
		delete arrayEDA_PcbClipBoardComp.GetAt(0);
		arrayEDA_PcbClipBoardComp.RemoveAt(0);
	}
	while (!arrayEDA_PcbClipBoardPolygon.IsEmpty())
	{
		delete arrayEDA_PcbClipBoardPolygon.GetAt(0);
		arrayEDA_PcbClipBoardPolygon.RemoveAt(0);
	}

	SaveSystemParam();

	return CWinApp::ExitInstance();
}

CDocument* CESDApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	CFile myfile;
	if(myfile.Open(lpszFileName, CFile::modeRead) == TRUE)
	{
		CArchive ar(&myfile, CArchive::load);
		CString onelinestr;
		if(ar.ReadString(onelinestr) == TRUE)
		{
			if(nEDA_System_State == PCB)
			{
				if( (onelinestr.Find("|HEADER=Protel for Windows - Schematic Capture Ascii File Version 5.0") >= 0)||\
					(onelinestr.Find("Protel for Windows - Schematic Capture Ascii File Version") >= 0) )
				{
					CMainFrame* pMainFrame = (CMainFrame*)this->m_pMainWnd;
					pMainFrame->OnToSch1();
				}
			}
			else
			{
				if( onelinestr.Find("|KIND=Protel_Advanced_PCB") >= 0)	
				{
					CMainFrame* pMainFrame = (CMainFrame*)this->m_pMainWnd;
					pMainFrame->OnToPcb1();
				}
			}
		}

		ar.Close();
		myfile.Close();
	}

	return CWinApp::OpenDocumentFile(lpszFileName);
}

void CESDApp::OnPerferences()
{
	CDlg_Perference dlg;

	dlg.m_nComboCursor = nEDA_Cursor_Mode;

	dlg.m_bCheckAutoJunc =  bEDA_AutoJunction;	
	dlg.m_nAutoJuncColor = cEDA_schautojunc.m_nColor;
	dlg.m_bCheckAutoWire =  bEDA_AutoOptimizeWire;
	dlg.m_bCheckAutoDiagram = bEDA_Sheet_AutoOn;
	dlg.m_bCheckBackup = bEDA_Backup;
	dlg.m_nEditBackup = nEDA_Backup_Time;

	dlg.m_fEditNameOffset = fEDA_PinNameOffset;
	dlg.m_fEditNumberOffset = fEDA_PinNumberOffset;

	dlg.m_csEditPowerGround = csEDA_PowerGround;
	dlg.m_csEditSignalGround = csEDA_SignalGround;
	dlg.m_csEditEarth = csEDA_EarthGround;

	dlg.m_nComboGrid = nEDA_SCH_VisibleGridMode;
	dlg.m_nGridColor = nEDA_SCH_VisibleGridColor;
	dlg.m_fEditI1 = fEDA_SCH_SnapGrid_I[0];
	dlg.m_fEditI2 = fEDA_SCH_SnapGrid_I[1];
	dlg.m_fEditI3 = fEDA_SCH_SnapGrid_I[2];

	dlg.m_nRadioIDSuffix = nEDA_PartID_Suffix;
	
	double f = fEDA_SCH_SnapGrid_M[0]/1000*25.4;
	int i1 = f*1000;  int i2 = f*10000 - i1*10; if(i2 >=5) i1++;
	f = (double)i1/1000;
	dlg.m_fEditM1 = f;
	f = fEDA_SCH_SnapGrid_M[1]/1000*25.4;
	i1 = f*1000;  i2 = f*10000 - i1*10; if(i2 >=5) i1++;
	f = (double)i1/1000;
	dlg.m_fEditM2 = f;
	f = fEDA_SCH_SnapGrid_M[2]/1000*25.4;
	i1 = f*1000;  i2 = f*10000 - i1*10; if(i2 >=5) i1++;
	f = (double)i1/1000;
	dlg.m_fEditM3 = f;

	dlg.m_nSelColor = nEDA_SelectionColor;	
	dlg.m_bCheckClearSel = nEDA_ClickClearSelection;

	dlg.m_nComboSheetSize = nEDA_SheetStandardStyle;

	if(dlg.DoModal() == IDOK)
	{
		nEDA_Cursor_Mode = dlg.m_nComboCursor;

		bEDA_AutoJunction = dlg.m_bCheckAutoJunc;
		cEDA_schautojunc.m_nColor = dlg.m_nAutoJuncColor;
		bEDA_AutoOptimizeWire = dlg.m_bCheckAutoWire;
		bEDA_Sheet_AutoOn = dlg.m_bCheckAutoDiagram;
		bEDA_Backup = dlg.m_bCheckBackup;
		nEDA_Backup_Time = dlg.m_nEditBackup;

		fEDA_PinNameOffset = dlg.m_fEditNameOffset;
		fEDA_PinNumberOffset = dlg.m_fEditNumberOffset;

		csEDA_PowerGround = dlg.m_csEditPowerGround;
		csEDA_SignalGround = dlg.m_csEditSignalGround;
		csEDA_EarthGround = dlg.m_csEditEarth;

		nEDA_SCH_VisibleGridMode = dlg.m_nComboGrid;
		nEDA_SCH_VisibleGridColor = dlg.m_nGridColor;
		if(dlg.m_fEditI1 <= 0) fEDA_SCH_SnapGrid_I[0] = 1;
		else fEDA_SCH_SnapGrid_I[0] = dlg.m_fEditI1;
		if(dlg.m_fEditI2 <= 0) fEDA_SCH_SnapGrid_I[1] = 1;
		else fEDA_SCH_SnapGrid_I[1] = dlg.m_fEditI2;
		if(dlg.m_fEditI3 <= 0) fEDA_SCH_SnapGrid_I[2] = 1;
		else fEDA_SCH_SnapGrid_I[2] = dlg.m_fEditI3;
		if(dlg.m_fEditM1 <= 0) fEDA_SCH_SnapGrid_M[0] = 1/25.4*1000;
		else fEDA_SCH_SnapGrid_M[0] = dlg.m_fEditM1/25.4*1000;
		if(dlg.m_fEditM2 <= 0) fEDA_SCH_SnapGrid_M[1] = 1/25.4*1000;
		else fEDA_SCH_SnapGrid_M[1] = dlg.m_fEditM2/25.4*1000;
		if(dlg.m_fEditM3 <= 0) fEDA_SCH_SnapGrid_M[2] = 1/25.4*1000;
		else fEDA_SCH_SnapGrid_M[2] = dlg.m_fEditM3/25.4*1000;

		nEDA_PartID_Suffix = dlg.m_nRadioIDSuffix;

		nEDA_SelectionColor = dlg.m_nSelColor;
		nEDA_ClickClearSelection = dlg.m_bCheckClearSel;

		nEDA_SheetStandardStyle = dlg.m_nComboSheetSize;

		if(bEDA_Backup == TRUE)
		{
			CMainFrame* pMainFrame = (CMainFrame*)this->m_pMainWnd;
			if(nEDA_Backup_Time < 1) nEDA_Backup_Time = 1;
			UINT u = nEDA_Backup_Time*60000;
			if(u < 60000) u = 60000;
			pMainFrame->KillTimer(ID_AUTO_BACKUP_TIMER);
			pMainFrame->SetTimer(ID_AUTO_BACKUP_TIMER,u,NULL);
		}
	}
}

CDocument* CESDApp::GetCurrentDoc()
{
	CMainFrame* pMainFrame = (CMainFrame*)(this->m_pMainWnd);
	if(pMainFrame == NULL) return NULL;
	CChildFrame* pActiveChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	if(pActiveChildFrame == NULL) return NULL;
	CESDView* pView = (CESDView*)pActiveChildFrame->GetActiveView();
	if(pView == NULL) return NULL;
	CESDDoc* pDoc = pView->GetDocument();
	return (CDocument*)pDoc;
}

CView* CESDApp::GetCurrentView()
{
	CMainFrame* pMainFrame = (CMainFrame*)(this->m_pMainWnd);
	if(pMainFrame == NULL) return NULL;
	CChildFrame* pActiveChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	if(pActiveChildFrame == NULL) return NULL;
	CESDView* pView = (CESDView*)pActiveChildFrame->GetActiveView();
	return (CView*)pView;
}

void CESDApp::Init_EDAElements()
{
	cEDA_schwire.m_nColor = 128;
	cEDA_schwire.m_nAttrib = LINE_WIRE;
	cEDA_schwire.m_nLineWidth = LINE_SMALL;
	cEDA_schwire.m_nLineType = LINE_SOLID;
	
	cEDA_schbus.m_nColor = 8388608;
	cEDA_schbus.m_nAttrib = LINE_BUS;
	cEDA_schbus.m_nLineWidth = LINE_SMALL;
	cEDA_schbus.m_nLineType = LINE_SOLID;

	cEDA_schbusentry.m_nColor = 8388608;
	cEDA_schbusentry.m_nAttrib = LINE_BUS_ENTRY;
	cEDA_schbusentry.m_nLineWidth = LINE_SMALL;
	cEDA_schbusentry.m_nLineType = LINE_SOLID;

	cEDA_schpolyline.m_nColor = 0;
	cEDA_schpolyline.m_nLineWidth = LINE_SMALL;
	cEDA_schpolyline.m_nLineType = LINE_SOLID;
	cEDA_schpolyline.m_nStartLineShape = 0;
	cEDA_schpolyline.m_nEndLineShape = 0;
	cEDA_schpolyline.m_nLineShapeSize = LINE_SMALLEST;
	
	cEDA_schjunc.m_nColor = 128;
	cEDA_schjunc.m_nSize = JUNCTION_SMALLEST;
	cEDA_schautojunc.m_nColor = RGB(0,0,255);
	cEDA_schautojunc.m_nSize = JUNCTION_SMALLEST;
        
	cEDA_schtext.m_fSx = 0; cEDA_schtext.m_fSy = 0;
	cEDA_schtext.m_nID = TEXT_TEXT;
	cEDA_schtext.m_nOrientation = 0;
	cEDA_schtext.m_nColor = 8388608;
	cEDA_schtext.m_nFont = 0;
	cEDA_schtext.m_csText = "Text";
	cEDA_schtext.m_nH_Justification = 0;
	cEDA_schtext.m_nV_Justification = 0;
	cEDA_schtext.m_bMirror = FALSE;

	cEDA_textfont.lfHeight = 10;
	cEDA_textfont.lfWidth = 0;
	cEDA_textfont.lfEscapement = cEDA_textfont.lfOrientation = 0;
	cEDA_textfont.lfWeight = FW_NORMAL;
	cEDA_textfont.lfItalic = 0;
	cEDA_textfont.lfUnderline = 0;
	cEDA_textfont.lfStrikeOut = 0;
	cEDA_textfont.lfCharSet = DEFAULT_CHARSET;
	cEDA_textfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	cEDA_textfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	cEDA_textfont.lfQuality = PROOF_QUALITY;
	cEDA_textfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(cEDA_textfont.lfFaceName, "Times New Roman");
				
	cEDA_schnetlabel.m_fSx = 0; cEDA_schnetlabel.m_fSy = 0;
	cEDA_schnetlabel.m_nID = TEXT_NETLABEL;
	cEDA_schnetlabel.m_nOrientation = 0;
	cEDA_schnetlabel.m_nColor = 128;
	cEDA_schnetlabel.m_nFont = 0;
	cEDA_schnetlabel.m_csText = "Netlabel";
	cEDA_schnetlabel.m_nH_Justification = 0;
	cEDA_schnetlabel.m_nV_Justification = 0;
	cEDA_schnetlabel.m_bMirror = FALSE;

	cEDA_netlabelfont.lfHeight = 10;
	cEDA_netlabelfont.lfWidth = 0;
	cEDA_netlabelfont.lfEscapement = cEDA_netlabelfont.lfOrientation = 0;
	cEDA_netlabelfont.lfWeight = FW_NORMAL;
	cEDA_netlabelfont.lfItalic = 0;
	cEDA_netlabelfont.lfUnderline = 0;
	cEDA_netlabelfont.lfStrikeOut = 0;
	cEDA_netlabelfont.lfCharSet = DEFAULT_CHARSET;
	cEDA_netlabelfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	cEDA_netlabelfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	cEDA_netlabelfont.lfQuality = PROOF_QUALITY;
	cEDA_netlabelfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(cEDA_netlabelfont.lfFaceName, "Times New Roman");

	cEDA_scharc.m_nWidth = LINE_SMALL;
	cEDA_scharc.m_nColor = 16711680;

	cEDA_schellipse.m_nWidth = LINE_SMALL;
	cEDA_schellipse.m_nColor = 16711680;

	cEDA_schellipsefill.m_nBorderWidth = LINE_SMALLEST;
	cEDA_schellipsefill.m_nBorderColor = 16711680;
	cEDA_schellipsefill.m_nFillColor = 12632256;
	cEDA_schellipsefill.m_bFillSolid = TRUE;
	cEDA_schellipsefill.m_bTransparent = FALSE;

	cEDA_schpie.m_nBorderWidth = LINE_SMALLEST;
	cEDA_schpie.m_nBorderColor = 16711680;
	cEDA_schpie.m_nFillColor = 12632256;
	cEDA_schpie.m_bFillSolid = TRUE;

	cEDA_schpolygon.m_nBorderWidth = LINE_SMALLEST;
	cEDA_schpolygon.m_nBorderColor  = 16711680;
	cEDA_schpolygon.m_nFillColor = 12632256;
	cEDA_schpolygon.m_bFillSolid = TRUE;
	cEDA_schpolygon.m_bTransparent = FALSE;

	cEDA_schrect.m_nBorderWidth = LINE_SMALLEST;
	cEDA_schrect.m_nBorderColor = 128;
	cEDA_schrect.m_nFillColor = 11599871;
	cEDA_schrect.m_bFillSolid = TRUE;
	cEDA_schrect.m_bTransparent = FALSE;

	cEDA_schroundrect.m_nBorderWidth = LINE_SMALLEST;
	cEDA_schroundrect.m_nBorderColor = 16711680;
	cEDA_schroundrect.m_nFillColor = 12632256;
	cEDA_schroundrect.m_bFillSolid = TRUE;
	cEDA_schroundrect.m_fRadius_x = 100;
	cEDA_schroundrect.m_fRadius_y = 100;

	cEDA_schtextframe.m_nBorderWidth = LINE_SMALLEST;
	cEDA_schtextframe.m_nFont = 0;
	cEDA_schtextframe.m_bClipToArea = FALSE;
	cEDA_schtextframe.m_bWordWrap = FALSE;
	cEDA_schtextframe.m_nAlignment = TEXT_ALIGNMENT_LEFT;
	cEDA_schtextframe.m_nTextColor = 0;
	cEDA_schtextframe.m_bDrawBorder = FALSE;
	cEDA_schtextframe.m_nBorderColor = 0;
	cEDA_schtextframe.m_nFillColor = RGB(255,255,255);
	cEDA_schtextframe.m_bFillSolid = TRUE;

	cEDA_schgraphic.m_nBorderWidth =  LINE_SMALLEST;
	cEDA_schgraphic.m_nBorderColor =  8388608;
	cEDA_schgraphic.m_bBorderOn = FALSE;
	cEDA_schgraphic.m_bAspect = TRUE;

	cEDA_schbezier.m_nWidth = LINE_SMALL;
	cEDA_schbezier.m_nColor = 8388608;

	cEDA_schport.m_fSize = 600;
	cEDA_schport.m_nStyle = PORT_STYLE_NH;
	cEDA_schport.m_nType = 0;
	cEDA_schport.m_nBorderColor = 128;
	cEDA_schport.m_nFillColor = 8454143;
	cEDA_schport.m_nTextColor = 128;
	cEDA_schport.m_nAlignment = 0;
	cEDA_schport.m_csName = "Port";

	cEDA_schpower.m_nStyle = 0;
	cEDA_schpower.m_nOrientation = 0;
	cEDA_schpower.m_bShowNetName = TRUE;
	cEDA_schpower.m_bIsCrossSheetConnector = FALSE;
	cEDA_schpower.m_csNetName = "VCC";
	cEDA_schpower.m_nColor = 128;

	cEDA_schground.m_nStyle = 2;
	cEDA_schground.m_nOrientation = 0;
	cEDA_schground.m_bShowNetName = TRUE;
	cEDA_schground.m_bIsCrossSheetConnector = FALSE;
	cEDA_schground.m_csNetName = "GND";
	cEDA_schground.m_nColor = 128;	

	cEDA_schoffsheet.m_nStyle = 0;
	cEDA_schoffsheet.m_nOrientation = 0;
	cEDA_schoffsheet.m_bShowNetName = TRUE;
	cEDA_schoffsheet.m_bIsCrossSheetConnector = TRUE;
	cEDA_schoffsheet.m_csNetName = "Offsheet";
	cEDA_schoffsheet.m_nColor = 128;

	cEDA_schsheetsymbol.m_nBorderWidth = LINE_SMALLEST;
	cEDA_schsheetsymbol.m_nBorderColor = 128;
	cEDA_schsheetsymbol.m_nFillColor = 8454016;
	cEDA_schsheetsymbol.m_bFillSolid = TRUE;
	cEDA_schsheetsymbol.m_bShowHiddenTextFields = FALSE;
	csEDA_schsheetsymboldesignator = "Designator";
	csEDA_schsheetsymbolfilename = "File Name";

	cEDA_schsheetentry.m_nStyle = ENTRY_STYLE_NH;
	cEDA_schsheetentry.m_nType = ENTRY_TYPE_UNSPECIFIED;
	cEDA_schsheetentry.m_nBorderColor = 128;
	cEDA_schsheetentry.m_nFillColor = 8454143;
	cEDA_schsheetentry.m_nTextColor = 128;
	cEDA_schsheetentry.m_csName = "0";

	cEDA_schieee.m_fSize = 10.0;
	cEDA_schieee.m_nOrientation = 0;
	cEDA_schieee.m_nWidth = 0;
	cEDA_schieee.m_bMirrored = FALSE;
	cEDA_schieee.m_nIeeeSymbol = 1;
	cEDA_schieee.m_nColor = 255;

	cEDA_schpin.m_nColor = 0;

	cEDA_schcomp.m_nColor = 128;
	cEDA_schcomp.m_nFillColor = 11599871;
	cEDA_schcomp.m_nPinColor = 0;

	cEDA_pcbarc.m_fWidth = 10;
	cEDA_pcbtrack.m_fWidth = 10;
	// ----- text
	cEDA_pcbtext.m_fHeight = 80;
	cEDA_pcbtext.m_fWidth = 4;
	cEDA_pcbtext.m_fRotation = 0;
	cEDA_pcbtext.m_csText.Empty();
	cEDA_pcbtext.m_bMirror = FALSE;
	cEDA_pcbtext.m_nStrokeFont = 0;
	cEDA_pcbtext.m_bUseTTFont = FALSE;
	cEDA_pcbtext.m_bBold = FALSE;
	cEDA_pcbtext.m_bItalic = FALSE;
	cEDA_pcbtext.m_bInverted = FALSE;
	cEDA_pcbtext.m_csTTFontName = "Arial";
	cEDA_pcbtext.m_fInvertedBorder = 20;

	cEDA_pcbcomp.m_bNameOn = TRUE;
	cEDA_pcbcomp.m_nNamePosition = MANUAL;
	cEDA_pcbcomp.m_bCommentOn = TRUE;
	cEDA_pcbcomp.m_nCommentPosition = MANUAL;

	cEDA_pcbcomp_designator.m_csText = "Designator";
	cEDA_pcbcomp_designator.m_fHeight = 60;
	cEDA_pcbcomp_designator.m_fWidth = 10;
	cEDA_pcbcomp_designator.m_fRotation = 0;
	cEDA_pcbcomp_designator.m_bMirror = FALSE;
	cEDA_pcbcomp_designator.m_nStrokeFont = 0;
	cEDA_pcbcomp_designator.m_bUseTTFont = FALSE;
	cEDA_pcbcomp_designator.m_bBold = FALSE;
	cEDA_pcbcomp_designator.m_bItalic = FALSE;
	cEDA_pcbcomp_designator.m_bInverted = FALSE;
	cEDA_pcbcomp_designator.m_csTTFontName = "Arial";
	cEDA_pcbcomp_designator.m_fInvertedBorder = 20;	

	cEDA_pcbcomp_comment.m_csText = "Comment";
	cEDA_pcbcomp_comment.m_fHeight = 60;
	cEDA_pcbcomp_comment.m_fWidth = 10;
	cEDA_pcbcomp_comment.m_fRotation = 0;
	cEDA_pcbcomp_comment.m_bMirror = FALSE;
	cEDA_pcbcomp_comment.m_nStrokeFont = 0;
	cEDA_pcbcomp_comment.m_bUseTTFont = FALSE;
	cEDA_pcbcomp_comment.m_bBold = FALSE;
	cEDA_pcbcomp_comment.m_bItalic = FALSE;
	cEDA_pcbcomp_comment.m_bInverted = FALSE;
	cEDA_pcbcomp_comment.m_csTTFontName = "Arial";
	cEDA_pcbcomp_comment.m_fInvertedBorder = 20;	

	cEDA_pcbpad.m_fRotation = 0;
	cEDA_pcbpad.m_nPadMode = PAD_MODE_SIMPLE;
	for(int i=0; i<SIGNAL_LAYER_NUM; i++)
	{
		cEDA_pcbpad.m_fSizeX[i] = 60;
		cEDA_pcbpad.m_fSizeY[i] = 60;
		cEDA_pcbpad.m_nShape[i] = PAD_SHAPE_ROUND;
	}
	cEDA_pcbpad.m_nHoleType = PADHOLE_ROUND;
	cEDA_pcbpad.m_fHoleDia = 30;
	cEDA_pcbpad.m_fHoleRotation = 0;
	cEDA_pcbpad.m_fHoleWidth = 30;
	cEDA_pcbpad.m_csName = "0";
	cEDA_pcbpad.m_bTentingTop = FALSE;
	cEDA_pcbpad.m_bTentingBottom = FALSE;
	
	cEDA_pcbvia.m_fDia = 50;
	cEDA_pcbvia.m_fHoleDia = 28;
	cEDA_pcbvia.m_nStartLayer = TOPLAYER;
	cEDA_pcbvia.m_nEndLayer = BOTTOMLAYER;
	cEDA_pcbvia.m_bTentingTop = FALSE;
	cEDA_pcbvia.m_bTentingBottom = FALSE;

	cEDA_pcbpolygon.m_nHatchStyle = PCB_HATCH_SOLID;
	cEDA_pcbpolygon.m_nHatchStyle = PCB_HATCH_90;
	cEDA_pcbpolygon.m_bRemoveIslandsByArea = TRUE;
	cEDA_pcbpolygon.m_fAreaThreshold = 3000;
	cEDA_pcbpolygon.m_bRemoveNecks = TRUE;
	cEDA_pcbpolygon.m_fNeckWidthThreshold = 5;
	cEDA_pcbpolygon.m_fArcResolution = 0.5;
	cEDA_pcbpolygon.m_fTrackWidth = 20;
	cEDA_pcbpolygon.m_fGridSize = 20;
	cEDA_pcbpolygon.m_bUseOctagons = FALSE;
	cEDA_pcbpolygon.m_fMinPrimLength = 3;
	cEDA_pcbpolygon.m_bPrimitiveLocked = TRUE;
	cEDA_pcbpolygon.m_bLocked = FALSE;
	cEDA_pcbpolygon.m_bIgnoreViolations = TRUE;
	cEDA_pcbpolygon.m_fPourGap = 20;

	cEDA_pcbcoordinate.m_fSize = 10;
	cEDA_pcbcoordinate.m_fLineWidth = 6;
	cEDA_pcbcoordinate.m_fTextHeight = 60;
	cEDA_pcbcoordinate.m_fTextWidth = 4;
	cEDA_pcbcoordinate.m_fRotation = 0;
	cEDA_pcbcoordinate.m_nUnitStyle = UNIT_STYLE_BRACKETS;
	cEDA_pcbcoordinate.m_nStrokeFont = 0;
	cEDA_pcbcoordinate.m_bUseTTFont = FALSE;
	cEDA_pcbcoordinate.m_bBold = FALSE;
	cEDA_pcbcoordinate.m_bItalic = FALSE;
	cEDA_pcbcoordinate.m_csTTFontName = "Arial";

	cEDA_pcbdimension.m_fHeight = 10;
	cEDA_pcbdimension.m_fLineWidth = 6;
	cEDA_pcbdimension.m_fRotation = 0;
	cEDA_pcbdimension.m_fArrowSize = 60;
	cEDA_pcbdimension.m_fArrowLineWidth = 4;
	cEDA_pcbdimension.m_fArrowLength = 100;
	cEDA_pcbdimension.m_nArrowPosition = 0;
	cEDA_pcbdimension.m_fExtensionOffset = 10;
	cEDA_pcbdimension.m_fExtensionLineWidth = 10;
	cEDA_pcbdimension.m_fExtensionPickGap = 10;
	cEDA_pcbdimension.m_fTextHeight = 60;
	cEDA_pcbdimension.m_fTextLineWidth = 4;
	cEDA_pcbdimension.m_fTextRotation = 0;
	cEDA_pcbdimension.m_nTextPosition = 0;
	cEDA_pcbdimension.m_fTextGap = 10;
	cEDA_pcbdimension.m_nTextFormat = 0;
	cEDA_pcbdimension.m_nTextDimensionUnit = 0;
	cEDA_pcbdimension.m_nTextPrecision = 2;
	cEDA_pcbdimension.m_csPrefix.Empty();
	cEDA_pcbdimension.m_csSuffix.Empty();
	cEDA_pcbdimension.m_nUnitStyle = UNIT_STYLE_BRACKETS;
	cEDA_pcbdimension.m_nDimensionKind = 0;
	cEDA_pcbdimension.m_nStrokeFont = 0;
	cEDA_pcbdimension.m_bUseTTFont = FALSE;
	cEDA_pcbdimension.m_bBold = FALSE;
	cEDA_pcbdimension.m_bItalic = FALSE;
	cEDA_pcbdimension.m_csTTFontName = "Arial";
}

void CESDApp::OnFilePrintPageSetup()
{
	CESDView* pView = (CESDView*)GetCurrentView();
	if(pView == NULL)
	{
		AfxMessageBox("打印错误, 未找到活动的视窗。", MB_ICONSTOP);
		return;
	}
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(pView->m_nSystem_State == SCH)
	{
		HANDLE m_hDevMode;
		HANDLE m_hDevNames;
		PRINTDLG pd;
		DEVMODE   *pDevMode   =   NULL;

		CString csPrinterName;

		if(!AfxGetApp()->GetPrinterDeviceDefaults(&pd) )
		{
			AfxMessageBox("未找到打印机设备!", MB_ICONWARNING);
			return;
		}

		m_hDevNames = CopyHandle(pd.hDevNames);
		m_hDevMode = CopyHandle(pd.hDevMode);
		
		
		CDlg_Print_Setup_SCH dlg;
		dlg.m_fEditScale = fEDA_Print_Scale;
		dlg.m_nComboScaleMode = nEDA_Print_Mode;
		dlg.m_nRadioColor = nEDA_Print_Color;
		dlg.m_bCheckHCenter = bEDA_Print_HCenter;
		dlg.m_bCheckVCenter = bEDA_Print_VCenter;
		dlg.m_fEditHMargin = fEDA_Print_Margin_X;
		dlg.m_fEditVMargin = fEDA_Print_Margin_Y;
		dlg.m_nRadioExtent = nEDA_Print_Extent;
		dlg.m_nRadioSheet = nEDA_Print_SheetMode;

		
		pDevMode   =   (DEVMODE*)::GlobalLock(m_hDevMode);
		csPrinterName = pDevMode->dmDeviceName;
		::GlobalUnlock(m_hDevMode);

		char buf[300][64];
		int i = DeviceCapabilities(csPrinterName, NULL, DC_PAPERNAMES, (LPSTR)buf, NULL);
		for(int j=0; j<i; j++)	strcpy(dlg.m_szPaperName[j], buf[j]);

		WORD buf1[300];
		i = DeviceCapabilities(csPrinterName, NULL, DC_PAPERS, (LPSTR)buf1, NULL);
		for(int j=0; j<i; j++)	dlg.m_nPaper[j] = (int)buf1[j];

		dlg.m_nTotalPaper = i;

		pDevMode   =   (DEVMODE*)::GlobalLock(m_hDevMode);

		if(pDevMode->dmOrientation == DMORIENT_PORTRAIT) dlg.m_nRadioOrientation = 0;
		else dlg.m_nRadioOrientation = 1;

		dlg.m_nCurrentPaperSizeNo = pDevMode->dmPaperSize;
		::GlobalUnlock(m_hDevMode);
		
		int nRet = dlg.DoModal();
		
		pDevMode   =   (DEVMODE*)::GlobalLock(m_hDevMode);
		if(dlg.m_nRadioOrientation == 0) pDevMode->dmOrientation = DMORIENT_PORTRAIT;
		else pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
		pDevMode->dmPaperSize = dlg.m_nPaper[dlg.m_nComboPaperSize];
		::GlobalUnlock(m_hDevMode);
		
		SelectPrinter(m_hDevNames,m_hDevMode,FALSE);

		fEDA_Print_Scale = dlg.m_fEditScale;
		nEDA_Print_Mode	= dlg.m_nComboScaleMode;
		nEDA_Print_Color = dlg.m_nRadioColor;
		bEDA_Print_HCenter = dlg.m_bCheckHCenter;
		bEDA_Print_VCenter = dlg.m_bCheckVCenter;
		fEDA_Print_Margin_X = dlg.m_fEditHMargin;
		fEDA_Print_Margin_Y = dlg.m_fEditVMargin;
		nEDA_Print_Extent = dlg.m_nRadioExtent;
		nEDA_Print_SheetMode = dlg.m_nRadioSheet;

		if(nRet == IDOK)
		{
		}
		else if(nRet == MYID_PRINT) pView->FilePrint();
		else if(nRet == MYID_PRINT_SETUP) OnFilePrintSetup();
		else if(nRet == MYID_PRINT_PREVIEW) pView->OnFilePrintPreview();
	}
	else
	{
		HANDLE m_hDevMode;
		HANDLE m_hDevNames;
		PRINTDLG pd;
		DEVMODE   *pDevMode   =   NULL;
		CString csPrinterName;

		if(!AfxGetApp()->GetPrinterDeviceDefaults(&pd) )
		{
			AfxMessageBox("未找到打印机设备!", MB_ICONWARNING);
			return;
		}
		m_hDevNames = CopyHandle(pd.hDevNames);
		m_hDevMode = CopyHandle(pd.hDevMode);
		
		CDlg_Print_Setup_PCB dlg;
		dlg.m_fEditScale = fEDA_PCBPrint_Scale;
		dlg.m_nComboScaleMode = nEDA_PCBPrint_Mode;
		dlg.m_nRadioColor = nEDA_PCBPrint_Color;
		dlg.m_bCheckHCenter = bEDA_PCBPrint_HCenter;
		dlg.m_bCheckVCenter = bEDA_PCBPrint_VCenter;
		dlg.m_fEditHMargin = fEDA_PCBPrint_Margin_X;
		dlg.m_fEditVMargin = fEDA_PCBPrint_Margin_Y;
		dlg.m_nRadioExtent = nEDA_PCBPrint_Extent;
		dlg.m_csEditX1 = pDoc->Convert_XY_To_String(pDoc->m_fPCBPrintX1 - pDoc->m_cPCB_Sheet.m_fOriginX);
		dlg.m_csEditY1 = pDoc->Convert_XY_To_String(pDoc->m_fPCBPrintY1 - pDoc->m_cPCB_Sheet.m_fOriginY);
		dlg.m_csEditX2 = pDoc->Convert_XY_To_String(pDoc->m_fPCBPrintX2 - pDoc->m_cPCB_Sheet.m_fOriginX);
		dlg.m_csEditY2 = pDoc->Convert_XY_To_String(pDoc->m_fPCBPrintY2 - pDoc->m_cPCB_Sheet.m_fOriginY);

		pDevMode   =   (DEVMODE*)::GlobalLock(m_hDevMode);

		csPrinterName = pDevMode->dmDeviceName;
		::GlobalUnlock(m_hDevMode);

		char buf[300][64];
		int i = DeviceCapabilities(csPrinterName, NULL, DC_PAPERNAMES, (LPSTR)buf, NULL);
		for(int j=0; j<i; j++)	strcpy(dlg.m_szPaperName[j], buf[j]);

		WORD buf1[300];
		i = DeviceCapabilities(csPrinterName, NULL, DC_PAPERS, (LPSTR)buf1, NULL);
		for(int j=0; j<i; j++)	dlg.m_nPaper[j] = (int)buf1[j];

		dlg.m_nTotalPaper = i;

		pDevMode   =   (DEVMODE*)::GlobalLock(m_hDevMode);
		if(pDevMode->dmOrientation == DMORIENT_PORTRAIT) dlg.m_nRadioOrientation = 0;
		else dlg.m_nRadioOrientation = 1;

		dlg.m_nCurrentPaperSizeNo = pDevMode->dmPaperSize;
		::GlobalUnlock(m_hDevMode);
		
		int nRet = dlg.DoModal();
		
		pDevMode   =   (DEVMODE*)::GlobalLock(m_hDevMode);

		if(dlg.m_nRadioOrientation == 0) pDevMode->dmOrientation = DMORIENT_PORTRAIT;
		else pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
		pDevMode->dmPaperSize = dlg.m_nPaper[dlg.m_nComboPaperSize];
		::GlobalUnlock(m_hDevMode);
		
		SelectPrinter(m_hDevNames,m_hDevMode,FALSE);
		
		fEDA_PCBPrint_Scale = dlg.m_fEditScale;
		nEDA_PCBPrint_Mode = dlg.m_nComboScaleMode;
		nEDA_PCBPrint_Color = dlg.m_nRadioColor;
		bEDA_PCBPrint_HCenter = dlg.m_bCheckHCenter;
		bEDA_PCBPrint_VCenter = dlg.m_bCheckVCenter;
		fEDA_PCBPrint_Margin_X = dlg.m_fEditHMargin;
		fEDA_PCBPrint_Margin_Y = dlg.m_fEditVMargin;
		nEDA_PCBPrint_Extent = dlg.m_nRadioExtent;
		pDoc->m_fPCBPrintX1 = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditX1) + pDoc->m_cPCB_Sheet.m_fOriginX;
		pDoc->m_fPCBPrintY1 = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditY1) + pDoc->m_cPCB_Sheet.m_fOriginY;
		pDoc->m_fPCBPrintX2 = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditX2) + pDoc->m_cPCB_Sheet.m_fOriginX;
		pDoc->m_fPCBPrintY2 = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditY2) + pDoc->m_cPCB_Sheet.m_fOriginY;

		if(nRet == IDOK)
		{
		}
		else if(nRet == MYID_PRINT) pView->FilePrint();
		else if(nRet == MYID_PRINT_SETUP) OnFilePrintSetup();
		else if(nRet == MYID_PRINT_PREVIEW) pView->OnFilePrintPreview();
		else if(nRet == MYID_PRINT_DEFINE)
		{
			pView->DefinePrintArea();
			OnFilePrintPageSetup();
		}	
	}
}

HANDLE CESDApp::CopyHandle(HANDLE h)
{
	if (!h) return NULL;
	DWORD   dwLen = GlobalSize(h);
	HANDLE hCopy = GlobalAlloc(GHND, dwLen);
	if(hCopy) {
		BYTE* lpCopy = (BYTE*)GlobalLock(hCopy);
		BYTE* lp     = (BYTE*)GlobalLock(h);
		CopyMemory(lpCopy,lp,dwLen);
		GlobalUnlock(hCopy);
		GlobalUnlock(h);
	}
	return hCopy;
}

int CESDApp::GetPrintInfo(int mode)
{
	int i;
	CDC dc;

	UpdatePrinterSelection(FALSE);
	SelectPrinter(m_hDevNames,m_hDevMode,TRUE);
	if(CreatePrinterDC(dc) == FALSE) 
	{
		AfxMessageBox("未找到打印设备。", MB_OK|MB_ICONSTOP);
		return 0;
	}
	switch(mode)
	{
	case 0:
		i=0;
		break;
	case 1:
		i = dc.GetDeviceCaps(LOGPIXELSX);	
		break;
	case 2:
		i = dc.GetDeviceCaps(LOGPIXELSY);	
		break;
	case 3:
		i = dc.GetDeviceCaps(HORZRES);	
		break;
	case 4:
		i = dc.GetDeviceCaps(VERTRES);	
		break;
	case 5:
		i = dc.GetDeviceCaps(HORZRES);	
		break;
	case 6:
		i = dc.GetDeviceCaps(VERTRES);	
		break;
	case 7:
		i = dc.GetDeviceCaps(RASTERCAPS);
		break;
	default:
		break;
	}
	return i;	
}

void CESDApp::LoadSystemParam()
{
	char filestring[MAX_PATH];
	sprintf(filestring,"%s\\ESD_V4.ini",csEDA_System_Path);
	if(cfileEDA_System.Open(filestring, CFile::modeRead) == TRUE)
	{
		CArchive ar(&cfileEDA_System, CArchive::load);

		ar >> nEDA_Cursor_Mode;

		ar >> nEDA_PartID_Suffix;

		ar >> nEDA_SCH_VisibleGridMode;
		ar >> nEDA_SCH_VisibleGridColor;
		ar >> bEDA_SCH_VisibleGridShow;
		ar >> fEDA_SCH_VisibleGrid;
		ar >> bEDA_SCH_SnapGridEnable;
		for(int i=0;i<3;i++)
		{
			ar >> fEDA_SCH_SnapGrid_I[i];
			ar >> fEDA_SCH_SnapGrid_M[i];
		}
		ar >> bEDA_SCH_ElecGridEnable;
		ar >> fEDA_SCH_ElecGrid;

		ar >> bEDA_Sheet_AutoOn;

		ar >> bEDA_AutoJunction;
		ar >> bEDA_AutoOptimizeWire;
	
		ar >> csEDA_PowerGround;
		ar >> csEDA_SignalGround;
		ar >> csEDA_EarthGround;

		ar >> nEDA_SheetStandardStyle;
		
		ar >> nEDA_Backup_Time;

		ar >> csEDA_LastFile;

		ar >> nEDA_FindScope;
		ar >> nEDA_FindObject;
		ar >> bEDA_FindCase;
		ar >> bEDA_FindWord;
		ar >> bEDA_FindJump;
		ar >> bEDA_FindPrompt;

		int num;
		ar >> num;
		for(int i=0; i<num; i++)
		{
			Struct_CompList complist;
			ar >> complist.basiclibrary;
			CString str;
			ar >> str;
			if(str.GetLength() < 100) strcpy(complist.compname, str);
			ar >> complist.length;
			ar >> complist.liboffset;
			ar >> complist.offset;
			arrayEDA_SchCompHistory.Add(complist);
		}
		ar >> csEDA_SchCompHistory;

		ar >> bEDA_PcbOnLineDRCEnable;
		ar >> bEDA_PcbConnectionAutoRefresh;
		ar >> bEDA_PcbPolygonHighlight;

		ar >> nEDA_SchRepeatNumber;
		ar >> nEDA_SchRepeatNumberDelta;
		ar >> fEDA_SchRepeatDeltaX;
		ar >> fEDA_SchRepeatDeltaY;

		ar >> fEDA_AutoArcMaxRadius;
		ar >> nEDA_PCB_Undo_Number;
		ar >> fEDA_LoadPackage_X;
		ar >> fEDA_LoadPackage_Y;

		ar >> bEDA_ConvertChineseLayerName;
		ar >> bEDA_ConvertEnglishLayerName;

		ar >> num;
		for(int i=0; i<num; i++)
		{
			Struct_CompList complist;
			ar >> complist.basiclibrary;
			CString str;
			ar >> str;
			if(str.GetLength() < 100) strcpy(complist.compname, str);
			ar >> complist.length;
			ar >> complist.liboffset;
			ar >> complist.offset;
			arrayEDA_PcbCompHistory.Add(complist);
		}
		ar >> csEDA_PcbCompHistory;

		ar.Flush();
		ar.Close();
		cfileEDA_System.Close();
	}
}

void CESDApp::SaveSystemParam()
{
	char filestring[MAX_PATH];
	sprintf(filestring,"%s\\ESD_V4.ini",csEDA_System_Path);
	if(cfileEDA_System.Open(filestring,  CFile::modeCreate|CFile::modeWrite) == TRUE)
	{
		CArchive ar(&cfileEDA_System, CArchive::store);

		ar << nEDA_Cursor_Mode;
		ar << nEDA_PartID_Suffix;
		
		ar << nEDA_SCH_VisibleGridMode;
		ar << nEDA_SCH_VisibleGridColor;
		ar << bEDA_SCH_VisibleGridShow;
		ar << fEDA_SCH_VisibleGrid;
		ar << bEDA_SCH_SnapGridEnable;
		for(int i=0;i<3;i++)
		{
			ar << fEDA_SCH_SnapGrid_I[i];
			ar << fEDA_SCH_SnapGrid_M[i];
		}
		ar << bEDA_SCH_ElecGridEnable;
		ar << fEDA_SCH_ElecGrid;
		
		ar << bEDA_Sheet_AutoOn;
		
		ar << bEDA_AutoJunction;
		ar << bEDA_AutoOptimizeWire;
		
		ar << csEDA_PowerGround;
		ar << csEDA_SignalGround;
		ar << csEDA_EarthGround;
		
		ar << nEDA_SheetStandardStyle;		
 
		ar << nEDA_Backup_Time;
		
		ar << csEDA_LastFile;
		
		ar << nEDA_FindScope;
		ar << nEDA_FindObject;
		ar << bEDA_FindCase;
		ar << bEDA_FindWord;
		ar << bEDA_FindJump;
		ar << bEDA_FindPrompt;
		
		int num = arrayEDA_SchCompHistory.GetCount();
		for(int i=0; i<num; i++)
		{
			Struct_CompList complist;
			complist = arrayEDA_SchCompHistory.GetAt(i);
			if(complist.basiclibrary != TRUE)
			{
				arrayEDA_SchCompHistory.RemoveAt(i);
				i--;
				num--;
			}
		}
		num = arrayEDA_SchCompHistory.GetCount();
		ar << num;
		for(int i=0; i<num; i++)
		{
			Struct_CompList complist;
			complist = arrayEDA_SchCompHistory.GetAt(i);
			ar << complist.basiclibrary;
			CString str = complist.compname;
			ar << str;
			ar << complist.length;
			ar << complist.liboffset;
			ar << complist.offset;
		}
		ar << csEDA_SchCompHistory;

		ar << bEDA_PcbOnLineDRCEnable;
		ar << bEDA_PcbConnectionAutoRefresh;
		ar << bEDA_PcbPolygonHighlight;

		ar << nEDA_SchRepeatNumber;
		ar << nEDA_SchRepeatNumberDelta;
		ar << fEDA_SchRepeatDeltaX;
		ar << fEDA_SchRepeatDeltaY;

		ar << fEDA_AutoArcMaxRadius;
		ar << nEDA_PCB_Undo_Number;
		ar << fEDA_LoadPackage_X;
		ar << fEDA_LoadPackage_Y;

		ar << bEDA_ConvertChineseLayerName;
		ar << bEDA_ConvertEnglishLayerName;

		num = arrayEDA_PcbCompHistory.GetCount();
		for(int i=0; i<num; i++)
		{
			Struct_CompList complist;
			complist = arrayEDA_PcbCompHistory.GetAt(i);
			if(complist.basiclibrary != TRUE)
			{
				arrayEDA_PcbCompHistory.RemoveAt(i);
				i--;
				num--;
			}
		}
		num = arrayEDA_PcbCompHistory.GetCount();
		ar << num;
		for(int i=0; i<num; i++)
		{
			Struct_CompList complist;
			complist = arrayEDA_PcbCompHistory.GetAt(i);
			ar << complist.basiclibrary;
			CString str = complist.compname;
			ar << str;
			ar << complist.length;
			ar << complist.liboffset;
			ar << complist.offset;
		}
		ar << csEDA_PcbCompHistory;

		ar.Flush();
		ar.Close();
		cfileEDA_System.Close();
	}
}

void CESDApp::FindTipsFile(LPCTSTR lpszTipsFile)
{
	if (!FILEEXISTS_S(lpszTipsFile))
	{
		TRY
		{
			CStdioFile file(lpszTipsFile,
				CFile::modeCreate | CFile::modeWrite | CFile::typeText);

			file.WriteString(_T("Technical support is just a phone call away, feel free to contact us at anytime with your support or sales questions, we are always here to help! (517) 625-5729\n"));
			file.WriteString(_T("You can easily change the appearance of your application to look like Office XP with a single line of code.  Choose \"View\" then \"Office XP Theme\" to see this in action!\n"));
			file.WriteString(_T("You can save thousands of hours in development time by using the many excellent controls found in the Xtreme Toolkit!\n"));

			file.Close();
		}
		CATCH( CFileException, e )
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		}
		END_CATCH
	}
}

void CESDApp::OnTipoftheday()
{
	CString strTipsFile;
	strTipsFile.Format("%s\\tips.txt",csEDA_System_Path);

	FindTipsFile(strTipsFile);

	CXTTipOfTheDay dlg(strTipsFile);
	dlg.DoModal();
}

void CESDApp::OnHelpTip()
{
	OnTipoftheday();
}

BOOL CALLBACK MyEnumFontFamProc(LPLOGFONT lplf,LPNEWTEXTMETRIC lpntm ,DWORD FontType,LPARAM aFontCount)
{
	CString cstring;
	cstring = lplf->lfFaceName;
	arrayEDA_SystemFont.Add(cstring);

	return TRUE;
}
void CESDApp::OnDownloadOnline()
{
    TCHAR key[MAX_PATH + MAX_PATH];	

    // First try ShellExecute()
    HINSTANCE result = ShellExecute(NULL, _T("open"), "https://github.com/richardiffusion", NULL,NULL, SW_SHOW);


    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR) 
	{		
        if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) 
		{
            lstrcat(key, _T("\\shell\\open\\command"));
            if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) 
			{
                TCHAR *pos;
                pos = _tcsstr(key, _T("\"%1\""));
                if (pos == NULL) {                     // No quotes found
                    pos = strstr(key, _T("%1"));       // Check for %1, without quotes
                    if (pos == NULL)                   // No parameter at all...
                        pos = key+lstrlen(key)-1;
                    else
                        *pos = '\0';                   // Remove the parameter
                }
                else
                    *pos = '\0';                       // Remove the parameter

                lstrcat(pos, _T(" "));
                lstrcat(pos, "www.eda.com.cn");
                result = (HINSTANCE) WinExec(key, SW_SHOW);
            }
        }
	}
}

int CESDApp::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);
    if (retval == ERROR_SUCCESS) {
        long datasize = MAX_PATH;
		TCHAR data[MAX_PATH];
		RegQueryValue(hkey, NULL, data, &datasize);
		lstrcpy(retdata,data);
		RegCloseKey(hkey);
    }
	return retval;
}

BOOL CESDApp::IsAD6SchBinary(CFile& myfile, CString& onelinestr)
{
	if( ((onelinestr[0]&0x00ff) == 0xd0)&&((onelinestr[1]&0x00ff) == 0xcf) )
	{
		myfile.Seek(0x1800, CFile::begin);
		int length;
		myfile.Read(&length, 4);

		char str[1000];
		if(length < 999)
		{
			myfile.Read(str, length);
			str[length] = '\0';
		}
		onelinestr = str;
		if(onelinestr.Find("|HEADER=Protel for Windows - Schematic Capture Binary File Version 5.0") >= 0) return TRUE;
	}

	return FALSE;
}



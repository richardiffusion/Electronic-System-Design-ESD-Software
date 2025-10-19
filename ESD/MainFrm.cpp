// MainFrm.cpp : implementation of the CMainFrame class
//
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESD.h"

#include "MainFrm.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "ChildFrm.h"
#include "DynamicHelpView.h"
#include ".\mainfrm.h"
#include "systemext.h"
#include "sysparam.h"

#include "Dlg_MyHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_WINDOW_AUTOHIDEALL, OnWindowAutohideall)
	ON_COMMAND(ID_WINDOW_MORE, OnWindowMore)
	

	//}}AFX_MSG_MAP
	//ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)

	//ON_COMMAND(ID_VIEW_FULLSCREEN, OnFullScreen)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, OnUpdateFullScreen)
	ON_COMMAND(ID_SCH_VIEW_FULLSCREEN, OnSchFullScreen)
	ON_UPDATE_COMMAND_UI(ID_SCH_VIEW_FULLSCREEN, OnUpdateSchFullScreen)
	ON_COMMAND(ID_PCB_VIEW_FULLSCREEN, OnPcbFullScreen)
	ON_UPDATE_COMMAND_UI(ID_PCB_VIEW_FULLSCREEN, OnUpdatePcbFullScreen)

	ON_COMMAND_RANGE(ID_VIEW_SOLUTIONEXPLORER, ID_HELP_DYNAMICHELP, OnShowView)
	ON_COMMAND_RANGE(ID_FILE_NEW_PROJECT + 1, 200, OnEmptyCommandRange)

	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_XTP_CREATECONTROL()

	ON_COMMAND_RANGE(ID_STATUS_SNAP1, ID_STATUS_SNAP13, OnStatusSnap)
	ON_COMMAND_RANGE(ID_STATUS_VISIBLE1, ID_STATUS_VISIBLE13, OnStatusVisible)
	//ON_COMMAND_RANGE(ID_THEME_VC6, ID_THEME_VC8, OnTheme)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_THEME_VC6, ID_THEME_VC8, OnUpdateTheme)

#ifdef _XTP_INCLUDE_SKINFRAMEWORK
	ON_COMMAND_RANGE(ID_SKINFRAMEWORK_VISTABLUE, ID_SKINFRAMEWORK_SYSTEM, OnSkinFramework)
	ON_COMMAND_RANGE(ID_THEME_SKINFRAMEWORK, ID_THEME_SKINFRAMEWORK, OnEmptyCommandRange)
#endif

	ON_XTP_EXECUTE(ID_EDIT_STATE, OnEditState)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STATE, OnUpdateEditState)

	ON_XTP_EXECUTE(ID_EDIT_PLATFORM, OnEditPlatform)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PLATFORM, OnUpdateEditPlatform)
	ON_WM_ACTIVATE()
	
	ON_COMMAND(ID_TO_PCB, OnToPcb)
	ON_COMMAND(ID_TO_SCH, OnToSch)
	ON_COMMAND(ID_DOC_TAB, OnDocTab)
	ON_UPDATE_COMMAND_UI(ID_DOC_TAB, OnUpdateDocTab)
	ON_COMMAND(ID_SHEET_TAB, OnSheetTab)
	ON_UPDATE_COMMAND_UI(ID_SHEET_TAB, OnUpdateSheetTab)	

	ON_WM_TIMER()
END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator

	IDS_INDICATOR_INFO,
	IDS_INDICATOR_XY,
	IDS_INDICATOR_UNIT,
	IDS_INDICATOR_CURSOR,
	IDS_INDICATOR_SNAP,
	IDS_INDICATOR_VISIBLE,
	IDS_INDICATOR_ELECTRICAL,

	//ID_INDICATOR_REC,
	//ID_INDICATOR_EXT,
	//ID_INDICATOR_OVR,
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};


static TCHAR G_szSection[] = _T("Settings");
static TCHAR G_szWindowPos[] = _T("WindowPos");
static TCHAR G_szFormat[] = _T("%u,%u,%d,%d,%d,%d,%d,%d,%d,%d");

static BOOL PASCAL NEAR ReadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = AfxGetApp()->GetProfileString(G_szSection,G_szWindowPos);
	if(strBuffer.IsEmpty()) return FALSE;

	WINDOWPLACEMENT wp;
	int nRead = _stscanf(strBuffer,G_szFormat,
		&wp.flags,&wp.showCmd,
		&wp.ptMinPosition.x, &wp.ptMinPosition.y,
		&wp.ptMaxPosition.x, &wp.ptMaxPosition.y,
		&wp.rcNormalPosition.left, &wp.rcNormalPosition.top,
		&wp.rcNormalPosition.right, &wp.rcNormalPosition.bottom);

	if(nRead!=10) return FALSE;
	
	wp.length = sizeof wp;
	*pwp = wp;
	return TRUE;
}

static void PASCAL NEAR WriteWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	TCHAR szBuffer[sizeof("-32767")*8+sizeof("65535")*2];

	wsprintf(szBuffer,G_szFormat,
		pwp->flags, pwp->showCmd,
		pwp->ptMinPosition.x, pwp->ptMinPosition.y,
		pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
		pwp->rcNormalPosition.left, pwp->rcNormalPosition.top,
		pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);
	AfxGetApp()->WriteProfileString(G_szSection,G_szWindowPos,szBuffer);
}

void CMainFrame::InitialShowWindow(UINT nCmdShow)
{
	WINDOWPLACEMENT wp;
	if(!ReadWindowPlacement(&wp))
	{
		ShowWindow(nCmdShow);
		return;
	}
	if(nCmdShow != SW_SHOWNORMAL) wp.showCmd = nCmdShow;
	SetWindowPlacement(&wp);
	ShowWindow(wp.showCmd);
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_nState = 1;
	m_nPlatform = 0;
	m_bSchFullScreen = FALSE;
	m_bPcbFullScreen = FALSE;

	// get path of executable
	TCHAR szBuff_SCH[_MAX_PATH];
	TCHAR szBuff_PCB[_MAX_PATH];
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuff_SCH, _MAX_PATH));
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuff_PCB, _MAX_PATH));

	LPTSTR lpszExt = _tcsrchr(szBuff_SCH, '.');
	lstrcpy(lpszExt, _T("SCH.ini"));
	m_strIniFileName_SCH = szBuff_SCH;

	lpszExt = _tcsrchr(szBuff_PCB, '.');
	lstrcpy(lpszExt, _T("PCB.ini"));
	m_strIniFileName_PCB = szBuff_PCB;


	m_pSchFullScreenLayout = NULL;
	m_pPcbFullScreenLayout = NULL;
	m_rcMainFrame.SetRectEmpty();

	m_nTheme = ID_THEME_VC8;

#ifdef _XTP_INCLUDE_SKINFRAMEWORK
	XTPSkinManager()->GetResourceFile()->SetModuleHandle(AfxGetInstanceHandle());
#endif
}

CMainFrame::~CMainFrame()
{
	if (m_pSchFullScreenLayout)
	{
		delete m_pSchFullScreenLayout;
	}
	if (m_pPcbFullScreenLayout)
	{
		delete m_pPcbFullScreenLayout;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	int nIndex = m_wndStatusBar.CommandToIndex(IDS_INDICATOR_INFO);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex)  |  SBPS_NOBORDERS );     //SBPS_POPOUT|SBPS_NOBORDERS|SBPS_STRETCH
	m_wndStatusBar.SetPaneWidth(nIndex, 300);
	nIndex = m_wndStatusBar.CommandToIndex(IDS_INDICATOR_XY);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex)  |  SBPS_NOBORDERS );     //SBPS_POPOUT|SBPS_NOBORDERS|SBPS_STRETCH
	m_wndStatusBar.SetPaneWidth(nIndex, 180);
	
	m_wndStatusBar.SetRibbonDividerIndex(m_wndStatusBar.GetPaneCount() - 1);
	AddZoomButton();
	AddZoomSlider();
	m_wndStatusBar.EnableCustomization();


	if (!InitCommandBars())
		return -1;


	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	pSCHMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_SCH_MAINMENU); 
	pSCHMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);

	
	pSCHMainToolBar = (CXTPToolBar*)pCommandBars->Add(_T("标准"), xtpBarTop);
	if (!pSCHMainToolBar ||
		!pSCHMainToolBar->LoadToolBar(IDR_MAINTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	pSCHMainToolBar->ShowExpandButton(FALSE);

	pSCHPlaceToolBar1 = (CXTPToolBar*)pCommandBars->Add(_T("放置"), xtpBarTop);
	if (!pSCHPlaceToolBar1 ||
		!pSCHPlaceToolBar1->LoadToolBar(IDR_PLACETOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	pSCHPlaceToolBar1->ShowExpandButton(FALSE);
	DockRightOf(pSCHPlaceToolBar1 ,pSCHMainToolBar);

	pSCHPlaceToolBar2 = (CXTPToolBar*)pCommandBars->Add(_T("绘图"), xtpBarTop);
	if (!pSCHPlaceToolBar2 ||
		!pSCHPlaceToolBar2->LoadToolBar(IDR_PLACETOOLBAR2))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	pSCHPlaceToolBar2->ShowExpandButton(FALSE);
	DockRightOf(pSCHPlaceToolBar2, pSCHPlaceToolBar1);

	pSCHOtherToolBar1 = (CXTPToolBar*)pCommandBars->Add(_T("辅助"), xtpBarTop);
	if (!pSCHOtherToolBar1 ||
		!pSCHOtherToolBar1->LoadToolBar(IDR_OTHERTOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	pSCHOtherToolBar1->ShowExpandButton(FALSE);

	
	pSCHOtherToolBar2 = (CXTPToolBar*)pCommandBars->Add(_T("未开放工具"), xtpBarTop);
	if (!pSCHOtherToolBar2 ||
		!pSCHOtherToolBar2->LoadToolBar(IDR_OTHERTOOLBAR2))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	DockRightOf(pSCHOtherToolBar2, pSCHOtherToolBar1);
	pSCHOtherToolBar2->ShowExpandButton(FALSE);
	pSCHOtherToolBar2->SetVisible(FALSE);


	XTPPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE;
	XTPImageManager()->SetIcons(IDR_TOOLBAR_EXT);


	pCommandBars->GetCommandBarsOptions()->ShowKeyboardCues(xtpKeyboardCuesShowWindowsDefault);
	
	
	pCommandBars->GetToolTipContext()->SetStyle(xtpToolTipOffice);
	
	

	CXTPShortcutManager* pXTPShortcutManager;
	pXTPShortcutManager = pCommandBars->GetShortcutManager();
	pXTPShortcutManager->SetKeyNameText(VK_PRIOR, _T("PgUp") );	
	pXTPShortcutManager->SetKeyNameText(VK_NEXT, _T("PgDn") );	
	pXTPShortcutManager->SetAccelerators(IDR_MAINFRAME);


	
	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeVisualStudio2005);


	
	CXTPPropExchangeXMLNode px(TRUE, 0, _T("Settings"));   

	if (px.LoadFromFile(m_strIniFileName_SCH))
	{
		CXTPPropExchangeSection pxNormalLayout(px.GetSection(_T("NormalLayout")));
		ExchangeLayout(&pxNormalLayout);

		m_pSchFullScreenLayout = DYNAMIC_DOWNCAST(CXTPPropExchangeXMLNode, px.GetSection(_T("FullScreenLayout")));
		ASSERT(m_pSchFullScreenLayout);
	}

	CXTPImageManager* pImageManager = pCommandBars->GetImageManager();

	pImageManager->InternalAddRef();
	m_paneManager.SetImageManager(pImageManager);

	m_paneManager.SetAlphaDockingContext(TRUE);
	m_paneManager.SetShowDockingContextStickers(TRUE);
	m_paneManager.SetShowContentsWhileDragging(TRUE);
	m_paneManager.SetDefaultPaneOptions(xtpPaneHasMenuButton);

	VERIFY(m_MTIClientWnd.Attach(this, TRUE));
	
	m_MTIClientWnd.EnableToolTips();

	m_MTIClientWnd.GetPaintManager()->m_bShowIcons = FALSE;
	m_MTIClientWnd.SetNewTabPosition(xtpWorkspaceNewTabLeftMost);
	m_MTIClientWnd.SetFlags(xtpWorkspaceHideArrowsAlways | xtpWorkspaceShowActiveFiles | xtpWorkspaceShowCloseSelectedTab);
	m_MTIClientWnd.SetAutoTheme();


	nEDA_System_State = SCH;

	Set_Cursor();

	if(bEDA_Backup == TRUE)
	{
		UINT u = nEDA_Backup_Time*60000;
		if(u < 60000) u = 60000;
		SetTimer(ID_AUTO_BACKUP_TIMER,u,NULL);
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.lpszClass = _T("XTPMainFrame");
	CXTPDrawHelpers::RegisterWndClass(AfxGetInstanceHandle(), cs.lpszClass, 
		CS_DBLCLKS, AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::ShowDockingPanePopupMenu(CXTPDockingPane* pPopupPane, CPoint pt)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_PANES));
	CMenu* pPopup = menu.GetSubMenu(0);
	
	
	if (pPopupPane->IsHidden())
	{
		pPopup->CheckMenuItem(ID_POPUP_AUTOHIDE, MF_BYCOMMAND|MF_CHECKED);
		pPopup->EnableMenuItem(ID_POPUP_FLOATING, MF_BYCOMMAND|MF_DISABLED);
		pPopup->EnableMenuItem(ID_POPUP_DOCKABLE, MF_BYCOMMAND|MF_DISABLED);
	}
	else if (pPopupPane->GetOptions() & xtpPaneNoDockable)
	{
		pPopup->CheckMenuItem(ID_POPUP_FLOATING, MF_BYCOMMAND|MF_CHECKED);
	}
	else
	{
		pPopup->CheckMenuItem(ID_POPUP_DOCKABLE, MF_BYCOMMAND|MF_CHECKED);
	}
	
	int nCommand = GetCommandBars()->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD, pt.x, pt.y);
	
	switch (nCommand)
	{
	case ID_POPUP_HIDE:
		pPopupPane->Close();
		break;		
		
	case ID_POPUP_AUTOHIDE:
		m_paneManager.ToggleAutoHide(pPopupPane);
		break;

	case ID_POPUP_DOCKABLE:
		if (pPopupPane->GetOptions() & xtpPaneNoDockable)
		{
			pPopupPane->SetOptions(pPopupPane->GetOptions() & ~xtpPaneNoDockable);
		}
		break;

	case ID_POPUP_FLOATING:
		if ((pPopupPane->GetOptions() & xtpPaneNoDockable) == 0)
		{
			if (!pPopupPane->IsFloating())
				m_paneManager.ToggleDocking(pPopupPane);
			
			pPopupPane->SetOptions(pPopupPane->GetOptions() | xtpPaneNoDockable);
		}

		break;
	}
}
	
void CMainFrame::OnShowView(UINT nID)
{
	CDlg_MyHelp dlg;
	dlg.DoModal();
	//m_paneManager.ShowPane(nID);
}

void CMainFrame::OnEmptyCommandRange(UINT)
{

}

void CMainFrame::OnWindowAutohideall()
{
	CXTPDockingPaneInfoList& lstPanes = m_paneManager.GetPaneList();
	POSITION pos = lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = lstPanes.GetNext(pos);
		if (!pPane->IsClosed() && !pPane->IsHidden())
			m_paneManager.HidePane(pPane);
	}
}

void CMainFrame::OnWindowMore()
{
	CXTWindowList dlg(this);
	dlg.DoModal();
}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if (!pToolBar)
			return FALSE;
		
		if (lpCreateControl->nID == ID_FILE_PRINT && pToolBar->GetBarID() == IDR_MAINTOOLBAR)
		{

			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
		else if (lpCreateControl->nID == ID_FILE_PRINT && pToolBar->GetBarID() == IDR_PCB_MAINTOOLBAR)
		{
			
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
		else if (lpCreateControl->nID == ID_PLACE_ARCCENTER && pToolBar->GetBarID() == IDR_PLACETOOLBAR2)
		{
			
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
		else if (lpCreateControl->nID == ID_PLACE_EARC_CENTER && pToolBar->GetBarID() == IDR_PLACETOOLBAR2)
		{
			
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
		else if (lpCreateControl->nID == ID_GRID_CYCLE_SNAP && pToolBar->GetBarID() == IDR_OTHERTOOLBAR1)
		{
			
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
		else if (lpCreateControl->nID == ID_HIGHLIGHT1 && pToolBar->GetBarID() == IDR_OTHERTOOLBAR1)
		{
			
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
		else if (lpCreateControl->nID == ID_PCB_PLACE_ARC_EDGE && pToolBar->GetBarID() == IDR_PCB_PLACETOOLBAR)
		{
			
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
		else if (lpCreateControl->nID == ID_PCB_TOOL_RATS_SHOW_ALL && pToolBar->GetBarID() == IDR_PCB_DESIGNTOOLBAR)
		{
			
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
		else if (lpCreateControl->nID == ID_PCB_TOOL_HIGHLIGHT_LOCATION && pToolBar->GetBarID() == IDR_PCB_DESIGNTOOLBAR)
		{
			
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
		else if (lpCreateControl->nID == ID_VIEW_GRIDS_TOGGLE_VISIBLE && pToolBar->GetBarID() == IDR_PCB_OTHERTOOLBAR1)
		{
			
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}

	}
	return FALSE;

}

void CMainFrame::OnCustomize()
{
	CXTPCustomizeSheet cs(GetCommandBars());

	CXTPCustomizeKeyboardPage pageKeyboard(&cs);
	cs.AddPage(&pageKeyboard);
	pageKeyboard.AddCategories(IDR_MAINFRAME, TRUE);

	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);

	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_MAINFRAME, TRUE);

	pCommands->InsertAllCommandsCategory();
	pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
	pCommands->InsertNewMenuCategory();

	cs.DoModal();
}

void CMainFrame::OnEditState(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
	{
		int nState = pControl->GetCurSel();
		if (nState != 2)
		{
			m_nState = nState;
		}
		else 
		{
			MessageBox(_T("Configuration Manager"));
			pControl->SetCurSel(m_nState);
		}
		*pResult = TRUE; // Handled
	}
}

void CMainFrame::OnUpdateEditState(CCmdUI* pCmdUI)
{
	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if (pToolBar && !XTPMouseManager()->IsTrackedLock())
	{
		CXTPControlComboBox* pStateCombo = (CXTPControlComboBox*)pToolBar->GetControls()->GetAt(pCmdUI->m_nIndex);

		if (pStateCombo->GetType() == xtpControlComboBox)
		{
			pStateCombo->SetCurSel(m_nState);
		}
	}

	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnEditPlatform(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
	{
		int nState = pControl->GetCurSel();
		if (nState != 1)
		{
			m_nPlatform = nState;
		}
		else
		{
			MessageBox(_T("Configuration Manager"));
			pControl->SetCurSel(m_nPlatform);
		}
		*pResult = TRUE; // Handled
	}
}

void CMainFrame::OnUpdateEditPlatform(CCmdUI* pCmdUI)
{
	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if (pToolBar && !XTPMouseManager()->IsTrackedLock())
	{
		CXTPControlComboBox* pStateCombo = (CXTPControlComboBox*)pToolBar->GetControls()->GetAt(pCmdUI->m_nIndex);

		if (pStateCombo->GetType() == xtpControlComboBox)
		{
			pStateCombo->SetCurSel(m_nPlatform);
		}
	}

	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnClose()
{
	if(nEDA_System_State == SCH)
	{
		CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));      // To serialize to XML file

		if (px.OnBeforeExchange()) 
		{
			// Reset Full Screen Mode
			if (m_bSchFullScreen)
			{
				OnSchFullScreen();
			}

			CXTPPropExchangeSection pxNormalLayout(px.GetSection(_T("NormalLayout")));
			ExchangeLayout(&pxNormalLayout);
		
			px.PutSection(m_pSchFullScreenLayout);

			px.SaveToFile(m_strIniFileName_SCH);
		}
	}
	else
	{
		CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));      // To serialize to XML file

		if (px.OnBeforeExchange()) 
		{
			// Reset Full Screen Mode
			if (m_bPcbFullScreen)
			{
				OnPcbFullScreen();
			}

			CXTPPropExchangeSection pxNormalLayout(px.GetSection(_T("NormalLayout")));
			ExchangeLayout(&pxNormalLayout);
		
			px.PutSection(m_pPcbFullScreenLayout);

			px.SaveToFile(m_strIniFileName_PCB);
		}
	}
	
	CMDIFrameWnd::OnClose();
}


void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{

// Toggle CommandBars
	GetCommandBars()->OnSetPreviewMode(bPreview);
// Toggle Tab Client
	m_MTIClientWnd.ShowWorkspace(!bPreview);

// Toggle Docking Panes.
	m_paneManager.OnSetPreviewMode(bPreview);


	CMDIFrameWnd::OnSetPreviewMode(bPreview, pState);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	static HWND hFocus = 0;
	if (nState == WA_INACTIVE)
	{
		hFocus = ::GetFocus();
	}
	
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	if (nState != WA_INACTIVE && hFocus)
	{
		::SetFocus(hFocus);		
		
	}
}

void CMainFrame::ExchangeLayout(CXTPPropExchange* pPX, BOOL bSerializeControls /*= TRUE*/)
{
	XTP_COMMANDBARS_PROPEXCHANGE_PARAM param; param.bSerializeControls = bSerializeControls; 

	CXTPPropExchangeSection pxCommandBars(pPX->GetSection(_T("CommandBars")));
	GetCommandBars()->DoPropExchange(&pxCommandBars, &param);

	CXTPPropExchangeSection secShortcuts(pPX->GetSection(_T("Shortcuts")));
	GetCommandBars()->GetShortcutManager()->DoPropExchange(&secShortcuts);

	CXTPPropExchangeSection secOptions(pPX->GetSection(_T("Options")));	
	GetCommandBars()->GetCommandBarsOptions()->DoPropExchange(&secOptions, TRUE);

	CXTPPropExchangeSection pxDockingPane(pPX->GetSection(_T("DockingPane")));	
	m_paneManager.DoPropExchange(&pxDockingPane);
}

void CMainFrame::OnSchFullScreen()
{
	m_bSchFullScreen ^= 1;

	//CXTPPropExchangeININode px(FALSE, 0, _T("Settings"));
	CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));
	
	CXTPPropExchangeXMLNode* pxLayoutSave = DYNAMIC_DOWNCAST(CXTPPropExchangeXMLNode, px.GetSection(_T("FullScreenLayout")));
	ASSERT(pxLayoutSave);

	// Save current layout
	if (pxLayoutSave != 0)
	{	
		ExchangeLayout(pxLayoutSave, FALSE);
	}

	// If Full screen layout exists
	if (m_pSchFullScreenLayout && m_pSchFullScreenLayout->IsSectionExists(_T("CommandBars")))
	{
		// Set it
		m_pSchFullScreenLayout->SetLoading(TRUE);
		ExchangeLayout(m_pSchFullScreenLayout, FALSE);
	}
	// Else create new fullscreen layout. Hide all toolbars and DockingPanes.
	else
	{		
		for (int i = 0; i < GetCommandBars()->GetCount(); i++)
		{
			CXTPToolBar* pToolBar = GetCommandBars()->GetAt(i);
			pToolBar->SetVisible((pToolBar->GetType() == xtpBarTypeMenuBar) || (pToolBar->GetBarID() == IDR_TOOLBAR_FULLSCREEN));
		}
		m_paneManager.CloseAll();
	}
	
	// Save old layout
	if (m_pSchFullScreenLayout)
	{
		delete m_pSchFullScreenLayout;
	}
	m_pSchFullScreenLayout = pxLayoutSave;


	if (m_bSchFullScreen)
	{
		GetWindowRect(&m_rcMainFrame);

		ModifyStyle(WS_CAPTION|WS_THICKFRAME, 0);
		
		// Now resize the main window
		CRect rcScreen = XTPMultiMonitor()->GetScreenArea(this);
		
		int cxBorder = ::GetSystemMetrics(SM_CXBORDER);
		int cyBorder = ::GetSystemMetrics(SM_CYBORDER);

		SetWindowPos(NULL, rcScreen.left - cxBorder, rcScreen.top - cyBorder, 
			rcScreen.Width() + cxBorder * 2, rcScreen.Height() + cyBorder * 2, SWP_NOZORDER);

		m_wndStatusBar.ShowWindow(SW_HIDE);

	} 
	else
	{
		ModifyStyle(0, WS_CAPTION|WS_THICKFRAME);
		MoveWindow(&m_rcMainFrame);
		m_wndStatusBar.ShowWindow(SW_SHOW);
	}
	RecalcLayout(TRUE);
}

void CMainFrame::OnPcbFullScreen()
{
	m_bPcbFullScreen ^= 1;

	//CXTPPropExchangeININode px(FALSE, 0, _T("Settings"));
	CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));
	
	CXTPPropExchangeXMLNode* pxLayoutSave = DYNAMIC_DOWNCAST(CXTPPropExchangeXMLNode, px.GetSection(_T("FullScreenLayout")));
	ASSERT(pxLayoutSave);

	// Save current layout
	if (pxLayoutSave != 0)
	{	
		ExchangeLayout(pxLayoutSave, FALSE);
	}

	// If Full screen layout exists
	if (m_pPcbFullScreenLayout && m_pPcbFullScreenLayout->IsSectionExists(_T("CommandBars")))
	{
		// Set it
		m_pPcbFullScreenLayout->SetLoading(TRUE);
		ExchangeLayout(m_pPcbFullScreenLayout, FALSE);
	}
	// Else create new fullscreen layout. Hide all toolbars and DockingPanes.
	else
	{		
		for (int i = 0; i < GetCommandBars()->GetCount(); i++)
		{
			CXTPToolBar* pToolBar = GetCommandBars()->GetAt(i);
			pToolBar->SetVisible((pToolBar->GetType() == xtpBarTypeMenuBar) || (pToolBar->GetBarID() == IDR_TOOLBAR_FULLSCREEN));
		}
		m_paneManager.CloseAll();
	}
	
	// Save old layout
	if (m_pPcbFullScreenLayout)
	{
		delete m_pPcbFullScreenLayout;
	}
	m_pPcbFullScreenLayout = pxLayoutSave;


	if (m_bPcbFullScreen)
	{
		GetWindowRect(&m_rcMainFrame);

		ModifyStyle(WS_CAPTION|WS_THICKFRAME, 0);
		
		// Now resize the main window
		CRect rcScreen = XTPMultiMonitor()->GetScreenArea(this);
		
		int cxBorder = ::GetSystemMetrics(SM_CXBORDER);
		int cyBorder = ::GetSystemMetrics(SM_CYBORDER);

		SetWindowPos(NULL, rcScreen.left - cxBorder, rcScreen.top - cyBorder, 
			rcScreen.Width() + cxBorder * 2, rcScreen.Height() + cyBorder * 2, SWP_NOZORDER);

		m_wndStatusBar.ShowWindow(SW_HIDE);

	} 
	else
	{
		ModifyStyle(0, WS_CAPTION|WS_THICKFRAME);
		MoveWindow(&m_rcMainFrame);
		m_wndStatusBar.ShowWindow(SW_SHOW);
	}
	RecalcLayout(TRUE);
}

void CMainFrame::OnUpdateSchFullScreen(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bSchFullScreen);

}

void CMainFrame::OnUpdatePcbFullScreen(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bPcbFullScreen);
}

class CVisualStudio6Theme : public CXTPDefaultTheme
{
	CSize DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
	{
		if (pBar->IsDialogBar() || pBar->GetPosition() == xtpBarFloating)
			return CXTPDefaultTheme::DrawCommandBarGripper(pDC, pBar, bDraw);
		
		CRect rc;
		pBar->GetClientRect(&rc);
		
		if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
		{
			if (pDC && bDraw)
			{
				Draw3dRect(pDC, CRect(3, 3, rc.right - 3, 6), COLOR_BTNHILIGHT, COLOR_3DSHADOW);
				Draw3dRect(pDC, CRect(3, 7, rc.right - 3, 10), COLOR_BTNHILIGHT, COLOR_3DSHADOW);
			}
			return CSize(0, 10);
		}
		else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
		{
			if (pDC && bDraw)
			{
				Draw3dRect(pDC, CRect(3, 3, 6, rc.bottom - 3), COLOR_BTNHILIGHT, COLOR_3DSHADOW);
				Draw3dRect(pDC, CRect(7, 3, 10, rc.bottom - 3), COLOR_BTNHILIGHT, COLOR_3DSHADOW);
			}
			return CSize(10, 0);
		}
		return 0;
	}
};

void CMainFrame::OnSkinFramework(UINT nID)
{
#ifdef _XTP_INCLUDE_SKINFRAMEWORK
	switch (nID)
	{
	case ID_SKINFRAMEWORK_VISTABLUE:
		XTPSkinManager()->LoadSkin(NULL, _T("NORMALBLUE.INI"));
		break;

	case ID_SKINFRAMEWORK_VISTABLACK:
		XTPSkinManager()->LoadSkin(NULL, _T("NORMALBLACK.INI"));
		break;

	default:
		XTPSkinManager()->LoadSkin(NULL);
		break;
	}
#endif
}


BOOL CMainFrame::DestroyWindow()
{
	WINDOWPLACEMENT wp;
	wp.length = sizeof(wp);	
	if(GetWindowPlacement(&wp))
	{
		wp.flags = 0;
		if(IsZoomed()) 
		{
			wp.flags |= WPF_RESTORETOMAXIMIZED;
		}
		WriteWindowPlacement(&wp);
	}

	return CXTPMDIFrameWnd::DestroyWindow();
}

void CMainFrame::OnToPcb()
{
	if(nEDA_System_State == PCB) return;

	if(nEDA_Print_Flag != 0)
	{
		AfxMessageBox("打印或打印预览中...", MB_ICONWARNING);
		return;
	}

	CMDIChildWnd *pChild = NULL;
	CESDView *pView = NULL;
	CESDDoc* pDoc = NULL;

	pChild = MDIGetActive();
	if(pChild != NULL)
	{
		pView = (CESDView *) pChild->GetActiveView();
		if(pView != NULL)
		{
			pDoc = (CESDDoc*)pView->GetDocument();
			if(pDoc != NULL)
			{
				if(pDoc->m_nDocumentType != DOC_ESD) 
				{
					TRACE("topcb:%s \n\n\n", pDoc->GetPathName());
					AfxMessageBox("命令无效, 当前文件保存格式不能包含印制板设计, 请“另存为”ESD文件格式。", MB_ICONSTOP);
					return;
				}
			}
		}
	}
	
	LockWindowUpdate();

	
	CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));      // To serialize to XML file
	if (px.OnBeforeExchange()) 
	{
		// Reset Full Screen Mode
		if (m_bSchFullScreen)
		{
			OnSchFullScreen();
		}

		CXTPPropExchangeSection pxNormalLayout(px.GetSection(_T("NormalLayout")));
		ExchangeLayout(&pxNormalLayout);
	
		px.PutSection(m_pSchFullScreenLayout);

		px.SaveToFile(m_strIniFileName_SCH);
	}
	
	CXTPCommandBars* pCommandBars = GetCommandBars();

	
	pPCBMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_PCB_MAINMENU); 
	pPCBMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);
	pCommandBars->Remove(pSCHMenuBar);
	
	
	pCommandBars->Remove(pSCHMainToolBar);
	pCommandBars->Remove(pSCHPlaceToolBar1);
	pCommandBars->Remove(pSCHPlaceToolBar2);
	pCommandBars->Remove(pSCHOtherToolBar1);
	pCommandBars->Remove(pSCHOtherToolBar2);

	
	pPCBMainToolBar = (CXTPToolBar*)pCommandBars->Add(_T("标准"), xtpBarTop);
	if (!pPCBMainToolBar ||  !pPCBMainToolBar->LoadToolBar(IDR_PCB_MAINTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pPCBMainToolBar->ShowExpandButton(FALSE);

	pPCBPlaceToolBar = (CXTPToolBar*)pCommandBars->Add(_T("放置"), xtpBarTop);
	if (!pPCBPlaceToolBar ||  !pPCBPlaceToolBar->LoadToolBar(IDR_PCB_PLACETOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pPCBPlaceToolBar->ShowExpandButton(FALSE);
	DockRightOf(pPCBPlaceToolBar ,pPCBMainToolBar);
	
	pPCBOtherToolBar1 = (CXTPToolBar*)pCommandBars->Add(_T("工具"), xtpBarTop);
	if (!pPCBOtherToolBar1 ||  !pPCBOtherToolBar1->LoadToolBar(IDR_PCB_OTHERTOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pPCBOtherToolBar1->ShowExpandButton(FALSE);
	DockRightOf(pPCBOtherToolBar1, pPCBPlaceToolBar);

	XTPPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE;
	XTPImageManager()->SetIcons(IDR_TOOLBAR_EXT);

	pPCBDesignToolBar = (CXTPToolBar*)pCommandBars->Add(_T("设计"), xtpBarTop);
	if (!pPCBDesignToolBar ||  !pPCBDesignToolBar->LoadToolBar(IDR_PCB_DESIGNTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pPCBDesignToolBar->ShowExpandButton(FALSE);
	DockRightOf(pPCBDesignToolBar, pPCBOtherToolBar1);

	
	
	CXTPPropExchangeXMLNode px1(TRUE, 0, _T("Settings"));   // To serialize to XML file

	if (px1.LoadFromFile(m_strIniFileName_PCB))
	{
		CXTPPropExchangeSection pxNormalLayout(px1.GetSection(_T("NormalLayout")));
		ExchangeLayout(&pxNormalLayout);

		m_pPcbFullScreenLayout = DYNAMIC_DOWNCAST(CXTPPropExchangeXMLNode, px1.GetSection(_T("FullScreenLayout")));
		ASSERT(m_pPcbFullScreenLayout);
	}

	nEDA_System_State = PCB;
	
	::DestroyAcceleratorTable(m_hAccelTable);
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_PCB_MAINMENU));
		
	
	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_PCB_MAINMENU);

	
	UnlockWindowUpdate();

	if(pView != NULL)
	{
		
		pView->m_nSystem_State = nEDA_System_State;			
		pView->m_fSCH_Scale = pView->m_fScale;
		pView->m_fSCH_ScreenX = pView->m_fScreenX;
		pView->m_fSCH_ScreenY = pView->m_fScreenY;
		pView->m_fScale = pView->m_fPCB_Scale;
		pView->m_fScreenX = pView->m_fPCB_ScreenX;
		pView->m_fScreenY = pView->m_fPCB_ScreenY;

		pView->Screen_XYControl();
		pView->ScrollBar_Control();
	}

	if(pDoc != NULL)
	{
		
		if( pDoc->m_sPCB_LayerParam[pView->m_nPCB_Current_Layer].bLayerShow == FALSE )
		{
			pDoc->m_sPCB_LayerParam[pView->m_nPCB_Current_Layer].bLayerShow = TRUE;
		}
	}

	Invalidate();

	Set_Status_Help(szEDA_Help_Ready);
}

void CMainFrame::OnToSch()
{
	if(nEDA_System_State == SCH) return;

	if(nEDA_Print_Flag != 0)
	{
		AfxMessageBox("打印或打印预览中...", MB_ICONWARNING);
		return;
	}

	CMDIChildWnd *pChild = NULL;
	CESDView *pView = NULL;
	CESDDoc* pDoc = NULL;
	
	pChild = MDIGetActive();
	if(pChild != NULL)
	{
		pView = (CESDView *) pChild->GetActiveView();
		if(pView != NULL)
		{
			pDoc = (CESDDoc*)pView->GetDocument();
			if(pDoc != NULL)
			{
				if(pDoc->m_nDocumentType != DOC_ESD) 
				{
					AfxMessageBox("命令无效, 当前文件保存格式不能包含印制板设计, 请“另存为”ESD文件格式。", MB_ICONSTOP);
					return;
				}
			}
		}
	}

	LockWindowUpdate();

	CXTPPropExchangeXMLNode px_PCB(FALSE, 0, _T("Settings"));      // To serialize to XML file
	if (px_PCB.OnBeforeExchange()) 
	{
		// Reset Full Screen Mode
		if (m_bPcbFullScreen)
		{
			OnPcbFullScreen();
		}
		CXTPPropExchangeSection pxNormalLayout(px_PCB.GetSection(_T("NormalLayout")));
		ExchangeLayout(&pxNormalLayout);
		px_PCB.PutSection(m_pPcbFullScreenLayout);
		px_PCB.SaveToFile(m_strIniFileName_PCB);
	}


	CXTPCommandBars* pCommandBars = GetCommandBars();

	
	pSCHMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_SCH_MAINMENU); 
	pSCHMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);
	pCommandBars->Remove(pPCBMenuBar);
	
	
	pCommandBars->Remove(pPCBMainToolBar);
	pCommandBars->Remove(pPCBPlaceToolBar);
	pCommandBars->Remove(pPCBDesignToolBar);
	pCommandBars->Remove(pPCBOtherToolBar1);

	
	pSCHMainToolBar = (CXTPToolBar*)pCommandBars->Add(_T("标准"), xtpBarTop);
	if (!pSCHMainToolBar ||
		!pSCHMainToolBar->LoadToolBar(IDR_MAINTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pSCHMainToolBar->ShowExpandButton(FALSE);

	pSCHPlaceToolBar1 = (CXTPToolBar*)pCommandBars->Add(_T("放置"), xtpBarTop);
	if (!pSCHPlaceToolBar1 ||
		!pSCHPlaceToolBar1->LoadToolBar(IDR_PLACETOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pSCHPlaceToolBar1->ShowExpandButton(FALSE);
	DockRightOf(pSCHPlaceToolBar1 ,pSCHMainToolBar);

	pSCHPlaceToolBar2 = (CXTPToolBar*)pCommandBars->Add(_T("绘图"), xtpBarTop);
	if (!pSCHPlaceToolBar2 ||
		!pSCHPlaceToolBar2->LoadToolBar(IDR_PLACETOOLBAR2))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pSCHPlaceToolBar2->ShowExpandButton(FALSE);
	DockRightOf(pSCHPlaceToolBar2, pSCHPlaceToolBar1);

	pSCHOtherToolBar1 = (CXTPToolBar*)pCommandBars->Add(_T("辅助"), xtpBarTop);
	if (!pSCHOtherToolBar1 ||
		!pSCHOtherToolBar1->LoadToolBar(IDR_OTHERTOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pSCHOtherToolBar1->ShowExpandButton(FALSE);
	
	
	pSCHOtherToolBar2 = (CXTPToolBar*)pCommandBars->Add(_T("隐藏"), xtpBarTop);
	if (!pSCHOtherToolBar2 ||
		!pSCHOtherToolBar2->LoadToolBar(IDR_OTHERTOOLBAR2))
	{
		TRACE0("Failed to create toolbar\n");
		return;
	}
	DockRightOf(pSCHOtherToolBar2, pSCHOtherToolBar1);
	pSCHOtherToolBar2->ShowExpandButton(FALSE);
	pSCHOtherToolBar2->SetVisible(FALSE);

	XTPPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE;
	XTPImageManager()->SetIcons(IDR_TOOLBAR_EXT);
	
	
	CXTPPropExchangeXMLNode px1(TRUE, 0, _T("Settings"));   // To serialize to XML file
	if (px1.LoadFromFile(m_strIniFileName_SCH))
	{
		CXTPPropExchangeSection pxNormalLayout(px1.GetSection(_T("NormalLayout")));
		ExchangeLayout(&pxNormalLayout);

		m_pSchFullScreenLayout = DYNAMIC_DOWNCAST(CXTPPropExchangeXMLNode, px1.GetSection(_T("FullScreenLayout")));
		ASSERT(m_pSchFullScreenLayout);
	}
	
	nEDA_System_State = SCH;


	::DestroyAcceleratorTable(m_hAccelTable);

	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_SCH_MAINMENU));

	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_SCH_MAINMENU);

	
	UnlockWindowUpdate();	

	if(pView != NULL)
	{
		
		pView->m_nSystem_State = nEDA_System_State;
		pView->m_fPCB_Scale = pView->m_fScale;
		pView->m_fPCB_ScreenX = pView->m_fScreenX;
		pView->m_fPCB_ScreenY = pView->m_fScreenY;
		pView->m_fScale = pView->m_fSCH_Scale;
		pView->m_fScreenX = pView->m_fSCH_ScreenX;
		pView->m_fScreenY = pView->m_fSCH_ScreenY;
				
		pView->Screen_XYControl();
		pView->ScrollBar_Control();
	}

	Invalidate();

	Set_Status_Help(szEDA_Help_Ready);
}

void CMainFrame::OnDocTab()
{
	
	int flag=0;

	CWinApp* pApp = AfxGetApp();
	CMultiDocTemplate* pDocTemplate;
	POSITION pos1 = pApp->GetFirstDocTemplatePosition();
	while( pos1 != NULL)
	{
		pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos1);
		if(pDocTemplate != NULL)
		{
			CESDDoc* pDoc;
			POSITION pos2 = pDocTemplate->GetFirstDocPosition();
			while(pos2 != NULL)
			{
				pDoc = (CESDDoc*)pDocTemplate->GetNextDoc(pos2);
				if(pDoc != NULL)
				{
					flag = 1;
					AfxMessageBox("改变文档标签显示模式, 需要先关闭所有文档。");
					break;
				}
			}
		}
	}

	if(flag == 0)
	{
		if(m_MTIClientWnd.IsAttached())	m_MTIClientWnd.Detach();
		else m_MTIClientWnd.Attach(this, TRUE);
	}
}

void CMainFrame::OnUpdateDocTab(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_MTIClientWnd.IsAttached());
}

void CMainFrame::OnSheetTab()
{
	if(nEDA_Print_Flag != 0)
	{
		AfxMessageBox("打印或打印预览中...", MB_ICONWARNING);
		return;
	}

	if(nEDA_Sheet_State == 0)
	{
		nEDA_Sheet_State = 1;
		CWinApp* pApp = AfxGetApp();
		CMultiDocTemplate* pDocTemplate;
		POSITION pos1 = pApp->GetFirstDocTemplatePosition();
		while( pos1 != NULL)
		{
			pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos1);
			if(pDocTemplate != NULL)
			{
				CESDDoc* pDoc;
				POSITION pos2 = pDocTemplate->GetFirstDocPosition();
				while(pos2 != NULL)
				{
					pDoc = (CESDDoc*)pDocTemplate->GetNextDoc(pos2);
					if(pDoc != NULL)
					{
						CESDView* pView;
						POSITION pos = pDoc->GetFirstViewPosition();
						while(pos != NULL)
						{
							pView = (CESDView*)pDoc->GetNextView(pos);
							if(pView != NULL)
							{		
								
								CChildFrame* pChildFrame = (CChildFrame*)pView->GetParentFrame();
								pChildFrame->m_wndSheet.ShowSheet();
								pChildFrame->Invalidate();							
							}
						}
					}
				}
			}
		}
	}
	else
	{
		nEDA_Sheet_State = 0;

		CWinApp* pApp = AfxGetApp();
		CMultiDocTemplate* pDocTemplate;
		POSITION pos1 = pApp->GetFirstDocTemplatePosition();
		while( pos1 != NULL)
		{
			pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos1);
			if(pDocTemplate != NULL)
			{
				CESDDoc* pDoc;
				POSITION pos2 = pDocTemplate->GetFirstDocPosition();
				while(pos2 != NULL)
				{
					pDoc = (CESDDoc*)pDocTemplate->GetNextDoc(pos2);
					if(pDoc != NULL)
					{
						CESDView* pView;
						POSITION pos = pDoc->GetFirstViewPosition();
						while(pos != NULL)
						{
							pView = (CESDView*)pDoc->GetNextView(pos);
							if(pView != NULL)
							{		
								
								CChildFrame* pChildFrame = (CChildFrame*)pView->GetParentFrame();
								pChildFrame->m_wndSheet.HideSheet();
								pChildFrame->Invalidate();							
							}
						}
					}
				}
			}
		}
	}
}

void CMainFrame::OnUpdateSheetTab(CCmdUI *pCmdUI)
{
	if(nEDA_Sheet_State == 1) pCmdUI->SetCheck(1);
	else	pCmdUI->SetCheck(0);
}

void CMainFrame::Set_Status_XY(char * string)
{
	RECT rect;
	int index;
	index = m_wndStatusBar.CommandToIndex(IDS_INDICATOR_XY);
	m_wndStatusBar.SetPaneText(index,string);
	m_wndStatusBar.GetItemRect( index, &rect );
	m_wndStatusBar.RedrawWindow( &rect );
}

void CMainFrame::Set_Status_Help(char * string)
{
	RECT rect;
	int index = 0;
	//index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_XY);
	m_wndStatusBar.SetPaneText(index,string);
	m_wndStatusBar.GetItemRect( index, &rect );
	m_wndStatusBar.RedrawWindow( &rect );
}

void CMainFrame::Set_Status_Info(char * string)
{
	RECT rect;
	int index;
	index = m_wndStatusBar.CommandToIndex(IDS_INDICATOR_INFO);
	m_wndStatusBar.SetPaneText(index,string);
	m_wndStatusBar.GetItemRect( index, &rect );
	m_wndStatusBar.RedrawWindow( &rect );
}

void CMainFrame::Set_Unit(char * string)
{
	RECT rect;
	int index;
	index = m_wndStatusBar.CommandToIndex(IDS_INDICATOR_UNIT);
	m_wndStatusBar.SetPaneText(index,string);
	m_wndStatusBar.GetItemRect( index, &rect );
	m_wndStatusBar.RedrawWindow( &rect );
}

void CMainFrame::Set_Snap(char * string)
{
	RECT rect;
	int index;
	index = m_wndStatusBar.CommandToIndex(IDS_INDICATOR_SNAP);
	m_wndStatusBar.SetPaneText(index,string);
	m_wndStatusBar.GetItemRect( index, &rect );
	m_wndStatusBar.RedrawWindow( &rect );
}

void CMainFrame::Set_Visible(char * string)
{
	RECT rect;
	int index;
	index = m_wndStatusBar.CommandToIndex(IDS_INDICATOR_VISIBLE);
	m_wndStatusBar.SetPaneText(index,string);
	m_wndStatusBar.GetItemRect( index, &rect );
	m_wndStatusBar.RedrawWindow( &rect );
}

void CMainFrame::Set_Electrical(char * string)
{
	RECT rect;
	int index;
	index = m_wndStatusBar.CommandToIndex(IDS_INDICATOR_ELECTRICAL);
	m_wndStatusBar.SetPaneText(index,string);
	m_wndStatusBar.GetItemRect( index, &rect );
	m_wndStatusBar.RedrawWindow( &rect );
}

void CMainFrame::Set_Cursor()
{
	RECT rect;
	int index;
	char string[100];

	if(nEDA_Cursor_Mode == 0) strcpy(string,"小十字");
	else if(nEDA_Cursor_Mode == 1) strcpy(string,"大十字");
	else strcpy(string,"斜十字");

	index = m_wndStatusBar.CommandToIndex(IDS_INDICATOR_CURSOR);
	m_wndStatusBar.SetPaneText(index,string);
	m_wndStatusBar.GetItemRect( index, &rect );
	m_wndStatusBar.RedrawWindow( &rect );
}


void CMainFrame::DrawToolButton(UINT nID, BOOL state)
{
/*	int i,k;

	if(nEDA_System_State == SCH)
	{
		CXTPControl* pCtrl;
		CXTPControls* pControls;
		
		pControls = m_pSCHZoom->GetControls();
		k = pControls->GetCount();
		for(i = 0;i < k; i++)
		{
			pCtrl = pControls->GetAt(i);
			if((pCtrl != NULL) && (pCtrl->GetID() == nID))
			{
				pCtrl->SetChecked(state);
				m_pSCHZoom->RedrawWindow();
				return;	
			}
		}
	}
	else if(nEDA_System_State == PCB)
	{
		CXTPControl* pCtrl;
		CXTPControls* pControls;
		
		pControls = m_pPCBZoom->GetControls();
		k = pControls->GetCount();
		for(i = 0;i < k; i++)
		{
			pCtrl = pControls->GetAt(i);
			if((pCtrl != NULL) && (pCtrl->GetID() == nID))
			{
				pCtrl->SetChecked(state);
				m_pPCBZoom->RedrawWindow();
				return;	
			}
		}	
	}*/
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_LBUTTONDOWN)
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(pMsg->hwnd);
		CXTPStatusBar* sBar = DYNAMIC_DOWNCAST(CXTPStatusBar,pWnd);
		if(sBar!=NULL)  
		{
			CMenu Menu;
			CPoint pt;
			CRect* pRect = new CRect(0,0,0,0);
			pt.x = LOWORD(pMsg->lParam);
			pt.y = HIWORD(pMsg->lParam);
			
			if(nEDA_System_State == SCH)
			{
				
				sBar->GetItemRect( sBar->CommandToIndex( IDS_INDICATOR_UNIT), pRect );
				if((pt.x>(pRect->left))&&(pt.x<(pRect->right)))
				{
					CChildFrame* pChild = (CChildFrame*)GetActiveFrame();
					CESDDoc* pDoc = (CESDDoc*)pChild->GetActiveDocument();
					char unitstring[100];					
					if(pDoc->m_nSCH_Unit%2 == 0)
					{
						pDoc->m_nSCH_Unit = 1;
						strcpy(unitstring, "公制");
					}
					else 
					{
						pDoc->m_nSCH_Unit = 0;
						strcpy(unitstring, "英制");
					}
					Set_Unit(unitstring);
					sBar->Invalidate();
				}
				
				sBar->GetItemRect( sBar->CommandToIndex( IDS_INDICATOR_SNAP), pRect );
				if((pt.x>(pRect->left))&&(pt.x<(pRect->right)))
				{
					CChildFrame* pChild = (CChildFrame*)GetActiveFrame();
					CESDDoc* pDoc = (CESDDoc*)pChild->GetActiveDocument();
					if(pDoc->m_nSCH_Unit%2 == 0)
					{
						
						if(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_I[0])
						{
							pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_I[0];
						}
						else if(pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_I[2])
						{
							pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_I[0];
						}
						else if((pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_I[0])&&(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_I[1])) pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_I[1];
						else if((pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_I[1])&&(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_I[2])) pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_I[2];
					}
					else
					{
						
						if(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_M[0])
						{
							pDoc->m_fSCH_SnapGrid =fEDA_SCH_SnapGrid_M[0];
						}
						else if(pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_M[2])
						{
							pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_M[0];
						}
						else if((pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_M[0])&&(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_M[1])) pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_M[1];
						else if((pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_M[1])&&(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_M[2])) pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_M[2];
					}

					CString str = pDoc->Convert_XY_To_String(pDoc->m_fSCH_SnapGrid);
					char string[100];
					strcpy(string, "S:");
					strcat(string, str);
					Set_Snap(string);
				}
				
				sBar->GetItemRect( sBar->CommandToIndex( IDS_INDICATOR_CURSOR), pRect );
				if((pt.x>(pRect->left))&&(pt.x<(pRect->right)))
				{
					nEDA_Cursor_Mode++;
					if(nEDA_Cursor_Mode > 2) nEDA_Cursor_Mode = 0;
					Set_Cursor();
				}
				
				sBar->GetItemRect( sBar->CommandToIndex( IDS_INDICATOR_VISIBLE), pRect );
				if((pt.x>(pRect->left))&&(pt.x<(pRect->right)))
				{
					CChildFrame* pChild = (CChildFrame*)GetActiveFrame();
					CESDDoc* pDoc = (CESDDoc*)pChild->GetActiveDocument();
					char string[100];
					if(pDoc->m_bSCH_VisibleGridShow == TRUE)
					{
						pDoc->m_bSCH_VisibleGridShow = FALSE;
						strcpy(string, "V:隐藏");
					}
					else 
					{
						pDoc->m_bSCH_VisibleGridShow = TRUE;
						strcpy(string, "V:显示");
					}

					Set_Visible(string);
				}
				
				sBar->GetItemRect( sBar->CommandToIndex( IDS_INDICATOR_ELECTRICAL), pRect );
				if((pt.x>(pRect->left))&&(pt.x<(pRect->right)))
				{
					CChildFrame* pChild = (CChildFrame*)GetActiveFrame();
					CESDDoc* pDoc = (CESDDoc*)pChild->GetActiveDocument();
					char string[100];
					if(pDoc->m_bSCH_ElecGridEnable == TRUE)
					{
						pDoc->m_bSCH_ElecGridEnable = FALSE;
						strcpy(string, "E:禁止");
					}
					else 
					{
						pDoc->m_bSCH_ElecGridEnable = TRUE;
						strcpy(string, "E:允许");
					}

					Set_Electrical(string);
				}
			}
			else
			{
				
				sBar->GetItemRect( sBar->CommandToIndex( IDS_INDICATOR_UNIT), pRect );
				if((pt.x>(pRect->left))&&(pt.x<(pRect->right)))
				{
					CChildFrame* pChild = (CChildFrame*)GetActiveFrame();
					CESDDoc* pDoc = (CESDDoc*)pChild->GetActiveDocument();
					char unitstring[100];					
					if(pDoc->m_cPCB_Sheet.m_nUnit == UNIT_PCB_M)
					{
						pDoc->m_cPCB_Sheet.m_nUnit = UNIT_PCB_I;
						strcpy(unitstring, "英制");
					}
					else 
					{
						pDoc->m_cPCB_Sheet.m_nUnit = UNIT_PCB_M;
						strcpy(unitstring, "公制");
					}
					Set_Unit(unitstring);
					sBar->Invalidate();
				}
				
				sBar->GetItemRect( sBar->CommandToIndex( IDS_INDICATOR_CURSOR), pRect );
				if((pt.x>(pRect->left))&&(pt.x<(pRect->right)))
				{
					nEDA_Cursor_Mode++;
					if(nEDA_Cursor_Mode > 2) nEDA_Cursor_Mode = 0;
					Set_Cursor();
				}
				
				sBar->GetItemRect( sBar->CommandToIndex( IDS_INDICATOR_SNAP), pRect );
				if((pt.x>(pRect->left))&&(pt.x<(pRect->right)))
				{
					CChildFrame* pChild = (CChildFrame*)GetActiveFrame();
					CESDDoc* pDoc = (CESDDoc*)pChild->GetActiveDocument();
					
					if(Menu.LoadMenu(IDR_STATUSMENU))
					{
						CMenu* pSubMenu = Menu.GetSubMenu(0);
						pWnd->ClientToScreen(&pt);
						if(pSubMenu != NULL) pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,pt.x, pt.y, this);
					}
				}
				
				sBar->GetItemRect( sBar->CommandToIndex( IDS_INDICATOR_VISIBLE), pRect );
				if((pt.x>(pRect->left))&&(pt.x<(pRect->right)))
				{
					CChildFrame* pChild = (CChildFrame*)GetActiveFrame();
					CESDDoc* pDoc = (CESDDoc*)pChild->GetActiveDocument();
					
					if(Menu.LoadMenu(IDR_STATUSMENU))
					{
						CMenu* pSubMenu = Menu.GetSubMenu(1);
						pWnd->ClientToScreen(&pt);
						if(pSubMenu != NULL) pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,pt.x, pt.y, this);
					}
				}
				
				sBar->GetItemRect( sBar->CommandToIndex( IDS_INDICATOR_ELECTRICAL), pRect );
				if((pt.x>(pRect->left))&&(pt.x<(pRect->right)))
				{
					CChildFrame* pChild = (CChildFrame*)GetActiveFrame();
					CESDDoc* pDoc = (CESDDoc*)pChild->GetActiveDocument();
					char string[100];
					if(pDoc->m_cPCB_Sheet.m_bElecGridEnable == TRUE)
					{
						pDoc->m_cPCB_Sheet.m_bElecGridEnable = FALSE;
						strcpy(string, "E:禁止");
					}
					else
					{
						pDoc->m_cPCB_Sheet.m_bElecGridEnable = TRUE;
						strcpy(string, "E:允许");
					}
					Set_Electrical(string);
				}
			}
		
			delete pRect;
		}
	}
	else if(pMsg->message == WM_CHAR)
	{
		char c = (TCHAR)pMsg->wParam;
		if( ( c == 'l')||( c == 'L'))
		{

			return TRUE;
		}

	}
	else if(pMsg->message == WM_TIMER)
	{
		switch(pMsg->wParam)
		{
			case ID_AUTO_BACKUP_TIMER:
			{
				KillTimer(pMsg->wParam);
				
				CWinApp* pApp = AfxGetApp();
				CMultiDocTemplate* pDocTemplate;
				POSITION pos1 = pApp->GetFirstDocTemplatePosition();
				while( pos1 != NULL)
				{
					pDocTemplate = (CMultiDocTemplate*)pApp->GetNextDocTemplate(pos1);
					if(pDocTemplate != NULL)
					{
						CESDDoc* pDoc;
						POSITION pos2 = pDocTemplate->GetFirstDocPosition();
						while(pos2 != NULL)
						{
							pDoc = (CESDDoc*)pDocTemplate->GetNextDoc(pos2);
							if(pDoc != NULL)
							{
								char filepath[MAX_PATH], filename[MAX_PATH];
								int i,k;
								CString path = pDoc->GetPathName();
								strcpy(filepath,path);
								if( strcmp(filepath,"") == 0 )
								{
									path = pDoc->GetTitle();
									strcpy(filepath,path);
									sprintf(filename,"%s%s",filepath,".TMP");
								}
								else
								{
									k = strlen(filepath);
									for(i=1;i<=4;i++)
									{
										if(filepath[k-i] == '.')
										{
											filepath[k-i] = '\0';
											break;
										}
									}
									sprintf(filename,"%s%s",filepath,".TMP");
								}
								CFile  tempfile;
								if(!tempfile.Open(filename, CFile::modeCreate|CFile::modeWrite))
								{
									AfxMessageBox("系统错误: 无法打开指定文件.");
								}
								else
								{
									CArchive ar( &tempfile, CArchive::store);
									pDoc->Serialize(ar);
								}
							}
						}
					}
				}
				
				if(bEDA_Backup == TRUE)
				{
					UINT u = nEDA_Backup_Time*60000;
					if(u < 60000) u = 60000;
					SetTimer(ID_AUTO_BACKUP_TIMER,u,NULL);
				}
			}
			break;
			/*case ID_WATCHDOG_TIMER:
			{

			}
			break;*/
		}
	}
	else if(pMsg->message == WM_SYSKEYDOWN)
	{
		int vkey = (TCHAR)pMsg->wParam;    
		if(vkey == 'X')
		{
			SendMessage(WM_CLOSE);
			return TRUE;
		}
	}

	return CXTPMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnToPcb1() 
{
	CMDIChildWnd *pChild = NULL;
	CESDView *pView = NULL;
	CESDDoc* pDoc = NULL;

	pChild = MDIGetActive();
	if(pChild == NULL) return;
	pView = (CESDView *) pChild->GetActiveView();
	if(pView == NULL) return;
	pDoc = (CESDDoc*)pView->GetDocument();
    if(pDoc == NULL) return;

	if(nEDA_System_State == PCB) return;	

	LockWindowUpdate();

	
	CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));      // To serialize to XML file
	if (px.OnBeforeExchange()) 
	{
		// Reset Full Screen Mode
		if (m_bSchFullScreen)
		{
			OnSchFullScreen();
		}

		CXTPPropExchangeSection pxNormalLayout(px.GetSection(_T("NormalLayout")));
		ExchangeLayout(&pxNormalLayout);
	
		px.PutSection(m_pSchFullScreenLayout);
		px.SaveToFile(m_strIniFileName_SCH);
	}

	
	CXTPCommandBars* pCommandBars = GetCommandBars();

	
	pPCBMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_PCB_MAINMENU); 
	pPCBMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);
	pCommandBars->Remove(pSCHMenuBar);
	
	
	pCommandBars->Remove(pSCHMainToolBar);
	pCommandBars->Remove(pSCHPlaceToolBar1);
	pCommandBars->Remove(pSCHPlaceToolBar2);
	pCommandBars->Remove(pSCHOtherToolBar1);
	pCommandBars->Remove(pSCHOtherToolBar2);

	
	pPCBMainToolBar = (CXTPToolBar*)pCommandBars->Add(_T("标准"), xtpBarTop);
	if (!pPCBMainToolBar ||  !pPCBMainToolBar->LoadToolBar(IDR_PCB_MAINTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pPCBMainToolBar->ShowExpandButton(FALSE);

	pPCBPlaceToolBar = (CXTPToolBar*)pCommandBars->Add(_T("放置"), xtpBarTop);
	if (!pPCBPlaceToolBar ||  !pPCBPlaceToolBar->LoadToolBar(IDR_PCB_PLACETOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pPCBPlaceToolBar->ShowExpandButton(FALSE);
	DockRightOf(pPCBPlaceToolBar ,pPCBMainToolBar);
	
	pPCBOtherToolBar1 = (CXTPToolBar*)pCommandBars->Add(_T("工具"), xtpBarTop);
	if (!pPCBOtherToolBar1 ||  !pPCBOtherToolBar1->LoadToolBar(IDR_PCB_OTHERTOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pPCBOtherToolBar1->ShowExpandButton(FALSE);
	DockRightOf(pPCBOtherToolBar1, pPCBPlaceToolBar);

	XTPPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE;
	XTPImageManager()->SetIcons(IDR_TOOLBAR_EXT);

	pPCBDesignToolBar = (CXTPToolBar*)pCommandBars->Add(_T("设计"), xtpBarTop);
	if (!pPCBDesignToolBar ||  !pPCBDesignToolBar->LoadToolBar(IDR_PCB_DESIGNTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pPCBDesignToolBar->ShowExpandButton(FALSE);
	DockRightOf(pPCBDesignToolBar, pPCBOtherToolBar1);

	XTPPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE;
	XTPImageManager()->SetIcons(IDR_TOOLBAR_EXT);

		

	CXTPPropExchangeXMLNode px1(TRUE, 0, _T("Settings"));   // To serialize to XML file

	if (px1.LoadFromFile(m_strIniFileName_PCB))
	{
		CXTPPropExchangeSection pxNormalLayout(px1.GetSection(_T("NormalLayout")));
		ExchangeLayout(&pxNormalLayout);

		m_pPcbFullScreenLayout = DYNAMIC_DOWNCAST(CXTPPropExchangeXMLNode, px1.GetSection(_T("FullScreenLayout")));
		ASSERT(m_pPcbFullScreenLayout);
	}

	nEDA_System_State = PCB;
	
	::DestroyAcceleratorTable(m_hAccelTable);
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_PCB_MAINMENU));

	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_PCB_MAINMENU);

	UnlockWindowUpdate();
	Invalidate();

	Set_Status_Help(szEDA_Help_Ready);
}

void CMainFrame::OnToSch1()  
{
	CMDIChildWnd *pChild = NULL;
	CESDView *pView = NULL;
	CESDDoc* pDoc = NULL;

	pChild = MDIGetActive();
	if(pChild == NULL) return;
	pView = (CESDView *) pChild->GetActiveView();
	if(pView == NULL) return;
	pDoc = (CESDDoc*)pView->GetDocument();
    if(pDoc == NULL) return;

	if(nEDA_System_State == SCH) return;

	LockWindowUpdate();

	CXTPPropExchangeXMLNode px_PCB(FALSE, 0, _T("Settings"));      // To serialize to XML file
	if (px_PCB.OnBeforeExchange()) 
	{
		// Reset Full Screen Mode
		if (m_bPcbFullScreen)
		{
			OnPcbFullScreen();
		}
		CXTPPropExchangeSection pxNormalLayout(px_PCB.GetSection(_T("NormalLayout")));
		ExchangeLayout(&pxNormalLayout);
		px_PCB.PutSection(m_pPcbFullScreenLayout);
		px_PCB.SaveToFile(m_strIniFileName_PCB);
	}


	CXTPCommandBars* pCommandBars = GetCommandBars();

	
	pSCHMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_SCH_MAINMENU); 
	pSCHMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);
	pCommandBars->Remove(pPCBMenuBar);
	
	
	pCommandBars->Remove(pPCBMainToolBar);
	pCommandBars->Remove(pPCBPlaceToolBar);
	pCommandBars->Remove(pPCBDesignToolBar);
	pCommandBars->Remove(pPCBOtherToolBar1);

	
	pSCHMainToolBar = (CXTPToolBar*)pCommandBars->Add(_T("标准"), xtpBarTop);
	if (!pSCHMainToolBar ||
		!pSCHMainToolBar->LoadToolBar(IDR_MAINTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pSCHMainToolBar->ShowExpandButton(FALSE);

	pSCHPlaceToolBar1 = (CXTPToolBar*)pCommandBars->Add(_T("放置"), xtpBarTop);
	if (!pSCHPlaceToolBar1 ||
		!pSCHPlaceToolBar1->LoadToolBar(IDR_PLACETOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pSCHPlaceToolBar1->ShowExpandButton(FALSE);
	DockRightOf(pSCHPlaceToolBar1 ,pSCHMainToolBar);

	pSCHPlaceToolBar2 = (CXTPToolBar*)pCommandBars->Add(_T("绘图"), xtpBarTop);
	if (!pSCHPlaceToolBar2 ||
		!pSCHPlaceToolBar2->LoadToolBar(IDR_PLACETOOLBAR2))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pSCHPlaceToolBar2->ShowExpandButton(FALSE);
	DockRightOf(pSCHPlaceToolBar2, pSCHPlaceToolBar1);

	pSCHOtherToolBar1 = (CXTPToolBar*)pCommandBars->Add(_T("辅助"), xtpBarTop);
	if (!pSCHOtherToolBar1 ||
		!pSCHOtherToolBar1->LoadToolBar(IDR_OTHERTOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	pSCHOtherToolBar1->ShowExpandButton(FALSE);
	
	
	pSCHOtherToolBar2 = (CXTPToolBar*)pCommandBars->Add(_T("隐藏"), xtpBarTop);
	if (!pSCHOtherToolBar2 ||
		!pSCHOtherToolBar2->LoadToolBar(IDR_OTHERTOOLBAR2))
	{
		TRACE0("Failed to create toolbar\n");
		return ;
	}
	DockRightOf(pSCHOtherToolBar2, pSCHOtherToolBar1);
	pSCHOtherToolBar2->ShowExpandButton(FALSE);
	pSCHOtherToolBar2->SetVisible(FALSE);

	XTPPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE;
	XTPImageManager()->SetIcons(IDR_TOOLBAR_EXT);
	

	CXTPPropExchangeXMLNode px1(TRUE, 0, _T("Settings"));   // To serialize to XML file
	if (px1.LoadFromFile(m_strIniFileName_SCH))
	{
		CXTPPropExchangeSection pxNormalLayout(px1.GetSection(_T("NormalLayout")));
		ExchangeLayout(&pxNormalLayout);

		m_pSchFullScreenLayout = DYNAMIC_DOWNCAST(CXTPPropExchangeXMLNode, px1.GetSection(_T("FullScreenLayout")));
		ASSERT(m_pSchFullScreenLayout);
	}
	
	nEDA_System_State = SCH;

	::DestroyAcceleratorTable(m_hAccelTable);
	
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_SCH_MAINMENU));

	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_SCH_MAINMENU);

	UnlockWindowUpdate();
	Invalidate();

	Set_Status_Help(szEDA_Help_Ready);
}

void CMainFrame::OnTimer(UINT nIDEvent)
{

	if(nIDEvent == ID_SHEET_TAB_REFRESH)
	{
		OnSheetTab();
		KillTimer(ID_SHEET_TAB_REFRESH);
	}

	CXTPMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::AddZoomSlider()
{
	CXTPStatusBarSliderPane* pZoomPane = (CXTPStatusBarSliderPane*)m_wndStatusBar.AddIndicator(new CXTPStatusBarSliderPane(), ID_INDICATOR_ZOOMSLIDER);
	pZoomPane->SetWidth(100);
	
	pZoomPane->SetPos(100);
	pZoomPane->SetRange(0, 200);
	pZoomPane->SetStyle(SBPS_NOBORDERS);

	pZoomPane->SetBeginGroup(FALSE);

	pZoomPane->SetCaption(_T("缩放控制条"));
	pZoomPane->SetTooltip(_T("缩放控制条"));

}

void CMainFrame::AddZoomButton()
{
	CXTPStatusBarPane* pPane = m_wndStatusBar.AddIndicator(ID_INDICATOR_ZOOM);
	pPane->SetText(_T("1000%"));
	pPane->SetWidth(pPane->GetBestFit());
	pPane->SetButton();
	pPane->SetTextAlignment(DT_CENTER);
	pPane->SetTooltip(_T("缩放比例, 100% = 1:1."));
	pPane->SetBeginGroup(FALSE);

	pPane->SetCaption(_T("缩放比例"));

}

void CMainFrame::OnStatusSnap(UINT nID)
{
	char* item[13] = {"1mil", "5mil", "10mil", "20mil", "25mil", "50mil", "100mil", "0.025mm", "0.100mm", "0.250mm", "0.500mm", "1.000mm", "2.500mm" };
	int index = nID - ID_STATUS_SNAP1;
TRACE1("%d\n\n\n", index);
	if((index < 0) || (index > 12)) return;

	CChildFrame* pChild = (CChildFrame*)GetActiveFrame();
	if(pChild != NULL)
	{
		CESDView* pView = (CESDView*)pChild->GetActiveView();
		if(pView != NULL)
		{
			CESDDoc* pDoc = (CESDDoc*)pChild->GetActiveDocument();
			if(pDoc != NULL)
			{
				CString str;
				str = item[index];
				pDoc->m_cPCB_Sheet.m_fSnapGridSizeX = pDoc->Convert_PCB_String_To_XY(str);
				pDoc->m_cPCB_Sheet.m_fSnapGridSize = pDoc->m_cPCB_Sheet.m_fSnapGridSizeY = pDoc->m_cPCB_Sheet.m_fSnapGridSizeX;

				str = pDoc->Convert_XY_To_String(pDoc->m_cPCB_Sheet.m_fSnapGridSizeX);
				char string[100];
				strcpy(string, "S:");
				strcat(string, str);
				Set_Snap(string);
			}
		}
	}
}

void CMainFrame::OnStatusVisible(UINT nID)
{
	char* item[13] = {"10mil", "20mil", "25mil", "50mil", "100mil", "0.250mm", "0.500mm", "1.000mm", "2.500mm", "1x 锁定网格", "2x 锁定网格", "5x 锁定网格", "10x 锁定网格"};
	int index = nID - ID_STATUS_VISIBLE1;
	if((index < 0) || (index > 12)) return;

	CChildFrame* pChild = (CChildFrame*)GetActiveFrame();
	if(pChild != NULL)
	{
		CESDView* pView = (CESDView*)pChild->GetActiveView();
		if(pView != NULL)
		{
			CESDDoc* pDoc = (CESDDoc*)pChild->GetActiveDocument();
			if(pDoc != NULL)
			{
				CString str;
				str = item[index];
				if(str.Find("锁定网格") >= 0)
				{
					if(str.Find("1x") >= 0) pDoc->m_cPCB_Sheet.m_fBigVisibleGridSize = pDoc->m_cPCB_Sheet.m_fSnapGridSizeX;
					else if(str.Find("2x") >= 0) pDoc->m_cPCB_Sheet.m_fBigVisibleGridSize = 2*pDoc->m_cPCB_Sheet.m_fSnapGridSizeX;
					else if(str.Find("5x") >= 0) pDoc->m_cPCB_Sheet.m_fBigVisibleGridSize = 5*pDoc->m_cPCB_Sheet.m_fSnapGridSizeX;
					else if(str.Find("10x") >= 0) pDoc->m_cPCB_Sheet.m_fBigVisibleGridSize = 10*pDoc->m_cPCB_Sheet.m_fSnapGridSizeX;
				}
				else pDoc->m_cPCB_Sheet.m_fBigVisibleGridSize = pDoc->Convert_PCB_String_To_XY(str);

				str = pDoc->Convert_XY_To_String(pDoc->m_cPCB_Sheet.m_fBigVisibleGridSize);
				char string[100];
				strcpy(string, "V:");
				strcat(string, str);
				Set_Visible(string);

				pView->Invalidate();
			}
		}
	}
}
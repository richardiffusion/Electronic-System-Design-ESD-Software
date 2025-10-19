// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__57DF813D_6BDB_4AA1_A29E_0AAA116BC4D5__INCLUDED_)
#define AFX_MAINFRM_H__57DF813D_6BDB_4AA1_A29E_0AAA116BC4D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "ClassViewPane.h"
//#include "ResourceViewPane.h"
//#include "SolutionExplorerPane.h"
//#include "PropertiesPane.h"
//#include "OutputPane.h"

class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

	CXTPDockingPaneManager m_paneManager;

	CXTPTabClientWnd m_MTIClientWnd;

	CXTPStatusBar  m_wndStatusBar;


	CXTPMenuBar* pSCHMenuBar;   
	CXTPMenuBar* pPCBMenuBar;   
	
	CXTPToolBar* pSCHMainToolBar;
	CXTPToolBar* pSCHPlaceToolBar1;
	CXTPToolBar* pSCHPlaceToolBar2;
	CXTPToolBar* pSCHOtherToolBar1;
	CXTPToolBar* pSCHOtherToolBar2;
	
	CXTPToolBar* pPCBMainToolBar;
	CXTPToolBar* pPCBPlaceToolBar;
	CXTPToolBar* pPCBDesignToolBar;
	CXTPToolBar* pPCBOtherToolBar1;


// Operations
public:
	void AddZoomButton();
	void AddZoomSlider();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
	//}}AFX_VIRTUAL	
	
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	void ShowDockingPanePopupMenu(CXTPDockingPane* pPopupPane, CPoint pt);

// Implementation
public:
	virtual ~CMainFrame();

	void OnToPcb1();//int mode);
	void OnToSch1();//int mode);
	void DrawToolButton(UINT nID,BOOL state);
	void Set_Status_XY(char* string);
	void Set_Status_Help(char* string);
	void Set_Status_Info(char* string);
	void Set_Unit(char* string);
	void Set_Cursor();
	void Set_Snap(char* string);
	void Set_Visible(char* string);
	void Set_Electrical(char* string);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowAutohideall();
	afx_msg void OnWindowMore();
	afx_msg void OnClose();
	//}}AFX_MSG

	//afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStatusSnap(UINT nID);
	afx_msg void OnStatusVisible(UINT nID);

	afx_msg void OnShowView(UINT nID);
	afx_msg void OnEmptyCommandRange(UINT);
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg void OnCustomize();

	afx_msg void OnEditState(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateEditState(CCmdUI* pCmdUI);

	afx_msg void OnEditPlatform(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateEditPlatform(CCmdUI* pCmdUI);
	afx_msg void OnSchFullScreen();
	afx_msg void OnUpdateSchFullScreen(CCmdUI* pCmdUI);
	afx_msg void OnPcbFullScreen();
	afx_msg void OnUpdatePcbFullScreen(CCmdUI* pCmdUI);

	afx_msg void OnSkinFramework(UINT nID);

	void ExchangeLayout(CXTPPropExchange* pPX, BOOL bSerializeControls = TRUE);
	

	int m_nState;
	int m_nPlatform;
	CString m_strIniFileName_SCH;
	CString	m_strIniFileName_PCB;

	BOOL m_bSchFullScreen;
	BOOL m_bPcbFullScreen;
	CRect m_rcMainFrame;
	UINT m_nTheme;
	
	CXTPPropExchangeXMLNode* m_pSchFullScreenLayout;
	CXTPPropExchangeXMLNode* m_pPcbFullScreenLayout;


public:
	void InitialShowWindow(UINT nCmdShow);


	DECLARE_MESSAGE_MAP()
	virtual BOOL DestroyWindow();

	afx_msg void OnDocTab();
	afx_msg void OnUpdateDocTab(CCmdUI *pCmdUI);
	afx_msg void OnSheetTab();
	afx_msg void OnUpdateSheetTab(CCmdUI *pCmdUI);

public:
	afx_msg void OnToPcb();
	afx_msg void OnToSch();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT nIDEvent);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__57DF813D_6BDB_4AA1_A29E_0AAA116BC4D5__INCLUDED_)

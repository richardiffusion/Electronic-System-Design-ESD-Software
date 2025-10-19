/************************************
  REVISION LOG ENTRY
  Revision By: Mihai Filimon
  Revised on 7/19/98 11:33:04 AM
  Comments: SheetsWnd.h : header file, CSheetsWnd window
 ************************************/

#if !defined(AFX_SHEETSWND_H__67869EBF_1ED0_11D2_999A_00400561FB65__INCLUDED_)
#define AFX_SHEETSWND_H__67869EBF_1ED0_11D2_999A_00400561FB65__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define IDTHIS  0x03112
#define IDBTNS  0
#define SM_PAGECHANGING WM_USER + 100
#define SM_PAGECHANGED SM_PAGECHANGING + 1

class CSheetsWnd : public CWnd
{
// Construction
public:
	CSheetsWnd();
	// WPARAM contain pOldView activate, LPARAM pView that will be activated
	static UINT GetMessagePageChanging() { return SM_PAGECHANGING;};
	// WPARAM contain pView activate, LPARAM notused
	static UINT GetMessagePageChanged() { return SM_PAGECHANGED;};

// Attributes
public:
	
	BOOL m_bShowState;
	int m_nColorBlock; 
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetsWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void HideSheet();
	void ShowSheet();
	int AddPage(LPCTSTR lpszTitle);
	void RemovePage(int nPage);
	void RemovePage(LPCTSTR lpszTitle);
	void RefreshPage();
	int GetFirstPage();
	int GetNextPage();
	int GetActivePage();
	void SetActivePage(int n_page);
	CString GetPageTitle(int n_page);

	WNDPROC GetWindowProc();
	static void ResizeStatical(HWND hWndParent);
	static void RestoreWndProc(HWND hWndParent);
	virtual CRect GetDrawRect() const;
	static CSheetsWnd* GetWndSheet(HWND hWndParent);
	virtual BOOL Attach(CWnd * pWndParent, COLORREF rgbBackground = GetSysColor(COLOR_3DFACE));
	virtual ~CSheetsWnd();
	
	CStringArray m_strTitles;
	// Generated message map functions
protected:
	virtual int GetPageIndex(LPCTSTR lpszTitle);
	virtual BOOL IsEqual(CString s, CString d);
	virtual CRect EraseExclude();
	int GetViewFromPoint(CPoint point);
	CWnd* GetNextView();
	CWnd* GetFirstView();
	void Resize(HWND hWndParent);
	void Draw(CDC* pDC);
protected:
	//{{AFX_MSG(CSheetsWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CRect m_DrawRect;
	CScrollBar m_btns;
	CRect GetRect(CWnd* pWndParent);
	CRect GetRect(HWND hWndParent);
private:
	int m_nHorzScroll, m_nMaxHorzScroll;
	CFont m_font;
	CWnd* m_pViewFind;
	CBrush m_brBackground;
	WNDPROC m_wndProcParent;

	// moved from sheetalpha
	int m_nActive;
	int m_nFind;
	
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETSWND_H__67869EBF_1ED0_11D2_999A_00400561FB65__INCLUDED_)

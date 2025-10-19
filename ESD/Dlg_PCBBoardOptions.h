#pragma once

#include "ESDDoc.h"

// CDlg_PCBBoardOptions 对话框

class CDlg_PCBBoardOptions : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBBoardOptions)

public:
	CDlg_PCBBoardOptions(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBBoardOptions();

// 对话框数据
	enum { IDD = IDD_PCB_BOARD_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CESDDoc* m_pDoc;

	int m_nComboUnit;
	CString m_csComboSnapX;
	CString m_csComboSnapY;
	CString m_csComboCompX;
	CString m_csComboCompY;
	BOOL m_bCheckEGridEnable;
	CString m_csComboEGridRange;
	int m_nComboVGridStyle;
	CString m_csComboVGrid1;
	CString m_csComboVGrid2;
	CString m_csEditSheetX;
	CString m_csEditSheetY;
	CString m_csEditSheetWidth;
	CString m_csEditSheetHeight;

	BOOL m_bCheckShowSheet;
	BOOL m_bCheckLockSheet;
	int m_nComboDesignatorDisplay;
	BOOL m_bCheckAllLayer;
	afx_msg void OnBnClickedSheetSelect();
	afx_msg void OnBnClickedSheetClear();
	//afx_msg void OnBnClickedButtonXy();
};

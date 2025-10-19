#pragma once

#include "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditOffSheet 对话框

class CDlg_EditOffSheet : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditOffSheet)

public:
	CDlg_EditOffSheet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_EditOffSheet();

// 对话框数据
	enum { IDD = IDD_EDIT_OFFSHEET };

	CBitmap_Transparent_Dialog m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboStyle;
	int m_nComboOrientation;
	CString m_csEditNet;
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	
	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticColor();

	BOOL m_bCheckGlobal;
};

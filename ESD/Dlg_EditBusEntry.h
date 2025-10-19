#pragma once

#include  "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditBusEntry 对话框

class CDlg_EditBusEntry : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditBusEntry)

public:
	CDlg_EditBusEntry(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_EditBusEntry();

// 对话框数据
	enum { IDD = IDD_EDIT_BUSENTRY };

	CBitmap_Transparent_Dialog	m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboWidth;


	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticColor();
	CString m_csEditLocationX1;
	CString m_csEditLocationY1;
	CString m_csEditLocationX2;
	CString m_csEditLocationY2;
	BOOL m_bCheckGlobal;
};

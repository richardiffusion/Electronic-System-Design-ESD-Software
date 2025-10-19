#pragma once

#include  "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditArc 对话框

class CDlg_EditArc : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditArc)

public:
	CDlg_EditArc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_EditArc();

// 对话框数据
	enum { IDD = IDD_EDIT_ARC };

	CBitmap_Transparent_Dialog	m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	CString m_csEditRadius;

	double m_fEditStartAngle;
	double m_fEditEndAngle;
	int m_nComboWidth;
	
	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticColor();

	BOOL m_bCheckGlobal;
};

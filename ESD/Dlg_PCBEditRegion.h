#pragma once

#include  "Bitmap_Transparent_Dialog.h"

// CDlg_PCBEditRegion 对话框

class CDlg_PCBEditRegion : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditRegion)

public:
	CDlg_PCBEditRegion(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBEditRegion();

// 对话框数据
	enum { IDD = IDD_PCB_EDIT_REGION };

	CBitmap_Transparent_Dialog	m_Bitmap;

	CStringArray arrayString;
	CStringArray arrayStringNet;
	CString m_csMultiLayerName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboLayer;
	BOOL m_bCheckLocked;
	BOOL m_bCheckPolygonCutout;
	BOOL m_bCheckBoardCutout;
	BOOL m_bCheckKeepout;
	afx_msg void OnBnClickedCheckBoardCutout();
	afx_msg void OnBnClickedCheckPolygonCutout();
	CString m_csComboNet;
};

#pragma once


#include  "Bitmap_Transparent_Dialog.h"

// CDlg_PCBEditArc 对话框

class CDlg_PCBEditArc : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditArc)

public:
	CDlg_PCBEditArc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBEditArc();

	CBitmap_Transparent_Dialog	m_Bitmap;

	CStringArray arrayString;
	CStringArray arrayStringNet;

// 对话框数据
	enum { IDD = IDD_PCB_EDIT_ARC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CString m_csEditRadius;
	CString m_csEditWidth;
	CString m_csEditX;
	CString m_csEditY;

	BOOL m_bCheckLocked;
	BOOL m_bCheckKeepout;
	double m_fEditSangle;
	double m_fEditEangle;
	int m_nComboLayer;
	CString m_csComboNet;
};

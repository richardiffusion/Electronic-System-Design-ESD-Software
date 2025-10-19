#pragma once


// CDlg_PCBEditComp 对话框

class CDlg_PCBEditComp : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditComp)

public:
	CDlg_PCBEditComp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBEditComp();

// 对话框数据
	enum { IDD = IDD_PCB_EDIT_COMP };
	
	CStringArray arrayFont;
	CStringArray arrayStringLayer;
	
	int nStroke_ID_Font;
	int nTT_ID_Font;
	int nStroke_C_Font;
	int nTT_C_Font;
	CString m_csStrokeFontName[3];

	int  m_nRadio_ID_Font;
	int  m_nRadio_C_Font;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CString m_csEdit_ID;
	CString m_csEdit_C;
	CString m_csEdit_ID_Height;
	CString m_csEdit_C_Height;
	CString m_csEdit_ID_Width;
	CString m_csEdit_C_Width;
	int m_nCombo_ID_Layer;
	int m_nCombo_C_Layer;
	CString m_csEdit_ID_X;
	CString m_csEdit_C_X;
	CString m_csEdit_ID_Y;
	CString m_csEdit_C_Y;
	int m_nCombo_ID_AutoPosition;
	int m_nCombo_C_AutoPosition;
	BOOL m_bCheck_ID_Hide;
	BOOL m_bCheck_C_Hide;
	BOOL m_bCheck_ID_Mirror;
	BOOL m_bCheck_C_Mirror;
	int m_nCombo_ID_Font;
	int m_nCombo_C_Font;
	BOOL m_bCheck_ID_Bold;
	BOOL m_bCheck_C_Bold;
	BOOL m_bCheck_ID_Italic;
	BOOL m_bCheck_C_Italic;
	BOOL m_bCheck_ID_Invert;
	BOOL m_bCheck_C_Invert;
	CString m_csEdit_ID_Invert;
	CString m_csEdit_C_Invert;
	int m_nComboCompLayer;
	CString m_csEditCompX;
	CString m_csEditCompY;
	BOOL m_bCheckLockString;
	BOOL m_bCheckLock;
	CString m_csEditPackage;
	CString m_csEditLib;
	CString m_csEditDescription;
	virtual BOOL OnInitDialog();
	double m_fEdit_ID_Rotation;
	double m_fEdit_C_Rotation;
	double m_fEditCompRotation;
	afx_msg void OnBnClickedRadioIdFont();
	afx_msg void OnBnClickedRadioIdStroke();
	afx_msg void OnBnClickedRadioCFont();
	afx_msg void OnBnClickedRadioCStroke();
};

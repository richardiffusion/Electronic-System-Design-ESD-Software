#pragma once
#include "afxwin.h"
#include "struct.h"


// CDlg_SchLib_Add 对话框

class CDlg_SchLib_Add : public CDialog
{
	DECLARE_DYNAMIC(CDlg_SchLib_Add)

public:
	CDlg_SchLib_Add(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_SchLib_Add();

// 对话框数据
	enum { IDD = IDD_SCHLIB_ADD };
	COb_schcomp* pcomp;

	//=============== 应该返回的数据
	int m_nRadioLib;
	CString m_csLibName;
	int m_nRadioComp;
	CString m_csEditCompName;		
	CString m_csEditRef;			
	CString m_csEditPack;			
	CString m_csEditDescription;	
	CString m_csStaticNewPart;		
	int m_nComboPart;				
	int m_nPartCount;				


	//================ 控件
	CListBox m_listboxUserLibrary;
	CListBox m_listboxComp;
	CComboBox m_comboboxPart;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioNewcomp();
	afx_msg void OnBnClickedRadioNewlib();
	afx_msg void OnBnClickedRadioOldcomp();
	afx_msg void OnBnClickedRadioOldlib();
	afx_msg void OnBnClickedButtonCheck();
	afx_msg void OnLbnSelchangeListLib();
	afx_msg void OnLbnSelchangeListComp();
protected:
	virtual void OnOK();
public:
	CString m_csStaticOldName;
};

#pragma once
#include "afxwin.h"


// CDlg_PCBMultiSelection 对话框

class CDlg_PCBMultiSelection : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBMultiSelection)

public:
	CDlg_PCBMultiSelection(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBMultiSelection();

// 对话框数据
	enum { IDD = IDD_PCB_MULTI_SELECTION };

	CStringArray m_arrayString;
	
	int m_nx, m_ny;
	CXTPButton   m_btn[16];

	void DrawSelection(int index);
	BOOL first_show;
	CTypedPtrArray<CObArray, CObject*> m_arrayObject;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

protected:

public:

	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnHotItemChangeButton1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton8(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton9(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton10(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton11(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton12(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton13(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton14(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton15(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnHotItemChangeButton16(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

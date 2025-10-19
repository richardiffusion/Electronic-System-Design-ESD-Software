#pragma once

#include "Static_SampleText.h"

// CDlg_Font �Ի���

class CDlg_Font : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Font)

public:
	CDlg_Font(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Font();
	
	void UpdateFont();

// �Ի�������
	enum { IDD = IDD_FONT };
	CStatic_SampleText	m_txtSample;
	CXTFlatComboBox	m_wndComboSize;
	CXTFontCombo	m_wndComboFont;
	CXTColorPicker	m_wndColorPicker;
	BOOL	m_bBold;
	BOOL	m_bItalic;
	BOOL	m_bUnderline;
	
	CXTLogFont m_lf;
	CString  m_strFontSize;
	CString  m_strFontName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	//HICON m_hIcon;
	//COLORREF m_crCurrent;


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFontUnderline();
	afx_msg void OnBnClickedFontItalic();
	afx_msg void OnBnClickedFontBold();
	afx_msg void OnCbnSelendokFontSize();
	afx_msg void OnCbnSelendokFontFont();
};

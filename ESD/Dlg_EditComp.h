#pragma once

#include "Static_Color.h"
#include "CompListCtrl.h"

// CDlg_EditComp 对话框

class CDlg_EditComp : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditComp)

public:
	CDlg_EditComp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_EditComp();

// 对话框数据
	enum { IDD = IDD_EDIT_COMPONENT };
	CDocument* pDocument;
	CCompListCtrl  m_listCtrl;
	struct text_others
	{
		BOOL visible;
		CString item;
		CString value;
	};
	text_others onetext;

	int m_nPartCount;
	int m_nPart;
	int m_nDisplayModeCount;
	int m_nComboDisplayMode;

	CArray<text_others, text_others&> m_arrayTextOthers;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditDesignator;
	CString m_csEditComment;
	BOOL m_bCheckDesignatorVisible;
	BOOL m_bCheckCommentVisible;
	CString m_csEditDescription;
	CString m_csEditUniqueID;
	int m_nComboType;
	CString m_csEditLibrary;
	CString m_csEditModelName;
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	int m_nComboOrientation;
	BOOL m_bCheckLocalColors;
	BOOL m_bCheckShowAllPins;
	BOOL m_bCheckLockPins;
	BOOL m_bCheckMirrored;
	CString m_csEditPackage;

	CStatic_Color m_cFillColor;
	COLORREF m_nFillColor;
	CStatic_Color m_cLineColor;
	COLORREF m_nLineColor;
	CStatic_Color m_cPinColor;
	COLORREF m_nPinColor;
		
	void ShowPackage();

	afx_msg void OnPaint();
	afx_msg void OnBnClickedCheckLocalColors();
	afx_msg void OnStnClickedStaticPincolor();
	afx_msg void OnStnClickedStaticLinecolor();
	afx_msg void OnStnClickedStaticFillcolor();
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonChoosepackage();
	afx_msg void OnBnClickedButtonBackword();
	afx_msg void OnBnClickedButtonForword();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedButtonEditpins();
	afx_msg void OnEnChangeEditPackage();
	afx_msg void OnBnClickedButtonIdGlobal();
	afx_msg void OnBnClickedButtonCommentGlobal();
};

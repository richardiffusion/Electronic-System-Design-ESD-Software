#pragma once
#include "afxcmn.h"
#include "MarkupStatic.h"

// CDlg_HighlightList �Ի���

class CDlg_HighlightList : public CDialog
{
	DECLARE_DYNAMIC(CDlg_HighlightList)

public:
	CDlg_HighlightList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_HighlightList();

// �Ի�������
	enum { IDD = IDD_HIGHLIGHT_LIST };
	char* pListBuffer;
	CMarkupStatic	m_wndStatic;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();

};

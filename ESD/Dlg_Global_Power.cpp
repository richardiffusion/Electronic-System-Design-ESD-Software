// Dlg_Global_Power.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Global_Power.h"


// CDlg_Global_Power �Ի���

IMPLEMENT_DYNAMIC(CDlg_Global_Power, CDialog)

CDlg_Global_Power::CDlg_Global_Power(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Global_Power::IDD, pParent)
	, m_bCheckStyle(TRUE)
	, m_bCheckColor(TRUE)
	, m_bCheckOrientation(FALSE)
	, m_bCheckVisible(TRUE)
{
	m_nRadioScope = 0;
	bIsCrossSheetConnector = FALSE;
}

CDlg_Global_Power::~CDlg_Global_Power()
{
}

void CDlg_Global_Power::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_STYLE, m_bCheckStyle);
	DDX_Check(pDX, IDC_CHECK_COLOR, m_bCheckColor);
	DDX_Check(pDX, IDC_CHECK_ORIENTATION, m_bCheckOrientation);
	DDX_Check(pDX, IDC_CHECK_VISIBLE, m_bCheckVisible);
	DDX_Radio(pDX, IDC_RADIO_SCOPE, m_nRadioScope);
}


BEGIN_MESSAGE_MAP(CDlg_Global_Power, CDialog)
END_MESSAGE_MAP()


// CDlg_Global_Power ��Ϣ�������

BOOL CDlg_Global_Power::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(bIsCrossSheetConnector == TRUE)
	{
		CWnd* pwnd = GetDlgItem(IDC_CHECK_VISIBLE);
		pwnd->ShowWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

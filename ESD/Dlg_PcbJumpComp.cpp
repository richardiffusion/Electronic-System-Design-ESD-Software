// Dlg_PcbJumpComp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PcbJumpComp.h"
#include ".\dlg_pcbjumpcomp.h"


// CDlg_PcbJumpComp �Ի���

IMPLEMENT_DYNAMIC(CDlg_PcbJumpComp, CDialog)
CDlg_PcbJumpComp::CDlg_PcbJumpComp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PcbJumpComp::IDD, pParent)
	, m_csCompDesignator(_T(""))
{
}

CDlg_PcbJumpComp::~CDlg_PcbJumpComp()
{
}

void CDlg_PcbJumpComp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_csCompDesignator);
}


BEGIN_MESSAGE_MAP(CDlg_PcbJumpComp, CDialog)
END_MESSAGE_MAP()


// CDlg_PcbJumpComp ��Ϣ�������

BOOL CDlg_PcbJumpComp::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(csTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

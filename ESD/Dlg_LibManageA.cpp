// Dlg_LibManageA.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_LibManageA.h"

// CDlg_LibManageA �Ի���

IMPLEMENT_DYNAMIC(CDlg_LibManageA, CDialog)

CDlg_LibManageA::CDlg_LibManageA(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_LibManageA::IDD, pParent)
{
	m_nLib = m_nComp = 0;
}

CDlg_LibManageA::~CDlg_LibManageA()
{
}

void CDlg_LibManageA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_LibManageA, CDialog)
END_MESSAGE_MAP()


// CDlg_LibManageA ��Ϣ�������

BOOL CDlg_LibManageA::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

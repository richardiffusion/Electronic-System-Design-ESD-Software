// Dlg_Edit_JumpComp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Edit_JumpComp.h"


// CDlg_Edit_JumpComp �Ի���

IMPLEMENT_DYNAMIC(CDlg_Edit_JumpComp, CDialog)
CDlg_Edit_JumpComp::CDlg_Edit_JumpComp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Edit_JumpComp::IDD, pParent)
	, m_csEditText(_T(""))
	, m_nComboScope(0)
{
}

CDlg_Edit_JumpComp::~CDlg_Edit_JumpComp()
{
}

void CDlg_Edit_JumpComp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_csEditText);
	DDX_CBIndex(pDX, IDC_COMBO_SCOPE, m_nComboScope);
}


BEGIN_MESSAGE_MAP(CDlg_Edit_JumpComp, CDialog)
END_MESSAGE_MAP()


// CDlg_Edit_JumpComp ��Ϣ�������

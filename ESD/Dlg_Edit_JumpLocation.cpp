// Dlg_Edit_JumpLocation.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Edit_JumpLocation.h"


// CDlg_Edit_JumpLocation 对话框

IMPLEMENT_DYNAMIC(CDlg_Edit_JumpLocation, CDialog)
CDlg_Edit_JumpLocation::CDlg_Edit_JumpLocation(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Edit_JumpLocation::IDD, pParent)

	, m_csEditX(_T(""))
	, m_csEditY(_T(""))
{
}

CDlg_Edit_JumpLocation::~CDlg_Edit_JumpLocation()
{
}

void CDlg_Edit_JumpLocation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_X, m_csEditX);
	DDX_Text(pDX, IDC_EDIT_Y, m_csEditY);
}


BEGIN_MESSAGE_MAP(CDlg_Edit_JumpLocation, CDialog)
END_MESSAGE_MAP()


// CDlg_Edit_JumpLocation 消息处理程序

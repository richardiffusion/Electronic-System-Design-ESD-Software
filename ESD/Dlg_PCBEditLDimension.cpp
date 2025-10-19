// Dlg_PCBEditLDimension.cpp : 实现文件
//

#include "stdafx.h"
#include "Dlg_PCBEditLDimension.h"
#include ".\dlg_pcbeditldimension.h"


// CDlg_PCBEditLDimension 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBEditLDimension, CDialog)
CDlg_PCBEditLDimension::CDlg_PCBEditLDimension(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBEditLDimension::IDD, pParent)
{
}

CDlg_PCBEditLDimension::~CDlg_PCBEditLDimension()
{
}

void CDlg_PCBEditLDimension::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_LDIMENSION, m_Bitmap);
}


BEGIN_MESSAGE_MAP(CDlg_PCBEditLDimension, CDialog)
END_MESSAGE_MAP()


// CDlg_PCBEditLDimension 消息处理程序

BOOL CDlg_PCBEditLDimension::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_PCB_EDIT_LDIMENSION);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

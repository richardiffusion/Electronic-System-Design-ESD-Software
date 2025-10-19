// Dlg_File_Insert.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_File_Insert.h"


// CDlg_File_Insert 对话框

IMPLEMENT_DYNAMIC(CDlg_File_Insert, CDialog)
CDlg_File_Insert::CDlg_File_Insert(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_File_Insert::IDD, pParent)
{
}

CDlg_File_Insert::~CDlg_File_Insert()
{
}

void CDlg_File_Insert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RADIO_OPEN, m_nOpenMode);
}


BEGIN_MESSAGE_MAP(CDlg_File_Insert, CDialog)
END_MESSAGE_MAP()


// CDlg_File_Insert 消息处理程序

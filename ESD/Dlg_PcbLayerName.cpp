// Dlg_PcbLayerName.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PcbLayerName.h"


// CDlg_PCBLayerName 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBLayerName, CDialog)
CDlg_PCBLayerName::CDlg_PCBLayerName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBLayerName::IDD, pParent)
	, m_csEditLayerName(_T(""))
{
}

CDlg_PCBLayerName::~CDlg_PCBLayerName()
{
}

void CDlg_PCBLayerName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LAYERNAME, m_csEditLayerName);
}


BEGIN_MESSAGE_MAP(CDlg_PCBLayerName, CDialog)
END_MESSAGE_MAP()


// CDlg_PCBLayerName 消息处理程序

// Dlg_PCBEditLDimension.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dlg_PCBEditLDimension.h"
#include ".\dlg_pcbeditldimension.h"


// CDlg_PCBEditLDimension �Ի���

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


// CDlg_PCBEditLDimension ��Ϣ�������

BOOL CDlg_PCBEditLDimension::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_PCB_EDIT_LDIMENSION);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

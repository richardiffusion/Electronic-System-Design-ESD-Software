// Dlg_PCBEditFill.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBEditFill.h"
#include ".\dlg_pcbeditfill.h"


// CDlg_PCBEditFill 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBEditFill, CDialog)
CDlg_PCBEditFill::CDlg_PCBEditFill(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBEditFill::IDD, pParent)
	, m_csEditY2(_T(""))
	, m_csEditX2(_T(""))
	, m_csEditY1(_T(""))
	, m_csEditX1(_T(""))
	, m_bCheckLocked(FALSE)
	, m_bCheckKeepout(FALSE)
	, m_nComboLayer(0)
	, m_csComboNet(_T(""))
	, m_fEditRotation(0)
{
}

CDlg_PCBEditFill::~CDlg_PCBEditFill()
{
}

void CDlg_PCBEditFill::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_FILL, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT_Y2, m_csEditY2);
	DDX_Text(pDX, IDC_EDIT_X2, m_csEditX2);
	DDX_Text(pDX, IDC_EDIT_Y1, m_csEditY1);
	DDX_Text(pDX, IDC_EDIT_X1, m_csEditX1);
	DDX_Check(pDX, IDC_CHECK_LOCKED, m_bCheckLocked);
	DDX_Check(pDX, IDC_CHECK_KEEPOUT, m_bCheckKeepout);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nComboLayer);
	DDX_CBString(pDX, IDC_COMBO_NET, m_csComboNet);
	DDX_Text(pDX, IDC_EDIT_ROTATION, m_fEditRotation);
}


BEGIN_MESSAGE_MAP(CDlg_PCBEditFill, CDialog)
END_MESSAGE_MAP()


// CDlg_PCBEditFill 消息处理程序

BOOL CDlg_PCBEditFill::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_PCB_EDIT_FILL);
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LAYER);
	pCombo->ResetContent();
	for(int i=0; i<arrayString.GetCount(); i++)
	{
		pCombo->AddString(arrayString.GetAt(i));
	}
	pCombo->SetCurSel(m_nComboLayer);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_NET);
	pCombo->ResetContent();
	for(int i=0; i<arrayStringNet.GetCount(); i++)
	{
		pCombo->AddString(arrayStringNet.GetAt(i));
	}
	pCombo->SelectString(-1, m_csComboNet);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

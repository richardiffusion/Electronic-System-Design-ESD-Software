// Dlg_PCBEditArc.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBEditArc.h"
#include ".\dlg_pcbeditarc.h"


// CDlg_PCBEditArc 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBEditArc, CDialog)
CDlg_PCBEditArc::CDlg_PCBEditArc(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBEditArc::IDD, pParent)
	, m_csEditRadius(_T(""))
	, m_csEditWidth(_T(""))
	, m_csEditX(_T(""))
	, m_csEditY(_T(""))
	, m_bCheckLocked(FALSE)
	, m_bCheckKeepout(FALSE)
	, m_fEditSangle(0)
	, m_fEditEangle(0)
	, m_nComboLayer(0)
	, m_csComboNet(_T(""))
{
}

CDlg_PCBEditArc::~CDlg_PCBEditArc()
{
}

void CDlg_PCBEditArc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_ARC, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_csEditRadius);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_csEditWidth);
	DDX_Text(pDX, IDC_EDIT_X, m_csEditX);
	DDX_Text(pDX, IDC_EDIT_Y, m_csEditY);
	DDX_Check(pDX, IDC_CHECK_LOCKED, m_bCheckLocked);
	DDX_Check(pDX, IDC_CHECK_KEEPOUT, m_bCheckKeepout);
	DDX_Text(pDX, IDC_EDIT_SANGLE, m_fEditSangle);
	DDX_Text(pDX, IDC_EDIT_EANGLE, m_fEditEangle);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nComboLayer);
	DDX_CBString(pDX, IDC_COMBO_NET, m_csComboNet);
}


BEGIN_MESSAGE_MAP(CDlg_PCBEditArc, CDialog)
END_MESSAGE_MAP()


// CDlg_PCBEditArc 消息处理程序

BOOL CDlg_PCBEditArc::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_PCB_EDIT_ARC);
	
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

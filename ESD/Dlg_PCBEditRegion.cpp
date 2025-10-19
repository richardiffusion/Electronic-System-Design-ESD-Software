// Dlg_PCBEditRegion.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBEditRegion.h"
#include ".\dlg_pcbeditregion.h"


// CDlg_PCBEditRegion 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBEditRegion, CDialog)
CDlg_PCBEditRegion::CDlg_PCBEditRegion(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBEditRegion::IDD, pParent)
	, m_nComboLayer(0)
	, m_bCheckLocked(FALSE)
	, m_bCheckPolygonCutout(FALSE)
	, m_bCheckBoardCutout(FALSE)
	, m_bCheckKeepout(FALSE)
	, m_csComboNet(_T(""))
{
}

CDlg_PCBEditRegion::~CDlg_PCBEditRegion()
{
}

void CDlg_PCBEditRegion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_EDIT_REGION, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nComboLayer);
	DDX_Check(pDX, IDC_CHECK_LOCKED, m_bCheckLocked);
	DDX_Check(pDX, IDC_CHECK_POLYGON_CUTOUT, m_bCheckPolygonCutout);
	DDX_Check(pDX, IDC_CHECK_BOARD_CUTOUT, m_bCheckBoardCutout);
	DDX_Check(pDX, IDC_CHECK_KEEPOUT, m_bCheckKeepout);
	DDX_CBString(pDX, IDC_COMBO_NET, m_csComboNet);
}


BEGIN_MESSAGE_MAP(CDlg_PCBEditRegion, CDialog)
	ON_BN_CLICKED(IDC_CHECK_BOARD_CUTOUT, OnBnClickedCheckBoardCutout)
	ON_BN_CLICKED(IDC_CHECK_POLYGON_CUTOUT, OnBnClickedCheckPolygonCutout)
END_MESSAGE_MAP()


// CDlg_PCBEditRegion 消息处理程序

BOOL CDlg_PCBEditRegion::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_PCB_EDIT_REGION);
	
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

	if(m_bCheckBoardCutout == TRUE)
	{
		CWnd* pWnd = GetDlgItem(IDC_CHECK_KEEPOUT);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_KEEPOUT);
		pWnd->ShowWindow(FALSE);

		pWnd = GetDlgItem(IDC_COMBO_LAYER);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_LAYER);
		pWnd->ShowWindow(FALSE);	

		pWnd = GetDlgItem(IDC_COMBO_NET);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_NET);
		pWnd->ShowWindow(FALSE);
	}
	else if(m_bCheckPolygonCutout == TRUE)
	{
		CWnd* pWnd = GetDlgItem(IDC_CHECK_KEEPOUT);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_KEEPOUT);
		pWnd->ShowWindow(FALSE);

		pWnd = GetDlgItem(IDC_COMBO_NET);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_NET);
		pWnd->ShowWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_PCBEditRegion::OnBnClickedCheckBoardCutout()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_BOARD_CUTOUT);
	if(pButton->GetCheck() == BST_CHECKED)
	{

		m_bCheckKeepout = TRUE;

		for(int i=0; i<arrayString.GetCount(); i++)
		{
			if(m_csMultiLayerName == arrayString.GetAt(i)) 
			{
				m_nComboLayer = i;
				break;
			}
		}		
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LAYER);
		pCombo->SetCurSel(m_nComboLayer);

		pButton = (CButton*)GetDlgItem(IDC_CHECK_POLYGON_CUTOUT);
		pButton->SetCheck(FALSE);

		CWnd* pWnd = GetDlgItem(IDC_CHECK_KEEPOUT);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_KEEPOUT);
		pWnd->ShowWindow(FALSE);


		pWnd = GetDlgItem(IDC_COMBO_LAYER);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_LAYER);
		pWnd->ShowWindow(FALSE);	

		pWnd = GetDlgItem(IDC_COMBO_NET);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_NET);
		pWnd->ShowWindow(FALSE);
	}
	else if(pButton->GetCheck() == BST_UNCHECKED)
	{
		CWnd* pWnd = GetDlgItem(IDC_CHECK_KEEPOUT);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_KEEPOUT);
		pWnd->ShowWindow(TRUE);

		pWnd = GetDlgItem(IDC_COMBO_LAYER);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_LAYER);
		pWnd->ShowWindow(TRUE);	

		pWnd = GetDlgItem(IDC_COMBO_NET);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_NET);
		pWnd->ShowWindow(TRUE);
	}
}

void CDlg_PCBEditRegion::OnBnClickedCheckPolygonCutout()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_POLYGON_CUTOUT);
	if(pButton->GetCheck() == BST_CHECKED)
	{
		pButton = (CButton*)GetDlgItem(IDC_CHECK_BOARD_CUTOUT);
		pButton->SetCheck(FALSE);
		
		CWnd* pWnd = GetDlgItem(IDC_COMBO_LAYER);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_LAYER);
		pWnd->ShowWindow(TRUE);	

		pWnd = GetDlgItem(IDC_CHECK_KEEPOUT);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_KEEPOUT);
		pWnd->ShowWindow(FALSE);

		pWnd = GetDlgItem(IDC_COMBO_NET);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_NET);
		pWnd->ShowWindow(FALSE);
	}
	else if(pButton->GetCheck() == BST_UNCHECKED)
	{
		CWnd* pWnd = GetDlgItem(IDC_CHECK_KEEPOUT);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_KEEPOUT);
		pWnd->ShowWindow(TRUE);

		pWnd = GetDlgItem(IDC_COMBO_NET);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_NET);
		pWnd->ShowWindow(TRUE);
	}	
}

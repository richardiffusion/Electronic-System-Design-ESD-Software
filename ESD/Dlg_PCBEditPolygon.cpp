// Dlg_PCBEditPolygon.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBEditPolygon.h"
#include ".\dlg_pcbeditpolygon.h"


// CDlg_PCBEditPolygon 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBEditPolygon, CDialog)
CDlg_PCBEditPolygon::CDlg_PCBEditPolygon(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBEditPolygon::IDD, pParent)
	, m_bCheckRemoveIsland(FALSE)
	, m_csEditArcPrecision(_T(""))
	, m_bCheckRemoveNeck(FALSE)
	, m_csEditRemoveNeck(_T(""))
	, m_csEditTrackWidth(_T(""))
	, m_csEditGridSize(_T(""))
	, m_nComboLayer(0)
	, m_nEditRemoveIsland(0)
	, m_csName(_T(""))
	, m_csEditMinLength(_T(""))
	, m_bCheckPrimitiveLock(FALSE)
	, m_bCheckLocked(FALSE)
	, m_bCheckIgnoreViolation(FALSE)
	, m_nComboPourStyle(0)
	, m_bCheckRemoveDead(FALSE)
	, m_csComboNet(_T(""))
	, m_csEditPourGap(_T(""))
{
	m_nFillMode = 0;
	m_nPadStyle = 0;
	m_nHatchStyle = 0;
}

CDlg_PCBEditPolygon::~CDlg_PCBEditPolygon()
{
}

void CDlg_PCBEditPolygon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_POLYGON1_1, m_Bitmap1_1);
	DDX_Control(pDX, IDC_STATIC_EDIT_POLYGON1_2, m_Bitmap1_2);
	DDX_Control(pDX, IDC_STATIC_EDIT_POLYGON1_3, m_Bitmap1_3);
	DDX_Control(pDX, IDC_STATIC_EDIT_POLYGON1_4, m_Bitmap1_4);
	DDX_Control(pDX, IDC_STATIC_EDIT_POLYGON2, m_Bitmap2);
	DDX_Control(pDX, IDC_STATIC_EDIT_POLYGON3, m_Bitmap3);
	DDX_Radio(pDX, IDC_RADIO_FILLMODE1, m_nFillMode);
	DDX_Radio(pDX, IDC_RADIO_PAD_STYLE1, m_nPadStyle);
	DDX_Radio(pDX, IDC_RADIO_HATCH_STYLE1, m_nHatchStyle);
	DDX_Check(pDX, IDC_CHECK_REMOVE_ISLAND, m_bCheckRemoveIsland);
	DDX_Text(pDX, IDC_EDIT_ARC_PRECISION, m_csEditArcPrecision);
	DDX_Check(pDX, IDC_CHECK_REMOVE_NECK, m_bCheckRemoveNeck);
	DDX_Text(pDX, IDC_EDIT_REMOVE_NECK, m_csEditRemoveNeck);
	DDX_Text(pDX, IDC_EDIT_TRACK_WIDTH, m_csEditTrackWidth);
	DDX_Text(pDX, IDC_EDIT_GRID_SIZE, m_csEditGridSize);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nComboLayer);
	DDX_Text(pDX, IDC_EDIT_REMOVE_ISLAND, m_nEditRemoveIsland);
	DDX_Text(pDX, IDC_EDIT_NAME, m_csName);
	DDX_Text(pDX, IDC_EDIT_MIN_LENGTH, m_csEditMinLength);
	DDX_Check(pDX, IDC_CHECK_PRIMITIVE_LOCK, m_bCheckPrimitiveLock);
	DDX_Check(pDX, IDC_CHECK_LOCKED, m_bCheckLocked);
	DDX_Check(pDX, IDC_CHECK_IGNORE_VIOLATION, m_bCheckIgnoreViolation);
	DDX_CBIndex(pDX, IDC_COMBO_POUR_STYLE, m_nComboPourStyle);
	DDX_Check(pDX, IDC_CHECK_REMOVE_DEAD, m_bCheckRemoveDead);
	DDX_CBString(pDX, IDC_COMBO_NET, m_csComboNet);
	DDX_Text(pDX, IDC_EDIT_POUR_GAP, m_csEditPourGap);
}


BEGIN_MESSAGE_MAP(CDlg_PCBEditPolygon, CDialog)
	ON_BN_CLICKED(IDC_RADIO_FILLMODE1, OnBnClickedRadioFillmode1)
	ON_BN_CLICKED(IDC_RADIO_FILLMODE2, OnBnClickedRadioFillmode2)
	ON_BN_CLICKED(IDC_RADIO_FILLMODE3, OnBnClickedRadioFillmode3)
	ON_BN_CLICKED(IDC_CHECK_REMOVE_ISLAND, OnBnClickedCheckRemoveIsland)
	ON_BN_CLICKED(IDC_CHECK_REMOVE_NECK, OnBnClickedCheckRemoveNeck)
END_MESSAGE_MAP()


// CDlg_PCBEditPolygon 消息处理程序

BOOL CDlg_PCBEditPolygon::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_Bitmap1_1.SetBitmap(IDB_PCB_EDIT_POLYGON1_1);
	m_Bitmap1_2.SetBitmap(IDB_PCB_EDIT_POLYGON1_2);
	m_Bitmap1_3.SetBitmap(IDB_PCB_EDIT_POLYGON1_3);
	m_Bitmap1_4.SetBitmap(IDB_PCB_EDIT_POLYGON1_4);
	m_Bitmap2.SetBitmap(IDB_PCB_EDIT_POLYGON2);
	m_Bitmap3.SetBitmap(IDB_PCB_EDIT_POLYGON3);

	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_1);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_3);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_4);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON3);
	pWnd->ShowWindow(FALSE);
	
	if(m_nFillMode == 0)
	{
		if((m_bCheckRemoveIsland == TRUE) && (m_bCheckRemoveNeck == TRUE))
		{
			pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_1);
			pWnd->ShowWindow(TRUE);
		}
		else if((m_bCheckRemoveIsland == TRUE) && (m_bCheckRemoveNeck == FALSE))
		{
			pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_2);
			pWnd->ShowWindow(TRUE);
		}
		else if((m_bCheckRemoveIsland == FALSE) && (m_bCheckRemoveNeck == TRUE))
		{
			pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_3);
			pWnd->ShowWindow(TRUE);
		}
		else if((m_bCheckRemoveIsland == FALSE) && (m_bCheckRemoveNeck == FALSE))
		{
			pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_4);
			pWnd->ShowWindow(TRUE);
		}

		pWnd = GetDlgItem(IDC_STATIC_TRACK_WIDTH);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_TRACK_WIDTH);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_GRID_SIZE);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_GRID_SIZE);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_PAD_STYLE);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_PAD_STYLE1);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_PAD_STYLE2);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_HATCH_STYLE);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE1);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE2);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE3);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE4);
		pWnd->ShowWindow(FALSE);	
	}
	else if(m_nFillMode == 1)
	{
		pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON2);
		pWnd->ShowWindow(TRUE);

		pWnd = GetDlgItem(IDC_STATIC_REMOVE_ISLAND);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_CHECK_REMOVE_ISLAND);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_REMOVE_ISLAND);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_ARC_PRECISION);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_ARC_PRECISION);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_CHECK_REMOVE_NECK);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_REMOVE_NECK);
		pWnd->ShowWindow(FALSE);
	}
	else if(m_nFillMode == 2)
	{
		pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON3);
		pWnd->ShowWindow(TRUE);

		pWnd = GetDlgItem(IDC_STATIC_REMOVE_ISLAND);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_CHECK_REMOVE_ISLAND);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_REMOVE_ISLAND);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_ARC_PRECISION);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_ARC_PRECISION);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_CHECK_REMOVE_NECK);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_REMOVE_NECK);
		pWnd->ShowWindow(FALSE);
		//灰化GridSize HatchMode
		pWnd = GetDlgItem(IDC_STATIC_GRID_SIZE);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_GRID_SIZE);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_HATCH_STYLE);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE1);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE2);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE3);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE4);
		pWnd->EnableWindow(FALSE);
	}


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

void CDlg_PCBEditPolygon::OnBnClickedRadioFillmode1()
{
	CWnd* pWnd;

	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_REMOVE_ISLAND);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_1);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_3);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_4);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON3);
	pWnd->ShowWindow(FALSE);
		
	if((m_bCheckRemoveIsland == TRUE) && (m_bCheckRemoveNeck == TRUE))
	{
		pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_1);
		pWnd->ShowWindow(TRUE);
	}
	else if((m_bCheckRemoveIsland == TRUE) && (m_bCheckRemoveNeck == FALSE))
	{
		pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_2);
		pWnd->ShowWindow(TRUE);
	}
	else if((m_bCheckRemoveIsland == FALSE) && (m_bCheckRemoveNeck == TRUE))
	{
		pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_3);
		pWnd->ShowWindow(TRUE);
	}
	else if((m_bCheckRemoveIsland == FALSE) && (m_bCheckRemoveNeck == FALSE))
	{
		pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_4);
		pWnd->ShowWindow(TRUE);
	}

	pWnd = GetDlgItem(IDC_STATIC_TRACK_WIDTH);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_TRACK_WIDTH);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_GRID_SIZE);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_GRID_SIZE);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_PAD_STYLE);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_PAD_STYLE1);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_PAD_STYLE2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_HATCH_STYLE);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE1);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE3);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE4);
	pWnd->ShowWindow(FALSE);
		

	pWnd = GetDlgItem(IDC_STATIC_REMOVE_ISLAND);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_CHECK_REMOVE_ISLAND);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_REMOVE_ISLAND);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_ARC_PRECISION);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_ARC_PRECISION);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_CHECK_REMOVE_NECK);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_REMOVE_NECK);
	pWnd->ShowWindow(TRUE);
}

void CDlg_PCBEditPolygon::OnBnClickedRadioFillmode2()
{
	CWnd* pWnd;

	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_REMOVE_ISLAND);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_1);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_3);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_4);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON3);
	pWnd->ShowWindow(FALSE);
		
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON2);
	pWnd->ShowWindow(TRUE);		


	pWnd = GetDlgItem(IDC_STATIC_TRACK_WIDTH);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_TRACK_WIDTH);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_GRID_SIZE);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_GRID_SIZE);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_PAD_STYLE);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_PAD_STYLE1);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_PAD_STYLE2);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_HATCH_STYLE);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE1);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE2);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE3);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE4);
	pWnd->ShowWindow(TRUE);
		

	pWnd = GetDlgItem(IDC_STATIC_GRID_SIZE);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_GRID_SIZE);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_HATCH_STYLE);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE1);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE2);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE3);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE4);
	pWnd->EnableWindow(TRUE);


	pWnd = GetDlgItem(IDC_STATIC_REMOVE_ISLAND);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_CHECK_REMOVE_ISLAND);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_REMOVE_ISLAND);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_ARC_PRECISION);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_ARC_PRECISION);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_CHECK_REMOVE_NECK);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_REMOVE_NECK);
	pWnd->ShowWindow(FALSE);
}

void CDlg_PCBEditPolygon::OnBnClickedRadioFillmode3()
{
	CWnd* pWnd;

	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_REMOVE_ISLAND);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_1);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_3);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_4);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON3);
	pWnd->ShowWindow(FALSE);
		
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON3);
	pWnd->ShowWindow(TRUE);		

	pWnd = GetDlgItem(IDC_STATIC_TRACK_WIDTH);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_TRACK_WIDTH);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_GRID_SIZE);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_GRID_SIZE);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_PAD_STYLE);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_PAD_STYLE1);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_PAD_STYLE2);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_HATCH_STYLE);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE1);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE2);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE3);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE4);
	pWnd->ShowWindow(TRUE);
		
	pWnd = GetDlgItem(IDC_STATIC_GRID_SIZE);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_GRID_SIZE);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_HATCH_STYLE);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE1);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE2);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE3);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_HATCH_STYLE4);
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_STATIC_REMOVE_ISLAND);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_CHECK_REMOVE_ISLAND);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_REMOVE_ISLAND);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_ARC_PRECISION);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_ARC_PRECISION);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_CHECK_REMOVE_NECK);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_REMOVE_NECK);
	pWnd->ShowWindow(FALSE);
}

void CDlg_PCBEditPolygon::OnBnClickedCheckRemoveIsland()
{
	CWnd* pWnd;

	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_REMOVE_ISLAND);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_1);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_3);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_4);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON3);
	pWnd->ShowWindow(FALSE);
	if(pButton->GetCheck() == BST_UNCHECKED)
	{
		if(m_bCheckRemoveNeck == TRUE)
		{
			pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_3);
			pWnd->ShowWindow(TRUE);
		}
		else
		{
			pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_4);
			pWnd->ShowWindow(TRUE);
		}
	}
	else 
	{
		if(m_bCheckRemoveNeck == TRUE)
		{
			pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_1);
			pWnd->ShowWindow(TRUE);
		}
		else
		{
			pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_2);
			pWnd->ShowWindow(TRUE);
		}
	}
}

void CDlg_PCBEditPolygon::OnBnClickedCheckRemoveNeck()
{
	CWnd* pWnd;

	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_REMOVE_NECK);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_1);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_3);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_4);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON2);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON3);
	pWnd->ShowWindow(FALSE);
	if(pButton->GetCheck() == BST_UNCHECKED)
	{
		if(m_bCheckRemoveIsland == TRUE)
		{
			pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_2);
			pWnd->ShowWindow(TRUE);
		}

		else
		{
			pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_4);
			pWnd->ShowWindow(TRUE);
		}
	}
	else 
	{
		if(m_bCheckRemoveIsland == TRUE)
		{
			pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_1);
			pWnd->ShowWindow(TRUE);
		}
		else
		{
			pWnd = GetDlgItem(IDC_STATIC_EDIT_POLYGON1_3);
			pWnd->ShowWindow(TRUE);
		}
	}
}

// Dlg_Perference.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Perference.h"
#include ".\dlg_perference.h"


// CDlg_Perference 对话框

IMPLEMENT_DYNAMIC(CDlg_Perference, CDialog)
CDlg_Perference::CDlg_Perference(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Perference::IDD, pParent)
	, m_nComboCursor(0)
	, m_bCheckAutoJunc(FALSE)
	, m_bCheckAutoWire(FALSE)
	, m_bCheckAutoDiagram(FALSE)
	, m_bCheckBackup(FALSE)
	, m_nEditBackup(0)

	, m_csEditPowerGround(_T(""))
	, m_csEditSignalGround(_T(""))
	, m_csEditEarth(_T(""))
	, m_nComboGrid(0)

	, m_bCheckClearSel(FALSE)
	, m_nComboSheetSize(0)
	, m_fEditNameOffset(0)
	, m_fEditNumberOffset(0)
	, m_fEditI1(0)
	, m_fEditI2(0)
	, m_fEditI3(0)
	, m_fEditM1(0)
	, m_fEditM2(0)
	, m_fEditM3(0)
{
	m_nRadioIDSuffix = 0;
}

CDlg_Perference::~CDlg_Perference()
{
}

void CDlg_Perference::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_CURSOR, m_nComboCursor);
	DDX_Check(pDX, IDC_CHECK_AUTOJUNC, m_bCheckAutoJunc);
	DDX_Check(pDX, IDC_CHECK_AUTOWIRE, m_bCheckAutoWire);
	DDX_Check(pDX, IDC_CHECK_AUTODIAGRAM, m_bCheckAutoDiagram);
	DDX_Check(pDX, IDC_CHECK_BACKUP, m_bCheckBackup);
	DDX_Text(pDX, IDC_EDIT_BACKUP, m_nEditBackup);

	DDX_Text(pDX, IDC_EDIT_POWERGROUND, m_csEditPowerGround);
	DDX_Text(pDX, IDC_EDIT_SIGNALGROUND, m_csEditSignalGround);
	DDX_Text(pDX, IDC_EDIT_EARTH, m_csEditEarth);
	DDX_CBIndex(pDX, IDC_COMBO_GRIDTYPE, m_nComboGrid);

	DDX_Check(pDX, IDC_CHECK_CLEARSEL, m_bCheckClearSel);
	DDX_CBIndex(pDX, IDC_COMBO_SHEETSIZE, m_nComboSheetSize);
	DDX_Text(pDX, IDC_EDIT_NAMEOFFSET, m_fEditNameOffset);
	DDX_Text(pDX, IDC_EDIT_NUMBEROFFSET, m_fEditNumberOffset);
	DDX_Text(pDX, IDC_EDIT_I1, m_fEditI1);
	DDX_Text(pDX, IDC_EDIT_I2, m_fEditI2);
	DDX_Text(pDX, IDC_EDIT_I3, m_fEditI3);
	DDX_Text(pDX, IDC_EDIT_M1, m_fEditM1);
	DDX_Text(pDX, IDC_EDIT_M2, m_fEditM2);
	DDX_Text(pDX, IDC_EDIT_M3, m_fEditM3);

	DDX_Radio(pDX, IDC_RADIO_ID_SUFFIX, m_nRadioIDSuffix);
}


BEGIN_MESSAGE_MAP(CDlg_Perference, CDialog)
	ON_STN_CLICKED(IDC_STATIC_GRIDCOLOR, OnStnClickedStaticGridcolor)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnBnClickedButtonDefault)
	ON_STN_CLICKED(IDC_STATIC_AUTOJUNCCOLOR, OnStnClickedStaticAutojunccolor)
	ON_STN_CLICKED(IDC_STATIC_SELCOLOR, OnStnClickedStaticSelcolor)
END_MESSAGE_MAP()



BOOL CDlg_Perference::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cSelColor.SubclassDlgItem(IDC_STATIC_SELCOLOR, this);
	m_cSelColor.m_nColor = m_nSelColor;
	m_cGridColor.SubclassDlgItem(IDC_STATIC_GRIDCOLOR, this);
	m_cGridColor.m_nColor = m_nGridColor;
	m_cAutoJuncColor.SubclassDlgItem(IDC_STATIC_AUTOJUNCCOLOR, this);
	m_cAutoJuncColor.m_nColor = m_nAutoJuncColor;


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_Perference::OnBnClickedButtonDefault()
{
	if(AfxMessageBox("确认恢复?", MB_YESNO|MB_ICONWARNING) != IDYES) return;
	
	m_nComboCursor = 0;

	m_bCheckAutoJunc = TRUE;

	m_nAutoJuncColor = RGB(0,0,255);
	m_cAutoJuncColor.m_nColor = m_nAutoJuncColor;
	m_cAutoJuncColor.Invalidate();
	m_bCheckAutoWire = TRUE;
	m_bCheckAutoDiagram = TRUE;
	m_bCheckBackup = TRUE;
	m_nEditBackup = 30;


	m_fEditNameOffset = 50;
	m_fEditNumberOffset = 80;


	m_csEditPowerGround = "GND";
	m_csEditSignalGround = "SGND";
	m_csEditEarth = "EARTH";
	

	m_nComboGrid = 0;

	m_nGridColor = RGB(236,236,236);
	m_cGridColor.m_nColor = m_nGridColor;
	m_cGridColor.Invalidate();
	m_fEditI1 = 10.0f;
	m_fEditI2 = 50.0f;
	m_fEditI3 = 100.0f;
	
	double f = 39.370079f/1000*25.4;
	int i1 = (int)(f*1000);
	int i2 = (int)(f*10000 - i1*10);
	if(i2 >=5) i1++;

	f = (double)i1/1000;
	m_fEditM1 = f;
	f = 98.425198f/1000*25.4;
	i1 = (int)(f*1000);  
	i2 = (int)(f*10000 - i1*10);
	if(i2 >=5) i1++;

	f = (double)i1/1000;
	m_fEditM2 = f;
	f = 196.850395f/1000*25.4;
	i1 = (int)(f*1000);
	i2 = (int)(f*10000 - i1*10); if(i2 >=5) i1++;
	f = (double)i1/1000;
	m_fEditM3 = f;


	m_nSelColor = RGB(0,255,0);
	m_cSelColor.m_nColor = m_nSelColor;
	m_cSelColor.Invalidate();
	m_bCheckClearSel = TRUE;

	m_nComboSheetSize = 0;

	UpdateData(FALSE);
}

void CDlg_Perference::OnStnClickedStaticAutojunccolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cAutoJuncColor.m_nColor = colordialog.GetColor();
		m_nAutoJuncColor = m_cAutoJuncColor.m_nColor;
		m_cAutoJuncColor.Invalidate();
	}
}

void CDlg_Perference::OnStnClickedStaticSelcolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cSelColor.m_nColor = colordialog.GetColor();
		m_nSelColor = m_cSelColor.m_nColor;
		m_cSelColor.Invalidate();
	}
}

void CDlg_Perference::OnStnClickedStaticGridcolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cGridColor.m_nColor = colordialog.GetColor();
		m_nGridColor = m_cGridColor.m_nColor;
		m_cGridColor.Invalidate();
	}
}
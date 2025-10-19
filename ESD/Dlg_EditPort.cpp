// Dlg_EditPort.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditPort.h"
#include ".\dlg_editport.h"


// CDlg_EditPort 对话框

IMPLEMENT_DYNAMIC(CDlg_EditPort, CDialog)
CDlg_EditPort::CDlg_EditPort(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditPort::IDD, pParent)
	, m_csEditName(_T(""))
	, m_nComboType(0)
	, m_nComboStyle(0)
	, m_nComboAlignment(0)
	, m_csEditSize(_T(""))
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditPort::~CDlg_EditPort()
{
}

void CDlg_EditPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_PORT, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT_NAME, m_csEditName);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nComboType);
	DDX_CBIndex(pDX, IDC_COMBO_STYLE, m_nComboStyle);
	DDX_CBIndex(pDX, IDC_COMBO_ALIGNMENT, m_nComboAlignment);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_csEditSize);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditPort, CDialog)
	ON_STN_CLICKED(IDC_STATIC_TEXTCOLOR, OnStnClickedStaticTextcolor)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, OnStnClickedStaticFillcolor)
	ON_STN_CLICKED(IDC_STATIC_BORDERCOLOR, OnStnClickedStaticBordercolor)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, OnCbnSelchangeComboStyle)
END_MESSAGE_MAP()


// CDlg_EditPort 消息处理程序

BOOL CDlg_EditPort::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_PORT);

	m_cFillColor.SubclassDlgItem(IDC_STATIC_FILLCOLOR, this);
	m_cFillColor.m_nColor = m_nFillColor;
	m_cBorderColor.SubclassDlgItem(IDC_STATIC_BORDERCOLOR, this);
	m_cBorderColor.m_nColor = m_nBorderColor;
	m_cTextColor.SubclassDlgItem(IDC_STATIC_TEXTCOLOR, this);
	m_cTextColor.m_nColor = m_nTextColor;

	//对齐 alignment根据方向的不同，item不同
	if(m_nComboStyle > 3)
	{
		CComboBox* pCombo;
		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ALIGNMENT);
		if(pCombo != NULL) 
		{
			pCombo->DeleteString(1);
			pCombo->DeleteString(1);
			pCombo->InsertString(1, "下");
			pCombo->InsertString(1, "上");
			pCombo->SetCurSel(m_nComboAlignment);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditPort::OnStnClickedStaticTextcolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cTextColor.m_nColor = colordialog.GetColor();
		m_nTextColor = m_cTextColor.m_nColor;
		m_cTextColor.Invalidate();
	}
}

void CDlg_EditPort::OnStnClickedStaticFillcolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cFillColor.m_nColor = colordialog.GetColor();
		m_nFillColor = m_cFillColor.m_nColor;
		m_cFillColor.Invalidate();
	}
}

void CDlg_EditPort::OnStnClickedStaticBordercolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cBorderColor.m_nColor = colordialog.GetColor();
		m_nBorderColor = m_cBorderColor.m_nColor;
		m_cBorderColor.Invalidate();
	}
}

void CDlg_EditPort::OnCbnSelchangeComboStyle()
{
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_STYLE);
	if(pCombo->GetCurSel() > 3)
	{
		CComboBox* pCombo;
		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ALIGNMENT);
		if(pCombo != NULL) 
		{
			pCombo->DeleteString(1);
			pCombo->DeleteString(1);
			pCombo->InsertString(1, "下");
			pCombo->InsertString(1, "上");
			pCombo->SetCurSel(m_nComboAlignment);
		}
	}
	else
	{
		CComboBox* pCombo;
		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ALIGNMENT);
		if(pCombo != NULL) 
		{
			pCombo->DeleteString(1);
			pCombo->DeleteString(1);
			pCombo->InsertString(1, "右");
			pCombo->InsertString(1, "左");
			pCombo->SetCurSel(m_nComboAlignment);
		}
	}
}

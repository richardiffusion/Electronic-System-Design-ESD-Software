// Dlg_SchLib_New.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_SchLib_New.h"
#include ".\dlg_schlib_new.h"


// CDlg_SchLib_New 对话框

IMPLEMENT_DYNAMIC(CDlg_SchLib_New, CDialog)
CDlg_SchLib_New::CDlg_SchLib_New(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_SchLib_New::IDD, pParent)
	, m_nEditPartNum(0)
	, m_nEditModeNum(0)
	, m_nComboPart(0)
	, m_nComboMode(0)
	, m_nEditPinNum(0)
	, m_fEditX(0)
	, m_fEditY(0)
	, m_nComboPinPosition(0)
	, m_fEditPinLength(0)
	, m_fEditWidth(0)
{
	m_nRadioPart = 0;
	m_nRadioMode = 0;
	m_nEditPartNum = 2;
	m_nEditModeNum = 2;
	m_nComboPart = 0;
	m_nComboMode = 0;
	m_nComboPinPosition = 0;
}

CDlg_SchLib_New::~CDlg_SchLib_New()
{
}

void CDlg_SchLib_New::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PARTNUM, m_nEditPartNum);
	DDV_MinMaxInt(pDX, m_nEditPartNum, 2, 26);
	DDX_Text(pDX, IDC_EDIT_MODENUM, m_nEditModeNum);
	DDV_MinMaxInt(pDX, m_nEditModeNum, 2, 5);
	DDX_CBIndex(pDX, IDC_COMBO_PART, m_nComboPart);
	DDX_CBIndex(pDX, IDC_COMBO_MODE, m_nComboMode);
	DDX_Text(pDX, IDC_EDIT_PINNUM, m_nEditPinNum);
	DDX_Radio(pDX, IDC_RADIO_PART, m_nRadioPart);
	DDX_Radio(pDX, IDC_RADIO_MODE, m_nRadioMode);
	DDX_Text(pDX, IDC_EDIT_X, m_fEditX);
	DDV_MinMaxDouble(pDX, m_fEditX, 0, 65000);
	DDX_Text(pDX, IDC_EDIT_Y, m_fEditY);
	DDV_MinMaxDouble(pDX, m_fEditY, 0, 65000);
	DDX_CBIndex(pDX, IDC_COMBO_PINPOSITION, m_nComboPinPosition);
	DDX_Text(pDX, IDC_EDIT_PINLENGTH, m_fEditPinLength);
	DDV_MinMaxDouble(pDX, m_fEditPinLength, 50, 500);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_fEditWidth);
	DDV_MinMaxDouble(pDX, m_fEditWidth, 100, 10000);
}


BEGIN_MESSAGE_MAP(CDlg_SchLib_New, CDialog)
	ON_BN_CLICKED(IDC_RADIO_MULTIMODE, OnBnClickedRadioMultimode)
	ON_BN_CLICKED(IDC_RADIO_MULTIPART, OnBnClickedRadioMultipart)
	ON_BN_CLICKED(IDC_RADIO_MODE, OnBnClickedRadioMode)
	ON_BN_CLICKED(IDC_RADIO_PART, OnBnClickedRadioPart)
	ON_EN_CHANGE(IDC_EDIT_PARTNUM, OnEnChangeEditPartnum)
	ON_EN_CHANGE(IDC_EDIT_MODENUM, OnEnChangeEditModenum)
END_MESSAGE_MAP()


// CDlg_SchLib_New 消息处理程序

BOOL CDlg_SchLib_New::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_EDIT_PARTNUM);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_MODENUM);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_COMBO_PART);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_COMBO_MODE);
	pWnd->EnableWindow(FALSE);

	OnEnChangeEditPartnum();
	OnEnChangeEditModenum();
	
	m_fEditX = 1000;
	m_fEditY = sheet_height - 1000;
	m_fEditPinLength = 300;
	m_fEditWidth = 600;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_SchLib_New::OnBnClickedRadioMultimode()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_EDIT_MODENUM);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_COMBO_MODE);
	pWnd->EnableWindow(TRUE);
}

void CDlg_SchLib_New::OnBnClickedRadioMultipart()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_EDIT_PARTNUM);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_COMBO_PART);
	pWnd->EnableWindow(TRUE);
	
}

void CDlg_SchLib_New::OnBnClickedRadioMode()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_EDIT_MODENUM);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_COMBO_MODE);
	pWnd->EnableWindow(FALSE);
}

void CDlg_SchLib_New::OnBnClickedRadioPart()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_EDIT_PARTNUM);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_COMBO_PART);
	pWnd->EnableWindow(FALSE);
}

void CDlg_SchLib_New::OnEnChangeEditPartnum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	CWnd* pWnd = GetDlgItem(IDC_EDIT_PARTNUM);
	CString cstring;
	pWnd->GetWindowText(cstring);
	
	int partnum = atoi(cstring);
	if(partnum > 26) partnum = 26;
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PART);
	pCombo->ResetContent();
	for(int i=0; i<partnum; i++)
	{
		char s[2];
		s[0] = 65+i; s[1] = '\0';
		//CString str = s;
		pCombo->AddString(s);
	}
	if(pCombo->GetCount() > 0) pCombo->SetCurSel(0);
}

void CDlg_SchLib_New::OnEnChangeEditModenum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	CWnd* pWnd = GetDlgItem(IDC_EDIT_MODENUM);
	CString cstring;
	pWnd->GetWindowText(cstring);
	
	int modenum = atoi(cstring);
	if(modenum > 5) modenum = 5;
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MODE);
	pCombo->ResetContent();
	for(int i=0; i<modenum; i++)
	{
		if(i==0) pCombo->AddString("标准");
		else
		{
			CString str;
			str.Format("可选%d", i);
			pCombo->AddString(str);
		}
	}
	if(pCombo->GetCount() > 0) pCombo->SetCurSel(0);
}

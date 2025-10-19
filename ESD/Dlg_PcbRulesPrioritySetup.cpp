// Dlg_PcbRulesPrioritySetup.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "ESDDoc.h"
#include "Dlg_PcbRulesPrioritySetup.h"
#include ".\dlg_pcbrulesprioritysetup.h"


// CDlg_PcbRulesPrioritySetup 对话框

IMPLEMENT_DYNAMIC(CDlg_PcbRulesPrioritySetup, CDialog)
CDlg_PcbRulesPrioritySetup::CDlg_PcbRulesPrioritySetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PcbRulesPrioritySetup::IDD, pParent)
	, m_nEditPriority(50)
	, m_csComboName(_T("未定义"))
	, m_nComboType(0)
{
}

CDlg_PcbRulesPrioritySetup::~CDlg_PcbRulesPrioritySetup()
{
}

void CDlg_PcbRulesPrioritySetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PRIORITY, m_nEditPriority);
	DDV_MinMaxInt(pDX, m_nEditPriority, 0, 100);
	DDX_CBString(pDX, IDC_COMBO_NAME, m_csComboName);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nComboType);
}


BEGIN_MESSAGE_MAP(CDlg_PcbRulesPrioritySetup, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnCbnSelchangeComboType)
END_MESSAGE_MAP()


// CDlg_PcbRulesPrioritySetup 消息处理程序

BOOL CDlg_PcbRulesPrioritySetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_nComboType == 0)	GetNet();
	else if(m_nComboType == 1) GetComp();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlg_PcbRulesPrioritySetup::OnCbnSelchangeComboType()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	if(pCombo->GetCurSel() == 0)
	{
		m_csComboName = "未定义";
		m_nEditPriority = 50;
		GetNet();
	}
	else if(pCombo->GetCurSel() == 1)
	{
		m_csComboName = "未定义";
		m_nEditPriority = 50;
		GetComp();
	}
}

void CDlg_PcbRulesPrioritySetup::GetNet()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_NAME);
	pCombo->ResetContent();
	pCombo->AddString("未定义");
	pCombo->SetCurSel(0);
	
	int num = 1;
	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;

		pCombo->AddString(pnet->m_csNetName);
		if(pnet->m_csNetName == m_csComboName) pCombo->SetCurSel(num);
		num++;
	}
}

void CDlg_PcbRulesPrioritySetup::GetComp()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_NAME);
	pCombo->ResetContent();
	pCombo->AddString("未定义");
	pCombo->SetCurSel(0);

	int num = 1;
	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag != FLAG_NORMAL) continue;

		CString cstring = pcomp->GetPcbCompDesignator(i);
		pCombo->AddString(cstring);
		if(cstring == m_csComboName) pCombo->SetCurSel(num);
		num++;
	}
}
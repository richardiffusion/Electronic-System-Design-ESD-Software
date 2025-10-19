// Dlg_Report.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Report.h"
#include ".\dlg_report.h"


// CDlg_Report �Ի���

IMPLEMENT_DYNAMIC(CDlg_Report, CDialog)
CDlg_Report::CDlg_Report(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Report::IDD, pParent)
{
}

CDlg_Report::~CDlg_Report()
{
}

void CDlg_Report::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ReportList);
	CString m_TitleText;


}


BEGIN_MESSAGE_MAP(CDlg_Report, CDialog)
END_MESSAGE_MAP()


// CDlg_Report ��Ϣ�������

BOOL CDlg_Report::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_TitleText);
	for(int i = 0; i < m_arrayReport.GetCount(); i++)
	{
		m_ReportList.AddString(m_arrayReport.GetAt(i));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlg_Report::OnOK()
{
	m_nSelected = m_ReportList.GetCurSel();
	if(m_nSelected != LB_ERR) m_ReportList.GetText(m_nSelected, m_Selected_Text);

	CDialog::OnOK();
}

// Dlg_Missing_Component.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Missing_Component.h"
#include ".\dlg_missing_component.h"


// CDlg_Missing_Component 对话框

IMPLEMENT_DYNAMIC(CDlg_Missing_Component, CDialog)
CDlg_Missing_Component::CDlg_Missing_Component(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Missing_Component::IDD, pParent)
{
}

CDlg_Missing_Component::~CDlg_Missing_Component()
{
}

void CDlg_Missing_Component::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MISSING_COMP, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(CDlg_Missing_Component, CDialog)
END_MESSAGE_MAP()


// CDlg_Missing_Component 消息处理程序

BOOL CDlg_Missing_Component::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pwnd = GetDlgItem(IDC_STATIC_NUM);
	pwnd->SetWindowText(csNum);

	CXTPReportColumn* pColumnIndex = new CXTPReportColumn(0, "序号", "",40, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnIndex->SetEditable(FALSE);	
	pColumnIndex->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumnIndex);
	
	CXTPReportColumn* pColumn1 = new CXTPReportColumn(1, "丢失元件:名称", "", 150, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumn1->SetEditable(FALSE);	
	pColumn1->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumn1);
	
	CXTPReportColumn* pColumn2 = new CXTPReportColumn(2, "丢失元件:封装", "", 150, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumn2->SetEditable(FALSE);	
	pColumn2->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumn2);

	int totalnum = arrayCompID.GetCount();
	for(int i=0; i<totalnum; i++)
	{
		CXTPReportRecord* pRecord =  new CXTPReportRecord();
			
		pRecord->AddItem( new CXTPReportRecordItemNumber(i+1));
		
		pRecord->AddItem( new CXTPReportRecordItemText( arrayCompID.GetAt(i)));

		pRecord->AddItem( new CXTPReportRecordItemText(arrayCompP.GetAt(i)));

		m_wndReportCtrl.AddRecord(pRecord);
	}
	
	m_wndReportCtrl.SetMultipleSelection(FALSE);
	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndReportCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndReportCtrl.Populate();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

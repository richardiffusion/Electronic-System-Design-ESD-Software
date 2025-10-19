// Dlg_Missing_Component.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Missing_Component.h"
#include ".\dlg_missing_component.h"


// CDlg_Missing_Component �Ի���

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


// CDlg_Missing_Component ��Ϣ�������

BOOL CDlg_Missing_Component::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pwnd = GetDlgItem(IDC_STATIC_NUM);
	pwnd->SetWindowText(csNum);

	CXTPReportColumn* pColumnIndex = new CXTPReportColumn(0, "���", "",40, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnIndex->SetEditable(FALSE);	
	pColumnIndex->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumnIndex);
	
	CXTPReportColumn* pColumn1 = new CXTPReportColumn(1, "��ʧԪ��:����", "", 150, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumn1->SetEditable(FALSE);	
	pColumn1->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumn1);
	
	CXTPReportColumn* pColumn2 = new CXTPReportColumn(2, "��ʧԪ��:��װ", "", 150, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

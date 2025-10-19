// Dlg_PcbHighlightPins.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PcbHighlightPins.h"
#include ".\dlg_pcbhighlightpins.h"


// CDlg_PcbHighlightPins �Ի���

IMPLEMENT_DYNAMIC(CDlg_PcbHighlightPins, CDialog)
CDlg_PcbHighlightPins::CDlg_PcbHighlightPins(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PcbHighlightPins::IDD, pParent)
{
}

CDlg_PcbHighlightPins::~CDlg_PcbHighlightPins()
{
}

void CDlg_PcbHighlightPins::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PINS, m_wndPins);
}


BEGIN_MESSAGE_MAP(CDlg_PcbHighlightPins, CDialog)
END_MESSAGE_MAP()


// CDlg_PcbHighlightPins ��Ϣ�������

BOOL CDlg_PcbHighlightPins::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_wndPins.AddColumn(new CXTPReportColumn(0, "���", "",35, FALSE, XTP_REPORT_NOICON, FALSE, TRUE));

	m_wndPins.AddColumn(new CXTPReportColumn(1, "Ԫ������", "", 100, FALSE, XTP_REPORT_NOICON, TRUE, TRUE));

	m_wndPins.AddColumn(new CXTPReportColumn(2, "����", "", 35, FALSE, XTP_REPORT_NOICON, TRUE, TRUE));
	

	for(int i=0; i<compID.GetCount(); i++)
	{
		CXTPReportRecord* pRecord =  new CXTPReportRecord();

		CXTPReportRecordItemNumber* pRecordItem1 = new CXTPReportRecordItemNumber(i+1);
		pRecordItem1->SetBackgroundColor(RGB(230,230,230));
		pRecord->AddItem( pRecordItem1); //new CXTPReportRecordItemNumber(i+1));

		CXTPReportRecordItemText* pRecordItem2 = new CXTPReportRecordItemText(compID.GetAt(i));
		pRecord->AddItem(pRecordItem2);


		CXTPReportRecordItemText* pRecordItem3 = new CXTPReportRecordItemText(pinNum.GetAt(i));
		pRecord->AddItem(pRecordItem3);

		m_wndPins.AddRecord(pRecord);
	}
	m_wndPins.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndPins.AllowEdit(FALSE);
	m_wndPins.SetMultipleSelection(FALSE);
	m_wndPins.SetGridStyle(TRUE, xtpReportGridLargeDots);
	m_wndPins.SetGridStyle(FALSE, xtpReportGridLargeDots );
	m_wndPins.SelectionEnable(FALSE);
	m_wndPins.Populate();	

	CWnd* pWnd = GetDlgItem(IDC_STATIC_NET);
	if(pWnd != NULL) pWnd->SetWindowText(csNet);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

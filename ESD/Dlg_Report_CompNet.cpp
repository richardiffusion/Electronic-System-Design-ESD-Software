// Dlg_Report_CompNet.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "ESDDoc.h"
#include "Dlg_Report_CompNet.h"
#include ".\dlg_report_compnet.h"


// CDlg_Report_CompNet 对话框

IMPLEMENT_DYNAMIC(CDlg_Report_CompNet, CDialog)
CDlg_Report_CompNet::CDlg_Report_CompNet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Report_CompNet::IDD, pParent)
{
}

CDlg_Report_CompNet::~CDlg_Report_CompNet()
{
}

void CDlg_Report_CompNet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_NETLIST, m_wndReportNetList);
	DDX_Control(pDX, IDC_REPORT_NET, m_wndReportNet);
	DDX_Control(pDX, IDC_REPORT_COMP, m_wndReportComp);
}


BEGIN_MESSAGE_MAP(CDlg_Report_CompNet, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_NETLIST, OnReportItemClick)
END_MESSAGE_MAP()


// CDlg_Report_CompNet 消息处理程序

BOOL CDlg_Report_CompNet::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_wndReportNetList.AddColumn(new CXTPReportColumn(0, "序号", "",35, FALSE, XTP_REPORT_NOICON, FALSE, TRUE));

	m_wndReportNetList.AddColumn(new CXTPReportColumn(1, "网络名称", "", 120, FALSE, XTP_REPORT_NOICON, TRUE, TRUE));
	

	for(int i=0; i<pDoc->m_arraySchNet.GetCount(); i++)
	{
		COb_schnet* pschnet = pDoc->m_arraySchNet.GetAt(i);

		CXTPReportRecord* pRecord =  new CXTPReportRecord();
		//序号
		CXTPReportRecordItemNumber* pRecordItem1 = new CXTPReportRecordItemNumber(i+1);
		pRecordItem1->SetBackgroundColor(RGB(230,230,230));
		pRecord->AddItem( pRecordItem1); //new CXTPReportRecordItemNumber(i+1));
		//网络名称
		CXTPReportRecordItemText* pRecordItem2 = new CXTPReportRecordItemText(pschnet->m_csNetName);
		pRecord->AddItem(pRecordItem2);
		m_wndReportNetList.AddRecord(pRecord);
	}

	m_wndReportNetList.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReportNetList.AllowEdit(FALSE);
	//m_wndReportNetList.FocusSubItems(TRUE);
	m_wndReportNetList.SetMultipleSelection(FALSE);
	m_wndReportNetList.SetGridStyle(TRUE, xtpReportGridLargeDots);
	m_wndReportNetList.SetGridStyle(FALSE, xtpReportGridLargeDots );
	m_wndReportNetList.SelectionEnable(FALSE);
	m_wndReportNetList.Populate();

	m_wndReportNet.AddColumn(new CXTPReportColumn(0, "序号", "",35, FALSE, XTP_REPORT_NOICON, FALSE, TRUE));

	m_wndReportNet.AddColumn(new CXTPReportColumn(1, "元件名称", "", 100, FALSE, XTP_REPORT_NOICON, TRUE, TRUE));

	m_wndReportNet.AddColumn(new CXTPReportColumn(2, "引脚", "", 35, FALSE, XTP_REPORT_NOICON, TRUE, TRUE));
	

	COb_schnet* pschnet = pDoc->m_arraySchNet.GetAt(0);
	for(int i=0; i<pschnet->m_arrayCompDesignator.GetCount(); i++)
	{
		CXTPReportRecord* pRecord =  new CXTPReportRecord();

		CXTPReportRecordItemNumber* pRecordItem1 = new CXTPReportRecordItemNumber(i+1);
		pRecordItem1->SetBackgroundColor(RGB(230,230,230));
		pRecord->AddItem( pRecordItem1); //new CXTPReportRecordItemNumber(i+1));

		CXTPReportRecordItemText* pRecordItem2 = new CXTPReportRecordItemText(pschnet->m_arrayCompDesignator.GetAt(i));
		pRecord->AddItem(pRecordItem2);


		CXTPReportRecordItemText* pRecordItem3 = new CXTPReportRecordItemText(pschnet->m_arrayPinNumber.GetAt(i));
		pRecord->AddItem(pRecordItem3);

		m_wndReportNet.AddRecord(pRecord);
	}
	m_wndReportNet.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReportNet.AllowEdit(FALSE);
	//m_wndReportNet.FocusSubItems(TRUE);
	m_wndReportNet.SetMultipleSelection(FALSE);
	m_wndReportNet.SetGridStyle(TRUE, xtpReportGridLargeDots);
	m_wndReportNet.SetGridStyle(FALSE, xtpReportGridLargeDots );
	m_wndReportNet.SelectionEnable(FALSE);
	m_wndReportNet.Populate();	


	m_wndReportComp.AddColumn(new CXTPReportColumn(0, "序号", "",35, FALSE, XTP_REPORT_NOICON, FALSE, TRUE));

	m_wndReportComp.AddColumn(new CXTPReportColumn(1, "名称", "", 100, FALSE, XTP_REPORT_NOICON, TRUE, TRUE));

	m_wndReportComp.AddColumn(new CXTPReportColumn(2, "注释", "", 100, FALSE, XTP_REPORT_NOICON, TRUE, TRUE));

	m_wndReportComp.AddColumn(new CXTPReportColumn(3, "封装", "", 100, FALSE, XTP_REPORT_NOICON, TRUE, TRUE));

	m_wndReportComp.AddColumn(new CXTPReportColumn(4, "封装库", "", 100, FALSE, XTP_REPORT_NOICON, TRUE, TRUE));


	for(int i=0; i<pDoc->m_arrayCompDesignator.GetCount(); i++)
	{
		CXTPReportRecord* pRecord =  new CXTPReportRecord();

		CXTPReportRecordItemNumber* pRecordItem1 = new CXTPReportRecordItemNumber(i+1);
		pRecordItem1->SetBackgroundColor(RGB(230,230,230));
		pRecord->AddItem( pRecordItem1);
		

		CXTPReportRecordItemText* pRecordItem2 = new CXTPReportRecordItemText(pDoc->m_arrayCompDesignator.GetAt(i));
		pRecord->AddItem(pRecordItem2);

		CXTPReportRecordItemText* pRecordItem3 = new CXTPReportRecordItemText(pDoc->m_arrayCompComment.GetAt(i));
		pRecord->AddItem(pRecordItem3);

		CXTPReportRecordItemText* pRecordItem4 = new CXTPReportRecordItemText(pDoc->m_arrayCompPackage.GetAt(i));
		pRecord->AddItem(pRecordItem4);

		CXTPReportRecordItemText* pRecordItem5 = new CXTPReportRecordItemText(pDoc->m_arrayPackageLibraryName.GetAt(i));
		pRecord->AddItem(pRecordItem5);
		m_wndReportComp.AddRecord(pRecord);
	}
	m_wndReportComp.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReportComp.AllowEdit(FALSE);
	m_wndReportComp.SetMultipleSelection(FALSE);
	m_wndReportComp.SetGridStyle(TRUE, xtpReportGridLargeDots);
	m_wndReportComp.SetGridStyle(FALSE, xtpReportGridLargeDots );
	m_wndReportComp.SelectionEnable(FALSE);
	m_wndReportComp.Populate();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_Report_CompNet::OnReportItemClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*struct XTP_NM_REPORTRECORDITEM
	{
		NMHDR hdr;                          // Standard structure, containing information about a notification message.
		CXTPReportRow* pRow;                // Pointer to the row associated with the notification.
		CXTPReportRecordItem* pItem;        // Pointer to the record item associated with the notification.
		CXTPReportColumn* pColumn;          // Pointer to the column associated with the notification.
		int   nHyperlink;                   // Index of clicked Hyperlink, if any, or -1 otherwise.
		POINT pt;                           // Point where the message has happened.
	};*/

	XTP_NM_REPORTREQUESTEDIT* nm = (XTP_NM_REPORTREQUESTEDIT*)pNMHDR;
	CXTPReportRow* pRow = nm->pRow;
	if(pRow == NULL) return;

	CXTPReportRecord* pRecord = pRow->GetRecord();

	CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
	CString csNetName = pItemText->GetValue();
	
	for(int i=0; i<pDoc->m_arraySchNet.GetCount(); i++)
	{
		COb_schnet* pschnet = pDoc->m_arraySchNet.GetAt(i);
		if(pschnet->m_csNetName == csNetName)
		{
			m_wndReportNet.ResetContent();
			for(int j=0; j<pschnet->m_arrayCompDesignator.GetCount(); j++)
			{
				CXTPReportRecord* pRecord =  new CXTPReportRecord();

				CXTPReportRecordItemNumber* pRecordItem1 = new CXTPReportRecordItemNumber(j+1);
				pRecordItem1->SetBackgroundColor(RGB(230,230,230));
				pRecord->AddItem( pRecordItem1);

				CXTPReportRecordItemText* pRecordItem2 = new CXTPReportRecordItemText(pschnet->m_arrayCompDesignator.GetAt(j));
				pRecord->AddItem(pRecordItem2);


				CXTPReportRecordItemText* pRecordItem3 = new CXTPReportRecordItemText(pschnet->m_arrayPinNumber.GetAt(j));
				pRecord->AddItem(pRecordItem3);

				m_wndReportNet.AddRecord(pRecord);
			}
			m_wndReportNet.Populate();	
		}
	}

	*pResult = 0;
}
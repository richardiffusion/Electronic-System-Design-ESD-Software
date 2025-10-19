#include "stdafx.h"
#include "ESD.h"
#include "ESDDoc.h"
#include "Page_PcbRules3.h"
#include ".\page_pcbrules3.h"

#include "Dlg_PcbRulesPrioritySetup.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char* typestr[4] = { "禁止布线", "水平布线", "垂直布线" ,"任意角度"};
char* objectstr[2] = { "网络", "元件"};

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules3 property page

IMPLEMENT_DYNCREATE(CPage_PcbRules3, CXTPPropertyPage)

CPage_PcbRules3::CPage_PcbRules3() : CXTPPropertyPage(CPage_PcbRules3::IDD)
, m_csEditSetBack(_T(""))
, m_nComboTopology(0)
, m_nComboStyle(0)
{
	//{{AFX_DATA_INIT(CPage_PcbRules3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPage_PcbRules3::~CPage_PcbRules3()
{
}

void CPage_PcbRules3::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CORNER1, m_Bitmap1);
	DDX_Control(pDX, IDC_STATIC_CORNER2, m_Bitmap2);
	DDX_Control(pDX, IDC_STATIC_CORNER3, m_Bitmap3);
	//{{AFX_DATA_MAP(CPage_PcbRules3)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SETBACK, m_csEditSetBack);
	DDX_CBIndex(pDX, IDC_COMBO_TOPOLOGY, m_nComboTopology);
	DDX_CBIndex(pDX, IDC_COMBO_STYLE, m_nComboStyle);
	DDX_Control(pDX, IDC_PCB_RULES_LAYER, m_wndReportCtrl);
	DDX_Control(pDX, IDC_PCB_RULES_PRIORITY, m_wndReportCtrl_Priority);
}


BEGIN_MESSAGE_MAP(CPage_PcbRules3, CXTPPropertyPage)
	//{{AFX_MSG_MAP(CPage_PcbRules3)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, OnCbnSelchangeComboStyle)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules3 message handlers

BOOL CPage_PcbRules3::OnInitDialog() 
{
	CXTPPropertyPage::OnInitDialog();

	m_Bitmap1.SetBitmap(IDB_PCB_RULES_CORNER1);
	m_Bitmap2.SetBitmap(IDB_PCB_RULES_CORNER2);
	m_Bitmap3.SetBitmap(IDB_PCB_RULES_CORNER3);

	CWnd* pWnd;
	if(m_nComboStyle == 0)
	{
		pWnd = GetDlgItem(IDC_STATIC_CORNER1);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_CORNER2);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_CORNER3);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_SETBACK);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_SETBACK);
		pWnd->ShowWindow(FALSE);
	}
	else if(m_nComboStyle == 1)
	{
		pWnd = GetDlgItem(IDC_STATIC_CORNER1);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_CORNER2);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_CORNER3);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_SETBACK);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_SETBACK);
		pWnd->ShowWindow(TRUE);
	}
	else
	{
		pWnd = GetDlgItem(IDC_STATIC_CORNER1);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_CORNER2);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_CORNER3);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_SETBACK);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_SETBACK);
		pWnd->ShowWindow(TRUE);
	}

	CESDDoc* pDoc = (CESDDoc*)pDocument;

	//-----------------------------------------------------------------------  布线 layer
	//设计层名称
	CXTPReportColumn* pColumnLayer = new CXTPReportColumn(0, "设计层", "", 70, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnLayer->SetEditable(FALSE);	//不允许修改
	pColumnLayer->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumnLayer);
	
	//布线策略
	CXTPReportColumn* pColumnStyle = new CXTPReportColumn(1, "布线方式", "", 80, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	//pColumnStyle->SetEditable(FALSE);	//不允许修改
	pColumnStyle->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumnStyle);
	
	//布线策略 combo button 及选择项
	pColumnStyle->GetEditOptions()->AddConstraint(_T("禁止布线"), 0); 
	pColumnStyle->GetEditOptions()->AddConstraint(_T("水平布线"), 1);
	pColumnStyle->GetEditOptions()->AddConstraint(_T("垂直布线"), 2); 
	pColumnStyle->GetEditOptions()->AddConstraint(_T("任意角度"), 3);
	pColumnStyle->GetEditOptions()->m_bConstraintEdit = TRUE;
	pColumnStyle->GetEditOptions()->m_bAllowEdit = FALSE;
	pColumnStyle->GetEditOptions()->AddComboButton();

	//设计层编号
	CXTPReportColumn* pColumnLayerNum = new CXTPReportColumn(2, "层编号", "", 80, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnLayerNum->SetEditable(FALSE);	//不允许修改
	pColumnLayerNum->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumnLayerNum);

	int layer = 0;
	for(int i=0; i<PCB_LAYER_NUM; i++)
	{
		if(layer <= BOTTOMLAYER)
		{
			CXTPReportRecord* pRecord =  new CXTPReportRecord();
		
			//层名称
			pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[layer].csLayerName));

			//层设置
			pRecord->AddItem( new CXTPReportRecordItemText(typestr[pDoc->m_nRulesLayer[layer]]) );

			//层编号
			pRecord->AddItem( new CXTPReportRecordItemNumber(layer));

			m_wndReportCtrl.AddRecord(pRecord);
		}

		if(layer == BOTTOMLAYER ) break;
		layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
	}

	m_wndReportCtrl.SetMultipleSelection(FALSE);
	m_wndReportCtrl.AllowEdit(TRUE);
	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndReportCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndReportCtrl.Populate();

	//------------------------------------------------------------------------ 布线 网络 priority
	//对象
	CXTPReportColumn* pColumnObject = new CXTPReportColumn(0, "对象", "", 50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnObject->SetEditable(FALSE);	//不允许修改
	pColumnObject->SetAllowDrag(FALSE);
	pColumnObject->SetSortable(TRUE);
	m_wndReportCtrl_Priority.AddColumn(pColumnObject);
	

	//名称
	CXTPReportColumn* pColumnName = new CXTPReportColumn(1, "名称", "", 90, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnName->SetEditable(FALSE);	//不允许修改
	pColumnName->SetAllowDrag(FALSE);
	pColumnName->SetSortable(TRUE);
	m_wndReportCtrl_Priority.AddColumn(pColumnName);

	//优先级
	CXTPReportColumn* pColumnPriority = new CXTPReportColumn(2, "优先级", "", 80, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnPriority->SetEditable(FALSE);	//不允许修改
	pColumnPriority->SetAllowDrag(FALSE);
	pColumnPriority->SetSortable(TRUE);
	m_wndReportCtrl_Priority.AddColumn(pColumnPriority);

	for(int i=0; i<pDoc->m_arrayRulesPriority.GetCount(); i++)
	{
		CXTPReportRecord* pRecord =  new CXTPReportRecord();
		
		Struct_Route_Priority priority = pDoc->m_arrayRulesPriority.GetAt(i);
		//对象
		pRecord->AddItem( new CXTPReportRecordItemText(objectstr[priority.nType]));
		//名称
		pRecord->AddItem( new CXTPReportRecordItemText(priority.csName) );
		//优先级
		char str[100];
		itoa(priority.nPriority, str, 10);
		if(priority.nPriority == 0) strcat(str, "(最低)");
		else if(priority.nPriority == 100) strcat(str, "(最高)");
		pRecord->AddItem( new CXTPReportRecordItemText(str));

		m_wndReportCtrl_Priority.AddRecord(pRecord);
	}

	m_wndReportCtrl_Priority.SetMultipleSelection(FALSE);
	//m_wndReportCtrl_Priority.AllowEdit(TRUE);
	m_wndReportCtrl_Priority.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndReportCtrl_Priority.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndReportCtrl_Priority.Populate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPage_PcbRules3::OnCbnSelchangeComboStyle()
{
	UpdateData(TRUE);
	
	CWnd* pWnd;
	if(m_nComboStyle == 0)
	{
		pWnd = GetDlgItem(IDC_STATIC_CORNER1);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_CORNER2);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_CORNER3);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_SETBACK);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_SETBACK);
		pWnd->ShowWindow(FALSE);
	}
	else if(m_nComboStyle == 1)
	{
		pWnd = GetDlgItem(IDC_STATIC_CORNER1);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_CORNER2);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_CORNER3);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_SETBACK);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_SETBACK);
		pWnd->ShowWindow(TRUE);
	}
	else
	{
		pWnd = GetDlgItem(IDC_STATIC_CORNER1);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_CORNER2);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_CORNER3);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_SETBACK);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_SETBACK);
		pWnd->ShowWindow(TRUE);
	}
}

void CPage_PcbRules3::OnBnClickedButtonAdd()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_PcbRulesPrioritySetup dlg;
	dlg.pDocument = pDocument;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_csComboName != "未定义")
		{
			Struct_Route_Priority priority;
			priority.nType = dlg.m_nComboType;
			priority.csName = dlg.m_csComboName;
			priority.nPriority = dlg.m_nEditPriority;

			//对象
			CXTPReportRecord* pRecord =  new CXTPReportRecord();
			pRecord->AddItem( new CXTPReportRecordItemText(objectstr[priority.nType]));
			//名称
			pRecord->AddItem( new CXTPReportRecordItemText(priority.csName) );
			//优先级
			char str[100];
			itoa(priority.nPriority, str, 10);
			pRecord->AddItem( new CXTPReportRecordItemText(str));
			m_wndReportCtrl_Priority.AddRecord(pRecord);
			m_wndReportCtrl_Priority.Populate();
		}
	}
}

void CPage_PcbRules3::OnBnClickedButtonDelete()
{
	CXTPReportSelectedRows* pSelRows = m_wndReportCtrl_Priority.GetSelectedRows();
	if(pSelRows == NULL) return;
	
	CXTPReportRow* pRow = pSelRows->GetAt(0);
	if(pRow == NULL) return;


	m_wndReportCtrl_Priority.RemoveRowEx(pRow);
	m_wndReportCtrl_Priority.Populate();
}

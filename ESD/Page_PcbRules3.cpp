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

char* typestr[4] = { "��ֹ����", "ˮƽ����", "��ֱ����" ,"����Ƕ�"};
char* objectstr[2] = { "����", "Ԫ��"};

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

	//-----------------------------------------------------------------------  ���� layer
	//��Ʋ�����
	CXTPReportColumn* pColumnLayer = new CXTPReportColumn(0, "��Ʋ�", "", 70, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnLayer->SetEditable(FALSE);	//�������޸�
	pColumnLayer->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumnLayer);
	
	//���߲���
	CXTPReportColumn* pColumnStyle = new CXTPReportColumn(1, "���߷�ʽ", "", 80, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	//pColumnStyle->SetEditable(FALSE);	//�������޸�
	pColumnStyle->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumnStyle);
	
	//���߲��� combo button ��ѡ����
	pColumnStyle->GetEditOptions()->AddConstraint(_T("��ֹ����"), 0); 
	pColumnStyle->GetEditOptions()->AddConstraint(_T("ˮƽ����"), 1);
	pColumnStyle->GetEditOptions()->AddConstraint(_T("��ֱ����"), 2); 
	pColumnStyle->GetEditOptions()->AddConstraint(_T("����Ƕ�"), 3);
	pColumnStyle->GetEditOptions()->m_bConstraintEdit = TRUE;
	pColumnStyle->GetEditOptions()->m_bAllowEdit = FALSE;
	pColumnStyle->GetEditOptions()->AddComboButton();

	//��Ʋ���
	CXTPReportColumn* pColumnLayerNum = new CXTPReportColumn(2, "����", "", 80, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnLayerNum->SetEditable(FALSE);	//�������޸�
	pColumnLayerNum->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumnLayerNum);

	int layer = 0;
	for(int i=0; i<PCB_LAYER_NUM; i++)
	{
		if(layer <= BOTTOMLAYER)
		{
			CXTPReportRecord* pRecord =  new CXTPReportRecord();
		
			//������
			pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[layer].csLayerName));

			//������
			pRecord->AddItem( new CXTPReportRecordItemText(typestr[pDoc->m_nRulesLayer[layer]]) );

			//����
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

	//------------------------------------------------------------------------ ���� ���� priority
	//����
	CXTPReportColumn* pColumnObject = new CXTPReportColumn(0, "����", "", 50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnObject->SetEditable(FALSE);	//�������޸�
	pColumnObject->SetAllowDrag(FALSE);
	pColumnObject->SetSortable(TRUE);
	m_wndReportCtrl_Priority.AddColumn(pColumnObject);
	

	//����
	CXTPReportColumn* pColumnName = new CXTPReportColumn(1, "����", "", 90, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnName->SetEditable(FALSE);	//�������޸�
	pColumnName->SetAllowDrag(FALSE);
	pColumnName->SetSortable(TRUE);
	m_wndReportCtrl_Priority.AddColumn(pColumnName);

	//���ȼ�
	CXTPReportColumn* pColumnPriority = new CXTPReportColumn(2, "���ȼ�", "", 80, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnPriority->SetEditable(FALSE);	//�������޸�
	pColumnPriority->SetAllowDrag(FALSE);
	pColumnPriority->SetSortable(TRUE);
	m_wndReportCtrl_Priority.AddColumn(pColumnPriority);

	for(int i=0; i<pDoc->m_arrayRulesPriority.GetCount(); i++)
	{
		CXTPReportRecord* pRecord =  new CXTPReportRecord();
		
		Struct_Route_Priority priority = pDoc->m_arrayRulesPriority.GetAt(i);
		//����
		pRecord->AddItem( new CXTPReportRecordItemText(objectstr[priority.nType]));
		//����
		pRecord->AddItem( new CXTPReportRecordItemText(priority.csName) );
		//���ȼ�
		char str[100];
		itoa(priority.nPriority, str, 10);
		if(priority.nPriority == 0) strcat(str, "(���)");
		else if(priority.nPriority == 100) strcat(str, "(���)");
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
		if(dlg.m_csComboName != "δ����")
		{
			Struct_Route_Priority priority;
			priority.nType = dlg.m_nComboType;
			priority.csName = dlg.m_csComboName;
			priority.nPriority = dlg.m_nEditPriority;

			//����
			CXTPReportRecord* pRecord =  new CXTPReportRecord();
			pRecord->AddItem( new CXTPReportRecordItemText(objectstr[priority.nType]));
			//����
			pRecord->AddItem( new CXTPReportRecordItemText(priority.csName) );
			//���ȼ�
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

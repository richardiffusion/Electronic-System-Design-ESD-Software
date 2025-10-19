// Dlg_PCBLayerSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "ESDDoc.h"
#include "ReportCtrlItem.h"
#include "Dlg_PCBLayerSetup.h"
#include ".\dlg_pcblayersetup.h"
#include "systemext.h"


// CDlg_PCBLayerSetup 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBLayerSetup, CDialog)
CDlg_PCBLayerSetup::CDlg_PCBLayerSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBLayerSetup::IDD, pParent)
	, m_bCheckSingalLayerUseStack(FALSE)
	, m_bCheckPlaneUseStack(FALSE)
	, m_bCheckMechLayerUseStack(FALSE)
{
}

CDlg_PCBLayerSetup::~CDlg_PCBLayerSetup()
{
}

void CDlg_PCBLayerSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SINGAL_ALLON, m_btn_signal_allon);
	DDX_Control(pDX, IDC_SINGAL_ALLOFF, m_btn_signal_alloff);
	DDX_Control(pDX, IDC_SINGAL_USEDON, m_btn_signal_usedon);

	DDX_Control(pDX, IDC_PLANE_ALLON, m_btn_plane_allon);
	DDX_Control(pDX, IDC_PLANE_ALLOFF, m_btn_plane_alloff);
	DDX_Control(pDX, IDC_PLANE_USEDON, m_btn_plane_usedon);

	DDX_Control(pDX, IDC_MECH_ALLON, m_btn_mech_allon);
	DDX_Control(pDX, IDC_MECH_ALLOFF, m_btn_mech_alloff);
	DDX_Control(pDX, IDC_MECH_USEDON, m_btn_mech_usedon);

	DDX_Control(pDX, IDC_MASK_ALLON, m_btn_mask_allon);
	DDX_Control(pDX, IDC_MASK_ALLOFF, m_btn_mask_alloff);
	DDX_Control(pDX, IDC_MASK_USEDON, m_btn_mask_usedon);

	DDX_Control(pDX, IDC_OTHER_ALLON, m_btn_other_allon);
	DDX_Control(pDX, IDC_OTHER_ALLOFF, m_btn_other_alloff);
	DDX_Control(pDX, IDC_OTHER_USEDON, m_btn_other_usedon);

	DDX_Control(pDX, IDC_SILK_ALLON, m_btn_silk_allon);
	DDX_Control(pDX, IDC_SILK_ALLOFF, m_btn_silk_alloff);
	DDX_Control(pDX, IDC_SILK_USEDON, m_btn_silk_usedon);

	DDX_Control(pDX, IDC_CONTROL_ALLON, m_btn_control_allon);
	DDX_Control(pDX, IDC_CONTROL_ALLOFF, m_btn_control_alloff);
	//DDX_Control(pDX, IDC_SILK_USEDON, m_btn_silk_usedon);

	DDX_Control(pDX, IDC_ALLON, m_btn_allon);
	DDX_Control(pDX, IDC_ALLOFF, m_btn_alloff);
	DDX_Control(pDX, IDC_USEDON, m_btn_usedon);

	DDX_Control(pDX, IDC_SINGAL_LAYER, m_wndSignalCtrl);
	DDX_Control(pDX, IDC_MECH_LAYER, m_wndMechCtrl);
	DDX_Control(pDX, IDC_PLANE, m_wndPlaneCtrl);
	DDX_Control(pDX, IDC_MASK_LAYER, m_wndMaskCtrl);
	DDX_Control(pDX, IDC_OTHER_LAYER, m_wndOtherCtrl);
	DDX_Control(pDX, IDC_SILK_LAYER, m_wndSilkCtrl);
	DDX_Control(pDX, IDC_CONTROL_LAYER, m_wndSystemCtrl);
	DDX_Check(pDX, IDC_SINGAL_LAYER_USESTACK, m_bCheckSingalLayerUseStack);
	DDX_Check(pDX, IDC_PLANE_USESTACK, m_bCheckPlaneUseStack);
	DDX_Check(pDX, IDC_MECH_LAYER_USESTACK, m_bCheckMechLayerUseStack);
}


BEGIN_MESSAGE_MAP(CDlg_PCBLayerSetup, CDialog)
	ON_BN_CLICKED(IDC_MECH_LAYER_USESTACK, OnBnClickedMechLayerUsestack)
	ON_BN_CLICKED(IDC_SINGAL_LAYER_USESTACK, OnBnClickedSingalLayerUsestack)
	ON_BN_CLICKED(IDC_PLANE_USESTACK, OnBnClickedPlaneUsestack)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_SINGAL_ALLON, OnBnClickedSingalAllon)
	ON_BN_CLICKED(IDC_SINGAL_ALLOFF, OnBnClickedSingalAlloff)
	ON_BN_CLICKED(IDC_SINGAL_USEDON, OnBnClickedSingalUsedon)
	ON_BN_CLICKED(IDC_PLANE_ALLOFF, OnBnClickedPlaneAlloff)
	ON_BN_CLICKED(IDC_PLANE_ALLON, OnBnClickedPlaneAllon)
	ON_BN_CLICKED(IDC_PLANE_USEDON, OnBnClickedPlaneUsedon)
	ON_BN_CLICKED(IDC_MECH_ALLON, OnBnClickedMechAllon)
	ON_BN_CLICKED(IDC_MECH_ALLOFF, OnBnClickedMechAlloff)
	ON_BN_CLICKED(IDC_MECH_USEDON, OnBnClickedMechUsedon)
	ON_BN_CLICKED(IDC_MASK_ALLON, OnBnClickedMaskAllon)
	ON_BN_CLICKED(IDC_MASK_ALLOFF, OnBnClickedMaskAlloff)
	ON_BN_CLICKED(IDC_MASK_USEDON, OnBnClickedMaskUsedon)
	ON_BN_CLICKED(IDC_OTHER_ALLON, OnBnClickedOtherAllon)
	ON_BN_CLICKED(IDC_OTHER_ALLOFF, OnBnClickedOtherAlloff)
	ON_BN_CLICKED(IDC_OTHER_USEDON, OnBnClickedOtherUsedon)
	ON_BN_CLICKED(IDC_SILK_ALLON, OnBnClickedSilkAllon)
	ON_BN_CLICKED(IDC_SILK_ALLOFF, OnBnClickedSilkAlloff)
	ON_BN_CLICKED(IDC_SILK_USEDON, OnBnClickedSilkUsedon)
	ON_BN_CLICKED(IDC_CONTROL_ALLOFF, OnBnClickedControlAlloff)
	ON_BN_CLICKED(IDC_CONTROL_ALLON, OnBnClickedControlAllon)
	ON_BN_CLICKED(IDC_ALLOFF, OnBnClickedAlloff)
	ON_BN_CLICKED(IDC_ALLON, OnBnClickedAllon)
	ON_BN_CLICKED(IDC_USEDON, OnBnClickedUsedon)
END_MESSAGE_MAP()


// CDlg_PCBLayerSetup 消息处理程序

BOOL CDlg_PCBLayerSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btn_signal_allon.SetFlatStyle(TRUE);
	m_btn_signal_alloff.SetFlatStyle(TRUE);
	m_btn_signal_usedon.SetFlatStyle(TRUE);

	m_btn_plane_allon.SetFlatStyle(TRUE);
	m_btn_plane_alloff.SetFlatStyle(TRUE);
	m_btn_plane_usedon.SetFlatStyle(TRUE);

	m_btn_mech_allon.SetFlatStyle(TRUE);
	m_btn_mech_alloff.SetFlatStyle(TRUE);
	m_btn_mech_usedon.SetFlatStyle(TRUE);

	m_btn_mask_allon.SetFlatStyle(TRUE);
	m_btn_mask_alloff.SetFlatStyle(TRUE);
	m_btn_mask_usedon.SetFlatStyle(TRUE);

	m_btn_other_allon.SetFlatStyle(TRUE);
	m_btn_other_alloff.SetFlatStyle(TRUE);
	m_btn_other_usedon.SetFlatStyle(TRUE);

	m_btn_silk_allon.SetFlatStyle(TRUE);
	m_btn_silk_alloff.SetFlatStyle(TRUE);
	m_btn_silk_usedon.SetFlatStyle(TRUE);

	m_btn_control_allon.SetFlatStyle(TRUE);
	m_btn_control_alloff.SetFlatStyle(TRUE);
	//m_btn_silk_usedon.SetFlatStyle(TRUE);

	m_btn_allon.SetFlatStyle(TRUE);
	m_btn_alloff.SetFlatStyle(TRUE);
	m_btn_usedon.SetFlatStyle(TRUE);

	m_btn_signal_allon.SetTheme((XTPButtonTheme)4);
	m_btn_signal_alloff.SetTheme((XTPButtonTheme)4);
	m_btn_signal_usedon.SetTheme((XTPButtonTheme)4);

	m_btn_plane_allon.SetTheme((XTPButtonTheme)4);
	m_btn_plane_alloff.SetTheme((XTPButtonTheme)4);
	m_btn_plane_usedon.SetTheme((XTPButtonTheme)4);

	m_btn_mech_allon.SetTheme((XTPButtonTheme)4);
	m_btn_mech_alloff.SetTheme((XTPButtonTheme)4);
	m_btn_mech_usedon.SetTheme((XTPButtonTheme)4);

	m_btn_mask_allon.SetTheme((XTPButtonTheme)4);
	m_btn_mask_alloff.SetTheme((XTPButtonTheme)4);
	m_btn_mask_usedon.SetTheme((XTPButtonTheme)4);

	m_btn_other_allon.SetTheme((XTPButtonTheme)4);
	m_btn_other_alloff.SetTheme((XTPButtonTheme)4);
	m_btn_other_usedon.SetTheme((XTPButtonTheme)4);

	m_btn_silk_allon.SetTheme((XTPButtonTheme)4);
	m_btn_silk_alloff.SetTheme((XTPButtonTheme)4);
	m_btn_silk_usedon.SetTheme((XTPButtonTheme)4);

	m_btn_control_allon.SetTheme((XTPButtonTheme)4);
	m_btn_control_alloff.SetTheme((XTPButtonTheme)4);
	//m_btn_silk_usedon.SetTheme((XTPButtonTheme)4);

	m_btn_allon.SetTheme((XTPButtonTheme)4);
	m_btn_alloff.SetTheme((XTPButtonTheme)4);
	m_btn_usedon.SetTheme((XTPButtonTheme)4);

	SingalReportCtrl(m_bCheckSingalLayerUseStack, TRUE);
	PlaneReportCtrl(m_bCheckPlaneUseStack, TRUE);
	MechReportCtrl(m_bCheckMechLayerUseStack, TRUE);
	MaskReportCtrl();
	OtherReportCtrl();
	SilkReportCtrl();
	SystemReportCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_PCBLayerSetup::SingalReportCtrl(BOOL use_stack_layers_only, BOOL create_column)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(create_column == TRUE)
	{
		
		CXTPReportColumn* pColumnLayer = new CXTPReportColumn(0, "信号层", "",105, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnLayer->SetEditable(FALSE);	
		pColumnLayer->SetAllowDrag(FALSE);
		m_wndSignalCtrl.AddColumn(pColumnLayer);

		
		CXTPReportColumn* pColumnColor = new CXTPReportColumn(1, "颜色", "", 40, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnColor->SetEditable(FALSE);
		pColumnColor->SetAllowDrag(FALSE);
		m_wndSignalCtrl.AddColumn(pColumnColor);
		m_wndSignalCtrl.SetFocusedColumn(pColumnColor);

		
		CXTPReportColumn* pColumnShow = new CXTPReportColumn(2, "显示", "", 65, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnShow->SetAllowDrag(FALSE);
		pColumnShow->SetHeaderAlignment(DT_CENTER);
		m_wndSignalCtrl.AddColumn(pColumnShow);
	}

	if(use_stack_layers_only == TRUE)
	{
		int layer = 0;
		for(int i=0; i<PCB_LAYER_NUM; i++)
		{
			if(layer <= BOTTOMLAYER)
			{
				CXTPReportRecord* pRecord =  new CXTPReportRecord();
				
				
				pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[layer].csLayerName));
				
				CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
				pColor->SetBackgroundColor(nEDA_PCB_LayerColor[layer]);
				pRecord->AddItem(pColor);
				
				pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[layer].bLayerShow));

				m_wndSignalCtrl.AddRecord(pRecord);
			}

			if(layer == BOTTOMLAYER ) break;
			layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
		}
	}
	else
	{
		for(int i=0; i<SIGNAL_LAYER_NUM; i++)
		{
			CXTPReportRecord* pRecord =  new CXTPReportRecord();

			
			pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[i].csLayerName));
			
			CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
			pColor->SetBackgroundColor(nEDA_PCB_LayerColor[i]);
			pRecord->AddItem(pColor);
			
			pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[i].bLayerShow));

			m_wndSignalCtrl.AddRecord(pRecord);
		}
	}

	m_wndSignalCtrl.SetMultipleSelection(FALSE);
	m_wndSignalCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndSignalCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndSignalCtrl.GetPaintManager()->SetColumnStyle(XTPReportColumnStyle(1));

	RECT rect;
	m_wndSignalCtrl.GetWindowRect(&rect);
	int num = m_wndSignalCtrl.GetRecords()->GetCount();
	num = MIN(num, 16);
	//m_wndSignalCtrl.SetWindowPos(NULL, 0, 0, rect.right-rect.left, 22 + num*19,SWP_NOZORDER|SWP_NOMOVE);
	m_wndSignalCtrl.SetWindowPos(NULL, 0, 0, rect.right-rect.left, 28 + num*25,SWP_NOZORDER|SWP_NOMOVE);

	m_wndSignalCtrl.Populate();
}

void CDlg_PCBLayerSetup::PlaneReportCtrl(BOOL use_stack_layers_only, BOOL create_column)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(create_column == TRUE)
	{
		
		CXTPReportColumn* pColumnLayer = new CXTPReportColumn(0, "内平面", "",120, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnLayer->SetEditable(FALSE);	
		pColumnLayer->SetAllowDrag(FALSE);
		m_wndPlaneCtrl.AddColumn(pColumnLayer);

		CXTPReportColumn* pColumnColor = new CXTPReportColumn(1, "颜色", "", 40, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnColor->SetEditable(FALSE);	
		pColumnColor->SetAllowDrag(FALSE);
		m_wndPlaneCtrl.AddColumn(pColumnColor);
		m_wndPlaneCtrl.SetFocusedColumn(pColumnColor);

		CXTPReportColumn* pColumnShow = new CXTPReportColumn(2, "显示", "", 50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnShow->SetAllowDrag(FALSE);
		pColumnShow->SetHeaderAlignment(DT_CENTER);
		m_wndPlaneCtrl.AddColumn(pColumnShow);
	}

	if(use_stack_layers_only == TRUE)
	{
		int layer = 0;
		for(int i=0; i<PCB_LAYER_NUM; i++)
		{
			if((layer >= POWERPLANE1)&&(layer <= POWERPLANE16))
			{
				CXTPReportRecord* pRecord =  new CXTPReportRecord();
				
				pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[layer].csLayerName));
		
				CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
				pColor->SetBackgroundColor(nEDA_PCB_LayerColor[layer]);
				pRecord->AddItem(pColor);
		
				pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[layer].bLayerShow));

				m_wndPlaneCtrl.AddRecord(pRecord);
			}

			if(layer == BOTTOMLAYER ) break;
			layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
		}
	}
	else
	{
		for(int i=POWERPLANE1; i<=POWERPLANE16; i++)
		{
			CXTPReportRecord* pRecord =  new CXTPReportRecord();

			pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[i].csLayerName));

			CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
			pColor->SetBackgroundColor(nEDA_PCB_LayerColor[i]);
			pRecord->AddItem(pColor);

			pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[i].bLayerShow));

			m_wndPlaneCtrl.AddRecord(pRecord);
		}
	}

	m_wndPlaneCtrl.SetMultipleSelection(FALSE);
	m_wndPlaneCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndPlaneCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndPlaneCtrl.GetPaintManager()->SetColumnStyle(XTPReportColumnStyle(1));

	RECT rect;
	m_wndPlaneCtrl.GetWindowRect(&rect);
	int num = m_wndPlaneCtrl.GetRecords()->GetCount();
	num = MIN(num, 16);
	//m_wndPlaneCtrl.SetWindowPos(NULL, 0, 0, rect.right-rect.left, 22 + num*19,SWP_NOZORDER|SWP_NOMOVE);
	m_wndPlaneCtrl.SetWindowPos(NULL, 0, 0, rect.right-rect.left, 28 + num*25,SWP_NOZORDER|SWP_NOMOVE);

	m_wndPlaneCtrl.Populate();
}

void CDlg_PCBLayerSetup::MechReportCtrl(BOOL use_enable_layers_only, BOOL create_column)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(create_column == TRUE)
	{

		CXTPReportColumn* pColumnLayer = new CXTPReportColumn(0, "机械层", "",105, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnLayer->SetEditable(FALSE);	
		pColumnLayer->SetAllowDrag(FALSE);
		m_wndMechCtrl.AddColumn(pColumnLayer);

		
		CXTPReportColumn* pColumnColor = new CXTPReportColumn(1, "颜色", "", 40, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnColor->SetEditable(FALSE);	
		pColumnColor->SetAllowDrag(FALSE);
		m_wndMechCtrl.AddColumn(pColumnColor);
		m_wndMechCtrl.SetFocusedColumn(pColumnColor);

		
		CXTPReportColumn* pColumnShow = new CXTPReportColumn(2, "显示", "", 50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnShow->SetAllowDrag(FALSE);
		pColumnShow->SetHeaderAlignment(DT_CENTER);
		m_wndMechCtrl.AddColumn(pColumnShow);

		//Enable
		CXTPReportColumn* pColumnEnable = new CXTPReportColumn(3, "使用", "", 50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnEnable->SetAllowDrag(FALSE);
		pColumnEnable->SetHeaderAlignment(DT_CENTER);
		m_wndMechCtrl.AddColumn(pColumnEnable);

		//Linked to Sheet
		CXTPReportColumn* pColumnLink = new CXTPReportColumn(4, "链接到图框", "", 80, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnLink->SetAllowDrag(FALSE);
		pColumnLink->SetHeaderAlignment(DT_CENTER);
		m_wndMechCtrl.AddColumn(pColumnLink);
	}

	if(use_enable_layers_only == TRUE)
	{
		for(int i=MECHANICAL1; i<=MECHANICAL16; i++)
		{
			if(pDoc->m_sPCB_LayerParam[i].bLayerMechEnabled == TRUE)
			{
				CXTPReportRecord* pRecord =  new CXTPReportRecord();
				
				
				pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[i].csLayerName));
				
				CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
				pColor->SetBackgroundColor(nEDA_PCB_LayerColor[i]);
				pRecord->AddItem(pColor);
				
				pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[i].bLayerShow));
				
				pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[i].bLayerMechEnabled));
				
				pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[i].bLayerSheetLinked));
				m_wndMechCtrl.AddRecord(pRecord);
			}
		}
	}
	else
	{
		for(int i=MECHANICAL1; i<=MECHANICAL16; i++)
		{
			CXTPReportRecord* pRecord =  new CXTPReportRecord();
				
			pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[i].csLayerName));

			CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
			pColor->SetBackgroundColor(nEDA_PCB_LayerColor[i]);
			pRecord->AddItem(pColor);

			pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[i].bLayerShow));

			pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[i].bLayerMechEnabled));

			pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[i].bLayerSheetLinked));
			m_wndMechCtrl.AddRecord(pRecord);
		}
	}

	m_wndMechCtrl.SetMultipleSelection(FALSE);
	m_wndMechCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndMechCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndMechCtrl.GetPaintManager()->SetColumnStyle(XTPReportColumnStyle(1));

	RECT rect;
	m_wndMechCtrl.GetWindowRect(&rect);
	int num = m_wndMechCtrl.GetRecords()->GetCount();
	num = MIN(num, 16);
	//m_wndMechCtrl.SetWindowPos(NULL, 0, 0, rect.right-rect.left, 22 + num*19,SWP_NOZORDER|SWP_NOMOVE);
	m_wndMechCtrl.SetWindowPos(NULL, 0, 0, rect.right-rect.left, 28 + num*25,SWP_NOZORDER|SWP_NOMOVE);

	m_wndMechCtrl.Populate();
}

void CDlg_PCBLayerSetup::MaskReportCtrl()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CXTPReportColumn* pColumnLayer = new CXTPReportColumn(0, "助焊层", "",105, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnLayer->SetEditable(FALSE);	
	pColumnLayer->SetAllowDrag(FALSE);
	m_wndMaskCtrl.AddColumn(pColumnLayer);

	CXTPReportColumn* pColumnColor = new CXTPReportColumn(1, "颜色", "", 40, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnColor->SetEditable(FALSE);	
	pColumnColor->SetAllowDrag(FALSE);
	m_wndMaskCtrl.AddColumn(pColumnColor);
	m_wndMaskCtrl.SetFocusedColumn(pColumnColor);

	CXTPReportColumn* pColumnShow = new CXTPReportColumn(2, "显示", "", 50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnShow->SetAllowDrag(FALSE);
	pColumnShow->SetHeaderAlignment(DT_CENTER);
	m_wndMaskCtrl.AddColumn(pColumnShow);

	for(int i=TOPPASTE; i<=BOTTOMSOLDER; i++)
	{
		CXTPReportRecord* pRecord =  new CXTPReportRecord();

		pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[i].csLayerName));

		CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
		pColor->SetBackgroundColor(nEDA_PCB_LayerColor[i]);
		pRecord->AddItem(pColor);

		pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[i].bLayerShow));

		m_wndMaskCtrl.AddRecord(pRecord);
	}

	m_wndMaskCtrl.SetMultipleSelection(FALSE);
	m_wndMaskCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndMaskCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndMaskCtrl.GetPaintManager()->SetColumnStyle(XTPReportColumnStyle(1));

	m_wndMaskCtrl.Populate();
}

void CDlg_PCBLayerSetup::SilkReportCtrl()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CXTPReportColumn* pColumnLayer = new CXTPReportColumn(0, "丝印层", "",105, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnLayer->SetEditable(FALSE);	
	pColumnLayer->SetAllowDrag(FALSE);
	m_wndSilkCtrl.AddColumn(pColumnLayer);

	CXTPReportColumn* pColumnColor = new CXTPReportColumn(1, "颜色", "", 40, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnColor->SetEditable(FALSE);	
	pColumnColor->SetAllowDrag(FALSE);
	m_wndSilkCtrl.AddColumn(pColumnColor);
	m_wndSilkCtrl.SetFocusedColumn(pColumnColor);


	CXTPReportColumn* pColumnShow = new CXTPReportColumn(2, "显示", "", 50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnShow->SetAllowDrag(FALSE);
	pColumnShow->SetHeaderAlignment(DT_CENTER);
	m_wndSilkCtrl.AddColumn(pColumnShow);

	for(int i=TOPOVERLAY; i<=BOTTOMOVERLAY; i++)
	{
		CXTPReportRecord* pRecord =  new CXTPReportRecord();

		pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[i].csLayerName));

		CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
		pColor->SetBackgroundColor(nEDA_PCB_LayerColor[i]);
		pRecord->AddItem(pColor);

		pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[i].bLayerShow));

		m_wndSilkCtrl.AddRecord(pRecord);
	}

	m_wndSilkCtrl.SetMultipleSelection(FALSE);
	m_wndSilkCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndSilkCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndSilkCtrl.GetPaintManager()->SetColumnStyle(XTPReportColumnStyle(1));

	m_wndSilkCtrl.Populate();
}

void CDlg_PCBLayerSetup::OtherReportCtrl()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;


	CXTPReportColumn* pColumnLayer = new CXTPReportColumn(0, "其他层", "",105, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnLayer->SetEditable(FALSE);	
	pColumnLayer->SetAllowDrag(FALSE);
	m_wndOtherCtrl.AddColumn(pColumnLayer);


	CXTPReportColumn* pColumnColor = new CXTPReportColumn(1, "颜色", "", 40, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnColor->SetEditable(FALSE);	
	pColumnColor->SetAllowDrag(FALSE);
	m_wndOtherCtrl.AddColumn(pColumnColor);
	m_wndOtherCtrl.SetFocusedColumn(pColumnColor);


	CXTPReportColumn* pColumnShow = new CXTPReportColumn(2, "显示", "", 50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnShow->SetAllowDrag(FALSE);
	pColumnShow->SetHeaderAlignment(DT_CENTER);
	m_wndOtherCtrl.AddColumn(pColumnShow);

	for(int i=DRILLGUIDE; i<=KEEPOUTLAYER; i++)
	{
		CXTPReportRecord* pRecord =  new CXTPReportRecord();


		pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[i].csLayerName));

		CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
		pColor->SetBackgroundColor(nEDA_PCB_LayerColor[i]);
		pRecord->AddItem(pColor);

		pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[i].bLayerShow));

		m_wndOtherCtrl.AddRecord(pRecord);
	}

	for(int i=DRILLDRAWING; i<=MULTILAYER; i++)
	{
		CXTPReportRecord* pRecord =  new CXTPReportRecord();


		pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[i].csLayerName));

		CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
		pColor->SetBackgroundColor(nEDA_PCB_LayerColor[i]);
		pRecord->AddItem(pColor);

		pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[i].bLayerShow));

		m_wndOtherCtrl.AddRecord(pRecord);
	}

	m_wndOtherCtrl.SetMultipleSelection(FALSE);
	m_wndOtherCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndOtherCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndOtherCtrl.GetPaintManager()->SetColumnStyle(XTPReportColumnStyle(1));

	m_wndOtherCtrl.Populate();
}

void CDlg_PCBLayerSetup::SystemReportCtrl()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;


	CXTPReportColumn* pColumnLayer = new CXTPReportColumn(0, "系统层和系统颜色", "",234, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnLayer->SetEditable(FALSE);	
	pColumnLayer->SetAllowDrag(FALSE);
	m_wndSystemCtrl.AddColumn(pColumnLayer);


	CXTPReportColumn* pColumnColor = new CXTPReportColumn(1, "颜色", "", 40, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnColor->SetEditable(FALSE);	
	pColumnColor->SetAllowDrag(FALSE);
	m_wndSystemCtrl.AddColumn(pColumnColor);
	m_wndSystemCtrl.SetFocusedColumn(pColumnColor);


	CXTPReportColumn* pColumnShow = new CXTPReportColumn(2, "显示", "", 50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnShow->SetAllowDrag(FALSE);
	pColumnShow->SetHeaderAlignment(DT_CENTER);
	m_wndSystemCtrl.AddColumn(pColumnShow);

	for(int i=CONNECTIONS; i<=VIAHOLES; i++)
	{
		if(i==BACKGROUND)continue;
		CXTPReportRecord* pRecord =  new CXTPReportRecord();


		pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[i].csLayerName));

		CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
		pColor->SetBackgroundColor(nEDA_PCB_LayerColor[i]);
		pRecord->AddItem(pColor);

		if(i != SELECTIONS)	pRecord->AddItem(new CXTPReportRecordItemCheck(pDoc->m_sPCB_LayerParam[i].bLayerShow));

		m_wndSystemCtrl.AddRecord(pRecord);
	}


	CXTPReportRecord* pRecord =  new CXTPReportRecord();

	pRecord->AddItem( new CXTPReportRecordItemText(szEDA_HighlightColor));

	CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
	pColor->SetBackgroundColor(nEDA_PCB_HighlightColor);
	pRecord->AddItem(pColor);
	m_wndSystemCtrl.AddRecord(pRecord);


	pRecord =  new CXTPReportRecord();

	pRecord->AddItem( new CXTPReportRecordItemText(szEDA_BoardLineColor));

	pColor = new CXTPReportRecordItemColor();
	pColor->SetBackgroundColor(nEDA_PCB_BoardLineColor);
	pRecord->AddItem(pColor);
	m_wndSystemCtrl.AddRecord(pRecord);


	pRecord =  new CXTPReportRecord();

	pRecord->AddItem( new CXTPReportRecordItemText(szEDA_BoardAreaColor));

	pColor = new CXTPReportRecordItemColor();
	pColor->SetBackgroundColor(nEDA_PCB_BoardAreaColor);
	pRecord->AddItem(pColor);
	m_wndSystemCtrl.AddRecord(pRecord);


	pRecord =  new CXTPReportRecord();

	pRecord->AddItem( new CXTPReportRecordItemText(szEDA_SheetLineColor));

	pColor = new CXTPReportRecordItemColor();
	pColor->SetBackgroundColor(nEDA_PCB_SheetLineColor);
	pRecord->AddItem(pColor);
	m_wndSystemCtrl.AddRecord(pRecord);


	pRecord =  new CXTPReportRecord();

	pRecord->AddItem( new CXTPReportRecordItemText(szEDA_SheetAreaColor));

	pColor = new CXTPReportRecordItemColor();
	pColor->SetBackgroundColor(nEDA_PCB_SheetAreaColor);
	pRecord->AddItem(pColor);
	m_wndSystemCtrl.AddRecord(pRecord);


	pRecord =  new CXTPReportRecord();

	pRecord->AddItem( new CXTPReportRecordItemText(szEDA_WordspaceColor));

	pColor = new CXTPReportRecordItemColor();
	pColor->SetBackgroundColor(nEDA_PCB_WorkspaceColor);
	pRecord->AddItem(pColor);
	m_wndSystemCtrl.AddRecord(pRecord);

	m_wndSystemCtrl.SetMultipleSelection(FALSE);
	m_wndSystemCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndSystemCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndSystemCtrl.GetPaintManager()->SetColumnStyle(XTPReportColumnStyle(1));

	m_wndSystemCtrl.Populate();
}

void CDlg_PCBLayerSetup::OnBnClickedSingalLayerUsestack()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_SINGAL_LAYER_USESTACK);
	if(pButton->GetCheck() == BST_CHECKED)
	{
		m_wndSignalCtrl.ResetContent();
		SingalReportCtrl(TRUE, FALSE);
	}
	else if(pButton->GetCheck() == BST_UNCHECKED)
	{
		m_wndSignalCtrl.ResetContent();
		m_wndSignalCtrl.Populate();
		SingalReportCtrl(FALSE, FALSE);
	}
}

void CDlg_PCBLayerSetup::OnBnClickedPlaneUsestack()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_PLANE_USESTACK);
	if(pButton->GetCheck() == BST_CHECKED)
	{
		m_wndPlaneCtrl.ResetContent();
		PlaneReportCtrl(TRUE, FALSE);
	}
	else if(pButton->GetCheck() == BST_UNCHECKED)
	{
		m_wndPlaneCtrl.ResetContent();
		m_wndPlaneCtrl.Populate();
		PlaneReportCtrl(FALSE, FALSE);
	}
}

void CDlg_PCBLayerSetup::OnBnClickedMechLayerUsestack()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_MECH_LAYER_USESTACK);
	if(pButton->GetCheck() == BST_CHECKED)
	{
		m_wndMechCtrl.ResetContent();
		MechReportCtrl(TRUE, FALSE);
	}
	else if(pButton->GetCheck() == BST_UNCHECKED)
	{
		m_wndMechCtrl.ResetContent();
		m_wndMechCtrl.Populate();
		MechReportCtrl(FALSE, FALSE);
	}
}

void CDlg_PCBLayerSetup::OnBnClickedButtonDefault()
{
		CESDDoc* pDoc = (CESDDoc*)pDocument;

		CXTPReportRecords* pRecords = m_wndSignalCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CString str = pItem->GetValue();
			for(int j=0; j<PCB_LAYER_NUM; j++)
			{
				if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
				{
					pColor->SetBackgroundColor(nEDA_PCB_DefaultColor[j]);
					break;
				}
			}
		}
		m_wndSignalCtrl.RedrawControl();

		pRecords = m_wndPlaneCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CString str = pItem->GetValue();
			for(int j=0; j<PCB_LAYER_NUM; j++)
			{
				if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
				{
					pColor->SetBackgroundColor(nEDA_PCB_DefaultColor[j]);
					break;
				}
			}
		}
		m_wndPlaneCtrl.RedrawControl();

		pRecords = m_wndMechCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CString str = pItem->GetValue();
			for(int j=0; j<PCB_LAYER_NUM; j++)
			{
				if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
				{
					pColor->SetBackgroundColor(nEDA_PCB_DefaultColor[j]);
					break;
				}
			}
		}
		m_wndMechCtrl.RedrawControl();

		pRecords = m_wndMaskCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CString str = pItem->GetValue();
			for(int j=0; j<PCB_LAYER_NUM; j++)
			{
				if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
				{
					pColor->SetBackgroundColor(nEDA_PCB_DefaultColor[j]);
					break;
				}
			}
		}
		m_wndMaskCtrl.RedrawControl();

		pRecords = m_wndOtherCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CString str = pItem->GetValue();
			for(int j=0; j<PCB_LAYER_NUM; j++)
			{
				if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
				{
					pColor->SetBackgroundColor(nEDA_PCB_DefaultColor[j]);
					break;
				}
			}
		}
		m_wndOtherCtrl.RedrawControl();

		pRecords = m_wndSilkCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CString str = pItem->GetValue();
			for(int j=0; j<PCB_LAYER_NUM; j++)
			{
				if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
				{
					pColor->SetBackgroundColor(nEDA_PCB_DefaultColor[j]);
					break;
				}
			}
		}
		m_wndSilkCtrl.RedrawControl();

		pRecords = m_wndSystemCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CString str = pItem->GetValue();
			if(str == szEDA_HighlightColor) pColor->SetBackgroundColor(nEDA_PCB_HighlightDefaultColor);
			else if(str == szEDA_BoardLineColor) pColor->SetBackgroundColor(nEDA_PCB_BoardLineDefaultColor);
			else if(str == szEDA_BoardAreaColor) pColor->SetBackgroundColor(nEDA_PCB_BoardAreaDefaultColor);
			else if(str == szEDA_SheetLineColor) pColor->SetBackgroundColor(nEDA_PCB_SheetLineDefaultColor);
			else if(str == szEDA_SheetAreaColor) pColor->SetBackgroundColor(nEDA_PCB_SheetAreaDefaultColor);
			else if(str == szEDA_WordspaceColor) pColor->SetBackgroundColor(nEDA_PCB_WorkspaceDefaultColor);
			else
			{
				for(int j=0; j<PCB_LAYER_NUM; j++)
				{
					if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
					{
						pColor->SetBackgroundColor(nEDA_PCB_DefaultColor[j]);
						break;
					}
				}
			}
		}

		m_wndSystemCtrl.RedrawControl();
}


void CDlg_PCBLayerSetup::OnBnClickedSingalAllon()
{
	CXTPReportRecords* pRecords = m_wndSignalCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		pCheck->SetChecked(TRUE);
	}
	m_wndSignalCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedSingalAlloff()
{
	CXTPReportRecords* pRecords = m_wndSignalCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		pCheck->SetChecked(FALSE);
	}
	m_wndSignalCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedSingalUsedon()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

}

void CDlg_PCBLayerSetup::OnBnClickedPlaneAllon()
{
	CXTPReportRecords* pRecords = m_wndPlaneCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		pCheck->SetChecked(TRUE);
	}
	m_wndPlaneCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedPlaneAlloff()
{
	CXTPReportRecords* pRecords = m_wndPlaneCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		pCheck->SetChecked(FALSE);
	}
	m_wndPlaneCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedPlaneUsedon()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlg_PCBLayerSetup::OnBnClickedMechAllon()
{
	CXTPReportRecords* pRecords = m_wndMechCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		pCheck->SetChecked(TRUE);
	}
	m_wndMechCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedMechAlloff()
{
	CXTPReportRecords* pRecords = m_wndMechCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		pCheck->SetChecked(FALSE);
	}
	m_wndMechCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedMechUsedon()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlg_PCBLayerSetup::OnBnClickedMaskAllon()
{
	CXTPReportRecords* pRecords = m_wndMaskCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		pCheck->SetChecked(TRUE);
	}
	m_wndMaskCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedMaskAlloff()
{
	CXTPReportRecords* pRecords = m_wndMaskCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		pCheck->SetChecked(FALSE);
	}
	m_wndMaskCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedMaskUsedon()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlg_PCBLayerSetup::OnBnClickedOtherAllon()
{
	CXTPReportRecords* pRecords = m_wndOtherCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		pCheck->SetChecked(TRUE);
	}
	m_wndOtherCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedOtherAlloff()
{
	CXTPReportRecords* pRecords = m_wndOtherCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		pCheck->SetChecked(FALSE);
	}
	m_wndOtherCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedOtherUsedon()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlg_PCBLayerSetup::OnBnClickedSilkAllon()
{
	CXTPReportRecords* pRecords = m_wndSilkCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		pCheck->SetChecked(TRUE);
	}
	m_wndSilkCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedSilkAlloff()
{
	CXTPReportRecords* pRecords = m_wndSilkCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		pCheck->SetChecked(FALSE);
	}
	m_wndSilkCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedSilkUsedon()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlg_PCBLayerSetup::OnBnClickedControlAllon()
{
	CXTPReportRecords* pRecords = m_wndSystemCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		if(pCheck != NULL)	pCheck->SetChecked(TRUE);
	}
	m_wndSystemCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedControlAlloff()
{
	CXTPReportRecords* pRecords = m_wndSystemCtrl.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
		if(pCheck != NULL)	pCheck->SetChecked(FALSE);
	}
	m_wndSystemCtrl.RedrawControl();
}

void CDlg_PCBLayerSetup::OnBnClickedAlloff()
{
	OnBnClickedSingalAlloff();
	OnBnClickedPlaneAlloff();
	OnBnClickedMechAlloff();
	OnBnClickedMaskAlloff();
	OnBnClickedOtherAlloff();
	OnBnClickedSilkAlloff();
	OnBnClickedControlAlloff();
}

void CDlg_PCBLayerSetup::OnBnClickedAllon()
{
	OnBnClickedSingalAllon();
	OnBnClickedPlaneAllon();
	OnBnClickedMechAllon();
	OnBnClickedMaskAllon();
	OnBnClickedOtherAllon();
	OnBnClickedSilkAllon();
	OnBnClickedControlAllon();
}

void CDlg_PCBLayerSetup::OnBnClickedUsedon()
{
	OnBnClickedSingalUsedon();
	OnBnClickedPlaneUsedon();
	OnBnClickedMechUsedon();
	OnBnClickedMaskUsedon();
	OnBnClickedOtherUsedon();
	OnBnClickedSilkUsedon();
}

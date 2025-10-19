// Dlg_ReportElements.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_ReportElements.h"
#include ".\dlg_reportelements.h"


// CDlg_ReportElements 对话框

IMPLEMENT_DYNAMIC(CDlg_ReportElements, CDialog)
CDlg_ReportElements::CDlg_ReportElements(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_ReportElements::IDD, pParent)
{
	m_nCurrentDiagram = 0;
}

CDlg_ReportElements::~CDlg_ReportElements()
{
}

void CDlg_ReportElements::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Radio(pDX, IDC_RADIO_SHEET, m_nCurrentDiagram);
}


BEGIN_MESSAGE_MAP(CDlg_ReportElements, CDialog)
	ON_BN_CLICKED(IDC_RADIO_SHEET, OnBnClickedRadioSheet)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO10, OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO11, OnBnClickedRadio11)
	ON_BN_CLICKED(IDC_RADIO12, OnBnClickedRadio12)
	ON_BN_CLICKED(IDC_RADIO13, OnBnClickedRadio13)
	ON_BN_CLICKED(IDC_RADIO14, OnBnClickedRadio14)
	ON_BN_CLICKED(IDC_RADIO15, OnBnClickedRadio15)
	ON_BN_CLICKED(IDC_RADIO16, OnBnClickedRadio16)
	ON_BN_CLICKED(IDC_RADIO17, OnBnClickedRadio17)
	ON_BN_CLICKED(IDC_RADIO18, OnBnClickedRadio18)
	ON_BN_CLICKED(IDC_RADIO19, OnBnClickedRadio19)
	ON_BN_CLICKED(IDC_RADIO20, OnBnClickedRadio20)
END_MESSAGE_MAP()


// CDlg_ReportElements 消息处理程序


BOOL CDlg_ReportElements::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_RADIO1);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[0]);
	if(pDoc->Get_SCH_Elements_Param(0,0,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO2);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[1]);
	if(pDoc->Get_SCH_Elements_Param(0,1,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO3);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[2]);
	if(pDoc->Get_SCH_Elements_Param(0,2,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO4);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[3]);
	if(pDoc->Get_SCH_Elements_Param(0,3,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO5);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[4]);
	if(pDoc->Get_SCH_Elements_Param(0,4,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO6);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[5]);
	if(pDoc->Get_SCH_Elements_Param(0,5,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO7);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[6]);
	if(pDoc->Get_SCH_Elements_Param(0,6,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO8);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[7]);
	if(pDoc->Get_SCH_Elements_Param(0,7,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO9);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[8]);
	if(pDoc->Get_SCH_Elements_Param(0,8,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO10);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[9]);
	if(pDoc->Get_SCH_Elements_Param(0,9,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO11);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[10]);
	if(pDoc->Get_SCH_Elements_Param(0,10,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO12);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[11]);
	if(pDoc->Get_SCH_Elements_Param(0,11,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO13);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[12]);
	if(pDoc->Get_SCH_Elements_Param(0,12,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO14);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[13]);
	if(pDoc->Get_SCH_Elements_Param(0,13,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO15);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[14]);
	if(pDoc->Get_SCH_Elements_Param(0,14,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO16);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[15]);
	if(pDoc->Get_SCH_Elements_Param(0,15,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO17);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[16]);
	if(pDoc->Get_SCH_Elements_Param(0,16,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO18);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[17]);
	if(pDoc->Get_SCH_Elements_Param(0,17,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO19);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[18]);
	if(pDoc->Get_SCH_Elements_Param(0,18,0) == 0) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO20);
	pWnd->SetWindowText(pDoc->m_csSCH_SheetName[19]);
	if(pDoc->Get_SCH_Elements_Param(0,19,0) == 0) pWnd->EnableWindow(FALSE);

	Refresh_Element_Data(0);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlg_ReportElements::Refresh_Element_Data(int diagram_radio_selection)
{
	CWnd* pWnd;
	int num;  char str[100];
	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_COMPONENT, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_COMPONENT, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_COMPNUM);
	pWnd->SetWindowText(str);
	
	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_WIRE, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_WIRE, diagram_radio_selection - 1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_WIRENUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_JUNCTION, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_JUNCTION, diagram_radio_selection - 1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_JUNCTIONNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_POWER, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_POWER, diagram_radio_selection - 1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_POWERNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_NETLABEL, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_NETLABEL, diagram_radio_selection - 1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_NETLABELNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_PORT, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_PORT, diagram_radio_selection - 1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_PORTNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_SHEETSYMBOL, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_SHEETSYMBOL, diagram_radio_selection - 1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_SHEETSYMBOLNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_BUS, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_BUS, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_BUSNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_BUSENTRY, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_BUSENTRY, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_BUSENTRYNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_TEXT, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_TEXT, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_TEXTNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_TEXTFRAME, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_TEXTFRAME, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_TEXTFRAMENUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_POLYLINE, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_POLYLINE, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_POLYLINENUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_ARC, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_ARC, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_ARCNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_PIE, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_PIE, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_PIENUM);
	pWnd->SetWindowText(str);	

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_POLYGON, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_POLYGON, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_POLYGONNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_RECT, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_RECT, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_RECTNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_ROUNDRECT, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_ROUNDRECT, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_ROUNDRECTNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_ELLIPSEFILL, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_ELLIPSEFILL, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_ELLIPSEFILLNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_ELLIPSE, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_ELLIPSE, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_ELLIPSENUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_BEZIER, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_BEZIER, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_BEZIERNUM);
	pWnd->SetWindowText(str);

	num = 0;
	if(diagram_radio_selection == 0) for(int i=0; i<SCH_SHEET_NUM; i++)	num += pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_GRAPHIC, i, 0);
	else num = pDoc->Get_SCH_Elements_Param(SCH_ELEMENT_GRAPHIC, diagram_radio_selection-1, 0);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_GRAPHICNUM);
	pWnd->SetWindowText(str);

	if(diagram_radio_selection == 0) num = pDoc->Get_SCH_Elements_Param(0,0,1);
	else num = pDoc->Get_SCH_Elements_Param(0,diagram_radio_selection-1,1);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_ELEMENT);
	pWnd->SetWindowText(str);

	if(diagram_radio_selection == 0) num = pDoc->Get_SCH_Elements_Param(0,0,2);
	else num = pDoc->Get_SCH_Elements_Param(0,diagram_radio_selection-1,2);
	itoa(num, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_AD6);
	pWnd->SetWindowText(str);
}

void CDlg_ReportElements::OnBnClickedRadioSheet()
{
	Refresh_Element_Data(0);
}

void CDlg_ReportElements::OnBnClickedRadio1()
{
	Refresh_Element_Data(1);
}

void CDlg_ReportElements::OnBnClickedRadio2()
{
	Refresh_Element_Data(2);
}

void CDlg_ReportElements::OnBnClickedRadio3()
{
	Refresh_Element_Data(3);
}

void CDlg_ReportElements::OnBnClickedRadio4()
{
	Refresh_Element_Data(4);
}

void CDlg_ReportElements::OnBnClickedRadio5()
{
	Refresh_Element_Data(5);
}

void CDlg_ReportElements::OnBnClickedRadio6()
{
	Refresh_Element_Data(6);
}

void CDlg_ReportElements::OnBnClickedRadio7()
{
	Refresh_Element_Data(7);
}

void CDlg_ReportElements::OnBnClickedRadio8()
{
	Refresh_Element_Data(8);
}

void CDlg_ReportElements::OnBnClickedRadio9()
{
	Refresh_Element_Data(9);
}

void CDlg_ReportElements::OnBnClickedRadio10()
{
	Refresh_Element_Data(10);
}

void CDlg_ReportElements::OnBnClickedRadio11()
{
	Refresh_Element_Data(11);
}

void CDlg_ReportElements::OnBnClickedRadio12()
{
	Refresh_Element_Data(12);
}

void CDlg_ReportElements::OnBnClickedRadio13()
{
	Refresh_Element_Data(13);
}

void CDlg_ReportElements::OnBnClickedRadio14()
{
	Refresh_Element_Data(14);
}

void CDlg_ReportElements::OnBnClickedRadio15()
{
	Refresh_Element_Data(15);
}

void CDlg_ReportElements::OnBnClickedRadio16()
{
	Refresh_Element_Data(16);
}

void CDlg_ReportElements::OnBnClickedRadio17()
{
	Refresh_Element_Data(17);
}

void CDlg_ReportElements::OnBnClickedRadio18()
{
	Refresh_Element_Data(18);
}

void CDlg_ReportElements::OnBnClickedRadio19()
{
	Refresh_Element_Data(19);
}

void CDlg_ReportElements::OnBnClickedRadio20()
{
	Refresh_Element_Data(20);
}

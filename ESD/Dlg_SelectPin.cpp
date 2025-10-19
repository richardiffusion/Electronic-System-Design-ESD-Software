// Dlg_SelectPin.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "ESDDoc.h"
#include "Dlg_SelectPin.h"
#include ".\dlg_selectpin.h"
#include "ReportCtrlItem.h"
#include "Dlg_EditPin.h"

enum
{
	EDA_V4_COLUMN_INDEX,
	EDA_V4_COLUMN_NAME,	
	EDA_V4_COLUMN_NUMBER,
	EDA_V4_COLUMN_DESCRIPTION,
	EDA_V4_COLUMN_TYPE,
	EDA_V4_COLUMN_OWNER,
	EDA_V4_COLUMN_SHOWPIN,
	EDA_V4_COLUMN_SHOWNUMBER,
	EDA_V4_COLUMN_SHOWNAME,
};

// CDlg_SelectPin 对话框

IMPLEMENT_DYNAMIC(CDlg_SelectPin, CDialog)
CDlg_SelectPin::CDlg_SelectPin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_SelectPin::IDD, pParent)
{

}

CDlg_SelectPin::~CDlg_SelectPin()
{
}

void CDlg_SelectPin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_REPORT_PIN, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(CDlg_SelectPin, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_PIN, OnBnClickedButtonEditPin)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_SelectPin 消息处理程序

BOOL CDlg_SelectPin::OnInitDialog()
{
	CDialog::OnInitDialog();

	char* typestr[8] = { "输入(Input)", "输入输出(IO)", "输出(Output)" ,"开集(open collector)", "被动(Passive)", "高阻(Hiz)", "射极输出(Emitter)", "电源(Power)" };

	CXTPReportColumn* pColumnIndex = new CXTPReportColumn(EDA_V4_COLUMN_INDEX, "序号", "",50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnIndex->SetEditable(FALSE);	//序号不允许修改
	m_wndReportCtrl.AddColumn(pColumnIndex);

	m_wndReportCtrl.AddColumn(new CXTPReportColumn(EDA_V4_COLUMN_NAME, "引脚名称", "", 100, FALSE, XTP_REPORT_NOICON, TRUE, TRUE));

	m_wndReportCtrl.AddColumn(new CXTPReportColumn(EDA_V4_COLUMN_NUMBER, "引脚编号", "",100, FALSE, XTP_REPORT_NOICON, TRUE, TRUE));

	m_wndReportCtrl.AddColumn(new CXTPReportColumn(EDA_V4_COLUMN_DESCRIPTION, "功能描述", "",160, FALSE, XTP_REPORT_NOICON, TRUE, TRUE));

	CXTPReportColumn* pColumnType = m_wndReportCtrl.AddColumn(new CXTPReportColumn(EDA_V4_COLUMN_TYPE, _T("种类"), "",120, FALSE, XTP_REPORT_NOICON, TRUE, TRUE));


	CXTPReportColumn* pColumnPart = new CXTPReportColumn(EDA_V4_COLUMN_OWNER, "所属单元", "",60, FALSE, XTP_REPORT_NOICON, TRUE, TRUE);  
	m_wndReportCtrl.AddColumn(pColumnPart);

	CXTPReportColumn* pColumnCheckPin = new CXTPReportColumn(EDA_V4_COLUMN_SHOWPIN, "显示引脚", "",60, FALSE, XTP_REPORT_NOICON, TRUE, TRUE);
	m_wndReportCtrl.AddColumn(pColumnCheckPin);
	pColumnCheckPin->SetHeaderAlignment(DT_CENTER);

	CXTPReportColumn* pColumnCheckName = new CXTPReportColumn(EDA_V4_COLUMN_SHOWNUMBER, "显示编号", "",60, FALSE, XTP_REPORT_NOICON, TRUE, TRUE);
	m_wndReportCtrl.AddColumn(pColumnCheckName);
	pColumnCheckName->SetHeaderAlignment(DT_CENTER);

	CXTPReportColumn* pColumnCheckNumber = new CXTPReportColumn(EDA_V4_COLUMN_SHOWNAME, "显示名称", "",60, FALSE, XTP_REPORT_NOICON, TRUE, TRUE);
	m_wndReportCtrl.AddColumn(pColumnCheckNumber);
	pColumnCheckNumber->SetHeaderAlignment(DT_CENTER);

	pColumnType->GetEditOptions()->AddConstraint(_T("输入(Input)"), 0); 
	pColumnType->GetEditOptions()->AddConstraint(_T("输入输出(IO)"), 1);
	pColumnType->GetEditOptions()->AddConstraint(_T("输出(Output)"), 2); 
	pColumnType->GetEditOptions()->AddConstraint(_T("开集(open collector)"), 3);
	pColumnType->GetEditOptions()->AddConstraint(_T("被动(Passive)"), 4);
	pColumnType->GetEditOptions()->AddConstraint(_T("高阻(Hiz)"), 5);
	pColumnType->GetEditOptions()->AddConstraint(_T("射极输出(Emitter)"), 6);
	pColumnType->GetEditOptions()->AddConstraint(_T("电源(Power)"), 7);
	pColumnType->GetEditOptions()->m_bConstraintEdit = TRUE;
	pColumnType->GetEditOptions()->m_bAllowEdit = FALSE;
	pColumnType->GetEditOptions()->AddComboButton();


	for(int i=0; i<=m_nPartCount; i++)
	{
		char str[100];   itoa(i, str, 10);
		if(i == 0) strcat(str, " (公用)");
		pColumnPart->GetEditOptions()->AddConstraint(str, i); 
	}
	pColumnPart->GetEditOptions()->m_bConstraintEdit = TRUE;
	pColumnPart->GetEditOptions()->m_bAllowEdit = FALSE;
	pColumnPart->GetEditOptions()->AddComboButton();
	
	COb_schpin* ppin;
	for(int i=0; i<m_arrayPins.GetCount(); i++)
	{
		ppin = (COb_schpin*)m_arrayPins.GetAt(i);
		m_arrayRecords[i] = new CXTPReportRecord();

		CXTPReportRecordItemNumber* pRecordItem1 = new CXTPReportRecordItemNumber(i+1);
		pRecordItem1->SetBackgroundColor(RGB(170,170,170));
		m_arrayRecords[i]->AddItem( pRecordItem1); //new CXTPReportRecordItemNumber(i+1));

		CXTPReportRecordItemText* pRecordItem2 = new CXTPReportRecordItemText(ppin->m_csName);
		m_arrayRecords[i]->AddItem(pRecordItem2);

		CXTPReportRecordItemText* pRecordItem3 = new CXTPReportRecordItemText(ppin->m_csNumber);
		m_arrayRecords[i]->AddItem(pRecordItem3);

		CXTPReportRecordItemText* pRecordItem4 = new CXTPReportRecordItemText(ppin->m_csDescription);
		m_arrayRecords[i]->AddItem(pRecordItem4);

		CXTPReportRecordItemText* pRecordItem5 = new CXTPReportRecordItemText(typestr[ppin->m_nElectricalType]);
		m_arrayRecords[i]->AddItem(pRecordItem5);

		char str[100]; itoa(ppin->m_nOwnerPartID, str, 10);
		if(ppin->m_nOwnerPartID == 0) strcat(str, " (公用)");
		CXTPReportRecordItemText* pRecordItem6 = new CXTPReportRecordItemText(str);
		m_arrayRecords[i]->AddItem(pRecordItem6);

		CXTPReportRecordItemCheck* pRecordItem7 = new CXTPReportRecordItemCheck(ppin->m_bHide?FALSE:TRUE);
		m_arrayRecords[i]->AddItem(pRecordItem7);

		CXTPReportRecordItemCheck* pRecordItem8 = new CXTPReportRecordItemCheck(ppin->m_bNumberVisible);
		m_arrayRecords[i]->AddItem(pRecordItem8);

		CXTPReportRecordItemCheck* pRecordItem9 = new CXTPReportRecordItemCheck(ppin->m_bNameVisible);
		m_arrayRecords[i]->AddItem(pRecordItem9);

		if(m_nPartCount > 1)
		{
			if(ppin->m_nOwnerPartID == 0)
			{
				pRecordItem2->SetBackgroundColor(RGB(255, 255,110));
				pRecordItem3->SetBackgroundColor(RGB(255, 255,110));
				pRecordItem4->SetBackgroundColor(RGB(255, 255,110));
				pRecordItem5->SetBackgroundColor(RGB(255, 255,110));
				pRecordItem6->SetBackgroundColor(RGB(255, 255,110));
				pRecordItem7->SetBackgroundColor(RGB(255, 255,110));
				pRecordItem8->SetBackgroundColor(RGB(255, 255,110));
				pRecordItem9->SetBackgroundColor(RGB(255, 255,110));
			}
			else if(ppin->m_nOwnerPartID == m_nPart)
			{
				pRecordItem2->SetBackgroundColor(RGB(255,190,190));
				pRecordItem3->SetBackgroundColor(RGB(255,190,190));
				pRecordItem4->SetBackgroundColor(RGB(255,190,190));
				pRecordItem5->SetBackgroundColor(RGB(255,190,190));
				pRecordItem6->SetBackgroundColor(RGB(255,190,190));
				pRecordItem7->SetBackgroundColor(RGB(255,190,190));
				pRecordItem8->SetBackgroundColor(RGB(255,190,190));
				pRecordItem9->SetBackgroundColor(RGB(255,190,190));
			}
		}

		m_wndReportCtrl.AddRecord(m_arrayRecords[i]);
	}

	m_wndReportCtrl.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReportCtrl.AllowEdit(TRUE);
	m_wndReportCtrl.FocusSubItems(TRUE);
	m_wndReportCtrl.SetMultipleSelection(FALSE);
	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridLargeDots); //xtpReportGridSolid);		//TRUE for changing vertical grid style,
	m_wndReportCtrl.SetGridStyle(FALSE, xtpReportGridLargeDots ); //xtpReportGridSolid);	//FALSE for changing horizontal grid style
	m_wndReportCtrl.Populate();
	//m_wndReportCtrl.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_SelectPin::OnBnClickedButtonEditPin()
{
	CXTPReportSelectedRows* pSelectedRows = m_wndReportCtrl.GetSelectedRows();
	CXTPReportRow* pRows = pSelectedRows->GetAt(0);

	CXTPReportRecord* pRecord = pRows->GetRecord();
	
	int index = pRecord->GetIndex();

	COb_schpin* ppin = m_arrayPins.GetAt(index);

	char* typestr[8] = { "输入(Input)", "输入输出(IO)", "输出(Output)" ,"开集(open collector)", "被动(Passive)", "高阻(Hiz)", "射极输出(Emitter)", "电源(Power)" };

								//0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  29 30  31 32 33 34 35 36 37 38 39 
	int		  inside_cov1[40] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 7, 8, 9, 0, 0, 0, 0,  0,10,  0,11, 0, 0, 0, 0, 0, 0, 0 };
	int  inside_edge_cov1[40] = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int outside_edge_cov1[40] = { 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int		 outside_cov1[40] = { 0, 0, 1, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0,  0, 0,  0, 0, 5, 6, 0, 0, 0, 0, 0 };
	
				  		  	   //0  1  2  3  4  5  6  7  8  9 10 11 
	int		  inside_cov2[12] = {0, 8, 9,10,11,12,13,22,23,24,30,32};
	int  inside_edge_cov2[2 ] = {0, 3};
	int outside_edge_cov2[4 ] = {0, 1, 4, 17};
	int		 outside_cov2[7 ] = {0, 2, 5, 6,25,33,34};

	CESDDoc* pDoc = (CESDDoc*)ppin->pDocument;

	CDlg_EditPin dlg;
	dlg.pDoc = pDoc;
	
	dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(ppin->m_fX);
	dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(ppin->m_fY);
	dlg.m_csEditPinLength = pDoc->Convert_XY_To_String(ppin->m_fPinLength);
	dlg.m_nComboOrientation = ppin->m_nOrientation/90;
	dlg.m_nColor = ppin->m_nColor;
	
	if(ppin->m_nInside > 39) dlg.m_nComboInside = 0;
	else dlg.m_nComboInside = inside_cov1[ppin->m_nInside];
	if(ppin->m_nInsideEdge > 39) dlg.m_nComboInsideEdge = 0;
	else dlg.m_nComboInsideEdge = inside_edge_cov1[ppin->m_nInsideEdge];
	if(ppin->m_nOutside > 39) dlg.m_nComboOutside = 0;
	else dlg.m_nComboOutside = outside_cov1[ppin->m_nOutside];
	if(ppin->m_nOutsideEdge > 39) dlg.m_nComboOutsideEdge = 0;
	else dlg.m_nComboOutsideEdge = outside_edge_cov1[ppin->m_nOutsideEdge];

	dlg.m_csEditConnectTo = ppin->m_csConnectTo.IsEmpty()?ppin->m_csName:ppin->m_csConnectTo; //ppin->m_csConnectTo;
	dlg.m_csEditUniqueID = ppin->m_csUniqueID;

	CXTPReportRecordItemText* pItemText;

	pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_NUMBER);
	dlg.m_csEditPinNumber = pItemText->GetValue();

	pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_NAME);
	dlg.m_csEditPinName = pItemText->GetValue();

	pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_DESCRIPTION);
	dlg.m_csEditDescription = pItemText->GetValue();

	pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_TYPE);
	int i;
	for(i=0; i<8; i++)
	{
		if(pItemText->GetValue() == typestr[i]) break;
	}
	if(i >= 8) i=0;
	dlg.m_nComboType = i;

	pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_OWNER);
	dlg.m_nEditPartNum = atoi(pItemText->GetValue());

	CXTPReportRecordItem* pItemCheck;
	pItemCheck = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_SHOWPIN);
	if(pItemCheck->IsChecked() == TRUE)	dlg.m_bCheckPinHide = FALSE;
	else dlg.m_bCheckPinHide = TRUE;

	pItemCheck = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_SHOWNUMBER);
	dlg.m_bCheckNumberVisible = pItemCheck->IsChecked();

	pItemCheck = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_SHOWNAME);
	dlg.m_bCheckNameVisible = pItemCheck->IsChecked();


	dlg.m_cPinDemo.m_pPin = ppin;
	
	if(dlg.DoModal() == IDOK)
	{
		ppin->m_fX = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
		ppin->m_fY = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
		ppin->m_fPinLength = pDoc->Convert_String_To_XY(dlg.m_csEditPinLength);
		ppin->m_nOrientation = dlg.m_nComboOrientation*90;
		ppin->m_nColor = dlg.m_nColor;
		
		ppin->m_nInside = inside_cov2[dlg.m_nComboInside];
		ppin->m_nInsideEdge = inside_edge_cov2[dlg.m_nComboInsideEdge];
		ppin->m_nOutside = outside_cov2[dlg.m_nComboOutside];
		ppin->m_nOutsideEdge = outside_edge_cov2[dlg.m_nComboOutsideEdge];


		ppin->m_csConnectTo = dlg.m_csEditConnectTo;
		

		CXTPReportRecordItemText* pItemText;

		ppin->m_csNumber = dlg.m_csEditPinNumber;
		pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_NUMBER);
		pItemText->SetValue(dlg.m_csEditPinNumber);

		ppin->m_csName = dlg.m_csEditPinName;
		pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_NAME);
		pItemText->SetValue(dlg.m_csEditPinName);

		ppin->m_csDescription = dlg.m_csEditDescription;
		pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_DESCRIPTION);
		pItemText->SetValue(dlg.m_csEditDescription);

		ppin->m_nElectricalType = dlg.m_nComboType;
		pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_TYPE);
		pItemText->SetValue(typestr[dlg.m_nComboType]);

		ppin->m_nOwnerPartID = dlg.m_nEditPartNum;
		pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_OWNER);
		char str[100]; itoa(dlg.m_nEditPartNum, str, 10);
		if(ppin->m_nOwnerPartID == 0) strcat(str, " (公用)");
		pItemText->SetValue(str);

		ppin->m_bHide = dlg.m_bCheckPinHide;
		CXTPReportRecordItem* pItemCheck;
		pItemCheck = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_SHOWPIN);
		if(dlg.m_bCheckPinHide == FALSE) pItemCheck->SetChecked(TRUE);
		else pItemCheck->SetChecked(FALSE);

		ppin->m_bNumberVisible = dlg.m_bCheckNumberVisible;
		pItemCheck = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_SHOWNUMBER);
		pItemCheck->SetChecked(dlg.m_bCheckNumberVisible);

		ppin->m_bNameVisible = dlg.m_bCheckNameVisible;
		pItemCheck = (CXTPReportRecordItemText*)pRecord->GetItem(EDA_V4_COLUMN_SHOWNAME);
		pItemCheck->SetChecked(dlg.m_bCheckNameVisible);

		m_wndReportCtrl.RedrawControl();
	}
}

void CDlg_SelectPin::OnBnClickedOk()
{
	char* typestr[8] = { "输入(Input)", "输入输出(IO)", "输出(Output)" ,"开集(open collector)", "被动(Passive)", "高阻(Hiz)", "射极输出(Emitter)", "电源(Power)" };
	
	for(int i=0; i < m_arrayRecords.GetCount(); i++)
	{
		int index = m_arrayRecords[i]->GetIndex();
		COb_schpin* ppin = m_arrayPins.GetAt(index);

		CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)m_arrayRecords[i]->GetItem(EDA_V4_COLUMN_NUMBER);
		ppin->m_csNumber = pItem->GetValue();

		pItem = (CXTPReportRecordItemText*)m_arrayRecords[i]->GetItem(EDA_V4_COLUMN_NAME);
		ppin->m_csName = pItem->GetValue();

		pItem = (CXTPReportRecordItemText*)m_arrayRecords[i]->GetItem(EDA_V4_COLUMN_DESCRIPTION);
		ppin->m_csDescription = pItem->GetValue();

		pItem = (CXTPReportRecordItemText*)m_arrayRecords[i]->GetItem(EDA_V4_COLUMN_TYPE);
		CString cstring = pItem->GetValue();

		int k;
		for(k=0; k<8; k++)
			if(cstring == typestr[k]) break;
		ppin->m_nElectricalType = k;

		pItem = (CXTPReportRecordItemText*)m_arrayRecords[i]->GetItem(EDA_V4_COLUMN_OWNER);
		cstring = pItem->GetValue();
		ppin->m_nOwnerPartID = atoi(cstring);

		CXTPReportRecordItem* pItemCheck = m_arrayRecords[i]->GetItem(EDA_V4_COLUMN_SHOWPIN);
		if(pItemCheck->IsChecked() == TRUE) ppin->m_bHide = FALSE;
		else ppin->m_bHide = TRUE;

		pItemCheck = m_arrayRecords[i]->GetItem(EDA_V4_COLUMN_SHOWNUMBER);
		ppin->m_bNumberVisible = pItemCheck->IsChecked();

		pItemCheck = m_arrayRecords[i]->GetItem(EDA_V4_COLUMN_SHOWNAME);
		ppin->m_bNameVisible = pItemCheck->IsChecked();

	}

	OnOK();
}

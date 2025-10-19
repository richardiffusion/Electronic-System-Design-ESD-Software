// SchSheetOptionsPage.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Page_SchOptions.h"
#include "systemext.h"
#include ".\page_schoptions.h"



// CPage_SchOptions 对话框

IMPLEMENT_DYNAMIC(CPage_SchOptions, CPropertyPage)
CPage_SchOptions::CPage_SchOptions()
	: CPropertyPage(CPage_SchOptions::IDD)
	, m_nComboSheetNum(0)
	, m_csEditSheetName(_T(""))
	, m_nComboOrientation(0)
	, m_bCheckTitle(FALSE)
	, m_nComboTitle(0)
	, m_nEditNumberSpace(0)
	, m_bCheckReference(FALSE)
	, m_bCheckShowBorder(FALSE)
	, m_nEditStandard(0)
	, m_bCheckSnap(FALSE)
	, m_bCheckVisible(FALSE)
	, m_bCheckElectrical(FALSE)
	, m_bCheckCustom(FALSE)
	, m_nEditXCount(0)
	, m_nEditYCount(0)
	, m_bCheckSheetEnable(FALSE)
	, m_csEditSnap(_T(""))
	, m_csEditVisible(_T(""))
	, m_csEditElectrical(_T(""))
	, m_csEditCustomWidth(_T(""))
	, m_csEditCustomHeight(_T(""))
	, m_csEditMargin(_T(""))
{
}

CPage_SchOptions::~CPage_SchOptions()
{
}

void CPage_SchOptions::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_SHEETNUM, m_nComboSheetNum);
	DDX_Text(pDX, IDC_EDIT_SHEETNAME, m_csEditSheetName);
	DDX_CBIndex(pDX, IDC_COMBO_ORIENTATION, m_nComboOrientation);
	DDX_Check(pDX, IDC_CHECK_TITLE, m_bCheckTitle);
	DDX_CBIndex(pDX, IDC_COMBO_TITLE, m_nComboTitle);
	DDX_Text(pDX, IDC_EDIT_NUMBERSPACE, m_nEditNumberSpace);
	DDX_Check(pDX, IDC_CHECK_REFERENCE, m_bCheckReference);
	DDX_Check(pDX, IDC_CHECK_SHOWBORDER, m_bCheckShowBorder);
	DDX_CBIndex(pDX, IDC_COMBO_STANDARD, m_nEditStandard);
	DDX_Check(pDX, IDC_CHECK_SNAP, m_bCheckSnap);
	DDX_Check(pDX, IDC_CHECK_VISIBLE, m_bCheckVisible);
	DDX_Check(pDX, IDC_CHECK_ELECTRICAL, m_bCheckElectrical);
	DDX_Check(pDX, IDC_CHECK_CUSTOM, m_bCheckCustom);
	DDX_Text(pDX, IDC_EDIT_XCOUNT, m_nEditXCount);
	DDX_Text(pDX, IDC_EDIT_YCOUNT, m_nEditYCount);
	DDX_Check(pDX, IDC_CHECK_SHEETENABLE, m_bCheckSheetEnable);
	DDX_Text(pDX, IDC_EDIT_SNAP, m_csEditSnap);
	DDX_Text(pDX, IDC_EDIT_VISIBLE, m_csEditVisible);
	DDX_Text(pDX, IDC_EDIT_ELECTRICAL, m_csEditElectrical);
	DDX_Text(pDX, IDC_EDIT_CUSTOMWIDTH, m_csEditCustomWidth);
	DDX_Text(pDX, IDC_EDIT_CUSTOMHEIGHT, m_csEditCustomHeight);
	DDX_Text(pDX, IDC_EDIT_MARGIN, m_csEditMargin);
}


BEGIN_MESSAGE_MAP(CPage_SchOptions, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_SHEETNUM, OnCbnSelchangeComboSheetnum)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM, OnBnClickedCheckCustom)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ALLON, OnBnClickedButtonAllon)
	ON_BN_CLICKED(IDC_BUTTON_USEDON, OnBnClickedButtonUsedon)
	ON_STN_CLICKED(IDC_STATIC_BORDERCOLOR, OnStnClickedStaticBordercolor)
	ON_STN_CLICKED(IDC_STATIC_SHEETCOLOR, OnStnClickedStaticSheetcolor)
	ON_BN_CLICKED(IDC_BUTTON_BORDERCOLOR_DEFAULT, OnBnClickedButtonBordercolorDefault)
	ON_BN_CLICKED(IDC_BUTTON_SHEETCOLOR_DEFAULT, OnBnClickedButtonSheetcolorDefault)
	ON_BN_CLICKED(IDC_BUTTON_USER_SHEET, OnBnClickedButtonUserSheet)
END_MESSAGE_MAP()


// CPage_SchOptions 消息处理程序

void CPage_SchOptions::OnCbnSelchangeComboSheetnum()
{
	int old_diagram, new_diagram;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SHEETNUM);
	if(pCombo == NULL) return;
		
	old_diagram = m_nComboSheetNum;
	new_diagram = pCombo->GetCurSel();
	

	if(old_diagram != new_diagram)
	{

		UpdateData(TRUE);
		m_pDoc->m_csSCH_SheetName[old_diagram] = m_csEditSheetName;
		m_pDoc->m_bSCH_SheetShow[old_diagram] = m_bCheckSheetEnable;
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_nOrientation = m_nComboOrientation;
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_bTitleBlock =  m_bCheckTitle;
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_nTitleBlockStyle =  m_nComboTitle;
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_nSheetNumberSpace =  m_nEditNumberSpace;
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_bShowReferenceZones =  m_bCheckReference;
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_bShowBorder =  m_bCheckShowBorder;
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_nStandardStyle =  m_nEditStandard;
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_bCustom =  m_bCheckCustom;
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_fCustomWidth =  m_pDoc->Convert_String_To_XY(m_csEditCustomWidth);
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_fCustomHeight =  m_pDoc->Convert_String_To_XY(m_csEditCustomHeight);
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_nCustomXRegionCount =  m_nEditXCount;
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_nCustomYRegionCount =  m_nEditYCount;
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_fCustomMargin =  m_pDoc->Convert_String_To_XY(m_csEditMargin);
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_nBorderColor = m_cBorderColor.m_nColor;
		m_pDoc->m_pSCH_Sheet[old_diagram]->m_nSheetColor = m_cSheetColor.m_nColor;
		
		m_nComboSheetNum = new_diagram;
		m_csEditSheetName = m_pDoc->m_csSCH_SheetName[new_diagram];
		m_bCheckSheetEnable = m_pDoc->m_bSCH_SheetShow[new_diagram];
		m_nComboOrientation = m_pDoc->m_pSCH_Sheet[new_diagram]->m_nOrientation;
		m_bCheckTitle = m_pDoc->m_pSCH_Sheet[new_diagram]->m_bTitleBlock;
		m_nComboTitle = m_pDoc->m_pSCH_Sheet[new_diagram]->m_nTitleBlockStyle;
		m_nEditNumberSpace = m_pDoc->m_pSCH_Sheet[new_diagram]->m_nSheetNumberSpace;
		m_bCheckReference = m_pDoc->m_pSCH_Sheet[new_diagram]->m_bShowReferenceZones;
		m_bCheckShowBorder = m_pDoc->m_pSCH_Sheet[new_diagram]->m_bShowBorder;
		m_nEditStandard = m_pDoc->m_pSCH_Sheet[new_diagram]->m_nStandardStyle;
		m_bCheckCustom = m_pDoc->m_pSCH_Sheet[new_diagram]->m_bCustom;
		m_csEditCustomWidth = m_pDoc->Convert_XY_To_String(m_pDoc->m_pSCH_Sheet[new_diagram]->m_fCustomWidth);
		m_csEditCustomHeight = m_pDoc->Convert_XY_To_String(m_pDoc->m_pSCH_Sheet[new_diagram]->m_fCustomHeight);
		m_nEditXCount = m_pDoc->m_pSCH_Sheet[new_diagram]->m_nCustomXRegionCount;
		m_nEditYCount = m_pDoc->m_pSCH_Sheet[new_diagram]->m_nCustomYRegionCount;
		m_csEditMargin = m_pDoc->Convert_XY_To_String(m_pDoc->m_pSCH_Sheet[new_diagram]->m_fCustomMargin);
		m_cBorderColor.m_nColor = m_pDoc->m_pSCH_Sheet[new_diagram]->m_nBorderColor;
		m_cBorderColor.Invalidate();
		m_cSheetColor.m_nColor = m_pDoc->m_pSCH_Sheet[new_diagram]->m_nSheetColor;
		m_cSheetColor.Invalidate();

		UpdateData(FALSE);

	}
}

BOOL CPage_SchOptions::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	
	int c;
	CButton* pButton;
	CWnd* pWnd;

	pButton = (CButton*)GetDlgItem(IDC_CHECK_CUSTOM);
	if(pButton != NULL) c = pButton->GetCheck();
	else return TRUE;


    if(c == BST_UNCHECKED)
	{
		pWnd = GetDlgItem(IDC_COMBO_STANDARD);
		if(pWnd != NULL) pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_CUSTOMWIDTH);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_CUSTOMHEIGHT);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_XCOUNT);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_YCOUNT);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_MARGIN);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	}
	else if(c == BST_CHECKED)
	{
		pWnd = GetDlgItem(IDC_COMBO_STANDARD);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_CUSTOMWIDTH);
		if(pWnd != NULL) pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_CUSTOMHEIGHT);
		if(pWnd != NULL) pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_XCOUNT);
		if(pWnd != NULL) pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_YCOUNT);
		if(pWnd != NULL) pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_MARGIN);
		if(pWnd != NULL) pWnd->EnableWindow(TRUE);
	}

	
	m_cBorderColor.SubclassDlgItem(IDC_STATIC_BORDERCOLOR, this);
	m_cBorderColor.m_nColor = m_pDoc->m_pSCH_Sheet[m_nComboSheetNum]->m_nBorderColor;
	m_cSheetColor.SubclassDlgItem(IDC_STATIC_SHEETCOLOR, this);
	m_cSheetColor.m_nColor = m_pDoc->m_pSCH_Sheet[m_nComboSheetNum]->m_nSheetColor;

	
	if(m_pDoc->m_nDocumentType != DOC_ESD)
	{
		pWnd = GetDlgItem(IDC_COMBO_SHEETNUM);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_CHECK_SHEETENABLE);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_SHEETNAME);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_BUTTON_ALLON);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_BUTTON_USEDON);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	}

	return TRUE;  
	
}

void CPage_SchOptions::OnBnClickedCheckCustom()
{
	
	int c;
	CButton* pButton;
	CWnd* pWnd;

	pButton = (CButton*)GetDlgItem(IDC_CHECK_CUSTOM);
	if(pButton != NULL) c = pButton->GetCheck();
	else return;

    if(c == BST_UNCHECKED)
	{
		pWnd = GetDlgItem(IDC_COMBO_STANDARD);
		if(pWnd != NULL) pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_CUSTOMWIDTH);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_CUSTOMHEIGHT);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_XCOUNT);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_YCOUNT);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_MARGIN);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	}
	else if(c == BST_CHECKED)
	{
		pWnd = GetDlgItem(IDC_COMBO_STANDARD);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_CUSTOMWIDTH);
		if(pWnd != NULL) pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_CUSTOMHEIGHT);
		if(pWnd != NULL) pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_XCOUNT);
		if(pWnd != NULL) pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_YCOUNT);
		if(pWnd != NULL) pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_MARGIN);
		if(pWnd != NULL) pWnd->EnableWindow(TRUE);
	}
}

void CPage_SchOptions::OnBnClickedButtonUpdate()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_STANDARD);
	if(pCombo != NULL)
	{
		UpdateData(TRUE);
		int i = pCombo->GetCurSel();
		m_csEditCustomWidth = m_pDoc->Convert_XY_To_String((double)sEDA_Standard_Sheet[i].width);
		m_csEditCustomHeight = m_pDoc->Convert_XY_To_String((double)sEDA_Standard_Sheet[i].height);
		m_nEditXCount = sEDA_Standard_Sheet[i].xregioncount;
		m_nEditYCount = sEDA_Standard_Sheet[i].yregioncount;
		m_csEditMargin = m_pDoc->Convert_XY_To_String((double)sEDA_Standard_Sheet[i].margin);

		UpdateData(FALSE);
	}

}


void CPage_SchOptions::OnBnClickedButtonAllon()
{
	for(int i=0; i<SCH_SHEET_NUM; i++)
	{
		m_pDoc->m_bSCH_SheetShow[i] = TRUE;
	}

	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_SHEETENABLE);
	if(pButton != NULL) pButton->SetCheck(TRUE);
}

void CPage_SchOptions::OnBnClickedButtonUsedon()
{
	int sheetused = m_pDoc->GetSchSheetUsed();
	for(int i=0; i<SCH_SHEET_NUM; i++)
	{
		m_pDoc->m_bSCH_SheetShow[i] = (sheetused>>i)&0x01;
	}

	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_SHEETENABLE);
	if(pButton != NULL)
	{
		if(m_pDoc->m_bSCH_SheetShow[m_nComboSheetNum] == TRUE)	pButton->SetCheck(TRUE);
	}
}

void CPage_SchOptions::OnStnClickedStaticBordercolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cBorderColor.m_nColor = colordialog.GetColor();
		m_cBorderColor.Invalidate();
	}
}

void CPage_SchOptions::OnStnClickedStaticSheetcolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cSheetColor.m_nColor = colordialog.GetColor();
		m_cSheetColor.Invalidate();
	}
}

void CPage_SchOptions::OnBnClickedButtonBordercolorDefault()
{
	m_cBorderColor.m_nColor = RGB(0,0,0);
	m_cBorderColor.Invalidate();

}

void CPage_SchOptions::OnBnClickedButtonSheetcolorDefault()
{
	m_cSheetColor.m_nColor = RGB(255,255,255);
	m_cSheetColor.Invalidate();
}

void CPage_SchOptions::OnBnClickedButtonUserSheet()
{
	
	int nDiagram = m_nComboSheetNum;
	char worning_str[2000];
	sprintf(worning_str, "指定图框样板文件的首页图形将插入到指定第%d页-%s中, 确认继续?", nDiagram+1, m_csEditSheetName);

	char filename[MAX_PATH];
	strcpy(filename, csEDA_System_Path);
	strcat(filename,"\\utility\\*.eda");

	char szFileFilter[] = "图框样板文件(*.eda)|*.eda;|所有文件 (*.*) |*.*||";
	CFileDialog importdialog(TRUE,NULL,filename, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_ENABLESIZING,szFileFilter,NULL);  

	if((importdialog.DoModal() == IDOK)&&(AfxMessageBox(worning_str, MB_YESNO|MB_ICONWARNING) == IDYES))
	{
		
		CFile myfile;
		if(myfile.Open(importdialog.GetPathName(), CFile::modeRead) == TRUE)
		{
			CArchive ar(&myfile, CArchive::load);
			
			CString onelinestr;
			
			if(ar.ReadString(onelinestr) == TRUE)
			{
				if((onelinestr.Find(csESDFileHead_V1_1) < 0) && (onelinestr.Find(csESDFileHead_V1_2) < 0))
				{
					AfxMessageBox("文件格式错误, 无法读入图框样板文件。");
					ar.Close();
					myfile.Close();
					return;
				}
			}
			else
			{
				AfxMessageBox("文件格式错误, 无法读入图框样板文件。");
				ar.Close();
				myfile.Close();
				return;
			}

			
			CArray <LOGFONT, LOGFONT&>  arrayFont[SCH_SHEET_NUM];
			double fSCH_ScreenX[SCH_SHEET_NUM];
			double fSCH_ScreenY[SCH_SHEET_NUM];
			double fSCH_Scale[SCH_SHEET_NUM];
	
			
			COb_schsheet cSCH_Sheet[SCH_SHEET_NUM];

			
			BOOL bSCH_SheetShow[SCH_SHEET_NUM];
			CString csSCH_SheetName[SCH_SHEET_NUM];	
	
			int nSCH_Unit;
			BOOL bSCH_VisibleGridShow;
			double fSCH_VisibleGrid;
			BOOL bSCH_SnapGridEnable;
			double fSCH_SnapGrid;
			BOOL bSCH_ElecGridEnable;
			double fSCH_ElecGrid;

			
			for(int nCurrentDiagram=0; nCurrentDiagram<SCH_SHEET_NUM; nCurrentDiagram++)
			{
				int fontNum;
				ar >> fontNum;
				arrayFont[nCurrentDiagram].RemoveAll();
				for(int fontNo=0; fontNo<fontNum; fontNo++)
				{
					LOGFONT logfont;
					logfont.lfHeight = 10;
					logfont.lfWidth = 0;
					logfont.lfEscapement = logfont.lfOrientation = 0;
					logfont.lfWeight = FW_NORMAL;
					logfont.lfItalic = 0;
					logfont.lfUnderline = 0;
					logfont.lfStrikeOut = 0;
					logfont.lfCharSet = DEFAULT_CHARSET;
					logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
					logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
					logfont.lfQuality = PROOF_QUALITY;
					logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
					strcpy(logfont.lfFaceName, "Times New Roman");
					
					ar >> logfont.lfHeight;
					ar >> logfont.lfOrientation;
					ar >> logfont.lfUnderline;
					ar >> logfont.lfItalic;
					ar >> logfont.lfWeight;
					ar >> logfont.lfStrikeOut;
					CString csFaceName;
					ar >> csFaceName;
					strcpy(logfont.lfFaceName, csFaceName);
					arrayFont[nCurrentDiagram].Add(logfont);
				}
			}
			
			for(int nCurrentDiagram=0; nCurrentDiagram<SCH_SHEET_NUM; nCurrentDiagram++)
			{
				ar >> fSCH_ScreenX[nCurrentDiagram];			
				ar >> fSCH_ScreenY[nCurrentDiagram];			
				ar >> fSCH_Scale[nCurrentDiagram];			
				ar >> bSCH_SheetShow[nCurrentDiagram];		
				ar >> csSCH_SheetName[nCurrentDiagram];		
				cSCH_Sheet[nCurrentDiagram].Serialize(ar);	
			}
			
			ar >> nSCH_Unit;
			ar >> bSCH_SnapGridEnable;
			ar >> fSCH_SnapGrid;
			ar >> bSCH_VisibleGridShow;
			ar >> fSCH_VisibleGrid;
			ar >> bSCH_ElecGridEnable;
			ar >> fSCH_ElecGrid;

			
			for(int nCurrentDiagram=0; nCurrentDiagram<1; nCurrentDiagram++)
			{
				int num;
				ar >> num;
				for(int i=0; i<num; i++)
				{	
					int elementNo;
					ar >> elementNo;
					TRACE3("+++++++++++ num=%d i=%d elementNo=%d\n", num, i, elementNo);
					if(elementNo == SCH_ELEMENT_LINE)
					{
						
						COb_schline* pline = new COb_schline();
						pline->pDocument = m_pDoc;
						pline->Serialize(ar);
						pline->m_nDiagram = nDiagram;
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(pline);				
					}
					else if(elementNo == SCH_ELEMENT_POLYLINE)
					{
						
						COb_schpolyline* ppolyline = new COb_schpolyline();
						ppolyline->pDocument = m_pDoc;
						ppolyline->Serialize(ar);
						ppolyline->m_nDiagram = nDiagram;
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(ppolyline);
			
					}
					else if(elementNo == SCH_ELEMENT_POLYGON)
					{
						
						COb_schpolygon* ppolygon = new COb_schpolygon();
						ppolygon->pDocument = m_pDoc;
						ppolygon->Serialize(ar);
						ppolygon->m_nDiagram = nDiagram;	
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(ppolygon);
					}
					else if(elementNo == SCH_ELEMENT_JUNCTION)
					{
						
						COb_schjunc* pjunc = new COb_schjunc();
						pjunc->pDocument = m_pDoc;
						pjunc->Serialize(ar);
						pjunc->m_nDiagram = nDiagram;							
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(pjunc);
					}
					else if(elementNo == SCH_ELEMENT_ELLIPSEFILL)
					{
						
						COb_schellipsefill* pellipsefill = new COb_schellipsefill();
						pellipsefill->pDocument = m_pDoc;
						pellipsefill->Serialize(ar);
						pellipsefill->m_nDiagram = nDiagram;							
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(pellipsefill);
					}
					else if(elementNo == SCH_ELEMENT_ELLIPSE)
					{
						
						COb_schellipse* pellipse = new COb_schellipse();
						pellipse->pDocument = m_pDoc;
						pellipse->Serialize(ar);
						pellipse->m_nDiagram = nDiagram;								
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(pellipse);
					}
					else if(elementNo == SCH_ELEMENT_ARC)
					{
						
						COb_scharc* parc = new COb_scharc();
						parc->pDocument = m_pDoc;
						parc->Serialize(ar);
						parc->m_nDiagram = nDiagram;	
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(parc);
					}
					else if(elementNo == SCH_ELEMENT_PIE)
					{
						
						COb_schpie* ppie = new COb_schpie();
						ppie->pDocument = m_pDoc;
						ppie->Serialize(ar);
						ppie->m_nDiagram = nDiagram;	
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(ppie);
					}
					else if(elementNo == SCH_ELEMENT_RECT)
					{
						
						COb_schrect* prect = new COb_schrect();
						prect->pDocument = m_pDoc;
						prect->Serialize(ar);
						prect->m_nDiagram = nDiagram;
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(prect);
					}
					else if(elementNo == SCH_ELEMENT_ROUNDRECT)
					{
						
						COb_schroundrect* proundrect = new COb_schroundrect();
						proundrect->pDocument = m_pDoc;
						proundrect->Serialize(ar);
						proundrect->m_nDiagram = nDiagram;
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(proundrect);
					}
					else if(elementNo == SCH_ELEMENT_PORT)
					{
						
						COb_schport* pport = new COb_schport();
						pport->pDocument = m_pDoc;
						pport->Serialize(ar);
						pport->m_nDiagram = nDiagram;
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(pport);
					}		
					else if(elementNo == SCH_ELEMENT_POWER)
					{
						
						COb_schpower* ppower = new COb_schpower();
						ppower->pDocument = m_pDoc;
						ppower->Serialize(ar);
						ppower->m_nDiagram = nDiagram;
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(ppower);
					}	
					else if(elementNo == SCH_ELEMENT_BEZIER)
					{
						
						COb_schbezier* pbezier = new COb_schbezier();
						pbezier->pDocument = m_pDoc;
						pbezier->Serialize(ar);
						pbezier->m_nDiagram = nDiagram;				
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(pbezier);
					}	
					else if(elementNo == SCH_ELEMENT_GRAPHIC)
					{
						
						COb_schgraphic* pgraphic = new COb_schgraphic();
						pgraphic->pDocument = m_pDoc;
						pgraphic->Serialize(ar);
						pgraphic->m_nDiagram = nDiagram;	
						pgraphic->LoadBitmap();
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(pgraphic);
					}	
					else if(elementNo == SCH_ELEMENT_TEXTFRAME)
					{
						
						COb_schtextframe* ptextframe = new COb_schtextframe();
						ptextframe->pDocument = m_pDoc;
						ptextframe->Serialize(ar);
						ptextframe->m_nDiagram = nDiagram;
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(ptextframe);
					
						LOGFONT logfont = arrayFont[0].GetAt(ptextframe->m_nFont);
						int font_index = m_pDoc->CheckFontExist(&logfont, nDiagram);
						if(font_index < 0) font_index = m_pDoc->m_arrayFont[nDiagram].Add(logfont);
						ptextframe->m_nFont = font_index;

					}
					else if(elementNo == SCH_ELEMENT_TEXT)
					{
						
						COb_schtext* ptext = new COb_schtext();
						ptext->pDocument = m_pDoc;
						ptext->Serialize(ar);
						ptext->m_nDiagram = nDiagram;
						
						m_pDoc->m_listSchObject[nDiagram].AddTail(ptext);

						LOGFONT logfont = arrayFont[0].GetAt(ptext->m_nFont);
						int font_index = m_pDoc->CheckFontExist(&logfont, nDiagram);
						if(font_index < 0) font_index = m_pDoc->m_arrayFont[nDiagram].Add(logfont);
						ptext->m_nFont = font_index;
					}
					else if(elementNo == SCH_ELEMENT_SHEETSYMBOL)
					{
						
						COb_schsheetsymbol* psheetsymbol = new COb_schsheetsymbol();
						psheetsymbol->pDocument = m_pDoc;
						psheetsymbol->Serialize(ar);
						psheetsymbol->m_nDiagram = nDiagram;	
						
						POSITION posParent = m_pDoc->m_listSchObject[nDiagram].AddTail(psheetsymbol);

						int subelementNum;
						ar >> subelementNum;
						for(int k=0; k<subelementNum; k++)
						{
							int subelementNo;
							ar >> subelementNo;
							if(subelementNo == SCH_ELEMENT_SHEETENTRY)
							{
								
								COb_schsheetentry* psheetentry = new COb_schsheetentry();
								psheetentry->pDocument = m_pDoc;
								psheetentry->Serialize(ar);
								psheetentry->m_nDiagram = nDiagram;	
								psheetentry->m_posParent = posParent;
								
								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(psheetentry);

								
								psheetsymbol->m_arrayPos.Add(pos);
							}
							else if(subelementNo == SCH_ELEMENT_TEXT)
							{
								
								COb_schtext* ptext = new COb_schtext();
								ptext->pDocument = m_pDoc;
								ptext->Serialize(ar);
								ptext->m_nDiagram = nDiagram;
								ptext->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(ptext);
								
								psheetsymbol->m_arrayPos.Add(pos);

								LOGFONT logfont = arrayFont[0].GetAt(ptext->m_nFont);
								int font_index = m_pDoc->CheckFontExist(&logfont, nDiagram);
								if(font_index < 0) font_index = m_pDoc->m_arrayFont[nDiagram].Add(logfont);
								ptext->m_nFont = font_index;
							}
						}

						i += subelementNum;
					}
					else if(elementNo == SCH_ELEMENT_COMPONENT)
					{
						
						COb_schcomp* pcomp = new COb_schcomp();
						pcomp->pDocument = m_pDoc;
						pcomp->Serialize(ar);
						pcomp->m_nDiagram = nDiagram;

						
						POSITION posParent = m_pDoc->m_listSchObject[nDiagram].AddTail(pcomp);
					
						int subelementNum;
						ar >> subelementNum;
						for(int k=0; k<subelementNum; k++)
						{
							
							ar >> elementNo;
							if(elementNo == SCH_ELEMENT_PIN)
							{
								
								COb_schpin* ppin = new COb_schpin();
								ppin->pDocument = m_pDoc;
								ppin->Serialize(ar);
								ppin->m_nDiagram = nDiagram;
								ppin->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(ppin);
					
								
								pcomp->m_arrayPos.Add(pos);
							}
							else if(elementNo == SCH_ELEMENT_IEEE)
							{
								
								COb_schieee* pieee = new COb_schieee();
								pieee->pDocument = m_pDoc;
								pieee->Serialize(ar);
								pieee->m_nDiagram = nDiagram;
								pieee->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(pieee);

								
								pcomp->m_arrayPos.Add(pos);
							}
							else if(elementNo == SCH_ELEMENT_ARC)
							{
								
								COb_scharc* parc = new COb_scharc();
								parc->pDocument = m_pDoc;
								parc->Serialize(ar);
								parc->m_nDiagram = nDiagram;	
								parc->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(parc);

								
								if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
							}
							else if(elementNo == SCH_ELEMENT_ELLIPSE)
							{
								
								COb_schellipse* pellipse = new COb_schellipse();
								pellipse->pDocument = m_pDoc;
								pellipse->Serialize(ar);
								pellipse->m_nDiagram = nDiagram;	
								pellipse->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(pellipse);

								
								if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
							}
							else if(elementNo == SCH_ELEMENT_ELLIPSEFILL)
							{
								
								COb_schellipsefill* pellipsefill = new COb_schellipsefill();
								pellipsefill->pDocument = m_pDoc;
								pellipsefill->Serialize(ar);
								pellipsefill->m_nDiagram = nDiagram;
								pellipsefill->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(pellipsefill);

								
								if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
							}
							else if(elementNo == SCH_ELEMENT_PIE)
							{
								
								COb_schpie* ppie = new COb_schpie();
								ppie->pDocument = m_pDoc;
								ppie->Serialize(ar);
								ppie->m_nDiagram = nDiagram;	
								ppie->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(ppie);

								
								if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
							}
							else if(elementNo == SCH_ELEMENT_POLYLINE)
							{
								
								COb_schpolyline* ppolyline = new COb_schpolyline();
								ppolyline->pDocument = m_pDoc;
								ppolyline->Serialize(ar);
								ppolyline->m_nDiagram = nDiagram;
								ppolyline->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(ppolyline);

								
								if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
							}
							else if(elementNo == SCH_ELEMENT_RECT)
							{
								
								COb_schrect* prect = new COb_schrect();
								prect->pDocument = m_pDoc;
								prect->Serialize(ar);
								prect->m_nDiagram = nDiagram;
								prect->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(prect);

								
								if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
							}
							else if(elementNo == SCH_ELEMENT_ROUNDRECT)
							{
								
								COb_schroundrect* proundrect = new COb_schroundrect();
								proundrect->pDocument = m_pDoc;
								proundrect->Serialize(ar);
								proundrect->m_nDiagram = nDiagram;
								proundrect->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(proundrect);

								if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
							}
							else if(elementNo == SCH_ELEMENT_POLYGON)
							{
								
								COb_schpolygon* ppolygon = new COb_schpolygon();
								ppolygon->pDocument = m_pDoc;
								ppolygon->Serialize(ar);
								ppolygon->m_nDiagram = nDiagram;	
								ppolygon->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(ppolygon);

								
								if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
							}
							else if(elementNo == SCH_ELEMENT_BEZIER)
							{
								
								COb_schbezier* pbezier = new COb_schbezier();
								pbezier->pDocument = m_pDoc;
								pbezier->Serialize(ar);
								pbezier->m_nDiagram = nDiagram;	
								pbezier->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(pbezier);

								
								if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
							}
							else if(elementNo == SCH_ELEMENT_TEXTFRAME)
							{
								
								COb_schtextframe* ptextframe = new COb_schtextframe();
								ptextframe->pDocument = m_pDoc;
								ptextframe->Serialize(ar);
								ptextframe->m_nDiagram = nDiagram;
								ptextframe->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(ptextframe);
								
								if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
								
								LOGFONT logfont = arrayFont[0].GetAt(ptextframe->m_nFont);
								int font_index = m_pDoc->CheckFontExist(&logfont, nDiagram);
								if(font_index < 0) font_index = m_pDoc->m_arrayFont[nDiagram].Add(logfont);
								ptextframe->m_nFont = font_index;
							}		
							else if(elementNo == SCH_ELEMENT_GRAPHIC)
							{
								
								COb_schgraphic* pgraphic = new COb_schgraphic();
								pgraphic->pDocument = m_pDoc;
								pgraphic->Serialize(ar);
								pgraphic->m_nDiagram = nDiagram;	
								pgraphic->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(pgraphic);

								
								if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
							}					
							else if(elementNo == SCH_ELEMENT_TEXT)
							{
								
								COb_schtext* ptext = new COb_schtext();
								ptext->pDocument = m_pDoc;
								ptext->Serialize(ar);
								ptext->m_nDiagram = nDiagram;
								ptext->m_posParent = posParent;

								
								POSITION pos = m_pDoc->m_listSchObject[nDiagram].AddTail(ptext);
								
								pcomp->m_arrayPos.Add(pos);
								
								LOGFONT logfont = arrayFont[0].GetAt(ptext->m_nFont);
								int font_index = m_pDoc->CheckFontExist(&logfont, nDiagram);
								if(font_index < 0) font_index = m_pDoc->m_arrayFont[nDiagram].Add(logfont);
								ptext->m_nFont = font_index;
							}
						}
						i += subelementNum;
					}
				}
			}


			ar.Flush();
			ar.Close();
			myfile.Close();

			
	
			m_bCheckSheetEnable = TRUE;
			m_nComboOrientation = cSCH_Sheet[0].m_nOrientation;
			m_bCheckTitle = cSCH_Sheet[0].m_bTitleBlock;
			m_nComboTitle = cSCH_Sheet[0].m_nTitleBlockStyle;
			m_nEditNumberSpace = cSCH_Sheet[0].m_nSheetNumberSpace;
			m_bCheckReference = cSCH_Sheet[0].m_bShowReferenceZones;
			m_bCheckShowBorder = cSCH_Sheet[0].m_bShowBorder;
			m_nEditStandard = cSCH_Sheet[0].m_nStandardStyle;
			m_bCheckCustom = cSCH_Sheet[0].m_bCustom;
			m_csEditCustomWidth = m_pDoc->Convert_XY_To_String(cSCH_Sheet[0].m_fCustomWidth);
			m_csEditCustomHeight = m_pDoc->Convert_XY_To_String(cSCH_Sheet[0].m_fCustomHeight);
			m_nEditXCount = cSCH_Sheet[0].m_nCustomXRegionCount;
			m_nEditYCount = cSCH_Sheet[0].m_nCustomYRegionCount;
			m_csEditMargin = m_pDoc->Convert_XY_To_String(cSCH_Sheet[0].m_fCustomMargin);

			EndDialog(IDOK);
		}
	}
}

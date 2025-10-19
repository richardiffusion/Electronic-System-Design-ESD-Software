// Dlg_EditComp.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditComp.h"
#include ".\dlg_editcomp.h"
#include "systemext.h"
#include "Dlg_PcbLib_Browse.h"

// CDlg_EditComp 对话框

IMPLEMENT_DYNAMIC(CDlg_EditComp, CDialog)
CDlg_EditComp::CDlg_EditComp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditComp::IDD, pParent)
	, m_csEditDesignator(_T(""))
	, m_csEditComment(_T(""))
	, m_bCheckDesignatorVisible(FALSE)
	, m_bCheckCommentVisible(FALSE)
	, m_csEditDescription(_T(""))
	, m_csEditUniqueID(_T(""))
	, m_nComboType(0)
	, m_csEditLibrary(_T(""))
	, m_csEditModelName(_T(""))
	, m_nComboOrientation(0)
	, m_nComboDisplayMode(0)
	, m_bCheckLocalColors(FALSE)
	, m_bCheckShowAllPins(FALSE)
	, m_bCheckLockPins(FALSE)
	, m_bCheckMirrored(FALSE)
	, m_csEditPackage(_T(""))
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
{
}

CDlg_EditComp::~CDlg_EditComp()
{
}

void CDlg_EditComp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DESIGNATOR, m_csEditDesignator);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_csEditComment);
	DDX_Check(pDX, IDC_CHECK_DESIGNATOR_VISIBLE, m_bCheckDesignatorVisible);
	DDX_Check(pDX, IDC_CHECK_COMMENT_VISIBLE, m_bCheckCommentVisible);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_csEditDescription);
	DDX_Text(pDX, IDC_EDIT_UNIQUE_ID, m_csEditUniqueID);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nComboType);
	DDX_Text(pDX, IDC_EDIT_LIBRARY, m_csEditLibrary);
	DDX_Text(pDX, IDC_EDIT_MODELNAME, m_csEditModelName);
	DDX_CBIndex(pDX, IDC_COMBO_ORIENTATION, m_nComboOrientation);
	DDX_CBIndex(pDX, IDC_COMBO_MODE, m_nComboDisplayMode);
	DDX_Check(pDX, IDC_CHECK_LOCAL_COLORS, m_bCheckLocalColors);
	DDX_Check(pDX, IDC_CHECK_SHOW_ALLPINS, m_bCheckShowAllPins);
	DDX_Check(pDX, IDC_CHECK_LOCKPINS, m_bCheckLockPins);
	DDX_Check(pDX, IDC_CHECK_MIRRORED, m_bCheckMirrored);
	DDX_Text(pDX, IDC_EDIT_PACKAGE, m_csEditPackage);
	DDX_Control(pDX, IDC_LIST_TEXTOTHERS, m_listCtrl);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
}


BEGIN_MESSAGE_MAP(CDlg_EditComp, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_LOCAL_COLORS, OnBnClickedCheckLocalColors)
	ON_STN_CLICKED(IDC_STATIC_PINCOLOR, OnStnClickedStaticPincolor)
	ON_STN_CLICKED(IDC_STATIC_LINECOLOR, OnStnClickedStaticLinecolor)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, OnStnClickedStaticFillcolor)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSEPACKAGE, OnBnClickedButtonChoosepackage)
	ON_BN_CLICKED(IDC_BUTTON_BACKWORD, OnBnClickedButtonBackword)
	ON_BN_CLICKED(IDC_BUTTON_FORWORD, OnBnClickedButtonForword)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_EDITPINS, OnBnClickedButtonEditpins)
	ON_EN_CHANGE(IDC_EDIT_PACKAGE, OnEnChangeEditPackage)
	ON_BN_CLICKED(IDC_BUTTON_ID_GLOBAL, &CDlg_EditComp::OnBnClickedButtonIdGlobal)
	ON_BN_CLICKED(IDC_BUTTON_COMMENT_GLOBAL, &CDlg_EditComp::OnBnClickedButtonCommentGlobal)
END_MESSAGE_MAP()


void CDlg_EditComp::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	if(m_nPartCount > 1)
	{
		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_STATIC_PARTNUM);
		CString str;
		str.Format("单元 %d/%d", m_nPart, m_nPartCount);
		pWnd->SetWindowText(str);
	}

	if(m_bCheckLocalColors == FALSE)
	{
		CWnd  *pWnd;
		pWnd = (CWnd*)GetDlgItem(IDC_STATIC_FILL);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = (CWnd*)GetDlgItem(IDC_STATIC_LINE);	
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PIN);	
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = (CWnd*)GetDlgItem(IDC_STATIC_FILLCOLOR);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = (CWnd*)GetDlgItem(IDC_STATIC_LINECOLOR);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PINCOLOR);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
	}

	ShowPackage();
}

void CDlg_EditComp::OnBnClickedCheckLocalColors()
{
	CButton* pButton;
	pButton = (CButton*)GetDlgItem(IDC_CHECK_LOCAL_COLORS);
	if(pButton != NULL)
	{
		if(pButton->GetCheck() == BST_UNCHECKED)
		{
			m_bCheckLocalColors = FALSE;
			CWnd  *pWnd;
			pWnd = (CWnd*)GetDlgItem(IDC_STATIC_FILL);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = (CWnd*)GetDlgItem(IDC_STATIC_LINE);	
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PIN);	
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = (CWnd*)GetDlgItem(IDC_STATIC_FILLCOLOR);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = (CWnd*)GetDlgItem(IDC_STATIC_LINECOLOR);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PINCOLOR);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			Invalidate();
		}
		else
		{
			m_bCheckLocalColors = TRUE;
			CWnd  *pWnd;
			pWnd = (CWnd*)GetDlgItem(IDC_STATIC_FILL);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = (CWnd*)GetDlgItem(IDC_STATIC_LINE);	
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PIN);	
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = (CWnd*)GetDlgItem(IDC_STATIC_FILLCOLOR);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = (CWnd*)GetDlgItem(IDC_STATIC_LINECOLOR);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PINCOLOR);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);	
			Invalidate();
		}
	}
}

void CDlg_EditComp::OnStnClickedStaticPincolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cPinColor.m_nColor = colordialog.GetColor();
		m_nPinColor = m_cPinColor.m_nColor;
		m_cPinColor.Invalidate();
	}
}

void CDlg_EditComp::OnStnClickedStaticLinecolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cLineColor.m_nColor = colordialog.GetColor();
		m_nLineColor = m_cLineColor.m_nColor;
		m_cLineColor.Invalidate();
	}
}

void CDlg_EditComp::OnStnClickedStaticFillcolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cFillColor.m_nColor = colordialog.GetColor();
		m_nFillColor = m_cFillColor.m_nColor;
		m_cFillColor.Invalidate();
	}
}

BOOL CDlg_EditComp::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cFillColor.SubclassDlgItem(IDC_STATIC_FILLCOLOR, this);
	m_cFillColor.m_nColor = m_nFillColor;
	m_cLineColor.SubclassDlgItem(IDC_STATIC_LINECOLOR, this);
	m_cLineColor.m_nColor = m_nLineColor;
	m_cPinColor.SubclassDlgItem(IDC_STATIC_PINCOLOR, this);
	m_cPinColor.m_nColor = m_nPinColor;

	m_listCtrl.InsertColumn(0, _T("显示"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(1, _T("条目"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(2, _T("值"), LVCFMT_LEFT, 100);
	
	int iItem;
	for (iItem = 0; iItem < m_arrayTextOthers.GetCount(); ++iItem)
	{
		//CString strItem;
		CString strItem = m_arrayTextOthers[iItem].item;
		CString strValue = m_arrayTextOthers[iItem].value;

		m_listCtrl.InsertItem(iItem, "", 0);
		m_listCtrl.SetItem(iItem, 1, LVIF_TEXT, strItem, 0, NULL, NULL, NULL);
		m_listCtrl.SetItem(iItem, 2, LVIF_TEXT, strValue, 0, NULL, NULL, NULL);
	}

	m_listCtrl.SetColumnWidth(0, 40);
	m_listCtrl.SetColumnWidth(1, 110);
	m_listCtrl.SetColumnWidth(2, 280);

	//Enable Full Row Selection
	m_listCtrl.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT);

	//Enable Grid Lines
	m_listCtrl.ModifyExtendedStyle(0, LVS_EX_GRIDLINES);

	//Enable Check Box
	m_listCtrl.ModifyExtendedStyle( 0, LVS_EX_CHECKBOXES);

	//Enable Drag Drop
	m_listCtrl.ModifyExtendedStyle(0, LVS_EX_HEADERDRAGDROP);

	for (iItem = 0; iItem < m_arrayTextOthers.GetCount(); ++iItem)
	{
		ListView_SetCheckState(m_listCtrl.m_hWnd, iItem, m_arrayTextOthers[iItem].visible);
	}

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MODE);
	for(int i=0; i<m_nDisplayModeCount; i++)
	{
		CString cstring;
		if(i == 0)
		{
			pCombo->AddString("标准");
		}
		else
		{
			cstring.Format("可选%d", i);
			pCombo->AddString(cstring);
		}
	}
	if(m_nComboDisplayMode < pCombo->GetCount() ) pCombo->SetCurSel(m_nComboDisplayMode);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditComp::OnBnClickedButtonChoosepackage()
{
	Struct_CompList complist;
	CDlg_PcbLib_Browse dlg2;
	dlg2.m_bMatchMore = bEDA_PcbMatchMore;
		
	if(dlg2.DoModal() != IDOK) return;

	bEDA_PcbMatchMore = dlg2.m_bMatchMore;
	complist = dlg2.m_CompList;

	int pcblib_number;
	for(pcblib_number=0; pcblib_number<PCB_SYSTEM_LIB_NUMBER; pcblib_number++)
	{
		if(pcb_library_offset[pcblib_number] == complist.liboffset) break;
	}

	m_csEditLibrary = pcb_library_name[pcblib_number];
	m_csEditPackage = complist.compname;

	CWnd* pWnd = GetDlgItem(IDC_EDIT_LIBRARY);
	pWnd->SetWindowText(m_csEditLibrary);
	pWnd = GetDlgItem(IDC_EDIT_PACKAGE);
	pWnd->SetWindowText(m_csEditPackage);

	ShowPackage();
}

void CDlg_EditComp::OnBnClickedButtonBackword()
{
	if(m_nPartCount > 1)
	{
		if(m_nPart > 1)
		{
			m_nPart = 1;
			CWnd* pWnd;
			pWnd = GetDlgItem(IDC_STATIC_PARTNUM);
			CString str;
			str.Format("单元 %d/%d", m_nPart, m_nPartCount);
			pWnd->SetWindowText(str);
			pWnd->Invalidate();
		}
	}
}

void CDlg_EditComp::OnBnClickedButtonForword()
{
	if(m_nPartCount > 1)
	{
		if(m_nPart < m_nPartCount)
		{
			m_nPart = m_nPartCount;
			CWnd* pWnd;
			pWnd = GetDlgItem(IDC_STATIC_PARTNUM);
			CString str;
			str.Format("单元 %d/%d", m_nPart, m_nPartCount);
			pWnd->SetWindowText(str);
			pWnd->Invalidate();
		}
	}
}

void CDlg_EditComp::OnBnClickedButtonNext()
{
	if(m_nPartCount > 1)
	{
		if(m_nPart < m_nPartCount)
		{
			m_nPart++;
			CWnd* pWnd;
			pWnd = GetDlgItem(IDC_STATIC_PARTNUM);
			CString str;
			str.Format("单元 %d/%d", m_nPart, m_nPartCount);
			pWnd->SetWindowText(str);
			pWnd->Invalidate();
		}
	}
}

void CDlg_EditComp::OnBnClickedButtonPrev()
{
	if(m_nPartCount > 1)
	{
		if(m_nPart > 1)
		{
			m_nPart--;
			CWnd* pWnd;
			pWnd = GetDlgItem(IDC_STATIC_PARTNUM);
			CString str;
			str.Format("单元 %d/%d", m_nPart, m_nPartCount);
			pWnd->SetWindowText(str);
			pWnd->Invalidate();
		}
	}
}

void CDlg_EditComp::OnBnClickedOk()
{
	int iItem;
	for (iItem = 0; iItem < m_arrayTextOthers.GetCount(); ++iItem)
	{
		m_arrayTextOthers[iItem].visible = ListView_GetCheckState(m_listCtrl.m_hWnd, iItem);
        m_arrayTextOthers[iItem].item = m_listCtrl.GetItemText(iItem, 1);
		m_arrayTextOthers[iItem].value = m_listCtrl.GetItemText(iItem, 2);
	}
	OnOK();
}

void CDlg_EditComp::OnBnClickedButtonEditpins()
{
	int iItem;
	for (iItem = 0; iItem < m_arrayTextOthers.GetCount(); ++iItem)
	{
		m_arrayTextOthers[iItem].visible = ListView_GetCheckState(m_listCtrl.m_hWnd, iItem);
        m_arrayTextOthers[iItem].item = m_listCtrl.GetItemText(iItem, 1);
		m_arrayTextOthers[iItem].value = m_listCtrl.GetItemText(iItem, 2);
	}
	UpdateData(TRUE);
	

	int nRet = 100; //ID_EDITPIN

	EndDialog(nRet);
}

void CDlg_EditComp::ShowPackage()
{
	if(m_csEditPackage.IsEmpty() == TRUE)
	{
		CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PACKAGE_PREVIEW);
		RECT rect;
		pWnd->GetClientRect(&rect);
			
		rect.left++;
		rect.right--;
		rect.top++;
		rect.bottom--;
			
		CClientDC dc(pWnd);
		dc.FillSolidRect( rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, RGB(254,251,218));
		dc.IntersectClipRect(&rect);

	}
	else
	{
		Struct_CompList complist;
		complist.liboffset = -1;
		if(m_csEditLibrary.IsEmpty() != TRUE)
		{
			int pcblib_number;
			for(pcblib_number=0; pcblib_number<PCB_SYSTEM_LIB_NUMBER; pcblib_number++)
			{
				if(m_csEditLibrary.CompareNoCase(pcb_library_name[pcblib_number]) == 0) break;
			}

			if(pcblib_number < PCB_SYSTEM_LIB_NUMBER)
			{
				for(int i=0; i<arrayEDA_PcbLibraryCompName[pcblib_number].GetCount(); i++)
				{
					complist = arrayEDA_PcbLibraryCompName[pcblib_number].GetAt(i);
					if(m_csEditPackage.CompareNoCase(complist.compname) == 0) break;
				}
			}
		}

		if(complist.liboffset < 0)
		{
			for(int i=0; i<PCB_SYSTEM_LIB_NUMBER; i++)
			{
				for(int j=0; j<arrayEDA_PcbLibraryCompName[i].GetCount(); j++)
				{
					Struct_CompList tempcomplist;
					tempcomplist = arrayEDA_PcbLibraryCompName[i].GetAt(j);
					if(m_csEditPackage.CompareNoCase(tempcomplist.compname) == 0)
					{
						complist.basiclibrary = tempcomplist.basiclibrary;
						strcpy(complist.compname,tempcomplist.compname);
						complist.length = tempcomplist.length;
						complist.liboffset = tempcomplist.liboffset;
						complist.offset = tempcomplist.offset;
						break;
					}
				}

				if(complist.liboffset >= 0) break;
			}
		}

		if(complist.liboffset < 0)
		{
			CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PACKAGE_PREVIEW);
			RECT rect;
			pWnd->GetClientRect(&rect);
				
			rect.left++;
			rect.right--;
			rect.top++;
			rect.bottom--;
				
			CClientDC dc(pWnd);
			dc.FillSolidRect( rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, RGB(254,251,218));
			dc.IntersectClipRect(&rect);

		}
		else
		{
			double fminx, fminy, fmaxx, fmaxy;
			CTypedPtrArray <CObArray, COb_pcbarc*> arrayPcbArc;
			CTypedPtrArray <CObArray, COb_pcbtrack*> arrayPcbTrack;
			CTypedPtrArray <CObArray, COb_pcbtext*> arrayPcbText;
			CTypedPtrArray <CObArray, COb_pcbfill*> arrayPcbFill;
			CTypedPtrArray <CObArray, COb_pcbregion*> arrayPcbRegion;	
			CTypedPtrArray <CObArray, COb_pcbpad*> arrayPcbPad;
			CTypedPtrArray <CObArray, COb_pcbvia*> arrayPcbVia;
			CString csPackageName, csDescription;

			int offset, length;

			unsigned char* pbuffer;

			if(complist.basiclibrary == TRUE)  
			{
				
				offset = complist.liboffset + complist.offset;
				length = complist.length;
		 
				pbuffer = (unsigned char*)malloc(length + 100); 

				cfileEDA_PcbLibrary.Seek(offset, CFile::begin);
				cfileEDA_PcbLibrary.Read(pbuffer, length);
			}
			else 
			{
				
				
				CString userlibrary, compname;
				userlibrary = arrayEDA_PcbUserLibraryName.GetAt(complist.liboffset);
				
				char libstring[MAX_PATH];
				sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, userlibrary);

				CFile cfileUserLibrary;
				if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE)  return;
				CArchive ar(&cfileUserLibrary, CArchive::load);
				
				CString headformat;
				ar >> headformat;
				if(headformat != PCB_Library_Head)	{	ar.Close();	cfileUserLibrary.Close();	return;	}
				int totalnum;
				ar >> totalnum;
				ar.Close();

				
				cfileUserLibrary.Seek(32 + 320*complist.offset, CFile::begin);

				Struct_Comp_Head comp_head;
				cfileUserLibrary.Read(&comp_head, 320);
				offset = comp_head.offset;
				length = comp_head.length;

				pbuffer = (unsigned char*)malloc(length + 100); //new unsigned char[250000];

				cfileUserLibrary.Seek(offset, CFile::begin);
				cfileUserLibrary.Read(pbuffer, length);

				cfileUserLibrary.Close();
			}
			
			CMemFile memfile(pbuffer, length);
			CArchive ar(&memfile,CArchive::load);
			
			fminx = fminy = 100000;
			fmaxx = fmaxy = -100000;
			int arcnum, tracknum, fillnum, padnum, vianum, textnum, regionnum;

			ar >> arcnum;
			for(int i=0; i<arcnum; i++)
			{
				COb_pcbarc* parc = new COb_pcbarc();
				parc->Serialize(ar);
				arrayPcbArc.Add(parc);
				if((parc->m_fSangle == parc->m_fEangle)||((parc->m_fEangle - parc->m_fSangle) == 360))
				{
					fminx = MIN(fminx, (parc->m_fCx - parc->m_fRadius));
					fminy = MIN(fminy, (parc->m_fCy - parc->m_fRadius));
					fmaxx = MAX(fmaxx, (parc->m_fCx + parc->m_fRadius));
					fmaxy = MAX(fmaxy, (parc->m_fCy + parc->m_fRadius));
				}
				else if(parc->m_fEangle > parc->m_fSangle)
				{
					if((180 > parc->m_fSangle)&&(180 < parc->m_fEangle)) fminx = MIN(fminx, (parc->m_fCx - parc->m_fRadius));
					else fminx = MIN(fminx, parc->m_fCx);

					if((270 > parc->m_fSangle)&&(270 < parc->m_fEangle)) fminy = MIN(fminy, (parc->m_fCy - parc->m_fRadius));
					else fminy = MIN(fminy, parc->m_fCy);

					fmaxx = MAX(fmaxx, parc->m_fCx);

					if((90 > parc->m_fSangle)&&(90 < parc->m_fEangle)) fmaxy = MAX(fmaxy, (parc->m_fCy + parc->m_fRadius));
					else fmaxy = MAX(fmaxy, parc->m_fCy);
				}
				else
				{
					if((180 > parc->m_fSangle)||(180 < parc->m_fEangle)) fminx = MIN(fminx, (parc->m_fCx - parc->m_fRadius));
					else fminx = MIN(fminx, parc->m_fCx);

					if((270 > parc->m_fSangle)||(270 < parc->m_fEangle)) fminy = MIN(fminy, (parc->m_fCy - parc->m_fRadius));
					else fminy = MIN(fminy, parc->m_fCy);

					fmaxx = MAX(fmaxx, (parc->m_fCx + parc->m_fRadius));

					if((90 > parc->m_fSangle)||(90 < parc->m_fEangle)) fmaxy = MAX(fmaxy, (parc->m_fCy + parc->m_fRadius));
					else fmaxy = MAX(fmaxy, parc->m_fCy);
				}
			}
			ar >> tracknum;
			for(int i=0; i<tracknum; i++)
			{
				COb_pcbtrack* ptrack = new COb_pcbtrack();
				ptrack->Serialize(ar);
				arrayPcbTrack.Add(ptrack);
				fminx = MIN(fminx, ptrack->m_fX1);
				fminy = MIN(fminy, ptrack->m_fY1);
				fmaxx = MAX(fmaxx, ptrack->m_fX2);
				fmaxy = MAX(fmaxy, ptrack->m_fY2);
			}
			ar >> fillnum;
			for(int i=0; i<fillnum; i++)
			{
				COb_pcbfill* pfill = new COb_pcbfill();
				pfill->Serialize(ar);
				arrayPcbFill.Add(pfill);
				fminx = MIN(fminx, pfill->m_fX1);
				fminy = MIN(fminy, pfill->m_fY1);
				fmaxx = MAX(fmaxx, pfill->m_fX2);
				fmaxy = MAX(fmaxy, pfill->m_fY2);
			}
			ar >> padnum;
			for(int i=0; i<padnum; i++)
			{
				COb_pcbpad* ppad = new COb_pcbpad();
				ppad->Serialize(ar);
				arrayPcbPad.Add(ppad);
				fminx = MIN(fminx, ppad->m_fCx - ppad->m_fSizeX[0]);
				fminy = MIN(fminy, ppad->m_fCy - ppad->m_fSizeY[0]);
				fmaxx = MAX(fmaxx, ppad->m_fCx + ppad->m_fSizeX[0]);
				fmaxy = MAX(fmaxy, ppad->m_fCy + ppad->m_fSizeY[0]);
			}
			ar >> vianum;
			for(int i=0; i<vianum; i++)
			{
				COb_pcbvia* pvia = new COb_pcbvia();
				pvia->Serialize(ar);
				arrayPcbVia.Add(pvia);
				fminx = MIN(fminx, pvia->m_fCx - pvia->m_fDia);
				fminy = MIN(fminy, pvia->m_fCy - pvia->m_fDia);
				fmaxx = MAX(fmaxx, pvia->m_fCx + pvia->m_fDia);
				fmaxy = MAX(fmaxy, pvia->m_fCy + pvia->m_fDia);
			}
			ar >> textnum;
			for(int i=0; i<textnum; i++)
			{
				COb_pcbtext* ptext = new COb_pcbtext();
				ptext->Serialize(ar);
				arrayPcbText.Add(ptext);
				fminx = MIN(fminx, ptext->m_fSx);
				fminy = MIN(fminy, ptext->m_fSy);
				fmaxx = MAX(fmaxx, ptext->m_fSx + ptext->m_csText.GetLength()*ptext->m_fHeight);
				fmaxy = MAX(fmaxy, ptext->m_fSy + ptext->m_fHeight);
			}
			ar >> regionnum;
			for(int i=0; i<regionnum; i++)
			{
				COb_pcbregion* pregion = new COb_pcbregion();
				pregion->Serialize(ar);
				arrayPcbRegion.Add(pregion);
				for(int k=0;k <pregion->m_cVertex.GetCount(); k++)
				{
					fminx = MIN(fminx, pregion->m_cVertex[k].fx);
					fminy = MIN(fminy, pregion->m_cVertex[k].fy);
					fmaxx = MAX(fmaxx, pregion->m_cVertex[k].fx);
					fmaxy = MAX(fmaxy, pregion->m_cVertex[k].fy);
				}
			}

			ar.Close();
			memfile.Close();
			free(pbuffer);

			CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PACKAGE_PREVIEW);
			RECT rect;
			pWnd->GetClientRect(&rect);
			
			rect.left++;
			rect.right--;
			rect.top++;
			rect.bottom--;
			
			CClientDC dc(pWnd);
			dc.FillSolidRect( rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, nEDA_PCB_BoardAreaColor);
			dc.IntersectClipRect(&rect);

			int nEx = rect.right-rect.left;
			int nEy = rect.bottom-rect.top;

			double scale1 = (fmaxx - fminx)*1.2/nEx;
			double scale2 = (fmaxy - fminy)*1.2/nEy;
			double scale;
			if(scale1 > scale2) scale = scale1;
			else scale = scale2;

			double origin_x = fminx - (nEx - (fmaxx - fminx)/scale)/2*scale;
			double origin_y = fminy - (nEy - (fmaxy - fminy)/scale)/2*scale;

			
			for(int i=0; i<arrayPcbArc.GetCount(); i++)
			{
				COb_pcbarc* parc = arrayPcbArc.GetAt(i);
				parc->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
			for(int i=0; i<arrayPcbTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = arrayPcbTrack.GetAt(i);
				ptrack->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}	
			for(int i=0; i<arrayPcbPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = arrayPcbPad.GetAt(i);
				ppad->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
			for(int i=0; i<arrayPcbVia.GetCount(); i++)
			{
				COb_pcbvia* pvia = arrayPcbVia.GetAt(i);
				pvia->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
			for(int i=0; i<arrayPcbFill.GetCount(); i++)
			{
				COb_pcbfill* pfill = arrayPcbFill.GetAt(i);
				pfill->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
			for(int i=0; i<arrayPcbText.GetCount(); i++)
			{
				COb_pcbtext* ptext = arrayPcbText.GetAt(i);
				ptext->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
			for(int i=0; i<arrayPcbRegion.GetCount(); i++)
			{
				COb_pcbregion* pregion = arrayPcbRegion.GetAt(i);
				pregion->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}

			while (!arrayPcbArc.IsEmpty())
			{
				delete arrayPcbArc.GetAt(0);
				arrayPcbArc.RemoveAt(0);
			}
			while (!arrayPcbTrack.IsEmpty())
			{
				delete arrayPcbTrack.GetAt(0);
				arrayPcbTrack.RemoveAt(0);
			}
			while (!arrayPcbPad.IsEmpty())
			{
				delete arrayPcbPad.GetAt(0);
				arrayPcbPad.RemoveAt(0);
			}
			while (!arrayPcbVia.IsEmpty())
			{
				delete arrayPcbVia.GetAt(0);
				arrayPcbVia.RemoveAt(0);
			}
			while (!arrayPcbText.IsEmpty())
			{
				delete arrayPcbText.GetAt(0);
				arrayPcbText.RemoveAt(0);
			}
			while (!arrayPcbFill.IsEmpty())
			{
				delete arrayPcbFill.GetAt(0);
				arrayPcbFill.RemoveAt(0);
			}
			while (!arrayPcbRegion.IsEmpty())
			{
				delete arrayPcbRegion.GetAt(0);
				arrayPcbRegion.RemoveAt(0);
			}

		}
	}
}

void CDlg_EditComp::OnEnChangeEditPackage()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_PACKAGE);
	pWnd->GetWindowText(m_csEditPackage);
	ShowPackage();
}

void CDlg_EditComp::OnBnClickedButtonIdGlobal()
{
	int iItem;
	for (iItem = 0; iItem < m_arrayTextOthers.GetCount(); ++iItem)
	{
		m_arrayTextOthers[iItem].visible = ListView_GetCheckState(m_listCtrl.m_hWnd, iItem);
        m_arrayTextOthers[iItem].item = m_listCtrl.GetItemText(iItem, 1);
		m_arrayTextOthers[iItem].value = m_listCtrl.GetItemText(iItem, 2);
	}
	UpdateData(TRUE);

	int nRet = 101; //ID Edit

	EndDialog(nRet);
}

void CDlg_EditComp::OnBnClickedButtonCommentGlobal()
{
	int iItem;
	for (iItem = 0; iItem < m_arrayTextOthers.GetCount(); ++iItem)
	{
		m_arrayTextOthers[iItem].visible = ListView_GetCheckState(m_listCtrl.m_hWnd, iItem);
        m_arrayTextOthers[iItem].item = m_listCtrl.GetItemText(iItem, 1);
		m_arrayTextOthers[iItem].value = m_listCtrl.GetItemText(iItem, 2);
	}
	UpdateData(TRUE);
	
	int nRet = 102; //Comment Edit

	EndDialog(nRet);
}

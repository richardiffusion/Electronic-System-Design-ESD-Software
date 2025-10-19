// Dlg_PcbNetParamEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "ESDDoc.h"
#include "ReportCtrlItem.h"
#include "Dlg_PcbNetParamEdit.h"
#include ".\dlg_pcbnetparamedit.h"
#include "systemext.h"

// CDlg_PcbNetParamEdit 对话框

IMPLEMENT_DYNAMIC(CDlg_PcbNetParamEdit, CDialog)
CDlg_PcbNetParamEdit::CDlg_PcbNetParamEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PcbNetParamEdit::IDD, pParent)
	, m_csEditTrackWidth(_T(""))
	, m_csEditViaHole(_T(""))
	, m_csEditVia(_T(""))
	, m_csEditNetName(_T(""))
	, m_bCheckHideConnection(FALSE)
	, m_bCheckRemoveLoop(FALSE)
	, m_bCheckStackLayerOnly(FALSE)
{
}

CDlg_PcbNetParamEdit::~CDlg_PcbNetParamEdit()
{
}

void CDlg_PcbNetParamEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_NET, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT_TRACKWIDTH, m_csEditTrackWidth);
	DDX_Text(pDX, IDC_EDIT_VIAHOLE, m_csEditViaHole);
	DDX_Text(pDX, IDC_EDIT_VIA, m_csEditVia);
	DDX_Text(pDX, IDC_EDIT_NETNAME, m_csEditNetName);
	DDX_Check(pDX, IDC_CHECK_HIDE_CONNECTION, m_bCheckHideConnection);
	DDX_Check(pDX, IDC_CHECK_REMOVE_LOOP, m_bCheckRemoveLoop);
	DDX_Check(pDX, IDC_CHECK_STACK_LAYER_ONLY, m_bCheckStackLayerOnly);
	DDX_Control(pDX, IDC_LAYER_TRACK_WIDTH, m_wndSignalCtrl);
	DDX_Control(pDX, IDC_PINS_OLD, m_wndPinsOld);
	DDX_Control(pDX, IDC_PINS_NEW, m_wndPinsNew);
}


BEGIN_MESSAGE_MAP(CDlg_PcbNetParamEdit, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
	ON_BN_CLICKED(IDC_CHECK_STACK_LAYER_ONLY, OnBnClickedCheckStackLayerOnly)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_PcbNetParamEdit 消息处理程序

BOOL CDlg_PcbNetParamEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_PCB_EDIT_NET);

	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;
	
	m_wndSignalCtrl.SetMultipleSelection(FALSE);
	m_wndSignalCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndSignalCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndSignalCtrl.GetPaintManager()->SetColumnStyle(XTPReportColumnStyle(1));
	m_wndSignalCtrl.AllowEdit(TRUE);
	SingalReportCtrl(m_bCheckStackLayerOnly, TRUE);
	
	PinsOldCtrl(TRUE);
	PinsNewCtrl(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_PcbNetParamEdit::OnStnClickedStaticColor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cColor.m_nColor = colordialog.GetColor();
		m_nColor = m_cColor.m_nColor;
		m_cColor.Invalidate();
	}
}

void CDlg_PcbNetParamEdit::SingalReportCtrl(BOOL use_stack_layers_only, BOOL create_column)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(create_column == TRUE)
	{

		CXTPReportColumn* pColumnTrackWidth = new CXTPReportColumn(0, "布线宽度", "", 120, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnTrackWidth->SetEditable(TRUE);
		pColumnTrackWidth->SetAllowDrag(FALSE);
		m_wndSignalCtrl.AddColumn(pColumnTrackWidth);

		CXTPReportColumn* pColumnColor = new CXTPReportColumn(1, "颜色", "", 40, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnColor->SetEditable(FALSE);
		pColumnColor->SetAllowDrag(FALSE);
		m_wndSignalCtrl.AddColumn(pColumnColor);
		m_wndSignalCtrl.SetFocusedColumn(pColumnColor);

		CXTPReportColumn* pColumnLayer = new CXTPReportColumn(2, "信号层名称", "",120, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnLayer->SetEditable(FALSE);	
		pColumnLayer->SetAllowDrag(FALSE);
		m_wndSignalCtrl.AddColumn(pColumnLayer);

		CXTPReportColumn* pColumnShow = new CXTPReportColumn(3, "层编号", "", 40, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
		pColumnShow->SetAllowDrag(FALSE);
		pColumnShow->SetEditable(FALSE);
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
				
				pRecord->AddItem(new CXTPReportRecordItemText(pDoc->Convert_XY_To_String(m_fTrackWidth[layer])));

				CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
				pColor->SetBackgroundColor(nEDA_PCB_LayerColor[layer]);
				pRecord->AddItem(pColor);

				pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[layer].csLayerName));

				char str[100]; itoa(layer, str, 10);
				pRecord->AddItem(new CXTPReportRecordItemText(str));

				m_wndSignalCtrl.AddRecord(pRecord);
			}

			if(layer == BOTTOMLAYER ) break;
			layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
		}
	}
	else
	{
		for(int layer=0; layer<SIGNAL_LAYER_NUM; layer++)
		{
				CXTPReportRecord* pRecord =  new CXTPReportRecord();
				
				pRecord->AddItem( new CXTPReportRecordItemText(pDoc->Convert_XY_To_String(m_fTrackWidth[layer])));

				CXTPReportRecordItemColor* pColor = new CXTPReportRecordItemColor();
				pColor->SetBackgroundColor(nEDA_PCB_LayerColor[layer]);
				pRecord->AddItem(pColor);

				pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[layer].csLayerName));

				char str[100]; itoa(layer, str, 10);
				pRecord->AddItem(new CXTPReportRecordItemText(str));

				m_wndSignalCtrl.AddRecord(pRecord);
		}
	}


	m_wndSignalCtrl.Populate();
}
void CDlg_PcbNetParamEdit::OnBnClickedCheckStackLayerOnly()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_STACK_LAYER_ONLY);
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

void CDlg_PcbNetParamEdit::PinsOldCtrl(BOOL create_column)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(create_column == TRUE)
	{
		CXTPReportColumn* pColumnPin = new CXTPReportColumn(0, "元件引脚", "", 80, FALSE, XTP_REPORT_NOICON, TRUE, TRUE);
		pColumnPin->SetEditable(TRUE);	
		pColumnPin->SetAllowDrag(FALSE);
		m_wndPinsOld.AddColumn(pColumnPin);

		CXTPReportColumn* pColumnNet = new CXTPReportColumn(1, "修改前所属网络", "",130, FALSE, XTP_REPORT_NOICON, TRUE, TRUE);
		pColumnNet->SetEditable(FALSE);	
		pColumnNet->SetAllowDrag(FALSE);
		m_wndPinsOld.AddColumn(pColumnNet);
	}

	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent < 0)) continue;

		CString csPadNet;
		if(ppad->m_nNet < 0) csPadNet = "未定义";
		else 
		{
			COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(ppad->m_nNet);
			csPadNet = pnet->m_csNetName;
			if(csPadNet == m_csEditNetName) continue;	
		}

		CString csPadname = ppad->m_csName;
		CString csDesignator;
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
		csDesignator = pcomp->GetPcbCompDesignator(ppad->m_nComponent);
		CString csPin = csDesignator + "-" + ppad->m_csName;
				
		CXTPReportRecord* pRecord =  new CXTPReportRecord();
		CXTPReportRecordItemText* pItemText =  new CXTPReportRecordItemText(csPin);
		pItemText->SetItemData(csDesignator.GetLength());
		pRecord->AddItem(pItemText);

		pRecord->AddItem( new CXTPReportRecordItemText(csPadNet));

		m_wndPinsOld.AddRecord(pRecord);
	}

	m_wndPinsOld.SetMultipleSelection(TRUE);
	m_wndPinsOld.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndPinsOld.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndPinsOld.GetPaintManager()->SetColumnStyle(XTPReportColumnStyle(1));

	m_wndPinsOld.Populate();
}

void CDlg_PcbNetParamEdit::PinsNewCtrl(BOOL create_column)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(create_column == TRUE)
	{
		CXTPReportColumn* pColumnPin = new CXTPReportColumn(0, "元件引脚", "", 80, FALSE, XTP_REPORT_NOICON, TRUE, TRUE);
		pColumnPin->SetEditable(TRUE);	
		pColumnPin->SetAllowDrag(FALSE);
		m_wndPinsNew.AddColumn(pColumnPin);

		CXTPReportColumn* pColumnNet = new CXTPReportColumn(1, "修改前所属网络", "",130, FALSE, XTP_REPORT_NOICON, TRUE, TRUE);
		pColumnNet->SetEditable(FALSE);	
		pColumnNet->SetAllowDrag(FALSE);
		m_wndPinsNew.AddColumn(pColumnNet);
	}

	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nNet < 0)||(ppad->m_nComponent < 0)) continue;

		CString csPadNet;
		COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(ppad->m_nNet);
		csPadNet = pnet->m_csNetName;
		if(csPadNet != m_csEditNetName) continue;	

		CString csPadname = ppad->m_csName;
		CString csDesignator;
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
		csDesignator = pcomp->GetPcbCompDesignator(ppad->m_nComponent);
		CString csPin = csDesignator + "-" + ppad->m_csName;
				
		CXTPReportRecord* pRecord =  new CXTPReportRecord();
		CXTPReportRecordItemText* pItemText =  new CXTPReportRecordItemText(csPin);
		pItemText->SetItemData(csDesignator.GetLength());
		pRecord->AddItem(pItemText);

		pRecord->AddItem( new CXTPReportRecordItemText(csPadNet));

		m_wndPinsNew.AddRecord(pRecord);
	}

	m_wndPinsNew.SetMultipleSelection(TRUE);
	m_wndPinsNew.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndPinsNew.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndPinsNew.GetPaintManager()->SetColumnStyle(XTPReportColumnStyle(1));

	m_wndPinsNew.Populate();
}

void CDlg_PcbNetParamEdit::OnBnClickedButtonAdd()
{
	CXTPReportSelectedRows* pSelectedRows;
	
	pSelectedRows = m_wndPinsOld.GetSelectedRows();
	int num = pSelectedRows->GetCount();
	for(int i=0; i<num; i++)
	{
		CXTPReportRow* pRow = pSelectedRows->GetAt(0);
		CXTPReportRecord* pRecordOld = pRow->GetRecord();
		CXTPReportRecordItemText* pOldItem_0 = (CXTPReportRecordItemText*)pRecordOld->GetItem(0);
		CXTPReportRecordItemText* pOldItem_1 = (CXTPReportRecordItemText*)pRecordOld->GetItem(1);

		
		CXTPReportRecord* pRecordNew =  new CXTPReportRecord();
		CXTPReportRecordItemText* pItemText =  new CXTPReportRecordItemText(pOldItem_0->GetValue());
		pItemText->SetItemData(pOldItem_0->GetItemData());
		pRecordNew->AddItem(pItemText);

		pRecordNew->AddItem( new CXTPReportRecordItemText(pOldItem_1->GetValue()));
		m_wndPinsNew.AddRecord(pRecordNew);

		m_wndPinsOld.RemoveRowEx(pRow);
	}

	m_wndPinsNew.Populate();
}

void CDlg_PcbNetParamEdit::OnBnClickedButtonDelete()
{
	CXTPReportSelectedRows* pSelectedRows;
	
	pSelectedRows = m_wndPinsNew.GetSelectedRows();
	int num = pSelectedRows->GetCount();
	for(int i=0; i<num; i++)
	{
		CXTPReportRow* pRow = pSelectedRows->GetAt(0);
		CXTPReportRecord* pRecordOld = pRow->GetRecord();
		CXTPReportRecordItemText* pOldItem_0 = (CXTPReportRecordItemText*)pRecordOld->GetItem(0);
		CXTPReportRecordItemText* pOldItem_1 = (CXTPReportRecordItemText*)pRecordOld->GetItem(1);

		
		CXTPReportRecord* pRecordNew =  new CXTPReportRecord();
		CXTPReportRecordItemText* pItemText =  new CXTPReportRecordItemText(pOldItem_0->GetValue());
		pItemText->SetItemData(pOldItem_0->GetItemData());
		pRecordNew->AddItem(pItemText);

		pRecordNew->AddItem( new CXTPReportRecordItemText(pOldItem_1->GetValue()));
		m_wndPinsOld.AddRecord(pRecordNew);

		m_wndPinsNew.RemoveRowEx(pRow);
	}

	m_wndPinsOld.Populate();
}

void CDlg_PcbNetParamEdit::OnBnClickedOk()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	UpdateData(TRUE);
	
	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		if(i == nNetIndex) continue;
		COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;
	
		if(m_csEditNetName.CompareNoCase(pnet->m_csNetName) == 0)
		{
			AfxMessageBox("网络名称已经存在.");
			return;
		}
	}


	OnOK();
}

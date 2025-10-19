// Dlg_PCBEditPad.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBEditPad.h"
#include ".\dlg_pcbeditpad.h"
#include "systemext.h"
#include "sysparam.h"


// CDlg_PCBEditPad 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBEditPad, CDialog)
CDlg_PCBEditPad::CDlg_PCBEditPad(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBEditPad::IDD, pParent)
	, m_csEditPadX(_T(""))
	, m_csEditPadY(_T(""))

	, m_fEditRotation(0)
	, m_csEditHoleSize(_T(""))
	, m_fHoleRotation(0)
	, m_csEditHoleWidth(_T(""))
	, m_csEditDesignator(_T(""))
	, m_nComboLayer(0)
	, m_nComboType(0)
	, m_bCheckPlated(FALSE)
	, m_bCheckLocked(FALSE)
	, m_csEditXSizeTop(_T(""))
	, m_csEditYSizeTop(_T(""))
	, m_nComboShapeTop(0)
	, m_csEditXSizeMid(_T(""))
	, m_csEditYSizeMid(_T(""))
	, m_nComboShapeMid(0)
	, m_csEditXSizeBottom(_T(""))
	, m_csEditYSizeBottom(_T(""))
	, m_nComboShapeBottom(0)
	, m_bCheckTentingTop(FALSE)
	, m_bCheckTentingBottom(FALSE)
	, m_csEditPaste(_T(""))
	, m_csEditSolder(_T(""))
	, m_csComboNet(_T(""))
{
	m_nRadioHoleType = 0;
	m_nRadioPadMode = 0;
	m_nRadioPaste = 0;
	m_nRadioSolder = 0;
}

CDlg_PCBEditPad::~CDlg_PCBEditPad()
{
}

void CDlg_PCBEditPad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CTRL, m_ctrlTab);
	DDX_Text(pDX, IDC_EDIT_PAD_X, m_csEditPadX);
	DDX_Text(pDX, IDC_EDIT_PAD_Y, m_csEditPadY);
	DDX_Text(pDX, IDC_EDIT_PAD_ROTATION, m_fEditRotation);
	DDX_Text(pDX, IDC_EDIT_HOLESIZE, m_csEditHoleSize);
	DDX_Text(pDX, IDC_EDIT_HOLE_ROTATION, m_fHoleRotation);
	DDX_Text(pDX, IDC_EDIT_HOLE_WIDTH, m_csEditHoleWidth);
	DDX_Text(pDX, IDC_EDIT_PCB_DESIGNATOR, m_csEditDesignator);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nComboLayer);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nComboType);
	DDX_Check(pDX, IDC_CHECK_PLATED, m_bCheckPlated);
	DDX_Check(pDX, IDC_CHECK_LOCKED, m_bCheckLocked);
	DDX_Text(pDX, IDC_EDIT_XSIZE_TOP, m_csEditXSizeTop);
	DDX_Text(pDX, IDC_EDIT_YSIZE_TOP, m_csEditYSizeTop);
	DDX_CBIndex(pDX, IDC_COMBO_SHAPE_TOP, m_nComboShapeTop);
	DDX_Text(pDX, IDC_EDIT_XSIZE_MID, m_csEditXSizeMid);
	DDX_Text(pDX, IDC_EDIT_YSIZE_MID, m_csEditYSizeMid);
	DDX_CBIndex(pDX, IDC_COMBO_SHAPE_MID, m_nComboShapeMid);
	DDX_Text(pDX, IDC_EDIT_XSIZE_BOTTOM, m_csEditXSizeBottom);
	DDX_Text(pDX, IDC_EDIT_YSIZE_BOTTOM, m_csEditYSizeBottom);
	DDX_CBIndex(pDX, IDC_COMBO_SHAPE_BOTTOM, m_nComboShapeBottom);
	DDX_Check(pDX, IDC_CHECK_TENTING_TOP, m_bCheckTentingTop);
	DDX_Check(pDX, IDC_CHECK_TENTING_BOTTOM, m_bCheckTentingBottom);
	DDX_Text(pDX, IDC_EDIT_PASTE, m_csEditPaste);
	DDX_Text(pDX, IDC_EDIT_SOLDER, m_csEditSolder);
	DDX_Radio(pDX, IDC_RADIO_HOLESHAPE, m_nRadioHoleType);
	DDX_Radio(pDX, IDC_RADIO_PAD_MODE, m_nRadioPadMode);
	DDX_Radio(pDX, IDC_RADIO_PASTE, m_nRadioPaste);
	DDX_Radio(pDX, IDC_RADIO_SOLDER, m_nRadioSolder);
	DDX_CBString(pDX, IDC_COMBO_NET, m_csComboNet);
}


BEGIN_MESSAGE_MAP(CDlg_PCBEditPad, CDialog)

	ON_BN_CLICKED(IDC_RADIO_PAD_MODE, OnBnClickedRadioPadMode)
	ON_BN_CLICKED(IDC_RADIO_PAD_MODE2, OnBnClickedRadioPadMode2)
	ON_BN_CLICKED(IDC_RADIO_HOLESHAPE, OnBnClickedRadioHoleshape)
	ON_BN_CLICKED(IDC_RADIO_HOLESHAPE1, OnBnClickedRadioHoleshape1)
	ON_BN_CLICKED(IDC_RADIO_HOLESHAPE2, OnBnClickedRadioHoleshape2)
	ON_BN_CLICKED(IDC_RADIO_PASTE, OnBnClickedRadioPaste)
	ON_BN_CLICKED(IDC_RADIO_PASTE1, OnBnClickedRadioPaste1)
	ON_BN_CLICKED(IDC_RADIO_SOLDER, OnBnClickedRadioSolder)
	ON_BN_CLICKED(IDC_RADIO_SOLDER1, OnBnClickedRadioSolder1)
	ON_NOTIFY(NM_CLICK, IDC_TAB_CTRL, OnNMClickTabCtrl)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER, OnCbnSelchangeComboLayer)
	ON_BN_CLICKED(IDC_CHECK_TENTING_BOTTOM, OnBnClickedCheckTentingBottom)
	ON_BN_CLICKED(IDC_CHECK_TENTING_TOP, OnBnClickedCheckTentingTop)
	ON_CBN_SELCHANGE(IDC_COMBO_NET, OnCbnSelchangeComboNet)
	ON_CBN_SELCHANGE(IDC_COMBO_SHAPE_BOTTOM, OnCbnSelchangeComboShapeBottom)
	ON_CBN_SELCHANGE(IDC_COMBO_SHAPE_MID, OnCbnSelchangeComboShapeMid)
	ON_CBN_SELCHANGE(IDC_COMBO_SHAPE_TOP, OnCbnSelchangeComboShapeTop)
	ON_EN_CHANGE(IDC_EDIT_HOLE_ROTATION, OnEnChangeEditHoleRotation)
	ON_EN_CHANGE(IDC_EDIT_HOLE_WIDTH, OnEnChangeEditHoleWidth)
	ON_EN_CHANGE(IDC_EDIT_HOLESIZE, OnEnChangeEditHolesize)
	ON_EN_CHANGE(IDC_EDIT_PAD_ROTATION, OnEnChangeEditPadRotation)
	ON_EN_CHANGE(IDC_EDIT_PAD_X, OnEnChangeEditPadX)
	ON_EN_CHANGE(IDC_EDIT_PAD_Y, OnEnChangeEditPadY)
	ON_EN_CHANGE(IDC_EDIT_PASTE, OnEnChangeEditPaste)
	ON_EN_CHANGE(IDC_EDIT_PCB_DESIGNATOR, OnEnChangeEditPcbDesignator)
	ON_EN_CHANGE(IDC_EDIT_SOLDER, OnEnChangeEditSolder)
	ON_EN_CHANGE(IDC_EDIT_XSIZE_BOTTOM, OnEnChangeEditXsizeBottom)
	ON_EN_CHANGE(IDC_EDIT_XSIZE_MID, OnEnChangeEditXsizeMid)
	ON_EN_CHANGE(IDC_EDIT_XSIZE_TOP, OnEnChangeEditXsizeTop)
	ON_EN_CHANGE(IDC_EDIT_YSIZE_BOTTOM, OnEnChangeEditYsizeBottom)
	ON_EN_CHANGE(IDC_EDIT_YSIZE_MID, OnEnChangeEditYsizeMid)
	ON_EN_CHANGE(IDC_EDIT_YSIZE_TOP, OnEnChangeEditYsizeTop)
END_MESSAGE_MAP()


// CDlg_PCBEditPad 消息处理程序


BOOL CDlg_PCBEditPad::OnInitDialog()
{
	CDialog::OnInitDialog();


	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LAYER);
	pCombo->ResetContent();
	for(int i=0; i<arrayStringLayer.GetCount(); i++)
	{
		pCombo->AddString(arrayStringLayer.GetAt(i));
	}
	pCombo->SetCurSel(m_nComboLayer);


	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_NET);
	pCombo->ResetContent();
	for(int i=0; i<arrayStringNet.GetCount(); i++)
	{
		pCombo->AddString(arrayStringNet.GetAt(i));
	}
	pCombo->SelectString(-1, m_csComboNet);

	if(m_nRadioPadMode == 0)
	{
		CWnd* pWnd = GetDlgItem(IDC_STATIC_MID);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_XSIZE_MID);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_YSIZE_MID);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_COMBO_SHAPE_MID);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_BOTTOM);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_XSIZE_BOTTOM);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_YSIZE_BOTTOM);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_COMBO_SHAPE_BOTTOM);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_TOP);
		pWnd->ShowWindow(FALSE);
	}

	if(m_nRadioHoleType == 0)
	{
		CWnd* pWnd = GetDlgItem(IDC_EDIT_HOLE_ROTATION);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_HOLE_ROTATION);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_HOLE_WIDTH);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_HOLE_WIDTH);
		pWnd->ShowWindow(FALSE);
	}
	else if(m_nRadioHoleType == 1)
	{
		CWnd* pWnd = GetDlgItem(IDC_EDIT_HOLE_WIDTH);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_HOLE_WIDTH);
		pWnd->ShowWindow(FALSE);
	}

	if(m_nRadioPaste == 0)
	{
		CWnd* pWnd = GetDlgItem(IDC_EDIT_PASTE);
		pWnd->EnableWindow(FALSE);
	}
	
	if(m_nRadioSolder == 0)
	{
		CWnd* pWnd = GetDlgItem(IDC_EDIT_SOLDER);
		pWnd->EnableWindow(FALSE);
	}

	//Tab风格
	//m_ctrlTab.GetPaintManager()->SetAppearance((XTPTabAppearanceStyle)7);
	m_ctrlTab.ModifyTabStyle(TCS_BOTTOM,TCS_BOTTOM);
	m_ctrlTab.ModifyTabStyle(TCS_FLATBUTTONS, TCS_FLATBUTTONS);
	//m_ctrlTab.SetTheme(xtThemeOffice2003);
	//m_ctrlTab.SetTheme(xtThemeDefault);
	//m_ctrlTab.SetTheme(xtThemeOfficeXP);

	//层控制
	int layer = GetLayerNumber(m_nComboLayer);
	if(layer == MULTILAYER)
	{
		int index = 0;
		for(int i=0; i<arrayStringLayer.GetCount(); i++)
		{
			CString csLayerName;
			csLayerName = arrayStringLayer.GetAt(i);
			m_ctrlTab.InsertItem(index, csLayerName);
			index++;
			if(GetLayerNumber(i) == BOTTOMLAYER) break;
		}
		m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[TOPPASTE].csLayerName);
		index++;
		m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[BOTTOMPASTE].csLayerName);
		index++;
		m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[TOPSOLDER].csLayerName);
		index++;
		m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[BOTTOMSOLDER].csLayerName);
		index++;
		m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[MULTILAYER].csLayerName);
	}
	else
	{
		//Disable Hole Control
		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_EDIT_HOLESIZE);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HOLESHAPE);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HOLESHAPE1);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HOLESHAPE2);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_HOLE_ROTATION);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_HOLE_WIDTH);
		pWnd->EnableWindow(FALSE);
		//Disable Pad Mode
		pWnd = GetDlgItem(IDC_RADIO_PAD_MODE);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_PAD_MODE2);
		pWnd->EnableWindow(FALSE);

		if(layer == TOPLAYER)
		{
			int index = 0;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[TOPLAYER].csLayerName);
			index++;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[TOPPASTE].csLayerName);
			index++;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[TOPSOLDER].csLayerName);
			//Disable TentingBottom
			pWnd = GetDlgItem(IDC_CHECK_TENTING_BOTTOM);
			pWnd->EnableWindow(FALSE);

		}
		else if(layer == BOTTOMLAYER)
		{
			int index = 0;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[BOTTOMLAYER].csLayerName);
			index++;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[BOTTOMPASTE].csLayerName);
			index++;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[BOTTOMSOLDER].csLayerName);
			//Disable TentingTop
			pWnd = GetDlgItem(IDC_CHECK_TENTING_TOP);
			pWnd->EnableWindow(FALSE);            
		}
		else
		{
			int index = 0;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[layer].csLayerName);
			//Disable TentingBottom
			pWnd = GetDlgItem(IDC_CHECK_TENTING_BOTTOM);
			pWnd->EnableWindow(FALSE);
			//Disable TentingTop
			pWnd = GetDlgItem(IDC_CHECK_TENTING_TOP);
			pWnd->EnableWindow(FALSE);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_PCBEditPad::OnBnClickedRadioPadMode()
{
	CWnd* pWnd = GetDlgItem(IDC_STATIC_MID);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_XSIZE_MID);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_YSIZE_MID);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_COMBO_SHAPE_MID);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_BOTTOM);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_XSIZE_BOTTOM);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_YSIZE_BOTTOM);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_COMBO_SHAPE_BOTTOM);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_TOP);
	pWnd->ShowWindow(FALSE);
}

void CDlg_PCBEditPad::OnBnClickedRadioPadMode2()
{
	CWnd* pWnd = GetDlgItem(IDC_STATIC_MID);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_XSIZE_MID);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_YSIZE_MID);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_COMBO_SHAPE_MID);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_BOTTOM);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_XSIZE_BOTTOM);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_YSIZE_BOTTOM);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_COMBO_SHAPE_BOTTOM);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_TOP);
	pWnd->ShowWindow(TRUE);
}

void CDlg_PCBEditPad::OnBnClickedRadioHoleshape()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_HOLE_ROTATION);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_HOLE_ROTATION);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_HOLE_WIDTH);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_HOLE_WIDTH);
	pWnd->ShowWindow(FALSE);

	DrawPad();
}

void CDlg_PCBEditPad::OnBnClickedRadioHoleshape1()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_HOLE_ROTATION);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_HOLE_ROTATION);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_HOLE_WIDTH);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_HOLE_WIDTH);
	pWnd->ShowWindow(FALSE);

	DrawPad();
}

void CDlg_PCBEditPad::OnBnClickedRadioHoleshape2()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_HOLE_ROTATION);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_HOLE_ROTATION);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_HOLE_WIDTH);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_HOLE_WIDTH);
	pWnd->ShowWindow(TRUE);

	DrawPad();
}

void CDlg_PCBEditPad::OnBnClickedRadioPaste()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_PASTE);
	pWnd->EnableWindow(FALSE);

	DrawPad();
}

void CDlg_PCBEditPad::OnBnClickedRadioPaste1()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_PASTE);
	pWnd->EnableWindow(TRUE);

	DrawPad();
}

void CDlg_PCBEditPad::OnBnClickedRadioSolder()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_SOLDER);
	pWnd->EnableWindow(FALSE);

	DrawPad();
}

void CDlg_PCBEditPad::OnBnClickedRadioSolder1()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_SOLDER);
	pWnd->EnableWindow(TRUE);

	DrawPad();
}

void CDlg_PCBEditPad::OnNMClickTabCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	DrawPad();

	*pResult = 0;
}

void CDlg_PCBEditPad::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DrawPad();
}

void CDlg_PCBEditPad::DrawPad()
{
	CString csLayerName;
	char str[1024];
	int index = m_ctrlTab.GetCurSel();
	TCITEM t;
	t.mask = TCIF_TEXT;
	t.cchTextMax = 1000;
	t.pszText = str;
	m_ctrlTab.GetItem(index, &t);

	csLayerName = str;
	int current_layer;
	for(current_layer=0; current_layer<PCB_LAYER_NUM; current_layer++)
	{
		if(csLayerName == pDoc->m_sPCB_LayerParam[current_layer].csLayerName) break;
	}

	RECT rect;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_PAD);
	CClientDC dc(pWnd);
	pWnd->GetClientRect(&rect);
	dc.FillSolidRect(rect.left, rect.top, rect.right, rect.bottom, nEDA_PCB_BoardAreaColor);
	dc.IntersectClipRect(&rect);


	UpdateData(TRUE);
	COb_pcbpad apad;
	apad.pDocument = pDoc;

	for(int i=0; i<=MULTILAYER; i++)
	{
		if(arrayStringLayer.GetAt(m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
		{
			apad.m_nLayer = i;
			break;
		}
	}
	for(int i=0; i<arrayStringNet.GetCount(); i++)
	{
		if(arrayStringNet.GetAt(i) == m_csComboNet)
		{
			apad.m_nNet = i-1;
		}
	}

	if(apad.m_nLayer == MULTILAYER)
	{
		apad.m_fHoleDia = pDoc->Convert_PCB_String_To_XY(m_csEditHoleSize);
		apad.m_nHoleType = m_nRadioHoleType;
		apad.m_fHoleRotation = m_fHoleRotation;
		apad.m_fHoleWidth = pDoc->Convert_PCB_String_To_XY(m_csEditHoleWidth);

		if(apad.m_fHoleDia <= 0) apad.m_fHoleDia = 20;
	}
	else
	{
		apad.m_fHoleDia = 0;
		apad.m_nHoleType = PADHOLE_ROUND;
		apad.m_fHoleRotation = 0;
		apad.m_fHoleWidth = 0;
	}

	apad.m_fCx = pDoc->Convert_PCB_String_To_XY(m_csEditPadX) + pDoc->m_cPCB_Sheet.m_fOriginX;
	apad.m_fCy = pDoc->Convert_PCB_String_To_XY(m_csEditPadY) + pDoc->m_cPCB_Sheet.m_fOriginY;
	apad.m_fRotation = m_fEditRotation;

	apad.m_csName = m_csEditDesignator;
	apad.m_nDaisychain = m_nComboType;
	apad.m_bLocked = m_bCheckLocked;
	apad.m_bPlated = m_bCheckPlated;

	apad.m_nPadMode = m_nRadioPadMode;
	if(apad.m_nPadMode == PAD_MODE_SIMPLE)
	{
		for(int i=0; i<32; i++)
		{
			apad.m_fSizeX[i] = pDoc->Convert_PCB_String_To_XY(m_csEditXSizeTop);
			apad.m_fSizeY[i] = pDoc->Convert_PCB_String_To_XY(m_csEditYSizeTop);
			apad.m_nShape[i] = m_nComboShapeTop;
		}
	}
	else
	{
		apad.m_fSizeX[TOPLAYER] = pDoc->Convert_PCB_String_To_XY(m_csEditXSizeTop);
		apad.m_fSizeY[TOPLAYER] = pDoc->Convert_PCB_String_To_XY(m_csEditYSizeTop);
		apad.m_nShape[TOPLAYER] = m_nComboShapeTop;

		apad.m_fSizeX[BOTTOMLAYER] = pDoc->Convert_PCB_String_To_XY(m_csEditXSizeBottom);
		apad.m_fSizeY[BOTTOMLAYER] = pDoc->Convert_PCB_String_To_XY(m_csEditYSizeBottom);
		apad.m_nShape[BOTTOMLAYER] = m_nComboShapeBottom;

		for(int i=MIDLAYER1; i<=MIDLAYER30; i++)
		{
			apad.m_fSizeX[i] = pDoc->Convert_PCB_String_To_XY(m_csEditXSizeMid);
			apad.m_fSizeY[i] = pDoc->Convert_PCB_String_To_XY(m_csEditYSizeMid);
			apad.m_nShape[i] = m_nComboShapeMid;
		}
	}

	apad.m_bTentingTop = m_bCheckTentingTop;
	apad.m_bTentingBottom = m_bCheckTentingBottom;

	apad.m_nCPEV = m_nRadioPaste + 1;
	apad.m_fCPE = pDoc->Convert_PCB_String_To_XY(m_csEditPaste);
	apad.m_nCSEV = m_nRadioSolder + 1;
	apad.m_fCSE = pDoc->Convert_PCB_String_To_XY(m_csEditSolder);

	double fminx, fminy, fmaxx, fmaxy;
	fminx = fminy = 100000;
	fmaxx = fmaxy = -100000;
	for(int i=0; i<SIGNAL_LAYER_NUM; i++)
	{
		double fmax = MAX(apad.m_fSizeX[i], apad.m_fSizeY[i]);
		fminx = MIN(fminx, apad.m_fCx - fmax/2);
		fminy = MIN(fminy, apad.m_fCy - fmax/2);
		fmaxx = MAX(fmaxx, apad.m_fCx + fmax/2);
		fmaxy = MAX(fmaxy, apad.m_fCy + fmax/2);
	}
	int nEx = rect.right-rect.left;
	int nEy = rect.bottom-rect.top;

	double scale1 = (fmaxx - fminx)*2/nEx;
	double scale2 = (fmaxy - fminy)*2/nEy;
	double scale;
	if(scale1 > scale2) scale = scale1;
	else scale = scale2;

	double origin_x = fminx - (nEx - (fmaxx - fminx)/scale)/2*scale;
	double origin_y = fminy - (nEy - (fmaxy - fminy)/scale)/2*scale;
	apad.DrawInLayerWindow(&dc, scale, origin_x, origin_y, nEy, current_layer);

}

int CDlg_PCBEditPad::GetLayerNumber(int index)
{
	CString csLayerName;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LAYER);
	pCombo->GetLBText(index, csLayerName);

	int layer;
	for(layer=0; layer<PCB_LAYER_NUM; layer++)
	{
		if(csLayerName == pDoc->m_sPCB_LayerParam[layer].csLayerName) return layer;
	}
	
	return 0;
}
void CDlg_PCBEditPad::OnCbnSelchangeComboLayer()
{
	m_ctrlTab.DeleteAllItems();

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LAYER);
	int index = pCombo->GetCurSel();
	int layer = GetLayerNumber(index);
	if(layer == MULTILAYER)
	{
		int index = 0;
		for(int i=0; i<arrayStringLayer.GetCount(); i++)
		{
			CString csLayerName;
			csLayerName = arrayStringLayer.GetAt(i);
			m_ctrlTab.InsertItem(index, csLayerName);
			index++;
			if(GetLayerNumber(i) == BOTTOMLAYER) break;
		}
		m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[TOPPASTE].csLayerName);
		index++;
		m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[BOTTOMPASTE].csLayerName);
		index++;
		m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[TOPSOLDER].csLayerName);
		index++;
		m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[BOTTOMSOLDER].csLayerName);
		index++;
		m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[MULTILAYER].csLayerName);

		//Enable Hole Control
		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_EDIT_HOLESIZE);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_RADIO_HOLESHAPE);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_RADIO_HOLESHAPE1);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_RADIO_HOLESHAPE2);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_HOLE_ROTATION);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_HOLE_WIDTH);
		pWnd->EnableWindow(TRUE);
		//Enable Pad Mode
		pWnd = GetDlgItem(IDC_RADIO_PAD_MODE);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_RADIO_PAD_MODE2);
		pWnd->EnableWindow(TRUE);
		//Enable Tenting
		pWnd = GetDlgItem(IDC_CHECK_TENTING_BOTTOM);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_CHECK_TENTING_TOP);
		pWnd->EnableWindow(TRUE);
	}
	else
	{
		//Disable Hole Control
		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_EDIT_HOLESIZE);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HOLESHAPE);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HOLESHAPE1);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_RADIO_HOLESHAPE2);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_HOLE_ROTATION);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_HOLE_WIDTH);
		pWnd->EnableWindow(FALSE);
		//Disable Pad Mode
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_PAD_MODE2);
		pButton->SetCheck(FALSE);
		pButton->EnableWindow(FALSE);
		pButton = (CButton*)GetDlgItem(IDC_RADIO_PAD_MODE);
		pButton->SetCheck(TRUE);
		pButton->EnableWindow(FALSE);
		m_nRadioPadMode = 0;
		//Disable x-size y-size shape
		pWnd = GetDlgItem(IDC_STATIC_MID);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_XSIZE_MID);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_YSIZE_MID);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_COMBO_SHAPE_MID);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_BOTTOM);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_XSIZE_BOTTOM);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_YSIZE_BOTTOM);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_COMBO_SHAPE_BOTTOM);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_TOP);
		pWnd->ShowWindow(FALSE);

		if(layer == TOPLAYER)
		{
			int index = 0;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[TOPLAYER].csLayerName);
			index++;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[TOPPASTE].csLayerName);
			index++;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[TOPSOLDER].csLayerName);
			//Disable TentingBottom
			CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_BOTTOM);
			pButton->SetCheck(FALSE);
			pButton->EnableWindow(FALSE);
			//Enable TentingTop
			pWnd = GetDlgItem(IDC_CHECK_TENTING_TOP);
			pWnd->EnableWindow(TRUE);  
		}
		else if(layer == BOTTOMLAYER)
		{
			int index = 0;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[BOTTOMLAYER].csLayerName);
			index++;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[BOTTOMPASTE].csLayerName);
			index++;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[BOTTOMSOLDER].csLayerName);
			//Disable TentingTop
			CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_TOP);
			pButton->SetCheck(FALSE);
			pButton->EnableWindow(FALSE);
			//Enable TentingBottom
			pWnd = GetDlgItem(IDC_CHECK_TENTING_BOTTOM);
			pWnd->EnableWindow(TRUE); 
		}
		else
		{
			int index = 0;
			m_ctrlTab.InsertItem(index, pDoc->m_sPCB_LayerParam[layer].csLayerName);
			//Disable TentingBottom/TentingTop
			CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_BOTTOM);
			pButton->SetCheck(FALSE);
			pButton->EnableWindow(FALSE);
			pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_TOP);
			pButton->SetCheck(FALSE);
			pButton->EnableWindow(FALSE);
		}
	}

	DrawPad();
}

void CDlg_PCBEditPad::OnBnClickedCheckTentingBottom()
{
	DrawPad();
}

void CDlg_PCBEditPad::OnBnClickedCheckTentingTop()
{
	DrawPad();
}

void CDlg_PCBEditPad::OnCbnSelchangeComboNet()
{
	DrawPad();
}

void CDlg_PCBEditPad::OnCbnSelchangeComboShapeBottom()
{
	DrawPad();
}

void CDlg_PCBEditPad::OnCbnSelchangeComboShapeMid()
{
	DrawPad();
}

void CDlg_PCBEditPad::OnCbnSelchangeComboShapeTop()
{
	DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditHoleRotation()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_HOLE_ROTATION);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditHoleWidth()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_HOLE_WIDTH);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditHolesize()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_HOLESIZE);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditPadRotation()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_PAD_ROTATION);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditPadX()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_PAD_X);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditPadY()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_PAD_Y);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditPaste()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_PASTE);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditPcbDesignator()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_PCB_DESIGNATOR);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditSolder()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_SOLDER);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditXsizeBottom()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_XSIZE_BOTTOM);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditXsizeMid()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_XSIZE_MID);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditXsizeTop()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_XSIZE_TOP);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditYsizeBottom()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_YSIZE_BOTTOM);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditYsizeMid()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_YSIZE_MID);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

void CDlg_PCBEditPad::OnEnChangeEditYsizeTop()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_YSIZE_TOP);
	CString cstring;
	pWnd->GetWindowText(cstring);
	if(cstring.IsEmpty() != TRUE) DrawPad();
}

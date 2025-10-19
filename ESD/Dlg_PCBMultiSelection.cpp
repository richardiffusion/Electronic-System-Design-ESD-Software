// Dlg_PCB_Multi_Selection.cpp : 实现文件
//

#include "stdafx.h"
#include "struct.h"
#include "ESD.h"
#include "ESDDoc.h"
#include "Dlg_PCBMultiSelection.h"
#include ".\dlg_pcbmultiselection.h"

// CDlg_PCBMultiSelection 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBMultiSelection, CDialog)
CDlg_PCBMultiSelection::CDlg_PCBMultiSelection(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBMultiSelection::IDD, pParent)
{
	first_show = TRUE;
}

CDlg_PCBMultiSelection::~CDlg_PCBMultiSelection()
{
}

void CDlg_PCBMultiSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON1, m_btn[0]);
	DDX_Control(pDX, IDC_BUTTON2, m_btn[1]);
	DDX_Control(pDX, IDC_BUTTON3, m_btn[2]);
	DDX_Control(pDX, IDC_BUTTON4, m_btn[3]);
	DDX_Control(pDX, IDC_BUTTON5, m_btn[4]);
	DDX_Control(pDX, IDC_BUTTON6, m_btn[5]);
	DDX_Control(pDX, IDC_BUTTON7, m_btn[6]);
	DDX_Control(pDX, IDC_BUTTON8, m_btn[7]);
	DDX_Control(pDX, IDC_BUTTON9, m_btn[8]);
	DDX_Control(pDX, IDC_BUTTON10, m_btn[9]);
	DDX_Control(pDX, IDC_BUTTON11, m_btn[10]);
	DDX_Control(pDX, IDC_BUTTON12, m_btn[11]);
	DDX_Control(pDX, IDC_BUTTON13, m_btn[12]);
	DDX_Control(pDX, IDC_BUTTON14, m_btn[13]);
	DDX_Control(pDX, IDC_BUTTON15, m_btn[14]);
	DDX_Control(pDX, IDC_BUTTON16, m_btn[15]);

}


BEGIN_MESSAGE_MAP(CDlg_PCBMultiSelection, CDialog)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, OnBnClickedButton16)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON1, OnBnHotItemChangeButton1)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON2, OnBnHotItemChangeButton2)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON3, OnBnHotItemChangeButton3)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON4, OnBnHotItemChangeButton4)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON5, OnBnHotItemChangeButton5)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON6, OnBnHotItemChangeButton6)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON7, OnBnHotItemChangeButton7)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON8, OnBnHotItemChangeButton8)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON9, OnBnHotItemChangeButton9)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON10, OnBnHotItemChangeButton10)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON11, OnBnHotItemChangeButton11)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON12, OnBnHotItemChangeButton12)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON13, OnBnHotItemChangeButton13)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON14, OnBnHotItemChangeButton14)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON15, OnBnHotItemChangeButton15)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON16, OnBnHotItemChangeButton16)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlg_PCBMultiSelection 消息处理程序



BOOL CDlg_PCBMultiSelection::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	for(int i=0; i<m_arrayString.GetCount(); i++)
	{
		m_btn[i].SetFlatStyle(TRUE);
		m_btn[i].SetTheme((XTPButtonTheme)4);

		CString cstring = m_arrayString.GetAt(i);
		if(cstring.Find("连线") >= 0) m_btn[i].SetBitmap(0, IDB_PCB_SEL1);
		else if(cstring.Find("圆弧") >= 0) m_btn[i].SetBitmap(0, IDB_PCB_SEL2);
		else if(cstring.Find("焊盘") >= 0) m_btn[i].SetBitmap(0, IDB_PCB_SEL3);	
		else if(cstring.Find("过孔") >= 0) m_btn[i].SetBitmap(0, IDB_PCB_SEL4);
		else if(cstring.Find("文字") >= 0) m_btn[i].SetBitmap(0, IDB_PCB_SEL5);
		else if(cstring.Find("填充") >= 0) m_btn[i].SetBitmap(0, IDB_PCB_SEL6);
		else if(cstring.Find("区域") >= 0) m_btn[i].SetBitmap(0, IDB_PCB_SEL7);
		else if(cstring.Find("元件") >= 0) m_btn[i].SetBitmap(0, IDB_PCB_SEL8);
		else if((cstring.Find("覆铜") >= 0)||(cstring.Find("分割") >= 0)) m_btn[i].SetBitmap(0, IDB_PCB_SEL9);
		
		//m_btn[i].SetBitmap(0, IDB_PCB_SEL1+i/2);
		int nVert = BS_VCENTER; //BS_TOP, BS_BOTTOM, BS_VCENTER
		int nHorz = BS_LEFT; //BS_LEFT, BS_RIGHT, BS_CENTER
		
		m_btn[i].SetImageAlignment(nVert | nHorz);
		m_btn[i].SetTextAlignment(nVert | nHorz);
		m_btn[i].SetImageGap(10);
		m_btn[i].SetWindowText(cstring);
	}
	for(int i = m_arrayString.GetCount(); i<16; i++)
	{
		m_btn[i].ShowWindow(SW_HIDE);
	}

	RECT rect;
	GetWindowRect(&rect);
	int x,y;
	x = m_nx - (rect.right - rect.left);				if(x<10) x=10;
	y = m_ny - (116 + 25*m_arrayString.GetCount());		if(y<100) y=100;
	
	//SetWindowPos(NULL, x, y, rect.right-rect.left,116 + 25*m_arrayString.GetCount(), SWP_NOZORDER);
	SetWindowPos(NULL, x, y, rect.right-rect.left,142 + 32*m_arrayString.GetCount(), SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_PCBMultiSelection::OnRButtonDown(UINT nFlags, CPoint point)
{
	OnCancel();

	CDialog::OnRButtonDown(nFlags, point);
}

void CDlg_PCBMultiSelection::OnBnClickedButton1()
{
	EndDialog(100);
}

void CDlg_PCBMultiSelection::OnBnClickedButton2()
{
	EndDialog(101);
}

void CDlg_PCBMultiSelection::OnBnClickedButton3()
{
	EndDialog(102);
}

void CDlg_PCBMultiSelection::OnBnClickedButton4()
{
	EndDialog(103);
}

void CDlg_PCBMultiSelection::OnBnClickedButton5()
{
	EndDialog(104);
}

void CDlg_PCBMultiSelection::OnBnClickedButton6()
{
	EndDialog(105);
}

void CDlg_PCBMultiSelection::OnBnClickedButton7()
{
	EndDialog(106);
}

void CDlg_PCBMultiSelection::OnBnClickedButton8()
{
	EndDialog(107);
}

void CDlg_PCBMultiSelection::OnBnClickedButton9()
{
	EndDialog(108);
}

void CDlg_PCBMultiSelection::OnBnClickedButton10()
{
	EndDialog(109);
}

void CDlg_PCBMultiSelection::OnBnClickedButton11()
{
	EndDialog(110);
}

void CDlg_PCBMultiSelection::OnBnClickedButton12()
{
	EndDialog(111);
}

void CDlg_PCBMultiSelection::OnBnClickedButton13()
{
	EndDialog(112);
}

void CDlg_PCBMultiSelection::OnBnClickedButton14()
{
	EndDialog(113);
}

void CDlg_PCBMultiSelection::OnBnClickedButton15()
{
	EndDialog(114);
}

void CDlg_PCBMultiSelection::OnBnClickedButton16()
{
	EndDialog(115);
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(0);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(1);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(2);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton4(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(3);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton5(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(4);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton6(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(5);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton7(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(6);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton8(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(7);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton9(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(8);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton10(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(9);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton11(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	
	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(10);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton12(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(11);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton13(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(12);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton14(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(13);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton15(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(14);
	}
}

void CDlg_PCBMultiSelection::OnBnHotItemChangeButton16(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 6 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0600。
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((pHotItem->dwFlags&0x00f0) == HICF_ENTERING)
	{
		DrawSelection(15);
	}
}

void CDlg_PCBMultiSelection::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if(first_show == TRUE)
	{
		first_show = FALSE;
		DrawSelection(-1);
	}
}

void CDlg_PCBMultiSelection::DrawSelection(int index)
{

	CWnd* pWnd = GetDlgItem(IDC_STATIC_SELECTION);
	RECT rect;
	pWnd->GetClientRect(&rect);
		
	rect.left++;
	rect.right--;
	rect.top++;
	rect.bottom--;
		
	CClientDC dc(pWnd);
	dc.FillSolidRect( rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, RGB(90,90,90));
	dc.IntersectClipRect(&rect);

	int x = (rect.right - rect.left)/2 ;
	int y = (rect.bottom - rect.top)/2;

	int nEx = rect.right-rect.left;
	int nEy = rect.bottom-rect.top;
	
	double fminx, fminy, fmaxx, fmaxy;
	fminx = fminy = 100000;
	fmaxx = fmaxy = -100000;
	if(index == -1) return;
	else
	{
		CObject* pobject = m_arrayObject.GetAt(index);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_pcbtrack )))
		{
			COb_pcbtrack* ptrack = (COb_pcbtrack*)pobject;
			fminx = MIN(fminx, ptrack->m_fX1);
			fminy = MIN(fminy, ptrack->m_fY1);
			fmaxx = MAX(fmaxx, ptrack->m_fX2);
			fmaxy = MAX(fmaxy, ptrack->m_fY2);

			double scale1 = (fmaxx - fminx)*1.2/nEx;
			double scale2 = (fmaxy - fminy)*1.2/nEy;
			double scale;
			if(scale1 > scale2) scale = scale1;
			else scale = scale2;

			double origin_x = fminx - (nEx - (fmaxx - fminx)/scale)/2*scale;
			double origin_y = fminy - (nEy - (fmaxy - fminy)/scale)/2*scale;
			ptrack->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_pcbarc )))
		{
			COb_pcbarc* parc = (COb_pcbarc*)pobject;
			fminx = MIN(fminx, (parc->m_fCx - parc->m_fRadius));
			fminy = MIN(fminy, (parc->m_fCy - parc->m_fRadius));
			fmaxx = MAX(fmaxx, (parc->m_fCx + parc->m_fRadius));
			fmaxy = MAX(fmaxy, (parc->m_fCy + parc->m_fRadius));
		
			double scale1 = (fmaxx - fminx)*1.2/nEx;
			double scale2 = (fmaxy - fminy)*1.2/nEy;
			double scale;
			if(scale1 > scale2) scale = scale1;
			else scale = scale2;

			double origin_x = fminx - (nEx - (fmaxx - fminx)/scale)/2*scale;
			double origin_y = fminy - (nEy - (fmaxy - fminy)/scale)/2*scale;
			parc->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_pcbvia )))
		{
			COb_pcbvia* pvia = (COb_pcbvia*)pobject;
			fminx = MIN(fminx, pvia->m_fCx - pvia->m_fDia);
			fminy = MIN(fminy, pvia->m_fCy - pvia->m_fDia);
			fmaxx = MAX(fmaxx, pvia->m_fCx + pvia->m_fDia);
			fmaxy = MAX(fmaxy, pvia->m_fCy + pvia->m_fDia);
			
			double scale1 = (fmaxx - fminx)*1.2/nEx;
			double scale2 = (fmaxy - fminy)*1.2/nEy;
			double scale;
			if(scale1 > scale2) scale = scale1;
			else scale = scale2;

			double origin_x = fminx - (nEx - (fmaxx - fminx)/scale)/2*scale;
			double origin_y = fminy - (nEy - (fmaxy - fminy)/scale)/2*scale;
			pvia->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_pcbtext )))
		{
			COb_pcbtext* ptext = (COb_pcbtext*)pobject;
			fminx = MIN(fminx, ptext->m_fSx);
			fminy = MIN(fminy, ptext->m_fSy);
			fmaxx = MAX(fmaxx, ptext->m_fSx + ptext->m_csText.GetLength()*ptext->m_fHeight);
			fmaxy = MAX(fmaxy, ptext->m_fSy + ptext->m_fHeight);
			
			double scale1 = (fmaxx - fminx)*1.2/nEx;
			double scale2 = (fmaxy - fminy)*1.2/nEy;
			double scale;
			if(scale1 > scale2) scale = scale1;
			else scale = scale2;

			double origin_x = fminx - (nEx - (fmaxx - fminx)/scale)/2*scale;
			double origin_y = fminy - (nEy - (fmaxy - fminy)/scale)/2*scale;
			ptext->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_pcbfill )))
		{
			COb_pcbfill* pfill = (COb_pcbfill*)pobject;
			fminx = MIN(fminx, pfill->m_fX1);
			fminy = MIN(fminy, pfill->m_fY1);
			fmaxx = MAX(fmaxx, pfill->m_fX2);
			fmaxy = MAX(fmaxy, pfill->m_fY2);
			
			double scale1 = (fmaxx - fminx)*1.2/nEx;
			double scale2 = (fmaxy - fminy)*1.2/nEy;
			double scale;
			if(scale1 > scale2) scale = scale1;
			else scale = scale2;

			double origin_x = fminx - (nEx - (fmaxx - fminx)/scale)/2*scale;
			double origin_y = fminy - (nEy - (fmaxy - fminy)/scale)/2*scale;
			pfill->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_pcbpad )))
		{
			COb_pcbpad* ppad = (COb_pcbpad*)pobject;
			fminx = MIN(fminx, ppad->m_fCx - ppad->m_fSizeX[0]);
			fminy = MIN(fminy, ppad->m_fCy - ppad->m_fSizeY[0]);
			fmaxx = MAX(fmaxx, ppad->m_fCx + ppad->m_fSizeX[0]);
			fmaxy = MAX(fmaxy, ppad->m_fCy + ppad->m_fSizeY[0]);
			
			double scale1 = (fmaxx - fminx)*1.2/nEx;
			double scale2 = (fmaxy - fminy)*1.2/nEy;
			double scale;
			if(scale1 > scale2) scale = scale1;
			else scale = scale2;

			double origin_x = fminx - (nEx - (fmaxx - fminx)/scale)/2*scale;
			double origin_y = fminy - (nEy - (fmaxy - fminy)/scale)/2*scale;
			ppad->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_pcbregion )))
		{
			COb_pcbregion* pregion = (COb_pcbregion*)pobject;
			for(int k=0;k <pregion->m_cVertex.GetCount(); k++)
			{
				fminx = MIN(fminx, pregion->m_cVertex[k].fx);
				fminy = MIN(fminy, pregion->m_cVertex[k].fy);
				fmaxx = MAX(fmaxx, pregion->m_cVertex[k].fx);
				fmaxy = MAX(fmaxy, pregion->m_cVertex[k].fy);
			}
			
			double scale1 = (fmaxx - fminx)*1.2/nEx;
			double scale2 = (fmaxy - fminy)*1.2/nEy;
			double scale;
			if(scale1 > scale2) scale = scale1;
			else scale = scale2;

			double origin_x = fminx - (nEx - (fmaxx - fminx)/scale)/2*scale;
			double origin_y = fminy - (nEy - (fmaxy - fminy)/scale)/2*scale;
			pregion->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_pcbpolygon )))
		{
			COb_pcbpolygon* ppolygon = (COb_pcbpolygon*)pobject;
			
			CESDDoc* pDoc = (CESDDoc*)ppolygon->pDocument;
			int polygonindex = -1;
			for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
			{
				COb_pcbpolygon* p = pDoc->m_arrayPcbPolygon.GetAt(i);
				if(p == ppolygon) 
				{
					polygonindex = i;
					break;
				}
			}
				
			if(polygonindex >= 0)
			{
				for(int k=0;k <ppolygon->m_cVertex.GetCount(); k++)
				{
					fminx = MIN(fminx, ppolygon->m_cVertex[k].fx);
					fminy = MIN(fminy, ppolygon->m_cVertex[k].fy);
					fmaxx = MAX(fmaxx, ppolygon->m_cVertex[k].fx);
					fmaxy = MAX(fmaxy, ppolygon->m_cVertex[k].fy);
				}
					
				double scale1 = (fmaxx - fminx)*1.2/nEx;
				double scale2 = (fmaxy - fminy)*1.2/nEy;
				double scale;
				if(scale1 > scale2) scale = scale1;
				else scale = scale2;

				double origin_x = fminx - (nEx - (fmaxx - fminx)/scale)/2*scale;
				double origin_y = fminy - (nEy - (fmaxy - fminy)/scale)/2*scale;			

				COb_pcbregion aregion;
				aregion.pDocument = ppolygon->pDocument;
				aregion.m_nLayer = ppolygon->m_nLayer;
				for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
				{
					Struct_RegionVertex vertex;
					vertex = ppolygon->m_cVertex.GetAt(i);
					aregion.m_cVertex.Add(vertex);
				}
				if(ppolygon->m_nPolygonType == COPPER_POLYGON)	aregion.m_bPolygonCutout = TRUE;
				aregion.DrawInWindow(&dc, scale, origin_x, origin_y, nEy);

				for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
					if(parc->m_nPolygon == polygonindex) parc->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
				}
				for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
					if(ptrack->m_nPolygon == polygonindex) ptrack->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
				}
				for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
					if(pregion->m_nPolygon == polygonindex) pregion->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_pcbcomp )))
		{
			COb_pcbcomp* pcomp = (COb_pcbcomp*)pobject;
			
			CESDDoc* pDoc = (CESDDoc*)pcomp->pDocument;
			int compindex = -1;
			for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
			{
				COb_pcbcomp* p = pDoc->m_arrayPcbComp.GetAt(i);
				if(p == pcomp) 
				{
					compindex = i;
					break;
				}
			}

			if(compindex >= 0)
			{
				double fcomp_x1, fcomp_y1, fcomp_x2, fcomp_y2, fcomp_x3, fcomp_y3, fcomp_x4, fcomp_y4;
				pcomp->GetPcbCompRect(&fcomp_x1, &fcomp_y1, &fcomp_x2, &fcomp_y2, &fcomp_x3, &fcomp_y3, &fcomp_x4, &fcomp_y4, compindex);
				CRect rect;
				rect.left = MIN(MIN(fcomp_x1, fcomp_x2), MIN(fcomp_x3, fcomp_x4));
				rect.right = MAX(MAX(fcomp_x1, fcomp_x2), MAX(fcomp_x3, fcomp_x4));
				rect.bottom = MIN(MIN(fcomp_y1, fcomp_y2), MIN(fcomp_y3, fcomp_y4));
				rect.top = MAX(MAX(fcomp_y1, fcomp_y2), MAX(fcomp_y3, fcomp_y4));

				fminx = MIN(fminx, rect.left);
				fminy = MIN(fminy, rect.bottom);
				fmaxx = MAX(fmaxx, rect.right);
				fmaxy = MAX(fmaxy, rect.top);
				
				double scale1 = (fmaxx - fminx)*1.2/nEx;
				double scale2 = (fmaxy - fminy)*1.2/nEy;
				double scale;
				if(scale1 > scale2) scale = scale1;
				else scale = scale2;

				double origin_x = fminx - (nEx - (fmaxx - fminx)/scale)/2*scale;
				double origin_y = fminy - (nEy - (fmaxy - fminy)/scale)/2*scale;
				

				for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
					if(parc->m_nComponent == compindex) parc->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
				}
				for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
					if(ptrack->m_nComponent == compindex) ptrack->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
				}
				for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
					if(ppad->m_nComponent == compindex) ppad->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_pcbcoordinate )))
		{
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_pcbdimension )))
		{
		}
	}
}

BOOL CDlg_PCBMultiSelection::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == 0x118)		//0x118 在对话框外按下鼠标左键或者右键
	{
		OnCancel();
	}

	return CDialog::PreTranslateMessage(pMsg);
}



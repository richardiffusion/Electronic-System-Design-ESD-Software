// Dlg_PCB_LoadPackageXY.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCB_LoadPackageXY.h"


// CDlg_PCB_LoadPackageXY �Ի���

IMPLEMENT_DYNAMIC(CDlg_PCB_LoadPackageXY, CDialog)
CDlg_PCB_LoadPackageXY::CDlg_PCB_LoadPackageXY(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCB_LoadPackageXY::IDD, pParent)
	, m_csEdit_X(_T(""))
	, m_csEdit_Y(_T(""))
{
}

CDlg_PCB_LoadPackageXY::~CDlg_PCB_LoadPackageXY()
{
}

void CDlg_PCB_LoadPackageXY::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_csEdit_X);
	DDX_Text(pDX, IDC_EDIT_Y, m_csEdit_Y);
}


BEGIN_MESSAGE_MAP(CDlg_PCB_LoadPackageXY, CDialog)
END_MESSAGE_MAP()


// CDlg_PCB_LoadPackageXY ��Ϣ�������

// Dlg_AnnotateReset.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_AnnotateReset.h"


// CDlg_AnnotateReset �Ի���

IMPLEMENT_DYNAMIC(CDlg_AnnotateReset, CDialog)
CDlg_AnnotateReset::CDlg_AnnotateReset(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_AnnotateReset::IDD, pParent)
{
}

CDlg_AnnotateReset::~CDlg_AnnotateReset()
{
}

void CDlg_AnnotateReset::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DIAGRAM, m_nRadioScope);
}


BEGIN_MESSAGE_MAP(CDlg_AnnotateReset, CDialog)
END_MESSAGE_MAP()


// CDlg_AnnotateReset ��Ϣ�������

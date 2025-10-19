// Dlg_PcbPerference.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PcbPerference.h"


// CDlg_PcbPerference 对话框

IMPLEMENT_DYNAMIC(CDlg_PcbPerference, CDialog)
CDlg_PcbPerference::CDlg_PcbPerference(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PcbPerference::IDD, pParent)
	, m_nEditNumber(0)
	, m_nEditNDelta(0)
	, m_csEditXDelta(_T(""))
	, m_csEditYDelta(_T(""))
	, m_bCheckRatsnestEnable(FALSE)
	, m_bCheckOnlineDrc(FALSE)
	, m_csEditMaxRadius(_T(""))
	, m_nEditUndoBuffer(0)
	, m_bCheckPolygonHighlight(FALSE)
	, m_csEditLoadPackageX(_T(""))
	, m_csEditLoadPackageY(_T(""))
	, m_bCheckFileImport(FALSE)
	, m_bCheckFileExport(FALSE)
{
}

CDlg_PcbPerference::~CDlg_PcbPerference()
{
}

void CDlg_PcbPerference::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUMBER, m_nEditNumber);
	DDV_MinMaxInt(pDX, m_nEditNumber, 1, 500);
	DDX_Text(pDX, IDC_EDIT_NDELTA, m_nEditNDelta);
	DDX_Text(pDX, IDC_EDIT_XDELTA, m_csEditXDelta);
	DDX_Text(pDX, IDC_EDIT_YDELTA, m_csEditYDelta);
	DDX_Check(pDX, IDC_CHECK_RATSNEST_ENABLE, m_bCheckRatsnestEnable);
	DDX_Check(pDX, IDC_CHECK_ONLINE_DRC, m_bCheckOnlineDrc);
	DDX_Text(pDX, IDC_EDIT_MAX_RADIUS, m_csEditMaxRadius);
	DDX_Text(pDX, IDC_EDIT_UNDO_BUFFER, m_nEditUndoBuffer);
	DDX_Check(pDX, IDC_CHECK_POLYGON_HIGHLIGHT, m_bCheckPolygonHighlight);
	DDX_Text(pDX, IDC_EDIT_LOAD_PACKAGE_X, m_csEditLoadPackageX);
	DDX_Text(pDX, IDC_EDIT_LOAD_PACKAGE_Y, m_csEditLoadPackageY);
	DDX_Check(pDX, IDC_CHECK_FILE_IMPORT, m_bCheckFileImport);
	DDX_Check(pDX, IDC_CHECK_FILE_EXPORT, m_bCheckFileExport);
}


BEGIN_MESSAGE_MAP(CDlg_PcbPerference, CDialog)
END_MESSAGE_MAP()


// CDlg_PcbPerference 消息处理程序

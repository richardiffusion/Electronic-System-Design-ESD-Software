#pragma once

#include  "Bitmap_Transparent_Dialog.h"

// CDlg_PCBEditLDimension 对话框

class CDlg_PCBEditLDimension : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditLDimension)

public:
	CDlg_PCBEditLDimension(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBEditLDimension();
	
	CBitmap_Transparent_Dialog	m_Bitmap;

// 对话框数据
	enum { IDD = IDD_PCB_EDIT_LDIMENSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

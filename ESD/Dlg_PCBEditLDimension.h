#pragma once

#include  "Bitmap_Transparent_Dialog.h"

// CDlg_PCBEditLDimension �Ի���

class CDlg_PCBEditLDimension : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditLDimension)

public:
	CDlg_PCBEditLDimension(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PCBEditLDimension();
	
	CBitmap_Transparent_Dialog	m_Bitmap;

// �Ի�������
	enum { IDD = IDD_PCB_EDIT_LDIMENSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

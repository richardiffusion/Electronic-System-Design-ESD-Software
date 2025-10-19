#pragma once

// CPage_Vertices �Ի���
#include "VertexListCtrl.h"

class CPage_Vertices : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Vertices)

public:
	CPage_Vertices();
	virtual ~CPage_Vertices();

// �Ի�������
	enum { IDD = IDD_VERTICES };

	CVertexListCtrl  m_listCtrl;
	CXTHeaderCtrl  m_header;
	
	int             m_nSortedCol;
	bool            m_bAscending;
	void SortColumn(int iCol, bool bAsc);
		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
};

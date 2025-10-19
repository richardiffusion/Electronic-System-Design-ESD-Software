#pragma once

// CPage_Vertices 对话框
#include "VertexListCtrl.h"

class CPage_Vertices : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Vertices)

public:
	CPage_Vertices();
	virtual ~CPage_Vertices();

// 对话框数据
	enum { IDD = IDD_VERTICES };

	CVertexListCtrl  m_listCtrl;
	CXTHeaderCtrl  m_header;
	
	int             m_nSortedCol;
	bool            m_bAscending;
	void SortColumn(int iCol, bool bAsc);
		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
};

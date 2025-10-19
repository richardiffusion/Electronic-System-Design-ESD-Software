#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlg_PCBBoardInfo �Ի���

class CDlg_PCBBoardInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBBoardInfo)

public:
	CDlg_PCBBoardInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PCBBoardInfo();

// �Ի�������
	enum { IDD = IDD_PCB_BOARD_INFO };

	int m_nTracks;
	int m_nArcs;
	int m_nPads;
	int m_nVias;
	int m_nFills;
	int m_nTexts;
	int m_nRegions;
	int m_nPolygons;
	int m_nCoordinates;
	int m_nDimensions;
	int m_nRoundHoles;
	int m_nSlotHoles;
	int m_nSquareHoles;
	int m_nDRCViolations;
	int m_nConnections;

	//---- Ԫ��
	int m_nComponents;
	int m_nTopComponents;
	int m_nBottomComponents;
	CStringArray m_arrayCompName;

	//--- ����
	int m_nNets;
	CStringArray m_arrayNetName;

	//--- ��ƽ��
	CDocument* pDocument;
	CStringArray m_arrayPlaneName;
	CArray<int, int> m_arrayPlaneLayer;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_listComp;
	CListBox m_listNet;
	//CTabCtrl m_tabPlane;
	CXTTabCtrl m_ctrlTab;
	CListBox m_listPlaneNet;
	CListBox m_listPlanePin;
	
	void SetPlaneNet();
	void SetPlanePin(CString& netname);
	afx_msg void OnTcnSelchangeTabPlane(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeListPlaneNet();
};

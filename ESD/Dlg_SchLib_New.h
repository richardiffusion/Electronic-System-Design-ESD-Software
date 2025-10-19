#pragma once


// CDlg_SchLib_New �Ի���

class CDlg_SchLib_New : public CDialog
{
	DECLARE_DYNAMIC(CDlg_SchLib_New)

public:
	CDlg_SchLib_New(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_SchLib_New();

// �Ի�������
	enum { IDD = IDD_SCHLIB_NEW };
	int m_nRadioPart;
	int m_nRadioMode;

	int sheet_height;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nEditPartNum;
	int m_nEditModeNum;
	int m_nComboPart;
	int m_nComboMode;
	int m_nEditPinNum;
	afx_msg void OnBnClickedRadioMultimode();
	afx_msg void OnBnClickedRadioMultipart();
	afx_msg void OnBnClickedRadioMode();
	afx_msg void OnBnClickedRadioPart();
	afx_msg void OnEnChangeEditPartnum();
	afx_msg void OnEnChangeEditModenum();
	double m_fEditX;
	double m_fEditY;
	int m_nComboPinPosition;
	double m_fEditPinLength;
	double m_fEditWidth;
};

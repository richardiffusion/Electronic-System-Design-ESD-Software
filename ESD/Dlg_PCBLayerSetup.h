#pragma once


// CDlg_PCBLayerSetup 对话框

class CDlg_PCBLayerSetup : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBLayerSetup)

public:
	CDlg_PCBLayerSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBLayerSetup();

// 对话框数据
	enum { IDD = IDD_PCB_LAYER_SETUP };
	
	CXTPButton   m_btn_signal_allon;
	CXTPButton   m_btn_signal_alloff;
	CXTPButton   m_btn_signal_usedon;

	CXTPButton   m_btn_plane_allon;
	CXTPButton   m_btn_plane_alloff;
	CXTPButton   m_btn_plane_usedon;

	CXTPButton   m_btn_mech_allon;
	CXTPButton   m_btn_mech_alloff;
	CXTPButton   m_btn_mech_usedon;

	CXTPButton   m_btn_mask_allon;
	CXTPButton   m_btn_mask_alloff;
	CXTPButton   m_btn_mask_usedon;

	CXTPButton   m_btn_other_allon;
	CXTPButton   m_btn_other_alloff;
	CXTPButton   m_btn_other_usedon;

	CXTPButton   m_btn_silk_allon;
	CXTPButton   m_btn_silk_alloff;
	CXTPButton   m_btn_silk_usedon;

	CXTPButton   m_btn_control_allon;
	CXTPButton   m_btn_control_alloff;
	//CXTPButton   m_btn_silk_usedon;

	CXTPButton   m_btn_allon;
	CXTPButton   m_btn_alloff;
	CXTPButton   m_btn_usedon;

	CXTPReportControl m_wndSignalCtrl;
	CXTPReportControl m_wndPlaneCtrl;
	CXTPReportControl m_wndMechCtrl;
	CXTPReportControl m_wndMaskCtrl;
	CXTPReportControl m_wndOtherCtrl;
	CXTPReportControl m_wndSilkCtrl;
	CXTPReportControl m_wndSystemCtrl;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDocument* pDocument;
	void SingalReportCtrl(BOOL use_stack_layers_only, BOOL create_column);
	void PlaneReportCtrl(BOOL use_stack_layers_only, BOOL create_column);
	void MechReportCtrl(BOOL use_enable_layers_only, BOOL create_column);
	void MaskReportCtrl();
	void OtherReportCtrl();
	void SilkReportCtrl();
	void SystemReportCtrl();
	virtual BOOL OnInitDialog();
	BOOL m_bCheckSingalLayerUseStack;
	BOOL m_bCheckPlaneUseStack;
	BOOL m_bCheckMechLayerUseStack;
	afx_msg void OnBnClickedMechLayerUsestack();
	afx_msg void OnBnClickedSingalLayerUsestack();
	afx_msg void OnBnClickedPlaneUsestack();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedSingalAllon();
	afx_msg void OnBnClickedSingalAlloff();
	afx_msg void OnBnClickedSingalUsedon();
	afx_msg void OnBnClickedPlaneAlloff();
	afx_msg void OnBnClickedPlaneAllon();
	afx_msg void OnBnClickedPlaneUsedon();
	afx_msg void OnBnClickedMechAllon();
	afx_msg void OnBnClickedMechAlloff();
	afx_msg void OnBnClickedMechUsedon();
	afx_msg void OnBnClickedMaskAllon();
	afx_msg void OnBnClickedMaskAlloff();
	afx_msg void OnBnClickedMaskUsedon();
	afx_msg void OnBnClickedOtherAllon();
	afx_msg void OnBnClickedOtherAlloff();
	afx_msg void OnBnClickedOtherUsedon();
	afx_msg void OnBnClickedSilkAllon();
	afx_msg void OnBnClickedSilkAlloff();
	afx_msg void OnBnClickedSilkUsedon();
	afx_msg void OnBnClickedControlAlloff();
	afx_msg void OnBnClickedControlAllon();
	afx_msg void OnBnClickedAlloff();
	afx_msg void OnBnClickedAllon();
	afx_msg void OnBnClickedUsedon();
};

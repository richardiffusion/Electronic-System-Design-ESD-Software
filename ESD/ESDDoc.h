// GESD.h : interface of the CESDDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ESDDOC_H__4E636A51_B851_4141_87F3_E7EA4C82E936__INCLUDED_)
#define AFX_ESDDOC_H__4E636A51_B851_4141_87F3_E7EA4C82E936__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include  "sysparam.h"
#include "struct.h"

class CESDDoc : public CDocument
{
protected: // create from serialization only
	CESDDoc();
	DECLARE_DYNCREATE(CESDDoc)

// Attributes
public:
	int m_nDocumentType;  
	int m_nFileSaveType;


	CArray <LOGFONT, LOGFONT&>  m_arrayFont[SCH_SHEET_NUM];


	CTypedPtrList <CObList, CObject*> m_listSchObject[SCH_SHEET_NUM];
	
	double m_fSCH_ScreenX[SCH_SHEET_NUM];
	double m_fSCH_ScreenY[SCH_SHEET_NUM];
	double m_fSCH_Scale[SCH_SHEET_NUM];
	
	COb_schsheet* m_pSCH_Sheet[SCH_SHEET_NUM];

	// File head parameters
	BOOL m_bSCH_SheetShow[SCH_SHEET_NUM];
	CString m_csSCH_SheetName[SCH_SHEET_NUM];	
	
	int m_nSCH_Unit;
	BOOL m_bSCH_VisibleGridShow;
	double m_fSCH_VisibleGrid;
	BOOL m_bSCH_SnapGridEnable;
	double m_fSCH_SnapGrid;
	BOOL m_bSCH_ElecGridEnable;
	double m_fSCH_ElecGrid;

	CStringArray m_arrayCompDesignator;
	CStringArray m_arrayCompComment;
	CStringArray m_arrayCompPackage;
	CStringArray m_arrayPackageLibraryName;
	CTypedPtrArray <CObArray, COb_schnet*> m_arraySchNet;
	
	int m_nSCH_Undo_Num;
	CTypedPtrList<CObList, CSchUndo*> m_listSchUndo[SCH_UNDO_NUMBER];

	void Clear_Discarded_Undo_Buffer(void);

	void Undo_Buffer_Transfer(void);

	CArray <POSITION, POSITION&> m_arraySelection[SCH_SHEET_NUM];
	Fpoint m_fpSelection[SCH_SHEET_NUM];
	double m_fSelection_X1[SCH_SHEET_NUM], m_fSelection_Y1[SCH_SHEET_NUM], m_fSelection_X2[SCH_SHEET_NUM], m_fSelection_Y2[SCH_SHEET_NUM];

	CArray<Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&> m_arrayPcbSelection;
	Fpoint m_fpPcbSelection;
	double m_fPcbSelection_X1, m_fPcbSelection_Y1, m_fPcbSelection_X2, m_fPcbSelection_Y2;
	

	struct 
	{	
		CString csLayerName;
		int nLayerPrev;
		int nLayerNext;
		BOOL bLayerMechEnabled;
		BOOL bLayerSheetLinked;
		CString csLayerCopThick;
		CString csLayerDielType;
		CString csLayerDielConst;
		CString csLayerDielHeight;
		CString csLayerDielMaterial;
		
		BOOL bLayerShow;
	} m_sPCB_LayerParam[PCB_LAYER_NUM];


	COb_pcbsheet m_cPCB_Sheet;

	CTypedPtrArray <CObArray, COb_pcbarc*> m_arrayPcbArc;
	CTypedPtrArray <CObArray, COb_pcbtrack*> m_arrayPcbTrack;
	CTypedPtrArray <CObArray, COb_pcbtext*> m_arrayPcbText;
	CTypedPtrArray <CObArray, COb_pcbfill*> m_arrayPcbFill;
	CTypedPtrArray <CObArray, COb_pcbregion*> m_arrayPcbRegion;	
	CTypedPtrArray <CObArray, COb_pcbpolygon*> m_arrayPcbPolygon;
	CTypedPtrArray <CObArray, COb_pcbpad*> m_arrayPcbPad;
	CTypedPtrArray <CObArray, COb_pcbvia*> m_arrayPcbVia;
	CTypedPtrArray <CObArray, COb_pcbcomp*> m_arrayPcbComp;
	CTypedPtrArray <CObArray, COb_pcbcoordinate*> m_arrayPcbCoordinate;
	CTypedPtrArray <CObArray, COb_pcbdimension*> m_arrayPcbDimension;
	

	CTypedPtrArray <CObArray, COb_pcbnet*> m_arrayPcbNet;

	CTypedPtrArray <CObArray, COb_pcbclass*> m_arrayPcbClass;

	CTypedPtrArray <CObArray, COb_pcbconnection*> m_arrayPcbConnection;


	CString m_csPowerPlane[POWER_LAYER_NUM];
	double m_fPlanePullBack[POWER_LAYER_NUM];

	double m_fPCB_ScreenX;
	double m_fPCB_ScreenY;
	double m_fPCB_Scale;
	
	int m_nPCB_Undo_Num;
	CTypedPtrList<CObList, CPcbUndo*> m_listPcbUndo[PCB_UNDO_NUMBER];

	CArray <int, int> m_arrayPcbLayerShow;
	void InitLayerShowArray();
	BOOL LayerVisible(int layer);
	void DeletePCBLayerObject(int layer);
	void AddPCBInternalPlaneObject(int layer);
	CArray<Struct_PCBLayerSet, Struct_PCBLayerSet&> m_arrayPCBLayerSet;

	Fpoint m_fPCB_Marker[10];

	BOOL m_bSelectionMemoryLock[8];

	double m_fDrillGuideHoleSize;
	double m_fDrillDrawSymbolSize;
	int m_nDrillSymbolKind;
	BOOL m_bMultiLayerOnPadMaster;
	BOOL m_bTopLayerOnPadMaster;
	BOOL m_bBottomLayerOnPadMaster;
	BOOL m_bIncludeViasInSolderMask;
	BOOL m_bIncludeUnconnectedPads;
	BOOL m_bIncludeMechWithAllPlots[16];
	BOOL m_bPlotLayers[109];
	BOOL m_bFlipLayers[109];

	double m_fPCBPrintX1, m_fPCBPrintY1, m_fPCBPrintX2, m_fPCBPrintY2;
	CString m_csDevice;
	CString m_csDriver;
	CString m_csOutput;
	BOOL m_bShowHoles;
	BOOL m_bScaleToFitPage;
	BOOL m_bUserPrinterFonts;
	BOOL m_bUseSoftaareArcs;
	int m_nBatchType;
	int m_nCompositeType;
	double m_fBorderSize;
	double m_fPrinterScale;
	double m_fPrinterCorrectX, m_fPrinterCorrectY;
	//------------------
	BOOL m_bPlotPadNets;
	BOOL m_bPlotPadNumbers;
	double m_fPlotterScale;
	double m_fPlotterCorrectX, m_fPlotterCorrectY;
	double m_fPlotterOffsetX, m_fPlotterOffsetY;
	BOOL m_bPlotterShowHoles;
	BOOL m_bPlotterUseOfArcs;
	BOOL m_bPlotterWaitBetweenSheets;
	int m_nPlotterOutputPort;	
	int m_nPlotterLanguage;		
	int m_nPlotterSpeed[8];		
	int m_nPlotterThickness[8]; 
	int m_nPlotterMode;			
	int m_nPlotterDriverType;	
	BOOL m_bPlotterLayers[PCB_LAYER_NUM];
	int m_nPP[PCB_LAYER_NUM];
	int m_nPM[PCB_LAYER_NUM];
	int m_nPC[PCB_LAYER_NUM];

	BOOL m_bSortOutput;
	BOOL m_bUseSoftwareArcs;
	BOOL m_bCenterPhotoPlots;
	BOOL m_bEmbedApertures;
	BOOL m_bPanelize;
	BOOL m_bG54;
	double m_fPlusTol;
	double m_fMinusTol;
	double m_fFilmSizeX, m_fFilmSizeY;
	double m_fGerberBorderSize;
	BOOL m_bApttable;
	double m_fMaxAperSize;
	BOOL m_bReliefShapesAllowed;
	BOOL m_bPadsFlashOnly;
	int m_nGerberUnits;
	int m_nGerberDecs;		
							
	
	double m_fPlaceLargeClear;
	double m_fPlaceSMallClear;
	BOOL m_bPlaceUseRotation;
	BOOL m_bPlaceUseLayerSwap;
	CString m_csPlacerByPassNet1, m_csPlacerByPassNet2;
	BOOL m_bPlaceUseAdvancedPlace;
	BOOL m_bPlaceUseGrouping;
	
	BOOL m_bDoMakeDRCFile;
	BOOL m_bDoMakeDRCErrorList;
	BOOL m_bDoSubNetDetails;
	int m_nRuleSetToCheck[49];
	int m_nOnLineRuleSetToCheck[49];
	CString m_csReportFileName;
	CString m_csExternalNetListFileName;
	BOOL m_bCheckExternalNetList;
	int m_nMaxViolationCount;
	BOOL m_bInternalPlaneWarnings;
	
	double m_fArcPrecision;

	double m_fRulesClearance;

	double m_fRulesTrackWidth;
	double m_fRulesViaDia;
	double m_fRulesViaHoleDia;

	int m_nRulesTopology;

	int m_nRulesLayer[SIGNAL_LAYER_NUM];

	int m_nRulesCornerStyle;
	double m_fRulesCornerSetback;

	CArray<Struct_Route_Priority, Struct_Route_Priority&> m_arrayRulesPriority;

	double m_fSolderMaskExpansion;

	double m_fPasteMaskExpansion;

	double m_fPlaneClearance;

	int m_nPlaneConnectStyle;
	int m_nPlaneConductors;
	double m_fPlaneConductorWidth;
	double m_fPlaneAirGap;
	double m_fPlaneExpansion;

	int m_nPolygonConnectStyle;
	int m_nPolygonConductors;
	double m_fPolygonConductorWidth;


	CArray<Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&> m_arrayPcbDrcErrors;
	CArray<Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&> m_arrayPcbOnLineDrcErrors;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CESDDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CESDDoc();

	BOOL Get_KeyWord_Data(CString& oneline, CString& keyword, CString& data);
	BOOL Get_Text_Data(CString& oneline, CString& keyword, CString& data);

	void Open_AD6_SCH_ASCII_File(CArchive& ar, int total_linenum, int nCurrentDiagram);
	void Save_AD6_SCH_ASCII_File(CArchive& ar, int nCurrentDiagram);
	void Open_AD6_PCB_ASCII_File(CArchive& ar, CString& firstline);
	void Save_AD6_PCB_ASCII_File(CArchive& ar);
	void Open_AD6_SCH_Binary_File(CFile* myfile, int nCurrentDiagram);
	void Open_AD6_SCH_File(int total_linenum, CStringArray& stringarray, int nCurrentDiagram);

	void Open_99_SCH_ASCII_File(CArchive& ar, int nCurrentDiagram);
	void Save_99_SCH_ASCII_File(CArchive& ar, int nCurrentDiagram);
	void Open_99_PCB_ASCII_File(CArchive& ar, CString& firstline);
	void Save_99_PCB_ASCII_File(CArchive& ar);

	void Open_Eda_V1(CArchive& ar);

	void Open_ESD(CArchive& ar, int version);
	void Save_ESD(CArchive& ar);


	int Divide_l(char *string, char s123[200][200]);
	int Divide_long(char *string, char s123[200][200]);
	void Get_Rotate_XY(double* fx, double* fy, double old_fx, double old_fy, int orientation, BOOL mirrored);
	void Get_Angle_XY(double* dx, double* dy, double old_dx, double old_dy, double angle);
	void Get_Rotate_Orientation(int* new_o, int old_o, int orientation, BOOL mirrored);
	void Get_Rotate_Angle(double* sangle, double* eangle, double old_sangle, double old_eangle, int orientation, BOOL mirrored);
	int ReadNetString(CFile * pFile, char * lpbuf);
	int EDA_V1_ESD_PCBLayerConvert(int oldlayer);
	void Convert_LayerNameToChinese();
	void Convert_LayerNameToEnglish();
	int CheckFontExist(LOGFONT* logfont, int diagram);
	void Get_Document_Maxy(double* fmax_x, double* fmax_y, double* fmin_x, double* fmin_y);
	int GetSchSheetUsed();
	BOOL CheckWireEnd(double fx, double fy, int nDiagram);	
	BOOL GetElectricalPoint(double* fx, double* fy, double fCurrentX, double fCurrentY, int nDiagram);
	BOOL PcbGetElectricalPoint(double* fx, double* fy, double fCurrentX, double fCurrentY, int layer);
	BOOL CheckJuncExist(double fx, double fy, int nDiagram);

	CString Convert_XY_To_String(double fxy);
	double Convert_String_To_XY(CString& cstring);
	double Convert_PCB_String_To_XY(CString& cstring);
	void mil_to_str(double fx, char *string, int format, int precision);	

	int Get_SCH_Elements_Param(int kind, int nDiagram, int type);

	int Get_PCB_Object(int kind);

	int Get_Correct_Orientation(int angle);
	double Get_Correct_Angle(double angle);

	BOOL InDistance_Point_Point(double fx1, double fy1, double fx2, double fy2, double distance);
	BOOL InDistance_Point_LINE(double fx, double fy, double fsx, double fsy, double fex, double fey, double distance);

	BOOL IsOnSignalLayer(int layer);
	BOOL IsConnectedToVia(int nLayer, COb_pcbvia* pvia);
	BOOL IsConnected_ViaToVia(COb_pcbvia* pvia1, COb_pcbvia* pvia2);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CESDDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnFileSaveAs();
	afx_msg void OnReportElements();
	afx_msg void OnPcbBoardOptions();
	afx_msg void OnPcbLayerStack();
	afx_msg void OnViewGridsToggleVisible();
	afx_msg void OnViewToggleUnits();
	afx_msg void OnPcbViewSnapSetup();
	afx_msg void OnPcbLayerSet();
	afx_msg void OnReportBoard();
	//afx_msg void OnPcbLibMake();
	//afx_msg void OnPcbLibMakelib();
	afx_msg void OnPcbNetEdit();
	afx_msg void OnPcbNetCleanall();
	afx_msg void OnPcbNetCleanone();
	afx_msg void OnPcbNetClearall();
	afx_msg void OnReportCompnet();
	afx_msg void OnPcbDesignRules();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ESDDOC_H__4E636A51_B851_4141_87F3_E7EA4C82E936__INCLUDED_)

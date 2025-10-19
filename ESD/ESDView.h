// ESD.h : interface of the CESDView class
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ESDVIEW_H__954BEB65_A878_40A1_A864_57F5C30DC303__INCLUDED_)
#define AFX_ESDVIEW_H__954BEB65_A878_40A1_A864_57F5C30DC303__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CESDView : public CScrollView
{
//protected: // create from serialization only
public:
	CESDView();
	DECLARE_DYNCREATE(CESDView)

// Attributes
public:
	CESDDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CESDView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CESDView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	int m_nMouse_X, m_nMouse_Y;

	int  m_nSystem_State;

	double  m_fScreenX;
	double  m_fScreenY;
	double  m_fScale;

	double  m_fSCH_ScreenX;
	double  m_fSCH_ScreenY;
	double  m_fSCH_Scale;

	double  m_fPCB_ScreenX;
	double  m_fPCB_ScreenY;
	double  m_fPCB_Scale;

	int  m_nDiagram;
	int  m_nPCB_Current_Layer;

	double  m_fCurrentSnapX, m_fCurrentSnapY;
	
	double  m_fCurrentX, m_fCurrentY;
	
	int  m_nViewport_Ey;
	int  m_nViewport_Ex;

	BOOL m_bActive;			
	BOOL m_bClipState;		
	BOOL m_bCursorState;	
	POINT m_CursorLocation;

	int m_nPrint_Flag;
	void FilePrint(void);
	int m_nPrint_X_Papernum;
	int m_nPrint_Y_Papernum;
	double m_fprint_orgx, m_fprint_orgy;

	void DefinePrintArea();
	void DefineSheetXY();

	int  m_nBackground_Pic;
	int  m_nBackground_Pic_Show;
	CBitmap m_Background_Pic;
	double m_fPic_Scale;
	int m_nPic_Offsetx, m_nPic_Offsety;

	// after we get the focus, windows sends us a mousemove message even if we did not touch the mouse. We set a flag to ignore that first mousemove
	bool		m_bAfterSetFocus;

	CArray <Struct_Find, Struct_Find&> m_arrayFindText;
	int m_indexFindText;
	
	CArray <Struct_ObjectPosition, Struct_ObjectPosition&> m_arrayHighlight[6];
	COLORREF m_nHighlightColor[6];
	BOOL m_bViewHighlightFlag;
	
	Struct_Jump m_sJump[10];
	Struct_Jump m_sPcbJump[10];

	BOOL m_bPcbFlipView;

	int m_nWhatIsUnderMouse;
	int m_nPcbWhatIsUnderMouse;

	Fpoint m_fpHotPoint1, m_fpHotPoint2;
	Fpoint m_fpPcbHotPoint1,  m_fpPcbHotPoint2;

	BOOL Package_Exist(CString &csDesignator);
	BOOL Load_Package(int nPartNo);

	CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&> m_arrayPcbHighlight;
	BOOL m_bPcbViewHighlightFlag;

// Generated message map functions
protected:
	//{{AFX_MSG(CESDView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void OnUpdateEditFind(CCmdUI* pCmd);
	void OnEditFind(NMHDR* pNMHDR, LRESULT* pResult);

public:

	void OnViewZoom_In();
	void OnViewZoom_Out();
	void OnViewZoomCenter();
	void OnViewZoomRedraw();
	void OnViewZoomSheet();
	void OnZoomSliderScroll(NMHDR* pNMHDR, LRESULT* pResult);
	void UpdateZoomSlider();
	void OnZoomIndicator();

	int MRetKey();
	void Command_In_Function(int ch1);

	void ElecHotPointsControl(CClientDC* pDC);
	void PcbElecHotPointsControl(CClientDC* pDC);

	int MouseKey(void);
	void DrawMyCursor(CClientDC* pDC, POINT* cpoint);
	double  Snap_Lock(double fxy);
	void Get_Current_XY(int mouse_x, int mouse_y);
	void Write_XY(double x,double y);
	void Get_LargeCross_XY(void);
	void Get_VPort_XY(int *vpx,int *vpy, double px, double py);
	void Screen_XYControl(void);
	void Restore_LargeCrossXY(void);
	void MoveCursorToCenter();
	void MoveCursorToXY(double fx, double fy);
	void DrawOnWindow(UINT x1,UINT y1,UINT x2,UINT y2);
	void ScrollBar_Control();
	void OnViewZoomSelect(UINT nID);
	void Pan_Left(void);
	void Pan_Right(void);
	void Pan_Up(void);
	void Pan_Down(void);
	void Change_Viewpoint(double fx, double fy, int diagram);
	void GetAllObjectsRect(Fpoint &fp1, Fpoint &fp2);
	void GetAllPCBObjectsRect(Fpoint &fp1, Fpoint &fp2);

	void CloseMouseCursor(void);
	void OpenMouseCursor(void);

	void DrawAllSCHObject(CDC* pDC);
	void DrawAllPCBObject(CDC* pDC);
	void DrawPcbOrigin(CDC* pDC);
	void DrawPcbConnections(CDC* pDC);
	void DrawPcbSelections(CDC* pDC);
	void DrawPcbHighlight(CDC* pDC, int mode);

	struct Struct_Multi_Selection SelectSCHObject(double fx, double fy, int mode);
	struct Struct_Multi_Selection FindSelectedObject(double fx, double fy);
	void OnMultiSelection(UINT nID);
	void SelectElement(int element, POSITION position);
	void SelectInsideArea(double fx1, double fy1, double fx2, double fy2, int nDiagram);
	void DeSelectInsideArea(double fx1, double fy1, double fx2, double fy2, int nDiagram);
	void DeSelectOutsideArea(double fx1, double fy1, double fx2, double fy2, int nDiagram);
	void MouseSelectInsideArea(void);
	void DrawSelectionKeyPoint(int x, int y, CDC* pDC);	
	void HideSelectionKeyPoint(int x, int y, CDC* pDC);	
	void SelectAll(BOOL Is_Selecting);
	void ToggleSelection(void);
	void DeleteSelection(void);
	void MoveAndRotateSelection(double x0, double y0,  double deltax, double deltay, int rotation, BOOL mirror);
	void CopyAndRotateSelection(double x0, double y0,  double deltax, double deltay, int rotation, BOOL mirror);
	Fpoint GetSelectionCenter(void);
	void ProcessingSelection(BOOL mode);
	int FindSelectionKeyPoint(double fx, double fy);
	void MoveSelection(void);
	
	struct Struct_Pcb_Multi_Selection SelectPcbObject(double fx, double fy, int mode);
	struct Struct_Pcb_Multi_Selection PcbFindSelectedObject(double fx, double fy);
	void PcbSelectElement(int element, int index);
	void PcbSelectInsideArea(double fx1, double fy1, double fx2, double fy2);
	void PcbDeSelectInsideArea(double fx1, double fy1, double fx2, double fy2);
	void PcbDeSelectOutsideArea(double fx1, double fy1, double fx2, double fy2);	
	void PcbMouseSelectInsideArea(void);

	void PcbSelectAll(BOOL Is_Selecting);
	void PcbToggleSelection(void);
	void PcbDeleteSelection(void);
	void PcbMoveAndRotateSelection(double x0, double y0,  double deltax, double deltay, int rotation, BOOL mirror);
	void PcbCopyAndRotateSelection(double x0, double y0,  double deltax, double deltay, int rotation, BOOL mirror);
	Fpoint PcbGetSelectionCenter(void);
	void PcbProcessingSelection(BOOL mode);	
	int PcbFindSelectionKeyPoint(double fx, double fy);
	void PcbMoveSelection(void);


	void EditElement(int element, POSITION position);
	void MoveElement(int element, POSITION position);
	void CopyElement(int element, POSITION position);
	void DeleteElement(int element, POSITION position);
	void PickupMoveOrCopy(int element, POSITION position);
	void AdvancedChangeMode(double fx, double fy);

	void PcbEditElement(int pcbelement, int index);
	void PcbDeleteElement(int pcbelement, int index);
	void PcbMoveElement(int pcbelement, int index);
	void PcbPickupMoveOrCopy(int pcbelement, int index);
	void PcbCopyElement(int pcbelement, int index);
	void PcbAdvancedChangeMode(double fx, double fy);

	void Plus_Sheet();
	void Minus_Sheet();
	void Plus_Layer();
	void Minus_Layer();
	void Write_Help(char* str);
	void Write_Info(char* str);
	void Error(char* string);
	void DrawStructButton(UINT nID, BOOL state);
	void JumpComponent(char* pointer);
	void JumpPackage(char* pointer);

	void Get_Rotate_XY(double* fx, double* fy, double old_fx, double old_fy, int orientation, BOOL mirrored);
	void Get_Rotate_Angle(double* sangle, double* eangle, double old_sangle, double old_eangle, int orientation, BOOL mirrored);
	void size_to_unitstr(double fx, char *string, int format, int precision, int unit);
	void mm_to_str(double fx, char *string);
	void mil_to_str(double fx, char *string);

	BOOL Exist_Junction(double x, double y);
	void SaveAreaRectangleToBitmap(CDC* pdc, CRect& rc, CBitmap** ppBitmap);
	void CopySavedAreaRectangleBack(CDC *pdc, CBitmap* pBitmap, CRect& rcOld);

	void AutoOptimizeWire();
	void AutoPlaceJunction();
	void AutoRemoveJunction();

	void HighlightWire(COb_schline* phighlightline, int n);
	void HighlightJunc(COb_schjunc* phighlightjunc, int n);
	void HighlightNetlabel(COb_schtext* phighlighttext, int n);
	void HighlightPower(COb_schpower* phighlightpower, int n);
	void HighlightPort(COb_schport* phighlightport, int n);
	void HighlightSheetEntry(COb_schsheetentry* phighlightsheetentry, int n);
	void HighlightPin(COb_schpin* phighlightpin, int n);
	BOOL HighlightExist(int diagram, POSITION pos, int n);
	
	void DrawPcbDrcErrors(CDC* pDC);
	void DrawHighlight(CDC* pDC);
	void DrawMask(CDC* pDC);
	CString GetNetName(int n);
	int GetHighlightPinNumber(int n);

	void CheckPcbCurrentLayer();
	void DrawDottedTrack(int x1, int y1, int x2, int y2, int width, CDC* pDC);
	BOOL Exist_PV_Near_Pad(COb_pcbpad* ppad_origin);
	BOOL Exist_PV_Near_Via(COb_pcbvia* pvia_origin);

	void Apply_Schnet_To_Pcb();
	void PCBMakeHighlight(BOOL bShowHighlight);
	void PCBHighlight_Line_Search(COb_pcbtrack* phighlight_track, BOOL bShowHighlight);
	void PCBHighlight_Pad_Search(COb_pcbpad* phighlight_pad, BOOL bShowHighlight);
	void PCBHighlight_Via_Search(COb_pcbvia* phighlight_via, BOOL bShowHighlight);
	void PCBHighlight_Arc_Search(COb_pcbarc* phighlight_arc, BOOL bShowHighlight);
	void PCBHighlight_Fill_Search(COb_pcbfill* phighlight_fill, BOOL bShowHighlight);
	void PCBHighlight_Region_Search(COb_pcbregion* phighlight_region, BOOL bShowHighlight);
	
	void PCBHighlightPolygon_Line_Search(COb_pcbtrack* phighlight_track, CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&>& arrayPolygonHighlight,  BOOL bShowHighlight);
	void PCBHighlightPolygon_Pad_Search(COb_pcbpad* phighlight_pad, CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&>& arrayPolygonHighlight,  BOOL bShowHighlight);
	void PCBHighlightPolygon_Via_Search(COb_pcbvia* phighlight_via, CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&>& arrayPolygonHighlight,  BOOL bShowHighlight);
	void PCBHighlightPolygon_Arc_Search(COb_pcbarc* phighlight_arc, CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&>& arrayPolygonHighlight,  BOOL bShowHighlight);
	void PCBHighlightPolygon_Fill_Search(COb_pcbfill* phighlight_fill, CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&>& arrayPolygonHighlight,  BOOL bShowHighlight);
	void PCBHighlightPolygon_Region_Search(COb_pcbregion* phighlight_region, CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&>& arrayPolygonHighlight, BOOL bShowHighlight);
	void PCBHighlight_PolygonElement_Search(CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&>& arrayPolygonHighlight, BOOL bShowHighlight);

	BOOL Line_Line_Connect(COb_pcbtrack* ptrack1, COb_pcbtrack* ptrack2, double drc);
	BOOL Line_Pad_Connect(COb_pcbtrack* ptrack, COb_pcbpad* ppad, double drc);
	BOOL Line_Via_Connect(COb_pcbtrack* ptrack, COb_pcbvia* pvia, double drc);
	BOOL Line_Arc_Connect(COb_pcbtrack* ptrack, COb_pcbarc* parc, double drc);
	BOOL Line_Fill_Connect(COb_pcbtrack* ptrack, COb_pcbfill* pfill, double drc);	
	BOOL Line_Region_Connect(COb_pcbtrack* ptrack, COb_pcbregion* pregion, double drc);	

	BOOL Pad_Pad_Connect(COb_pcbpad* ppad1, COb_pcbpad* ppad2, double drc);
	BOOL Pad_Via_Connect(COb_pcbpad* ppad, COb_pcbvia* pvia, double drc);
	BOOL Pad_Arc_Connect(COb_pcbpad* ppad, COb_pcbarc* parc, double drc);
	BOOL Pad_Fill_Connect(COb_pcbpad* ppad, COb_pcbfill* pfill, double drc);
	BOOL Pad_Region_Connect(COb_pcbpad* ppad, COb_pcbregion* pregion, double drc);

	BOOL Via_Via_Connect(COb_pcbvia* pvia1, COb_pcbvia* pvia2, double drc);
	BOOL Via_Arc_Connect(COb_pcbvia* pvia, COb_pcbarc* parc, double drc);
	BOOL Via_Fill_Connect(COb_pcbvia* pvia, COb_pcbfill* pfill, double drc);
	BOOL Via_Region_Connect(COb_pcbvia* pvia, COb_pcbregion* pregion, double drc);

	BOOL Arc_Arc_Connect(COb_pcbarc* parc1, COb_pcbarc* parc2, double drc);
	BOOL Arc_Fill_Connect(COb_pcbarc* parc, COb_pcbfill* pfill, double drc);
	BOOL Arc_Region_Connect(COb_pcbarc* parc1, COb_pcbregion* pregion, double drc);

	BOOL Fill_Fill_Connect(COb_pcbfill* pfill1, COb_pcbfill* pfill2, double drc);
	BOOL Fill_Region_Connect(COb_pcbfill* pfill, COb_pcbregion* pregion, double drc);

	BOOL Region_Region_Connect(COb_pcbregion* pregion1, COb_pcbregion* pregion2, double drc);

	BOOL Dist_P_P(double fx1, double fy1, double fx2, double fy2, double d);
	BOOL Dist_P_Line(double fx1, double fy1, double fxs, double fys, double fxe, double fye, double d);
	BOOL Dist_Line_Line(double aaa_fx1, double aaa_fy1, double aaa_fx2, double aaa_fy2, double bbb_fx1, double bbb_fy1, double bbb_fx2, double bbb_fy2, double d);
	BOOL Dist_P_Arc(double fx, double fy, double fcx, double fcy, double r, double sangle, double eangle, double d);
	BOOL Dist_Line_Arc(double fx1, double fy1, double fx2, double fy2, double fcx, double fcy, double r, double sangle, double eangle, double d);
	BOOL IsOnArc(double alpha, double sangle, double eangle);

	BOOL ExistInDrcErrors(int pcbelement, int index);
	void FindDrcErrors();
	BOOL ExistInOnLineDrcErrors(int pcbelement, int index);
	BOOL FindTrackOnLineDrcErrors(COb_pcbtrack* ptrack);
	BOOL FindArcOnLineDrcErrors(COb_pcbarc* parc);
	BOOL FindViaOnLineDrcErrors(COb_pcbvia* pvia);
	CArray<Fpoint, Fpoint> tempPoint;

	BOOL CreatePolygonPour(COb_pcbpolygon* ppolygon, int index);
	void PolygonPourDrawVertex(CDC* pDC, CArray<Fpoint, Fpoint&>& arrayVertex, double fminx, double fminy);
	BOOL IsOnSolidPoint(int x, int y, char* buffer,  int dx, int cy);
	BOOL IsInArcSweepRange(Struct_VertexOutline vt, double fx, double fy, double *fangle);
	void CleanVertex(CArray<Fpoint, Fpoint&>& arrayVertex);
	double GetRegionArea(COb_pcbregion* pregion);
	BOOL GetRegionFillLine(COb_pcbregion* pregion, CArray<Struct_VertexOutline, Struct_VertexOutline&>* pFillLine, double fTrackWidth, double fTrackGap, int nHatchStyle);
	BOOL GreatePadTearDrop(double fx, double fy, COb_pcbpad* ppad, COb_pcbtrack* ptrack, BOOL clear_undo_buffer_flag, int nRadioSize, CESDDoc* pDoc);
	BOOL GreateViaTearDrop(double fx, double fy, COb_pcbvia* pvia, COb_pcbtrack* ptrack, BOOL clear_undo_buffer_flag, int nRadioSize, CESDDoc* pDoc);

	void Refresh_Ratsnest();
	void Refresh_Package_Ratsnest(int index);
	void Refresh_Net_Ratsnest(int nNet);

	BOOL Route_H(COb_pcbconnection* pconnection);
	BOOL Route_V(COb_pcbconnection* pconnection);
	BOOL Route_L(COb_pcbconnection* pconnection);
	BOOL Route_Z(COb_pcbconnection* pconnection);
	BOOL Route_C(COb_pcbconnection* pconnection);
	BOOL Fast_Route_H(COb_pcbconnection* pconnection, CTypedPtrArray <CObArray, COb_pcbtrack*>& arrayTrack_H);
	BOOL Fast_Route_V(COb_pcbconnection* pconnection, CTypedPtrArray <CObArray, COb_pcbtrack*>& arrayTrack_V);

public:
	afx_msg void OnViewRefresh();
	afx_msg void OnViewPan();
	afx_msg void OnViewFitAllObjects();
	afx_msg void OnViewFitDocument();
	afx_msg void OnView100();
	afx_msg void OnView200();
	afx_msg void OnView400();
	afx_msg void OnView50();
	afx_msg void OnViewArea();
	afx_msg void OnViewCenter();
	afx_msg void OnViewZoomIn();
	afx_msg void OnViewZoomOut();
	afx_msg void OnViewZoomLast();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDesignOption();
	afx_msg void OnEditChange();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnPlaceWire();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnPlaceBus();
	afx_msg void OnPlacePolyline();
	afx_msg void OnPlaceJunction();
	afx_msg void OnPlaceText();
	afx_msg void OnPlaceArccenter();
	afx_msg void OnPlaceArcedged();
	afx_msg void OnPlaceEarcCenter();
	afx_msg void OnPlaceEarcEdged();
	afx_msg void OnPlaceEllipticalFill();
	afx_msg void OnPlaceNetlabel();
	afx_msg void OnPlacePie();
	afx_msg void OnPlacePolygon();
	afx_msg void OnPlaceRect();
	afx_msg void OnPlaceRoundrect();
	afx_msg void OnPlaceTextframe();
	afx_msg void OnPlaceSheetsymbol();
	afx_msg void OnPlaceImage();
	afx_msg void OnPlaceBezier();
	afx_msg void OnPlacePort();
	afx_msg void OnEditMove();
	afx_msg void OnPlaceBusentry();
	afx_msg void OnPlaceOffsheet();
	afx_msg void OnPlacePower();
	afx_msg void OnPlaceSheetentry();
	afx_msg void OnEditDeselectAll();
	afx_msg void OnEditSelectInside();
	afx_msg void OnEditSelectOutside();
	afx_msg void OnEditDeselectInside();
	afx_msg void OnEditDeselectOutside();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditDelete();
	afx_msg void OnEditMoveSelection();
	afx_msg void OnPlacePart();
	afx_msg void OnLibAdd();
	afx_msg void OnLibNew();
	afx_msg void OnLibCombine();
	afx_msg void OnLibBrowse();
	afx_msg void OnLibDelete();
	afx_msg void OnLibList();
	afx_msg void OnLibEdit();
	afx_msg void OnEditMoveFront();
	afx_msg void OnEditMoveBack();
	afx_msg void OnEditSelectionToggle();
	afx_msg void OnEditFindText();
	afx_msg void OnEditFindNext();
	afx_msg void OnEditReplaceText();
	afx_msg void OnUpdateEditFindNext(CCmdUI *pCmdUI);
	afx_msg void OnEditAlignLeft();
	afx_msg void OnEditAlignTop();
	afx_msg void OnEditAlignVplace();
	afx_msg void OnEditAlignHplace();
	afx_msg void OnPcbEditAlignLeft();
	afx_msg void OnPcbEditAlignTop();
	afx_msg void OnPcbEditAlignVplace();
	afx_msg void OnPcbEditAlignHplace();
	afx_msg void OnEditDuplicate();
	afx_msg void OnEditJump(UINT nID);
	afx_msg void OnEditSetJump(UINT nID);
	afx_msg void OnEditJumpOrigin();
	afx_msg void OnEditJumpLocation();
	afx_msg void OnEditJumpComponent();
	afx_msg void OnGridCycleSnap();
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnViewSelectedObjects();
	afx_msg void OnUpdateEditMoveSelection(CCmdUI *pCmdUI);
	afx_msg void OnEditRepeat();
	afx_msg void OnUpdateEditDuplicate(CCmdUI *pCmdUI);
	afx_msg void OnPlaceGround();
	afx_msg void OnUpdateViewSelectedObjects(CCmdUI *pCmdUI);
	afx_msg void OnEditBreakWire();
	afx_msg void OnGridToggleElectric();
	afx_msg void OnGridToggleVisible();
	afx_msg void OnUpdateGridToggleElectric(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGridToggleVisible(CCmdUI *pCmdUI);
	afx_msg void OnPcbGridVisible();
	afx_msg void OnPcbGridElectrical();
	afx_msg void OnUpdatePcbGridVisible(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePcbGridElectrical(CCmdUI *pCmdUI);
	afx_msg void OnAutoAnnotateReset();
	afx_msg void OnAutoAnnotate();

	afx_msg void OnHighlight(UINT nID);

	afx_msg void OnHighlightReset();
	afx_msg void OnUpdateHighlightReset(CCmdUI *pCmdUI);

	afx_msg void OnHighlightList();
	afx_msg void OnUpdateHighlightList(CCmdUI *pCmdUI);

	afx_msg void OnHighlightView();
	afx_msg void OnUpdateHighlightView(CCmdUI *pCmdUI);
	afx_msg void OnAutoCompile();
	afx_msg void OnReportBm1();
	afx_msg void OnReportBm2();
	afx_msg void OnReportErr();
	afx_msg void OnReportWir();
	afx_msg void OnReportNet();
	afx_msg void OnViewFlipBoard();
	afx_msg void OnViewFitBoard();
	afx_msg void OnEditJumpCurrentOrigin();
	afx_msg void OnEditOriginReset();
	afx_msg void OnEditOriginSet();
	afx_msg void OnPcbPlaceArcCenter();
	afx_msg void OnPcbPlaceArcCircle();
	afx_msg void OnPcbPlaceArcEdge();
	afx_msg void OnPcbPlaceTrack();
	afx_msg void OnPcbPlaceVia();
	afx_msg void OnPcbPlacePad();
	afx_msg void OnPcbPlaceString();
	afx_msg void OnPcbPlaceFill();
	afx_msg void OnPcbPlaceRegion();
	afx_msg void OnPcbPlacePolygonCutout();
	afx_msg void OnPcbPlacePolygon();
	afx_msg void OnPcbPlaceComp();
	afx_msg void OnPcbPlaceCoordinate();
	afx_msg void OnPcbPlaceDimension();
	afx_msg void OnPcbLibBrowse();
	afx_msg void OnPcbLibAdd();
	afx_msg void OnPcbBoardShape();
	afx_msg void OnPcbEditDuplicate();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPcbEditMoveSelection();
	afx_msg void OnPcbToolLoadComponent();
	afx_msg void OnPcbToolPlacementManual();
	afx_msg void OnPcbToolRatsHideAll();
	afx_msg void OnPcbToolRatsHideComp();
	afx_msg void OnPcbToolRatsHideNet();
	afx_msg void OnPcbToolRatsShowAll();
	afx_msg void OnPcbToolRatsShowComp();
	afx_msg void OnPcbToolRatsShowNet();
	afx_msg void OnPcbToolHighlightClear();
	afx_msg void OnPcbToolHighlightLocation();
	afx_msg void OnPcbToolHighlightNet();
	afx_msg void OnPcbToolHighlightPins();
	afx_msg void OnPcbEditRepeat();
	afx_msg void OnPcbJumpComp();
	afx_msg void OnPcbJumpText();
	afx_msg void OnPcbToolDrc();
	afx_msg void OnPcbReportDrc();
	afx_msg void OnPcbToolClearDrcerror();
	afx_msg void OnPcbToolExplodeComponent();
	afx_msg void OnPcbToolExplodePolygon();
	afx_msg void OnPcbToolCreateComponent();
	afx_msg void OnPcbEditCut();
	afx_msg void OnUpdatePcbEditCut(CCmdUI *pCmdUI);
	afx_msg void OnPcbEditPaste();
	afx_msg void OnUpdatePcbEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnPcbEditCopy();
	afx_msg void OnUpdatePcbEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnRouteManual();
	afx_msg void OnRouteAll();

	afx_msg void OnPcbToolUnrouteAll();
	afx_msg void OnPcbToolUnrouteComponent();
	afx_msg void OnPcbToolUnrouteConnection();
	afx_msg void OnPcbToolUnrouteNet();
	afx_msg void OnDesignRuleWizard();
	afx_msg void OnPcbLibDelete();
	afx_msg void OnPcbLibEdit();
	afx_msg void OnPcbLibList();
	afx_msg void OnPcbToolTear();
	afx_msg void OnPcbToolPerference();
	afx_msg void OnPcbBoardVertex();
	afx_msg void OnPcbBoardMove();
	afx_msg void OnPcbViewHighlight();
	afx_msg void OnUpdatePcbViewHighlight(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePcbToolHighlightClear(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePcbToolHighlightPins(CCmdUI *pCmdUI);
	afx_msg void OnPcbSplitPlane();
	afx_msg void OnPcbToolRatsRefresh();
	afx_msg void OnPcbLayerSetup();
	afx_msg void OnPcbMeasureDistance();
};

#ifndef _DEBUG  // debug version in ESDView.cpp
inline CESDDoc* CESDView::GetDocument()
	{ return (CESDDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ESDVIEW_H__954BEB65_A878_40A1_A864_57F5C30DC303__INCLUDED_)


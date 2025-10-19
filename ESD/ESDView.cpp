// ESD.cpp : implementation of the CESDView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESD.h"
#include "systemext.h"
//#include "function.h"

#include "ESDDoc.h"
#include "ESDView.h"

#include "MainFrm.h"
#include "Childfrm.h"
#include ".\esdview.h"

#include "Sheet_SchOptions.h"
#include "XTPPreviewView.h"

#include "Sheet_EditWire.h"
//#include "Page_EditWire.h"
#include "Sheet_EditBus.h"
#include "Sheet_EditPolyline.h"
//#include "Page_EditPolyline.h"
//#include "Page_EditBus.h"
#include "Dlg_EditBusEntry.h"
//#include "Page_Vertices.h"

#include "Dlg_EditJunction.h"
#include "Dlg_EditText.h"
#include "Dlg_EditNetlabel.h"
#include "Dlg_EditArc.h"
#include "Dlg_EditEllipse.h"
#include "Dlg_EditEllipsefill.h"
#include "Dlg_EditPie.h"
#include "Sheet_EditPolygon.h"
//#include "Page_EditPolygon.h"
#include "Dlg_EditRect.h"
#include "Dlg_EditRoundRect.h"
#include "Dlg_EditPower.h"
#include "Dlg_EditPort.h"
#include "Dlg_EditOffsheet.h"
#include "Dlg_EditSheetEntry.h"
#include "Dlg_EditSheetSymbol.h"
#include "Dlg_PlaceComp.h"
#include "Dlg_SchLib_Add.h"
#include "Dlg_SchLib_New.h"
#include "Dlg_SchLib_Browse.h"
#include "Dlg_SchLib_SourceComp.h"
#include "Dlg_SchLib_Attrib.h"
#include "Dlg_SchLib_List.h"
#include "Dlg_Edit_FindText.h"
#include "Dlg_Edit_ReplaceText.h"
#include "Dlg_Edit_JumpLocation.h"
#include "Dlg_Edit_JumpComp.h"
#include "Dlg_Edit_Repeat.h"
#include "Dlg_AnnotateReset.h"
#include "Dlg_Annotate.h"
#include "Dlg_HighlightList.h"
#include "Dlg_Report.h"

#include "Dlg_PCBMultiSelection.h"
#include "Dlg_PCBEditArc.h"
#include "Dlg_PCBEditTrack.h"
#include "Dlg_PCBEditVia.h"
#include "Dlg_PCBEditPad.h"
#include "Dlg_PCBEditFill.h"
#include "Dlg_PCBEditText.h"
#include "Dlg_PCBEditPolygon.h"
#include "Dlg_PCBEditCoordinate.h"
#include "Dlg_PCBEditDimension.h"
#include "Dlg_PCBLib_Browse.h"
#include "Dlg_PCBLib_Attrib.h"
#include "Dlg_PCBLib_Add.h"
#include "Dlg_PlacePackage.h"

#include "Dlg_Missing_Component.h"
#include "Dlg_PCB_LoadPackageXY.h"
#include "Dlg_PCB_LoadPackageByName.h"
#include "Dlg_PcbHighlightPins.h"
#include "Dlg_PcbJumpComp.h"
#include "Dlg_PcbNetName.h"
#include "Dlg_PcbTearDrop.h"
#include "Dlg_PcbPerference.h"

#include "Dlg_PCBLayerSetup.h"
#include "ReportCtrlItem.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESDView

IMPLEMENT_DYNCREATE(CESDView, CScrollView)

BEGIN_MESSAGE_MAP(CESDView, CScrollView)
	//{{AFX_MSG_MAP(CESDView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_VIEW_PAN, OnViewPan)
	ON_COMMAND(ID_VIEW_FIT_ALL_OBJECTS, OnViewFitAllObjects)
	ON_COMMAND(ID_VIEW_FIT_DOCUMENT, OnViewFitDocument)
	ON_COMMAND(ID_VIEW_ZOOM_IN, OnViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOM_OUT, OnViewZoomOut)
	ON_COMMAND(ID_VIEW_ZOOM_LAST, OnViewZoomLast)
	ON_COMMAND(ID_VIEW_CENTER, OnViewCenter)
	ON_COMMAND(ID_VIEW_AREA, OnViewArea)
	ON_COMMAND(ID_VIEW_100, OnView100)
	ON_COMMAND(ID_VIEW_200, OnView200)
	ON_COMMAND(ID_VIEW_400, OnView400)
	ON_COMMAND(ID_VIEW_50, OnView50)
	
	ON_WM_SIZE()	
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()	
	ON_WM_SETFOCUS()	
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()	
	ON_WM_CHAR()
	ON_WM_SYSCHAR()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()	

	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)

	//ON_XTP_EXECUTE(ID_EDIT_FIND_EX, OnEditFind)
	//ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_EX, OnUpdateEditFind)

	ON_WM_ACTIVATE()

	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_DESIGN_OPTION, OnDesignOption)
	ON_COMMAND(ID_EDIT_CHANGE, OnEditChange)
	ON_COMMAND_RANGE(ID_SELECTION_1,ID_SELECTION_20,OnMultiSelection)

	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_PLACE_WIRE, OnPlaceWire)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_PLACE_BUS, OnPlaceBus)
	ON_COMMAND(ID_PLACE_POLYLINE, OnPlacePolyline)
	ON_COMMAND(ID_PLACE_JUNCTION, OnPlaceJunction)
	ON_COMMAND(ID_PLACE_TEXT, OnPlaceText)
	ON_COMMAND(ID_PLACE_ARCCENTER, OnPlaceArccenter)
	ON_COMMAND(ID_PLACE_ARCEDGED, OnPlaceArcedged)
	ON_COMMAND(ID_PLACE_EARC_CENTER, OnPlaceEarcCenter)
	ON_COMMAND(ID_PLACE_EARC_DEGED, OnPlaceEarcEdged)
	ON_COMMAND(ID_PLACE_ELLIPTICAL_FILL, OnPlaceEllipticalFill)
	ON_COMMAND(ID_PLACE_NETLABEL, OnPlaceNetlabel)
	ON_COMMAND(ID_PLACE_PIE, OnPlacePie)
	ON_COMMAND(ID_PLACE_POLYGON, OnPlacePolygon)
	ON_COMMAND(ID_PLACE_RECT, OnPlaceRect)
	ON_COMMAND(ID_PLACE_ROUNDRECT, OnPlaceRoundrect)
	ON_COMMAND(ID_PLACE_TEXTFRAME, OnPlaceTextframe)
	ON_COMMAND(ID_PLACE_SHEETSYMBOL, OnPlaceSheetsymbol)
	ON_COMMAND(ID_PLACE_IMAGE, OnPlaceImage)
	ON_COMMAND(ID_PLACE_BEZIER, OnPlaceBezier)
	ON_COMMAND(ID_PLACE_PORT, OnPlacePort)
	ON_COMMAND(ID_EDIT_MOVE, OnEditMove)
	ON_COMMAND(ID_PLACE_BUSENTRY, OnPlaceBusentry)
	ON_COMMAND(ID_PLACE_OFFSHEET, OnPlaceOffsheet)
	ON_COMMAND(ID_PLACE_POWER, OnPlacePower)
	ON_COMMAND(ID_PLACE_SHEETENTRY, OnPlaceSheetentry)
	ON_COMMAND(ID_EDIT_DESELECT_ALL, OnEditDeselectAll)
	ON_COMMAND(ID_EDIT_SELECT_INSIDE, OnEditSelectInside)
	ON_COMMAND(ID_EDIT_SELECT_OUTSIDE, OnEditSelectOutside)
	ON_COMMAND(ID_EDIT_DESELECT_INSIDE, OnEditDeselectInside)
	ON_COMMAND(ID_EDIT_DESELECT_OUTSIDE, OnEditDeselectOutside)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_MOVE_SELECTION, OnEditMoveSelection)
	ON_COMMAND(ID_PLACE_PART, OnPlacePart)
	ON_COMMAND(ID_LIB_ADD, OnLibAdd)
	ON_COMMAND(ID_LIB_NEW, OnLibNew)
	ON_COMMAND(ID_LIB_COMBINE, OnLibCombine)
	ON_COMMAND(ID_LIB_BROWSE, OnLibBrowse)
	ON_COMMAND(ID_LIB_DELETE, OnLibDelete)
	ON_COMMAND(ID_LIB_LIST, OnLibList)
	ON_COMMAND(ID_LIB_EDIT, OnLibEdit)
	ON_COMMAND(ID_EDIT_MOVE_FRONT, OnEditMoveFront)
	ON_COMMAND(ID_EDIT_MOVE_BACK, OnEditMoveBack)
	ON_COMMAND(ID_EDIT_SELECTION_TOGGLE, OnEditSelectionToggle)
	ON_COMMAND(ID_EDIT_FIND, OnEditFindText)
	ON_COMMAND(ID_EDIT_FIND_NEXT, OnEditFindNext)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplaceText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_NEXT, OnUpdateEditFindNext)
	ON_COMMAND(ID_EDIT_ALIGN_LEFT, OnEditAlignLeft)
	ON_COMMAND(ID_EDIT_ALIGN_TOP, OnEditAlignTop)
	ON_COMMAND(ID_EDIT_ALIGN_VPLACE, OnEditAlignVplace)
	ON_COMMAND(ID_EDIT_ALIGN_HPLACE, OnEditAlignHplace)
	ON_COMMAND(ID_PCB_EDIT_ALIGN_LEFT, OnPcbEditAlignLeft)
	ON_COMMAND(ID_PCB_EDIT_ALIGN_TOP, OnPcbEditAlignTop)
	ON_COMMAND(ID_PCB_EDIT_ALIGN_VPLACE, OnPcbEditAlignVplace)
	ON_COMMAND(ID_PCB_EDIT_ALIGN_HPLACE, OnPcbEditAlignHplace)
	ON_COMMAND(ID_EDIT_DUPLICATE, OnEditDuplicate)
	ON_COMMAND_RANGE(ID_EDIT_JUMP_1,ID_EDIT_JUMP_10,OnEditJump)
	ON_COMMAND_RANGE(ID_EDIT_SETJUMP_1,ID_EDIT_SETJUMP_10,OnEditSetJump)
	ON_COMMAND(ID_EDIT_JUMP_ORIGIN, OnEditJumpOrigin)
	ON_COMMAND(ID_EDIT_JUMP_LOCATION, OnEditJumpLocation)
	ON_COMMAND(ID_EDIT_JUMP_COMPONENT, OnEditJumpComponent)
	ON_COMMAND(ID_GRID_CYCLE_SNAP, OnGridCycleSnap)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_VIEW_SELECTED_OBJECTS, OnViewSelectedObjects)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVE_SELECTION, OnUpdateEditMoveSelection)
	ON_COMMAND(ID_EDIT_REPEAT, OnEditRepeat)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DUPLICATE, OnUpdateEditDuplicate)
	ON_NOTIFY(XTP_SBN_SCROLL, ID_INDICATOR_ZOOMSLIDER, OnZoomSliderScroll)
	ON_COMMAND(ID_INDICATOR_ZOOM, OnZoomIndicator)
	ON_COMMAND(ID_PLACE_GROUND, OnPlaceGround)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECTED_OBJECTS, OnUpdateViewSelectedObjects)
	ON_COMMAND(ID_EDIT_BREAK_WIRE, OnEditBreakWire)
	ON_COMMAND(ID_GRID_TOGGLE_ELECTRIC, OnGridToggleElectric)
	ON_COMMAND(ID_GRID_TOGGLE_VISIBLE, OnGridToggleVisible)
	ON_UPDATE_COMMAND_UI(ID_GRID_TOGGLE_ELECTRIC, OnUpdateGridToggleElectric)
	ON_UPDATE_COMMAND_UI(ID_GRID_TOGGLE_VISIBLE, OnUpdateGridToggleVisible)
	ON_COMMAND(ID_PCB_GRID_VISIBLE, OnPcbGridVisible)
	ON_COMMAND(ID_PCB_GRID_ELECTRICAL, OnPcbGridElectrical)
	ON_UPDATE_COMMAND_UI(ID_PCB_GRID_VISIBLE, OnUpdatePcbGridVisible)
	ON_UPDATE_COMMAND_UI(ID_PCB_GRID_ELECTRICAL, OnUpdatePcbGridElectrical)
	
	ON_COMMAND(ID_AUTO_ANNOTATE_RESET, OnAutoAnnotateReset)
	ON_COMMAND(ID_AUTO_ANNOTATE, OnAutoAnnotate)
	//highlight
	ON_COMMAND_RANGE(ID_HIGHLIGHT1,ID_HIGHLIGHT6,OnHighlight)
	//highlight resetall
	ON_COMMAND(ID_HIGHLIGHT_RESET, OnHighlightReset)
	ON_UPDATE_COMMAND_UI(ID_HIGHLIGHT_RESET, OnUpdateHighlightReset)
	//highlight list first
	ON_COMMAND(ID_HIGHLIGHT_LIST, OnHighlightList)
	ON_UPDATE_COMMAND_UI(ID_HIGHLIGHT_LIST, OnUpdateHighlightList)
	//highlight view first
	ON_COMMAND(ID_VIEW_HIGHLIGHT, OnHighlightView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HIGHLIGHT, OnUpdateHighlightView)
	
	ON_COMMAND(ID_AUTO_COMPILE, OnAutoCompile)
	ON_COMMAND(ID_REPORT_BM1, OnReportBm1)
	ON_COMMAND(ID_REPORT_BM2, OnReportBm2)
	ON_COMMAND(ID_REPORT_ERR, OnReportErr)
	ON_COMMAND(ID_REPORT_WIR, OnReportWir)
	ON_COMMAND(ID_REPORT_NET, OnReportNet)
	ON_COMMAND(ID_VIEW_FLIP_BOARD, OnViewFlipBoard)
	ON_COMMAND(ID_VIEW_FIT_BOARD, OnViewFitBoard)
	ON_COMMAND(ID_EDIT_JUMP_CURRENT_ORIGIN, OnEditJumpCurrentOrigin)
	ON_COMMAND(ID_EDIT_ORIGIN_RESET, OnEditOriginReset)
	ON_COMMAND(ID_EDIT_ORIGIN_SET, OnEditOriginSet)
	ON_COMMAND(ID_PCB_PLACE_ARC_CENTER, OnPcbPlaceArcCenter)
	ON_COMMAND(ID_PCB_PLACE_ARC_CIRCLE, OnPcbPlaceArcCircle)
	ON_COMMAND(ID_PCB_PLACE_ARC_EDGE, OnPcbPlaceArcEdge)
	ON_COMMAND(ID_PCB_PLACE_TRACK, OnPcbPlaceTrack)
	ON_COMMAND(ID_PCB_PLACE_VIA, OnPcbPlaceVia)
	ON_COMMAND(ID_PCB_PLACE_PAD, OnPcbPlacePad)
	ON_COMMAND(ID_PCB_PLACE_STRING, OnPcbPlaceString)
	ON_COMMAND(ID_PCB_PLACE_FILL, OnPcbPlaceFill)
	ON_COMMAND(ID_PCB_PLACE_REGION, OnPcbPlaceRegion)
	ON_COMMAND(ID_PCB_PLACE_POLYGON_CUTOUT, OnPcbPlacePolygonCutout)
	ON_COMMAND(ID_PCB_PLACE_POLYGON, OnPcbPlacePolygon)
	ON_COMMAND(ID_PCB_PLACE_COMP, OnPcbPlaceComp)
	ON_COMMAND(ID_PCB_PLACE_COORDINATE, OnPcbPlaceCoordinate)
	ON_COMMAND(ID_PCB_PLACE_DIMENSION, OnPcbPlaceDimension)
	ON_COMMAND(ID_PCB_LIB_BROWSE, OnPcbLibBrowse)
	ON_COMMAND(ID_PCB_LIB_ADD, OnPcbLibAdd)
	ON_COMMAND(ID_PCB_BOARD_SHAPE, OnPcbBoardShape)
	ON_COMMAND(ID_PCB_EDIT_DUPLICATE, OnPcbEditDuplicate)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_COMMAND(ID_PCB_EDIT_MOVE_SELECTION, OnPcbEditMoveSelection)
	ON_COMMAND(ID_PCB_TOOL_LOAD_COMPONENT, OnPcbToolLoadComponent)
	ON_COMMAND(ID_PCB_TOOL_PLACEMENT_MANUAL, OnPcbToolPlacementManual)
	ON_COMMAND(ID_PCB_TOOL_RATS_HIDE_ALL, OnPcbToolRatsHideAll)
	ON_COMMAND(ID_PCB_TOOL_RATS_HIDE_COMP, OnPcbToolRatsHideComp)
	ON_COMMAND(ID_PCB_TOOL_RATS_HIDE_NET, OnPcbToolRatsHideNet)
	ON_COMMAND(ID_PCB_TOOL_RATS_SHOW_ALL, OnPcbToolRatsShowAll)
	ON_COMMAND(ID_PCB_TOOL_RATS_SHOW_COMP, OnPcbToolRatsShowComp)
	ON_COMMAND(ID_PCB_TOOL_RATS_SHOW_NET, OnPcbToolRatsShowNet)
	ON_COMMAND(ID_PCB_TOOL_HIGHLIGHT_CLEAR, OnPcbToolHighlightClear)
	ON_COMMAND(ID_PCB_TOOL_HIGHLIGHT_LOCATION, OnPcbToolHighlightLocation)
	ON_COMMAND(ID_PCB_TOOL_HIGHLIGHT_NET, OnPcbToolHighlightNet)
	ON_COMMAND(ID_PCB_TOOL_HIGHLIGHT_PINS, OnPcbToolHighlightPins)
	ON_COMMAND(ID_PCB_EDIT_REPEAT, OnPcbEditRepeat)
	ON_COMMAND(ID_PCB_JUMP_COMP, OnPcbJumpComp)
	ON_COMMAND(ID_PCB_JUMP_TEXT, OnPcbJumpText)
	ON_COMMAND(ID_PCB_TOOL_DRC, OnPcbToolDrc)
	ON_COMMAND(ID_PCB_REPORT_DRC, OnPcbReportDrc)
	ON_COMMAND(ID_PCB_TOOL_CLEAR_DRCERROR, OnPcbToolClearDrcerror)
	ON_COMMAND(ID_PCB_TOOL_EXPLODE_COMPONENT, OnPcbToolExplodeComponent)
	ON_COMMAND(ID_PCB_TOOL_EXPLODE_POLYGON, OnPcbToolExplodePolygon)
	ON_COMMAND(ID_PCB_TOOL_CREATE_COMPONENT, OnPcbToolCreateComponent)
	ON_COMMAND(ID_PCB_EDIT_CUT, OnPcbEditCut)
	ON_UPDATE_COMMAND_UI(ID_PCB_EDIT_CUT, OnUpdatePcbEditCut)
	ON_COMMAND(ID_PCB_EDIT_PASTE, OnPcbEditPaste)
	ON_UPDATE_COMMAND_UI(ID_PCB_EDIT_PASTE, OnUpdatePcbEditPaste)
	ON_COMMAND(ID_PCB_EDIT_COPY, OnPcbEditCopy)
	ON_UPDATE_COMMAND_UI(ID_PCB_EDIT_COPY, OnUpdatePcbEditCopy)
	ON_COMMAND(ID_ROUTE_MANUAL, OnRouteManual)
	ON_COMMAND(ID_ROUTE_ALL, OnRouteAll)
	//ON_COMMAND(ID_ROUTE_COMPONENT, OnRouteComponent)
	//ON_COMMAND(ID_ROUTE_NET, OnRouteNet)
	//ON_COMMAND(ID_ROUTE_SETUP, OnRouteSetup)
	ON_COMMAND(ID_PCB_TOOL_UNROUTE_ALL, OnPcbToolUnrouteAll)
	ON_COMMAND(ID_PCB_TOOL_UNROUTE_COMPONENT, OnPcbToolUnrouteComponent)
	ON_COMMAND(ID_PCB_TOOL_UNROUTE_CONNECTION, OnPcbToolUnrouteConnection)
	ON_COMMAND(ID_PCB_TOOL_UNROUTE_NET, OnPcbToolUnrouteNet)
	ON_COMMAND(ID_DESIGN_RULE_WIZARD, OnDesignRuleWizard)
	ON_COMMAND(ID_PCB_LIB_DELETE, OnPcbLibDelete)
	ON_COMMAND(ID_PCB_LIB_EDIT, OnPcbLibEdit)
	ON_COMMAND(ID_PCB_LIB_LIST, OnPcbLibList)
	ON_COMMAND(ID_PCB_TOOL_TEAR, OnPcbToolTear)
	ON_COMMAND(ID_PCB_TOOL_PERFERENCE, OnPcbToolPerference)
	ON_COMMAND(ID_PCB_BOARD_VERTEX, OnPcbBoardVertex)
	ON_COMMAND(ID_PCB_BOARD_MOVE, OnPcbBoardMove)
	ON_COMMAND(ID_PCB_VIEW_HIGHLIGHT, OnPcbViewHighlight)
	ON_UPDATE_COMMAND_UI(ID_PCB_VIEW_HIGHLIGHT, OnUpdatePcbViewHighlight)
	ON_UPDATE_COMMAND_UI(ID_PCB_TOOL_HIGHLIGHT_CLEAR, OnUpdatePcbToolHighlightClear)
	ON_UPDATE_COMMAND_UI(ID_PCB_TOOL_HIGHLIGHT_PINS, OnUpdatePcbToolHighlightPins)
	ON_COMMAND(ID_PCB_SPLIT_PLANE, OnPcbSplitPlane)
	ON_COMMAND(ID_PCB_TOOL_RATS_REFRESH, OnPcbToolRatsRefresh)
	ON_COMMAND(ID_PCB_LAYER_SETUP, OnPcbLayerSetup)
	ON_COMMAND(ID_PCB_MEASURE_DISTANCE, OnPcbMeasureDistance)
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESDView construction/destruction

//static int mouse_fx, mouse_fy;

CESDView::CESDView()
{
	// TODO: add construction code here
	m_bActive = FALSE;
	m_bClipState = FALSE;
	m_bCursorState = FALSE; 
	m_bAfterSetFocus = FALSE;
	
	m_nWhatIsUnderMouse = NOTHING_SELECTION;
	m_nPcbWhatIsUnderMouse = NOTHING_SELECTION;

	m_CursorLocation.x = m_CursorLocation.y = 0;

	m_nDiagram = 0;
	m_nPCB_Current_Layer = 0;

	m_fScale = 10.0;
	m_fScreenX = m_fScreenY = 0;
	m_fSCH_Scale = 10.0;
	m_fSCH_ScreenX = m_fSCH_ScreenY = 0;
	m_fPCB_Scale = 10.0;
	m_fPCB_ScreenX = m_fPCB_ScreenY = 0;

	m_nSystem_State = nEDA_System_State;  
	
	m_nBackground_Pic = 0;
	m_nBackground_Pic_Show = 0;
	m_fPic_Scale = 3.333333f;
	m_nPic_Offsetx = m_nPic_Offsety = 0;

	m_nPrint_Flag = 0;

	SetScrollSizes(MM_TEXT, CSize(0,0));//, CSize(0,0), CSize(0,0));

	m_arrayFindText.RemoveAll();
	m_indexFindText = 0;

	for(int i=0; i<10; i++)
	{
		//sch
		m_sJump[i].fx = m_sJump[i].fy = -1;
		m_sJump[i].nDiagram = 0;
		//pcb
		m_sPcbJump[i].fx = m_sPcbJump[i].fy = -1;
	}

	m_fpHotPoint1.fx = 0;
	m_fpHotPoint1.fy = 0;
	m_fpHotPoint2.fx = 0;
	m_fpHotPoint2.fy = 0;
	m_fpPcbHotPoint1.fx = 0;
	m_fpPcbHotPoint1.fy = 0;
	m_fpPcbHotPoint2.fx = 0;
	m_fpPcbHotPoint2.fy = 0;


	m_nHighlightColor[0] = RGB(150,150,255);
	m_nHighlightColor[1] = RGB(255,150,150);
	m_nHighlightColor[2] = RGB(150,255,150);
	m_nHighlightColor[3] = RGB(0,0,255);
	m_nHighlightColor[4] = RGB(255,0,0);
	m_nHighlightColor[5] = RGB(0,255,0);

	m_bViewHighlightFlag = FALSE;
	m_bPcbViewHighlightFlag = FALSE;

	m_bPcbFlipView = FALSE;
}

CESDView::~CESDView()
{
}

BOOL CESDView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = csESDViewClassName;

	BOOL bPreCreated = CScrollView::PreCreateWindow(cs);

	return bPreCreated;
}

void CESDView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(m_nSystem_State == SCH)
	{
		// current in use
		m_fScale = pDoc->m_fSCH_Scale[m_nDiagram];
		m_fScreenX = pDoc->m_fSCH_ScreenX[m_nDiagram];
		m_fScreenY = pDoc->m_fSCH_ScreenY[m_nDiagram];
		// for switch to sch
		m_fSCH_Scale = m_fScale;
		m_fSCH_ScreenX = m_fScreenX;
		m_fSCH_ScreenY = m_fScreenY;
		// for switch to pcb
		m_fPCB_Scale = pDoc->m_fPCB_Scale;
		m_fPCB_ScreenX = pDoc->m_fPCB_ScreenX;
		m_fPCB_ScreenY = pDoc->m_fPCB_ScreenY;
		//}
	}
	else if(m_nSystem_State == PCB)
	{
		// current in use
		m_fScale = pDoc->m_fPCB_Scale;
		m_fScreenX = pDoc->m_fPCB_ScreenX;
		m_fScreenY = pDoc->m_fPCB_ScreenY;
		// for switch to sch
		m_fSCH_Scale = pDoc->m_fSCH_Scale[m_nDiagram];
		m_fSCH_ScreenX = pDoc->m_fSCH_ScreenX[m_nDiagram];
		m_fSCH_ScreenY = pDoc->m_fSCH_ScreenY[m_nDiagram];
		// for switch to pcb
		m_fPCB_Scale = m_fScale;
		m_fPCB_ScreenX = m_fScreenX;
		m_fPCB_ScreenY = m_fScreenY;
		pFrame->OnToPcb();
	}
	
	UpdateZoomSlider();
	Screen_XYControl();
	ScrollBar_Control();
}

void CESDView::OnDraw(CDC* pDC)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	if(!pDC->IsPrinting())	
	{
		if(m_nPrint_Flag != 0) return;
		CESDDoc* pDoc = (CESDDoc*)GetDocument();
		ASSERT_VALID(pDoc);

		CRect rect;
		GetClientRect(&rect);
		m_nViewport_Ex = rect.right;
		m_nViewport_Ey = rect.bottom;
				
		pDC->SetWindowOrg(0,0);
		pDC->SetViewportOrg(0,0);
		pDC->SetWindowExt(m_nViewport_Ex, m_nViewport_Ey);
		pDC->SetViewportExt(m_nViewport_Ex, m_nViewport_Ey);

		nEDA_RedrawFlag = ON;

		CDC memdc;
		memdc.CreateCompatibleDC(pDC);
		CBitmap bmpFace; 
		bmpFace.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height()); 
		CBitmap* pOldBmp = NULL; 
		pOldBmp = memdc.SelectObject(&bmpFace); 

		CBrush *oldbrush;
		CBrush brush;
		if(m_nSystem_State == SCH)
		{
			brush.CreateSolidBrush(RGB(255,255,255));
		}
		else
		{
			brush.CreateSolidBrush(nEDA_PCB_WorkspaceColor);
		}
		oldbrush = memdc.SelectObject(&brush);
		memdc.FillRect(&rect,&brush);
		memdc.SelectObject(oldbrush);

		if(m_nSystem_State == SCH)
		{	 
			pDoc->m_pSCH_Sheet[m_nDiagram]->Draw(&memdc, this);
			DrawAllSCHObject(&memdc);
			if(m_bViewHighlightFlag == TRUE) DrawMask(&memdc);
			DrawHighlight(&memdc);

			int drawmode = memdc.GetROP2();
			memdc.SetROP2(R2_NOTXORPEN);
			
			CPen apen;
			apen.CreatePen(PS_SOLID, 1, RGB(255, 0 ,0));
			CPen* oldpen = memdc.SelectObject(&apen);

			if(( m_fpHotPoint2.fx != 0) && ( m_fpHotPoint2.fy != 0))
			{
				int x,y;
				Get_VPort_XY(&x, &y, m_fpHotPoint2.fx, m_fpHotPoint2.fy);
				if((nEDA_Cursor_Mode == 0)||(nEDA_Cursor_Mode == 1))
				{
					memdc.MoveTo(x-20, y-20); memdc.LineTo(x+20, y+20);
					memdc.MoveTo(x-20, y+20); memdc.LineTo(x+20, y-20);
				}
				else
				{
					memdc.MoveTo(x, y-20); memdc.LineTo(x, y+20);
					memdc.MoveTo(x-20, y); memdc.LineTo(x+20, y);
				}
			}
			memdc.SelectObject(oldpen);
			memdc.SetROP2(drawmode);
			
		}
		else
		{
			pDoc->m_cPCB_Sheet.Draw(&memdc, this);

			DrawAllPCBObject(&memdc);

			DrawPcbDrcErrors(&memdc);

			DrawPcbConnections(&memdc);

			if(m_bPcbViewHighlightFlag == TRUE)
			{
				DrawMask(&memdc);
				DrawPcbHighlight(&memdc, 0);
			}
			else DrawPcbHighlight(&memdc, 1);

			DrawPcbOrigin(&memdc);

			int drawmode = memdc.GetROP2();
			memdc.SetROP2(R2_NOTXORPEN);
			
			CPen apen;
			apen.CreatePen(PS_SOLID, 1, RGB(0, 0 ,0));
			CPen* oldpen = memdc.SelectObject(&apen);

			if(( m_fpPcbHotPoint2.fx != 0) && ( m_fpPcbHotPoint2.fy != 0))
			{
				int x,y;
				Get_VPort_XY(&x, &y, m_fpPcbHotPoint2.fx, m_fpPcbHotPoint2.fy);
				memdc.Arc(x-15, y+15, x+15, y-15, x-15,y,x-15,y);
			}
			memdc.SelectObject(oldpen);
			memdc.SetROP2(drawmode);

			for(int i=0; i<tempPoint.GetCount(); i++)
			{
				Fpoint point = tempPoint.GetAt(i);
				int x,y;
				Get_VPort_XY(&x, &y, point.fx, point.fy);
				//memdc.SelectStockObject(WHITE_PEN);
				DrawSelectionKeyPoint(x, y, &memdc);
			}
		}
		
		if((nEDA_System_State == PCB)&&(m_bPcbFlipView == TRUE)) pDC->StretchBlt( m_nViewport_Ex, 0, -m_nViewport_Ex, m_nViewport_Ey,  (CDC*)&memdc, 0 , 0, m_nViewport_Ex, m_nViewport_Ey, SRCCOPY);
		else pDC->BitBlt(0,0, m_nViewport_Ex, m_nViewport_Ey, (CDC*)&memdc,0 , 0, SRCCOPY);
		memdc.SelectObject(pOldBmp); 
		bmpFace.DeleteObject();

		if(m_bCursorState == TRUE)
		{
			DrawMyCursor((CClientDC*)pDC,&m_CursorLocation);
		}
	}
	else 
	{
		double scalebak, screenxbak, screenybak;

		int xOrg = 0;

		CESDApp* pApp = (CESDApp*)AfxGetApp();
		int ypaper_length = pApp->GetPrintInfo(4);

		int yOrg = ypaper_length;
		pDC->SetViewportOrg(xOrg, yOrg);

		int dx = nEDA_Print_X_Pixel;
		int dy = nEDA_Print_Y_Pixel;

		scalebak = m_fScale;
		m_fScale = 1.0;
		
		screenxbak = m_fScreenX;
		screenybak = m_fScreenY;
		m_fScreenX = m_fprint_orgx;
		m_fScreenY = m_fprint_orgy;

		if(m_nSystem_State == SCH)
		{
			pDoc->m_pSCH_Sheet[m_nDiagram]->Draw(pDC, this);
			DrawAllSCHObject(pDC);
		}
		else 
		{
			pDoc->m_cPCB_Sheet.Draw(pDC, this);
			DrawAllPCBObject(pDC);
		}
		
		m_fScale = scalebak;
		m_fScreenX = screenxbak;
		m_fScreenY = screenybak;
	}
}


void CESDView::OnFilePrintPreview()
{
	if(nEDA_Print_Flag == 1) 
	{
		AfxMessageBox("前一次打印或打印预览未完成, 请先终止先前的打印或打印预览。");
		return;
	}

	if(nEDA_Sheet_State == 1)
	{
		CChildFrame* pChildFrame = (CChildFrame*)GetParentFrame();
		pChildFrame->m_wndSheet.HideSheet();
		pChildFrame->m_wndSheet.Invalidate();
	}

	// must not create this on the frame.  Must outlive this function
	CPrintPreviewState* pState = new CPrintPreviewState;

	// DoPrintPreview's return value does not necessarily indicate that
	// Print preview succeeded or failed, but rather what actions are necessary
	// at this point.  If DoPrintPreview returns TRUE, it means that
	// OnEndPrintPreview will be (or has already been) called and the
	// pState structure will be/has been deleted.
	// If DoPrintPreview returns FALSE, it means that OnEndPrintPreview
	// WILL NOT be called and that cleanup, including deleting pState
	// must be done here.
	if ( !DoPrintPreview( XTP_IDD_PREVIEW_DIALOGBAR, this,
		RUNTIME_CLASS( CXTPPreviewView ), pState ))
	{
		// In derived classes, reverse special window handling here for
		// Preview failure case
		TRACE0( "Error: DoPrintPreview failed.\n" );
		AfxMessageBox( AFX_IDP_COMMAND_FAILURE );
		delete pState;      // preview failed to initialize, delete State now

	}
}

void CESDView::FilePrint(void)
{
	CESDApp* pApp = (CESDApp*)AfxGetApp();
	if(pApp->GetPrintInfo(1) == 0)  return;
	
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if( (m_nSystem_State == PCB) || (nEDA_Print_SheetMode == 0) ) OnFilePrint();
	else
	{
		int k = m_nDiagram;
		for(int j = 0; j<SCH_SHEET_NUM; j++)
		{
			m_nDiagram = j;
			if(pDoc->m_bSCH_SheetShow[j] == TRUE)
			{
				if(pDoc->Get_SCH_Elements_Param(0, m_nDiagram, 0) == 0)
				{
					CString cstring;
					cstring = "要打印的图页:[";
					cstring += pDoc->m_csSCH_SheetName[m_nDiagram];
					cstring += "]除了图框以外没有设计内容, 继续打印?";

					if(AfxMessageBox(cstring, MB_YESNO|MB_ICONWARNING) != IDYES) continue;

				}

				OnFilePrint();
			}
		}
		m_nDiagram = k;
	}
}

BOOL CESDView::OnPreparePrinting(CPrintInfo* pInfo)
{
	if(nEDA_Print_Flag == 1) 
	{
		AfxMessageBox("前一次打印或打印预览未完成, 请先终止先前的打印或打印预览。");
		return FALSE;
	}

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CESDApp* tApp = (CESDApp*)AfxGetApp();

	nEDA_Print_Xdpi = tApp->GetPrintInfo(1);
	nEDA_Print_Ydpi = tApp->GetPrintInfo(2);
	nEDA_Print_X_Pixel = tApp->GetPrintInfo(5);
	nEDA_Print_Y_Pixel = tApp->GetPrintInfo(6);


	if((nEDA_Print_X_Pixel <= 0)||(nEDA_Print_Y_Pixel <= 0)|| (nEDA_Print_Xdpi <= 0) ||(nEDA_Print_Ydpi<= 0))
	{
		AfxMessageBox("无法识别打印设备参数, 打印失败。", MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	if(m_nSystem_State == SCH)
	{
		if(nEDA_Print_Extent == 0)
		{
			fEDA_Print_Minx = 0;
			fEDA_Print_Miny = 0;

			
			if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_bCustom == TRUE)
			{
				fEDA_Print_Maxx = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth;
				fEDA_Print_Maxy = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight;
			}
			else
			{
				fEDA_Print_Maxx = sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].width;
				fEDA_Print_Maxy = sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;
			}
		}
		else
		{
			fEDA_Print_Minx = m_fScreenX;
			fEDA_Print_Miny = m_fScreenY;
			fEDA_Print_Maxx = m_fScreenX + m_nViewport_Ex*m_fScale;
			fEDA_Print_Maxy = m_fScreenY + m_nViewport_Ey*m_fScale;
		}
	}
	else
	{
		if(nEDA_PCBPrint_Extent == 0)
		{
			fEDA_Print_Minx = pDoc->m_cPCB_Sheet.m_fSheetX;
			fEDA_Print_Miny = pDoc->m_cPCB_Sheet.m_fSheetY;
			fEDA_Print_Maxx = pDoc->m_cPCB_Sheet.m_fSheetX + pDoc->m_cPCB_Sheet.m_fSheetWidth;
			fEDA_Print_Maxy = pDoc->m_cPCB_Sheet.m_fSheetY + pDoc->m_cPCB_Sheet.m_fSheetHeight;
		}
		else if(nEDA_PCBPrint_Extent == 1)
		{
			double fx1, fy1, fx2, fy2;
			pDoc->m_cPCB_Sheet.GetBoardRect(&fx1, &fy1, &fx2, &fy2);
			fEDA_Print_Minx = fx1;
			fEDA_Print_Miny = fy1;
			fEDA_Print_Maxx = fx2;
			fEDA_Print_Maxy = fy2;
		}
		else
		{
			fEDA_Print_Minx = pDoc->m_fPCBPrintX1;
			fEDA_Print_Miny = pDoc->m_fPCBPrintY1;
			fEDA_Print_Maxx = pDoc->m_fPCBPrintX2;
			fEDA_Print_Maxy = pDoc->m_fPCBPrintY2;
		}

	}

	UINT papernum;
	if(nEDA_System_State == SCH)
	{
		if(nEDA_Print_Mode == 1)
		{
			int paperwidth = 1000*nEDA_Print_X_Pixel/nEDA_Print_Xdpi;
			int paperheight = 1000*nEDA_Print_Y_Pixel/nEDA_Print_Ydpi;

			double scale_x = paperwidth/(fEDA_Print_Maxx-fEDA_Print_Minx);
			double scale_y = paperheight/(fEDA_Print_Maxy-fEDA_Print_Miny);

			fEDA_Print_Scale = MIN(scale_x,scale_y) - 0.01; //加入一点小的修正，改善打印边界太满的状况
			
			m_nPrint_X_Papernum = 1;
			m_nPrint_Y_Papernum = 1;
			papernum = 1;
		}
		else
		{

			m_nPrint_X_Papernum = (int)ceil(((fEDA_Print_Maxx-fEDA_Print_Minx)*nEDA_Print_Xdpi/1000*fEDA_Print_Scale)/nEDA_Print_X_Pixel); 
			m_nPrint_Y_Papernum = (int)ceil(((fEDA_Print_Maxy-fEDA_Print_Miny)*nEDA_Print_Ydpi/1000*fEDA_Print_Scale)/nEDA_Print_Y_Pixel); 
		
			papernum = (UINT)m_nPrint_X_Papernum*(UINT)m_nPrint_Y_Papernum;
		}
	}
	else
	{
		if(nEDA_PCBPrint_Mode == 1)
		{
			int paperwidth = 1000*nEDA_Print_X_Pixel/nEDA_Print_Xdpi;
			int paperheight = 1000*nEDA_Print_Y_Pixel/nEDA_Print_Ydpi;

			double scale_x = paperwidth/(fEDA_Print_Maxx-fEDA_Print_Minx);
			double scale_y = paperheight/(fEDA_Print_Maxy-fEDA_Print_Miny);

			fEDA_PCBPrint_Scale = MIN(scale_x,scale_y) - 0.01;
			
			m_nPrint_X_Papernum = 1;
			m_nPrint_Y_Papernum = 1;
			papernum = 1;
		}
		else
		{

			m_nPrint_X_Papernum = (int)ceil(((fEDA_Print_Maxx-fEDA_Print_Minx)*nEDA_Print_Xdpi/1000*fEDA_PCBPrint_Scale)/nEDA_Print_X_Pixel); 
			m_nPrint_Y_Papernum = (int)ceil(((fEDA_Print_Maxy-fEDA_Print_Miny)*nEDA_Print_Ydpi/1000*fEDA_PCBPrint_Scale)/nEDA_Print_Y_Pixel); 
		
			papernum = (UINT)m_nPrint_X_Papernum*(UINT)m_nPrint_Y_Papernum;
		}
	}

	pInfo->SetMaxPage(papernum);

	int i = DoPreparePrinting(pInfo);

	pInfo->m_nCurPage = 1;
	
	if(pInfo->m_bPreview == TRUE)
	{
		// define the string of page printing
		char c1[100] = "打印预览: %u 页";
		char c2[100];
		char c3[100] = "\n正在打印: %u-%u 页";
		char c5[100] = "\n";
		char c[200];
		sprintf(c2,"(总计 %d 页)",papernum);
		sprintf(c,"%s%s%s%s%s",c1,c2,c3,c2,c5);
		pInfo->m_strPageDesc = c;
	}

	return i;
	
	// default CScrollView preparation
	//return CScrollView::OnPreparePrinting(pInfo);
}

void CESDView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_nPrint_Flag = 1;
	nEDA_Print_Flag = 1;

	// Default CScrollView begin printing.
	CScrollView::OnBeginPrinting(pDC, pInfo);
}

void CESDView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	if(pDC->IsPrinting())
	{
		if(nEDA_System_State == SCH)
		{
			int currentpage = pInfo->m_nCurPage;
			int x,y,z;
			x=0;

			y = m_nPrint_Y_Papernum-1;  
			z = currentpage;
			for(;;)
			{
				if(z <= m_nPrint_X_Papernum) 
				{
					x = z-1;
					break;
				}
				else
				{
					//y++;
					y--;
					z -= m_nPrint_X_Papernum;
				}
			}
			
			double x_offset, y_offset;
			if(bEDA_Print_HCenter == TRUE)
			{
				x_offset = ( nEDA_Print_X_Pixel*m_nPrint_X_Papernum - (fEDA_Print_Maxx-fEDA_Print_Minx)*nEDA_Print_Xdpi/1000*fEDA_Print_Scale  )/2;
				x_offset = (x_offset/nEDA_Print_Xdpi)*1000;
			
			}
			else x_offset = fEDA_Print_Margin_X;

			if(bEDA_Print_VCenter == TRUE)
			{
				y_offset = ( nEDA_Print_Y_Pixel*m_nPrint_Y_Papernum - (fEDA_Print_Maxy-fEDA_Print_Miny)*nEDA_Print_Ydpi/1000*fEDA_Print_Scale  )/2;
				y_offset = (y_offset/nEDA_Print_Ydpi)*1000;
			}
			else  y_offset = fEDA_Print_Margin_Y;

			m_fprint_orgx = fEDA_Print_Minx + x*(double)nEDA_Print_X_Pixel/nEDA_Print_Xdpi*1000/fEDA_Print_Scale - x_offset/fEDA_Print_Scale;
			m_fprint_orgy = fEDA_Print_Miny + y*(double)nEDA_Print_Y_Pixel/nEDA_Print_Ydpi*1000/fEDA_Print_Scale - y_offset/fEDA_Print_Scale;
		}
		else
		{
			int currentpage = pInfo->m_nCurPage;
			int x,y,z;
			x=0;

			y = m_nPrint_Y_Papernum-1; 
			z = currentpage;
			for(;;)
			{
				if(z <= m_nPrint_X_Papernum) 
				{
					x = z-1;
					break;
				}
				else
				{
					y--;
					z -= m_nPrint_X_Papernum;
				}
			}
			
			double x_offset, y_offset;
			if(bEDA_PCBPrint_HCenter == TRUE)
			{
				x_offset = ( nEDA_Print_X_Pixel*m_nPrint_X_Papernum - (fEDA_Print_Maxx-fEDA_Print_Minx)*nEDA_Print_Xdpi/1000*fEDA_PCBPrint_Scale  )/2;
				x_offset = (x_offset/nEDA_Print_Xdpi)*1000;
			
			}
			else x_offset = fEDA_PCBPrint_Margin_X;

			if(bEDA_PCBPrint_VCenter == TRUE)
			{
				y_offset = ( nEDA_Print_Y_Pixel*m_nPrint_Y_Papernum - (fEDA_Print_Maxy-fEDA_Print_Miny)*nEDA_Print_Ydpi/1000*fEDA_PCBPrint_Scale  )/2;
				y_offset = (y_offset/nEDA_Print_Ydpi)*1000;
			}
			else  y_offset = fEDA_PCBPrint_Margin_Y;

			m_fprint_orgx = fEDA_Print_Minx + x*(double)nEDA_Print_X_Pixel/nEDA_Print_Xdpi*1000/fEDA_PCBPrint_Scale - x_offset/fEDA_PCBPrint_Scale;
			m_fprint_orgy = fEDA_Print_Miny + y*(double)nEDA_Print_Y_Pixel/nEDA_Print_Ydpi*1000/fEDA_PCBPrint_Scale - y_offset/fEDA_PCBPrint_Scale;
		}
	}

	CScrollView::OnPrepareDC(pDC, pInfo);
}

void CESDView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 在此添加专用代码和/或调用基类

	CScrollView::OnPrint(pDC, pInfo);
}

void CESDView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CScrollView end printing
	CScrollView::OnEndPrinting(pDC, pInfo);
	
	m_nPrint_Flag = 0;
	nEDA_Print_Flag = 0;	
	
	if(nEDA_Sheet_State == 1)
	{
		nEDA_Sheet_State = 0;
		CChildFrame* pChildFrame = (CChildFrame*)GetParentFrame();
		CMainFrame* pMainFrame = (CMainFrame*)pChildFrame->GetParentFrame();
		::SetTimer(pMainFrame->m_hWnd, ID_SHEET_TAB_REFRESH, 200, NULL);
	}

	if(m_nSystem_State != nEDA_System_State) m_nSystem_State = nEDA_System_State;

	Invalidate();
}

void CESDView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView)
{
	// TODO: 在此添加专用代码和/或调用基类
	CScrollView::OnEndPrintPreview(pDC, pInfo, point, pView);
}


#ifdef _DEBUG
void CESDView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CESDView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CESDDoc* CESDView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CESDDoc)));
	return (CESDDoc*)m_pDocument;
}
#endif //_DEBUG



void CESDView::OnEditFind(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
	{
		CString strFind = pControl->GetEditText();

		TRACE1("%s\n",strFind);

		*pResult = 1; // Handled;
	}
}

void CESDView::OnUpdateEditFind(CCmdUI* pCmd)
{
	TRACE("===================\n");
	pCmd->Enable(TRUE);
}

BOOL CESDView::PreTranslateMessage(MSG* pMsg)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if( (pMsg->message == WM_MBUTTONDOWN)||(pMsg->message == WM_MBUTTONUP)||(pMsg->message == WM_MBUTTONDBLCLK) )  return TRUE;

	if(pMsg->message == WM_KEYDOWN)
	{
		int vkey = (TCHAR)pMsg->wParam;    // VKey code 
		//TRACE1("%d\n", (TCHAR)pMsg->wParam);

		if(vkey == VK_NEXT) 	OnViewZoom_Out();
		else if(vkey == VK_PRIOR) OnViewZoom_In();
		else if(vkey == VK_DELETE)
		{
			if(m_nSystem_State == SCH)	DeleteSelection();
			else PcbDeleteSelection();
		}
		else if(vkey == VK_SPACE)
		{
			if(m_nSystem_State == SCH) 
			{
				pDoc->m_fpSelection[m_nDiagram] = GetSelectionCenter();
				MoveAndRotateSelection( pDoc->m_fpSelection[m_nDiagram].fx, pDoc->m_fpSelection[m_nDiagram].fy, 0, 0, 90, FALSE);
			}
			else 
			{
				pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
				PcbMoveAndRotateSelection( pDoc->m_fpPcbSelection.fx, pDoc->m_fpPcbSelection.fy, 0, 0, 90, FALSE);
			}
		}
		else if(vkey == VK_F3)
		{
			if(m_nSystem_State == SCH) OnEditFindNext();
			else
			{
			}
			return TRUE;
		}
		else if(vkey == 'F')
		{
			if(m_nSystem_State == SCH)
			{
				if((GetKeyState(VK_CONTROL)&0x80) != 0 )
				{
					OnEditFindText();
					return TRUE;
				}
			}
			else
			{
			}
		}
		else if(vkey == 'H')
		{
			if(m_nSystem_State == SCH)
			{
				if((GetKeyState(VK_CONTROL)&0x80) != 0 )
				{
					OnEditReplaceText();
					return TRUE;
				}
			}
			else
			{
			}
		}
		else if(vkey == 'L')
		{
			CMenu Menu;
			CPoint pt;
			GetCursorPos(&pt);
			if(m_nSystem_State == SCH)
			{
				if(Menu.LoadMenu(IDR_SCH_L) == TRUE)
				{
					CMenu* pSubMenu = Menu.GetSubMenu(0);
					if(pSubMenu != NULL)
					{
						ClipCursor(NULL); m_bClipState = FALSE;
						pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
					}
				}
			}
			else
			{
				OnPcbLayerSetup();
			}
			return TRUE;
		}
	}

	return CScrollView::PreTranslateMessage(pMsg);
}

void CESDView::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == ID_PICKUP_TIMER)
	{
		bEDA_MouseLButtonMoveEnable = TRUE;
		KillTimer(ID_PICKUP_TIMER);
	}
	else if(nIDEvent == ID_SELECTION_TIMER)
	{
		KillTimer(ID_SELECTION_TIMER);
		
		Get_Current_XY(m_nMouse_X, m_nMouse_Y);
		Struct_Pcb_Multi_Selection pcb_multi_selection;
		if(bEDA_PcbSelectionFirstEnable == TRUE)	pcb_multi_selection = SelectPcbObject(m_fCurrentX, m_fCurrentY, THE_FIRST_ONE);
		else pcb_multi_selection = SelectPcbObject(m_fCurrentX, m_fCurrentY, INCLUDE_CPAD);

		if((pcb_multi_selection.pcbelement > 0)&&(pcb_multi_selection.index >= 0))
		{
			if((GetKeyState(VK_SHIFT)&0x80) == 0) PcbSelectAll(FALSE);
			PcbSelectElement(pcb_multi_selection.pcbelement, pcb_multi_selection.index);
			Invalidate();
		}
		else 
		{
			CESDDoc* pDoc = (CESDDoc*)GetDocument();
			if(pDoc->m_arrayPcbSelection.GetCount() > 0)
			{
				if((GetKeyState(VK_SHIFT)&0x80) == 0)
				{
					PcbSelectAll(FALSE);
					Invalidate();
				}
			}
		}
	}

	CScrollView::OnTimer(nIDEvent);
}

void CESDView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(nChar == '+')
	{
		if(m_nSystem_State == SCH) Plus_Sheet();
		else Plus_Layer();
	}
	else if( nChar == '-' )
	{
		if(m_nSystem_State == SCH) Minus_Sheet();
		else Minus_Layer();
	}
	else if((nChar == 'a')||(nChar == 'A'))
	{
		CMenu Menu;
		CPoint pt;
		GetCursorPos(&pt);
		if((m_nSystem_State==SCH)&&(Menu.LoadMenu(IDR_SCH_A)))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(0);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL); m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}
	}
	else if((nChar == 'c')||(nChar == 'C'))
	{
		CMenu Menu;
		CPoint pt;
		GetCursorPos(&pt);
		if((m_nSystem_State==SCH)&&(Menu.LoadMenu(IDR_SCH_C)))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(0);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL); m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}
	}
	else if((nChar == 'e')||(nChar == 'E'))
	{
		CMenu Menu;
		CPoint pt;
		GetCursorPos(&pt);
		if((m_nSystem_State==SCH)&&(Menu.LoadMenu(IDR_SCH_E)))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(0);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL); m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}
	}
	else if((nChar == 'g')||(nChar == 'G'))
	{
		if(m_nSystem_State==SCH)
		{
			if(pDoc->m_nSCH_Unit%2 == 0)
			{
				//英制
				if(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_I[0])
				{
					pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_I[0];
				}
				else if(pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_I[2])
				{
					pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_I[0];
				}
				else if((pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_I[0])&&(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_I[1])) pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_I[1];
				else if((pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_I[1])&&(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_I[2])) pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_I[2];
			}
			else
			{
				//公制
				if(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_M[0])
				{
					pDoc->m_fSCH_SnapGrid =fEDA_SCH_SnapGrid_M[0];
				}
				else if(pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_M[2])
				{
					pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_M[0];
				}
				else if((pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_M[0])&&(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_M[1])) pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_M[1];
				else if((pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_M[1])&&(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_M[2])) pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_M[2];
			}

			CString str = pDoc->Convert_XY_To_String(pDoc->m_fSCH_SnapGrid);
			char string[100];
			strcpy(string, "S:");
			strcat(string, str);
			CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			pFrame->Set_Snap(string);
		}
	}
	else if((nChar == 'j')||(nChar == 'J'))
	{
		CMenu Menu;
		CPoint pt;
		GetCursorPos(&pt);
		if((m_nSystem_State==SCH)&&(Menu.LoadMenu(IDR_SCH_J)))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(0);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL); m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}
	}
	else if((nChar == 'm')||(nChar == 'M'))
	{
		CMenu Menu;
		CPoint pt;
		GetCursorPos(&pt);
		if((m_nSystem_State==SCH)&&(Menu.LoadMenu(IDR_SCH_M)))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(0);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL); m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}
	}
	else if((nChar == 'o')||(nChar == 'O'))
	{
		if(m_nSystem_State==SCH) 
		{
			OnDesignOption();
		}
	}
	else if((nChar == 'p')||(nChar == 'P'))
	{
		CMenu Menu;
		CPoint pt;
		GetCursorPos(&pt);
		if((m_nSystem_State==SCH)&&(Menu.LoadMenu(IDR_SCH_MAINMENU)))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(3);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL); m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}
	}
	else if((nChar == 'r')||(nChar == 'R'))
	{
		if(m_nSystem_State==SCH) OnEditRepeat();
	}
	else if((nChar == 's')||(nChar == 'S'))
	{
		CMenu Menu;
		CPoint pt;
		GetCursorPos(&pt);
		if((m_nSystem_State == SCH)&&(Menu.LoadMenu(IDR_SCH_S)))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(0);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL); m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}
		else if((m_nSystem_State == PCB)&&(Menu.LoadMenu(IDR_PCB_S)))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(0);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL); m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}
	}
	else if((nChar == 'u')||(nChar == 'U'))
	{
		if(m_nSystem_State==SCH)
		{
			char unitstring[100];					
			if(pDoc->m_nSCH_Unit%2 == 0)
			{
				pDoc->m_nSCH_Unit = 1;
				strcpy(unitstring, "公制");
			}
			else 
			{
				pDoc->m_nSCH_Unit = 0;
				strcpy(unitstring, "英制");
			}

			CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			pFrame->Set_Unit(unitstring);
			pFrame->m_wndStatusBar.Invalidate();
		}
	}
	else if((nChar == 'x')||(nChar == 'X'))
	{
		CMenu Menu;
		CPoint pt;
		GetCursorPos(&pt);
		if((m_nSystem_State==SCH)&&(Menu.LoadMenu(IDR_SCH_X)))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(0);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL); m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}
		else if((m_nSystem_State == PCB)&&(Menu.LoadMenu(IDR_PCB_X)))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(0);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL); m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}
	}
	else if((nChar == 'z')||(nChar == 'Z'))
	{
		CMenu Menu;
		CPoint pt;
		GetCursorPos(&pt);
		if((m_nSystem_State==SCH)&&(Menu.LoadMenu(IDR_SCH_Z)))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(0);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL); m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}
		if((m_nSystem_State == PCB)&&(Menu.LoadMenu(IDR_PCB_Z)))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(0);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL); m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}
	}
	else if(nChar == 'v')
	{
		OnEditJumpLocation();
	}


	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}

void CESDView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//TRACE("L Button Down\n\n");
	bEDA_MouseLButtonDown = TRUE;
	if(m_nSystem_State == SCH)
	{
		if(m_nWhatIsUnderMouse == SOMETHING_SELECTION_MOVE)
		{
			if(bEDA_MouseMoveElement == FALSE)
			{
				bEDA_MouseMoveElement = TRUE;
				MoveSelection();
				bEDA_MouseMoveElement = FALSE;
			}
		}
		else if(m_nWhatIsUnderMouse == SOMETHING_SELECTION_COPY)
		{
			if(bEDA_MouseMoveElement == FALSE)
			{
				bEDA_MouseMoveElement = TRUE;
				OnEditDuplicate();
				bEDA_MouseMoveElement = FALSE;
			}
		}
		else if(m_nWhatIsUnderMouse == KEY_SPOT_ON_SELECTION)
		{
			if(bEDA_MouseMoveElement == FALSE)
			{
				bEDA_MouseMoveElement = TRUE;
				Get_Current_XY(point.x, point.y);
				AdvancedChangeMode(m_fCurrentX, m_fCurrentY);
				bEDA_MouseMoveElement = FALSE;
			}
		}
		else
		{
			nEDA_Pickup_X = point.x;
			nEDA_Pickup_Y = point.y;
			bEDA_MouseLButtonMoveEnable = FALSE;
			SetTimer(ID_PICKUP_TIMER, 150, NULL);
		}
	}
	else
	{
		if(m_nPcbWhatIsUnderMouse == SOMETHING_SELECTION_MOVE)
		{
			if(bEDA_MouseMoveElement == FALSE)
			{
				bEDA_MouseMoveElement = TRUE;
				PcbMoveSelection();
				bEDA_MouseMoveElement = FALSE;
			}
		}
		else if(m_nPcbWhatIsUnderMouse == SOMETHING_SELECTION_COPY)
		{
			if(bEDA_MouseMoveElement == FALSE)
			{
				bEDA_MouseMoveElement = TRUE;
				OnPcbEditDuplicate();
				bEDA_MouseMoveElement = FALSE;
			}
		}
		else if(m_nPcbWhatIsUnderMouse == KEY_SPOT_ON_SELECTION)
		{
			if(bEDA_MouseMoveElement == FALSE)
			{
				bEDA_MouseMoveElement = TRUE;
				Get_Current_XY(point.x, point.y);
				PcbAdvancedChangeMode(m_fCurrentX, m_fCurrentY);
				bEDA_MouseMoveElement = FALSE;
			}
		}
		else
		{
			nEDA_Pickup_X = point.x;
			nEDA_Pickup_Y = point.y;
			bEDA_MouseLButtonMoveEnable = FALSE;
			SetTimer(ID_PICKUP_TIMER, 150, NULL);
		}
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CESDView::OnLButtonUp(UINT nFlags, CPoint point)
{
	KillTimer(ID_PICKUP_TIMER);
	if(bEDA_MouseLButtonDown == TRUE)
	{
		bEDA_MouseLButtonDown = FALSE;
		int x = point.x; 
		int y = point.y;

		if(m_nSystem_State==SCH)
		{
			Get_Current_XY(x, y);
			Struct_Multi_Selection multi_selection = SelectSCHObject(m_fCurrentX, m_fCurrentY, THE_FIRST_ONE);
			if((multi_selection.element > 0)&&(multi_selection.pos != NULL))
			{
				if((GetKeyState(VK_SHIFT)&0x80) == 0) SelectAll(FALSE);
				SelectElement(multi_selection.element, multi_selection.pos);
				Invalidate();
			}
			else 
			{
				CESDDoc* pDoc = (CESDDoc*)GetDocument();
				if(pDoc->m_arraySelection[m_nDiagram].GetCount() > 0)
				{
					if((GetKeyState(VK_SHIFT)&0x80) == 0)
					{
						SelectAll(FALSE);
						Invalidate();
					}
				}
			}
		}
		else
		{
			SetTimer(ID_SELECTION_TIMER, 150, NULL);
			m_nMouse_X = x;
			m_nMouse_Y = y;
		}
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

void CESDView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if(bEDA_MouseRButtonPressedZooming == FALSE)
	{
		CMenu Menu;
		CPoint pt;
		pt.x = point.x;
		pt.y = point.y;
		ClientToScreen(&pt);
		if((m_nSystem_State==SCH)&&(Menu.LoadMenu(IDR_SCH_MAINMENU)))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(1);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL); m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}
		else if((m_nSystem_State == PCB) && Menu.LoadMenu(IDR_PCB_MAINMENU)  )
		{
			CMenu* pSubMenu = Menu.GetSubMenu(1);
			if(pSubMenu != NULL)
			{
				ClipCursor(NULL);  m_bClipState = FALSE;
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
			}
		}		
	}
	else
	{
		bEDA_MouseRButtonPressedZooming = FALSE;

		HCURSOR	hCur = ::LoadCursor(NULL,IDC_ARROW);
		SetCursor(hCur);
		
		ClipCursor(NULL); 
		m_bClipState = FALSE;
	}

	CScrollView::OnRButtonUp(nFlags, point);
}

void CESDView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	TRACE("============================== DBClick, pickup_edit\n");
	int x = point.x;
	int y = point.y;
				
	Get_Current_XY(x, y);
	if(m_nSystem_State == SCH)
	{
		Struct_Multi_Selection selection = SelectSCHObject(m_fCurrentX, m_fCurrentY, MULTI_SELECTION);
		EditElement(selection.element, selection.pos);
	}
	else
	{
		KillTimer(ID_SELECTION_TIMER);
		Struct_Pcb_Multi_Selection selection = SelectPcbObject(m_fCurrentX, m_fCurrentY, INCLUDE_CPAD);
		PcbEditElement(selection.pcbelement, selection.index);
	}

	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CESDView::OnMouseMove(UINT nFlags, CPoint point)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	static int mx, my;
	
	if(m_bActive == FALSE) return;

	if(nEDA_Set_Mouse_Pos == TRUE) 
	{
		nEDA_Set_Mouse_Pos = FALSE;
		return;
	}

	if((nFlags&MK_LBUTTON) != 0)
	{
		if(m_nSystem_State == SCH)
		{
			if((m_nWhatIsUnderMouse == NOTHING_SELECTION)&&(bEDA_MouseLButtonMoveEnable == TRUE))
			{
				bEDA_MouseLButtonMoveEnable = FALSE;

				Get_Current_XY(nEDA_Pickup_X, nEDA_Pickup_Y);
				Struct_Multi_Selection multi_selection = SelectSCHObject(m_fCurrentX, m_fCurrentY, MULTI_SELECTION);

				if((multi_selection.element > 0)&&(multi_selection.pos != NULL))
				{
					TRACE("============================== Move Object\n");
					PickupMoveOrCopy(multi_selection.element, multi_selection.pos);					
				}
				else
				{
					TRACE("============================== NoObject - select in area\n");
					if((GetKeyState(VK_SHIFT)&0x80) == 0) SelectAll(FALSE);
					MouseSelectInsideArea();
					Invalidate();
				}

			}
		}
		else
		{
			if((m_nPcbWhatIsUnderMouse == NOTHING_SELECTION)&&(bEDA_MouseLButtonMoveEnable == TRUE))
			{
				bEDA_MouseLButtonMoveEnable = FALSE;

				Get_Current_XY(nEDA_Pickup_X, nEDA_Pickup_Y);
				Struct_Pcb_Multi_Selection multi_selection = SelectPcbObject(m_fCurrentX, m_fCurrentY, NOT_INCLUDE_CPAD);

				if((multi_selection.pcbelement > 0)&&(multi_selection.index >= 0))
				{
					TRACE("==============================PCB Move Object\n");
					PcbPickupMoveOrCopy(multi_selection.pcbelement, multi_selection.index);					
				}
				else
				{
					TRACE("==============================PCB NoObject - select in area\n");
					if((GetKeyState(VK_SHIFT)&0x80) == 0) PcbSelectAll(FALSE);
					PcbMouseSelectInsideArea();
					Invalidate();
				}
			}
		}
	}
	else if((nFlags&MK_RBUTTON) != 0)
	{
		if(bEDA_MouseRButtonPressedZooming == FALSE)
		{
			bEDA_MouseRButtonPressedZooming = TRUE;
			mx = point.x;
			my = point.y;
			CESDApp* pApp = (CESDApp*)AfxGetApp();
			HCURSOR	hCur = pApp->LoadCursor(IDC_HANDZOOM);
			SetCursor(hCur);

			RECT rect;
			rect.left = 0; rect.top = 0;
			rect.right = m_nViewport_Ex; rect.bottom = m_nViewport_Ey;
			ClientToScreen(&rect);

			ClipCursor(&rect);
			m_bClipState = TRUE;
		}

		int d = (int)sqrt(((double)point.x - mx)*((double)point.x - mx) + ((double)point.y - my)*((double)point.y - my));
		if(d > 20)
		{
			m_fScreenX += (mx - point.x)*m_fScale;
			m_fScreenY += (point.y - my)*m_fScale;


			double Size_Maxx, Size_Maxy;
			if(m_nSystem_State == SCH)
			{
				if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_bCustom == FALSE)
				{
					if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0)  //landscape
					{
						Size_Maxx = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].width;
						Size_Maxy = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;
					}
					else
					{
						Size_Maxx = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;
						Size_Maxy = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].width;
					}
				}
				else
				{
					if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0)  //landscape
					{
						Size_Maxx = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth;
						Size_Maxy = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight;
					}
					else
					{
						Size_Maxx = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight;
						Size_Maxy = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth;
					}
				}
			}
			else
			{
				Size_Maxx = fEDA_PCB_SizeMaxx;
				Size_Maxy = fEDA_PCB_SizeMaxy;	
			}

			if((m_fScreenX + m_nViewport_Ex*m_fScale) > Size_Maxx) m_fScreenX = Size_Maxx - m_nViewport_Ex*m_fScale;
			if((m_fScreenY + m_nViewport_Ey*m_fScale) > Size_Maxy) m_fScreenY = Size_Maxy - m_nViewport_Ey*m_fScale;
			if(m_fScreenX < 0) m_fScreenX = 0;
			if(m_fScreenY < 0) m_fScreenY = 0;	

			ScrollBar_Control();
			Invalidate();
			mx = point.x;
			my = point.y;
		}
	}
	else
	{
		if(m_bClipState == FALSE)
		{
			if(m_nSystem_State == SCH)
			{
				int selection_num = pDoc->m_arraySelection[m_nDiagram].GetCount();
				if(selection_num <= 0)
				{
					HCURSOR	hCur = ::LoadCursor(NULL,IDC_ARROW);
					SetCursor(hCur);
					m_nWhatIsUnderMouse = NOTHING_SELECTION;
					Write_Info(szEDA_Info_Ready);
				}
				else
				{
					CESDApp* pApp = (CESDApp*)AfxGetApp();
					HCURSOR	hCur;	
					int index = FindSelectionKeyPoint(m_fCurrentX, m_fCurrentY);
					if((selection_num < 20)&&(index > 0))
					{
						hCur = pApp->LoadCursor(IDC_PICKUP);
						SetCursor(hCur);
						m_nWhatIsUnderMouse = KEY_SPOT_ON_SELECTION;
						if(index == 1)	Write_Info("<左键>移动连线端点");
						else if(index == 2) Write_Info("<左键>移动线段, <CTRL+左键>折断连线");
						else if(index == 3) Write_Info("<左键>修改终点");
						else if(index == 4) Write_Info("<左键>修改起点");
						else if(index == 5) Write_Info("<左键>修改半径");
						else if(index == 6) Write_Info("<左键>修改X半径");
						else if(index == 7) Write_Info("<左键>修改Y半径");
						else if(index == 8) Write_Info("<左键>修改圆角半径");
						else if(index == 9) Write_Info("<左键>修改尺寸");
						else if(index == 10) Write_Info("<左键>移动顶点");
					}
					else
					{
						Struct_Multi_Selection multi_selection = FindSelectedObject(m_fCurrentX, m_fCurrentY);
						if((multi_selection.element != 0)&&(multi_selection.pos != NULL)&&(selection_num > 1))
						{
							if((GetKeyState(VK_SHIFT)&0x80) == 0) 
							{
								hCur = pApp->LoadCursor(IDC_SELECTIONMOVE);
								Write_Info("<左键>移动所有选择的图形对象");
								m_nWhatIsUnderMouse = SOMETHING_SELECTION_MOVE;
							}
							else 
							{
								hCur = pApp->LoadCursor(IDC_SELECTIONCOPY);
								Write_Info("<左键>复制所有选择的图形对象");
								m_nWhatIsUnderMouse = SOMETHING_SELECTION_COPY;
							}
							SetCursor(hCur);
							
						}
						else
						{
							HCURSOR	hCur = ::LoadCursor(NULL,IDC_ARROW);
							SetCursor(hCur);
							m_nWhatIsUnderMouse = NOTHING_SELECTION;
							Write_Info(szEDA_Info_Ready);
						}
					}
				}
			}
			else
			{
				int selection_num = pDoc->m_arrayPcbSelection.GetCount();
				if(selection_num <= 0)
				{
					HCURSOR	hCur = ::LoadCursor(NULL,IDC_ARROW);
					SetCursor(hCur);
					m_nPcbWhatIsUnderMouse = NOTHING_SELECTION;
					Write_Info(szEDA_Info_Ready);
				}
				else
				{
					CESDApp* pApp = (CESDApp*)AfxGetApp();
					HCURSOR	hCur;	
					int index = PcbFindSelectionKeyPoint(m_fCurrentX, m_fCurrentY);
					if((selection_num < 20)&&(index > 0))
					{
						hCur = pApp->LoadCursor(IDC_PICKUP);
						SetCursor(hCur);
						m_nPcbWhatIsUnderMouse = KEY_SPOT_ON_SELECTION;
						if(index == 1)	Write_Info("<左键>移动连线端点");
						else if(index == 2) Write_Info("<左键>移动线段, <CTRL+左键>折断连线");
						else if(index == 3) Write_Info("<左键>修改终点");
						else if(index == 4) Write_Info("<左键>修改起点");
						else if(index == 5) Write_Info("<左键>修改半径");
						else if(index == 6) Write_Info("<左键>修改填充区顶点");
						else if(index == 7) Write_Info("<左键>修改填充区角度");
						else if(index == 8) Write_Info("<左键>修改文字角度");
						else if(index == 9) Write_Info("<左键>移动顶点");
						else if(index == 10) Write_Info("<左键>移动顶点");
					}
					else
					{
						Struct_Pcb_Multi_Selection multi_selection = PcbFindSelectedObject(m_fCurrentX, m_fCurrentY);	
						if((multi_selection.pcbelement > 0)&&(multi_selection.index >= 0)&&(selection_num > 1))
						{
							if((GetKeyState(VK_SHIFT)&0x80) == 0) 
							{
								hCur = pApp->LoadCursor(IDC_SELECTIONMOVE);
								Write_Info("<左键>移动所有选择的图形对象");
								m_nPcbWhatIsUnderMouse = SOMETHING_SELECTION_MOVE;
							}
							else 
							{
								hCur = pApp->LoadCursor(IDC_PCB_SELECTIONCOPY);
								Write_Info("<左键>复制所有选择的图形对象");
								m_nPcbWhatIsUnderMouse = SOMETHING_SELECTION_COPY;
							}
							SetCursor(hCur);
						}
						else
						{
							HCURSOR	hCur = ::LoadCursor(NULL,IDC_ARROW);
							SetCursor(hCur);
							m_nPcbWhatIsUnderMouse = NOTHING_SELECTION;
							Write_Info(szEDA_Info_Ready);
						}
					}
				}
			}
		}

		Get_Current_XY(point.x,point.y);
		Write_XY(m_fCurrentSnapX, m_fCurrentSnapY);
	}


	CScrollView::OnMouseMove(nFlags, point);
}

void CESDView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	double Size_Maxy;
	if(m_nSystem_State == SCH)
	{
		if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_bCustom == FALSE)
		{
			if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0) Size_Maxy = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;
			else Size_Maxy = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].width;
		}
		else
		{
			if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0) Size_Maxy = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight;
			else Size_Maxy = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth;
		}
	}
	else Size_Maxy = fEDA_PCB_SizeMaxy;

	// calc new y position
	int y, yOrig, y_limit;
	y = GetScrollPos(SB_VERT);
	yOrig = y;

	switch (nSBCode)
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = (int)Size_Maxy;
		break;
	case SB_LINEUP:
		y -= 10;
		break;
	case SB_LINEDOWN:
		y += 10;
		break;
	case SB_PAGEUP:
		y -= 100;
		break;
	case SB_PAGEDOWN:
		y += 100;
		break;
	case SB_THUMBPOSITION:
		return;
	case SB_THUMBTRACK:
		SCROLLINFO  info;
		GetScrollInfo(SB_VERT, &info);
		y = info.nTrackPos;
		if(y != yOrig)
		{
			y_limit = GetScrollLimit(SB_VERT);
			if(y < 0) y = 0;
			if(y > y_limit) y = y_limit;
			SetScrollPos(SB_VERT,y,TRUE);
			m_fScreenY = ( y_limit - y )*m_fScale;
			Screen_XYControl();
			Invalidate();
		}
		return;
	case SB_ENDSCROLL:
		return;
	}

	if(y != yOrig)
	{
		y_limit = GetScrollLimit(SB_VERT);
		if(y < 0) y = 0;
		if(y > y_limit) y = y_limit;
		SetScrollPos(SB_VERT,y,TRUE);
		m_fScreenY = (y_limit - y )*m_fScale;
		Screen_XYControl();
		Invalidate();
	}
	//CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CESDView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	double Size_Maxx;
	if(m_nSystem_State == SCH)
	{
		if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_bCustom == FALSE)
		{
			if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0) Size_Maxx = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].width;
			else Size_Maxx = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;
		}
		else
		{
			if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0)		Size_Maxx = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth;
			else	Size_Maxx = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight;
		}
	}

	else Size_Maxx = fEDA_PCB_SizeMaxx;

	int x, xOrig, x_limit;
	x = GetScrollPos(SB_HORZ);
	xOrig = x;

	switch (nSBCode)
	{
	case SB_LEFT:
		x = 0;
		break;
	case SB_RIGHT:
		x = (int)Size_Maxx;
		break;
	case SB_LINELEFT:
		x -= 10;
		break;
	case SB_LINERIGHT:
		x += 10;
		break;
	case SB_PAGELEFT:
		x -= 100;
		break;
	case SB_PAGERIGHT:
		x += 100;
		break;
	case SB_THUMBPOSITION:
		return;
	case SB_THUMBTRACK:
		SCROLLINFO  info;
		GetScrollInfo(SB_HORZ, &info);
		x = info.nTrackPos;
		if(x != xOrig)
		{
			x_limit = GetScrollLimit(SB_HORZ);
			if(x < 0) x = 0;
			if(x > x_limit) x = x_limit;
			m_fScreenX = x*m_fScale;
			Screen_XYControl();
			SetScrollPos(SB_HORZ,x,TRUE);
			Invalidate();
		}
		return;
	case SB_ENDSCROLL:
		return;
	}

	if(x != xOrig)
	{
		x_limit = GetScrollLimit(SB_HORZ);
		if(x < 0) x = 0;
		if(x > x_limit) x = x_limit;
		SetScrollPos(SB_HORZ,x,TRUE);
		m_fScreenX = x*m_fScale;
		Screen_XYControl();
		Invalidate();
	}

}

void CESDView::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CScrollView::OnSysChar(nChar, nRepCnt, nFlags);
}

void CESDView::OnSetFocus(CWnd* pOldWnd)
{
	CScrollView::OnSetFocus(pOldWnd);

	TRACE("SetFocus\n");
	m_bActive = TRUE;
	m_bAfterSetFocus = true;

	if( m_bClipState == TRUE)
	{
		RECT rect;
		rect.left = 0; rect.top = 0;
		rect.right = m_nViewport_Ex; rect.bottom = m_nViewport_Ey;
		ClientToScreen(&rect);

		ClipCursor(&rect);
	}

	Invalidate();
}

void CESDView::OnKillFocus(CWnd* pNewWnd)
{
	TRACE("KillFocus\n");
	m_bActive = FALSE;
	
	if( m_bClipState == TRUE) ClipCursor(NULL);

	bEDA_MouseRButtonPressedZooming = FALSE;

	CScrollView::OnKillFocus(pNewWnd);
}

void CESDView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	m_nViewport_Ex = cx;
	m_nViewport_Ey = cy;
}

BOOL CESDView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return TRUE;

	
	if( (nFlags&MK_SHIFT) != 0)
	{

		if(zDelta < 0)
		{
			double Size_Maxx;
			if(m_nSystem_State == SCH)
			{
				if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_bCustom == FALSE)
				{
					if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0) 	Size_Maxx = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].width;
					else 	Size_Maxx = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;
				}
				else
				{
					if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0) Size_Maxx = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth;
					else Size_Maxx = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight;
				}
			}
			else
			{
				Size_Maxx = fEDA_PCB_SizeMaxx;
			}

			m_fScreenX += (m_nViewport_Ey/10)*m_fScale;
			if((m_fScreenX + m_nViewport_Ex*m_fScale) > Size_Maxx) m_fScreenX = Size_Maxx - m_nViewport_Ex*m_fScale;
			if(m_fScreenX < 0) m_fScreenX = 0;
			ScrollBar_Control();
			Invalidate();

		}
		else
		{
			m_fScreenX -= (m_nViewport_Ex/10)*m_fScale;
			if(m_fScreenX < 0) m_fScreenX = 0;
			ScrollBar_Control();
			Invalidate();

		}	
	}
	else if(((nFlags&MK_CONTROL) != 0) || ((nFlags&MK_MBUTTON) != 0))
	{
		
		if(zDelta < 0)
		{
			if(m_fScale < fEDA_ScaleMax)
			{
				double fx, fy;
				fx = m_fScreenX + (m_nViewport_Ex/2)*m_fScale;
				fy = m_fScreenY + (m_nViewport_Ey/2)*m_fScale;

				m_fScale = m_fScale*1.2f;
				if(m_fScale > fEDA_ScaleMax) m_fScale = fEDA_ScaleMax;
	
				m_fScreenX = fx - (m_nViewport_Ex*m_fScale/2);
				m_fScreenY = fy - (m_nViewport_Ey*m_fScale/2);

				double Size_Maxx, Size_Maxy;
				if(m_nSystem_State == SCH)
				{
					if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_bCustom == FALSE)
					{
						if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0)  //landscape
						{
							Size_Maxx = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].width;
							Size_Maxy = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;
						}
						else
						{
							Size_Maxx = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;
							Size_Maxy = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].width;
						}
					}
					else
					{
						if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0)  //landscape
						{
							Size_Maxx = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth;
							Size_Maxy = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight;
						}
						else
						{
							Size_Maxx = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight;
							Size_Maxy = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth;
						}
					}
				}
				else
				{
					Size_Maxx = fEDA_PCB_SizeMaxx;
					Size_Maxy = fEDA_PCB_SizeMaxy;	
				}
				
				if(m_fScreenX > (Size_Maxx - m_nViewport_Ex*m_fScale)) m_fScreenX = Size_Maxx - m_nViewport_Ex*m_fScale;
				if(m_fScreenY > (Size_Maxy - m_nViewport_Ey*m_fScale)) m_fScreenY = Size_Maxy - m_nViewport_Ey*m_fScale;
				
				if(m_fScreenX < 0) m_fScreenX = 0;
				if(m_fScreenY < 0) m_fScreenY = 0;
				
				UpdateZoomSlider();
				ScrollBar_Control();
				Invalidate();
			}
		}
		else
		{
			double fscalemin;
			if(m_nSystem_State == SCH) fscalemin = fEDA_ScaleMin;
			else fscalemin = fEDA_ScaleMin/4;
			if(m_fScale > fscalemin)
			{
				double fx, fy;
				fx = m_fScreenX + (m_nViewport_Ex/2)*m_fScale;
				fy = m_fScreenY + (m_nViewport_Ey/2)*m_fScale;

				m_fScale = m_fScale/1.2f;
				if(m_fScale < fscalemin) m_fScale = fscalemin;

				m_fScreenX = fx - (m_nViewport_Ex*m_fScale/2);
				m_fScreenY = fy - (m_nViewport_Ey*m_fScale/2);

				
				double Size_Maxx, Size_Maxy;
				if(m_nSystem_State == SCH)
				{
					if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_bCustom == FALSE)
					{
						if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0)  //landscape
						{
							Size_Maxx = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].width;
							Size_Maxy = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;
						}
						else
						{
							Size_Maxx = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;
							Size_Maxy = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].width;
						}
					}
					else
					{
						if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0)  //landscape
						{
							Size_Maxx = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth;
							Size_Maxy = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight;
						}
						else
						{
							Size_Maxx = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight;
							Size_Maxy = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth;
						}
					}
				}
				else
				{
					Size_Maxx = fEDA_PCB_SizeMaxx;
					Size_Maxy = fEDA_PCB_SizeMaxy;	
				}
				
				if(m_fScreenX > (Size_Maxx - m_nViewport_Ex*m_fScale)) m_fScreenX = Size_Maxx - m_nViewport_Ex*m_fScale;
				if(m_fScreenY > (Size_Maxy - m_nViewport_Ey*m_fScale)) m_fScreenY = Size_Maxy - m_nViewport_Ey*m_fScale;
				
				if(m_fScreenX < 0) m_fScreenX = 0;
				if(m_fScreenY < 0) m_fScreenY = 0;

				UpdateZoomSlider();
				ScrollBar_Control();
				Invalidate();
			}			
		}	
	}
	else
	{
		if(zDelta < 0)
		{
			m_fScreenY -= (m_nViewport_Ey/10)*m_fScale;
			if(m_fScreenY < 0) m_fScreenY = 0;
			ScrollBar_Control();
			Invalidate();
		}
		else
		{
			double Size_Maxy;
			if(m_nSystem_State == SCH)
			{
				if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_bCustom == FALSE)
				{
					if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0)		Size_Maxy = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;
					else Size_Maxy = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].width;
				}
				else
				{
					if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0) Size_Maxy = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight;
					else 	Size_Maxy = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth;
				}
			}
			else 	Size_Maxy = fEDA_PCB_SizeMaxy;	

			m_fScreenY += (m_nViewport_Ey/10)*m_fScale;
			if((m_fScreenY + m_nViewport_Ey*m_fScale) > Size_Maxy) m_fScreenY = Size_Maxy - m_nViewport_Ey*m_fScale;
			if(m_fScreenY < 0) m_fScreenY = 0;
			ScrollBar_Control();
			Invalidate();
		}
	}
   
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CESDView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if((GetKeyState(VK_SHIFT)&0x80) == 0)  
	{
		if(nChar == 0x25)
		{
			double Snap_Grid;
			if(m_nSystem_State == SCH) Snap_Grid = pDoc->m_fSCH_SnapGrid;
			else Snap_Grid = pDoc->m_cPCB_Sheet.m_fSnapGridSize;

			if((m_fCurrentSnapX - Snap_Grid) >= m_fScreenX)
			{
				m_fCurrentSnapX -= Snap_Grid;
				Restore_LargeCrossXY();

			}
			else  Pan_Left();
		}
		if(nChar == 0x26)
		{
			double Snap_Grid;
			if(m_nSystem_State == SCH) Snap_Grid = pDoc->m_fSCH_SnapGrid;
			else Snap_Grid = pDoc->m_cPCB_Sheet.m_fSnapGridSize;
	
			double PSnap_MAXY;
			if(m_nSystem_State == SCH) PSnap_MAXY = fEDA_SCH_SnapMaxy;
			else PSnap_MAXY = fEDA_PCB_SnapMaxy;

			if(((m_fCurrentSnapY + Snap_Grid) <= (m_fScreenY + m_nViewport_Ey*m_fScale))&&\
				((m_fCurrentSnapY + Snap_Grid) <=	PSnap_MAXY))
			{
				m_fCurrentSnapY += Snap_Grid;
				Restore_LargeCrossXY();
			}
			else Pan_Up();
		}
		if(nChar == 0x27)
		{
			double Snap_Grid;
			if(m_nSystem_State == SCH) Snap_Grid = pDoc->m_fSCH_SnapGrid;
			else Snap_Grid = pDoc->m_cPCB_Sheet.m_fSnapGridSize;
			
			double PSnap_MAXX;
			if(m_nSystem_State == SCH) PSnap_MAXX = fEDA_SCH_SnapMaxx;
			else PSnap_MAXX = fEDA_PCB_SnapMaxx;

			if(((m_fCurrentSnapX +Snap_Grid) <= (m_fScreenX + m_nViewport_Ex*m_fScale))&&\
						((m_fCurrentSnapX + Snap_Grid) <= PSnap_MAXX))
			{
				m_fCurrentSnapX += Snap_Grid;
				Restore_LargeCrossXY();
			}
			else Pan_Right();
		}
		if(nChar == 0x28) 
		{
			double Snap_Grid;
			if(m_nSystem_State == SCH) Snap_Grid = pDoc->m_fSCH_SnapGrid;
			else Snap_Grid = pDoc->m_cPCB_Sheet.m_fSnapGridSize;

			if((m_fCurrentSnapY - Snap_Grid) >= m_fScreenY)
			{
				m_fCurrentSnapY -= Snap_Grid;
				Restore_LargeCrossXY();
			}
			else Pan_Down();
		}
	}
	else 
	{
		if(nChar == 0x25)
		{
			double Snap_Grid;
			if(m_nSystem_State == SCH) Snap_Grid = pDoc->m_fSCH_SnapGrid;
			else Snap_Grid = pDoc->m_cPCB_Sheet.m_fSnapGridSize;

			if((m_fCurrentSnapX - 2*Snap_Grid) >= m_fScreenX)
			{
				m_fCurrentSnapX -= 2*Snap_Grid;
				Restore_LargeCrossXY();

			}
			else  Pan_Left();
		}
		if(nChar == 0x26) 
		{
			double Snap_Grid;
			if(m_nSystem_State == SCH) Snap_Grid = pDoc->m_fSCH_SnapGrid;
			else Snap_Grid = pDoc->m_cPCB_Sheet.m_fSnapGridSize;
	
			double PSnap_MAXY;
			if(m_nSystem_State == SCH) PSnap_MAXY = fEDA_SCH_SnapMaxy;
			else PSnap_MAXY = fEDA_PCB_SnapMaxy;

			if(((m_fCurrentSnapY + 2*Snap_Grid) <= (m_fScreenY + m_nViewport_Ey*m_fScale))&&\
				((m_fCurrentSnapY + 2*Snap_Grid) <=	PSnap_MAXY))
			{
				m_fCurrentSnapY += 2*Snap_Grid;
				Restore_LargeCrossXY();
			}
			else Pan_Up();
		}
		if(nChar == 0x27) 
		{
			double Snap_Grid;
			if(m_nSystem_State == SCH) Snap_Grid = pDoc->m_fSCH_SnapGrid;
			else Snap_Grid = pDoc->m_cPCB_Sheet.m_fSnapGridSize;
			
			double PSnap_MAXX;
			if(m_nSystem_State == SCH) PSnap_MAXX = fEDA_SCH_SnapMaxx;
			else PSnap_MAXX = fEDA_PCB_SnapMaxx;

			if(((m_fCurrentSnapX + 2*Snap_Grid) <= (m_fScreenX + m_nViewport_Ex*m_fScale))&&\
						((m_fCurrentSnapX + 2*Snap_Grid) <= PSnap_MAXX))
			{
				m_fCurrentSnapX += 2*Snap_Grid;
				Restore_LargeCrossXY();
			}
			else Pan_Right();
		}
		if(nChar == 0x28) 
		{
			double Snap_Grid;
			if(m_nSystem_State == SCH) Snap_Grid = pDoc->m_fSCH_SnapGrid;
			else Snap_Grid = pDoc->m_cPCB_Sheet.m_fSnapGridSize;

			if((m_fCurrentSnapY - 2*Snap_Grid) >= m_fScreenY)
			{
				m_fCurrentSnapY -= 2*Snap_Grid;
				Restore_LargeCrossXY();
			}
			else Pan_Down();
		}
	}
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CESDView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CESDView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CScrollView::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
}


void CESDView::DrawStructButton(UINT nID, BOOL state)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->DrawToolButton(nID, state);
}

void CESDView::Write_Help(char * str)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->Set_Status_Help(str);	
}

void CESDView::Write_Info(char * str)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->Set_Status_Info(str);
}

void CESDView::Get_Rotate_XY(double* fx, double* fy, double old_fx, double old_fy, int orientation, BOOL mirrored)
{
	int state;
	state = (mirrored == TRUE? 1 : 0)*4 + orientation/90;
	*fx = fEDA_Rotate_XY[state][0]*old_fx + fEDA_Rotate_XY[state][1]*old_fy;
	*fy = fEDA_Rotate_XY[state][2]*old_fx + fEDA_Rotate_XY[state][3]*old_fy;
}

void CESDView::Get_Rotate_Angle(double* sangle, double* eangle, double old_sangle, double old_eangle, int orientation, BOOL mirrored)
{
	double sa, ea;

	if(mirrored == FALSE)
	{
		sa = old_sangle + orientation;
		if(sa >= 360) sa -= 360;
		ea = old_eangle + orientation;
		if(ea >= 360) ea -= 360;
	}
	else
	{
		sa = old_eangle + orientation;
		if(sa >= 360) sa -= 360;
		ea = old_sangle + orientation;
		if(ea >= 360) ea -= 360;
		sa = 180 - sa;
		if(sa < 0) sa += 360;
		ea = 180 - ea;
		if(ea < 0) ea += 360;
	}
	*sangle = sa;
	*eangle = ea;
}

void CESDView::size_to_unitstr(double fx, char *string, int format, int precision, int unit)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	int flag = 0;
    int ndig = 10;
    int dec,sign;
    char xx[100],*str;

	if( (unit == UNIT_MM) || ((unit == UNIT_AUTO)&&(pDoc->m_cPCB_Sheet.m_nUnit == UNIT_PCB_M)) ) fx = fx*25.4f/1000.0f;
	else if( unit == UNIT_CM ) fx = fx*25.4f/10000.0f;
	else if( unit == UNIT_INCH ) fx = fx/1000;
	else if( unit == UNIT_AUTO_I)
	{
		if(fx > 1000) 
		{
			fx = fx/1000; 
			flag = 1;
		}
	}
	else if( unit == UNIT_AUTO_M )
	{
		fx = fx*25.4f/1000.0f;
		if(fx > 10)  
		{
			fx = fx/10;
			flag = 1;
		}
	}


	str = _fcvt(fx, precision, &dec, &sign);

	if(strlen(str) > 0)
	{
		if(dec<=0)
		{
			if(dec == 0) strcpy(xx, "0.");
			else if(dec == -1) strcpy(xx, "0.0");
			else if(dec == -2) strcpy(xx, "0.00");
			else if(dec == -3) strcpy(xx, "0.000");
			else if(dec == -4) strcpy(xx, "0.0000");
			else if(dec == -5) strcpy(xx, "0.00000");
			else if(dec == -6) strcpy(xx, "0.000000");
			strcat(xx, str);
		}
		else
		{
			for(int i=0; i<dec; i++)
			{
				xx[i] = str[i];
				xx[i+1] = NULL;
			}
			if((int)strlen(str) > dec )
			{
				strcat(xx,".");
				strcat(xx, str+dec);
			}
		}
	}
	else strcpy(xx, NULL);

	if(format == 0) string[0] = NULL;
	else if(format == 1)
	{
		strcpy(string, xx);
	}
	else if(format == 2)
	{
		if(unit == UNIT_MIL)
		{
			strcat(xx, "mil");
			strcpy(string ,xx);
		}
		else if(unit == UNIT_MM)
		{
			strcat(xx, "mm");
			strcpy(string ,xx);
		}
		else if(unit == UNIT_INCH)
		{
			strcat(xx, "in");
			strcpy(string ,xx);
		}
		else if(unit == UNIT_CM)
		{
			strcat(xx, "cm");
			strcpy(string ,xx);
		}
		else if(unit == UNIT_AUTO_I)
		{
			if(flag == 1)  // UNIT_AUTO_I,  INCH
			{
				strcat(xx, "in");
				strcpy(string ,xx);
			}
			else
			{
				strcat(xx, "mil");
				strcpy(string ,xx);
			}
		}
		else if(unit == UNIT_AUTO)
		{
			if(pDoc->m_cPCB_Sheet.m_nUnit == UNIT_PCB_I) strcat(xx, "mil");
			else strcat(xx, "mm");
			strcpy(string ,xx);
		}
	}
	else
	{
		if(unit == UNIT_MIL)
		{
			strcat(xx, " (mil)");
			strcpy(string ,xx);
		}
		else if(unit == UNIT_MM)
		{
			strcat(xx, " (mm)");
			strcpy(string ,xx);
		}
		else if(unit == UNIT_INCH)
		{
			strcat(xx, " (in)");
			strcpy(string ,xx);
		}
		else if(unit == UNIT_CM)
		{
			strcat(xx, " (cm)");
			strcpy(string ,xx);
		}
		else if(unit == UNIT_AUTO_M)
		{
			if(flag == 1)  // UNIT_AUTO_M,  CM
			{
				strcat(xx, " (cm)");
				strcpy(string ,xx);
			}
			else
			{
				strcat(xx, " (mm)");
				strcpy(string ,xx);
			}
		}
		else if(unit == UNIT_AUTO)
		{
			if(pDoc->m_cPCB_Sheet.m_nUnit == UNIT_PCB_I) strcat(xx, " (mil)");
			else strcat(xx, " (mm)");
			strcpy(string ,xx);
		}
	}
}

void CESDView::mm_to_str(double fx, char *string)
{
    int ndig = 3;
    int dec,sign;
    char xx[100],*str;

    fx = fx*25.4f/1000.0f;

	int ih, il; 
	double fh, fl;
	ih = (int)(fx*1000);
	fh = (double)ih;
	fl = fx*1000 - fh;
	il = (int)(fl*10);
	if(il >= 5) fh++;
	fx = fh/1000;

	str = _fcvt(fx,ndig,&dec,&sign);

		 if(dec==-3){ xx[0]='0';	xx[1]='.';	  xx[2]='0';	xx[3]='0';	  xx[4]='0';    xx[5]='\0';}
    else if(dec==-2){ xx[0]='0';	xx[1]='.';	  xx[2]='0';	xx[3]='0';	  xx[4]=str[0]; xx[5]='\0';}
    else if(dec==-1){ xx[0]='0';	xx[1]='.';	  xx[2]='0';	xx[3]=str[0]; xx[4]=str[1]; xx[5]='\0';}
    else if(dec==0) { xx[0]='0';	xx[1]='.';	  xx[2]=str[0];	xx[3]=str[1]; xx[4]=str[2]; xx[5]='\0';}
    else if(dec==1) { xx[0]=str[0]; xx[1]='.';	  xx[2]=str[1]; xx[3]=str[2]; xx[4]=str[3]; xx[5]='\0';}
    else if(dec==2) { xx[0]=str[0]; xx[1]=str[1]; xx[2]='.';    xx[3]=str[2]; xx[4]=str[3]; xx[5]=str[4]; xx[6]='\0';}
    else if(dec==3) { xx[0]=str[0]; xx[1]=str[1]; xx[2]=str[2]; xx[3]='.';    xx[4]=str[3]; xx[5]=str[4]; xx[6]=str[5]; xx[7]='\0'; }
    else if(dec==4) { xx[0]=str[0]; xx[1]=str[1]; xx[2]=str[2]; xx[3]=str[3]; xx[4]='.';    xx[5]=str[4]; xx[6]=str[5]; xx[7]=str[6];  xx[8]='\0'; }
	else			{ xx[0]='0';	xx[1]='.';	  xx[2]='0';	xx[3]='0';	  xx[4]='0';    xx[5]='\0';}
	
	int strl = strlen(xx);
	while((xx[strl-1] == '0') || (xx[strl-1] == '.'))
	{
		if(xx[strl-1] == '.')
		{
			xx[strl-1] = 0;
			break;
		}
		else
		{
			xx[strl-1] = 0;
		}
		strl = strlen(xx);
	}
	strcat(xx, "mm");

	if(fx < 0)
	{
		strcpy(string, "-");
		strcat(string, xx);
	}
	else strcpy(string,xx);
}

void CESDView::mil_to_str(double fx, char *string)
{
    int ndig = 3;
    int dec,sign;
    char xx[100],*str;

	int ih, il; 
	double fh, fl;
	ih = (int)(fx*1000);
	fh = (double)ih;
	fl = fx*1000 - fh;
	il = (int)(fl*10);
	if(il >= 5) fh++;
	fx = fh/1000;

    str = _fcvt(fx,ndig,&dec,&sign);

    if(dec == -2)   { xx[0]='0';	xx[1]='.';	  xx[2]='0';	xx[3]='0';	  xx[4]=str[0]; xx[5]='\0';}
    else if(dec==-1){ xx[0]='0';	xx[1]='.';	  xx[2]='0';	xx[3]=str[0]; xx[4]=str[1]; xx[5]='\0';}
    else if(dec==0) { xx[0]='0';	xx[1]='.';	  xx[2]=str[0];	xx[3]=str[1]; xx[4]=str[2]; xx[5]='\0';}
    else if(dec==1) { xx[0]=str[0]; xx[1]='.';	  xx[2]=str[1]; xx[3]=str[2]; xx[4]=str[3]; xx[5]='\0';}
    else if(dec==2) { xx[0]=str[0]; xx[1]=str[1]; xx[2]='.';    xx[3]=str[2]; xx[4]=str[3]; xx[5]=str[4]; xx[6]='\0';}
    else if(dec==3) { xx[0]=str[0]; xx[1]=str[1]; xx[2]=str[2]; xx[3]='.';    xx[4]=str[3]; xx[5]=str[4]; xx[6]=str[5]; xx[7]='\0'; }
    else if(dec==4) { xx[0]=str[0]; xx[1]=str[1]; xx[2]=str[2]; xx[3]=str[3]; xx[4]='.';    xx[5]=str[4]; xx[6]=str[5]; xx[7]=str[6]; xx[8]='\0'; }
	else if(dec==5) { xx[0]=str[0]; xx[1]=str[1]; xx[2]=str[2]; xx[3]=str[3]; xx[4]=str[4]; xx[5]='.';    xx[6]=str[5]; xx[7]=str[6]; xx[8]=str[7]; xx[9]='\0';}
	else            { xx[0]='0';	xx[1]='.';	  xx[2]='0';	xx[3]='0';	  xx[4]='0';	xx[5]='\0';}

	int strl = strlen(xx);
	while((xx[strl-1] == '0') || (xx[strl-1] == '.'))
	{
		if(xx[strl-1] == '.')
		{
			xx[strl-1] = 0;
			break;
		}
		else
		{
			xx[strl-1] = 0;
		}
		strl = strlen(xx);
	}
	strcat(xx, "mil");

	if(fx < 0)
	{
		strcpy(string, "-");
		strcat(string, xx);
	}
	else strcpy(string,xx);
}

void CESDView::DrawAllSCHObject(CDC* pDC)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
	POSITION pos;
	pos = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;

		pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;
			if(pline->PickMe(this) == TRUE) pline->Draw(pDC, this);	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag != FLAG_NORMAL))	continue;
			ppolyline->Draw(pDC, this);	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag != FLAG_NORMAL))	continue;
			pjunc->Draw(pDC, this);	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag != FLAG_NORMAL))	continue;
			if(ptext->PickMe(pDC, this) == TRUE) 
			{
				if(ptext->m_nID == TEXT_SHEETTEXT) { continue; }
				else if(ptext->m_nID == TEXT_NETLABEL) ptext->DrawSignal(pDC, this);
				else ptext->Draw(pDC, this);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag != FLAG_NORMAL))	continue;
			if(parc->PickMe(this) == TRUE) parc->Draw(pDC, this);	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag != FLAG_NORMAL))	continue;
			if(pellipse->PickMe(this) == TRUE) pellipse->Draw(pDC, this);	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag != FLAG_NORMAL))	continue;
			if(pellipsefill->PickMe(this) == TRUE) pellipsefill->Draw(pDC, this);	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag != FLAG_NORMAL))	continue;
			if(ppie->PickMe(this) == TRUE)	ppie->Draw(pDC, this);	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag != FLAG_NORMAL))	continue;
			if(ppolygon->PickMe(this) == TRUE) ppolygon->Draw(pDC, this);	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag != FLAG_NORMAL))	continue;
			if(ptextframe->PickMe(this) == TRUE) ptextframe->Draw(pDC, this);	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag != FLAG_NORMAL))	continue;
			prect->Draw(pDC, this);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag != FLAG_NORMAL))	continue;
			if(proundrect->PickMe(this) == TRUE) proundrect->Draw(pDC, this);	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			if((psheetsymbol->m_nDiagram != m_nDiagram)||(psheetsymbol->m_nFlag != FLAG_NORMAL))
			{
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);

				continue;
			}
			psheetsymbol->Draw(pDC, this);

			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
			{
				pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
				{
					COb_schsheetentry* psheetentry;
					psheetentry = (COb_schsheetentry*)pobject;
					if((psheetentry->m_nDiagram != m_nDiagram)||(psheetentry->m_nFlag == FLAG_DELETED))	continue;
					psheetentry->Draw(pDC, this);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext;
					ptext = (COb_schtext*)pobject;
					if(ptext->PickMe(pDC,this) == TRUE) ptext->Draw(pDC, this);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag != FLAG_NORMAL))	continue;
			pport->Draw(pDC, this);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag != FLAG_NORMAL))	continue;
			ppower->Draw(pDC, this);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag != FLAG_NORMAL))	continue;
			if(pbezier->PickMe(this) == TRUE) pbezier->Draw(pDC, this);	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag != FLAG_NORMAL))	continue;
			if(pgraphic->PickMe(this) == TRUE) pgraphic->Draw(pDC, this);	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram == m_nDiagram)&&(pcomp->m_nFlag == FLAG_NORMAL))	pcomp->Draw(pDC, this);

			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
	}

	//Draw Selection
	if(nEDA_Print_Flag == 0)
	{
		pos = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
		for(int i=0; i<num; i++)
		{	
			if(pos == NULL) break;

			pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
			
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;
				if(pline->PickMe(this) == TRUE)
				{
					if(pline->m_bSelection == TRUE) pline->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
			{
				COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
				if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag != FLAG_NORMAL))	continue;
				if(ppolyline->m_bSelection == TRUE) ppolyline->DrawSelection(pDC, this);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc;
				pjunc = (COb_schjunc*)pobject;
				if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag != FLAG_NORMAL))	continue;
				if(pjunc->m_bSelection == TRUE) pjunc->DrawSelection(pDC, this);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext;
				ptext = (COb_schtext*)pobject;
				if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag != FLAG_NORMAL))	continue;
				if(ptext->PickMe(pDC, this) == TRUE) 
				{
					if(ptext->m_bSelection == TRUE) ptext->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
			{
				COb_scharc* parc;
				parc = (COb_scharc*)pobject;
				if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag != FLAG_NORMAL))	continue;
				if(parc->PickMe(this) == TRUE)
				{
					if(parc->m_bSelection == TRUE) parc->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
			{
				COb_schellipse* pellipse;
				pellipse = (COb_schellipse*)pobject;
				if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag != FLAG_NORMAL))	continue;
				if(pellipse->PickMe(this) == TRUE)
				{
					if(pellipse->m_bSelection == TRUE) pellipse->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
			{
				COb_schellipsefill* pellipsefill;
				pellipsefill = (COb_schellipsefill*)pobject;
				if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag != FLAG_NORMAL))	continue;
				if(pellipsefill->PickMe(this) == TRUE)
				{
					if(pellipsefill->m_bSelection == TRUE) pellipsefill->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
			{
				COb_schpie* ppie;
				ppie = (COb_schpie*)pobject;
				if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag != FLAG_NORMAL))	continue;
				if(ppie->PickMe(this) == TRUE)	
				{
					if(ppie->m_bSelection == TRUE) ppie->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
			{
				COb_schpolygon* ppolygon;
				ppolygon = (COb_schpolygon*)pobject;
				if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag != FLAG_NORMAL))	continue;
				if(ppolygon->PickMe(this) == TRUE)
				{
					if(ppolygon->m_bSelection == TRUE) ppolygon->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
			{
				COb_schtextframe* ptextframe;
				ptextframe = (COb_schtextframe*)pobject;
				if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag != FLAG_NORMAL))	continue;
				if(ptextframe->PickMe(this) == TRUE)
				{
					if(ptextframe->m_bSelection == TRUE) ptextframe->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
			{
				COb_schrect* prect;
				prect = (COb_schrect*)pobject;
				if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag != FLAG_NORMAL))	continue;
				if(prect->m_bSelection == TRUE) prect->DrawSelection(pDC, this);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
			{
				COb_schroundrect* proundrect;
				proundrect = (COb_schroundrect*)pobject;
				if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag != FLAG_NORMAL))	continue;
				if(proundrect->PickMe(this) == TRUE)
				{
					if(proundrect->m_bSelection == TRUE) proundrect->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol;
				psheetsymbol = (COb_schsheetsymbol*)pobject;
				if((psheetsymbol->m_nDiagram != m_nDiagram)||(psheetsymbol->m_nFlag != FLAG_NORMAL))
				{
					for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);

					continue;
				}
				if(psheetsymbol->m_bSelection == TRUE) psheetsymbol->DrawSelection(pDC, this);

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
				{
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry;
						psheetentry = (COb_schsheetentry*)pobject;
						if((psheetentry->m_nDiagram != m_nDiagram)||(psheetentry->m_nFlag == FLAG_DELETED))	continue;
						if(psheetentry->m_bSelection == TRUE) psheetentry->DrawSelection(pDC, this);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext;
						ptext = (COb_schtext*)pobject;
						if(ptext->PickMe(pDC,this) == TRUE) 
						{
							if(ptext->m_bSelection == TRUE) ptext->DrawSelection(pDC, this);
						}
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport;
				pport = (COb_schport*)pobject;
				if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag != FLAG_NORMAL))	continue;
				if(pport->m_bSelection == TRUE) pport->DrawSelection(pDC, this);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower;
				ppower = (COb_schpower*)pobject;
				if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag != FLAG_NORMAL))	continue;
				if(ppower->m_bSelection == TRUE) ppower->DrawSelection(pDC, this);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
			{
				COb_schbezier* pbezier;
				pbezier = (COb_schbezier*)pobject;
				if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag != FLAG_NORMAL))	continue;
				if(pbezier->PickMe(this) == TRUE)
				{
					if(pbezier->m_bSelection == TRUE) pbezier->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
			{
				COb_schgraphic* pgraphic;
				pgraphic = (COb_schgraphic*)pobject;
				if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag != FLAG_NORMAL))	continue;
				if(pgraphic->PickMe(this) == TRUE)
				{
					if(pgraphic->m_bSelection == TRUE) pgraphic->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp;
				pcomp = (COb_schcomp*)pobject;
				if((pcomp->m_nDiagram == m_nDiagram)&&(pcomp->m_nFlag == FLAG_NORMAL))	
				{
					if(pcomp->m_bSelection == TRUE) pcomp->DrawSelection(pDC, this);
				}

				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
			}
		}
	}
}

void CESDView::OnMultiSelection(UINT nID)
{
	nEDA_Multi_Selection = nID - ID_SELECTION_1;
}

void CESDView::DrawPcbOrigin(CDC* pDC)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	int x, y;
	Get_VPort_XY(&x, &y, pDoc->m_cPCB_Sheet.m_fOriginX, pDoc->m_cPCB_Sheet.m_fOriginY);

	CPen apen;
	apen.CreatePen(PS_SOLID, 1, nEDA_PCB_LayerColor[CONNECTIONS]);
	CPen* oldpen = pDC->SelectObject(&apen);

	pDC->MoveTo(x-18, y-18);
	pDC->LineTo(x+18, y+18);
	pDC->MoveTo(x-18, y+18);
	pDC->LineTo(x+18, y-18);

	pDC->Arc(x-18, y-18, x+18, y+18, x-18, y-18, x-18, y-18);

	pDC->SelectObject(oldpen);
}

void CESDView::DrawPcbConnections(CDC* pDC)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(pDoc->m_sPCB_LayerParam[CONNECTIONS].bLayerShow == FALSE) return;

	for(int i=0; i<pDoc->m_arrayPcbConnection.GetCount(); i++)
	{
		COb_pcbconnection* pconnection = (COb_pcbconnection*)pDoc->m_arrayPcbConnection.GetAt(i);
		pconnection->Draw(pDC, this);
	}
}

void CESDView::DrawPcbDrcErrors(CDC* pDC)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(pDoc->m_sPCB_LayerParam[DRCERROR].bLayerShow == FALSE) return;

	for(int i=0; i<pDoc->m_arrayPcbDrcErrors.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element = pDoc->m_arrayPcbDrcErrors.GetAt(i);
		if((selected_element.pcbelement == 0)||( selected_element.index < 0)) continue;

		if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
			if(ptrack->m_nFlag != FLAG_NORMAL) continue;

			ptrack->DrawHighlighted(pDC, this, DRCERROR);
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
			if(parc->m_nFlag != FLAG_NORMAL) continue;

			parc->DrawHighlighted(pDC, this, DRCERROR);
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
			if(pfill->m_nFlag != FLAG_NORMAL) continue;

			pfill->DrawHighlighted(pDC, this, DRCERROR);

		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
			if(pregion->m_nFlag != FLAG_NORMAL) continue;

			pregion->DrawHighlighted(pDC, this, DRCERROR);
		}	
		/*else if(selected_element.pcbelement == PCB_ELEMENT_POLYGON)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(selected_element.index);

		}	*/
	}
	for(int i=0; i<pDoc->m_arrayPcbDrcErrors.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element = pDoc->m_arrayPcbDrcErrors.GetAt(i);
		if((selected_element.pcbelement == 0)||( selected_element.index < 0)) continue;

		if(selected_element.pcbelement == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
			if(ppad->m_nFlag != FLAG_NORMAL) continue;

			ppad->DrawHighlighted(pDC, this, DRCERROR);
		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
			if(pvia->m_nFlag != FLAG_NORMAL) continue;

			pvia->DrawHighlighted(pDC, this, DRCERROR);
		}		
	}
}

void CESDView::DrawAllPCBObject(CDC* pDC)
{
	int num;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	CArray <int, int> arrayLayerShowOrder;
	for(int j=pDoc->m_arrayPcbLayerShow.GetCount()-1; j>=0; j--)
	{
		int layer = pDoc->m_arrayPcbLayerShow.GetAt(j);
		if((layer == m_nPCB_Current_Layer)||(layer == MULTILAYER)||(layer == TOPOVERLAY)||(layer == BOTTOMOVERLAY)) continue;

		arrayLayerShowOrder.Add(layer);
	}
	if( pDoc->LayerVisible(m_nPCB_Current_Layer) == TRUE) arrayLayerShowOrder.Add(m_nPCB_Current_Layer);
	if( pDoc->LayerVisible(MULTILAYER) == TRUE) arrayLayerShowOrder.Add(MULTILAYER);
	if( pDoc->LayerVisible(BOTTOMOVERLAY) == TRUE) arrayLayerShowOrder.Add(BOTTOMOVERLAY);
	if( pDoc->LayerVisible(TOPOVERLAY) == TRUE) arrayLayerShowOrder.Add(TOPOVERLAY);

	for(int j=0; j<arrayLayerShowOrder.GetCount(); j++)
	{
		int layer = arrayLayerShowOrder.GetAt(j);

		num = pDoc->m_arrayPcbCoordinate.GetCount();
		for(int i=0; i<num; i++)
		{	
			COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(i);
			if((pcoordinate->m_nFlag != FLAG_NORMAL)||(pcoordinate->m_nLayer != layer))	continue;

			pcoordinate->Draw(pDC, this, 0);	
		}

		num = pDoc->m_arrayPcbDimension.GetCount();
		for(int i=0; i<num; i++)
		{	
			COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(i);
			if((pdimension->m_nFlag != FLAG_NORMAL)||(pdimension->m_nLayer != layer))	continue;

			pdimension->Draw(pDC, this, 0);	
		}

		num = pDoc->m_arrayPcbTrack.GetCount();
		for(int i=0; i<num; i++)
		{	
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
			if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nLayer != layer))	continue;

			if(ptrack->PickMe(this) == TRUE) ptrack->Draw(pDC, this);	
		}

		num = pDoc->m_arrayPcbArc.GetCount();
		for(int i=0; i<num; i++)
		{	
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
			if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nLayer != layer))	continue;

			if(parc->PickMe(this) == TRUE) parc->Draw(pDC, this);	
		}

		num = pDoc->m_arrayPcbText.GetCount();
		for(int i=0; i<num; i++)
		{	
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);

			if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_nLayer != layer))	continue;

			if(ptext->m_bDesignator == TRUE)
			{
				COb_pcbcomp* pcbcomp;
				pcbcomp = pDoc->m_arrayPcbComp.GetAt(ptext->m_nComponent);
				if(pcbcomp->m_bNameOn == FALSE) continue;
			}
			else if(ptext->m_bComment == TRUE)
			{
				COb_pcbcomp* pcbcomp;
				pcbcomp = pDoc->m_arrayPcbComp.GetAt(ptext->m_nComponent);
				if(pcbcomp->m_bCommentOn == FALSE) continue;
			}

			if(ptext->PickMe(pDC, this) == TRUE) ptext->Draw(pDC, this);	

		}

		num = pDoc->m_arrayPcbRegion.GetCount();
		for(int i=0; i<num; i++)
		{	
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nLayer != layer))	continue;

			if(pregion->PickMe(this) == TRUE) pregion->Draw(pDC, this);	
		}

		num = pDoc->m_arrayPcbPolygon.GetCount();
		for(int i=0; i<num; i++)
		{	
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
			if((ppolygon->m_nFlag != FLAG_NORMAL)||(ppolygon->m_nLayer != layer))	continue;

			if(ppolygon->ShowPolygonOutline(i) == TRUE) ppolygon->Draw(pDC, this);	
		}

		num = pDoc->m_arrayPcbFill.GetCount();
		for(int i=0; i<num; i++)
		{	
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
			if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nLayer != layer))	continue;

			if(pfill->PickMe(this) == TRUE) pfill->Draw(pDC, this);	
		}

		num = pDoc->m_arrayPcbPad.GetCount();
		for(int i=0; i<num; i++)
		{	
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
			if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nLayer == MULTILAYER)) continue;

			if(pDoc->m_sPCB_LayerParam[ppad->m_nLayer].bLayerShow == TRUE)
			{
				if(ppad->m_nLayer == layer)
				{
					if(ppad->PickMe(this) == TRUE) ppad->Draw(pDC, this, DRAW_NORMAL);	
				}
			}
			else
			{
				if(ppad->PickMe(this) == TRUE) ppad->DrawSingleLayerPadSpecial(pDC, this, layer);
			}
		}

	}

	num = pDoc->m_arrayPcbVia.GetCount();
	for(int i=0; i<num; i++)
	{	
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag != FLAG_NORMAL)	continue;

		if(pvia->PickMe(this) == TRUE) pvia->Draw(pDC, this, 0);
	}

	num = pDoc->m_arrayPcbPad.GetCount();
	for(int i=0; i<num; i++)
	{	
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nLayer == MULTILAYER))
		{
			if(ppad->PickMe(this) == TRUE) ppad->Draw(pDC, this, DRAW_NORMAL);
		}
	}

	if(nEDA_Print_Flag == 0) DrawPcbSelections(pDC);

}

void CESDView::DrawPcbSelections(CDC* pDC)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nComponent >= 0)||(parc->m_nPolygon >= 0)) continue;
			
		if(parc->m_bSelection == TRUE) parc->DrawSelection(pDC, this);
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;
			
		if(ptrack->m_bSelection == TRUE) ptrack->DrawSelection(pDC, this);
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag != FLAG_NORMAL)||((ptext->m_nComponent >= 0)&&(ptext->m_bDesignator == FALSE)&&(ptext->m_bComment == FALSE))) continue;
			
		if(ptext->m_bSelection == TRUE) ptext->DrawSelection(pDC, this);
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)) continue;
			
		if(pfill->m_bSelection == TRUE) pfill->DrawSelection(pDC, this);
	}
	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nComponent >= 0)||(pregion->m_nPolygon >= 0)) continue;
			
		if(pregion->m_bSelection == TRUE) pregion->DrawSelection(pDC, this);
	}
	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
		if(ppolygon->m_nFlag != FLAG_NORMAL) continue;
			
		if(ppolygon->m_bSelection ==TRUE)  ppolygon->DrawSelection(pDC, this);
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		//if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent >= 0)) continue;
		if(ppad->m_nFlag != FLAG_NORMAL) continue;
			
		if(ppad->m_bSelection == TRUE) ppad->DrawSelection(pDC, this);
	}		
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent >= 0)) continue;
			
		if(pvia->m_bSelection ==TRUE) pvia->DrawSelection(pDC, this);
	}	
	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag != FLAG_NORMAL) continue;
			
		if(pcomp->m_bSelection == TRUE) pcomp->DrawSelection(pDC, this, i);
	}	
	for(int i=0; i<pDoc->m_arrayPcbCoordinate.GetCount(); i++)
	{
		COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(i);
		if(pcoordinate->m_nFlag != FLAG_NORMAL) continue;
			
		if(pcoordinate->m_bSelection == TRUE) pcoordinate->DrawSelection(pDC, this);
	}	
	for(int i=0; i<pDoc->m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(i);
		if(pdimension->m_nFlag != FLAG_NORMAL) continue;
			
		if(pdimension->m_bSelection == TRUE) pdimension->DrawSelection(pDC, this);
	}	
}

void CESDView::OnDesignOption()
{
	int i;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	CSheet_SchOptions options_sheet("分页选项", NULL, 0);

	options_sheet.m_PageOptions.m_pDoc = pDoc;
	options_sheet.m_PageOptions.m_nComboSheetNum = m_nDiagram;
	options_sheet.m_PageOptions.m_csEditSheetName = pDoc->m_csSCH_SheetName[m_nDiagram];
	options_sheet.m_PageOptions.m_bCheckSheetEnable = pDoc->m_bSCH_SheetShow[m_nDiagram];
	options_sheet.m_PageOptions.m_nComboOrientation = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation;
	options_sheet.m_PageOptions.m_bCheckTitle = pDoc->m_pSCH_Sheet[m_nDiagram]->m_bTitleBlock;
	options_sheet.m_PageOptions.m_nComboTitle = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nTitleBlockStyle;
	options_sheet.m_PageOptions.m_nEditNumberSpace = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetNumberSpace;
	options_sheet.m_PageOptions.m_bCheckReference = pDoc->m_pSCH_Sheet[m_nDiagram]->m_bShowReferenceZones;
	options_sheet.m_PageOptions.m_bCheckShowBorder = pDoc->m_pSCH_Sheet[m_nDiagram]->m_bShowBorder;
	options_sheet.m_PageOptions.m_nEditStandard = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle;
	options_sheet.m_PageOptions.m_bCheckSnap = pDoc->m_bSCH_SnapGridEnable;
	options_sheet.m_PageOptions.m_bCheckVisible = pDoc->m_bSCH_VisibleGridShow;
	options_sheet.m_PageOptions.m_bCheckElectrical = pDoc->m_bSCH_ElecGridEnable;
	options_sheet.m_PageOptions.m_csEditSnap = pDoc->Convert_XY_To_String(pDoc->m_fSCH_SnapGrid);
	options_sheet.m_PageOptions.m_csEditVisible = pDoc->Convert_XY_To_String(pDoc->m_fSCH_VisibleGrid);
	options_sheet.m_PageOptions.m_csEditElectrical = pDoc->Convert_XY_To_String(pDoc->m_fSCH_ElecGrid);
	options_sheet.m_PageOptions.m_bCheckCustom = pDoc->m_pSCH_Sheet[m_nDiagram]->m_bCustom;
	options_sheet.m_PageOptions.m_csEditCustomWidth = pDoc->Convert_XY_To_String(pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth);
	options_sheet.m_PageOptions.m_csEditCustomHeight = pDoc->Convert_XY_To_String(pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight);
	options_sheet.m_PageOptions.m_nEditXCount = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nCustomXRegionCount;
	options_sheet.m_PageOptions.m_nEditYCount = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nCustomYRegionCount;
	options_sheet.m_PageOptions.m_csEditMargin = pDoc->Convert_XY_To_String(pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomMargin);
	
	options_sheet.m_PageUnit.m_pDoc = pDoc;
	if((pDoc->m_nSCH_Unit%2) == 0)
	{
		options_sheet.m_PageUnit.m_bCheckI = TRUE;
		options_sheet.m_PageUnit.m_bCheckM = FALSE;
		options_sheet.m_PageUnit.m_nComboI = pDoc->m_nSCH_Unit/2;
		options_sheet.m_PageUnit.m_nComboM = 0;
	}
	else
	{
		options_sheet.m_PageUnit.m_bCheckI = FALSE;
		options_sheet.m_PageUnit.m_bCheckM = TRUE;
		options_sheet.m_PageUnit.m_nComboI = 0;
		options_sheet.m_PageUnit.m_nComboM = pDoc->m_nSCH_Unit/2;
	}

	if(options_sheet.DoModal() == IDOK)
	{
		int diagram = options_sheet.m_PageOptions.m_nComboSheetNum;
		pDoc->m_csSCH_SheetName[diagram] = options_sheet.m_PageOptions.m_csEditSheetName;
		pDoc->m_pSCH_Sheet[diagram]->m_nOrientation = options_sheet.m_PageOptions.m_nComboOrientation;
		pDoc->m_bSCH_SheetShow[diagram] = options_sheet.m_PageOptions.m_bCheckSheetEnable;
		pDoc->m_pSCH_Sheet[diagram]->m_bTitleBlock = options_sheet.m_PageOptions.m_bCheckTitle;
		pDoc->m_pSCH_Sheet[diagram]->m_nTitleBlockStyle = options_sheet.m_PageOptions.m_nComboTitle;
		pDoc->m_pSCH_Sheet[diagram]->m_nSheetNumberSpace = options_sheet.m_PageOptions.m_nEditNumberSpace;
		pDoc->m_pSCH_Sheet[diagram]->m_bShowReferenceZones = options_sheet.m_PageOptions.m_bCheckReference;
		pDoc->m_pSCH_Sheet[diagram]->m_bShowBorder = options_sheet.m_PageOptions.m_bCheckShowBorder;
		pDoc->m_pSCH_Sheet[diagram]->m_nStandardStyle = options_sheet.m_PageOptions.m_nEditStandard;
		pDoc->m_bSCH_SnapGridEnable = options_sheet.m_PageOptions.m_bCheckSnap;
		pDoc->m_bSCH_VisibleGridShow = options_sheet.m_PageOptions.m_bCheckVisible;
		pDoc->m_bSCH_ElecGridEnable = options_sheet.m_PageOptions.m_bCheckElectrical;
		pDoc->m_fSCH_SnapGrid = pDoc->Convert_String_To_XY(options_sheet.m_PageOptions.m_csEditSnap);
		pDoc->m_fSCH_VisibleGrid = 	pDoc->Convert_String_To_XY(options_sheet.m_PageOptions.m_csEditVisible);
		pDoc->m_fSCH_ElecGrid = pDoc->Convert_String_To_XY(options_sheet.m_PageOptions.m_csEditElectrical);
		pDoc->m_pSCH_Sheet[diagram]->m_bCustom = options_sheet.m_PageOptions.m_bCheckCustom;
		pDoc->m_pSCH_Sheet[diagram]->m_fCustomWidth = pDoc->Convert_String_To_XY(options_sheet.m_PageOptions.m_csEditCustomWidth);
		pDoc->m_pSCH_Sheet[diagram]->m_fCustomHeight = pDoc->Convert_String_To_XY(options_sheet.m_PageOptions.m_csEditCustomHeight);
		pDoc->m_pSCH_Sheet[diagram]->m_nCustomXRegionCount = options_sheet.m_PageOptions.m_nEditXCount;
		pDoc->m_pSCH_Sheet[diagram]->m_nCustomYRegionCount = options_sheet.m_PageOptions.m_nEditYCount;
		pDoc->m_pSCH_Sheet[diagram]->m_fCustomMargin = pDoc->Convert_String_To_XY(options_sheet.m_PageOptions.m_csEditMargin);
		pDoc->m_pSCH_Sheet[diagram]->m_nBorderColor = options_sheet.m_PageOptions.m_cBorderColor.m_nColor;
		pDoc->m_pSCH_Sheet[diagram]->m_nSheetColor = options_sheet.m_PageOptions.m_cSheetColor.m_nColor;

		if(options_sheet.m_PageUnit.m_bCheckI == TRUE) pDoc->m_nSCH_Unit = options_sheet.m_PageUnit.m_nComboI*2;
		else  pDoc->m_nSCH_Unit = options_sheet.m_PageUnit.m_nComboM*2 + 1;

	}

	for(i=0; i<SCH_SHEET_NUM; i++)	if(pDoc->m_bSCH_SheetShow[i] == TRUE) break;
	if(i >= SCH_SHEET_NUM) pDoc->m_bSCH_SheetShow[0] = TRUE;

	Invalidate();
	CWnd* pWnd = GetParent();
	pWnd->Invalidate();
}

void CESDView::Plus_Sheet()
{
	int i, diagrambak;
	diagrambak = m_nDiagram;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(pDoc->m_nDocumentType != DOC_ESD) return;

	if(m_nDiagram == SCH_SHEET_NUM-1) return;

	for(i=m_nDiagram+1; i<SCH_SHEET_NUM; i++)
	{ 
		if(pDoc->m_bSCH_SheetShow[i] == TRUE) break;
	}

	if(i < SCH_SHEET_NUM)
	{
		m_nDiagram = i;
	}

	if(diagrambak != m_nDiagram)
	{
		pDoc->m_fSCH_ScreenX[diagrambak] = m_fScreenX;
		pDoc->m_fSCH_ScreenY[diagrambak] = m_fScreenY;
		pDoc->m_fSCH_Scale[diagrambak] = m_fScale;
		m_fScreenX = pDoc->m_fSCH_ScreenX[m_nDiagram];
		m_fScreenY = pDoc->m_fSCH_ScreenY[m_nDiagram];
		m_fScale = pDoc->m_fSCH_Scale[m_nDiagram];

		UpdateZoomSlider();
		ScrollBar_Control();
		Invalidate();
		CWnd* pWnd = GetParent();
		pWnd->Invalidate();
	}
	else
	{
		if((bEDA_Sheet_AutoOn == TRUE)&&(pDoc->Get_SCH_Elements_Param(0, m_nDiagram, 0) > 0))
		{
			for(i=m_nDiagram+1; i<SCH_SHEET_NUM; i++)
			{ 
				if(pDoc->Get_SCH_Elements_Param(0,i,0) == 0) break;
			}
			
			if(i < SCH_SHEET_NUM)
			{
				m_nDiagram = i;
			}
			pDoc->m_bSCH_SheetShow[m_nDiagram] = TRUE;

			pDoc->m_fSCH_ScreenX[diagrambak] = m_fScreenX;
			pDoc->m_fSCH_ScreenY[diagrambak] = m_fScreenY;
			pDoc->m_fSCH_Scale[diagrambak] = m_fScale;
			m_fScreenX = pDoc->m_fSCH_ScreenX[m_nDiagram];
			m_fScreenY = pDoc->m_fSCH_ScreenY[m_nDiagram];
			m_fScale = pDoc->m_fSCH_Scale[m_nDiagram];
			
			UpdateZoomSlider();
			ScrollBar_Control();
			Invalidate();
			CWnd* pWnd = GetParent();
			pWnd->Invalidate();
		}
	}
}

void CESDView::Minus_Sheet()
{
	int i, diagrambak;
    diagrambak = m_nDiagram;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	if(pDoc->m_nDocumentType != DOC_ESD) return;

	if(m_nDiagram == 0) return;
	
	for(i=m_nDiagram-1; i>=0; i--)
	{ 
		if(pDoc->m_bSCH_SheetShow[i] == TRUE) break;
	}

	if(i >= 0)
	{
		m_nDiagram = i;
	}

	if(diagrambak != m_nDiagram)
	{
		if((bEDA_Sheet_AutoOn == TRUE)&&(pDoc->Get_SCH_Elements_Param(0, diagrambak, 0) == 0)) pDoc->m_bSCH_SheetShow[diagrambak] = FALSE;

		pDoc->m_fSCH_ScreenX[diagrambak] = m_fScreenX;
		pDoc->m_fSCH_ScreenY[diagrambak] = m_fScreenY;
		pDoc->m_fSCH_Scale[diagrambak] = m_fScale;
		m_fScreenX = pDoc->m_fSCH_ScreenX[m_nDiagram];
		m_fScreenY = pDoc->m_fSCH_ScreenY[m_nDiagram];
		m_fScale = pDoc->m_fSCH_Scale[m_nDiagram];

		UpdateZoomSlider();
		ScrollBar_Control();
		Invalidate();
		CWnd* pWnd = GetParent();
		pWnd->Invalidate();
	}
}

void CESDView::Plus_Layer()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	int i, layerbak;
	layerbak = m_nPCB_Current_Layer;

	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	CChildFrame* pChild = (CChildFrame*)pFrame->MDIGetActive();
	if(pChild != NULL)
	{
		CString cstring;
		for(i=0; i<pChild->m_wndSheet.m_strTitles.GetCount(); i++)
		{
			if(pDoc->m_sPCB_LayerParam[m_nPCB_Current_Layer].csLayerName == pChild->m_wndSheet.m_strTitles.GetAt(i)) 
			{
				i++;
				if(i >= pChild->m_wndSheet.m_strTitles.GetCount()) i = 0;
				cstring = pChild->m_wndSheet.m_strTitles.GetAt(i);
				break;
			}
		}

		for(i=0; i<=MULTILAYER; i++)
		{ 
			if(pDoc->m_sPCB_LayerParam[i].csLayerName == cstring) 
			{
				m_nPCB_Current_Layer = i;
				break;
			}
		}
	}

	if(layerbak != m_nPCB_Current_Layer)
	{
		Invalidate();
		CWnd* pWnd = GetParent();
		pWnd->Invalidate();
	}
}

void CESDView::Minus_Layer()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	int i, layerbak;
	layerbak = m_nPCB_Current_Layer;

	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	CChildFrame* pChild = (CChildFrame*)pFrame->MDIGetActive();
	if(pChild != NULL)
	{
		CString cstring;
		for(i=0; i<pChild->m_wndSheet.m_strTitles.GetCount(); i++)
		{
			if(pDoc->m_sPCB_LayerParam[m_nPCB_Current_Layer].csLayerName == pChild->m_wndSheet.m_strTitles.GetAt(i)) 
			{
				i--;
				if(i < 0 ) i = pChild->m_wndSheet.m_strTitles.GetCount() - 1;
				cstring = pChild->m_wndSheet.m_strTitles.GetAt(i);
				break;
			}
		}

		for(i=0; i<=MULTILAYER; i++)
		{ 
			if(pDoc->m_sPCB_LayerParam[i].csLayerName == cstring) 
			{
				m_nPCB_Current_Layer = i;
				break;
			}
		}
	}

	if(layerbak != m_nPCB_Current_Layer)
	{
		Invalidate();
		CWnd* pWnd = GetParent();
		pWnd->Invalidate();
	}
}

void CESDView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(m_nSystem_State == SCH)
	{
		if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].GetCount() != 0) pCmdUI->Enable(TRUE);
		else pCmdUI->Enable(FALSE);
	}
	else
	{
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].GetCount() != 0) pCmdUI->Enable(TRUE);
		else pCmdUI->Enable(FALSE);
	}
}

void CESDView::OnEditRedo()
{
	int i,num;
	POSITION pos;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	if(m_nSystem_State == SCH)
	{
		CSchUndo* pundo;
		if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == TRUE) return;
		
		num = pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].GetCount();
		pos = pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].GetHeadPosition();
		
		for(i=0; i<num; i++)
		{
			if(pos == NULL) break;
			pundo = pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].GetNext(pos);
			if(pos == NULL) pundo->changeviewpoint = TRUE; 
			pundo->Redo();
		}
		pDoc->m_nSCH_Undo_Num++;
	}
	else
	{
		CPcbUndo* pundo;
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == TRUE) return;
		
		num = pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].GetCount();
		pos = pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].GetHeadPosition();
		
		for(i=0; i<num; i++)
		{
			if(pos == NULL) break;
			pundo = pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].GetNext(pos);
			if(pos == NULL) pundo->changeviewpoint = TRUE; 
			pundo->Redo();
		}
		pDoc->m_nPCB_Undo_Num++;

	}
}

void CESDView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(m_nSystem_State == SCH)
	{
		if(pDoc->m_nSCH_Undo_Num != 0) pCmdUI->Enable(TRUE);
		else pCmdUI->Enable(FALSE);
	}
	else
	{
		if(pDoc->m_nPCB_Undo_Num != 0) pCmdUI->Enable(TRUE);
		else pCmdUI->Enable(FALSE);
	}
}

void CESDView::OnEditUndo()
{
	int i,num;
	POSITION pos;
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	if(m_nSystem_State == SCH)
	{
		CSchUndo* pundo;
		if(pDoc->m_nSCH_Undo_Num == 0) return;

		pDoc->m_nSCH_Undo_Num--;
		num = pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].GetCount();
		pos = pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].GetTailPosition();
		for(i=0; i<num; i++)
		{
			if(pos == NULL) break;
			pundo = pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].GetPrev(pos);
			if(pos == NULL) pundo->changeviewpoint = TRUE;  
			pundo->Undo();
		}
	}
	else
	{
		CPcbUndo* pundo;
		if(pDoc->m_nPCB_Undo_Num == 0) return;
		
		pDoc->m_nPCB_Undo_Num--;
		num = pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].GetCount();
		pos = pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].GetTailPosition();
		for(i=0; i<num; i++)
		{
			if(pos == NULL) break;
			pundo = pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].GetPrev(pos);
			if(pos == NULL) pundo->changeviewpoint = TRUE;  
			pundo->Undo();
		}

	}

}

void CESDView::Change_Viewpoint(double fx, double fy, int diagram)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(m_nSystem_State == SCH)
	{
		if(m_nDiagram != diagram)
		{
			pDoc->m_fSCH_ScreenX[m_nDiagram] = m_fScreenX;
			pDoc->m_fSCH_ScreenY[m_nDiagram] = m_fScreenY;
			pDoc->m_fSCH_Scale[m_nDiagram] = m_fScale;
			
			m_nDiagram = diagram;
			
			pDoc->m_bSCH_SheetShow[m_nDiagram] = TRUE;

			m_fScreenX = pDoc->m_fSCH_ScreenX[m_nDiagram];
			m_fScreenY = pDoc->m_fSCH_ScreenY[m_nDiagram];
			m_fScale = pDoc->m_fSCH_Scale[m_nDiagram];

			m_fCurrentSnapX = fx;
			m_fCurrentSnapY = fy;
			OnViewZoomCenter();

			CWnd* pWnd = GetParent();
			pWnd->Invalidate();			
		}
		else
		{
			int xx,yy;
			Get_VPort_XY(&xx,&yy,fx,fy);
			if((xx > 0)&&(xx < m_nViewport_Ex)&&\
			   (yy > 0)&&(yy < m_nViewport_Ey))
			{
			}
			else
			{
				m_fCurrentSnapX = fx;
				m_fCurrentSnapY = fy;
				OnViewZoomCenter();
			}
		}
	}
	else
	{
		
		int xx,yy;
		Get_VPort_XY(&xx,&yy,fx,fy);
		if((xx > 0)&&(xx < m_nViewport_Ex)&&\
		   (yy > 0)&&(yy < m_nViewport_Ey))
		{
			//MoveCursorToXY(fx, fy);
		}
		else
		{
			m_fCurrentSnapX = fx;
			m_fCurrentSnapY = fy;
			OnViewZoomCenter();
		}
	}
}

void CESDView::ElecHotPointsControl(CClientDC* pDC)
{
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);

	CPen apen;
	apen.CreatePen(PS_SOLID, 1, RGB(255, 0 ,0));
	CPen* oldpen = pDC->SelectObject(&apen);

	if(( m_fpHotPoint2.fx != 0) && ( m_fpHotPoint2.fy != 0))
	{
		int x,y;
		Get_VPort_XY(&x, &y, m_fpHotPoint2.fx, m_fpHotPoint2.fy);
		if((nEDA_Cursor_Mode == 0)||(nEDA_Cursor_Mode == 1))
		{
			pDC->MoveTo(x-20, y-20); pDC->LineTo(x+20, y+20);
			pDC->MoveTo(x-20, y+20); pDC->LineTo(x+20, y-20);
		}
		else
		{
			pDC->MoveTo(x, y - 20); pDC->LineTo(x, y + 20);
			pDC->MoveTo(x - 20, y); pDC->LineTo(x + 20, y);
		}
		m_fpHotPoint2.fx = 0;
		m_fpHotPoint2.fy = 0;
	}
	if(( m_fpHotPoint1.fx != 0) && ( m_fpHotPoint1.fy != 0))
	{
		int x,y;
		Get_VPort_XY(&x, &y, m_fpHotPoint1.fx, m_fpHotPoint1.fy);
		if((nEDA_Cursor_Mode == 0)||(nEDA_Cursor_Mode == 1))
		{
			pDC->MoveTo(x-20, y-20); pDC->LineTo(x+20, y+20);
			pDC->MoveTo(x-20, y+20); pDC->LineTo(x+20, y-20);
		}
		else
		{
			pDC->MoveTo(x, y-20); pDC->LineTo(x, y+20);
			pDC->MoveTo(x-20, y); pDC->LineTo(x+20, y);
		}
		m_fpHotPoint2.fx = m_fpHotPoint1.fx;
		m_fpHotPoint2.fy = m_fpHotPoint1.fy;
		m_fpHotPoint1.fx = 0;
		m_fpHotPoint1.fy = 0;
	}

	pDC->SelectObject(oldpen);
	pDC->SetROP2(drawmode);
}

void CESDView::PcbElecHotPointsControl(CClientDC* pDC)
{
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);

	CPen apen;
	apen.CreatePen(PS_SOLID, 1, RGB(0, 0 ,0));
	CPen* oldpen = pDC->SelectObject(&apen);

	if(( m_fpPcbHotPoint2.fx != 0) && ( m_fpPcbHotPoint2.fy != 0))
	{
		int x,y;
		Get_VPort_XY(&x, &y, m_fpPcbHotPoint2.fx, m_fpPcbHotPoint2.fy);
		pDC->Arc(x-15, y+15, x+15, y-15, x-15,y,x-15,y);
		m_fpPcbHotPoint2.fx = 0;
		m_fpPcbHotPoint2.fy = 0;
	}

	if(( m_fpPcbHotPoint1.fx != 0) && ( m_fpPcbHotPoint1.fy != 0))
	{ 
		int x,y;
		Get_VPort_XY(&x, &y, m_fpPcbHotPoint1.fx, m_fpPcbHotPoint1.fy);

		pDC->Arc(x-15, y+15, x+15, y-15, x-15,y,x-15,y);
		m_fpPcbHotPoint2.fx = m_fpPcbHotPoint1.fx;
		m_fpPcbHotPoint2.fy = m_fpPcbHotPoint1.fy;
		m_fpPcbHotPoint1.fx = 0;
		m_fpPcbHotPoint1.fy = 0;
	}

	pDC->SelectObject(oldpen);
	pDC->SetROP2(drawmode);
}

void  CESDView::CheckPcbCurrentLayer()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc->m_sPCB_LayerParam[m_nPCB_Current_Layer].bLayerShow == FALSE)
	{
		if(pDoc->m_arrayPcbLayerShow.GetCount() > 0) m_nPCB_Current_Layer = pDoc->m_arrayPcbLayerShow.GetAt(0);
		else m_nPCB_Current_Layer = TOPLAYER;
	}
}

void CESDView::OnEditMove()
{
	unsigned ch1;
	char szMoveText[] = "移动图形对象: <左键><回车>选择对象移动, <右键><ESC>结束命令";
	MoveCursorToCenter();

	Write_Help(szMoveText);
	do{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			if(m_nSystem_State == SCH)
			{
				Struct_Multi_Selection multi_selection = SelectSCHObject(m_fCurrentX, m_fCurrentY, MULTI_SELECTION);
				MoveElement(multi_selection.element, multi_selection.pos);
			}
			else
			{
				Struct_Pcb_Multi_Selection multi_selection = SelectPcbObject(m_fCurrentX, m_fCurrentY, NOT_INCLUDE_CPAD);
				PcbMoveElement(multi_selection.pcbelement, multi_selection.index);
			}
			Write_Help(szMoveText);
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnEditDelete()
{
	unsigned ch1;
	char szMoveText[] = "删除图形对象: <左键><回车>选择对象删除, <右键><ESC>结束命令";

	Write_Help(szMoveText);
	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			if(m_nSystem_State == SCH)
			{
				Struct_Multi_Selection multi_selection = SelectSCHObject(m_fCurrentX, m_fCurrentY, MULTI_SELECTION);
				DeleteElement(multi_selection.element, multi_selection.pos);
			}
			else
			{
				Struct_Pcb_Multi_Selection multi_selection = SelectPcbObject(m_fCurrentX, m_fCurrentY, NOT_INCLUDE_CPAD);
				PcbDeleteElement(multi_selection.pcbelement, multi_selection.index);
			}
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnEditSelectInside()
{
	int ch1;
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	Write_Help("定义区域一角");
	flag = 0;

	int drawmode;
	drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	fx1 = fy1 = fx2 = fy2 = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			fx1 = m_fCurrentSnapX; fy1 = m_fCurrentSnapY;
			Get_VPort_XY(&xs,&ys,fx1,fy1);
			xe=xs;
			ye=ys;
			Write_Help("定义区域另一角");
			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,fx1,fy1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto LABEL;

				}
				if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{ 
						flag=1;
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
					else
					{
						if(nEDA_RedrawFlag == ON){ nEDA_RedrawFlag = OFF;}
						else 
						{ 
							dc.MoveTo(xs,ys);
							dc.LineTo(xe,ys);
							dc.LineTo(xe,ye);
							dc.LineTo(xs,ye);
							dc.LineTo(xs,ys);
						}
						fx2 = m_fCurrentSnapX; fy2 = m_fCurrentSnapY;
						Get_VPort_XY(&xe,&ye,fx2,fy2);
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
				}
				if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{  
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto LABEL;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		else Command_In_Function(ch1);
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

LABEL:
	if((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC)&&(fx1 != fx2)&&(fy1 != fy2))
	{
		double fminx, fminy, fmaxx, fmaxy;
		fminx = MIN(fx1, fx2);
		fmaxx = MAX(fx1, fx2);
		fminy = MIN(fy1, fy2);
		fmaxy = MAX(fy1, fy2);
	
		if(m_nSystem_State == SCH)
		{
			SelectAll(FALSE);
			SelectInsideArea(fminx, fminy, fmaxx, fmaxy, m_nDiagram);
		}
		else
		{
			PcbSelectAll(FALSE);
			PcbSelectInsideArea(fminx, fminy, fmaxx, fmaxy);
		}
	}
	
	Invalidate();

	dc.SetROP2(drawmode);

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnEditSelectOutside()
{
	int ch1;
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	Write_Help("定义区域一角");
	flag = 0;

	int drawmode;
	drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	fx1 = fy1 = fx2 = fy2 = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			fx1 = m_fCurrentSnapX; fy1 = m_fCurrentSnapY;
			Get_VPort_XY(&xs,&ys,fx1,fy1);
			xe=xs;
			ye=ys;
			Write_Help("定义区域另一角");
			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,fx1,fy1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto LABEL;

				}
				if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{ 
						flag=1;
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
					else
					{
						if(nEDA_RedrawFlag == ON){ nEDA_RedrawFlag = OFF;}
						else 
						{ 
							dc.MoveTo(xs,ys);
							dc.LineTo(xe,ys);
							dc.LineTo(xe,ye);
							dc.LineTo(xs,ye);
							dc.LineTo(xs,ys);
						}
						fx2 = m_fCurrentSnapX; fy2 = m_fCurrentSnapY;
						Get_VPort_XY(&xe,&ye,fx2,fy2);
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
				}
				if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{  
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto LABEL;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		else Command_In_Function(ch1);
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

LABEL:
	if((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC)&&(fx1 != fx2)&&(fy1 != fy2))
	{
		double fminx, fminy, fmaxx, fmaxy;
		fminx = MIN(fx1, fx2);
		fmaxx = MAX(fx1, fx2);
		fminy = MIN(fy1, fy2);
		fmaxy = MAX(fy1, fy2);
	
		if(m_nSystem_State == SCH)
		{
			SelectAll(FALSE);
			SelectInsideArea(fminx, fminy, fmaxx, fmaxy, m_nDiagram);
			ToggleSelection();
		}
		else
		{
			PcbSelectAll(FALSE);
			PcbSelectInsideArea(fminx, fminy, fmaxx, fmaxy);
			PcbToggleSelection();
		}
	}
	
	Invalidate();

	dc.SetROP2(drawmode);

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnEditDeselectAll()
{
	if(m_nSystem_State == SCH) SelectAll(FALSE);
	else PcbSelectAll(FALSE);
	
	Invalidate();
}

void CESDView::OnEditSelectAll()
{
	if(m_nSystem_State == SCH) SelectAll(TRUE);
	else PcbSelectAll(TRUE);
	
	Invalidate();
}

void CESDView::OnEditDeselectInside()
{
	int ch1;
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	Write_Help("定义区域一角");
	flag = 0;

	int drawmode;
	drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	fx1 = fy1 = fx2 = fy2 = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			fx1 = m_fCurrentSnapX; fy1 = m_fCurrentSnapY;
			Get_VPort_XY(&xs,&ys,fx1,fy1);
			xe=xs;
			ye=ys;
			Write_Help("定义区域另一角");
			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,fx1,fy1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto LABEL;

				}
				if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{ 
						flag=1;
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
					else
					{
						if(nEDA_RedrawFlag == ON){ nEDA_RedrawFlag = OFF;}
						else 
						{ 
							dc.MoveTo(xs,ys);
							dc.LineTo(xe,ys);
							dc.LineTo(xe,ye);
							dc.LineTo(xs,ye);
							dc.LineTo(xs,ys);
						}
						fx2 = m_fCurrentSnapX; fy2 = m_fCurrentSnapY;
						Get_VPort_XY(&xe,&ye,fx2,fy2);
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
				}
				if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{  
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto LABEL;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		else Command_In_Function(ch1);
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

LABEL:
	if((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC)&&(fx1 != fx2)&&(fy1 != fy2))
	{
		double fminx, fminy, fmaxx, fmaxy;
		fminx = MIN(fx1, fx2);
		fmaxx = MAX(fx1, fx2);
		fminy = MIN(fy1, fy2);
		fmaxy = MAX(fy1, fy2);
	
		if(m_nSystem_State == SCH)	DeSelectInsideArea(fminx, fminy, fmaxx, fmaxy, m_nDiagram);	
		else PcbDeSelectInsideArea(fminx, fminy, fmaxx, fmaxy);	
	}
	
	Invalidate();

	dc.SetROP2(drawmode);

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnEditDeselectOutside()
{
	int ch1;
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	Write_Help("定义区域一角");
	flag = 0;

	int drawmode;
	drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	fx1 = fy1 = fx2 = fy2 = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			fx1 = m_fCurrentSnapX; fy1 = m_fCurrentSnapY;
			Get_VPort_XY(&xs,&ys,fx1,fy1);
			xe=xs;
			ye=ys;
			Write_Help("定义区域另一角");
			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,fx1,fy1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto LABEL;

				}
				if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{ 
						flag=1;
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
					else
					{
						if(nEDA_RedrawFlag == ON){ nEDA_RedrawFlag = OFF;}
						else 
						{ 
							dc.MoveTo(xs,ys);
							dc.LineTo(xe,ys);
							dc.LineTo(xe,ye);
							dc.LineTo(xs,ye);
							dc.LineTo(xs,ys);
						}
						fx2 = m_fCurrentSnapX; fy2 = m_fCurrentSnapY;
						Get_VPort_XY(&xe,&ye,fx2,fy2);
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
				}
				if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{  
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto LABEL;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		else Command_In_Function(ch1);
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

LABEL:
	if((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC)&&(fx1 != fx2)&&(fy1 != fy2))
	{
		double fminx, fminy, fmaxx, fmaxy;
		fminx = MIN(fx1, fx2);
		fmaxx = MAX(fx1, fx2);
		fminy = MIN(fy1, fy2);
		fmaxy = MAX(fy1, fy2);
	
		if(m_nSystem_State == SCH)	DeSelectOutsideArea(fminx, fminy, fmaxx, fmaxy, m_nDiagram);	
		else PcbDeSelectOutsideArea(fminx, fminy, fmaxx, fmaxy);	
	}
	
	Invalidate();

	dc.SetROP2(drawmode);

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnEditSelectionToggle()
{
	unsigned ch1;
	char szHelpText[] = "变换选择状态: <左键><回车>选择对象, <右键><ESC>结束命令";

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);
	Write_Help(szHelpText);

	if(m_nSystem_State == SCH)
	{
		do
		{
			ch1 = MouseKey();
			if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
			{
					Struct_Multi_Selection multi_selection = SelectSCHObject(m_fCurrentX, m_fCurrentY, MULTI_SELECTION);
					if((multi_selection.element == 0)||( multi_selection.pos == NULL)) continue;

					CObject* pobject;
					pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(multi_selection.pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						COb_schcomp* pcomp = (COb_schcomp*)pobject;
						if(pcomp->m_bSelection == FALSE) pcomp->m_bSelection = TRUE;
						else pcomp->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
					{
						COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
						if(psheetsymbol->m_bSelection == FALSE) psheetsymbol->m_bSelection = TRUE;
						else psheetsymbol->m_bSelection = FALSE;
					}			
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
					{
						COb_schline* pline = (COb_schline*)pobject;
						if(pline->m_bSelection == FALSE) pline->m_bSelection = TRUE;
						else pline->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
					{
						COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
						if(ppolyline->m_bSelection == FALSE) ppolyline->m_bSelection = TRUE;
						else ppolyline->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
					{
						COb_schjunc* pjunc = (COb_schjunc*)pobject;
						if(pjunc->m_bSelection == FALSE) pjunc->m_bSelection = TRUE;
						else pjunc->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						if(ptext->m_bSelection == FALSE) ptext->m_bSelection = TRUE;
						else ptext->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
					{
						COb_scharc* parc = (COb_scharc*)pobject;
						if(parc->m_bSelection == FALSE) parc->m_bSelection = TRUE;
						else parc->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
					{
						COb_schellipse* pellipse = (COb_schellipse*)pobject;
						if(pellipse->m_bSelection == FALSE) pellipse->m_bSelection = TRUE;
						else pellipse->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
					{
						COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
						if(pellipsefill->m_bSelection == FALSE) pellipsefill->m_bSelection = TRUE;
						else pellipsefill->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
					{
						COb_schpie* ppie = (COb_schpie*)pobject;
						if(ppie->m_bSelection == FALSE) ppie->m_bSelection = TRUE;
						else ppie->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
					{
						COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
						if(ppolygon->m_bSelection == FALSE) ppolygon->m_bSelection = TRUE;
						else ppolygon->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
					{
						COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
						if(ptextframe->m_bSelection == FALSE) ptextframe->m_bSelection = TRUE;
						else ptextframe->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
					{
						COb_schrect* prect = (COb_schrect*)pobject;
						if(prect->m_bSelection == FALSE) prect->m_bSelection = TRUE;
						else prect->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
					{
						COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
						if(proundrect->m_bSelection == FALSE) proundrect->m_bSelection = TRUE;
						else proundrect->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
					{
						COb_schport* pport = (COb_schport*)pobject;
						if(pport->m_bSelection == FALSE) pport->m_bSelection = TRUE;
						else pport->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
					{
						COb_schpower* ppower = (COb_schpower*)pobject;
						if(ppower->m_bSelection == FALSE) ppower->m_bSelection = TRUE;
						else ppower->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
					{
						COb_schbezier* pbezier = (COb_schbezier*)pobject;
						if(pbezier->m_bSelection == FALSE) pbezier->m_bSelection = TRUE;
						else pbezier->m_bSelection = FALSE;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
					{
						COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
						if(pgraphic->m_bSelection == FALSE) pgraphic->m_bSelection = TRUE;
						else pgraphic->m_bSelection = FALSE;
					}

					Write_Help(szHelpText);
					Invalidate();
			}
		}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
		
		pDoc->m_fpSelection[m_nDiagram] = GetSelectionCenter();
	}
	else
	{
		do
		{
			ch1 = MouseKey();
			if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
			{
				Struct_Pcb_Multi_Selection selected_element = SelectPcbObject(m_fCurrentX, m_fCurrentY, NOT_INCLUDE_CPAD);
				if((selected_element.pcbelement == 0)||( selected_element.index < 0)) continue;

				if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
					if(ptrack->m_bSelection == TRUE) ptrack->m_bSelection = FALSE;
					else ptrack->m_bSelection = TRUE;
				}
				else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
					if(parc->m_bSelection == TRUE) parc->m_bSelection = FALSE;
					else parc->m_bSelection = TRUE;					
				}
				else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
					if(ppad->m_bSelection == TRUE) ppad->m_bSelection = FALSE;
					else ppad->m_bSelection = TRUE;						
				}	
				else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
				{
					COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
					if(pvia->m_bSelection == TRUE) pvia->m_bSelection = FALSE;
					else pvia->m_bSelection = TRUE;						
				}		
				else if(selected_element.pcbelement == PCB_ELEMENT_TEXT)
				{
					COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(selected_element.index);
					if(ptext->m_bSelection == TRUE) ptext->m_bSelection = FALSE;
					else ptext->m_bSelection = TRUE;						
				}		
				else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
				{
					COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
					if(pfill->m_bSelection == TRUE) pfill->m_bSelection = FALSE;
					else pfill->m_bSelection = TRUE;						
				}	
				else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
					if(pregion->m_bSelection == TRUE) pregion->m_bSelection = FALSE;
					else pregion->m_bSelection = TRUE;						
				}	
				else if(selected_element.pcbelement == PCB_ELEMENT_COMP)
				{
					COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(selected_element.index);
					if(pcomp->m_bSelection == TRUE) pcomp->m_bSelection = FALSE;
					else pcomp->m_bSelection = TRUE;						
				}	
				else if(selected_element.pcbelement == PCB_ELEMENT_POLYGON)
				{
					COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(selected_element.index);
					if(ppolygon->m_bSelection == TRUE) ppolygon->m_bSelection = FALSE;
					else ppolygon->m_bSelection = TRUE;						
				}	
				else if(selected_element.pcbelement == PCB_ELEMENT_COORDINATE)
				{
					COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(selected_element.index);
					if(pcoordinate->m_bSelection == TRUE) pcoordinate->m_bSelection = FALSE;
					else pcoordinate->m_bSelection = TRUE;					
				}	
				else if(selected_element.pcbelement == PCB_ELEMENT_DIMENSION)
				{
					COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(selected_element.index);
					if(pdimension->m_bSelection == TRUE) pdimension->m_bSelection = FALSE;
					else pdimension->m_bSelection = TRUE;						
				}

				Write_Help(szHelpText);
				Invalidate();
			}
		}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
		
		pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
	}

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}



#include "ESDView_Change.cpp"
#include "ESDView_Delete.cpp"
#include "ESDView_Move.cpp"
#include "ESDView_Place.cpp"
#include "ESDView_Select.cpp"
#include "ESDView_Zoom.cpp"
#include "ESDView_SchLib.cpp"
#include "ESDView_Others1.cpp"
#include "ESDView_sch_cut_copy_paste.cpp"
#include "ESDView_Auto.cpp"
//----------- PCB
#include "ESDView_PCB_Move.cpp"
#include "ESDView_PCB_Place.cpp"
#include "ESDView_Pcb_Select.cpp"
#include "ESDView_Pcb_Auto1.cpp"
#include "ESDView_Pcb_lib.cpp"
#include "ESDView_pcb_cut_copy_paste.cpp"
#include "ESDView_pcb_route.cpp"
#include "ESDView_pcb_unroute.cpp"
#include "ESDView_pcb_highlight.cpp"
#include "ESDView_pcb_drc.cpp"
#include "ESDView_pcb_pour.cpp"
#include "ESDView_pcb_design.cpp"

void CESDView::OnPcbToolExplodeComponent()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	unsigned ch1;
	char szHelpText[] = "分解元件: <左键><回车>选择元件, <右键><ESC>结束命令";
	MoveCursorToCenter();

	Write_Help(szHelpText);
	do{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			Struct_Pcb_Multi_Selection multi_selection = SelectPcbObject(m_fCurrentX, m_fCurrentY, NOT_INCLUDE_CPAD);
			if(multi_selection.pcbelement == PCB_ELEMENT_COMP)
			{
				if(AfxMessageBox("这个元件将被分解, 确定?", MB_YESNO|MB_ICONWARNING) != IDYES) break;
				int compindex = multi_selection.index;
				COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(compindex);
				pcomp->m_bSelection = FALSE;
				pcomp->m_nFlag = FLAG_DELETED;

				if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = PCB_ELEMENT_COMP;
				pundo->m_nIndex = compindex;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;

				
				for(int subindex=0; subindex<pDoc->m_arrayPcbTrack.GetCount(); subindex++)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(subindex);
					if(ptrack->m_nComponent == compindex)
					{
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = subindex;
						pundo->pcbtrack.Copy(ptrack);
							
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

						ptrack->m_nComponent = -1;
					}
				}
				for(int subindex=0; subindex<pDoc->m_arrayPcbArc.GetCount(); subindex++)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(subindex);
					if(parc->m_nComponent == compindex)
					{
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = PCB_ELEMENT_ARC;
						pundo->m_nIndex = subindex;
						pundo->pcbarc.Copy(parc);
							
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

						parc->m_nComponent = -1;
					}
				}
				for(int subindex=0; subindex<pDoc->m_arrayPcbText.GetCount(); subindex++)
				{
					COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(subindex);
					if(ptext->m_nComponent == compindex)
					{
						if((ptext->m_bDesignator == TRUE)||(ptext->m_bComment == TRUE))
						{
							ptext->m_bSelection = FALSE;
							ptext->m_nFlag = FLAG_DELETED;
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_UNDELETE;
							pundo->m_nOb = PCB_ELEMENT_TEXT;
							pundo->m_nIndex = subindex;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						}
						else
						{
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = PCB_ELEMENT_TEXT;
							pundo->m_nIndex = subindex;
							pundo->pcbtext.Copy(ptext);
								
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

							ptext->m_nComponent = -1;
						}

					}
				}
				for(int subindex=0; subindex<pDoc->m_arrayPcbFill.GetCount(); subindex++)
				{
					COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(subindex);
					if(pfill->m_nComponent == compindex)
					{
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = PCB_ELEMENT_FILL;
						pundo->m_nIndex = subindex;
						pundo->pcbfill.Copy(pfill);
							
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

						pfill->m_nComponent = -1;
					}
				}
				for(int subindex=0; subindex<pDoc->m_arrayPcbPad.GetCount(); subindex++)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(subindex);
					if(ppad->m_nComponent == compindex)
					{
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = PCB_ELEMENT_PAD;
						pundo->m_nIndex = subindex;
						pundo->pcbpad.Copy(ppad);
							
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						ppad->m_nComponent = -1;
					}
				}
				for(int subindex=0; subindex<pDoc->m_arrayPcbVia.GetCount(); subindex++)
				{
					COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(subindex);
					if(pvia->m_nComponent == compindex)
					{
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = PCB_ELEMENT_VIA;
						pundo->m_nIndex = subindex;
						pundo->pcbvia.Copy(pvia);
							
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

						pvia->m_nComponent = -1;
					}
				}
			}
			Write_Help(szHelpText);
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
	
	Write_Help(szEDA_Help_Ready);
	
	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnPcbToolExplodePolygon()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	unsigned ch1;
	char szHelpText[] = "分解元件: <左键><回车>选择元件, <右键><ESC>结束命令";
	MoveCursorToCenter();

	Write_Help(szHelpText);
	do{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			Struct_Pcb_Multi_Selection multi_selection = SelectPcbObject(m_fCurrentX, m_fCurrentY, NOT_INCLUDE_CPAD);
			if(multi_selection.pcbelement == PCB_ELEMENT_POLYGON)
			{
				if(AfxMessageBox("这个覆铜区将被分解, 确定?", MB_YESNO|MB_ICONWARNING) != IDYES) break;

				int polygonindex = multi_selection.index;
				COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(polygonindex);
				ppolygon->m_bSelection = FALSE;
				ppolygon->m_nFlag = FLAG_DELETED;
				if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = PCB_ELEMENT_POLYGON;
				pundo->m_nIndex = polygonindex;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;

				
				for(int subindex=0; subindex<pDoc->m_arrayPcbTrack.GetCount(); subindex++)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(subindex);
					if(ptrack->m_nPolygon == polygonindex)
					{
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = subindex;
						pundo->pcbtrack.Copy(ptrack);
							
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
						ptrack->m_nPolygon = -1;
						ptrack->m_bPolygonOutline = FALSE;
						ptrack->m_nSubPolyIndex = -1;
					}
				}
				for(int subindex=0; subindex<pDoc->m_arrayPcbArc.GetCount(); subindex++)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(subindex);
					if(parc->m_nPolygon == polygonindex)
					{
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = PCB_ELEMENT_ARC;
						pundo->m_nIndex = subindex;
						pundo->pcbarc.Copy(parc);
							
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
						parc->m_nPolygon = -1;
						parc->m_bPolygonOutline = FALSE;
						parc->m_nSubPolyIndex = -1;
					}
				}
				for(int subindex=0; subindex<pDoc->m_arrayPcbRegion.GetCount(); subindex++)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(subindex);
					if(pregion->m_nPolygon == polygonindex)
					{
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = PCB_ELEMENT_REGION;
						pundo->m_nIndex = subindex;
						pundo->pcbregion.Copy(pregion);
							
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
						pregion->m_nPolygon = -1;
						pregion->m_nSubPolyIndex = -1;
					}
				}
			}
			Write_Help(szHelpText);
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
	
	Write_Help(szEDA_Help_Ready);
	
	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnPcbToolCreateComponent()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
		
	
	int flag = 0;
	for(int i=0; i<pDoc->m_arrayPcbSelection.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element = pDoc->m_arrayPcbSelection.GetAt(i);

		if((selected_element.pcbelement == 0)||( selected_element.index < 0)) continue;
		if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
			if(ptrack->m_nComponent >= 0) continue;
			flag = 1;
			break;
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
			if(parc->m_nComponent >= 0) continue;
			flag = 1;
			break;
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
			if(pfill->m_nComponent >= 0) continue;
			flag = 1;
			break;
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_TEXT)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(selected_element.index);
			if(ptext->m_nComponent >= 0) continue;
			flag = 1;
			break;
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
			if(ppad->m_nComponent >= 0) continue;
			flag = 1;
			break;
		}			
		else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
			if(pvia->m_nComponent >= 0) continue;
			flag = 1;
			break;
		}
	}

	if(flag == 0) 
	{
		AfxMessageBox("先选中组成元件的的图形对象(焊盘/连线/圆弧/过孔/文字/矩形)。");
		return;
	}

	unsigned ch1;
	CPcbUndo *pundo;
	char szHelpText[] = "设置元件基准参考点: <左键><回车>设置, <右键><ESC>取消";

	double ref_x, ref_y;
	ref_x = -10000;
	ref_y = -10000;

	Write_Help(szHelpText);
	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
		   ref_x = m_fCurrentSnapX;
		   ref_y = m_fCurrentSnapY;
		   break;
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
	Write_Help(szEDA_Help_Ready);

	if((ref_x >= 0)&&(ref_y >= 0)) 
	{
		
		COb_pcbcomp* pcomp = new COb_pcbcomp();
		pcomp->pDocument = pDoc;
		pcomp->m_fX = ref_x;
		pcomp->m_fY = ref_y;
		pcomp->m_fRotation = 0;
		pcomp->m_fHeight = 50;

		pcomp->m_nLayer = TOPLAYER;

		pcomp->m_bNameOn = TRUE;
		pcomp->m_nNamePosition = MANUAL;
		pcomp->m_bCommentOn = TRUE;
		pcomp->m_nCommentPosition = MANUAL;

		pcomp->m_csPattern.Empty();
		pcomp->m_csFootprintDescription.Empty();

		
		int index = pDoc->m_arrayPcbComp.Add(pcomp);
		
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_DELETE;
		pundo->m_nOb = PCB_ELEMENT_COMP;
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		

		
		COb_pcbtext* ptext = new COb_pcbtext();
		ptext->pDocument = pDoc;
		ptext->m_nComponent = index;
		ptext->m_nLayer = TOPOVERLAY;
		ptext->m_fSx = pcomp->m_fX;
		ptext->m_fSy = pcomp->m_fY + 100;
		ptext->m_bComment = FALSE;
		ptext->m_bDesignator = TRUE;

		ptext->m_fRotation = 0;
		ptext->m_csText = "ID";
		ptext->m_fHeight = 60;
		ptext->m_fWidth = 10;
		ptext->m_bMirror = FALSE;
		ptext->m_nStrokeFont = 0;
		ptext->m_bUseTTFont = FALSE;
		ptext->m_bBold = FALSE;
		ptext->m_bItalic = FALSE;;
		ptext->m_bInverted = FALSE;
		ptext->m_csTTFontName = "Arial";
		ptext->m_fInvertedBorder = FALSE;

		int subindex = pDoc->m_arrayPcbText.Add(ptext);
		ptext->UpdateStringBorder(this, TRUE);
		
		pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_DELETE;
		pundo->m_nOb = PCB_ELEMENT_TEXT;
		pundo->m_nIndex = subindex;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		

		
		ptext = new COb_pcbtext();
		ptext->pDocument = pDoc;
		ptext->m_nComponent = index;
		ptext->m_nLayer = TOPOVERLAY;
		ptext->m_fSx = pcomp->m_fX;
		ptext->m_fSy = pcomp->m_fY;
		ptext->m_bComment = TRUE;
		ptext->m_bDesignator = FALSE;

		ptext->m_fRotation = 0;
		ptext->m_csText = "Comment";
		ptext->m_fHeight = 60;
		ptext->m_fWidth = 10;
		ptext->m_bMirror = FALSE;
		ptext->m_nStrokeFont = 0;
		ptext->m_bUseTTFont = FALSE;
		ptext->m_bBold = FALSE;
		ptext->m_bItalic = FALSE;;
		ptext->m_bInverted = FALSE;
		ptext->m_csTTFontName = "Arial";
		ptext->m_fInvertedBorder = FALSE;

		subindex = pDoc->m_arrayPcbText.Add(ptext);
		ptext->UpdateStringBorder(this, TRUE);
		
		pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_DELETE;
		pundo->m_nOb = PCB_ELEMENT_TEXT;
		pundo->m_nIndex = subindex;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		

		for(int i=0; i<pDoc->m_arrayPcbSelection.GetCount(); i++)
		{
			Struct_Pcb_Multi_Selection selected_element = pDoc->m_arrayPcbSelection.GetAt(i);

			if((selected_element.pcbelement == 0)||( selected_element.index < 0)) continue;
			if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
				if(ptrack->m_nComponent >= 0) continue;
				
				pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = PCB_ELEMENT_TRACK;
				pundo->m_nIndex = selected_element.index;
				pundo->pcbtrack.Copy(ptrack);
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				

				ptrack->m_nComponent = index;
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
				if(parc->m_nComponent >= 0) continue;
				
				pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = PCB_ELEMENT_ARC;
				pundo->m_nIndex = selected_element.index;
				pundo->pcbarc.Copy(parc);
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				

				parc->m_nComponent = index;
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
				if(pfill->m_nComponent >= 0) continue;
				
				pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = PCB_ELEMENT_FILL;
				pundo->m_nIndex = selected_element.index;
				pundo->pcbfill.Copy(pfill);
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				

				pfill->m_nComponent = index;
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_TEXT)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(selected_element.index);
				if(ptext->m_nComponent >= 0) continue;
				
				pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = PCB_ELEMENT_TEXT;
				pundo->m_nIndex = selected_element.index;
				pundo->pcbtext.Copy(ptext);
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				

				ptext->m_nComponent = index;
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
				if(ppad->m_nComponent >= 0) continue;
				
				pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = PCB_ELEMENT_PAD;
				pundo->m_nIndex = selected_element.index;
				pundo->pcbpad.Copy(ppad);
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				

				ppad->m_nComponent = index;
			}			
			else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
				if(pvia->m_nComponent >= 0) continue;
				
				pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = PCB_ELEMENT_VIA;
				pundo->m_nIndex = selected_element.index;
				pundo->pcbvia.Copy(pvia);
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				

				pvia->m_nComponent = index;
			}
		}

		
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		
	}
}

void CESDView::OnPcbToolPerference()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	CDlg_PcbPerference dlg;
	dlg.m_bCheckOnlineDrc = bEDA_PcbOnLineDRCEnable;
	dlg.m_bCheckRatsnestEnable = bEDA_PcbConnectionAutoRefresh;
	dlg.m_bCheckPolygonHighlight = bEDA_PcbPolygonHighlight;

	dlg.m_nEditNumber = nEDA_SchRepeatNumber;
	dlg.m_nEditNDelta = nEDA_SchRepeatNumberDelta;
	dlg.m_csEditXDelta = pDoc->Convert_XY_To_String(fEDA_SchRepeatDeltaX);
	dlg.m_csEditYDelta = pDoc->Convert_XY_To_String(fEDA_SchRepeatDeltaY);

	dlg.m_csEditMaxRadius = pDoc->Convert_XY_To_String(fEDA_AutoArcMaxRadius);
	dlg.m_nEditUndoBuffer = nEDA_PCB_Undo_Number;
	dlg.m_csEditLoadPackageX = pDoc->Convert_XY_To_String(fEDA_LoadPackage_X);
	dlg.m_csEditLoadPackageY = pDoc->Convert_XY_To_String(fEDA_LoadPackage_Y);

	dlg.m_bCheckFileImport = bEDA_ConvertChineseLayerName;
	dlg.m_bCheckFileExport = bEDA_ConvertEnglishLayerName;

	if(dlg.DoModal() == TRUE)
	{
		bEDA_PcbOnLineDRCEnable = dlg.m_bCheckOnlineDrc;
		bEDA_PcbConnectionAutoRefresh = dlg.m_bCheckRatsnestEnable;
		bEDA_PcbPolygonHighlight = dlg.m_bCheckPolygonHighlight;

		nEDA_SchRepeatNumber = dlg.m_nEditNumber;
		nEDA_SchRepeatNumberDelta = dlg.m_nEditNDelta;
		fEDA_SchRepeatDeltaX = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditXDelta);
		fEDA_SchRepeatDeltaY = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditYDelta);

		fEDA_AutoArcMaxRadius = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditMaxRadius);
		if(fEDA_AutoArcMaxRadius < 100) fEDA_AutoArcMaxRadius = 100;
		nEDA_PCB_Undo_Number = dlg.m_nEditUndoBuffer;
		if(nEDA_PCB_Undo_Number < 30) nEDA_PCB_Undo_Number = 30;
		fEDA_LoadPackage_X = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditLoadPackageX);
		if(fEDA_LoadPackage_X < 0) fEDA_LoadPackage_X = 0;
		if(fEDA_LoadPackage_X > 100000) fEDA_LoadPackage_X = 100000;
		fEDA_LoadPackage_Y = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditLoadPackageY);
		if(fEDA_LoadPackage_Y < 0) fEDA_LoadPackage_Y = 0;
		if(fEDA_LoadPackage_Y > 100000) fEDA_LoadPackage_Y = 100000;

		bEDA_ConvertChineseLayerName = dlg.m_bCheckFileImport;
		bEDA_ConvertEnglishLayerName = dlg.m_bCheckFileExport;
	}
}

void CESDView::Refresh_Ratsnest()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	
	while (!pDoc->m_arrayPcbConnection.IsEmpty())
	{
		delete pDoc->m_arrayPcbConnection.GetAt(0);
		pDoc->m_arrayPcbConnection.RemoveAt(0);
	}

	
	for(int nNet=0; nNet<pDoc->m_arrayPcbNet.GetCount(); nNet++)
	{
		Refresh_Net_Ratsnest(nNet);
	}
}

void CESDView::Refresh_Package_Ratsnest(int index)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CArray<int, int> arrayUsedNet;
	
	
	while (!pDoc->m_arrayPcbConnection.IsEmpty())
	{
		delete pDoc->m_arrayPcbConnection.GetAt(0);
		pDoc->m_arrayPcbConnection.RemoveAt(0);
	}

	COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(index);
	for(int padNum=0; padNum<pDoc->m_arrayPcbPad.GetCount(); padNum++)
	{
		COb_pcbpad* pcomppad = pDoc->m_arrayPcbPad.GetAt(padNum);
		if(pcomppad->m_nComponent != index) continue;		

		int nNet = pcomppad->m_nNet;
		if(nNet >= 0)
		{
			COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(nNet);
			if((pnet->m_nFlag != FLAG_NORMAL)||(pnet->m_bVisible == FALSE)) continue;	
			

			
			CArray<Struct_Connection_Point, Struct_Connection_Point&> objectArray;
			Struct_Connection_Point apoint;

			for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
				if((ppad->m_nNet == nNet)&&(ppad->m_nFlag != FLAG_DELETED)&&(ppad->m_nComponent >= 0)&&(ppad->m_nComponent != index))	
				{
					COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
					if((pcomp->m_fX != fEDA_LoadPackage_X)||(pcomp->m_fY != fEDA_LoadPackage_Y))
					{
						apoint.fx = ppad->m_fCx;
						apoint.fy = ppad->m_fCy;
						apoint.nLayer = ppad->m_nLayer;
						objectArray.Add(apoint);
					}
				}
			}

			
			double dis = 100000;
			Struct_Connection_Point apoint1, apoint2, apoint1_bak;

			apoint2.fx = pcomppad->m_fCx;
			apoint2.fy = pcomppad->m_fCy;
			apoint2.nLayer = pcomppad->m_nLayer;
			for(int i=0; i<objectArray.GetCount(); i++)
			{
				apoint1 = objectArray.GetAt(i);
				double distance;
				distance = sqrt(abs(apoint2.fx - apoint1.fx)*abs(apoint2.fx - apoint1.fx) + abs(apoint2.fy - apoint1.fy)*abs(apoint2.fy - apoint1.fy));
				if(distance < dis)
				{
					
					dis = distance;
					apoint1_bak.fx = apoint1.fx;
					apoint1_bak.fy = apoint1.fy;
					apoint1_bak.nLayer = apoint1.nLayer;
				}
			}
				
			if((dis < 100000)&&(dis >= 0))
			{
				COb_pcbconnection* pconnection = new COb_pcbconnection();
				pDoc->m_arrayPcbConnection.Add(pconnection);
					
				pconnection->m_fX1 = apoint1_bak.fx;
				pconnection->m_fY1 = apoint1_bak.fy;
				pconnection->m_nLayer1 = apoint1_bak.nLayer;
				pconnection->m_fX2 = apoint2.fx;
				pconnection->m_fY2 = apoint2.fy;
				pconnection->m_nLayer2 = apoint2.nLayer;
				pconnection->m_nNet = nNet;
			}
		}
	}
}

void CESDView::Refresh_Net_Ratsnest(int nNet)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(nNet);
	if((pnet->m_nFlag != FLAG_NORMAL)||(pnet->m_bVisible == FALSE)) return;

	
	for(int i=0; i<pDoc->m_arrayPcbConnection.GetCount(); i++)
	{
		COb_pcbconnection* pconnection = pDoc->m_arrayPcbConnection.GetAt(i);
		if(pconnection->m_nNet == nNet)
		{
			pDoc->m_arrayPcbConnection.RemoveAt(i);
			i--;
			delete pconnection;
		}
	}

	Struct_Connection_Point apoint;
	CTypedPtrArray<CPtrArray,  CArray<Struct_Connection_Point, Struct_Connection_Point&>*> arrayGroups;

	char s[1000];
	sprintf(s, "网络: %s", pnet->m_csNetName);
	Write_Info(s);
	for(;;)
	{
		m_arrayPcbHighlight.RemoveAll();
		
		Struct_Pcb_Multi_Selection  connection_object;
		for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
		{
			COb_pcbarc* ppcbarc = pDoc->m_arrayPcbArc.GetAt(i);
			if( (ppcbarc->m_nPolygon == -1)&&(ppcbarc->m_nComponent == -1)&&(ppcbarc->m_nNet == nNet)&&\
				(ppcbarc->m_nFlag == FLAG_NORMAL)&&(pDoc->IsOnSignalLayer(ppcbarc->m_nLayer) == TRUE) )
			{
				connection_object.pcbelement = PCB_ELEMENT_ARC;
				connection_object.index = i;
				ppcbarc->m_nFlag = FLAG_PROCESSING;
				m_arrayPcbHighlight.Add(connection_object);
				goto LOCAL_LABEL_RATS;
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
			if( (ptrack->m_nPolygon == -1)&&(ptrack->m_nComponent == -1)&&(ptrack->m_nNet == nNet)&&\
				(ptrack->m_nFlag == FLAG_NORMAL)&&(pDoc->IsOnSignalLayer(ptrack->m_nLayer) == TRUE) )
			{
				connection_object.pcbelement = PCB_ELEMENT_TRACK;
				connection_object.index = i;
				ptrack->m_nFlag = FLAG_PROCESSING;
				m_arrayPcbHighlight.Add(connection_object);
				goto LOCAL_LABEL_RATS;
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
			if((pvia->m_nNet == nNet)&&(pvia->m_nFlag == FLAG_NORMAL))
			{
				connection_object.pcbelement = PCB_ELEMENT_VIA;
				connection_object.index = i;
				pvia->m_nFlag = FLAG_PROCESSING;
				m_arrayPcbHighlight.Add(connection_object);
				goto LOCAL_LABEL_RATS;
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
		{
			COb_pcbfill* ppcbfill = pDoc->m_arrayPcbFill.GetAt(i);
			if( (ppcbfill->m_nComponent == -1)&&(ppcbfill->m_nNet == nNet)&&\
				(ppcbfill->m_nFlag == FLAG_NORMAL)&&(pDoc->IsOnSignalLayer(ppcbfill->m_nLayer) == TRUE) )
			{
				connection_object.pcbelement = PCB_ELEMENT_FILL;
				connection_object.index = i;
				ppcbfill->m_nFlag = FLAG_PROCESSING;
				m_arrayPcbHighlight.Add(connection_object);
				goto LOCAL_LABEL_RATS;
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
			if((ppad->m_nNet == nNet)&&(ppad->m_nFlag == FLAG_NORMAL)&&(pDoc->IsOnSignalLayer(ppad->m_nLayer) == TRUE))
			{
				if(ppad->m_nComponent >= 0)
				{
					COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
					if((pcomp->m_fX == fEDA_LoadPackage_X)&&(pcomp->m_fY == fEDA_LoadPackage_Y)) continue;
				}
				connection_object.pcbelement = PCB_ELEMENT_PAD;
				connection_object.index = i;
				ppad->m_nFlag = FLAG_PROCESSING;
				m_arrayPcbHighlight.Add(connection_object);
				goto LOCAL_LABEL_RATS;
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
			if( (pregion->m_nPolygon == -1)&&(pregion->m_nComponent == -1)&&(pregion->m_nNet == nNet)&&\
				(pregion->m_nFlag == FLAG_NORMAL)&&(pDoc->IsOnSignalLayer(pregion->m_nLayer) == TRUE))
			{
				connection_object.pcbelement = PCB_ELEMENT_REGION;
				connection_object.index = i;
				pregion->m_nFlag = FLAG_PROCESSING;
				m_arrayPcbHighlight.Add(connection_object);
				goto LOCAL_LABEL_RATS;
			}
		}

LOCAL_LABEL_RATS:

		if(m_arrayPcbHighlight.GetCount() > 0)
		{
			PCBMakeHighlight(FALSE);
			
			CArray<Struct_Connection_Point, Struct_Connection_Point&>* pArray = new CArray<Struct_Connection_Point, Struct_Connection_Point&>;
			for(int i=0; i<m_arrayPcbHighlight.GetCount(); i++)
			{
				Struct_Pcb_Multi_Selection connection_object;
				connection_object = m_arrayPcbHighlight.GetAt(i);
				if(connection_object.pcbelement == PCB_ELEMENT_TRACK)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(connection_object.index);
					apoint.fx = ptrack->m_fX1;
					apoint.fy = ptrack->m_fY1;
					apoint.nLayer = ptrack->m_nLayer;
					pArray->Add(apoint);
					apoint.fx = ptrack->m_fX2;
					apoint.fy = ptrack->m_fY2;
					apoint.nLayer = ptrack->m_nLayer;
					pArray->Add(apoint);
				}
				else if(connection_object.pcbelement == PCB_ELEMENT_ARC)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(connection_object.index);
					apoint.fx = parc->m_fCx + parc->m_fRadius*cos(parc->m_fSangle*fEDA_pi/180);
					apoint.fy = parc->m_fCy + parc->m_fRadius*sin(parc->m_fSangle*fEDA_pi/180);
					apoint.nLayer = parc->m_nLayer;
					pArray->Add(apoint);
					apoint.fx = parc->m_fCx + parc->m_fRadius*cos(parc->m_fEangle*fEDA_pi/180);
					apoint.fy = parc->m_fCy + parc->m_fRadius*sin(parc->m_fEangle*fEDA_pi/180);
					apoint.nLayer = parc->m_nLayer;
					pArray->Add(apoint);
				}
				else if(connection_object.pcbelement == PCB_ELEMENT_PAD)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(connection_object.index);
					apoint.fx = ppad->m_fCx;
					apoint.fy = ppad->m_fCy;
					apoint.nLayer = ppad->m_nLayer;
					pArray->Add(apoint);
				}	
				else if(connection_object.pcbelement == PCB_ELEMENT_VIA)
				{
					COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(connection_object.index);
					apoint.fx = pvia->m_fCx;
					apoint.fy = pvia->m_fCy;
					apoint.nLayer = pvia->m_nLayer;
					pArray->Add(apoint);
				}		
				else if(connection_object.pcbelement == PCB_ELEMENT_FILL)
				{
					COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(connection_object.index);
					double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
					pfill->GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);
					apoint.fx = fx1;
					apoint.fy = fy1;
					apoint.nLayer = pfill->m_nLayer;
					pArray->Add(apoint);
					apoint.fx = fx2;
					apoint.fy = fy2;
					apoint.nLayer = pfill->m_nLayer;
					pArray->Add(apoint);
					apoint.fx = fx3;
					apoint.fy = fy3;
					apoint.nLayer = pfill->m_nLayer;
					pArray->Add(apoint);
					apoint.fx = fx4;
					apoint.fy = fy4;
					apoint.nLayer = pfill->m_nLayer;
					pArray->Add(apoint);			
				}	
				else if(connection_object.pcbelement == PCB_ELEMENT_REGION)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(connection_object.index);
					for(int j=0; j<pregion->m_cVertex.GetCount(); j++)
					{
						Struct_RegionVertex vertex = pregion->m_cVertex.GetAt(j);
						apoint.fx = vertex.fx;
						apoint.fy = vertex.fy;
						apoint.nLayer = pregion->m_nLayer;
						pArray->Add(apoint);
					}
				}	
			}
			
			
			arrayGroups.Add(pArray);
		}
		else 
		{
			PcbProcessingSelection(FALSE);
			
			for(int groupnum=0; groupnum<arrayGroups.GetCount()-1; groupnum++)
			{
				double dis = 100000;
				Struct_Connection_Point apoint1, apoint2, apoint1_bak, apoint2_bak;

				CArray<Struct_Connection_Point, Struct_Connection_Point&>* pArray2 = arrayGroups.GetAt(groupnum+1);
				for(int i=0; i<=groupnum; i++)
				{
					CArray<Struct_Connection_Point, Struct_Connection_Point&>* pArray1 = arrayGroups.GetAt(i);

					for(int k1=0; k1<pArray1->GetCount(); k1++)
					{
						apoint1 = pArray1->GetAt(k1);
						for(int k2=0; k2<pArray2->GetCount(); k2++)
						{
							apoint2 = pArray2->GetAt(k2);
							double dis_xy;
							dis_xy = abs(apoint2.fx - apoint1.fx) + abs(apoint2.fy - apoint1.fy);
							if(dis_xy < dis)
							{
								
								dis = dis_xy;
								apoint1_bak.fx = apoint1.fx;
								apoint1_bak.fy = apoint1.fy;
								apoint1_bak.nLayer = apoint1.nLayer;
								apoint2_bak.fx = apoint2.fx;
								apoint2_bak.fy = apoint2.fy;
								apoint2_bak.nLayer = apoint2.nLayer;
							}
						}
					}
				}
				
				if((dis < 100000)&&(dis >= 0))
				{
					COb_pcbconnection* pconnection = new COb_pcbconnection();
					pDoc->m_arrayPcbConnection.Add(pconnection);
					
					pconnection->m_fX1 = apoint1_bak.fx;
					pconnection->m_fY1 = apoint1_bak.fy;
					pconnection->m_nLayer1 = apoint1_bak.nLayer;
					pconnection->m_fX2 = apoint2_bak.fx;
					pconnection->m_fY2 = apoint2_bak.fy;
					pconnection->m_nLayer2 = apoint2_bak.nLayer;
					pconnection->m_nNet = nNet;
				}
			}

			
			while (!arrayGroups.IsEmpty())
			{	
				delete (CArray<Struct_Connection_Point, Struct_Connection_Point&>*)arrayGroups.GetAt(0);
				arrayGroups.RemoveAt(0);
			}
			break;
		}
	}

	
}

void CESDView::OnPcbToolRatsRefresh()
{
	Refresh_Ratsnest();

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	int num = pDoc->m_arrayPcbConnection.GetCount();
	char str[100];
	sprintf(str, "剩余飞线连接 = %d", num);
	AfxMessageBox(str);
}

void CESDView::OnPcbLayerSetup()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	
	CDlg_PCBLayerSetup dlg;
	dlg.pDocument = pDoc;
	dlg.m_bCheckMechLayerUseStack = TRUE;
	dlg.m_bCheckPlaneUseStack = TRUE;
	dlg.m_bCheckSingalLayerUseStack = TRUE;

	if(dlg.DoModal() == IDOK)
	{
		CXTPReportRecords* pRecords;
		pRecords = dlg.m_wndSignalCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);

			CString str = pItem->GetValue();
			for(int j=0; j<PCB_LAYER_NUM; j++)
			{
				if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
				{
					nEDA_PCB_LayerColor[j] = pColor->GetBackgroundColor();
					pDoc->m_sPCB_LayerParam[j].bLayerShow = pCheck->IsChecked();
					break;
				}
			}
		}
		
		pRecords = dlg.m_wndPlaneCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);

			CString str = pItem->GetValue();
			for(int j=0; j<PCB_LAYER_NUM; j++)
			{
				if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
				{
					nEDA_PCB_LayerColor[j] = pColor->GetBackgroundColor();
					pDoc->m_sPCB_LayerParam[j].bLayerShow = pCheck->IsChecked();
					break;
				}
			}
		}

		pRecords = dlg.m_wndMechCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);
			CXTPReportRecordItemCheck* pEnable = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(3);
			CXTPReportRecordItemCheck* pLinked = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(4);

			CString str = pItem->GetValue();
			for(int j=0; j<PCB_LAYER_NUM; j++)
			{
				if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
				{
					nEDA_PCB_LayerColor[j] = pColor->GetBackgroundColor();
					pDoc->m_sPCB_LayerParam[j].bLayerShow = pCheck->IsChecked();
					pDoc->m_sPCB_LayerParam[j].bLayerMechEnabled = pEnable->IsChecked();
					pDoc->m_sPCB_LayerParam[j].bLayerSheetLinked = pLinked->IsChecked();

					break;
				}
			}
		}

		pRecords = dlg.m_wndMaskCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);

			CString str = pItem->GetValue();
			for(int j=0; j<PCB_LAYER_NUM; j++)
			{
				if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
				{
					nEDA_PCB_LayerColor[j] = pColor->GetBackgroundColor();
					pDoc->m_sPCB_LayerParam[j].bLayerShow = pCheck->IsChecked();
					break;
				}
			}
		}

		pRecords = dlg.m_wndOtherCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);

			CString str = pItem->GetValue();
			for(int j=0; j<PCB_LAYER_NUM; j++)
			{
				if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
				{
					nEDA_PCB_LayerColor[j] = pColor->GetBackgroundColor();
					pDoc->m_sPCB_LayerParam[j].bLayerShow = pCheck->IsChecked();
					break;
				}
			}
		}

		pRecords = dlg.m_wndSilkCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);

			CString str = pItem->GetValue();
			for(int j=0; j<PCB_LAYER_NUM; j++)
			{
				if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
				{
					nEDA_PCB_LayerColor[j] = pColor->GetBackgroundColor();
					pDoc->m_sPCB_LayerParam[j].bLayerShow = pCheck->IsChecked();
					break;
				}
			}
		}

		pRecords = dlg.m_wndSystemCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecords->GetAt(i)->GetItem(0);
			CXTPReportRecordItemColor* pColor = (CXTPReportRecordItemColor*)pRecords->GetAt(i)->GetItem(1);
			CXTPReportRecordItemCheck* pCheck = (CXTPReportRecordItemCheck*)pRecords->GetAt(i)->GetItem(2);

			CString str = pItem->GetValue();
			if(str == szEDA_HighlightColor) nEDA_PCB_HighlightColor = pColor->GetBackgroundColor();
			else if(str == szEDA_BoardLineColor) nEDA_PCB_BoardLineColor = pColor->GetBackgroundColor();
			else if(str == szEDA_BoardAreaColor) nEDA_PCB_BoardAreaColor = pColor->GetBackgroundColor();
			else if(str == szEDA_SheetLineColor) nEDA_PCB_SheetLineColor = pColor->GetBackgroundColor();
			else if(str == szEDA_SheetAreaColor) nEDA_PCB_SheetAreaColor = pColor->GetBackgroundColor();
			else if(str == szEDA_WordspaceColor) nEDA_PCB_WorkspaceColor = pColor->GetBackgroundColor();
			else
			{
				for(int j=0; j<PCB_LAYER_NUM; j++)
				{
					if(str == pDoc->m_sPCB_LayerParam[j].csLayerName)
					{
						nEDA_PCB_LayerColor[j] = pColor->GetBackgroundColor();
						if(j != SELECTIONS)	pDoc->m_sPCB_LayerParam[j].bLayerShow = pCheck->IsChecked();
						break;
					}
				}
			}
		}

		pDoc->InitLayerShowArray();

		
		if(pDoc->m_sPCB_LayerParam[m_nPCB_Current_Layer].bLayerShow == FALSE)
		{
			if(pDoc->m_arrayPcbLayerShow.IsEmpty() != TRUE)
			{
				m_nPCB_Current_Layer = pDoc->m_arrayPcbLayerShow.GetAt(0);
			}
		}

		CChildFrame* pChildFrame = (CChildFrame*)GetParentFrame();
		pChildFrame->Invalidate();
	}
}

void CESDView::OnPcbEditAlignLeft()
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nComponent != -1)||(ptrack->m_nPolygon != -1)) continue;

		if(ptrack->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = i;
			pundo->pcbtrack.Copy(ptrack);
			pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double x0 = MIN(ptrack->m_fX1, ptrack->m_fX2);
			double deltax = pDoc->m_fPcbSelection_X1 - x0;
			ptrack->m_fX1 += deltax;
			ptrack->m_fX2 += deltax;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nComponent != -1)||(parc->m_nPolygon != -1)) continue;
		if(parc->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_ARC;
			pundo->m_nIndex = i;
			pundo->pcbarc.Copy(parc);
			pundo->pcbarc.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double x0 = parc->m_fCx;
			double deltax = pDoc->m_fPcbSelection_X1 - x0;
			parc->m_fCx += deltax;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_nComponent != -1)) continue;
		if(ptext->m_bSelection == TRUE)
		{			
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TEXT;
			pundo->m_nIndex = i;
			pundo->pcbtext.Copy(ptext);
			pundo->pcbtext.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double x0 = ptext->m_fSx;
			double deltax = pDoc->m_fPcbSelection_X1 - x0;
			ptext->m_fSx += deltax;
			ptext->UpdateStringBorder(this, FALSE);
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent != -1)) continue;
		if(pfill->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_FILL;
			pundo->m_nIndex = i;
			pundo->pcbfill.Copy(pfill);
			pundo->pcbfill.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double x0 = MIN(pfill->m_fX1, pfill->m_fX2);
			double deltax = pDoc->m_fPcbSelection_X1 - x0;
			pfill->m_fX1 += deltax;
			pfill->m_fX2 += deltax;
		}			
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent != -1)) continue;
		if(ppad->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_PAD;
			pundo->m_nIndex = i;
			pundo->pcbpad.Copy(ppad);
			pundo->pcbpad.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double x0 = ppad->m_fCx;
			double deltax = pDoc->m_fPcbSelection_X1 - x0;
			ppad->m_fCx += deltax;
		}				
	}		
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent != -1)) continue;
		if(pvia->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_VIA;
			pundo->m_nIndex = i;
			pundo->pcbvia.Copy(pvia);
			pundo->pcbvia.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double x0 = pvia->m_fCx;
			double deltax = pDoc->m_fPcbSelection_X1 - x0;
			pvia->m_fCx += deltax;
		}				
	}
	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag != FLAG_NORMAL) continue;
		if(pcomp->m_bSelection == TRUE)
		{	
			double x0 = pcomp->m_fX;
			double deltax = pDoc->m_fPcbSelection_X1 - x0;

			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_COMP;
			pundo->m_nIndex = i;
			pundo->pcbcomp.Copy(pcomp);
			pundo->pcbcomp.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			pcomp->m_fX += deltax;

			for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
				if(ptrack->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = j;
					pundo->pcbtrack.Copy(ptrack);
					pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					ptrack->m_fX1 += deltax;
					ptrack->m_fX2 += deltax;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
				if(parc->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = j;
					pundo->pcbarc.Copy(parc);
					pundo->pcbarc.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					parc->m_fCx += deltax;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
				if(pfill->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = j;
					pundo->pcbfill.Copy(pfill);
					pundo->pcbfill.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					pfill->m_fX1 += deltax;
					pfill->m_fX2 += deltax;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
				if(ppad->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_PAD;
					pundo->m_nIndex = j;
					pundo->pcbpad.Copy(ppad);
					pundo->pcbpad.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					ppad->m_fCx += deltax;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
				if(pvia->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = j;
					pundo->pcbvia.Copy(pvia);
					pundo->pcbvia.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					pvia->m_fCx += deltax;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbText.GetCount(); j++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(j);
				if(ptext->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TEXT;
					pundo->m_nIndex = j;
					pundo->pcbtext.Copy(ptext);
					pundo->pcbtext.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					ptext->m_fSx += deltax;
					ptext->UpdateStringBorder(this, FALSE);
				}
			}
		}
	}	

	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}	
	
	Invalidate();
}

void CESDView::OnPcbEditAlignTop()
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nComponent != -1)||(ptrack->m_nPolygon != -1)) continue;

		if(ptrack->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = i;
			pundo->pcbtrack.Copy(ptrack);
			pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double y0 = MIN(ptrack->m_fY1, ptrack->m_fY2);
			double deltay = pDoc->m_fPcbSelection_Y1 - y0;
			ptrack->m_fY1 += deltay;
			ptrack->m_fY2 += deltay;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nComponent != -1)||(parc->m_nPolygon != -1)) continue;
		if(parc->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_ARC;
			pundo->m_nIndex = i;
			pundo->pcbarc.Copy(parc);
			pundo->pcbarc.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double y0 = parc->m_fCy;
			double deltay = pDoc->m_fPcbSelection_Y1 - y0;
			parc->m_fCy += deltay;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_nComponent != -1)) continue;
		if(ptext->m_bSelection == TRUE)
		{			
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TEXT;
			pundo->m_nIndex = i;
			pundo->pcbtext.Copy(ptext);
			pundo->pcbtext.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double y0 = ptext->m_fSy;
			double deltay = pDoc->m_fPcbSelection_Y1 - y0;
			ptext->m_fSy += deltay;
			ptext->UpdateStringBorder(this, FALSE);
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent != -1)) continue;
		if(pfill->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_FILL;
			pundo->m_nIndex = i;
			pundo->pcbfill.Copy(pfill);
			pundo->pcbfill.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double y0 = MIN(pfill->m_fY1, pfill->m_fY2);
			double deltay = pDoc->m_fPcbSelection_Y1 - y0;
			pfill->m_fY1 += deltay;
			pfill->m_fY2 += deltay;
		}			
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent != -1)) continue;
		if(ppad->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_PAD;
			pundo->m_nIndex = i;
			pundo->pcbpad.Copy(ppad);
			pundo->pcbpad.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double y0 = ppad->m_fCy;
			double deltay = pDoc->m_fPcbSelection_Y1 - y0;
			ppad->m_fCy += deltay;
		}				
	}		
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent != -1)) continue;
		if(pvia->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_VIA;
			pundo->m_nIndex = i;
			pundo->pcbvia.Copy(pvia);
			pundo->pcbvia.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double y0 = pvia->m_fCy;
			double deltay = pDoc->m_fPcbSelection_Y1 - y0;
			pvia->m_fCy += deltay;
		}				
	}
	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag != FLAG_NORMAL) continue;
		if(pcomp->m_bSelection == TRUE)
		{	
			double y0 = pcomp->m_fY;
			double deltay = pDoc->m_fPcbSelection_Y1 - y0;

			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_COMP;
			pundo->m_nIndex = i;
			pundo->pcbcomp.Copy(pcomp);
			pundo->pcbcomp.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			pcomp->m_fY += deltay;

			for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
				if(ptrack->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = j;
					pundo->pcbtrack.Copy(ptrack);
					pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					ptrack->m_fY1 += deltay;
					ptrack->m_fY2 += deltay;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
				if(parc->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = j;
					pundo->pcbarc.Copy(parc);
					pundo->pcbarc.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					parc->m_fCy += deltay;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
				if(pfill->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = j;
					pundo->pcbfill.Copy(pfill);
					pundo->pcbfill.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					pfill->m_fY1 += deltay;
					pfill->m_fY2 += deltay;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
				if(ppad->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_PAD;
					pundo->m_nIndex = j;
					pundo->pcbpad.Copy(ppad);
					pundo->pcbpad.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					ppad->m_fCy += deltay;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
				if(pvia->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = j;
					pundo->pcbvia.Copy(pvia);
					pundo->pcbvia.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					pvia->m_fCy += deltay;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbText.GetCount(); j++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(j);
				if(ptext->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TEXT;
					pundo->m_nIndex = j;
					pundo->pcbtext.Copy(ptext);
					pundo->pcbtext.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					ptext->m_fSy += deltay;
					ptext->UpdateStringBorder(this, FALSE);
				}
			}
		}
	}	

	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}	
	
	Invalidate();
}

void CESDView::OnPcbEditAlignHplace()
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	int totalnum = pDoc->m_arrayPcbSelection.GetCount();
	int currentnum = 0;
	double totalspace =  pDoc->m_fPcbSelection_X2 -  pDoc->m_fPcbSelection_X1;

	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nComponent != -1)||(ptrack->m_nPolygon != -1)) continue;

		if(ptrack->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = i;
			pundo->pcbtrack.Copy(ptrack);
			pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double x0 = MIN(ptrack->m_fX1, ptrack->m_fX2);
			double deltax = pDoc->m_fPcbSelection_X1 + totalspace*currentnum/totalnum - x0;
			currentnum++;
			ptrack->m_fX1 += deltax;
			ptrack->m_fX2 += deltax;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nComponent != -1)||(parc->m_nPolygon != -1)) continue;
		if(parc->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_ARC;
			pundo->m_nIndex = i;
			pundo->pcbarc.Copy(parc);
			pundo->pcbarc.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double x0 = parc->m_fCx;
			double deltax = pDoc->m_fPcbSelection_X1 + totalspace*currentnum/totalnum - x0;
			currentnum++;
			parc->m_fCx += deltax;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_nComponent != -1)) continue;
		if(ptext->m_bSelection == TRUE)
		{			
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TEXT;
			pundo->m_nIndex = i;
			pundo->pcbtext.Copy(ptext);
			pundo->pcbtext.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double x0 = ptext->m_fSx;
			double deltax = pDoc->m_fPcbSelection_X1 + totalspace*currentnum/totalnum - x0;
			currentnum++;
			ptext->m_fSx += deltax;
			ptext->UpdateStringBorder(this, FALSE);
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent != -1)) continue;
		if(pfill->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_FILL;
			pundo->m_nIndex = i;
			pundo->pcbfill.Copy(pfill);
			pundo->pcbfill.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double x0 = MIN(pfill->m_fX1, pfill->m_fX2);
			double deltax = pDoc->m_fPcbSelection_X1 + totalspace*currentnum/totalnum - x0;
			currentnum++;
			pfill->m_fX1 += deltax;
			pfill->m_fX2 += deltax;
		}			
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent != -1)) continue;
		if(ppad->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_PAD;
			pundo->m_nIndex = i;
			pundo->pcbpad.Copy(ppad);
			pundo->pcbpad.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double x0 = ppad->m_fCx;
			double deltax = pDoc->m_fPcbSelection_X1 + totalspace*currentnum/totalnum - x0;
			currentnum++;
			ppad->m_fCx += deltax;
		}				
	}		
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent != -1)) continue;
		if(pvia->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_VIA;
			pundo->m_nIndex = i;
			pundo->pcbvia.Copy(pvia);
			pundo->pcbvia.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double x0 = pvia->m_fCx;
			double deltax = pDoc->m_fPcbSelection_X1 + totalspace*currentnum/totalnum - x0;
			currentnum++;
			pvia->m_fCx += deltax;
		}				
	}
	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag != FLAG_NORMAL) continue;
		if(pcomp->m_bSelection == TRUE)
		{	
			double x0 = pcomp->m_fX;
			double deltax = pDoc->m_fPcbSelection_X1 + totalspace*currentnum/totalnum - x0;
			currentnum++;

			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_COMP;
			pundo->m_nIndex = i;
			pundo->pcbcomp.Copy(pcomp);
			pundo->pcbcomp.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			pcomp->m_fX += deltax;

			for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
				if(ptrack->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = j;
					pundo->pcbtrack.Copy(ptrack);
					pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					ptrack->m_fX1 += deltax;
					ptrack->m_fX2 += deltax;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
				if(parc->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = j;
					pundo->pcbarc.Copy(parc);
					pundo->pcbarc.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					parc->m_fCx += deltax;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
				if(pfill->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = j;
					pundo->pcbfill.Copy(pfill);
					pundo->pcbfill.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					pfill->m_fX1 += deltax;
					pfill->m_fX2 += deltax;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
				if(ppad->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_PAD;
					pundo->m_nIndex = j;
					pundo->pcbpad.Copy(ppad);
					pundo->pcbpad.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					ppad->m_fCx += deltax;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
				if(pvia->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = j;
					pundo->pcbvia.Copy(pvia);
					pundo->pcbvia.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					pvia->m_fCx += deltax;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbText.GetCount(); j++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(j);
				if(ptext->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TEXT;
					pundo->m_nIndex = j;
					pundo->pcbtext.Copy(ptext);
					pundo->pcbtext.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					ptext->m_fSx += deltax;
					ptext->UpdateStringBorder(this, FALSE);
				}
			}
		}
	}	

	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}	
	
	Invalidate();
}

void CESDView::OnPcbEditAlignVplace()
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);
	
	int totalnum = pDoc->m_arrayPcbSelection.GetCount();
	int currentnum = 0;
	double totalspace =  pDoc->m_fPcbSelection_Y2 -  pDoc->m_fPcbSelection_Y1;

	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nComponent != -1)||(ptrack->m_nPolygon != -1)) continue;

		if(ptrack->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = i;
			pundo->pcbtrack.Copy(ptrack);
			pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double y0 = MIN(ptrack->m_fY1, ptrack->m_fY2);
			double deltay = pDoc->m_fPcbSelection_Y2 - totalspace*currentnum/totalnum - y0;
			currentnum++;
			ptrack->m_fY1 += deltay;
			ptrack->m_fY2 += deltay;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nComponent != -1)||(parc->m_nPolygon != -1)) continue;
		if(parc->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_ARC;
			pundo->m_nIndex = i;
			pundo->pcbarc.Copy(parc);
			pundo->pcbarc.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double y0 = parc->m_fCy;
			double deltay = pDoc->m_fPcbSelection_Y2 - totalspace*currentnum/totalnum - y0;
			currentnum++;
			parc->m_fCy += deltay;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_nComponent != -1)) continue;
		if(ptext->m_bSelection == TRUE)
		{			
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TEXT;
			pundo->m_nIndex = i;
			pundo->pcbtext.Copy(ptext);
			pundo->pcbtext.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double y0 = ptext->m_fSy;
			double deltay = pDoc->m_fPcbSelection_Y2 - totalspace*currentnum/totalnum - y0;
			currentnum++;
			ptext->m_fSy += deltay;
			ptext->UpdateStringBorder(this, FALSE);
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent != -1)) continue;
		if(pfill->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_FILL;
			pundo->m_nIndex = i;
			pundo->pcbfill.Copy(pfill);
			pundo->pcbfill.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double y0 = MIN(pfill->m_fY1, pfill->m_fY2);
			double deltay = pDoc->m_fPcbSelection_Y2 - totalspace*currentnum/totalnum - y0;
			currentnum++;
			pfill->m_fY1 += deltay;
			pfill->m_fY2 += deltay;
		}			
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent != -1)) continue;
		if(ppad->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_PAD;
			pundo->m_nIndex = i;
			pundo->pcbpad.Copy(ppad);
			pundo->pcbpad.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double y0 = ppad->m_fCy;
			double deltay = pDoc->m_fPcbSelection_Y2 - totalspace*currentnum/totalnum - y0;
			currentnum++;
			ppad->m_fCy += deltay;
		}				
	}		
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent != -1)) continue;
		if(pvia->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_VIA;
			pundo->m_nIndex = i;
			pundo->pcbvia.Copy(pvia);
			pundo->pcbvia.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double y0 = pvia->m_fCy;
			double deltay = pDoc->m_fPcbSelection_Y2 - totalspace*currentnum/totalnum - y0;
			currentnum++;
			pvia->m_fCy += deltay;
		}				
	}
	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag != FLAG_NORMAL) continue;
		if(pcomp->m_bSelection == TRUE)
		{	
			double y0 = pcomp->m_fY;
			double deltay = pDoc->m_fPcbSelection_Y2 - totalspace*currentnum/totalnum - y0;
			currentnum++;

			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_COMP;
			pundo->m_nIndex = i;
			pundo->pcbcomp.Copy(pcomp);
			pundo->pcbcomp.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			pcomp->m_fY += deltay;

			for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
				if(ptrack->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = j;
					pundo->pcbtrack.Copy(ptrack);
					pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					ptrack->m_fY1 += deltay;
					ptrack->m_fY2 += deltay;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
				if(parc->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = j;
					pundo->pcbarc.Copy(parc);
					pundo->pcbarc.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					parc->m_fCy += deltay;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
				if(pfill->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = j;
					pundo->pcbfill.Copy(pfill);
					pundo->pcbfill.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					pfill->m_fY1 += deltay;
					pfill->m_fY2 += deltay;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
				if(ppad->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_PAD;
					pundo->m_nIndex = j;
					pundo->pcbpad.Copy(ppad);
					pundo->pcbpad.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					ppad->m_fCy += deltay;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
				if(pvia->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = j;
					pundo->pcbvia.Copy(pvia);
					pundo->pcbvia.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					pvia->m_fCy += deltay;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbText.GetCount(); j++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(j);
				if(ptext->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TEXT;
					pundo->m_nIndex = j;
					pundo->pcbtext.Copy(ptext);
					pundo->pcbtext.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					ptext->m_fSy += deltay;
					ptext->UpdateStringBorder(this, FALSE);
				}
			}
		}
	}	

	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}	
	
	Invalidate();
}


void CESDView::OnPcbMeasureDistance()
{
	int ch1;
	double fx1,fy1,fx2,fy2;
	int x1,y1,x2,y2;

	int drawmode;
	char szPlaceLineStart[]  = "测量距离: <左键><回车>定义起点, <右键><ESC>结束命令";
	char szPlaceLineEnd[]  = "测量距离: <左键><回车>定义终点, <右键><ESC>结束命令";
	
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	MoveCursorToCenter();

	Write_Help(szPlaceLineStart);

	int width = 1;

	int layer = m_nPCB_Current_Layer;
	COLORREF color = nEDA_PCB_LayerColor[m_nPCB_Current_Layer];

	CPen *OverwritePen1, *oldpen;
	OverwritePen1 = new CPen(PS_SOLID, width, color);

	int flag = 1;
	nEDA_RedrawFlag = OFF;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			PcbElecHotPointsControl(&dc);

			fx1 = m_fCurrentSnapX;
			fy1 = m_fCurrentSnapY;
			fx2 = fx1; 
			fy2 = fy1;

			Write_Help(szPlaceLineEnd);
			do
			{
				ch1 = MouseKey();
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{ 
					PcbElecHotPointsControl(&dc);

					Write_Help(szPlaceLineStart);
					Write_Info(szEDA_Info_Ready);

					drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);

					
					oldpen = dc.SelectObject(OverwritePen1);
					
					Get_VPort_XY(&x1,&y1,fx1,fy1);
					Get_VPort_XY(&x2,&y2,fx2,fy2);							
					if((x1 != x2)||(y1 != y2))
					{
						dc.MoveTo(x1,y1);
						dc.LineTo(x2,y2);
					}
					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);

					ch1 = VK_ESCAPE;
					break;
				}
				else if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{ 
						flag = 1;

					}
					else
					{
						if(nEDA_RedrawFlag == ON)
						{ 
							nEDA_RedrawFlag = OFF;
						}
						else  
						{
							
							
							drawmode = dc.GetROP2();
							dc.SetROP2(R2_NOTXORPEN);							

							
							oldpen = dc.SelectObject(OverwritePen1);
							
							Get_VPort_XY(&x1,&y1,fx1,fy1);
							Get_VPort_XY(&x2,&y2,fx2,fy2);							
							if((x1 != x2)||(y1 != y2))
							{
								dc.MoveTo(x1,y1);
								dc.LineTo(x2,y2);
							}

							dc.SelectObject(oldpen);
							dc.SetROP2(drawmode);
						}


						fx2 = m_fCurrentSnapX; 
						fy2 = m_fCurrentSnapY;


						drawmode = dc.GetROP2();
						dc.SetROP2(R2_NOTXORPEN);
						
						
						oldpen = dc.SelectObject(OverwritePen1);
						
						Get_VPort_XY(&x1,&y1,fx1,fy1);
						Get_VPort_XY(&x2,&y2,fx2,fy2);							
						if((x1 != x2)||(y1 != y2))
						{
							dc.MoveTo(x1,y1);
							dc.LineTo(x2,y2);
						}

						dc.SelectObject(oldpen);
						dc.SetROP2(drawmode);
					}
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);
						
					
					oldpen = dc.SelectObject(OverwritePen1);
					
					Get_VPort_XY(&x1,&y1,fx1,fy1);
					Get_VPort_XY(&x2,&y2,fx2,fy2);							
					if((x1 != x2)||(y1 != y2))
					{
						dc.MoveTo(x1,y1);
						dc.LineTo(x2,y2);
					}

					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);

					CString csStrd = pDoc->Convert_XY_To_String(SQRT(fx1, fy1, fx2, fy2));
					CString csStrx = pDoc->Convert_XY_To_String(fabs(fx2 - fx1));
					CString csStry = pDoc->Convert_XY_To_String(fabs(fy2 - fy1));

					csStrd = CString("距离 = ") + csStrd + " (dx = " + csStrx + ", dy = " + csStry + ")";
					AfxMessageBox(csStrd);

					ch1 = MOUSEMOVE;
					break;
				}
			}while(1);
		}
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));
	
	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	
	delete OverwritePen1;

	Write_Help(szEDA_Help_Ready);
}

void CESDView::OnRouteAll()
{
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	
	Refresh_Ratsnest();
	if(pDoc->m_arrayPcbConnection.GetCount() <= 0) return;

	
	int judge = 1;
	for(;;)
	{
		if(judge == 1) judge = 0;
		else if(judge == 0) break;
		for(int i=0; i<pDoc->m_arrayPcbConnection.GetCount()-1; i++)
		{
			COb_pcbconnection* pconnection1 = pDoc->m_arrayPcbConnection.GetAt(i);
			COb_pcbconnection* pconnection2 = pDoc->m_arrayPcbConnection.GetAt(i+1);

			if(SQRT(pconnection1->m_fX1, pconnection1->m_fY1, pconnection1->m_fX2, pconnection1->m_fY2) > SQRT(pconnection2->m_fX1, pconnection2->m_fY1, pconnection2->m_fX2, pconnection2->m_fY2))
			{
				judge=1;
				pDoc->m_arrayPcbConnection.SetAt(i, pconnection2);
				pDoc->m_arrayPcbConnection.SetAt(i+1, pconnection1);
			}
		}
	}

	BOOL undo_flag = FALSE;
	
	if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
	{
		pDoc->Clear_Discarded_Undo_Buffer();
	}
	

	int totalnum = pDoc->m_arrayPcbConnection.GetCount();
	char* s_hv = "水平垂直布线";
	char* s_l = "L型布线";
	char* s_z = "Z型布线";
	char* s_c = "C型布线";
	char s1[100] = "布通率 = ";
	char s2[100] = "%";
	
	Write_Help(s_hv);
	for(int i=0; i<pDoc->m_arrayPcbConnection.GetCount(); i++)
	{
		COb_pcbconnection* pconnection = pDoc->m_arrayPcbConnection.GetAt(i);
		if((pconnection->m_nLayer1 == pconnection->m_nLayer2)||(pconnection->m_nLayer1 == MULTILAYER)||(pconnection->m_nLayer2 == MULTILAYER))
		{
			if(fabs(pconnection->m_fX1 - pconnection->m_fX2) > fabs(pconnection->m_fY1 - pconnection->m_fY2))
			{
				if(fabs(pconnection->m_fY1 - pconnection->m_fY2) <= fEDA_PcbRouteDelta)
				{
					if(Route_H(pconnection) == TRUE)
					{
						pconnection->Draw(&dc, this);
						delete pconnection;
						pDoc->m_arrayPcbConnection.RemoveAt(i);
						i--;
						undo_flag = TRUE;
	
						char str[1000];
						sprintf(str, "%s: %s%d/%d(%.1f%s)", s_hv, s1, (totalnum-pDoc->m_arrayPcbConnection.GetCount()), totalnum, ((double)totalnum - pDoc->m_arrayPcbConnection.GetCount())*100/totalnum, s2);
						Write_Help(str);
						continue;
					}
				}
			}
			else
			{
				if(fabs(pconnection->m_fX1 - pconnection->m_fX2) <= fEDA_PcbRouteDelta)
				{
					if(Route_V(pconnection) == TRUE)
					{
						pconnection->Draw(&dc, this);
						delete pconnection;
						pDoc->m_arrayPcbConnection.RemoveAt(i);
						i--;
						undo_flag = TRUE;

						char str[1000];
						sprintf(str, "%s: %s%d/%d(%.1f%s)", s_hv, s1, (totalnum-pDoc->m_arrayPcbConnection.GetCount()), totalnum, ((double)totalnum - pDoc->m_arrayPcbConnection.GetCount())*100/totalnum, s2);
						Write_Help(str);
						continue;
					}
					
				}
			}
		}
	}

	
	Write_Help(s_l);
	for(int i=0; i<pDoc->m_arrayPcbConnection.GetCount(); i++)
	{
		COb_pcbconnection* pconnection = pDoc->m_arrayPcbConnection.GetAt(i);
		if(Route_L(pconnection) == TRUE)
		{
			pconnection->Draw(&dc, this);
			delete pconnection;
			pDoc->m_arrayPcbConnection.RemoveAt(i);
			i--;
			undo_flag = TRUE;
	
			char str[1000];
			sprintf(str, "%s: %s%d/%d(%.1f%s)", s_l, s1, (totalnum-pDoc->m_arrayPcbConnection.GetCount()), totalnum, ((double)totalnum - pDoc->m_arrayPcbConnection.GetCount())*100/totalnum, s2);
			Write_Help(str);
			continue;
		}
	}
	
	
	Write_Help(s_z);
	for(int i=0; i<pDoc->m_arrayPcbConnection.GetCount(); i++)
	{
		COb_pcbconnection* pconnection = pDoc->m_arrayPcbConnection.GetAt(i);
		if(Route_Z(pconnection) == TRUE)
		{
			pconnection->Draw(&dc, this);
			delete pconnection;
			pDoc->m_arrayPcbConnection.RemoveAt(i);
			i--;
			undo_flag = TRUE;
			
			char str[1000];
			sprintf(str, "%s: %s%d/%d(%.1f%s)", s_z, s1, (totalnum-pDoc->m_arrayPcbConnection.GetCount()), totalnum, ((double)totalnum - pDoc->m_arrayPcbConnection.GetCount())*100/totalnum, s2);
			Write_Help(str);
			continue;
		}
	}

	
	Write_Help(s_c);
	for(int i=0; i<pDoc->m_arrayPcbConnection.GetCount(); i++)
	{
		COb_pcbconnection* pconnection = pDoc->m_arrayPcbConnection.GetAt(i);
		if(Route_C(pconnection) == TRUE)
		{
			pconnection->Draw(&dc, this);
			delete pconnection;
			pDoc->m_arrayPcbConnection.RemoveAt(i);
			i--;
			undo_flag = TRUE;
			
			char str[1000];
			sprintf(str, "%s: %s%d/%d(%.1f%s)", s_c, s1, (totalnum-pDoc->m_arrayPcbConnection.GetCount()), totalnum, ((double)totalnum - pDoc->m_arrayPcbConnection.GetCount())*100/totalnum, s2);
			Write_Help(str);
			continue;
		}
	}

	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
			
	char str[1000];
	sprintf(str, "布线完成, 连接总数=%d, 已布线=%d, 未布线=%d, 布通率=%.1f%s", totalnum, totalnum-pDoc->m_arrayPcbConnection.GetCount(), pDoc->m_arrayPcbConnection.GetCount(), ((double)totalnum - pDoc->m_arrayPcbConnection.GetCount())*100/totalnum, s2);
	AfxMessageBox(str);

	Write_Help(szEDA_Help_Ready);
}

BOOL CESDView::Route_H(COb_pcbconnection* pconnection)
{
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		int layer = pDoc->m_arrayPcbLayerShow.GetAt(i);
		if((layer >= TOPLAYER) && (layer <= BOTTOMLAYER))
		{
			if((pDoc->m_nRulesLayer[layer] == HORIZONTAL_ROUTE)||(pDoc->m_nRulesLayer[layer] == ANY_ROUTE))
			{
				if( ((pconnection->m_nLayer1 != MULTILAYER)&&(pconnection->m_nLayer1 != layer))||\
					((pconnection->m_nLayer2 != MULTILAYER)&&(pconnection->m_nLayer2 != layer)) ) continue;
				
				
				COb_pcbtrack atrack[5];
				for(int j=0; j<5; j++)
				{
					atrack[j].pDocument = pDoc;
					atrack[j].m_nLayer = layer;
					atrack[j].m_fWidth = pDoc->m_fRulesTrackWidth;
					atrack[j].m_nNet = pconnection->m_nNet;
				}
				
				
				if(pconnection->m_fX1 > pconnection->m_fX2)
				{
					double fx, fy;
					int l;
					fx = pconnection->m_fX1;
					fy = pconnection->m_fY1;
					l = pconnection->m_nLayer1;
					pconnection->m_fX1 = pconnection->m_fX2;
					pconnection->m_fY1 = pconnection->m_fY2;
					pconnection->m_nLayer1 = pconnection->m_nLayer2;
					pconnection->m_fX2 = fx;
					pconnection->m_fY2 = fy;
					pconnection->m_nLayer2 = l;
				}				
				
				if(pconnection->m_fY1 == pconnection->m_fY2)
				{
					
					atrack[0].m_fX1 = pconnection->m_fX1;
					atrack[0].m_fY1 = pconnection->m_fY1;
					atrack[0].m_fX2 = pconnection->m_fX2;
					atrack[0].m_fY2 = pconnection->m_fY2;

					if(FindTrackOnLineDrcErrors(&atrack[0]) == FALSE)
					{
						COb_pcbtrack* pnewtrack = new COb_pcbtrack();
						pnewtrack->Copy(&atrack[0]);
						int index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						return TRUE;
					}
				}
				else
				{
					
				
					double delta_x = fabs(pconnection->m_fX2 - pconnection->m_fX1);
					double delta_y = fabs(pconnection->m_fY2 - pconnection->m_fY1);
					for(int j=0; j<=(int)(delta_x/2); j++)
					{
						
						if((pconnection->m_fX1 + delta_x/2 + j + delta_y/2) < pconnection->m_fX2)
						{
							atrack[0].m_fX1 = pconnection->m_fX1;
							atrack[0].m_fY1 = pconnection->m_fY1;
							atrack[0].m_fX2 = pconnection->m_fX1 + delta_x/2 + j - delta_y/2;
							atrack[0].m_fY2 = pconnection->m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE) break;		

							atrack[2].m_fX1 = pconnection->m_fX1 + delta_x/2 + j + delta_y/2;
							atrack[2].m_fY1 = pconnection->m_fY2;	
							atrack[2].m_fX2 = pconnection->m_fX2;
							atrack[2].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[2]) == TRUE) continue;
						
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) continue;

							COb_pcbtrack* pnewtrack;
							int index;
							CPcbUndo *pundo;
							
							pnewtrack = new COb_pcbtrack();
							pnewtrack->Copy(&atrack[0]);
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[1]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);							
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[2]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);		
							return TRUE;
						}
						else
						{
							atrack[0].m_fX1 = pconnection->m_fX1;
							atrack[0].m_fY1 = pconnection->m_fY1;
							atrack[0].m_fX2 = pconnection->m_fX2 - delta_y;
							atrack[0].m_fY2 = pconnection->m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE) break;
							
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = pconnection->m_fX2;
							atrack[1].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) break;

							COb_pcbtrack* pnewtrack;
							int index;
							CPcbUndo *pundo;
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[0]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[1]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							return TRUE;
						}
					}
					for(int j=0; j<=(int)(delta_x/2); j++)
					{
						
						if((pconnection->m_fX1 + delta_x/2 - j - delta_y/2) > pconnection->m_fX1)
						{
							atrack[2].m_fX1 = pconnection->m_fX1 + delta_x/2 - j + delta_y/2;
							atrack[2].m_fY1 = pconnection->m_fY2;	
							atrack[2].m_fX2 = pconnection->m_fX2;
							atrack[2].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[2]) == TRUE) break;	

							atrack[0].m_fX1 = pconnection->m_fX1;
							atrack[0].m_fY1 = pconnection->m_fY1;
							atrack[0].m_fX2 = pconnection->m_fX1 + delta_x/2 - j - delta_y/2;
							atrack[0].m_fY2 = pconnection->m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE) continue;		
						
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) continue;

							COb_pcbtrack* pnewtrack;
							int index;
							CPcbUndo *pundo;
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[0]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[1]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[2]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							return TRUE;
						}
						else
						{
							atrack[2].m_fX1 = pconnection->m_fX1 + delta_y;
							atrack[2].m_fY1 = pconnection->m_fY1;	
							atrack[2].m_fX2 = pconnection->m_fX2;
							atrack[2].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[2]) == TRUE) break;

							atrack[1].m_fX1 = pconnection->m_fX1;
							atrack[1].m_fY1 = pconnection->m_fY1;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) break;

							COb_pcbtrack* pnewtrack;
							int index;
							CPcbUndo *pundo;
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[1]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[2]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							return TRUE;
						}
					}
				}
					
				double fx1, fy1, fx2, fy2;
				fx1 = pconnection->m_fX1;
				fy1 = pconnection->m_fY1;
				fx2 = pconnection->m_fX2;
				fy2 = pconnection->m_fY1;
				
				atrack[0].m_fX1 = fx1;
				atrack[0].m_fY1 = fy1;
				atrack[0].m_fX2 = fx2;
				atrack[0].m_fY2 = fy2;
				if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE)	
				{
					double ftemp_x1, ftemp_y1, ftemp_x2, ftemp_y2, ftemp_x, ftemp_y;
					ftemp_x1 = fx1;		
					ftemp_y1 = fy1;
					ftemp_x2 = fx2;
					ftemp_y2 = fy2;
					ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
					ftemp_y = (ftemp_y1 + ftemp_y2)/2;
					atrack[0].m_fX2 = ftemp_x;
					atrack[0].m_fY2 = ftemp_y;
					double f_x2, f_y2;
					f_x2 = fx1;
					f_y2 = fy1;
					for(;;)
					{
						if(FindTrackOnLineDrcErrors(&atrack[0]) == FALSE)	
						{
							f_x2 = atrack[0].m_fX2;
							f_y2 = atrack[0].m_fY2;
							ftemp_x1 = ftemp_x;	
							ftemp_y1 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[0].m_fX2 = ftemp_x;
							atrack[0].m_fY2 = ftemp_y;
						}
						else if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE)	
						{
							ftemp_x2 = ftemp_x;	
							ftemp_y2 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[0].m_fX2 = ftemp_x;
							atrack[0].m_fY2 = ftemp_y;
						}
						if((fabs(ftemp_x2 - ftemp_x1) < 1)&&(fabs(ftemp_y2 - ftemp_y1) < 1))
						{
							atrack[0].m_fX2 = f_x2;
							atrack[0].m_fY2 = f_y2;
							break;
						}
					}
				}
				
				fx1 = pconnection->m_fX2;
				fy1 = pconnection->m_fY2;
				fx2 = pconnection->m_fX1;
				fy2 = pconnection->m_fY2;

				atrack[4].m_fX1 = fx1;
				atrack[4].m_fY1 = fy1;
				atrack[4].m_fX2 = fx2;
				atrack[4].m_fY2 = fy2;
				
				if(FindTrackOnLineDrcErrors(&atrack[4]) == TRUE)	
				{
					double ftemp_x1, ftemp_y1, ftemp_x2, ftemp_y2, ftemp_x, ftemp_y;
					ftemp_x1 = fx1;		
					ftemp_y1 = fy1;
					ftemp_x2 = fx2;
					ftemp_y2 = fy2;
					ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
					ftemp_y = (ftemp_y1 + ftemp_y2)/2;
					atrack[4].m_fX2 = ftemp_x;
					atrack[4].m_fY2 = ftemp_y;
					double f_x2, f_y2;
					f_x2 = fx1;
					f_y2 = fy1;
					for(;;)
					{
						if(FindTrackOnLineDrcErrors(&atrack[4]) == FALSE)	
						{
							f_x2 = atrack[4].m_fX2;
							f_y2 = atrack[4].m_fY2;
							ftemp_x1 = ftemp_x;	
							ftemp_y1 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[4].m_fX2 = ftemp_x;
							atrack[4].m_fY2 = ftemp_y;
						}
						else if(FindTrackOnLineDrcErrors(&atrack[4]) == TRUE)	
						{
							ftemp_x2 = ftemp_x;	
							ftemp_y2 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[4].m_fX2 = ftemp_x;
							atrack[4].m_fY2 = ftemp_y;
						}
						if((fabs(ftemp_x2 - ftemp_x1) < 1)&&(fabs(ftemp_y2 - ftemp_y1) < 1))
						{
							atrack[4].m_fX2 = f_x2;
							atrack[4].m_fY2 = f_y2;
							break;
						}
					}
				}

				
				if( fabs(pconnection->m_fY1 - pconnection->m_fY2) > 50 )
				{
					
					int nDelta = fabs(pconnection->m_fY1 - pconnection->m_fY2);
					double fy_start = MAX(pconnection->m_fY1, pconnection->m_fY2);
					for(int k=0; k<2; k++)
					{
						for(int j=1;  j<nDelta; j++)
						{
							atrack[2].m_fY1 = atrack[2].m_fY2 = fy_start - j;
							if(k == 0) 
							{
								atrack[2].m_fX1 = atrack[0].m_fX2 + fabs(atrack[0].m_fY2 - atrack[2].m_fY1);
								atrack[2].m_fX2 = atrack[4].m_fX2 - fabs(atrack[4].m_fY2 - atrack[2].m_fY2);
								if(atrack[2].m_fX1 > atrack[2].m_fX2) break;
							}
							else if(k == 1) 
							{
								atrack[2].m_fX1 = atrack[0].m_fX2 + fabs(atrack[0].m_fY2 - atrack[2].m_fY1)/2;
								atrack[2].m_fX2 = atrack[4].m_fX2 - fabs(atrack[4].m_fY2 - atrack[2].m_fY2)/2;
								if( ((atrack[0].m_fX2 - fabs(atrack[2].m_fY1 - atrack[0].m_fY1)/2) < atrack[0].m_fX1) ||\
									((atrack[4].m_fX2 + fabs(atrack[2].m_fY1 - atrack[4].m_fY1)/2) > atrack[4].m_fX1) ) break;
							}
					
							if(FindTrackOnLineDrcErrors(&atrack[2]) == FALSE)
							{
								atrack[0].m_fX2 = atrack[2].m_fX1 - fabs(atrack[2].m_fY1 - atrack[0].m_fY1);
								atrack[4].m_fX2 = atrack[2].m_fX2 + fabs(atrack[2].m_fY1 - atrack[4].m_fY1);
								
								atrack[1].m_fX1 = atrack[0].m_fX2;
								atrack[1].m_fY1 = atrack[0].m_fY2;
								atrack[1].m_fX2 = atrack[2].m_fX1;
								atrack[1].m_fY2 = atrack[2].m_fY1;

								atrack[3].m_fX1 = atrack[2].m_fX2;
								atrack[3].m_fY1 = atrack[2].m_fY2;
								atrack[3].m_fX2 = atrack[4].m_fX2;
								atrack[3].m_fY2 = atrack[4].m_fY2;
								
								if((FindTrackOnLineDrcErrors(&atrack[1]) == FALSE)&&(FindTrackOnLineDrcErrors(&atrack[3]) == FALSE))
								{

									COb_pcbtrack* pnewtrack;
									int index;
									CPcbUndo *pundo;
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[0]);						
									index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
									pundo = new CPcbUndo;
									pundo->m_nOp = UNDO_DELETE;	
									pundo->m_nOb = PCB_ELEMENT_TRACK;
									pundo->m_nIndex = index;
									pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[1]);						
									index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
									pundo = new CPcbUndo;
									pundo->m_nOp = UNDO_DELETE;	
									pundo->m_nOb = PCB_ELEMENT_TRACK;
									pundo->m_nIndex = index;
									pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[2]);						
									index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
									pundo = new CPcbUndo;
									pundo->m_nOp = UNDO_DELETE;	
									pundo->m_nOb = PCB_ELEMENT_TRACK;
									pundo->m_nIndex = index;
									pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[3]);						
									index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
									pundo = new CPcbUndo;
									pundo->m_nOp = UNDO_DELETE;	
									pundo->m_nOb = PCB_ELEMENT_TRACK;
									pundo->m_nIndex = index;
									pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[4]);						
									index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
									pundo = new CPcbUndo;
									pundo->m_nOp = UNDO_DELETE;	
									pundo->m_nOb = PCB_ELEMENT_TRACK;
									pundo->m_nIndex = index;
									pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
									return TRUE;
								}
							}
						}
					}
				}
				
				int nDelta = 50;
				double fy_start = MAX(pconnection->m_fY1, pconnection->m_fY2);
				for(int k=0; k<2; k++)
				{
					for(int j=1;  j<nDelta; j++)
					{
						atrack[2].m_fY1 = atrack[2].m_fY2 = fy_start + j;
						if(k == 0) 
						{
							atrack[2].m_fX1 = atrack[0].m_fX2 + fabs(atrack[0].m_fY2 - atrack[2].m_fY1);
							atrack[2].m_fX2 = atrack[4].m_fX2 - fabs(atrack[4].m_fY2 - atrack[2].m_fY2);
							if(atrack[2].m_fX1 > atrack[2].m_fX2) break;
						}
						else if(k == 1) 
						{
							atrack[2].m_fX1 = atrack[0].m_fX2 + fabs(atrack[0].m_fY2 - atrack[2].m_fY1)/2;
							atrack[2].m_fX2 = atrack[4].m_fX2 - fabs(atrack[4].m_fY2 - atrack[2].m_fY2)/2;
							if( ((atrack[0].m_fX2 - fabs(atrack[2].m_fY1 - atrack[0].m_fY1)/2) < atrack[0].m_fX1) ||\
								((atrack[4].m_fX2 + fabs(atrack[2].m_fY1 - atrack[4].m_fY1)/2) > atrack[4].m_fX1) ) break;
						}

						if(FindTrackOnLineDrcErrors(&atrack[2]) == FALSE)
						{
							atrack[0].m_fX2 = atrack[2].m_fX1 - fabs(atrack[2].m_fY1 - atrack[0].m_fY1);
							atrack[4].m_fX2 = atrack[2].m_fX2 + fabs(atrack[2].m_fY1 - atrack[4].m_fY1);
							
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;

							atrack[3].m_fX1 = atrack[2].m_fX2;
							atrack[3].m_fY1 = atrack[2].m_fY2;
							atrack[3].m_fX2 = atrack[4].m_fX2;
							atrack[3].m_fY2 = atrack[4].m_fY2;

							if((FindTrackOnLineDrcErrors(&atrack[1]) == FALSE)&&(FindTrackOnLineDrcErrors(&atrack[3]) == FALSE))
							{
								COb_pcbtrack* pnewtrack;
								int index;
								CPcbUndo *pundo;
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[0]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;	
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[1]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;	
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[2]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;	
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[3]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;	
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[4]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;	
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
								return TRUE;
							}
						}
					}
				}


				
				fy_start = MIN(pconnection->m_fY1, pconnection->m_fY2);
				for(int k=0; k<2; k++)
				{
					for(int j=1;  j<nDelta; j++)
					{
						atrack[2].m_fY1 = atrack[2].m_fY2 = fy_start - j;
						if(k == 0) 
						{
							atrack[2].m_fX1 = atrack[0].m_fX2 + fabs(atrack[0].m_fY2 - atrack[2].m_fY1);
							atrack[2].m_fX2 = atrack[4].m_fX2 - fabs(atrack[4].m_fY2 - atrack[2].m_fY2);
							if(atrack[2].m_fX1 > atrack[2].m_fX2) break;
						}
						else if(k == 1) 
						{
							atrack[2].m_fX1 = atrack[0].m_fX2 + fabs(atrack[0].m_fY2 - atrack[2].m_fY1)/2;
							atrack[2].m_fX2 = atrack[4].m_fX2 - fabs(atrack[4].m_fY2 - atrack[2].m_fY2)/2;
							if( ((atrack[0].m_fX2 - fabs(atrack[2].m_fY1 - atrack[0].m_fY1)/2) < atrack[0].m_fX1) ||\
								((atrack[4].m_fX2 + fabs(atrack[2].m_fY1 - atrack[4].m_fY1)/2) > atrack[4].m_fX1) )break;
						}

						
						if(FindTrackOnLineDrcErrors(&atrack[2]) == FALSE)
						{
							atrack[0].m_fX2 = atrack[2].m_fX1 - fabs(atrack[2].m_fY1 - atrack[0].m_fY1);
							atrack[4].m_fX2 = atrack[2].m_fX2 + fabs(atrack[2].m_fY1 - atrack[4].m_fY1);
							
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;

							atrack[3].m_fX1 = atrack[2].m_fX2;
							atrack[3].m_fY1 = atrack[2].m_fY2;
							atrack[3].m_fX2 = atrack[4].m_fX2;
							atrack[3].m_fY2 = atrack[4].m_fY2;

							if((FindTrackOnLineDrcErrors(&atrack[1]) == FALSE)&&(FindTrackOnLineDrcErrors(&atrack[3]) == FALSE))
							{
								COb_pcbtrack* pnewtrack;
								int index;
								CPcbUndo *pundo;
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[0]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;	
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[1]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;	
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[2]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;	
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[3]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;	
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[4]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;	
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
								return TRUE;
							}
						}
					}
				}
			}
		}
	}

	return FALSE;
}

BOOL CESDView::Route_V(COb_pcbconnection* pconnection)
{
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		int layer = pDoc->m_arrayPcbLayerShow.GetAt(i);
		if((layer >= TOPLAYER) && (layer <= BOTTOMLAYER))
		{
			if((pDoc->m_nRulesLayer[layer] == VERTICAL_ROUTE)||(pDoc->m_nRulesLayer[layer] == ANY_ROUTE))
			{
				if( ((pconnection->m_nLayer1 != MULTILAYER)&&(pconnection->m_nLayer1 != layer))||\
					((pconnection->m_nLayer2 != MULTILAYER)&&(pconnection->m_nLayer2 != layer)) ) continue;
				
				
				COb_pcbtrack atrack[5];
				for(int j=0; j<5; j++)
				{
					atrack[j].pDocument = pDoc;
					atrack[j].m_nLayer = layer;
					atrack[j].m_fWidth = pDoc->m_fRulesTrackWidth;
					atrack[j].m_nNet = pconnection->m_nNet;
				}
				
				
				if(pconnection->m_fY1 > pconnection->m_fY2)
				{
					double fx, fy;
					int l;
					fx = pconnection->m_fX1;
					fy = pconnection->m_fY1;
					l = pconnection->m_nLayer1;
					pconnection->m_fX1 = pconnection->m_fX2;
					pconnection->m_fY1 = pconnection->m_fY2;
					pconnection->m_nLayer1 = pconnection->m_nLayer2;
					pconnection->m_fX2 = fx;
					pconnection->m_fY2 = fy;
					pconnection->m_nLayer2 = l;
				}				
				
				if(pconnection->m_fX1 == pconnection->m_fX2)
				{
					
					atrack[0].m_fX1 = pconnection->m_fX1;
					atrack[0].m_fY1 = pconnection->m_fY1;
					atrack[0].m_fX2 = pconnection->m_fX2;
					atrack[0].m_fY2 = pconnection->m_fY2;

					if(FindTrackOnLineDrcErrors(&atrack[0]) == FALSE)
					{
						COb_pcbtrack* pnewtrack = new COb_pcbtrack();
						int index;
						CPcbUndo *pundo;
						pnewtrack->Copy(&atrack[0]);
						index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
						pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;	
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
						return TRUE;
					}
				}
				else
				{
					
					double delta_x = fabs(pconnection->m_fX2 - pconnection->m_fX1);
					double delta_y = fabs(pconnection->m_fY2 - pconnection->m_fY1);
					for(int j=0; j<=(int)(delta_y/2); j++)
					{
						
						if((pconnection->m_fY1 + delta_y/2 + j + delta_x/2) < pconnection->m_fY2)
						{
							atrack[0].m_fX1 = pconnection->m_fX1;
							atrack[0].m_fY1 = pconnection->m_fY1;
							atrack[0].m_fX2 = pconnection->m_fX1;
							atrack[0].m_fY2 = pconnection->m_fY1 + delta_y/2 + j - delta_x/2;
							if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE) break;		

							atrack[2].m_fX1 = pconnection->m_fX2;
							atrack[2].m_fY1 = pconnection->m_fY1 + delta_y/2 + j + delta_x/2;
							atrack[2].m_fX2 = pconnection->m_fX2;
							atrack[2].m_fY2 = pconnection->m_fY2;
							if(FindTrackOnLineDrcErrors(&atrack[2]) == TRUE) continue;
						
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) continue;

							COb_pcbtrack* pnewtrack;
							int index;
							CPcbUndo *pundo;
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[0]);						
							index =pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							
							pnewtrack = new COb_pcbtrack();		
							pnewtrack->Copy(&atrack[1]);		
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[2]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							return TRUE;
						}
						else
						{
							atrack[0].m_fX1 = pconnection->m_fX1;
							atrack[0].m_fY1 = pconnection->m_fY1;
							atrack[0].m_fX2 = pconnection->m_fX1;
							atrack[0].m_fY2 = pconnection->m_fY2 - delta_x;
							if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE) break;
							
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = pconnection->m_fX2;
							atrack[1].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) break;

							COb_pcbtrack* pnewtrack;
							int index;
							CPcbUndo *pundo;
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[0]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[1]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							return TRUE;
						}
					}
					for(int j=0; j<=(int)(delta_y/2); j++)
					{
						
						if((pconnection->m_fY1 + delta_y/2 - j - delta_x/2) > pconnection->m_fY1)
						{
							atrack[2].m_fX1 = pconnection->m_fX2;
							atrack[2].m_fY1 = pconnection->m_fY1 + delta_y/2 - j + delta_x/2;
							atrack[2].m_fX2 = pconnection->m_fX2;
							atrack[2].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[2]) == TRUE) break;	

							atrack[0].m_fX1 = pconnection->m_fX1;
							atrack[0].m_fY1 = pconnection->m_fY1;
							atrack[0].m_fX2 = pconnection->m_fX1;
							atrack[0].m_fY2 = pconnection->m_fY1 + delta_y/2 - j - delta_x/2;
							if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE) continue;		
						
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) continue;

							COb_pcbtrack* pnewtrack;
							int index;
							CPcbUndo *pundo;
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[0]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[1]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[2]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							return TRUE;
						}
						else
						{
							atrack[2].m_fX1 = pconnection->m_fX1;
							atrack[2].m_fY1 = pconnection->m_fY1 + delta_x;
							atrack[2].m_fX2 = pconnection->m_fX2;
							atrack[2].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[2]) == TRUE) break;

							atrack[1].m_fX1 = pconnection->m_fX1;
							atrack[1].m_fY1 = pconnection->m_fY1;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) break;

							COb_pcbtrack* pnewtrack;
							int index;
							CPcbUndo *pundo;
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[1]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[2]);						
							index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
							pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;	
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = index;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
							return TRUE;
						}
					}
				}
					
				double fx1, fy1, fx2, fy2;
				fx1 = pconnection->m_fX1;
				fy1 = pconnection->m_fY1;
				fx2 = pconnection->m_fX1;
				fy2 = pconnection->m_fY2;
				
				atrack[0].m_fX1 = fx1;
				atrack[0].m_fY1 = fy1;
				atrack[0].m_fX2 = fx2;
				atrack[0].m_fY2 = fy2;
				if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE)	
				{
					double ftemp_x1, ftemp_y1, ftemp_x2, ftemp_y2, ftemp_x, ftemp_y;
					ftemp_x1 = fx1;		
					ftemp_y1 = fy1;
					ftemp_x2 = fx2;
					ftemp_y2 = fy2;
					ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
					ftemp_y = (ftemp_y1 + ftemp_y2)/2;
					atrack[0].m_fX2 = ftemp_x;
					atrack[0].m_fY2 = ftemp_y;
					double f_x2, f_y2;
					f_x2 = fx1;
					f_y2 = fy1;
					for(;;)
					{
						if(FindTrackOnLineDrcErrors(&atrack[0]) == FALSE)	
						{
							f_x2 = atrack[0].m_fX2;
							f_y2 = atrack[0].m_fY2;
							ftemp_x1 = ftemp_x;	
							ftemp_y1 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[0].m_fX2 = ftemp_x;
							atrack[0].m_fY2 = ftemp_y;
						}
						else if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE)	
						{
							ftemp_x2 = ftemp_x;	
							ftemp_y2 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[0].m_fX2 = ftemp_x;
							atrack[0].m_fY2 = ftemp_y;
						}
						if((fabs(ftemp_x2 - ftemp_x1) < 1)&&(fabs(ftemp_y2 - ftemp_y1) < 1))
						{
							atrack[0].m_fX2 = f_x2;
							atrack[0].m_fY2 = f_y2;
							break;
						}
					}
				}
				
				fx1 = pconnection->m_fX2;
				fy1 = pconnection->m_fY2;
				fx2 = pconnection->m_fX2;
				fy2 = pconnection->m_fY1;

				atrack[4].m_fX1 = fx1;
				atrack[4].m_fY1 = fy1;
				atrack[4].m_fX2 = fx2;
				atrack[4].m_fY2 = fy2;
				
				if(FindTrackOnLineDrcErrors(&atrack[4]) == TRUE)	
				{
					double ftemp_x1, ftemp_y1, ftemp_x2, ftemp_y2, ftemp_x, ftemp_y;
					ftemp_x1 = fx1;		
					ftemp_y1 = fy1;
					ftemp_x2 = fx2;
					ftemp_y2 = fy2;
					ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
					ftemp_y = (ftemp_y1 + ftemp_y2)/2;
					atrack[4].m_fX2 = ftemp_x;
					atrack[4].m_fY2 = ftemp_y;
					double f_x2, f_y2;
					f_x2 = fx1;
					f_y2 = fy1;
					for(;;)
					{
						if(FindTrackOnLineDrcErrors(&atrack[4]) == FALSE)	
						{
							f_x2 = atrack[4].m_fX2;
							f_y2 = atrack[4].m_fY2;
							ftemp_x1 = ftemp_x;	
							ftemp_y1 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[4].m_fX2 = ftemp_x;
							atrack[4].m_fY2 = ftemp_y;
						}
						else if(FindTrackOnLineDrcErrors(&atrack[4]) == TRUE)	
						{
							ftemp_x2 = ftemp_x;	
							ftemp_y2 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[4].m_fX2 = ftemp_x;
							atrack[4].m_fY2 = ftemp_y;
						}
						if((fabs(ftemp_x2 - ftemp_x1) < 1)&&(fabs(ftemp_y2 - ftemp_y1) < 1))
						{
							atrack[4].m_fX2 = f_x2;
							atrack[4].m_fY2 = f_y2;
							break;
						}
					}
				}

				if( fabs(pconnection->m_fX1 - pconnection->m_fX2) > 50 )
				{
					int nDelta = fabs(pconnection->m_fX1 - pconnection->m_fX2);
					double fx_start = MAX(pconnection->m_fX1, pconnection->m_fX2);
					for(int k=0; k<2; k++)
					{
						for(int j=1;  j<nDelta; j++)
						{
							atrack[2].m_fX1 = atrack[2].m_fX2 = fx_start - j;
							if(k == 0) 
							{
								atrack[2].m_fY1 = atrack[0].m_fY2 + fabs(atrack[0].m_fX2 - atrack[2].m_fX1);
								atrack[2].m_fY2 = atrack[4].m_fY2 - fabs(atrack[4].m_fX2 - atrack[2].m_fX2);
								if(atrack[2].m_fY1 > atrack[2].m_fY2) break;
							}
							else if(k == 1) 
							{
								atrack[2].m_fY1 = atrack[0].m_fY2 + fabs(atrack[0].m_fX2 - atrack[2].m_fX1)/2;
								atrack[2].m_fY2 = atrack[4].m_fY2 - fabs(atrack[4].m_fX2 - atrack[2].m_fX2)/2;
								if( ((atrack[0].m_fY2 - fabs(atrack[2].m_fX1 - atrack[0].m_fX1)/2) < atrack[0].m_fY1) ||\
									((atrack[4].m_fY2 + fabs(atrack[2].m_fX1 - atrack[4].m_fX1)/2) > atrack[4].m_fY1) ) break;
							}

							if(FindTrackOnLineDrcErrors(&atrack[2]) == FALSE)
							{
								atrack[0].m_fY2 = atrack[2].m_fY1 - fabs(atrack[2].m_fX1 - atrack[0].m_fX1);
								atrack[4].m_fY2 = atrack[2].m_fY2 + fabs(atrack[2].m_fX1 - atrack[4].m_fX1);
								
								atrack[1].m_fX1 = atrack[0].m_fX2;
								atrack[1].m_fY1 = atrack[0].m_fY2;
								atrack[1].m_fX2 = atrack[2].m_fX1;
								atrack[1].m_fY2 = atrack[2].m_fY1;

								atrack[3].m_fX1 = atrack[2].m_fX2;
								atrack[3].m_fY1 = atrack[2].m_fY2;
								atrack[3].m_fX2 = atrack[4].m_fX2;
								atrack[3].m_fY2 = atrack[4].m_fY2;

								if((FindTrackOnLineDrcErrors(&atrack[1]) == FALSE)&&(FindTrackOnLineDrcErrors(&atrack[3]) == FALSE))
								{
									COb_pcbtrack* pnewtrack;
									int index;
									CPcbUndo *pundo;
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[0]);						
									index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
									pundo = new CPcbUndo;
									pundo->m_nOp = UNDO_DELETE;	
									pundo->m_nOb = PCB_ELEMENT_TRACK;
									pundo->m_nIndex = index;
									pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[1]);						
									index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
									pundo = new CPcbUndo;
									pundo->m_nOp = UNDO_DELETE;	
									pundo->m_nOb = PCB_ELEMENT_TRACK;
									pundo->m_nIndex = index;
									pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[2]);						
									index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
									pundo = new CPcbUndo;
									pundo->m_nOp = UNDO_DELETE;	
									pundo->m_nOb = PCB_ELEMENT_TRACK;
									pundo->m_nIndex = index;
									pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[3]);						
									index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
									pundo = new CPcbUndo;
									pundo->m_nOp = UNDO_DELETE;	
									pundo->m_nOb = PCB_ELEMENT_TRACK;
									pundo->m_nIndex = index;
									pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);	
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[4]);						
									index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
									pundo = new CPcbUndo;
									pundo->m_nOp = UNDO_DELETE;
									pundo->m_nOb = PCB_ELEMENT_TRACK;
									pundo->m_nIndex = index;
									pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
									return TRUE;
								}
							}
						}
					}
				}
				
				int nDelta = 50;
				
				double fx_start = MIN(pconnection->m_fX1, pconnection->m_fX2);
				for(int k=0; k<2; k++)
				{
					for(int j=1;  j<nDelta; j++)
					{
						atrack[2].m_fX1 = atrack[2].m_fX2 = fx_start - j;
						if(k == 0) 
						{
							atrack[2].m_fY1 = atrack[0].m_fY2 + fabs(atrack[0].m_fX2 - atrack[2].m_fX1);
							atrack[2].m_fY2 = atrack[4].m_fY2 - fabs(atrack[4].m_fX2 - atrack[2].m_fX2);
							if(atrack[2].m_fX1 > atrack[2].m_fX2) break;
						}
						else if(k == 1)
						{
							atrack[2].m_fY1 = atrack[0].m_fY2 + fabs(atrack[0].m_fX2 - atrack[2].m_fX1)/2;
							atrack[2].m_fY2 = atrack[4].m_fY2 - fabs(atrack[4].m_fX2 - atrack[2].m_fX2)/2;
							if( ((atrack[0].m_fY2 - fabs(atrack[2].m_fX1 - atrack[0].m_fX1)/2) < atrack[0].m_fY1) ||\
								((atrack[4].m_fY2 + fabs(atrack[2].m_fX1 - atrack[4].m_fX1)/2) > atrack[4].m_fY1) )break;
						}

						
						if(FindTrackOnLineDrcErrors(&atrack[2]) == FALSE)
						{
							atrack[0].m_fY2 = atrack[2].m_fY1 - fabs(atrack[2].m_fX1 - atrack[0].m_fX1);
							atrack[4].m_fY2 = atrack[2].m_fY2 + fabs(atrack[2].m_fX1 - atrack[4].m_fX1);
							
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;

							atrack[3].m_fX1 = atrack[2].m_fX2;
							atrack[3].m_fY1 = atrack[2].m_fY2;
							atrack[3].m_fX2 = atrack[4].m_fX2;
							atrack[3].m_fY2 = atrack[4].m_fY2;

							if((FindTrackOnLineDrcErrors(&atrack[1]) == FALSE)&&(FindTrackOnLineDrcErrors(&atrack[3]) == FALSE))
							{
								COb_pcbtrack* pnewtrack;
								int index;
								CPcbUndo *pundo;
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[0]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[1]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[2]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[3]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[4]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								return TRUE;
							}
						}
					}
				}
				
				
				fx_start = MAX(pconnection->m_fX1, pconnection->m_fX2);
				for(int k=0; k<2; k++)
				{
					for(int j=1;  j<nDelta; j++)
					{
						atrack[2].m_fX1 = atrack[2].m_fX2 = fx_start + j;
						if(k == 0) 
						{
							atrack[2].m_fY1 = atrack[0].m_fY2 + fabs(atrack[0].m_fX2 - atrack[2].m_fX1);
							atrack[2].m_fY2 = atrack[4].m_fY2 - fabs(atrack[4].m_fX2 - atrack[2].m_fX2);
							if(atrack[2].m_fX1 > atrack[2].m_fX2) break;
						}
						else if(k == 1)
						{
							atrack[2].m_fY1 = atrack[0].m_fY2 + fabs(atrack[0].m_fX2 - atrack[2].m_fX1)/2;
							atrack[2].m_fY2 = atrack[4].m_fY2 - fabs(atrack[4].m_fX2 - atrack[2].m_fX2)/2;
							if( ((atrack[0].m_fY2 - fabs(atrack[2].m_fX1 - atrack[0].m_fX1)/2) < atrack[0].m_fY1) ||\
								((atrack[4].m_fY2 + fabs(atrack[2].m_fX1 - atrack[4].m_fX1)/2) > atrack[4].m_fY1) ) break;
						}
						
						if(FindTrackOnLineDrcErrors(&atrack[2]) == FALSE)
						{
							atrack[0].m_fY2 = atrack[2].m_fY1 - fabs(atrack[2].m_fX1 - atrack[0].m_fX1);
							atrack[4].m_fY2 = atrack[2].m_fY2 + fabs(atrack[2].m_fX1 - atrack[4].m_fX1);
							
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;

							atrack[3].m_fX1 = atrack[2].m_fX2;
							atrack[3].m_fY1 = atrack[2].m_fY2;
							atrack[3].m_fX2 = atrack[4].m_fX2;
							atrack[3].m_fY2 = atrack[4].m_fY2;

							if((FindTrackOnLineDrcErrors(&atrack[1]) == FALSE)&&(FindTrackOnLineDrcErrors(&atrack[3]) == FALSE))
							{
								COb_pcbtrack* pnewtrack;
								int index;
								CPcbUndo *pundo;
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[0]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[1]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[2]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[3]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[4]);						
								index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		pnewtrack->Draw(&dc, this);
								pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = index;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								return TRUE;
							}

						}
					}
				}
			}
		}
	}

	return FALSE;
}

BOOL CESDView::Route_L(COb_pcbconnection* pconnection)
{
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	double fVia_Step = 50;
	double fDelta = 100;
	if((fabs(pconnection->m_fX1 - pconnection->m_fX2) < fDelta) || (fabs(pconnection->m_fY1 - pconnection->m_fY2) < fDelta)) return FALSE;

	
	CTypedPtrArray <CObArray, COb_pcbtrack*> arrayTrack_H;
	CTypedPtrArray <CObArray, COb_pcbtrack*> arrayTrack_V;

	COb_pcbvia avia;
	avia.pDocument = pDoc;
	avia.m_nStartLayer = TOPLAYER;
	avia.m_nEndLayer = BOTTOMLAYER;
	avia.m_fDia = pDoc->m_fRulesViaDia;
	avia.m_fHoleDia = pDoc->m_fRulesViaHoleDia;
	avia.m_nLayer = MULTILAYER;
	avia.m_nNet = pconnection->m_nNet;	

	double fDxy_1[9][2]={ 0,0, 1,1, 0,1, 1,0, -1,-1, -1,0, -1,1, 0,-1, 1,-1};
	double fDxy_2[25][2]={ 0,0, 1,1, 0,1, 1,0, -1,-1, -1,0, -1,1, 0,-1, 1,-1, 2,0, -2,0, 0,2, 0,-2, 2,1, 2,-1, -2,1, -2,-1, -1,2, 1,2, -1,-2, 1,-2, 2,2, -2,-2, 2,-2, -2,2};
	
	
	for(int i=0; i<9; i++)
	{
		avia.m_fCx = pconnection->m_fX1 + fDxy_1[i][0]*fVia_Step;
		avia.m_fCy = pconnection->m_fY2 + fDxy_1[i][1]*fVia_Step;
		if(FindViaOnLineDrcErrors(&avia) == FALSE)
		{
			COb_pcbconnection aconnection;
			aconnection.m_nNet = pconnection->m_nNet;

			aconnection.m_fX1 = avia.m_fCx;
			aconnection.m_fY1 = avia.m_fCy;
			aconnection.m_nLayer1 = MULTILAYER;
			aconnection.m_fX2 = pconnection->m_fX2;
			aconnection.m_fY2 = pconnection->m_fY2;
			aconnection.m_nLayer2 = pconnection->m_nLayer2;
			
			
			while (!arrayTrack_H.IsEmpty())
			{
				delete arrayTrack_H.GetAt(0);
				arrayTrack_H.RemoveAt(0);
			}
			while (!arrayTrack_V.IsEmpty())
			{
				delete arrayTrack_V.GetAt(0);
				arrayTrack_V.RemoveAt(0);
			}
			if(Fast_Route_H(&aconnection, arrayTrack_H) == TRUE)
			{
				
				aconnection.m_fX2 = pconnection->m_fX1;
				aconnection.m_fY2 = pconnection->m_fY1;
				aconnection.m_nLayer2 = pconnection->m_nLayer1;
				if(Fast_Route_V(&aconnection, arrayTrack_V) == TRUE)
				{
					COb_pcbvia* pnewvia = new COb_pcbvia();
					pnewvia->Copy(&avia);
					int index = pDoc->m_arrayPcbVia.Add(pnewvia);	
					pnewvia->Draw(&dc, this, 0);
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					for(int k=0; k<arrayTrack_H.GetCount(); k++)
					{
						COb_pcbtrack* pnewtrack = arrayTrack_H.GetAt(k);
						index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		
						pnewtrack->Draw(&dc, this);
						pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					}
					
					for(int k=0; k<arrayTrack_V.GetCount(); k++)
					{
						COb_pcbtrack* pnewtrack = arrayTrack_V.GetAt(k);
						index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		
						pnewtrack->Draw(&dc, this);
						pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					}

					return TRUE;
				}
				else break;
			}
			else break;
		}
	}

	
	for(int i=0; i<9; i++)
	{
		avia.m_fCx = pconnection->m_fX2 + fDxy_1[i][0]*fVia_Step;
		avia.m_fCy = pconnection->m_fY1 + fDxy_1[i][1]*fVia_Step;
		if(FindViaOnLineDrcErrors(&avia) == FALSE)
		{
			COb_pcbconnection aconnection;
			aconnection.m_nNet = pconnection->m_nNet;

			aconnection.m_fX1 = avia.m_fCx;
			aconnection.m_fY1 = avia.m_fCy;
			aconnection.m_nLayer1 = MULTILAYER;
			aconnection.m_fX2 = pconnection->m_fX2;
			aconnection.m_fY2 = pconnection->m_fY2;
			aconnection.m_nLayer2 = pconnection->m_nLayer2;

			
			while (!arrayTrack_H.IsEmpty())
			{
				delete arrayTrack_H.GetAt(0);
				arrayTrack_H.RemoveAt(0);
			}
			while (!arrayTrack_V.IsEmpty())
			{
				delete arrayTrack_V.GetAt(0);
				arrayTrack_V.RemoveAt(0);
			}
			if(Fast_Route_V(&aconnection, arrayTrack_V) == TRUE)
			{
				
				aconnection.m_fX2 = pconnection->m_fX1;
				aconnection.m_fY2 = pconnection->m_fY1;
				aconnection.m_nLayer2 = pconnection->m_nLayer1;
				if(Fast_Route_H(&aconnection, arrayTrack_H) == TRUE)
				{
					COb_pcbvia* pnewvia = new COb_pcbvia();
					pnewvia->Copy(&avia);
					int index = pDoc->m_arrayPcbVia.Add(pnewvia);	
					pnewvia->Draw(&dc, this, 0);
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					for(int k=0; k<arrayTrack_H.GetCount(); k++)
					{
						COb_pcbtrack* pnewtrack = arrayTrack_H.GetAt(k);
						index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		
						pnewtrack->Draw(&dc, this);
						pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					}
					
					for(int k=0; k<arrayTrack_V.GetCount(); k++)
					{
						COb_pcbtrack* pnewtrack = arrayTrack_V.GetAt(k);
						index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		
						pnewtrack->Draw(&dc, this);
						pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					}
					return TRUE;
				}
			}
		}
	}


					
	
	while (!arrayTrack_H.IsEmpty())
	{
		delete arrayTrack_H.GetAt(0);
		arrayTrack_H.RemoveAt(0);
	}
	while (!arrayTrack_V.IsEmpty())
	{
		delete arrayTrack_V.GetAt(0);
		arrayTrack_V.RemoveAt(0);
	}
	return FALSE;
}

BOOL CESDView::Route_Z(COb_pcbconnection* pconnection)
{
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	double fVia_Step = 50;
	double fDelta = 100;
	if((fabs(pconnection->m_fX1 - pconnection->m_fX2) < fDelta) || (fabs(pconnection->m_fY1 - pconnection->m_fY2) < fDelta)) return FALSE;

	
	CTypedPtrArray <CObArray, COb_pcbtrack*> arrayTrack_1;
	CTypedPtrArray <CObArray, COb_pcbtrack*> arrayTrack_2;
	CTypedPtrArray <CObArray, COb_pcbtrack*> arrayTrack_3;
				
	COb_pcbvia avia1, avia2;
	avia1.pDocument = pDoc;
	avia1.m_nStartLayer = TOPLAYER;
	avia1.m_nEndLayer = BOTTOMLAYER;
	avia1.m_fDia = pDoc->m_fRulesViaDia;
	avia1.m_fHoleDia = pDoc->m_fRulesViaHoleDia;
	avia1.m_nLayer = MULTILAYER;
	avia1.m_nNet = pconnection->m_nNet;	
	
	avia2.Copy(&avia1);

	double delta1[3] = { 0,-1,1}, delta2[3]= {0,1,-1};
	double fDxy_1[9][2]={ 0,0, 1,1, 0,1, 1,0, -1,-1, -1,0, -1,1, 0,-1, 1,-1};
	
	
	if(pconnection->m_fX1 > pconnection->m_fX2)
	{
		double fx, fy;
		int l;
		fx = pconnection->m_fX1;
		fy = pconnection->m_fY1;
		l = pconnection->m_nLayer1;
		pconnection->m_fX1 = pconnection->m_fX2;
		pconnection->m_fY1 = pconnection->m_fY2;
		pconnection->m_nLayer1 = pconnection->m_nLayer2;
		pconnection->m_fX2 = fx;
		pconnection->m_fY2 = fy;
		pconnection->m_nLayer2 = l;
	}	
	int num = 0;
	for(;;)
	{
		num++;
		avia1.m_fCx = pconnection->m_fX1 + num*fVia_Step;
		if(avia1.m_fCx > pconnection->m_fX2) break;

		int flag_via1 = 0;
		for(int i=0; i<3; i++)
		{
			avia1.m_fCy = pconnection->m_fY1 + delta1[i]*fVia_Step;
			if(FindViaOnLineDrcErrors(&avia1) == FALSE)
			{
				flag_via1 = 1;
				break;
			}
		}

		if(flag_via1 == 1)
		{
			COb_pcbconnection aconnection;
			aconnection.m_nNet = pconnection->m_nNet;

			aconnection.m_fX1 = avia1.m_fCx;
			aconnection.m_fY1 = avia1.m_fCy;
			aconnection.m_nLayer1 = MULTILAYER;
			aconnection.m_fX2 = pconnection->m_fX1;
			aconnection.m_fY2 = pconnection->m_fY1;
			aconnection.m_nLayer2 = pconnection->m_nLayer1;
			
			
			while (!arrayTrack_1.IsEmpty())
			{
				delete arrayTrack_1.GetAt(0);
				arrayTrack_1.RemoveAt(0);
			}
			if(Fast_Route_H(&aconnection, arrayTrack_1) == FALSE) break;
			
			
			int flag_via2 = 0;
			for(int i=0; i<9; i++)
			{
				avia2.m_fCx = avia1.m_fCx + fDxy_1[i][0]*fVia_Step;
				avia2.m_fCy = pconnection->m_fY2 + fDxy_1[i][1]*fVia_Step;
				if(avia2.m_fCx > pconnection->m_fX2) break;
				
				if(FindViaOnLineDrcErrors(&avia2) == FALSE)
				{
					flag_via2 = 1;
					break;
				}
			}

			if(flag_via2 == 1)
			{
				aconnection.m_fX1 = avia2.m_fCx;
				aconnection.m_fY1 = avia2.m_fCy;
				aconnection.m_nLayer1 = MULTILAYER;
				aconnection.m_fX2 = pconnection->m_fX2;
				aconnection.m_fY2 = pconnection->m_fY2;
				aconnection.m_nLayer2 = pconnection->m_nLayer2;
				
				
				while (!arrayTrack_3.IsEmpty())
				{
					delete arrayTrack_3.GetAt(0);
					arrayTrack_3.RemoveAt(0);
				}
				if(Fast_Route_H(&aconnection, arrayTrack_3) == FALSE) continue;

				aconnection.m_fX1 = avia1.m_fCx;
				aconnection.m_fY1 = avia1.m_fCy;
				aconnection.m_nLayer1 = MULTILAYER;
				aconnection.m_fX2 = avia2.m_fCx;
				aconnection.m_fY2 = avia2.m_fCy;
				aconnection.m_nLayer2 = MULTILAYER;
				
				
				while (!arrayTrack_2.IsEmpty())
				{
					delete arrayTrack_2.GetAt(0);
					arrayTrack_2.RemoveAt(0);
				}
				if(Fast_Route_V(&aconnection, arrayTrack_2) == TRUE)
				{
					COb_pcbvia* pnewvia = new COb_pcbvia();
					pnewvia->Copy(&avia1);
					int index = pDoc->m_arrayPcbVia.Add(pnewvia);	
					pnewvia->Draw(&dc, this, 0);
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					pnewvia = new COb_pcbvia();
					pnewvia->Copy(&avia2);
					index = pDoc->m_arrayPcbVia.Add(pnewvia);
					pnewvia->Draw(&dc, this, 0);
					pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					for(int k=0; k<arrayTrack_1.GetCount(); k++)
					{
						COb_pcbtrack* pnewtrack = arrayTrack_1.GetAt(k);
						index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		
						pnewtrack->Draw(&dc, this);
						pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					}
					for(int k=0; k<arrayTrack_2.GetCount(); k++)
					{
						COb_pcbtrack* pnewtrack = arrayTrack_2.GetAt(k);
						index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		
						pnewtrack->Draw(&dc, this);
						pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					}					
					for(int k=0; k<arrayTrack_3.GetCount(); k++)
					{
						COb_pcbtrack* pnewtrack = arrayTrack_3.GetAt(k);
						index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		
						pnewtrack->Draw(&dc, this);
						pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					}

					return TRUE;
				}
			}

		}
	}

	
	if(pconnection->m_fY1 > pconnection->m_fY2)
	{
		double fx, fy;
		int l;
		fx = pconnection->m_fX1;
		fy = pconnection->m_fY1;
		l = pconnection->m_nLayer1;
		pconnection->m_fX1 = pconnection->m_fX2;
		pconnection->m_fY1 = pconnection->m_fY2;
		pconnection->m_nLayer1 = pconnection->m_nLayer2;
		pconnection->m_fX2 = fx;
		pconnection->m_fY2 = fy;
		pconnection->m_nLayer2 = l;
	}	
	num = 0;
	for(;;)
	{
		num++;
		avia1.m_fCy = pconnection->m_fY1 + num*fVia_Step;
		if(avia1.m_fCy > pconnection->m_fY2) break;

		int flag_via1 = 0;
		for(int i=0; i<3; i++)
		{
			avia1.m_fCx = pconnection->m_fX1 + delta1[i]*fVia_Step;
			if(FindViaOnLineDrcErrors(&avia1) == FALSE)
			{
				flag_via1 = 1;
				break;
			}
		}

		if(flag_via1 == 1)
		{
			COb_pcbconnection aconnection;
			aconnection.m_nNet = pconnection->m_nNet;

			aconnection.m_fX1 = avia1.m_fCx;
			aconnection.m_fY1 = avia1.m_fCy;
			aconnection.m_nLayer1 = MULTILAYER;
			aconnection.m_fX2 = pconnection->m_fX1;
			aconnection.m_fY2 = pconnection->m_fY1;
			aconnection.m_nLayer2 = pconnection->m_nLayer1;
			
			
			while (!arrayTrack_1.IsEmpty())
			{
				delete arrayTrack_1.GetAt(0);
				arrayTrack_1.RemoveAt(0);
			}
			if(Fast_Route_V(&aconnection, arrayTrack_1) == FALSE) break;
			
			
			int flag_via2 = 0;
			for(int i=0; i<9; i++)
			{
				avia2.m_fCx = pconnection->m_fX2 + fDxy_1[i][0]*fVia_Step;
				avia2.m_fCy = avia1.m_fCy + fDxy_1[i][1]*fVia_Step;
				if(avia2.m_fCy > pconnection->m_fY2) break;
				
				if(FindViaOnLineDrcErrors(&avia2) == FALSE)
				{
					flag_via2 = 1;
					break;
				}
			}

			if(flag_via2 == 1)
			{
				aconnection.m_fX1 = avia2.m_fCx;
				aconnection.m_fY1 = avia2.m_fCy;
				aconnection.m_nLayer1 = MULTILAYER;
				aconnection.m_fX2 = pconnection->m_fX2;
				aconnection.m_fY2 = pconnection->m_fY2;
				aconnection.m_nLayer2 = pconnection->m_nLayer2;
				
				
				while (!arrayTrack_3.IsEmpty())
				{
					delete arrayTrack_3.GetAt(0);
					arrayTrack_3.RemoveAt(0);
				}
				if(Fast_Route_V(&aconnection, arrayTrack_3) == FALSE) continue;

				aconnection.m_fX1 = avia1.m_fCx;
				aconnection.m_fY1 = avia1.m_fCy;
				aconnection.m_nLayer1 = MULTILAYER;
				aconnection.m_fX2 = avia2.m_fCx;
				aconnection.m_fY2 = avia2.m_fCy;
				aconnection.m_nLayer2 = MULTILAYER;
				
				
				while (!arrayTrack_2.IsEmpty())
				{
					delete arrayTrack_2.GetAt(0);
					arrayTrack_2.RemoveAt(0);
				}
				if(Fast_Route_H(&aconnection, arrayTrack_2) == TRUE)
				{
					COb_pcbvia* pnewvia = new COb_pcbvia();
					pnewvia->Copy(&avia1);
					int index = pDoc->m_arrayPcbVia.Add(pnewvia);	
					pnewvia->Draw(&dc, this, 0);
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					pnewvia = new COb_pcbvia();
					pnewvia->Copy(&avia2);
					index = pDoc->m_arrayPcbVia.Add(pnewvia);
					pnewvia->Draw(&dc, this, 0);
					pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					for(int k=0; k<arrayTrack_1.GetCount(); k++)
					{
						COb_pcbtrack* pnewtrack = arrayTrack_1.GetAt(k);
						index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		
						pnewtrack->Draw(&dc, this);
						pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					}
					for(int k=0; k<arrayTrack_2.GetCount(); k++)
					{
						COb_pcbtrack* pnewtrack = arrayTrack_2.GetAt(k);
						index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		
						pnewtrack->Draw(&dc, this);
						pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					}					
					for(int k=0; k<arrayTrack_3.GetCount(); k++)
					{
						COb_pcbtrack* pnewtrack = arrayTrack_3.GetAt(k);
						index = pDoc->m_arrayPcbTrack.Add(pnewtrack);		
						pnewtrack->Draw(&dc, this);
						pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					}

					return TRUE;
				}
			}

		}
	}
	
	
	while (!arrayTrack_1.IsEmpty())
	{
		delete arrayTrack_1.GetAt(0);
		arrayTrack_1.RemoveAt(0);
	}
	while (!arrayTrack_2.IsEmpty())
	{
		delete arrayTrack_2.GetAt(0);
		arrayTrack_2.RemoveAt(0);
	}
	while (!arrayTrack_3.IsEmpty())
	{
		delete arrayTrack_3.GetAt(0);
		arrayTrack_3.RemoveAt(0);
	}
	return FALSE;
}

BOOL CESDView::Route_C(COb_pcbconnection* pconnection)
{
	return FALSE;
}

BOOL CESDView::Fast_Route_H(COb_pcbconnection* pconnection, CTypedPtrArray <CObArray, COb_pcbtrack*>& arrayTrack_H)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		int layer = pDoc->m_arrayPcbLayerShow.GetAt(i);
		if((layer >= TOPLAYER) && (layer <= BOTTOMLAYER))
		{
			if((pDoc->m_nRulesLayer[layer] == HORIZONTAL_ROUTE)||(pDoc->m_nRulesLayer[layer] == ANY_ROUTE))
			{
				if( ((pconnection->m_nLayer1 != MULTILAYER)&&(pconnection->m_nLayer1 != layer))||\
					((pconnection->m_nLayer2 != MULTILAYER)&&(pconnection->m_nLayer2 != layer)) ) continue;
				
				
				COb_pcbtrack atrack[5];
				for(int j=0; j<5; j++)
				{
					atrack[j].pDocument = pDoc;
					atrack[j].m_nLayer = layer;
					atrack[j].m_fWidth = pDoc->m_fRulesTrackWidth;
					atrack[j].m_nNet = pconnection->m_nNet;
				}
				
				
				if(pconnection->m_fX1 > pconnection->m_fX2)
				{
					double fx, fy;
					int l;
					fx = pconnection->m_fX1;
					fy = pconnection->m_fY1;
					l = pconnection->m_nLayer1;
					pconnection->m_fX1 = pconnection->m_fX2;
					pconnection->m_fY1 = pconnection->m_fY2;
					pconnection->m_nLayer1 = pconnection->m_nLayer2;
					pconnection->m_fX2 = fx;
					pconnection->m_fY2 = fy;
					pconnection->m_nLayer2 = l;
				}				
				
				if(pconnection->m_fY1 == pconnection->m_fY2)
				{
					
					atrack[0].m_fX1 = pconnection->m_fX1;
					atrack[0].m_fY1 = pconnection->m_fY1;
					atrack[0].m_fX2 = pconnection->m_fX2;
					atrack[0].m_fY2 = pconnection->m_fY2;

					if(FindTrackOnLineDrcErrors(&atrack[0]) == FALSE)
					{
						COb_pcbtrack* pnewtrack = new COb_pcbtrack();
						pnewtrack->Copy(&atrack[0]);
						arrayTrack_H.Add(pnewtrack);
						return TRUE;
					}
				}
				else
				{
									
					double delta_x = fabs(pconnection->m_fX2 - pconnection->m_fX1);
					double delta_y = fabs(pconnection->m_fY2 - pconnection->m_fY1);
					for(int j=0; j<=(int)(delta_x/2); j++)
					{
						
						if((pconnection->m_fX1 + delta_x/2 + j + delta_y/2) < pconnection->m_fX2)
						{
							atrack[0].m_fX1 = pconnection->m_fX1;
							atrack[0].m_fY1 = pconnection->m_fY1;
							atrack[0].m_fX2 = pconnection->m_fX1 + delta_x/2 + j - delta_y/2;
							atrack[0].m_fY2 = pconnection->m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE) break;		

							atrack[2].m_fX1 = pconnection->m_fX1 + delta_x/2 + j + delta_y/2;
							atrack[2].m_fY1 = pconnection->m_fY2;	
							atrack[2].m_fX2 = pconnection->m_fX2;
							atrack[2].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[2]) == TRUE) continue;
						
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) continue;

							COb_pcbtrack* pnewtrack;
							
							pnewtrack = new COb_pcbtrack();
							pnewtrack->Copy(&atrack[0]);
							arrayTrack_H.Add(pnewtrack);
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[1]);						
							arrayTrack_H.Add(pnewtrack);
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[2]);						
							arrayTrack_H.Add(pnewtrack);
							return TRUE;
						}
						else
						{
							atrack[0].m_fX1 = pconnection->m_fX1;
							atrack[0].m_fY1 = pconnection->m_fY1;
							atrack[0].m_fX2 = pconnection->m_fX2 - delta_y;
							atrack[0].m_fY2 = pconnection->m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE) break;
							
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = pconnection->m_fX2;
							atrack[1].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) break;

							COb_pcbtrack* pnewtrack;
							
							pnewtrack = new COb_pcbtrack();
							pnewtrack->Copy(&atrack[0]);	
							arrayTrack_H.Add(pnewtrack);
							
							pnewtrack = new COb_pcbtrack();
							pnewtrack->Copy(&atrack[1]);
							arrayTrack_H.Add(pnewtrack);
							return TRUE;
						}
					}
					for(int j=0; j<=(int)(delta_x/2); j++)
					{
						
						if((pconnection->m_fX1 + delta_x/2 - j - delta_y/2) > pconnection->m_fX1)
						{
							atrack[2].m_fX1 = pconnection->m_fX1 + delta_x/2 - j + delta_y/2;
							atrack[2].m_fY1 = pconnection->m_fY2;	
							atrack[2].m_fX2 = pconnection->m_fX2;
							atrack[2].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[2]) == TRUE) break;	

							atrack[0].m_fX1 = pconnection->m_fX1;
							atrack[0].m_fY1 = pconnection->m_fY1;
							atrack[0].m_fX2 = pconnection->m_fX1 + delta_x/2 - j - delta_y/2;
							atrack[0].m_fY2 = pconnection->m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE) continue;		
						
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) continue;

							COb_pcbtrack* pnewtrack;
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[0]);
							arrayTrack_H.Add(pnewtrack);
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[1]);
							arrayTrack_H.Add(pnewtrack);
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[2]);
							arrayTrack_H.Add(pnewtrack);
							return TRUE;
						}
						else
						{
							atrack[2].m_fX1 = pconnection->m_fX1 + delta_y;
							atrack[2].m_fY1 = pconnection->m_fY1;	
							atrack[2].m_fX2 = pconnection->m_fX2;
							atrack[2].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[2]) == TRUE) break;

							atrack[1].m_fX1 = pconnection->m_fX1;
							atrack[1].m_fY1 = pconnection->m_fY1;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) break;

							COb_pcbtrack* pnewtrack;
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[1]);
							arrayTrack_H.Add(pnewtrack);
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[2]);
							arrayTrack_H.Add(pnewtrack);
							return TRUE;
						}
					}
				}
					
				
				double fx1, fy1, fx2, fy2;
				fx1 = pconnection->m_fX1;
				fy1 = pconnection->m_fY1;
				fx2 = pconnection->m_fX2;
				fy2 = pconnection->m_fY1;
				
				atrack[0].m_fX1 = fx1;
				atrack[0].m_fY1 = fy1;
				atrack[0].m_fX2 = fx2;
				atrack[0].m_fY2 = fy2;
				if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE)	
				{
					double ftemp_x1, ftemp_y1, ftemp_x2, ftemp_y2, ftemp_x, ftemp_y;
					ftemp_x1 = fx1;		
					ftemp_y1 = fy1;
					ftemp_x2 = fx2;
					ftemp_y2 = fy2;
					ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
					ftemp_y = (ftemp_y1 + ftemp_y2)/2;
					atrack[0].m_fX2 = ftemp_x;
					atrack[0].m_fY2 = ftemp_y;
					double f_x2, f_y2;
					f_x2 = fx1;
					f_y2 = fy1;
					for(;;)
					{
						if(FindTrackOnLineDrcErrors(&atrack[0]) == FALSE)	
						{
							f_x2 = atrack[0].m_fX2;
							f_y2 = atrack[0].m_fY2;
							ftemp_x1 = ftemp_x;	
							ftemp_y1 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[0].m_fX2 = ftemp_x;
							atrack[0].m_fY2 = ftemp_y;
						}
						else if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE)	
						{
							ftemp_x2 = ftemp_x;	
							ftemp_y2 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[0].m_fX2 = ftemp_x;
							atrack[0].m_fY2 = ftemp_y;
						}
						if((fabs(ftemp_x2 - ftemp_x1) < 1)&&(fabs(ftemp_y2 - ftemp_y1) < 1))
						{
							atrack[0].m_fX2 = f_x2;
							atrack[0].m_fY2 = f_y2;
							break;
						}
					}
				}
				
				fx1 = pconnection->m_fX2;
				fy1 = pconnection->m_fY2;
				fx2 = pconnection->m_fX1;
				fy2 = pconnection->m_fY2;

				atrack[4].m_fX1 = fx1;
				atrack[4].m_fY1 = fy1;
				atrack[4].m_fX2 = fx2;
				atrack[4].m_fY2 = fy2;
				
				if(FindTrackOnLineDrcErrors(&atrack[4]) == TRUE)	
				{
					double ftemp_x1, ftemp_y1, ftemp_x2, ftemp_y2, ftemp_x, ftemp_y;
					ftemp_x1 = fx1;		
					ftemp_y1 = fy1;
					ftemp_x2 = fx2;
					ftemp_y2 = fy2;
					ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
					ftemp_y = (ftemp_y1 + ftemp_y2)/2;
					atrack[4].m_fX2 = ftemp_x;
					atrack[4].m_fY2 = ftemp_y;
					double f_x2, f_y2;
					f_x2 = fx1;
					f_y2 = fy1;
					for(;;)
					{
						if(FindTrackOnLineDrcErrors(&atrack[4]) == FALSE)	
						{
							f_x2 = atrack[4].m_fX2;
							f_y2 = atrack[4].m_fY2;
							ftemp_x1 = ftemp_x;	
							ftemp_y1 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[4].m_fX2 = ftemp_x;
							atrack[4].m_fY2 = ftemp_y;
						}
						else if(FindTrackOnLineDrcErrors(&atrack[4]) == TRUE)	
						{
							ftemp_x2 = ftemp_x;	
							ftemp_y2 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[4].m_fX2 = ftemp_x;
							atrack[4].m_fY2 = ftemp_y;
						}
						if((fabs(ftemp_x2 - ftemp_x1) < 1)&&(fabs(ftemp_y2 - ftemp_y1) < 1))
						{
							atrack[4].m_fX2 = f_x2;
							atrack[4].m_fY2 = f_y2;
							break;
						}
					}
				}

				
				if( fabs(pconnection->m_fY1 - pconnection->m_fY2) > 50 )
				{
					
					int nDelta = fabs(pconnection->m_fY1 - pconnection->m_fY2);
					double fy_start = MAX(pconnection->m_fY1, pconnection->m_fY2);
					for(int k=0; k<2; k++)
					{
						for(int j=1;  j<nDelta; j++)
						{
							atrack[2].m_fY1 = atrack[2].m_fY2 = fy_start - j;
							if(k == 0) 
							{
								atrack[2].m_fX1 = atrack[0].m_fX2 + fabs(atrack[0].m_fY2 - atrack[2].m_fY1);
								atrack[2].m_fX2 = atrack[4].m_fX2 - fabs(atrack[4].m_fY2 - atrack[2].m_fY2);
								if(atrack[2].m_fX1 > atrack[2].m_fX2) break;
							}
							else if(k == 1) 
							{
								atrack[2].m_fX1 = atrack[0].m_fX2 + fabs(atrack[0].m_fY2 - atrack[2].m_fY1)/2;
								atrack[2].m_fX2 = atrack[4].m_fX2 - fabs(atrack[4].m_fY2 - atrack[2].m_fY2)/2;
								if( ((atrack[0].m_fX2 - fabs(atrack[2].m_fY1 - atrack[0].m_fY1)/2) < atrack[0].m_fX1) ||\
									((atrack[4].m_fX2 + fabs(atrack[2].m_fY1 - atrack[4].m_fY1)/2) > atrack[4].m_fX1) ) break;
							}
					
							if(FindTrackOnLineDrcErrors(&atrack[2]) == FALSE)
							{
								atrack[0].m_fX2 = atrack[2].m_fX1 - fabs(atrack[2].m_fY1 - atrack[0].m_fY1);
								atrack[4].m_fX2 = atrack[2].m_fX2 + fabs(atrack[2].m_fY1 - atrack[4].m_fY1);
								
								atrack[1].m_fX1 = atrack[0].m_fX2;
								atrack[1].m_fY1 = atrack[0].m_fY2;
								atrack[1].m_fX2 = atrack[2].m_fX1;
								atrack[1].m_fY2 = atrack[2].m_fY1;

								atrack[3].m_fX1 = atrack[2].m_fX2;
								atrack[3].m_fY1 = atrack[2].m_fY2;
								atrack[3].m_fX2 = atrack[4].m_fX2;
								atrack[3].m_fY2 = atrack[4].m_fY2;
								
								if((FindTrackOnLineDrcErrors(&atrack[1]) == FALSE)&&(FindTrackOnLineDrcErrors(&atrack[3]) == FALSE))
								{

									COb_pcbtrack* pnewtrack;
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[0]);
									arrayTrack_H.Add(pnewtrack);
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[1]);
									arrayTrack_H.Add(pnewtrack);
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[2]);
									arrayTrack_H.Add(pnewtrack);
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[3]);
									arrayTrack_H.Add(pnewtrack);
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[4]);
									arrayTrack_H.Add(pnewtrack);
									return TRUE;
								}
							}
						}
					}
				}
				
				int nDelta = 50;
				double fy_start = MAX(pconnection->m_fY1, pconnection->m_fY2);
				for(int k=0; k<2; k++)
				{
					for(int j=1;  j<nDelta; j++)
					{
						atrack[2].m_fY1 = atrack[2].m_fY2 = fy_start + j;
						if(k == 0) 
						{
							atrack[2].m_fX1 = atrack[0].m_fX2 + fabs(atrack[0].m_fY2 - atrack[2].m_fY1);
							atrack[2].m_fX2 = atrack[4].m_fX2 - fabs(atrack[4].m_fY2 - atrack[2].m_fY2);
							if(atrack[2].m_fX1 > atrack[2].m_fX2) break;
						}
						else if(k == 1) 
						{
							atrack[2].m_fX1 = atrack[0].m_fX2 + fabs(atrack[0].m_fY2 - atrack[2].m_fY1)/2;
							atrack[2].m_fX2 = atrack[4].m_fX2 - fabs(atrack[4].m_fY2 - atrack[2].m_fY2)/2;
							if( ((atrack[0].m_fX2 - fabs(atrack[2].m_fY1 - atrack[0].m_fY1)/2) < atrack[0].m_fX1) ||\
								((atrack[4].m_fX2 + fabs(atrack[2].m_fY1 - atrack[4].m_fY1)/2) > atrack[4].m_fX1) ) break;
						}

						if(FindTrackOnLineDrcErrors(&atrack[2]) == FALSE)
						{
							atrack[0].m_fX2 = atrack[2].m_fX1 - fabs(atrack[2].m_fY1 - atrack[0].m_fY1);
							atrack[4].m_fX2 = atrack[2].m_fX2 + fabs(atrack[2].m_fY1 - atrack[4].m_fY1);
							
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;

							atrack[3].m_fX1 = atrack[2].m_fX2;
							atrack[3].m_fY1 = atrack[2].m_fY2;
							atrack[3].m_fX2 = atrack[4].m_fX2;
							atrack[3].m_fY2 = atrack[4].m_fY2;

							if((FindTrackOnLineDrcErrors(&atrack[1]) == FALSE)&&(FindTrackOnLineDrcErrors(&atrack[3]) == FALSE))
							{
								COb_pcbtrack* pnewtrack;
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[0]);
								arrayTrack_H.Add(pnewtrack);
								
								pnewtrack = new COb_pcbtrack();
								pnewtrack->Copy(&atrack[1]);
								arrayTrack_H.Add(pnewtrack);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[2]);
								arrayTrack_H.Add(pnewtrack);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[3]);
								arrayTrack_H.Add(pnewtrack);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[4]);
								arrayTrack_H.Add(pnewtrack);
								return TRUE;
							}
						}
					}
				}


				
				fy_start = MIN(pconnection->m_fY1, pconnection->m_fY2);
				for(int k=0; k<2; k++)
				{
					for(int j=1;  j<nDelta; j++)
					{
						atrack[2].m_fY1 = atrack[2].m_fY2 = fy_start - j;
						if(k == 0) 
						{
							atrack[2].m_fX1 = atrack[0].m_fX2 + fabs(atrack[0].m_fY2 - atrack[2].m_fY1);
							atrack[2].m_fX2 = atrack[4].m_fX2 - fabs(atrack[4].m_fY2 - atrack[2].m_fY2);
							if(atrack[2].m_fX1 > atrack[2].m_fX2) break;
						}
						else if(k == 1) 
						{
							atrack[2].m_fX1 = atrack[0].m_fX2 + fabs(atrack[0].m_fY2 - atrack[2].m_fY1)/2;
							atrack[2].m_fX2 = atrack[4].m_fX2 - fabs(atrack[4].m_fY2 - atrack[2].m_fY2)/2;
							if( ((atrack[0].m_fX2 - fabs(atrack[2].m_fY1 - atrack[0].m_fY1)/2) < atrack[0].m_fX1) ||\
								((atrack[4].m_fX2 + fabs(atrack[2].m_fY1 - atrack[4].m_fY1)/2) > atrack[4].m_fX1) )break;
						}

						
						if(FindTrackOnLineDrcErrors(&atrack[2]) == FALSE)
						{
							atrack[0].m_fX2 = atrack[2].m_fX1 - fabs(atrack[2].m_fY1 - atrack[0].m_fY1);
							atrack[4].m_fX2 = atrack[2].m_fX2 + fabs(atrack[2].m_fY1 - atrack[4].m_fY1);
							
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;

							atrack[3].m_fX1 = atrack[2].m_fX2;
							atrack[3].m_fY1 = atrack[2].m_fY2;
							atrack[3].m_fX2 = atrack[4].m_fX2;
							atrack[3].m_fY2 = atrack[4].m_fY2;

							if((FindTrackOnLineDrcErrors(&atrack[1]) == FALSE)&&(FindTrackOnLineDrcErrors(&atrack[3]) == FALSE))
							{
								COb_pcbtrack* pnewtrack;
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[0]);
								arrayTrack_H.Add(pnewtrack);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[1]);
								arrayTrack_H.Add(pnewtrack);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[2]);
								arrayTrack_H.Add(pnewtrack);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[3]);
								arrayTrack_H.Add(pnewtrack);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[4]);
								arrayTrack_H.Add(pnewtrack);
								return TRUE;
							}
						}
					}
				}
			}
		}
	}

	return FALSE;
}

BOOL CESDView::Fast_Route_V(COb_pcbconnection* pconnection, CTypedPtrArray <CObArray, COb_pcbtrack*>& arrayTrack_V)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		int layer = pDoc->m_arrayPcbLayerShow.GetAt(i);
		if((layer >= TOPLAYER) && (layer <= BOTTOMLAYER))
		{
			if((pDoc->m_nRulesLayer[layer] == VERTICAL_ROUTE)||(pDoc->m_nRulesLayer[layer] == ANY_ROUTE))
			{
				if( ((pconnection->m_nLayer1 != MULTILAYER)&&(pconnection->m_nLayer1 != layer))||\
					((pconnection->m_nLayer2 != MULTILAYER)&&(pconnection->m_nLayer2 != layer)) ) continue;
				
				
				COb_pcbtrack atrack[5];
				for(int j=0; j<5; j++)
				{
					atrack[j].pDocument = pDoc;
					atrack[j].m_nLayer = layer;
					atrack[j].m_fWidth = pDoc->m_fRulesTrackWidth;
					atrack[j].m_nNet = pconnection->m_nNet;
				}
				
				
				if(pconnection->m_fY1 > pconnection->m_fY2)
				{
					double fx, fy;
					int l;
					fx = pconnection->m_fX1;
					fy = pconnection->m_fY1;
					l = pconnection->m_nLayer1;
					pconnection->m_fX1 = pconnection->m_fX2;
					pconnection->m_fY1 = pconnection->m_fY2;
					pconnection->m_nLayer1 = pconnection->m_nLayer2;
					pconnection->m_fX2 = fx;
					pconnection->m_fY2 = fy;
					pconnection->m_nLayer2 = l;
				}				
				
				if(pconnection->m_fX1 == pconnection->m_fX2)
				{
					
					atrack[0].m_fX1 = pconnection->m_fX1;
					atrack[0].m_fY1 = pconnection->m_fY1;
					atrack[0].m_fX2 = pconnection->m_fX2;
					atrack[0].m_fY2 = pconnection->m_fY2;

					if(FindTrackOnLineDrcErrors(&atrack[0]) == FALSE)
					{
						COb_pcbtrack* pnewtrack = new COb_pcbtrack();
						pnewtrack->Copy(&atrack[0]);
						arrayTrack_V.Add(pnewtrack);
						return TRUE;
					}
				}
				else
				{
									
					double delta_x = fabs(pconnection->m_fX2 - pconnection->m_fX1);
					double delta_y = fabs(pconnection->m_fY2 - pconnection->m_fY1);
					for(int j=0; j<=(int)(delta_y/2); j++)
					{
						
						if((pconnection->m_fY1 + delta_y/2 + j + delta_x/2) < pconnection->m_fY2)
						{
							atrack[0].m_fX1 = pconnection->m_fX1;
							atrack[0].m_fY1 = pconnection->m_fY1;
							atrack[0].m_fX2 = pconnection->m_fX1;
							atrack[0].m_fY2 = pconnection->m_fY1 + delta_y/2 + j - delta_x/2;
							if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE) break;		

							atrack[2].m_fX1 = pconnection->m_fX2;
							atrack[2].m_fY1 = pconnection->m_fY1 + delta_y/2 + j + delta_x/2;
							atrack[2].m_fX2 = pconnection->m_fX2;
							atrack[2].m_fY2 = pconnection->m_fY2;
							if(FindTrackOnLineDrcErrors(&atrack[2]) == TRUE) continue;
						
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) continue;

							COb_pcbtrack* pnewtrack;
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[0]);
							arrayTrack_V.Add(pnewtrack);
							
							pnewtrack = new COb_pcbtrack();		
							pnewtrack->Copy(&atrack[1]);
							arrayTrack_V.Add(pnewtrack);
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[2]);
							arrayTrack_V.Add(pnewtrack);
							return TRUE;
						}
						else
						{
							atrack[0].m_fX1 = pconnection->m_fX1;
							atrack[0].m_fY1 = pconnection->m_fY1;
							atrack[0].m_fX2 = pconnection->m_fX1;
							atrack[0].m_fY2 = pconnection->m_fY2 - delta_x;
							if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE) break;
							
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = pconnection->m_fX2;
							atrack[1].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) break;

							COb_pcbtrack* pnewtrack;
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[0]);
							arrayTrack_V.Add(pnewtrack);
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[1]);
							arrayTrack_V.Add(pnewtrack);
							return TRUE;
						}
					}
					for(int j=0; j<=(int)(delta_y/2); j++)
					{
						
						if((pconnection->m_fY1 + delta_y/2 - j - delta_x/2) > pconnection->m_fY1)
						{
							atrack[2].m_fX1 = pconnection->m_fX2;
							atrack[2].m_fY1 = pconnection->m_fY1 + delta_y/2 - j + delta_x/2;
							atrack[2].m_fX2 = pconnection->m_fX2;
							atrack[2].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[2]) == TRUE) break;	

							atrack[0].m_fX1 = pconnection->m_fX1;
							atrack[0].m_fY1 = pconnection->m_fY1;
							atrack[0].m_fX2 = pconnection->m_fX1;
							atrack[0].m_fY2 = pconnection->m_fY1 + delta_y/2 - j - delta_x/2;
							if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE) continue;		
						
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) continue;

							COb_pcbtrack* pnewtrack;
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[0]);
							arrayTrack_V.Add(pnewtrack);
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[1]);
							arrayTrack_V.Add(pnewtrack);
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[2]);
							arrayTrack_V.Add(pnewtrack);
							return TRUE;
						}
						else
						{
							atrack[2].m_fX1 = pconnection->m_fX1;
							atrack[2].m_fY1 = pconnection->m_fY1 + delta_x;
							atrack[2].m_fX2 = pconnection->m_fX2;
							atrack[2].m_fY2 = pconnection->m_fY2;	
							if(FindTrackOnLineDrcErrors(&atrack[2]) == TRUE) break;

							atrack[1].m_fX1 = pconnection->m_fX1;
							atrack[1].m_fY1 = pconnection->m_fY1;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;
							if(FindTrackOnLineDrcErrors(&atrack[1]) == TRUE) break;

							COb_pcbtrack* pnewtrack;
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[1]);
							arrayTrack_V.Add(pnewtrack);
							
							pnewtrack = new COb_pcbtrack();						
							pnewtrack->Copy(&atrack[2]);
							arrayTrack_V.Add(pnewtrack);
							return TRUE;
						}
					}
				}
					
				
				double fx1, fy1, fx2, fy2;
				fx1 = pconnection->m_fX1;
				fy1 = pconnection->m_fY1;
				fx2 = pconnection->m_fX1;
				fy2 = pconnection->m_fY2;
				
				atrack[0].m_fX1 = fx1;
				atrack[0].m_fY1 = fy1;
				atrack[0].m_fX2 = fx2;
				atrack[0].m_fY2 = fy2;
				if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE)	
				{
					double ftemp_x1, ftemp_y1, ftemp_x2, ftemp_y2, ftemp_x, ftemp_y;
					ftemp_x1 = fx1;		
					ftemp_y1 = fy1;
					ftemp_x2 = fx2;
					ftemp_y2 = fy2;
					ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
					ftemp_y = (ftemp_y1 + ftemp_y2)/2;
					atrack[0].m_fX2 = ftemp_x;
					atrack[0].m_fY2 = ftemp_y;
					double f_x2, f_y2;
					f_x2 = fx1;
					f_y2 = fy1;
					for(;;)
					{
						if(FindTrackOnLineDrcErrors(&atrack[0]) == FALSE)	
						{
							f_x2 = atrack[0].m_fX2;
							f_y2 = atrack[0].m_fY2;
							ftemp_x1 = ftemp_x;	
							ftemp_y1 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[0].m_fX2 = ftemp_x;
							atrack[0].m_fY2 = ftemp_y;
						}
						else if(FindTrackOnLineDrcErrors(&atrack[0]) == TRUE)	
						{
							ftemp_x2 = ftemp_x;	
							ftemp_y2 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[0].m_fX2 = ftemp_x;
							atrack[0].m_fY2 = ftemp_y;
						}
						if((fabs(ftemp_x2 - ftemp_x1) < 1)&&(fabs(ftemp_y2 - ftemp_y1) < 1))
						{
							atrack[0].m_fX2 = f_x2;
							atrack[0].m_fY2 = f_y2;
							break;
						}
					}
				}
				
				fx1 = pconnection->m_fX2;
				fy1 = pconnection->m_fY2;
				fx2 = pconnection->m_fX2;
				fy2 = pconnection->m_fY1;

				atrack[4].m_fX1 = fx1;
				atrack[4].m_fY1 = fy1;
				atrack[4].m_fX2 = fx2;
				atrack[4].m_fY2 = fy2;
				
				if(FindTrackOnLineDrcErrors(&atrack[4]) == TRUE)	
				{
					double ftemp_x1, ftemp_y1, ftemp_x2, ftemp_y2, ftemp_x, ftemp_y;
					ftemp_x1 = fx1;		
					ftemp_y1 = fy1;
					ftemp_x2 = fx2;
					ftemp_y2 = fy2;
					ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
					ftemp_y = (ftemp_y1 + ftemp_y2)/2;
					atrack[4].m_fX2 = ftemp_x;
					atrack[4].m_fY2 = ftemp_y;
					double f_x2, f_y2;
					f_x2 = fx1;
					f_y2 = fy1;
					for(;;)
					{
						if(FindTrackOnLineDrcErrors(&atrack[4]) == FALSE)	
						{
							f_x2 = atrack[4].m_fX2;
							f_y2 = atrack[4].m_fY2;
							ftemp_x1 = ftemp_x;	
							ftemp_y1 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[4].m_fX2 = ftemp_x;
							atrack[4].m_fY2 = ftemp_y;
						}
						else if(FindTrackOnLineDrcErrors(&atrack[4]) == TRUE)	
						{
							ftemp_x2 = ftemp_x;	
							ftemp_y2 = ftemp_y;
							ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
							ftemp_y = (ftemp_y1 + ftemp_y2)/2;
							atrack[4].m_fX2 = ftemp_x;
							atrack[4].m_fY2 = ftemp_y;
						}
						if((fabs(ftemp_x2 - ftemp_x1) < 1)&&(fabs(ftemp_y2 - ftemp_y1) < 1))
						{
							atrack[4].m_fX2 = f_x2;
							atrack[4].m_fY2 = f_y2;
							break;
						}
					}
				}

				if( fabs(pconnection->m_fX1 - pconnection->m_fX2) > 50 )
				{
					int nDelta = fabs(pconnection->m_fX1 - pconnection->m_fX2);
					double fx_start = MAX(pconnection->m_fX1, pconnection->m_fX2);
					for(int k=0; k<2; k++)
					{
						for(int j=1;  j<nDelta; j++)
						{
							atrack[2].m_fX1 = atrack[2].m_fX2 = fx_start - j;
							if(k == 0) 
							{
								atrack[2].m_fY1 = atrack[0].m_fY2 + fabs(atrack[0].m_fX2 - atrack[2].m_fX1);
								atrack[2].m_fY2 = atrack[4].m_fY2 - fabs(atrack[4].m_fX2 - atrack[2].m_fX2);
								if(atrack[2].m_fY1 > atrack[2].m_fY2) break;
							}
							else if(k == 1) 
							{
								atrack[2].m_fY1 = atrack[0].m_fY2 + fabs(atrack[0].m_fX2 - atrack[2].m_fX1)/2;
								atrack[2].m_fY2 = atrack[4].m_fY2 - fabs(atrack[4].m_fX2 - atrack[2].m_fX2)/2;
								if( ((atrack[0].m_fY2 - fabs(atrack[2].m_fX1 - atrack[0].m_fX1)/2) < atrack[0].m_fY1) ||\
									((atrack[4].m_fY2 + fabs(atrack[2].m_fX1 - atrack[4].m_fX1)/2) > atrack[4].m_fY1) ) break;
							}

							if(FindTrackOnLineDrcErrors(&atrack[2]) == FALSE)
							{
								atrack[0].m_fY2 = atrack[2].m_fY1 - fabs(atrack[2].m_fX1 - atrack[0].m_fX1);
								atrack[4].m_fY2 = atrack[2].m_fY2 + fabs(atrack[2].m_fX1 - atrack[4].m_fX1);
								
								atrack[1].m_fX1 = atrack[0].m_fX2;
								atrack[1].m_fY1 = atrack[0].m_fY2;
								atrack[1].m_fX2 = atrack[2].m_fX1;
								atrack[1].m_fY2 = atrack[2].m_fY1;

								atrack[3].m_fX1 = atrack[2].m_fX2;
								atrack[3].m_fY1 = atrack[2].m_fY2;
								atrack[3].m_fX2 = atrack[4].m_fX2;
								atrack[3].m_fY2 = atrack[4].m_fY2;

								if((FindTrackOnLineDrcErrors(&atrack[1]) == FALSE)&&(FindTrackOnLineDrcErrors(&atrack[3]) == FALSE))
								{
									COb_pcbtrack* pnewtrack;
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[0]);
									arrayTrack_V.Add(pnewtrack);
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[1]);
									arrayTrack_V.Add(pnewtrack);
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[2]);
									arrayTrack_V.Add(pnewtrack);
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[3]);
									arrayTrack_V.Add(pnewtrack);
									
									pnewtrack = new COb_pcbtrack();						
									pnewtrack->Copy(&atrack[4]);
									arrayTrack_V.Add(pnewtrack);
									return TRUE;
								}
							}
						}
					}
				}
				
				int nDelta = 50;
				
				double fx_start = MIN(pconnection->m_fX1, pconnection->m_fX2);
				for(int k=0; k<2; k++)
				{
					for(int j=1;  j<nDelta; j++)
					{
						atrack[2].m_fX1 = atrack[2].m_fX2 = fx_start - j;
						if(k == 0) 
						{
							atrack[2].m_fY1 = atrack[0].m_fY2 + fabs(atrack[0].m_fX2 - atrack[2].m_fX1);
							atrack[2].m_fY2 = atrack[4].m_fY2 - fabs(atrack[4].m_fX2 - atrack[2].m_fX2);
							if(atrack[2].m_fX1 > atrack[2].m_fX2) break;
						}
						else if(k == 1)
						{
							atrack[2].m_fY1 = atrack[0].m_fY2 + fabs(atrack[0].m_fX2 - atrack[2].m_fX1)/2;
							atrack[2].m_fY2 = atrack[4].m_fY2 - fabs(atrack[4].m_fX2 - atrack[2].m_fX2)/2;
							if( ((atrack[0].m_fY2 - fabs(atrack[2].m_fX1 - atrack[0].m_fX1)/2) < atrack[0].m_fY1) ||\
								((atrack[4].m_fY2 + fabs(atrack[2].m_fX1 - atrack[4].m_fX1)/2) > atrack[4].m_fY1) )break;
						}

						
						if(FindTrackOnLineDrcErrors(&atrack[2]) == FALSE)
						{
							atrack[0].m_fY2 = atrack[2].m_fY1 - fabs(atrack[2].m_fX1 - atrack[0].m_fX1);
							atrack[4].m_fY2 = atrack[2].m_fY2 + fabs(atrack[2].m_fX1 - atrack[4].m_fX1);
							
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;

							atrack[3].m_fX1 = atrack[2].m_fX2;
							atrack[3].m_fY1 = atrack[2].m_fY2;
							atrack[3].m_fX2 = atrack[4].m_fX2;
							atrack[3].m_fY2 = atrack[4].m_fY2;

							if((FindTrackOnLineDrcErrors(&atrack[1]) == FALSE)&&(FindTrackOnLineDrcErrors(&atrack[3]) == FALSE))
							{
								COb_pcbtrack* pnewtrack;
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[0]);
								arrayTrack_V.Add(pnewtrack);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[1]);
								arrayTrack_V.Add(pnewtrack);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[2]);
								arrayTrack_V.Add(pnewtrack);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[3]);
								arrayTrack_V.Add(pnewtrack);
								
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[4]);
								arrayTrack_V.Add(pnewtrack);
								return TRUE;
							}
						}
					}
				}
				
				
				fx_start = MAX(pconnection->m_fX1, pconnection->m_fX2);
				for(int k=0; k<2; k++)
				{
					for(int j=1;  j<nDelta; j++)
					{
						atrack[2].m_fX1 = atrack[2].m_fX2 = fx_start + j;
						if(k == 0) 
						{
							atrack[2].m_fY1 = atrack[0].m_fY2 + fabs(atrack[0].m_fX2 - atrack[2].m_fX1);
							atrack[2].m_fY2 = atrack[4].m_fY2 - fabs(atrack[4].m_fX2 - atrack[2].m_fX2);
							if(atrack[2].m_fX1 > atrack[2].m_fX2) break;
						}
						else if(k == 1)
						{
							atrack[2].m_fY1 = atrack[0].m_fY2 + fabs(atrack[0].m_fX2 - atrack[2].m_fX1)/2;
							atrack[2].m_fY2 = atrack[4].m_fY2 - fabs(atrack[4].m_fX2 - atrack[2].m_fX2)/2;
							if( ((atrack[0].m_fY2 - fabs(atrack[2].m_fX1 - atrack[0].m_fX1)/2) < atrack[0].m_fY1) ||\
								((atrack[4].m_fY2 + fabs(atrack[2].m_fX1 - atrack[4].m_fX1)/2) > atrack[4].m_fY1) ) break;
						}
						
						if(FindTrackOnLineDrcErrors(&atrack[2]) == FALSE)
						{
							atrack[0].m_fY2 = atrack[2].m_fY1 - fabs(atrack[2].m_fX1 - atrack[0].m_fX1);
							atrack[4].m_fY2 = atrack[2].m_fY2 + fabs(atrack[2].m_fX1 - atrack[4].m_fX1);
							
							atrack[1].m_fX1 = atrack[0].m_fX2;
							atrack[1].m_fY1 = atrack[0].m_fY2;
							atrack[1].m_fX2 = atrack[2].m_fX1;
							atrack[1].m_fY2 = atrack[2].m_fY1;

							atrack[3].m_fX1 = atrack[2].m_fX2;
							atrack[3].m_fY1 = atrack[2].m_fY2;
							atrack[3].m_fX2 = atrack[4].m_fX2;
							atrack[3].m_fY2 = atrack[4].m_fY2;

							if((FindTrackOnLineDrcErrors(&atrack[1]) == FALSE)&&(FindTrackOnLineDrcErrors(&atrack[3]) == FALSE))
							{
								COb_pcbtrack* pnewtrack;
								//1
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[0]);
								arrayTrack_V.Add(pnewtrack);
								//2
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[1]);
								arrayTrack_V.Add(pnewtrack);
								//3
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[2]);
								arrayTrack_V.Add(pnewtrack);
								//4
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[3]);
								arrayTrack_V.Add(pnewtrack);
								//5
								pnewtrack = new COb_pcbtrack();						
								pnewtrack->Copy(&atrack[4]);
								arrayTrack_V.Add(pnewtrack);
								return TRUE;
							}

						}
					}
				}
			}
		}
	}

	return FALSE;
}

#if !defined(AFX_SYSTEMEXT_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_)
#define AFX_SYSTEMEXT_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_

#include "sysparam.h"
#include "struct.h"

extern CString csESDViewClassName;
extern CString	csEDA_System_Path;

extern int nEDA_System_State;
extern int nEDA_Set_Mouse_Pos;

extern double fEDA_pi;
extern double fEDA_Prec;
extern double fEDA_AnglePrec;

extern int nEDA_Sheet_State;
extern int nEDA_Sheet_Tab_Width;

extern double fEDA_ScaleMax;
extern double fEDA_ScaleMin;
extern double fEDA_Scale100;
extern double fEDA_Font_Scale;

extern int  nEDA_LargeCrossX,nEDA_LargeCrossY;

extern CString csESDFileHead_V1_0;
extern CString csESDFileHead_V1_1;
extern CString csESDFileHead_V1_2;

extern int  nEDA_RedrawFlag;
extern BOOL bEDA_Backup;
extern int  nEDA_Backup_Time;

extern COLORREF nEDA_BasicColor[240];

extern BOOL bEDA_MouseLButtonDown;
extern BOOL bEDA_MouseLButtonMoveEnable;
extern int nEDA_Pickup_X, nEDA_Pickup_Y;
extern BOOL bEDA_MouseRButtonPressedZooming;
extern BOOL bEDA_MouseMoveElement;

extern int nEDA_Selection_Offset;
extern int nEDA_PcbSelection_Offset;

extern char* szEDA_Multi_Selection[50];
extern int nEDA_Multi_Selection;
extern char* szEDA_PCB_Multi_Selection[11];

extern CString csEDA_Font_FontName;
extern CString csEDA_Font_FontSize;

extern BOOL bEDA_Font_Bold;
extern BOOL bEDA_Font_Italic;
extern BOOL bEDA_Font_Underline;

extern BOOL bEDA_Sheet_AutoOn;
extern CString csEDA_LastFile;
extern int nEDA_Lastfile_Flag;

extern struct Struct_Sch_Sheet sEDA_Standard_Sheet[18];

extern int nEDA_SCH_VisibleGridMode;
extern COLORREF nEDA_SCH_VisibleGridColor;

extern BOOL bEDA_SCH_VisibleGridShow;
extern double fEDA_SCH_VisibleGrid;
extern BOOL bEDA_SCH_SnapGridEnable;
extern double fEDA_SCH_SnapGrid_I[3];
extern double fEDA_SCH_SnapGrid_M[3];
extern BOOL bEDA_SCH_ElecGridEnable;
extern double fEDA_SCH_ElecGrid;
extern BOOL bEDA_NeedElectricalGrid;

extern COLORREF nEDA_SelectionColor;
extern int nEDA_ClickClearSelection;

extern int nEDA_SCH_Default_Unit_Type;

extern double fEDA_SCH_SizeMaxx;
extern double fEDA_SCH_SizeMaxy;

extern double fEDA_Rotate_XY[8][4];

extern int  nEDA_Cursor_Mode;	
extern int  nEDA_PartID_Suffix; 

extern int nEDA_Line_Width[4];
extern int nEDA_Bus_Width[4];
extern int nEDA_Junc_Size[4];

extern int  nEDA_Print_Flag;
extern int  nEDA_Print_Mode;
extern double fEDA_Print_Scale;

extern double fEDA_Print_Margin_X;
extern double fEDA_Print_Margin_Y;
extern BOOL bEDA_Print_HCenter;
extern BOOL bEDA_Print_VCenter;
extern int  nEDA_Print_Color;	
extern int  nEDA_Print_Extent;	
extern int  nEDA_Print_SheetMode;

extern int nEDA_PCBPrint_Mode;
extern double fEDA_PCBPrint_Scale;
extern double fEDA_PCBPrint_Margin_X;
extern double fEDA_PCBPrint_Margin_Y;
extern BOOL bEDA_PCBPrint_HCenter;
extern BOOL bEDA_PCBPrint_VCenter;
extern int nEDA_PCBPrint_Color;
extern int nEDA_PCBPrint_Extent;
extern BOOL bEDA_PCBPrint_Holes;

extern int  nEDA_Print_Orientation;
extern double  fEDA_Print_Minx, fEDA_Print_Miny, fEDA_Print_Maxx, fEDA_Print_Maxy;
extern int  nEDA_Print_Xdpi;
extern int  nEDA_Print_Ydpi;
extern int  nEDA_Print_X_Pixel;
extern int  nEDA_Print_Y_Pixel;

extern int nEDA_SCH_PlaceWireMode; 

extern CFile cfileEDA_SchLibrary;
extern CString currentschlib;
extern char* sch_library_name[SCH_SYSTEM_LIB_NUMBER];
extern int sch_library_offset[SCH_SYSTEM_LIB_NUMBER];
extern char* sch_library_original_name[SCH_SYSTEM_LIB_NUMBER];

extern CStringArray arrayEDA_SchLibraryTreeStructure;
extern int nEDA_SchLibrarySelectedItem;
extern CArray <Struct_CompList, Struct_CompList&>  arrayEDA_SchLibraryCompList;
extern CArray <Struct_CompList, Struct_CompList&>  arrayEDA_SchLibraryCompName[SCH_SYSTEM_LIB_NUMBER];
extern int nEDA_SchCompSelectedItem;
extern CString csEDA_SchSearchCompName;
extern BOOL bEDA_SchMatchMore;
extern CStringArray arrayEDA_SchUserLibraryName;
extern CArray <Struct_CompList, Struct_CompList&>  arrayEDA_SchCompHistory;
extern CString csEDA_SchCompHistory;
extern int nEDA_SchCompMaxSearchNumber;

extern CFile cfileEDA_PcbLibrary;
extern CString currentpcblib;
extern char* pcb_library_name[PCB_SYSTEM_LIB_NUMBER];
extern int pcb_library_offset[PCB_SYSTEM_LIB_NUMBER];

extern CStringArray arrayEDA_PcbLibraryTreeStructure;
extern int nEDA_PcbLibrarySelectedItem;
extern CArray <Struct_CompList, Struct_CompList&>  arrayEDA_PcbLibraryCompList;
extern CArray <Struct_CompList, Struct_CompList&>  arrayEDA_PcbLibraryCompName[PCB_SYSTEM_LIB_NUMBER];
extern int nEDA_PcbCompSelectedItem;
extern CString csEDA_PcbSearchCompName;
extern BOOL bEDA_PcbMatchMore;
extern CStringArray arrayEDA_PcbUserLibraryName;
extern CArray <Struct_CompList, Struct_CompList&>  arrayEDA_PcbCompHistory;
extern CString csEDA_PcbCompHistory;
extern int nEDA_PcbCompMaxSearchNumber;

extern int nEDA_FindScope;
extern int nEDA_FindObject;
extern CString csEDA_FindText;
extern CString csEDA_ReplaceText;
extern BOOL bEDA_FindCase;
extern BOOL bEDA_FindWord;
extern BOOL bEDA_FindJump;
extern BOOL bEDA_FindPrompt;

extern CTypedPtrList <CObList, CObject*> listEDA_SchClipBoard;
extern CArray <LOGFONT, LOGFONT&>  arrayEDA_ClipBoardFont;
extern double fEDA_ClipBoard_X1, fEDA_ClipBoard_Y1, fEDA_ClipBoard_X2, fEDA_ClipBoard_Y2;

extern BOOL bEDA_AutoJunction;
extern BOOL bEDA_AutoOptimizeWire;

extern int nEDA_SheetStandardStyle;

extern double fEDA_PinNameOffset;
extern double fEDA_PinNumberOffset;

extern COb_schcomp			cEDA_schcomp;
extern COb_schpin			cEDA_schpin;
extern COb_schieee			cEDA_schieee;

extern COb_schline			cEDA_schwire;
extern COb_schline			cEDA_schbus;
extern COb_schline			cEDA_schbusentry;
extern COb_schpolyline		cEDA_schpolyline;

extern COb_schtext			cEDA_schtext;
extern LOGFONT				cEDA_textfont;
extern COb_schtext			cEDA_schnetlabel;	
extern LOGFONT				cEDA_netlabelfont;

extern COb_schbezier		cEDA_schbezier;

extern COb_schpolygon		cEDA_schpolygon;
extern COb_schellipsefill	cEDA_schellipsefill;
extern COb_schpie			cEDA_schpie;
extern COb_schroundrect		cEDA_schroundrect;
extern COb_schellipse		cEDA_schellipse;
extern COb_scharc			cEDA_scharc;
extern COb_schrect			cEDA_schrect;
extern COb_schpower			cEDA_schpower;
extern COb_schpower			cEDA_schground;
extern COb_schpower			cEDA_schoffsheet;
extern COb_schport			cEDA_schport;

extern COb_schsheetentry	cEDA_schsheetentry;
extern COb_schsheetsymbol	cEDA_schsheetsymbol;
extern CString csEDA_schsheetsymboldesignator;
extern CString csEDA_schsheetsymbolfilename;

extern COb_schtextframe		cEDA_schtextframe;
extern COb_schjunc			cEDA_schjunc;
extern COb_schjunc			cEDA_schautojunc;
extern COb_schgraphic		cEDA_schgraphic;
extern COb_schsheet			cEDA_schsheet;

extern CString csEDA_PowerGround;
extern CString csEDA_SignalGround;
extern CString csEDA_EarthGround;

extern int nEDA_SchRepeatNumber;
extern int nEDA_SchRepeatNumberDelta;
extern double fEDA_SchRepeatDeltaX;
extern double fEDA_SchRepeatDeltaY;

extern CString csEDA_PCB_LayerName[PCB_LAYER_NUM];
extern BOOL bEDA_ConvertEnglishLayerName;
extern BOOL bEDA_ConvertChineseLayerName;
extern CString csEDA_PCB_EnglishLayerName[PCB_LAYER_NUM];
extern CString csEDA_PCB_EnglishLayerSetName[PCB_LAYER_NUM];
extern CString csEDA_PCB_LayerLabel[PCB_LAYER_NUM];
extern COLORREF  nEDA_PCB_LayerColor[PCB_LAYER_NUM];
extern COLORREF nEDA_PCB_HighlightColor;
extern COLORREF nEDA_PCB_BoardLineColor;
extern COLORREF nEDA_PCB_BoardAreaColor;
extern COLORREF nEDA_PCB_SheetLineColor;
extern COLORREF nEDA_PCB_SheetAreaColor;
extern COLORREF nEDA_PCB_WorkspaceColor;
extern COLORREF  nEDA_PCB_DefaultColor[PCB_LAYER_NUM];
extern COLORREF nEDA_PCB_HighlightDefaultColor;
extern COLORREF nEDA_PCB_BoardLineDefaultColor;
extern COLORREF nEDA_PCB_BoardAreaDefaultColor;
extern COLORREF nEDA_PCB_SheetLineDefaultColor;
extern COLORREF nEDA_PCB_SheetAreaDefaultColor;
extern COLORREF nEDA_PCB_WorkspaceDefaultColor;

extern double fEDA_PCB_SizeMaxx;
extern double fEDA_PCB_SizeMaxy;

extern struct Struct_Pcb_Vector_Font nEDA_DeafultFont[96];
extern struct Struct_Pcb_Vector_Font nEDA_SansSerifFont[96];
extern struct Struct_Pcb_Vector_Font nEDA_SerifFont[96];
extern CString csPCB_StrokeFontName[3];
extern CString csPCB_PadShape[4];
extern CString csPCB_PadElecTpye[3];
extern CString csPCB_PlaneStyle[3];

extern int nEDA_PCB_Undo_Number;

extern COb_pcbarc			cEDA_pcbarc;
extern COb_pcbtrack			cEDA_pcbtrack;
extern COb_pcbcomp			cEDA_pcbcomp;
extern COb_pcbtext			cEDA_pcbtext;
extern COb_pcbfill			cEDA_pcbfill;
extern COb_pcbpad			cEDA_pcbpad;
extern COb_pcbvia			cEDA_pcbvia;
extern COb_pcbregion		cEDA_pcbregion;
extern COb_pcbpolygon		cEDA_pcbpolygon;
extern COb_pcbcoordinate	cEDA_pcbcoordinate;
extern COb_pcbdimension		cEDA_pcbdimension;
extern COb_pcbtext			cEDA_pcbcomp_designator;
extern COb_pcbtext			cEDA_pcbcomp_comment;

extern int nEDA_PCB_PlaceTrackMode; 
extern double fEDA_AutoArcMaxRadius;

extern int nEDA_PcbRepeatNumber;
extern int nEDA_PcbRepeatNumberDelta;
extern double fEDA_PcbRepeatDeltaX;
extern double fEDA_PcbRepeatDeltaY;

extern double fEDA_LoadPackage_X;
extern double fEDA_LoadPackage_Y;
extern BOOL bEDA_LoadPackageByName;

extern BOOL bEDA_PcbPolygonHighlight;

extern CTypedPtrArray <CObArray, COb_pcbarc*> arrayEDA_PcbClipBoardArc;
extern CTypedPtrArray <CObArray, COb_pcbtrack*> arrayEDA_PcbClipBoardTrack;
extern CTypedPtrArray <CObArray, COb_pcbtext*> arrayEDA_PcbClipBoardText;
extern CTypedPtrArray <CObArray, COb_pcbfill*> arrayEDA_PcbClipBoardFill;
extern CTypedPtrArray <CObArray, COb_pcbregion*> arrayEDA_PcbClipBoardRegion;	
extern CTypedPtrArray <CObArray, COb_pcbpolygon*> arrayEDA_PcbClipBoardPolygon;
extern CTypedPtrArray <CObArray, COb_pcbpad*> arrayEDA_PcbClipBoardPad;
extern CTypedPtrArray <CObArray, COb_pcbvia*> arrayEDA_PcbClipBoardVia;
extern CTypedPtrArray <CObArray, COb_pcbcomp*> arrayEDA_PcbClipBoardComp;
extern CTypedPtrArray <CObArray, COb_pcbcoordinate*> arrayEDA_PcbClipBoardCoordinate;
extern CTypedPtrArray <CObArray, COb_pcbdimension*> arrayEDA_PcbClipBoardDimension;
extern double fEDA_PcbClipBoard_X1, fEDA_PcbClipBoard_Y1, fEDA_PcbClipBoard_X2, fEDA_PcbClipBoard_Y2; 

extern BOOL bEDA_PcbSelectionFirstEnable;

extern BOOL bEDA_PcbConnectionAutoRefresh;
extern int  nEDA_PcbConnectionDrawWidth;

extern BOOL bEDA_PcbOnLineDRCEnable;

extern double fEDA_PcbRouteDelta;

extern char szEDA_Help_Ready[];
extern char szEDA_Info_Ready[];
extern char szEDA_HighlightColor[];
extern char szEDA_BoardLineColor[];
extern char szEDA_BoardAreaColor[];
extern char szEDA_SheetLineColor[];
extern char szEDA_SheetAreaColor[];
extern char szEDA_WordspaceColor[];
extern char szEDA_ProgramAbort[];

extern CStringArray arrayEDA_SystemFont;

extern CString SCH_Library_Head;
extern CString PCB_Library_Head;

#endif // !defined(AFX_SYSTEMEXT_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_)
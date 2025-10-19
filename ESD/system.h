#if !defined(AFX_SYSTEM_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_)
#define AFX_SYSTEM_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_

#include "sysparam.h"
#include "struct.h"


CString	csESDViewClassName;
CString	csEDA_System_Path;

CFile cfileEDA_System;

int nEDA_System_State = SCH;		
int nEDA_Set_Mouse_Pos = FALSE;		

double fEDA_pi = 3.141592653589793238462f;
double fEDA_Prec = 0.001;
double fEDA_AnglePrec = 0.1;

int nEDA_Sheet_State = 1;		
int nEDA_Sheet_Tab_Width = 16;  

double fEDA_ScaleMax = 200.0f;
double fEDA_ScaleMin = 0.32f;
double fEDA_Scale100 = 10.416666666667f;
double fEDA_Font_Scale = 10.05f;  

int  nEDA_LargeCrossX=-1,nEDA_LargeCrossY=-1;

CString csESDFileHead_V1_0  = "Protel V4.0 Design File";
CString csESDFileHead_V1_1 = "ESD_V41 SCH_PCB File V1_1";
CString csESDFileHead_V1_2 = "ESD_V41 SCH_PCB File V1_2";

int  nEDA_RedrawFlag = OFF;	
BOOL bEDA_Backup = TRUE;   
int  nEDA_Backup_Time = 30;  
COLORREF nEDA_BasicColor[240] = {	127485,		6759642,	13278208,	0,			33789,		2232046,	8323857,	61183,		1462238,	6946977,	9306309,	8978557,
									8060976,	8847372,	11367680,	7250688,	134408,		12967653,	13097436,	10792378,	9279142,	7501954,	9411503,	9279657,
									7633541,	7040632,	5724256,	4078660,	11913679,	12240586,	11711934,	10921648,	9604753,	10001322,	9473425,	8287349,
									6838875,	6049101,	4404530,	8517375,	37796,		6221823,	32398,		55807,		8056319,	39107,		7924479,	28036,
									1754623,	609882,		40191,		6411775,	32453,		7654143,	26005,		5020405,	406861,		30967,		9161727,	25282,
									8103423,	15229,		6129151,	272712,		3162822,	9406719,	1382776,	9336556,	1052745,	5514966,	12436479,	3408013,
									10649065,	3410771,	8467419,	3148100,	5636220,	11296232,	4587607,	10368208,	14197247,	7405696,	12948958,	7209062,
									12465579,	13875686,	5640005,	16726460,	5767224,	14360714,	5373992,	9048648,	16751055,	6225952,	12218252,	4525340,
									9306163,	14469570,	5180928,	14840927,	4460032,	11796480,	13217931,	8732672,	13741180,	6109440,	13085014,	13817758,
									8479232,	11973746,	5981952,	12703346,	5589760,	7963671,	12839342,	7108608,	10932348,	5331724,	9021481,	2502144,
									6063383,	10342018,	4873252,	8508508,	2308113,	5485115,	10942152,	4229937,	7332791,	1600582,	5433280,	676421,
									57008,		6747895,	44716,		6486772,	39327,		2159345,	231054,		60649,		11520467,	5527646,	9936042,	659217,
									6580069,	12305352,	6314845,	11578546,	4730408,	8354714,	2235169,	7170665,	1454394,	8822189,	26999,		10208458,
									3116968,	10022647,	7708866,	10821,		9218505,	3039647,	10080240,	6712978,	10401745,	7307173,	2109519,	9284591,
									3286880,	11646447,	3685490,	12176880,	10130630,	3219266,	9470930,	7286632,	12756459,	9524876,	13486297,	10121894,
									4070189,	10456492,	7807054,	13476562,	9116738,	14795724,	10059915,	4989720,	12360588,	6632219,	12955568,	6899487,
									13488067,	10723209,	3218176,	10526868,	15896,		10138021,	7832426,	12045505,	8757620,	2172695,	10662316,	3886870,
									11721142,	30550,		10151133,	7709086,	1131839,	9555144,	37264,		7597287,	6922405,	12632256,	15269887,	16773375,
									16777200,	15461320,	11599871,	8454143,	8421504,	128,		32768,		8388608,	32896,		8421376,	8388736,	255,
									65280,		16711680,	65535,		16711935,	16776960,	16777215,	48896,		191,		12517376,	49087,		12517567,	12566272 };


BOOL bEDA_MouseLButtonDown = FALSE;		
BOOL bEDA_MouseLButtonMoveEnable = FALSE;
int nEDA_Pickup_X, nEDA_Pickup_Y;	
BOOL bEDA_MouseRButtonPressedZooming = FALSE;
BOOL bEDA_MouseMoveElement = FALSE;		

int nEDA_Selection_Offset = 15;  
int nEDA_PcbSelection_Offset = 5;  

char* szEDA_Multi_Selection[50] = {	"",				"元件",			"引脚",			"IEEE符号",		"文字",			"样条曲线",		"连线",			"多边形",		"椭圆充填",			"饼形图",		\
									"圆方形",		"椭圆圆弧",		"圆弧",			"导线",			"矩形",			"分图符号",		"分图入口",		"电源接地符号",	"接口",				"",				\
									"",				"",				"",				"",				"",				"网络标号",		"总线",			"导线",			"文字框",			"结点",			\
									"图形",			"图框",			"分图符号名称", "分图符号注释", "元件名称",		"",				"",				"",				"",					"",				\
									"",				"元件注释",		"",				"",				"元件封装",		"",				"",				"",				"",					""		};
int nEDA_Multi_Selection = -1;
char* szEDA_PCB_Multi_Selection[11] = {	"",			"走线带",		"圆弧",			"焊盘",			"过孔",			"文字",			"填充区",		"区域",			"元件",				"坐标标注",	   "尺寸标注"};

CString csEDA_Font_FontName = "Times New Roman";
CString csEDA_Font_FontSize = "10";
BOOL bEDA_Font_Bold = FALSE;
BOOL bEDA_Font_Italic = FALSE;
BOOL bEDA_Font_Underline = FALSE;

BOOL bEDA_Sheet_AutoOn = TRUE;

CString csEDA_LastFile;
int nEDA_Lastfile_Flag = 0;


struct Struct_Sch_Sheet sEDA_Standard_Sheet[18] = {	11500,    7600,       4,        4,       200,   
													15500,   11100,       8,        4,       200,   
													22300,   15700,       8,        4,       300,   
													31500,   22300,      12,        4,       300,   
													44600,   31500,      16,        4,       400,   
													9500,    7500,       4,        4,       200,   
													15000,    9500,       6,        4,       200,   
													20000,   15000,       6,        4,       300,   
													32000,   20000,       8,        4,       300,   
													42000,   32000,      16,        4,       400,   
													11000,    8500,       4,        4,       200,   
													14000,    8500,       4,        4,       200,   
													17000,   11000,       6,        4,       200,   
													 9900,    7900,       4,        4,       200,   
													15400,    9900,       6,        4,       200,   
													20600,   15600,       6,        4,       300,   
													32600,   20600,       8,        4,       300,   
													42800,   32800,      16,        4,       400 }; 
												

int nEDA_SCH_VisibleGridMode = GRID_LINE;
COLORREF nEDA_SCH_VisibleGridColor = RGB(236,236,236);

BOOL bEDA_SCH_VisibleGridShow = TRUE;
double fEDA_SCH_VisibleGrid = 100.0f;

BOOL bEDA_SCH_SnapGridEnable = TRUE;
double fEDA_SCH_SnapGrid_I[3] = {10.0f, 50.0f, 100.0f};	//英制
double fEDA_SCH_SnapGrid_M[3] = {39.370079f, 98.425198f, 196.850396f}; //公制

BOOL bEDA_SCH_ElecGridEnable = TRUE;
double fEDA_SCH_ElecGrid = 40.0f;
BOOL bEDA_NeedElectricalGrid = FALSE;

COLORREF nEDA_SelectionColor = RGB(0,255,0);
int nEDA_ClickClearSelection  = TRUE;

int nEDA_SCH_Default_Unit_Type = UNIT_MIL;

double fEDA_SCH_SizeMaxx = 67000;
double fEDA_SCH_SizeMaxy = 67000;

double fEDA_Rotate_XY[8][4] = {	1.0f,	0,  0,	1.0f,		
								0,  -1.0f,	1.0f, 0,		
								-1.0f, 0,  0,	-1.0f,		
								0,	1.0f, -1.0f,	0,		
								-1.0f, 0,  0,	1.0f,		
								0,	1.0f,	1.0f, 0,		
								1.0f,	0,  0,	-1.0f,		
								0,  -1.0f,	-1.0f,  0	};	
							
int  nEDA_Cursor_Mode = 0;	
int  nEDA_PartID_Suffix = 0; 

int nEDA_Line_Width[4] = {1, 10, 30, 50};
int nEDA_Bus_Width[4] = {20, 30, 50, 70};
int nEDA_Junc_Size[4] = {20, 30, 50, 100};

int  nEDA_Print_Flag = 0;	

int  nEDA_Print_Mode = 0;	
double fEDA_Print_Scale = 0.98;	
double fEDA_Print_Margin_X = 0;	
double fEDA_Print_Margin_Y = 0;	
BOOL bEDA_Print_HCenter = TRUE; 
BOOL bEDA_Print_VCenter = TRUE; 
int  nEDA_Print_Color = 2;		
int  nEDA_Print_Extent = 0;		
int  nEDA_Print_SheetMode = 0;	

int nEDA_PCBPrint_Mode = 0;
double fEDA_PCBPrint_Scale = 0.98;
double fEDA_PCBPrint_Margin_X = 0;
double fEDA_PCBPrint_Margin_Y = 0;
BOOL bEDA_PCBPrint_HCenter = TRUE;
BOOL bEDA_PCBPrint_VCenter = TRUE;
int nEDA_PCBPrint_Color = 2;
int nEDA_PCBPrint_Extent = 1;
BOOL bEDA_PCBPrint_Holes = TRUE;

int  nEDA_Print_Orientation = 0; 
double  fEDA_Print_Minx=0, fEDA_Print_Miny=0, fEDA_Print_Maxx=67000, fEDA_Print_Maxy=67000;
int  nEDA_Print_Xdpi = 600;
int  nEDA_Print_Ydpi = 600;
int  nEDA_Print_X_Pixel;
int  nEDA_Print_Y_Pixel;

int nEDA_SCH_PlaceWireMode = SCH_PLACE_90_START; 

CString currentschlib;

#include "schlibrary.h"
CString SCH_Library_Head = "ESD SCH Library V4 2022";

CFile cfileEDA_SchLibrary;
CArray <Struct_CompList, Struct_CompList&>  arrayEDA_SchLibraryCompName[SCH_SYSTEM_LIB_NUMBER];

CStringArray arrayEDA_SchUserLibraryName;

CStringArray arrayEDA_SchLibraryTreeStructure;
int nEDA_SchLibrarySelectedItem = -1;

CArray <Struct_CompList, Struct_CompList&>  arrayEDA_SchLibraryCompList;
int nEDA_SchCompSelectedItem = -1;

CString csEDA_SchSearchCompName;
BOOL bEDA_SchMatchMore = TRUE;

CArray <Struct_CompList, Struct_CompList&>  arrayEDA_SchCompHistory;
CString csEDA_SchCompHistory;
int nEDA_SchCompMaxSearchNumber = 2000;

CString currentpcblib;

#include "pcblibrary.h"
CString PCB_Library_Head = "ESD PCB Library V4 2022";

CFile cfileEDA_PcbLibrary;
CArray <Struct_CompList, Struct_CompList&>  arrayEDA_PcbLibraryCompName[PCB_SYSTEM_LIB_NUMBER];

CStringArray arrayEDA_PcbUserLibraryName;

CStringArray arrayEDA_PcbLibraryTreeStructure;
int nEDA_PcbLibrarySelectedItem = -1;

CArray <Struct_CompList, Struct_CompList&>  arrayEDA_PcbLibraryCompList;
int nEDA_PcbCompSelectedItem = -1;

CString csEDA_PcbSearchCompName;
BOOL bEDA_PcbMatchMore = TRUE;

CArray <Struct_CompList, Struct_CompList&>  arrayEDA_PcbCompHistory;
CString csEDA_PcbCompHistory;
int nEDA_PcbCompMaxSearchNumber = 2000;

int nEDA_FindScope = 1;
int nEDA_FindObject = 0;
CString csEDA_FindText;
CString csEDA_ReplaceText;
BOOL bEDA_FindCase = FALSE;
BOOL bEDA_FindWord = FALSE;
BOOL bEDA_FindJump = TRUE;
BOOL bEDA_FindPrompt = TRUE;

CTypedPtrList <CObList, CObject*> listEDA_SchClipBoard;
CArray <LOGFONT, LOGFONT&>  arrayEDA_ClipBoardFont;
double fEDA_ClipBoard_X1, fEDA_ClipBoard_Y1, fEDA_ClipBoard_X2, fEDA_ClipBoard_Y2; 

BOOL bEDA_AutoJunction = TRUE;
BOOL bEDA_AutoOptimizeWire = TRUE;

int nEDA_SheetStandardStyle = 0;

double fEDA_PinNameOffset = 50;
double fEDA_PinNumberOffset = 80;

COb_schcomp			cEDA_schcomp;
COb_schpin			cEDA_schpin;
COb_schieee			cEDA_schieee;
COb_schline			cEDA_schwire;
COb_schline			cEDA_schbus;
COb_schline			cEDA_schbusentry;
COb_schpolyline		cEDA_schpolyline;
COb_schtext			cEDA_schtext;
LOGFONT				cEDA_textfont;
COb_schtext			cEDA_schnetlabel;	
LOGFONT				cEDA_netlabelfont;
COb_schbezier		cEDA_schbezier;
COb_schpolygon		cEDA_schpolygon;
COb_schellipsefill	cEDA_schellipsefill;
COb_schpie			cEDA_schpie;
COb_schroundrect	cEDA_schroundrect;
COb_schellipse		cEDA_schellipse;
COb_scharc			cEDA_scharc;
COb_schrect			cEDA_schrect;
COb_schpower		cEDA_schpower;
COb_schpower		cEDA_schground;
COb_schpower		cEDA_schoffsheet;
COb_schport			cEDA_schport;
COb_schsheetentry	cEDA_schsheetentry;
COb_schsheetsymbol	cEDA_schsheetsymbol;
CString csEDA_schsheetsymboldesignator;
CString csEDA_schsheetsymbolfilename;
COb_schtextframe	cEDA_schtextframe;
COb_schjunc			cEDA_schjunc;
COb_schjunc			cEDA_schautojunc;
COb_schgraphic		cEDA_schgraphic;
COb_schsheet		cEDA_schsheet;

CString csEDA_PowerGround = "GND";
CString csEDA_SignalGround = "SGND";
CString csEDA_EarthGround = "EARTH";

int nEDA_SchRepeatNumber = 1;
int nEDA_SchRepeatNumberDelta = 1;
double fEDA_SchRepeatDeltaX = 100;
double fEDA_SchRepeatDeltaY = 0;

CString csEDA_PCB_LayerName[PCB_LAYER_NUM] = {	"元件面", \
											"中间1层","中间2层","中间3层","中间4层","中间5层","中间6层","中间7层","中间8层","中间9层","中间10层",
											"中间11层","中间12层","中间13层","中间14层","中间15层","中间16层","中间17层","中间18层","中间19层","中间20层",
											"中间21层","中间22层","中间23层","中间24层","中间25层","中间26层","中间27层","中间28层","中间29层","中间30层",
											"焊接面",\
											"元件面丝印","焊接面丝印",
											"元件面助焊","焊接面助焊","元件面阻焊","焊接面阻焊",
											"内平面1","内平面2","内平面3","内平面4","内平面5","内平面6","内平面7","内平面8",
											"内平面9","内平面10","内平面11","内平面12","内平面13","内平面14","内平面15","内平面16",
											"孔位置图","禁止布线层",
											"机械层1","机械层2","机械层3","机械层4","机械层5","机械层6","机械层7","机械层8",
											"机械层9","机械层10","机械层11","机械层12","机械层13","机械层14","机械层15","机械层16",
											"打孔图","通孔层","连接层","背景层","标记层","选择层","显示网格1","显示网格2","焊盘通孔","过孔通孔"};
BOOL bEDA_ConvertEnglishLayerName = TRUE;
BOOL bEDA_ConvertChineseLayerName = TRUE;
CString csEDA_PCB_EnglishLayerName[PCB_LAYER_NUM] = { "Top Layer",\
											"Mid-Layer 1","Mid-Layer 2","Mid-Layer 3","Mid-Layer 4","Mid-Layer 5","Mid-Layer 6","Mid-Layer 7","Mid-Layer 8","Mid-Layer 9","Mid-Layer 10",\
											"Mid-Layer 11","Mid-Layer 12","Mid-Layer 13","Mid-Layer 14","Mid-Layer 15","Mid-Layer 16","Mid-Layer 17","Mid-Layer 18","Mid-Layer 19","Mid-Layer 20",\
											"Mid-Layer 21","Mid-Layer 22","Mid-Layer 23","Mid-Layer 24","Mid-Layer 25","Mid-Layer 26","Mid-Layer 27","Mid-Layer 28","Mid-Layer 29","Mid-Layer 30",\
											"Bottom Layer",\
											"Top Overlay","Bottom Overlay",\
											"Top Paste","Bottom Paste","Top Solder","Bottom Solder",\
											"Internal Plane 1","Internal Plane 2","Internal Plane 3","Internal Plane 4","Internal Plane 5","Internal Plane 6","Internal Plane 7","Internal Plane 8",\
											"Internal Plane 9","Internal Plane 10","Internal Plane 11","Internal Plane 12","Internal Plane 13","Internal Plane 14","Internal Plane 15","Internal Plane 16",\
											"Drill Guide","Keep-Out Layer",\
											"Mechanical 1","Mechanical 2","Mechanical 3","Mechanical 4","Mechanical 5","Mechanical 6","Mechanical 7","Mechanical 8",\
											"Mechanical 9","Mechanical 10","Mechanical 11","Mechanical 12","Mechanical 13","Mechanical 14","Mechanical 15","Mechanical 16",\
											"Drill Drawing","Multi-Layer","Connections","Background","DRC Error Markers","Selections","Visible Grid 1","Visible Grid 2","Pad Holes","Via Holes" };

CString csEDA_PCB_EnglishLayerSetName[PCB_LAYER_NUM] = { "TopLayer",\
											"MidLayer1","MidLayer2","MidLayer3","MidLayer4","MidLayer5","MidLayer6","MidLayer7","MidLayer8","MidLayer9","MidLayer10",\
											"MidLayer11","MidLayer12","MidLayer13","MidLayer14","MidLayer15","MidLayer16","MidLayer17","MidLayer18","MidLayer19","MidLayer20",\
											"MidLayer21","MidLayer22","MidLayer23","MidLayer24","MidLayer25","MidLayer26","MidLayer27","MidLayer28","MidLayer29","MidLayer30",\
											"BottomLayer",\
											"TopOverlay","BottomOverlay",\
											"TopPaste","BottomPaste","TopSolder","BottomSolder",\
											"InternalPlane1","InternalPlane2","InternalPlane3","InternalPlane4","InternalPlane5","InternalPlane6","InternalPlane7","InternalPlane8",\
											"InternalPlane9","InternalPlane10","InternalPlane11","InternalPlane12","InternalPlane13","InternalPlane14","InternalPlane15","InternalPlane16",\
											"DrillGuide","KeepOutLayer",\
											"Mechanical1","Mechanical2","Mechanical3","Mechanical4","Mechanical5","Mechanical6","Mechanical7","Mechanical8",\
											"Mechanical9","Mechanical10","Mechanical11","Mechanical12","Mechanical13","Mechanical14","Mechanical15","Mechanical16",\
											"DrillDrawing","MultiLayer","Connections","Background","DRC Error Markers","Selections","Visible Grid 1","Visible Grid 2","Pad Holes","Via Holes" };

CString csEDA_PCB_LayerLabel[PCB_LAYER_NUM] = { "TOP",
											"MID1","MID2","MID3","MID4","MID5","MID6","MID7","MID8","MID9","MID10",
											"MID11","MID12","MID13","MID14","MID15","MID16","MID17","MID18","MID19","MID20",
											"MID21","MID22","MID23","MID24","MID25","MID26","MID27","MID28","MID29","MID30",
											"BOTTOM",
											"TOPOVERLAY","BOTTOMOVERLAY",
											"TOPPASTE","BOTTOMPASTE","TOPSOLDER","BOTTOMSOLDER",
											"PLANE1","PLANE2","PLANE3","PLANE4","PLANE5","PLANE6","PLANE7","PLANE8",
											"PLANE0","PLANE10","PLANE11","PLANE12","PLANE13","PLANE14","PLANE15","PLANE16",
											"DRILLGUIDE","KEEPOUT",
											"MECHANICAL1","MECHANICAL2","MECHANICAL3","MECHANICAL4","MECHANICAL5","MECHANICAL6","MECHANICAL7","MECHANICAL8",
											"MECHANICAL9","MECHANICAL10","MECHANICAL11","MECHANICAL12","MECHANICAL13","MECHANICAL14","MECHANICAL15","MECHANICAL16",
											"DRILLDRAWING","MULTILAYER","","","","","","","",""};

COLORREF  nEDA_PCB_LayerColor[PCB_LAYER_NUM] = {	RGB(255,0,0),  
													RGB(188,142,0),		RGB(112,219,250),	RGB(0,204,102),		RGB(153,102,255),	RGB(0,255,255),   
													RGB(128,0,128),		RGB(255,0,255),		RGB(128,128,0),		RGB(255,255,0),		RGB(128,128,128), 
													RGB(255,255,255),	RGB(128,0,128),		RGB(0,128,128),		RGB(192,192,192),	RGB(128,0,0),	  
													RGB(0,128,0),		RGB(0,255,0),		RGB(0,0,128),		RGB(0,255,255),		RGB(128,0,128),   
													RGB(255,0,255),		RGB(128,128,0),		RGB(255,255,0),		RGB(128,128,128),	RGB(255,255,255), 
													RGB(128,0,128),		RGB(0,128,128),		RGB(192,192,192),	RGB(128,0,0),		RGB(0,128,0),     
													RGB(0,0,255),	
													RGB(255,255,0),		RGB(128,128,0),	
													RGB(128,128,128),	RGB(128,0,0),		RGB(128,0,128),		RGB(255,0,255), 
													RGB(0,128,0),		RGB(128,0,0),		RGB(128,0,128),		RGB(0,128,128),		RGB(0,128,0),		RGB(128,0,0),		RGB(128,0,128),		RGB(0,128,128), 
													RGB(0,128,0),		RGB(128,0,0),		RGB(128,0,128),		RGB(0,128,128),		RGB(0,128,0),		RGB(128,0,0),		RGB(128,0,12),		RGB(0,128,128), 
													RGB(128,0,0),		RGB(255,0,255), 
													RGB(255,0,255),		RGB(128,0,128),		RGB(0,128,0),		RGB(128,128,0),		RGB(255,0,255),		RGB(128,0,128),		RGB(0,128,0),		RGB(128,128,0), 
													RGB(255,0,255),		RGB(128,0,128),		RGB(0,128,0),		RGB(128,128,0),		RGB(255,0,255),		RGB(128,0,128),		RGB(0,128,0),		RGB(0,0,0),     
													RGB(255,0,42),		RGB(192,192,192), 
													RGB(158,161,117),	RGB(0,0,0),			RGB(0,255,0),		RGB(255,255,255),	RGB(50,50,50),		RGB(80, 80, 80),	RGB(0,145,144),		RGB(129,98,0) };

COLORREF nEDA_PCB_HighlightColor = RGB(255,255,255);
COLORREF nEDA_PCB_BoardLineColor = RGB(0,0,0);
COLORREF nEDA_PCB_BoardAreaColor = RGB(0,0,0);
COLORREF nEDA_PCB_SheetLineColor = RGB(0,0,0);
COLORREF nEDA_PCB_SheetAreaColor = RGB(255,255,255);
COLORREF nEDA_PCB_WorkspaceColor = RGB(106,106,106);

COLORREF  nEDA_PCB_DefaultColor[PCB_LAYER_NUM] = {	RGB(255,0,0),  
													RGB(188,142,0),		RGB(112,219,250),	RGB(0,204,102),		RGB(153,102,255),	RGB(0,255,255),   
													RGB(128,0,128),		RGB(255,0,255),		RGB(128,128,0),		RGB(255,255,0),		RGB(128,128,128), 
													RGB(255,255,255),	RGB(128,0,128),		RGB(0,128,128),		RGB(192,192,192),	RGB(128,0,0),	  
													RGB(0,128,0),		RGB(0,255,0),		RGB(0,0,128),		RGB(0,255,255),		RGB(128,0,128),   
													RGB(255,0,255),		RGB(128,128,0),		RGB(255,255,0),		RGB(128,128,128),	RGB(255,255,255), 
													RGB(128,0,128),		RGB(0,128,128),		RGB(192,192,192),	RGB(128,0,0),		RGB(0,128,0),     
													RGB(0,0,255),
													RGB(255,255,0),		RGB(128,128,0),	
													RGB(128,128,128),	RGB(128,0,0),		RGB(128,0,128),		RGB(255,0,255), 
													RGB(0,128,0),		RGB(128,0,0),		RGB(128,0,128),		RGB(0,128,128),		RGB(0,128,0),		RGB(128,0,0),		RGB(128,0,128),		RGB(0,128,128), 
													RGB(0,128,0),		RGB(128,0,0),		RGB(128,0,128),		RGB(0,128,128),		RGB(0,128,0),		RGB(128,0,0),		RGB(128,0,12),		RGB(0,128,128), 
													RGB(128,0,0),		RGB(255,0,255), 
													RGB(255,0,255),		RGB(128,0,128),		RGB(0,128,0),		RGB(128,128,0),		RGB(255,0,255),		RGB(128,0,128),		RGB(0,128,0),		RGB(128,128,0), 
													RGB(255,0,255),		RGB(128,0,128),		RGB(0,128,0),		RGB(128,128,0),		RGB(255,0,255),		RGB(128,0,128),		RGB(0,128,0),		RGB(0,0,0),     
													RGB(255,0,42),		RGB(192,192,192), 
													RGB(158,161,117),	RGB(0,0,0),			RGB(0,255,0),		RGB(255,255,255),	RGB(50,50,50),		RGB(80, 80, 80),	RGB(0,145,144),		RGB(129,98,0) };  

COLORREF nEDA_PCB_HighlightDefaultColor = RGB(255,255,255);
COLORREF nEDA_PCB_BoardLineDefaultColor = RGB(0,0,0);
COLORREF nEDA_PCB_BoardAreaDefaultColor = RGB(0,0,0);
COLORREF nEDA_PCB_SheetLineDefaultColor = RGB(0,0,0);
COLORREF nEDA_PCB_SheetAreaDefaultColor = RGB(255,255,255);
COLORREF nEDA_PCB_WorkspaceDefaultColor = RGB(106,106,106);

double fEDA_PCB_SizeMaxx = 99999.999;
double fEDA_PCB_SizeMaxy = 99999.999;

#include "VectorFont.h"
CString csPCB_StrokeFontName[3] = {"DEFAULT", "SANSSERIF", "SERIF"};
CString csPCB_PadShape[4] = {"ROUND", "RECTANGLE", "OCTAGONAL", "ROUNDEDRECTANGLE"};
CString csPCB_PadElecTpye[3] = {"Load", "Terminator","Source"};
CString csPCB_PlaneStyle[3] = {"Relief", "Direct", "NoConnect"};

int nEDA_PCB_Undo_Number = 50;

COb_pcbarc			cEDA_pcbarc;
COb_pcbtrack		cEDA_pcbtrack;
COb_pcbcomp			cEDA_pcbcomp;
COb_pcbtext			cEDA_pcbtext;
COb_pcbfill			cEDA_pcbfill;
COb_pcbpad			cEDA_pcbpad;
COb_pcbvia			cEDA_pcbvia;
COb_pcbregion		cEDA_pcbregion;
COb_pcbpolygon		cEDA_pcbpolygon;
COb_pcbcoordinate	cEDA_pcbcoordinate;
COb_pcbdimension	cEDA_pcbdimension;
COb_pcbtext			cEDA_pcbcomp_designator;
COb_pcbtext			cEDA_pcbcomp_comment;

int nEDA_PCB_PlaceTrackMode = PCB_PLACE_90_START; 
double fEDA_AutoArcMaxRadius = 100;

int nEDA_PcbRepeatNumber = 1;
int nEDA_PcbRepeatNumberDelta = 1;
double fEDA_PcbRepeatDeltaX = 100;
double fEDA_PcbRepeatDeltaY = 0;

double fEDA_LoadPackage_X = 0;
double fEDA_LoadPackage_Y = 0;
BOOL bEDA_LoadPackageByName = FALSE;

BOOL bEDA_PcbPolygonHighlight = TRUE;

CTypedPtrArray <CObArray, COb_pcbarc*> arrayEDA_PcbClipBoardArc;
CTypedPtrArray <CObArray, COb_pcbtrack*> arrayEDA_PcbClipBoardTrack;
CTypedPtrArray <CObArray, COb_pcbtext*> arrayEDA_PcbClipBoardText;
CTypedPtrArray <CObArray, COb_pcbfill*> arrayEDA_PcbClipBoardFill;
CTypedPtrArray <CObArray, COb_pcbregion*> arrayEDA_PcbClipBoardRegion;	
CTypedPtrArray <CObArray, COb_pcbpolygon*> arrayEDA_PcbClipBoardPolygon;
CTypedPtrArray <CObArray, COb_pcbpad*> arrayEDA_PcbClipBoardPad;
CTypedPtrArray <CObArray, COb_pcbvia*> arrayEDA_PcbClipBoardVia;
CTypedPtrArray <CObArray, COb_pcbcomp*> arrayEDA_PcbClipBoardComp;
CTypedPtrArray <CObArray, COb_pcbcoordinate*> arrayEDA_PcbClipBoardCoordinate;
CTypedPtrArray <CObArray, COb_pcbdimension*> arrayEDA_PcbClipBoardDimension;
double fEDA_PcbClipBoard_X1, fEDA_PcbClipBoard_Y1, fEDA_PcbClipBoard_X2, fEDA_PcbClipBoard_Y2; 

BOOL bEDA_PcbSelectionFirstEnable = FALSE;

BOOL bEDA_PcbConnectionAutoRefresh = TRUE;
int  nEDA_PcbConnectionDrawWidth = 2;

BOOL bEDA_PcbOnLineDRCEnable = TRUE;

double fEDA_PcbRouteDelta = 500;

char szEDA_Help_Ready[] = {"就绪"};
char szEDA_Info_Ready[] = {"属性"};
char szEDA_HighlightColor[] = {"高亮颜色"};
char szEDA_BoardLineColor[] = {"板边线颜色"};
char szEDA_BoardAreaColor[] = {"板背景颜色"};
char szEDA_SheetLineColor[] = {"图框边线颜色"};
char szEDA_SheetAreaColor[] = {"图框背景颜色"};
char szEDA_WordspaceColor[] = {"工作区背景颜色"};
char szEDA_ProgramAbort[] = {"无法找到和打开系统元件库,可能库文件丢失或者ESD已经在运行中,程序终止。"};

CStringArray arrayEDA_SystemFont;
BOOL CALLBACK MyEnumFontFamProc(LPLOGFONT lplf,LPNEWTEXTMETRIC lpntm ,DWORD FontType,LPARAM aFontCount);

#endif // !defined(AFX_SYSTEM_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_)
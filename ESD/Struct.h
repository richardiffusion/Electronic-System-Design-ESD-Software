
#if !defined(ESD_Struct_H_INCLUDED_)
#define ESD_Struct_H_INCLUDED_

struct Fpoint
{
	double fx;
	double fy;
};

struct Struct_RegionVertex
{
	double fx;
	double fy;
	double cx;
	double cy;
	double sangle;
	double eangle;
	double radius;

};

struct Struct_VertexOutline
{
	int kind;
	double x1, y1, x2, y2;
	double cx, cy, r, sangle, eangle;
	int flag;
};

#include "ESD_SCH_Sheet.h"
#include "ESD_SCH_Line.h"
#include "ESD_SCH_PolyLine.h"
#include "ESD_SCH_Junc.h"
#include "ESD_SCH_Text.h"
#include "ESD_SCH_Arc.h"
#include "ESD_SCH_Ellipse.h"
#include "ESD_SCH_EllipseFill.h"
#include "ESD_SCH_Pie.h"
#include "ESD_SCH_Polygon.h"
#include "ESD_SCH_TextFrame.h"
#include "ESD_SCH_Rect.h"
#include "ESD_SCH_RoundRect.h"
#include "ESD_SCH_SheetSymbol.h"
#include "ESD_SCH_SheetEntry.h"
#include "ESD_SCH_PORT.h"
#include "ESD_SCH_Power.h"
#include "ESD_SCH_Bezier.h"
#include "ESD_SCH_Graphic.h"
#include "ESD_SCH_Net.h"

#include "ESD_SCH_Pin.h"
#include "ESD_SCH_Comp.h"
#include "ESD_SCH_Ieee.h"

#include "ESD_Lib_CompHead.h"

#include "ESD_PCB_Track.h"
#include "ESD_PCB_Arc.h"
#include "ESD_PCB_Via.h"
#include "ESD_PCB_Pad.h"
#include "ESD_PCB_Text.h"
#include "ESD_PCB_Region.h"
#include "ESD_PCB_Fill.h"
#include "ESD_PCB_Sheet.h"
#include "ESD_PCB_Comp.h"
#include "ESD_PCB_Polygon.h"
#include "ESD_PCB_Coordinate.h"
#include "ESD_PCB_Dimension.h"
#include "ESD_PCB_Net.h"
#include "ESD_PCB_Class.h"
#include "ESD_PCB_Connection.h"

#include "SchUndo.h"
#include "PcbUndo.h"


struct Struct_Sch_Sheet 
{
	int width, height, xregioncount, yregioncount, margin;
};


struct Struct_Pcb_Vector_Font
{ 
	float yoffset;
	float w;
	int number;
	int pu_pd[60][3];
};

struct Struct_Multi_Selection
{
	int element;
	POSITION pos;
};

struct Struct_Pcb_Multi_Selection
{
	int pcbelement;
	int index;
};

struct Struct_Font
{
	int nHeight;
	int nOrientation;
	BOOL bUnderline;
	BOOL bItalic;
	BOOL bBold;
	BOOL bStrikeout;
	CString csFontName;
};

struct Struct_Library_Head
{
	char format[28];
	int totalcompnum;
};

struct Struct_Comp_Head
{
	char name[100];
	char description[212];
	int offset;
	int length;
};

struct Struct_CompList
{
	char compname[100];
	BOOL basiclibrary;
	int liboffset;
	int offset;
	int length;
};

struct Struct_Find
{
	int nDiagram;
	POSITION pos;
};

struct Struct_Jump
{
	int nDiagram;
	double fx;
	double fy;
};

struct Struct_Annotate
{
	CString designator;
	POSITION comppos;
	POSITION textpos;
	double fx;
	double fy;
	int diagram;
	CString comment;
	int partcount;
	int partnum;
};

struct Struct_ObjectPosition
{
	POSITION pos;
	int diagram;
};

struct Struct_PCBLayerSet
{
	CString m_csSetName;
	BOOL m_bLayerShow[PCB_LAYER_NUM];
	int m_nActiveLayer;
	BOOL m_bFlipBoard;
	BOOL m_bLocked;
	BOOL m_bCurrentSelection;
};

struct Struct_Connection_Point
{
	double fx;
	double fy;
	int nLayer;
};

struct Struct_Route_Priority
{
	int nType;
	CString csName;
	int nPriority;
};

#endif // !defined(EasyEDA_Struct_H_INCLUDED_)
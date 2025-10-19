#if !defined(AFX_SYSPARAM_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_)
#define AFX_SYSPARAM_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_

#include "math.h"

#define SCH 0
#define PCB 1

#define DOC_ESD 1
#define DOC_AD6_SCH 3
#define DOC_AD6_PCB 4
#define DOC_99_SCH  5
#define DOC_99_PCB  6
#define DOC_DXF		7
#define DOC_AD6_SCH_BINARY 8

#define UNIT_MIL	0
#define UNIT_MM		1
#define UNIT_INCH	2
#define UNIT_CM		3
#define UNIT_DXP	4
#define UNIT_M		5
#define UNIT_AUTO_I	6
#define UNIT_AUTO_M	7
#define UNIT_AUTO	8

#define UNIT_PCB_M 0
#define UNIT_PCB_I 1

#define GRID_LINE	0
#define GRID_DOT	1

#define FLAG_NORMAL			0
#define FLAG_PROCESSING		1
#define FLAG_DELETED		0xff

#define MYID_PRINT	100
#define MYID_PRINT_SETUP 101
#define MYID_PRINT_PREVIEW 102
#define MYID_PRINT_DEFINE  103
#define MYID_SHEET_DEFINE  104

#define MAX_STRING_LENGTH   250

#define OVERWRITE_YES TRUE
#define OVERWRITE_NO  FALSE
#define	DRAWSTRING	TRUE
#define GETEXTENT	FALSE

#define SCH_UNDO_NUMBER 50
#define PCB_UNDO_NUMBER 50

#define LINE_LINE		0
#define LINE_WIRE		1
#define LINE_BUS		2
#define LINE_BUS_ENTRY	3

#define LINE_SOLID	0
#define LINE_DASHED 1
#define LINE_DOTTED 2

#define LINE_SMALLEST	0
#define LINE_SMALL		1
#define LINE_MEDIUM		2
#define LINE_LARGE		3

#define JUNCTION_SMALLEST   0
#define JUNCTION_SMALL		1
#define JUNCTION_MEDIUM		2
#define JUNCTION_LARGE		3

#define LINE_SHAPE_SMALLEST_WIDTH	40
#define LINE_SHAPE_SMALL_WIDTH		80
#define LINE_SHAPE_MEDIUM_WIDTH		120
#define LINE_SHAPE_LARGE_WIDTH		160

#define TEXT_ALIGNMENT_CENTER	0
#define TEXT_ALIGNMENT_LEFT		1
#define TEXT_ALIGNMENT_RIGHT	2
#define TEXT_ALIGNMENT_TOP		1
#define TEXT_ALIGNMENT_BOTTOM	2

#define ENTRY_SIDE_LEFT		0
#define ENTRY_SIDE_RIGHT	1
#define ENTRY_SIDE_TOP		2
#define ENTRY_SIDE_BOTTOM	3

#define ENTRY_STYLE_NH			0
#define ENTRY_STYLE_LEFT		1
#define ENTRY_STYLE_RIGHT		2
#define ENTRY_STYLE_LEFTRIGHT	3
#define ENTRY_STYLE_NV			4
#define ENTRY_STYLE_TOP			5
#define ENTRY_STYLE_BOTTOM		6
#define ENTRY_STYLE_TOPBOTTOM	7

#define ENTRY_TYPE_UNSPECIFIED		0
#define ENTRY_TYPE_OUTPUT			1
#define ENTRY_TYPE_INPUT			2
#define ENTRY_TYPE_BIDIRECTIONAL	3

#define PORT_TYPE_UNSPECIFIED		0
#define PORT_TYPE_OUTPUT			1
#define PORT_TYPE_INPUT				2
#define PORT_TYPE_BIDIRECTIONAL		3

#define ARROW_LEFT			0
#define ARROW_RIGHT			1
#define ARROW_TOP			2
#define ARROW_BOTTOM		3
#define ARROW_LEFT_RIGHT	4
#define ARROW_TOP_BOTTOM	5
#define NOARROW_LEFT_RIGHT	6
#define NOARROW_TOP_BOTTOM	7

#define PORT_STYLE_NH			0
#define PORT_STYLE_LEFT			1
#define PORT_STYLE_RIGHT		2
#define PORT_STYLE_LEFTRIGHT	3
#define PORT_STYLE_NV			4
#define PORT_STYLE_TOP			5
#define PORT_STYLE_BOTTOM		6
#define PORT_STYLE_TOPBOTTOM	7

#define TEXT_TEXT					0
#define TEXT_NETLABEL				1
#define TEXT_SHEETSYMBOLDESIGNATOR	2
#define TEXT_SHEETSYMBOLFILENAME	3
#define TEXT_SHEETSYMBOLTEXT		4
#define TEXT_COMPDESIGNATOR			5
#define TEXT_COMPCOMMENT			6
#define TEXT_COMPPACKAGE			7
#define TEXT_COMPTEXT				8
#define TEXT_PINTEXT				9
#define TEXT_SHEETTEXT				10

#define POWER_STYLE_CIRCLE			0
#define POWER_STYLE_ARROW			1
#define POWER_STYLE_BAR				2
#define POWER_STYLE_WAVE			3
#define POWER_STYLE_POWERGROUND		4
#define POWER_STYLE_SIGNALGROUND	5
#define POWER_STYLE_EARTH			6

#define TEXTPARAM_DESIGNATOR	0
#define TEXTPARAM_COMMENT		1
#define TEXTPARAM_OTHERS		2

#define SCH_PLACE_90_START	0
#define SCH_PLACE_90_END	1
#define SCH_PLACE_45_START	2
#define SCH_PLACE_45_END	3
#define SCH_PLACE_ANYANGLE	4
#define SCH_PLACE_AUTO		5

#define PCB_PLACE_90_START		0
#define PCB_PLACE_90_END		1
#define PCB_PLACE_90ARC_START	2
#define PCB_PLACE_90ARC_END		3
#define PCB_PLACE_45_START		4
#define PCB_PLACE_45_END		5
#define PCB_PLACE_45ARC_START	6
#define PCB_PLACE_45ARC_END 	7
#define PCB_PLACE_ANYANGLE		8

#define PCB_HATCH_SOLID	0
#define PCB_HATCH_90	1
#define PCB_HATCH_45	2
#define PCB_HATCH_H		3
#define PCB_HATCH_V		4
#define PCB_HATCH_NONE	5

#define PCB_RELIEF_CONNECT	0
#define PCB_DIRECT_CONNECT	1
#define PCB_NO_CONNECT		2

#define  MAX(x,y)  (((x) > (y)) ? (x) : (y))
#define  MIN(x,y)  (((x) < (y)) ? (x) : (y))

#define  OFF  0
#define  ON   1

#define  CONNECT_NO  FALSE
#define  CONNECT_YES TRUE

#define  SQRT(fx1,fy1,fx2,fy2)  sqrt((fx1-fx2)*(fx1-fx2)+(fy1-fy2)*(fy1-fy2))

#define  SCH_SHEET_NUM 20

#define  PCB_LAYER_NUM 82

#define LANDSCAPE	0
#define PORTRAIT	1

#define fEDA_SCH_SnapMaxx  Snap_Lock(fEDA_SCH_SizeMaxx)
#define fEDA_SCH_SnapMaxy  Snap_Lock(fEDA_SCH_SizeMaxy)

#define fEDA_PCB_SnapMaxx  Snap_Lock(fEDA_PCB_SizeMaxx)
#define fEDA_PCB_SnapMaxy  Snap_Lock(fEDA_PCB_SizeMaxy)

#define MAXOPENFILENUMBER 10

#define WITHOUT_MIL	0
#define WITH_MIL	1

#define COPPER_POLYGON 0
#define SPLITPLANE 1

#define SCH_SYSTEM_LIB_NUMBER	1130
#define PCB_SYSTEM_LIB_NUMBER	237

#define RELIEF_CONNECT 0
#define DIRECT_CONNECT 1
#define NO_CONNECT 2

#define PADHOLE_ROUND	0
#define PADHOLE_SQUARE	1
#define PADHOLE_SLOT	2

#define MANUAL			0
#define LEFTTOP			1
#define LEFTCENTER		2
#define LEFTBOTTOM		3
#define CENTERTOP		4
#define CENTERCENTER	5
#define CENTERBOTTOM	6
#define RIGHTTOP		7
#define RIGHTCENTER		8
#define RIGHTBOTTOM		9

#define TEXTPOSITION_AUTO		0
#define TEXTPOSITION_CENTER		1
#define TEXTPOSITION_TOP		2
#define TEXTPOSITION_BOTTOM		3
#define TEXTPOSITION_RIGHT		4
#define TEXTPOSITION_LEFT		5
#define TEXTPOSITION_INSIDERIGHT	6
#define TEXTPOSITION_INSIDELEFT		7
#define TEXTPOSITION_UNIDIRECTIONAL	8
#define TEXTPOSITION_MANUAL		9

#define FLAG_LEFT	0
#define FLAG_RIGHT	1
#define FLAG_TOP	2
#define FLAG_BOTTOM	3

#define UNIT_STYLE_NONE		0
#define UNIT_STYLE_NORMAL	1
#define UNIT_STYLE_BRACKETS	2

#define NOTHING_SELECTION			0
#define SOMETHING_SELECTION_MOVE	1
#define SOMETHING_SELECTION_COPY	2
#define KEY_SPOT_ON_SELECTION		3

#define THE_FIRST_ONE		0
#define MULTI_SELECTION		1
#define INCLUDE_CPAD		2
#define NOT_INCLUDE_CPAD	3
#define PCB_HIGHLIGHT_SELECTION 4

#define NO_ROUTE			0
#define HORIZONTAL_ROUTE	1
#define VERTICAL_ROUTE		2
#define ANY_ROUTE			3

#define DRAW_NORMAL		0
#define DRAW_DRAFT		1

#define  TOPLAYER		0
#define  MIDLAYER1		1
#define  MIDLAYER2		2
#define  MIDLAYER3		3
#define  MIDLAYER4		4
#define  MIDLAYER5		5
#define  MIDLAYER6		6
#define  MIDLAYER7		7	
#define  MIDLAYER8		8
#define  MIDLAYER9		9 
#define  MIDLAYER10		10
#define  MIDLAYER11		11
#define  MIDLAYER12		12
#define  MIDLAYER13		13
#define  MIDLAYER14		14
#define  MIDLAYER15		15
#define  MIDLAYER16		16
#define  MIDLAYER17		17
#define  MIDLAYER18		18
#define  MIDLAYER19		19
#define  MIDLAYER20		20
#define  MIDLAYER21		21
#define  MIDLAYER22		22
#define  MIDLAYER23		23
#define  MIDLAYER24		24
#define  MIDLAYER25		25
#define  MIDLAYER26		26
#define  MIDLAYER27		27
#define  MIDLAYER28		28
#define  MIDLAYER29		29
#define  MIDLAYER30		30
#define  BOTTOMLAYER	31

#define  TOPOVERLAY		32
#define  BOTTOMOVERLAY	33

#define  TOPPASTE		34
#define  BOTTOMPASTE	35
#define  TOPSOLDER		36
#define  BOTTOMSOLDER	37

#define  POWERPLANE1	38
#define  POWERPLANE2	39
#define  POWERPLANE3	40
#define  POWERPLANE4	41
#define  POWERPLANE5	42
#define	 POWERPLANE6	43
#define  POWERPLANE7	44
#define  POWERPLANE8	45
#define  POWERPLANE9	46
#define  POWERPLANE10	47
#define  POWERPLANE11	48
#define  POWERPLANE12	49
#define  POWERPLANE13	50
#define  POWERPLANE14	51
#define  POWERPLANE15	52
#define  POWERPLANE16	53

#define  DRILLGUIDE		54
#define  KEEPOUTLAYER	55

#define  MECHANICAL1	56
#define  MECHANICAL2	57
#define  MECHANICAL3	58
#define  MECHANICAL4	59
#define  MECHANICAL5	60
#define  MECHANICAL6	61
#define  MECHANICAL7	62
#define  MECHANICAL8	63
#define  MECHANICAL9	64
#define  MECHANICAL10	65
#define  MECHANICAL11	66
#define  MECHANICAL12	67
#define  MECHANICAL13	68
#define  MECHANICAL14	69
#define  MECHANICAL15	70
#define  MECHANICAL16	71

#define  DRILLDRAWING	72
#define  MULTILAYER		73
#define  CONNECTIONS	74
#define  BACKGROUND		75
#define  DRCERROR		76
#define  SELECTIONS		77
#define  VISIBLEGRID1	78
#define  VISIBLEGRID2	79
#define  PADHOLES		80
#define  VIAHOLES		81

#define  POWER_LAYER_NUM	16
#define  SIGNAL_LAYER_NUM	32

#define  PAD_SHAPE_ROUND			0
#define  PAD_SHAPE_RECTANGLE		1
#define  PAD_SHAPE_OCTAGONAL		2
#define  PAD_SHAPE_ROUNDEDRECTANGLE	3

#define  PAD_MODE_SIMPLE		0
#define  PAD_MODE_T_M_B			1
#define  PAD_MODE_FULLSTACK		2

#define  PAD_ELECTYPE_LOAD			0
#define  PAD_ELECTYPE_TERMINATOR	1
#define  PAD_ELECTYPE_SOURCE		2

#define SCH_ELEMENT_COMPONENT	1
#define SCH_ELEMENT_PIN			2
#define SCH_ELEMENT_IEEE		3
#define SCH_ELEMENT_TEXT		4
#define SCH_ELEMENT_BEZIER		5
#define SCH_ELEMENT_POLYLINE	6
#define SCH_ELEMENT_POLYGON		7
#define SCH_ELEMENT_ELLIPSEFILL	8
#define SCH_ELEMENT_PIE			9
#define SCH_ELEMENT_ROUNDRECT	10
#define SCH_ELEMENT_ELLIPSE		11
#define SCH_ELEMENT_ARC			12
#define SCH_ELEMENT_LINE		13
#define SCH_ELEMENT_RECT		14
#define SCH_ELEMENT_SHEETSYMBOL	15
#define SCH_ELEMENT_SHEETENTRY	16
#define SCH_ELEMENT_POWER		17
#define SCH_ELEMENT_PORT		18
#define SCH_ELEMENT_NETLABEL	25
#define SCH_ELEMENT_TEXTFRAME	28
#define SCH_ELEMENT_JUNCTION	29
#define SCH_ELEMENT_GRAPHIC		30
#define SCH_ELEMENT_SHEET		31

#define SCH_ELEMENT_BUS			26
#define SCH_ELEMENT_WIRE		27
#define SCH_ELEMENT_BUSENTRY    37

#define PCB_ELEMENT_TRACK		1
#define PCB_ELEMENT_ARC			2
#define PCB_ELEMENT_PAD			3
#define PCB_ELEMENT_VIA			4
#define PCB_ELEMENT_TEXT		5
#define PCB_ELEMENT_FILL		6
#define PCB_ELEMENT_REGION		7
#define PCB_ELEMENT_COMP		8
#define PCB_ELEMENT_POLYGON		9
#define PCB_ELEMENT_COORDINATE	10
#define PCB_ELEMENT_DIMENSION	11
#define PCB_ELEMENT_NET			12

#define  ROUTESEQUENCE      1
#define  ROUTESHORTESTTREE  0
#define  ROUTEDELTA        50
#define  ALLNET             0
#define  PARTNETONLY        1

#define  UNDO_MOVE			0
#define  UNDO_CHANGE		1
#define  UNDO_DELETE		2
#define  UNDO_UNDELETE		3

#define TAB       0x09
#define ENTER     0xD
#define ESC       0x1B
#define BACKSP    0x8
#define SPACE     0x20

#define KEY_0     0x30
#define KEY_1     0x31
#define KEY_2     0x32
#define KEY_3     0x33
#define KEY_4     0x34
#define KEY_5     0x35
#define KEY_6     0x36
#define KEY_7     0x37
#define KEY_8     0x38
#define KEY_9     0x39
#define KEY_A     0x41
#define KEY_B     0x42
#define KEY_C     0x43
#define KEY_D     0x44
#define KEY_E     0x45
#define KEY_F     0x46
#define KEY_G     0x47
#define KEY_H     0x48
#define KEY_I     0x49
#define KEY_J     0x4A
#define KEY_K     0x4B
#define KEY_L     0x4C
#define KEY_M     0x4D
#define KEY_N     0x4E
#define KEY_O     0x4F
#define KEY_P     0x50
#define KEY_Q     0x51
#define KEY_R     0x52
#define KEY_S     0x53
#define KEY_T     0x54
#define KEY_U     0x55
#define KEY_V     0x56
#define KEY_W     0x57
#define KEY_X     0x58
#define KEY_Y     0x59
#define KEY_Z     0x5A
#define KEY_a     0x61
#define KEY_b     0x62
#define KEY_c     0x63
#define KEY_d     0x64
#define KEY_e     0x65
#define KEY_f     0x66
#define KEY_g     0x67
#define KEY_h     0x68
#define KEY_i     0x69
#define KEY_j     0x6A
#define KEY_k     0x6B
#define KEY_l     0x6C
#define KEY_m     0x6D
#define KEY_n     0x6E
#define KEY_o     0x6F
#define KEY_p     0x70
#define KEY_q     0x71
#define KEY_r     0x72
#define KEY_s     0x73
#define KEY_t     0x74
#define KEY_u     0x75
#define KEY_v     0x76
#define KEY_w     0x77
#define KEY_x     0x78
#define KEY_y     0x79
#define KEY_z     0x7A

#define DEL       0x153
#define INS       0x152
#define HOME      0x147
#define END       0x14F
#define PGUP      0x149
#define PGDN      0x151
#define UP        0x148
#define DOWN      0x150
#define LEFT      0x14B
#define RIGHT     0x14D

#define F1        0x13B
#define F2        0x13C
#define F3        0x13D
#define F4        0x13E
#define F5        0x13F
#define F6        0x140
#define F7        0x141
#define F8        0x142
#define F9        0x143
#define F10       0x144
#define F11       0x185
#define F12       0x186

#define SHIFT_F1  0x154
#define SHIFT_F2  0x155
#define SHIFT_F3  0x156
#define SHIFT_F4  0x157
#define SHIFT_F5  0x158
#define SHIFT_F6  0x159
#define SHIFT_F7  0x15A
#define SHIFT_F8  0x15B
#define SHIFT_F9  0x15C
#define SHIFT_F10 0x15D
#define SHIFT_F11 0x187
#define SHIFT_F12 0x188

#define ALT_A     0x11E
#define ALT_B     0x130
#define ALT_C     0x12E
#define ALT_D     0x120
#define ALT_E     0x112
#define ALT_F     0x121
#define ALT_G     0x122
#define ALT_H     0x123
#define ALT_I     0x117
#define ALT_J     0x124
#define ALT_K     0x125
#define ALT_L     0x126
#define ALT_M     0x132
#define ALT_N     0x131
#define ALT_O     0x118
#define ALT_P     0x119
#define ALT_Q     0x110
#define ALT_R     0x113
#define ALT_S     0x11F
#define ALT_T     0x114
#define ALT_U     0x116
#define ALT_V     0x12F
#define ALT_W     0x111
#define ALT_X     0x12D
#define ALT_Y     0x115
#define ALT_Z     0x12C

#define ALT_BKSP  0x10E
#define ALT_TAB   0x1A5

#define ALT_F1    0x168
#define ALT_F2    0x169
#define ALT_F3    0x16A
#define ALT_F4    0x16B
#define ALT_F5    0x16C
#define ALT_F6    0x16D
#define ALT_F7    0x16E
#define ALT_F8    0x16F
#define ALT_F9    0x170
#define ALT_F10   0x171
#define ALT_F11   0x18B
#define ALT_F12   0x18C

#define CTR_A     0x01
#define CTR_B     0x02
#define CTR_C     0x03
#define CTR_D     0x04
#define CTR_E     0x05
#define CTR_F     0x06
#define CTR_G     0x07
#define CTR_H     0x08
#define CTR_I     0x09
#define CTR_J     0x0A
#define CTR_K     0x0B
#define CTR_L     0x0C
#define CTR_M     0x0D
#define CTR_N     0x0E
#define CTR_O     0x0F
#define CTR_P     0x010
#define CTR_Q     0x011
#define CTR_R     0x012
#define CTR_S     0x013
#define CTR_T     0x014
#define CTR_U     0x015
#define CTR_V     0x016
#define CTR_W     0x017
#define CTR_X     0x018
#define CTR_Y     0x019
#define CTR_Z     0x01A

#define CTR_BKSP  0x07F
#define CTR_ENTER 0x0A
#define CTR_TAB   0x194
#define CTR_DEL   0x193
#define CTR_INS   0x192
#define CTR_RIGHT 0x174
#define CTR_LEFT  0x173
#define CTR_HOME  0x177
#define CTR_END   0x175
#define CTR_UP    0x18D
#define CTR_DOWN  0x191
#define CTR_PGUP  0x184
#define CTR_PGDN  0x176

#define SHIFT_TAB       0x10F
#define SHIFT_DEL       0x253
#define SHIFT_INS       0x252
#define SHIFT_HOME      0x247
#define SHIFT_END       0x24F
#define SHIFT_PGUP      0x249
#define SHIFT_PGDN      0x251
#define SHIFT_UP        0x248
#define SHIFT_DOWN      0x250
#define SHIFT_LEFT      0x24B
#define SHIFT_RIGHT     0x24D

#define PUSHMOUSE       0x254
#define MOUSEMOVE       0x255
#define PUSHMOUSEESC    0x256
#define MOUSE_LBUTTON_UP	0x257
#define MOUSE_RBUTTON_UP	0x258

#endif // !defined(AFX_SYSPARAM_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_)
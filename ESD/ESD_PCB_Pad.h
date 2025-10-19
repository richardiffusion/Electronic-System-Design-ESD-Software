#if !defined(EDA_SELF_DEFINATION_COB_PCBPAD)
#define EDA_SELF_DEFINATION_COB_PCBPAD

#include "sysparam.h"

class  COb_pcbpad: public CObject
{
	public:
	COb_pcbpad();

protected:
	DECLARE_SERIAL(COb_pcbpad)

// Attributes
public:
	double m_fCx, m_fCy;
	double m_fRotation;	
	
	int m_nPadMode;

	double m_fSizeX[SIGNAL_LAYER_NUM], m_fSizeY[SIGNAL_LAYER_NUM];
	int m_nShape[SIGNAL_LAYER_NUM];	

	int m_nHoleType;  
	double m_fHoleDia;
	double m_fHoleRotation;
	double m_fHoleWidth;  


	CString m_csName;
	int m_nLayer;
	int m_nNet;
	int m_nComponent;

	BOOL m_bLocked;
	BOOL m_bPlated;
	int m_nDaisychain; 
	int m_nJumperID;
	
	BOOL m_bTestingTop;
	BOOL m_bTestingBottom;
	BOOL m_bTenting;
	BOOL m_bTentingTop;
	BOOL m_bTentingBottom;

	BOOL m_bPolygonOutline;
	BOOL m_bUserRouted;
	int m_nUnionIndex;

	int m_nCPLV;	
	int m_nCPL;
	
	int m_nCPEV;	
	double m_fCPE;	
	int m_nCSEV;	
	double m_fCSE;	
	int m_nCPCV;	
	double m_fCPC;	
	int m_nCCSV;	
	int m_nCCS;		
	int m_nCENV;	
	int m_nCEN;		
	int m_nCCWV;	
	double m_fCCW;	
	int	m_nCAGV;	
	double m_fCAG;	
	int m_nCPRV;	
	double m_fCPR;	
	
	BOOL m_bSelection;
	int m_nFlag;

// Operations
public:
	CDocument* pDocument;
	
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
	void DrawInLayerWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy, int Layer);
	
	void Draw(CDC* pDC, CView* pV, int mode);
	void DrawSingleLayerPadSpecial(CDC* pDC, CView* pV, int layer);

	void DrawPadRound(CDC* pDC, CView* pV, int layer, int color_layer, double expansion);
	void DrawPadRectangle(CDC* pDC, CView* pV, int layer, int color_layer, double expansion);
	void DrawPadOctagonal(CDC* pDC, CView* pV, int layer, int color_layer, double expansion);
	void DrawPadRoundedRectangle(CDC* pDC, CView* pV, int layer, int color_layer, double expansion);
	BOOL PickMe(CView* pV);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	BOOL Is_On_Me(double fx, double fy);
	BOOL Is_On_Round(double fx, double fy, int layer);
	BOOL Is_On_Rect(double fx, double fy, int layer);
	BOOL Is_On_Oct(double fx, double fy, int layer);
	void GetRectCorner(double *fx11, double *fy11, double *fx22, double *fy22, double *fx33, double *fy33, double *fx44, double *fy44, int layer);
	void Copy(COb_pcbpad* ppad);
	void Change(int index);
	BOOL IsVisible();
	BOOL ConnectToInternalPlane(int planelayer);
	BOOL DrawTopPaste(CDC* pDC, CView* pV);
	BOOL DrawBottomPaste(CDC* pDC, CView* pV);
	BOOL DrawTopSolder(CDC* pDC, CView* pV);
	BOOL DrawBottomSolder(CDC* pDC, CView* pV);
	BOOL DrawInternalPlane(CDC* pDC, CView* pV);
	BOOL DrawPad(CDC* pDC, CView* pV);
	BOOL DrawPadHoles(CDC* pDC, CView* pV);
	void DrawPlaneMark(CDC* pDC, CView* pV);
	void DrawNetName(CDC* pDC, CView* pV);
	void Move(CClientDC* pDC, CView* pV,int index);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);

	//Selection
	void DrawSelection(CDC* pDC, CView* pV);
	void DrawSelectedPadRectangle(CDC* pDC, CView* pV, int layer, int color_layer, double expansion);
	void DrawSelectedPadOctagonal(CDC* pDC, CView* pV, int layer, int color_layer, double expansion);
	void DrawSelectedPadRound(CDC* pDC, CView* pV, int layer, int color_layer, double expansion);

	void DrawHighlighted(CDC* pDC, CView* pV, int layer);
	BOOL TearDropExist(CView* pV);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCBPAD)



#if !defined(EDA_SELF_DEFINATION_COB_PCBVIA)
#define EDA_SELF_DEFINATION_COB_PCBVIA

class  COb_pcbvia: public CObject
{
	public:
	COb_pcbvia();

protected:
	DECLARE_SERIAL(COb_pcbvia)

// Attributes
public:
	double m_fCx, m_fCy;
	double m_fDia;
	double m_fHoleDia;
	int m_nStartLayer;
	int m_nEndLayer;

	int m_nLayer;
	int m_nNet;
	int m_nComponent;
	int m_nPolygon;

	BOOL m_bLocked;

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
	void Copy(COb_pcbvia* ppcbvia);
	BOOL IsVisible();
	void Draw(CDC* pdc, CView* pV, int mode);
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
	void DrawSelection(CDC* pDC, CView* pV);
	BOOL PickMe(CView* pV);
	BOOL Is_On_Me(double fx, double fy);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	void Change(int index);
	BOOL ConnectToInternalPlane(int planelayer);
	BOOL DrawTopSolder(CDC* pDC, CView* pV);
	BOOL DrawBottomSolder(CDC* pDC, CView* pV);
	BOOL DrawInternalPlane(CDC* pDC, CView* pV);
	BOOL DrawVia(CDC* pDC, CView* pV);
	BOOL DrawViaHoles(CDC* pDC, CView* pV);
	void DrawPlaneMark(CDC* pDC, CView* pV);
	void DrawNetName(CDC* pDC, CView* pV);

	void Move(CClientDC* pDC, CView* pV,int index);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);
	
	void DrawHighlighted(CDC* pDC, CView* pV, int layer);
	BOOL TearDropExist(CView* pV);

	void GetViaVertexOutline( CArray<Struct_VertexOutline, Struct_VertexOutline&>& arrayVertexOutline, double drc);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCBVIA)

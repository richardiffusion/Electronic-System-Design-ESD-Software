#if !defined(EDA_SELF_DEFINATION_COB_PCBTRACK)
#define EDA_SELF_DEFINATION_COB_PCBTRACK

class  COb_pcbtrack: public CObject
{
	public:
	COb_pcbtrack();

protected:
	DECLARE_SERIAL(COb_pcbtrack)

// Attributes
public:
	double m_fX1, m_fY1, m_fX2, m_fY2;
	double m_fWidth;

	int m_nLayer;
	int m_nNet;
	int m_nComponent;
	int m_nPolygon;

	BOOL m_bLocked;
	BOOL m_bKeepout;

	BOOL m_bPolygonOutline;
	BOOL m_bTearDrop;

	int m_nUnionIndex;
	int m_nSubPolyIndex;	

	BOOL m_bSelection;
	int m_nFlag;
	
	// Operations
public:
	CDocument* pDocument;
	void Copy(COb_pcbtrack* ppcbtrack);
	void Draw(CDC* pdc,CView* pV);
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
	void DrawSelection(CDC* pDC, CView* pV);
	void Change(int index);
	BOOL Is_On_Me(double fx, double fy);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	BOOL PickMe(CView* pV);
	void Move(CClientDC* pDC, CView* pV, int index);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);
	void MoveAndRotate(CView* pV, double x0, double y0,  double deltax, double deltay, int rotation, BOOL mirror, int index);

	void DrawHighlighted(CDC* pDC, CView* pV, int layer);
	void GetTrackVertexOutline( CArray<Struct_VertexOutline, Struct_VertexOutline&>& arrayVertexOutline, double drc);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCBTRACK)
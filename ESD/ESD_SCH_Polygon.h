#if !defined(EDA_SELF_DEFINATION_COB_SCHPOLY)
#define EDA_SELF_DEFINATION_COB_SCHPOLY

class  COb_schpolygon: public CObject
{
	public:
	COb_schpolygon();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schpolygon)

// Attributes
public:

	int m_nBorderWidth;
	COLORREF m_nFillColor;
	COLORREF m_nBorderColor;
	BOOL m_bTransparent;
	BOOL m_bFillSolid;
	
	CArray<Fpoint, Fpoint&> m_cVertex;

	int m_nOwnerPartID;
	int m_nDisplayMode;
	POSITION m_posParent;

	int m_nDiagram;
	BOOL m_bSelection;
	int m_nFlag;

	// Operations
public:
	CDocument* pDocument;
	void Copy(COb_schpolygon* pschpolygon);
	void Draw(CDC* pDC, CView* pV);
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
	BOOL PickMe(CView* pV);
    BOOL Is_On_Me(double x, double y);
	void Change(POSITION);
	void Move(CClientDC* pDC, CView* pV, POSITION pos);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	void DrawSelection(CDC* pDC, CView* pV);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHPOLY)
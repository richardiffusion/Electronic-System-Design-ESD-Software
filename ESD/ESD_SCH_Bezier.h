#if !defined(EDA_SELF_DEFINATION_COB_SCHBEZIER)
#define EDA_SELF_DEFINATION_COB_SCHBEZIER

class  COb_schbezier: public CObject
{
	public:
	COb_schbezier();

protected:
	DECLARE_SERIAL(COb_schbezier)

// Attributes
public:
	int m_nWidth;		
	COLORREF m_nColor;					

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
	void Draw(CDC* pdc,CView* pV);
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
	void Copy(COb_schbezier* pbezier);
	BOOL Is_On_Me(double fx, double fy);
	BOOL PickMe(CView* pV);
	void Change(POSITION pos);
	void Move(CClientDC* pDC, CView* pV, POSITION pos);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	void DrawSelection(CDC* pDC, CView* pV);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHBEZIER)
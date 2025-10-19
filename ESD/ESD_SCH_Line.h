#if !defined(EDA_SELF_DEFINATION_COB_SCHLINE)
#define EDA_SELF_DEFINATION_COB_SCHLINE

class  COb_schline: public CObject
{
	public:
	COb_schline();

protected:
	DECLARE_SERIAL(COb_schline)

// Attributes
public:
	int m_nAttrib;		

	int m_nLineWidth;	
	int m_nLineType;	
	COLORREF m_nColor;					

	int m_nDiagram;
	
	BOOL m_bSelection;
	int m_nFlag;
	
	CArray<Fpoint, Fpoint&> m_cVertex;


	// Operations
public:
	CDocument* pDocument;
	void Copy(COb_schline* pline);
	void Draw(CDC* pdc,CView* pV);
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
	BOOL Is_On_Me(double fx, double fy);
	BOOL PickMe(CView* pV);
	void Change(POSITION pos);
	void Move(CClientDC* pDC, CView* pV, POSITION pos);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	void DrawSelection(CDC* pDC, CView* pV);
	void CombineVertex(void);
	void DrawHighlight(CDC* pDC, CView* pV, COLORREF color);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHLINE)
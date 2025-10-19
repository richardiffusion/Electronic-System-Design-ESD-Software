#if !defined(EDA_SELF_DEFINATION_COB_SCHPOLYLINE)
#define EDA_SELF_DEFINATION_COB_SCHPOLYLINE

class  COb_schpolyline: public CObject
{
	public:
	COb_schpolyline();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schpolyline)

// Attributes
public:
	int m_nLineWidth;		
	int m_nLineType;		
	int m_nStartLineShape;	
	int m_nEndLineShape;	
	int m_nLineShapeSize;   
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
	void Copy(COb_schpolyline* pschpolyline);
	void Draw(CDC* pdc,CView* pV);
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
	BOOL Is_On_Me(double fx, double fy);
	void Change(POSITION pos);
	void Move(CClientDC* pDC, CView* pV, POSITION pos);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	void DrawSelection(CDC* pDC, CView* pV);
	void CombineVertex(void);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHPOLYLINE)
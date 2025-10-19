
#if !defined(EDA_SELF_DEFINATION_COB_SCHROUNDRECT)
#define EDA_SELF_DEFINATION_COB_SCHROUNDRECT

class  COb_schroundrect: public CObject
{
	public:
	COb_schroundrect();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schroundrect)

// Attributes
public:
    double m_fX1, m_fY1, m_fX2, m_fY2;
	double m_fRadius_x;
	double m_fRadius_y;
	int m_nBorderWidth;
	COLORREF m_nFillColor;
	COLORREF m_nBorderColor;
	//BOOL m_bTransparent;
	BOOL m_bFillSolid;

	int m_nOwnerPartID;
	int m_nDisplayMode;
	POSITION m_posParent;

	int m_nDiagram;
	BOOL m_bSelection;
	int m_nFlag;

// Operations
public:
	CDocument* pDocument;
	void Copy(COb_schroundrect* pschroundrect);
	void Draw(CDC* pdc, CView* pV);
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
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

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHROUNDRECT)
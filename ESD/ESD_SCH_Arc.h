#if !defined(EDA_SELF_DEFINATION_COB_SCHARC)
#define EDA_SELF_DEFINATION_COB_SCHARC

class  COb_scharc: public CObject
{
	public:
	COb_scharc();

protected:
	DECLARE_SERIAL(COb_scharc)

// Attributes
public:
	double m_fCx, m_fCy;
	double m_fRadius;
	double m_fSangle; 
	double m_fEangle; 

	int m_nWidth;
	COLORREF m_nColor;
	
	int m_nOwnerPartID;
	int m_nDisplayMode;
	POSITION m_posParent;	

	int m_nDiagram;
	BOOL m_bSelection;
	int m_nFlag;

// Operations
public:
	CDocument* pDocument;
	void Copy(COb_scharc* pscharc);
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

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHARC)
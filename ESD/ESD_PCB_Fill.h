
#if !defined(EDA_SELF_DEFINATION_COB_PCBFILL)
#define EDA_SELF_DEFINATION_COB_PCBFILL

class  COb_pcbfill: public CObject
{
	public:
	COb_pcbfill();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_pcbfill)

// Attributes
public:
    double m_fX1, m_fY1, m_fX2, m_fY2;
	double m_fRotation;

	int m_nLayer;
	int m_nNet;
	int m_nComponent;

	BOOL m_bLocked;
	BOOL m_bKeepout;

	BOOL m_bPolygonOutline;
	BOOL m_bUserRouted;
	int m_nUnionIndex;

	BOOL m_bSelection;
	int m_nFlag;

// Operations
public:
	CDocument* pDocument;
	BOOL PickMe(CView* pV);
	void Draw(CDC* pdc, CView* pV);
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
	void DrawSelection(CDC* pDC, CView* pV);
	BOOL Is_On_Me(double fx, double fy);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	void GetFillCorner(double *fx11, double *fy11, double *fx22, double *fy22, double *fx33, double *fy33, double *fx44, double *fy44);
	void Copy(COb_pcbfill* ppcbfill);
	void Change(int index);
	void Move(CClientDC* pDC, CView* pV, int index);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);

	void DrawSelectionRect(CDC* pDC, CView* pV);
	void DrawFrame(CDC* pDC, CView* pV);
	
	void DrawHighlighted(CDC* pDC, CView* pV, int layer);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCBFILL)


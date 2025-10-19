
#if !defined(EDA_SELF_DEFINATION_COB_SCHJUNC)
#define EDA_SELF_DEFINATION_COB_SCHJUNC

class  COb_schjunc: public CObject
{
	public:
	COb_schjunc();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schjunc)

// Attributes
public:
	double m_fCx, m_fCy;
	int m_nSize;
	COLORREF m_nColor;
	BOOL m_bManual;

	int m_nDiagram;
	BOOL m_bSelection;
	int m_nFlag;

public:
	CDocument* pDocument;
	void Copy(COb_schjunc* pschjunc);
	void Draw(CDC* pdc, CView* pV);
	BOOL Is_On_Me(double fx, double fy);
	void Change(POSITION pos);
	void Move(CClientDC* pDC, CView* pV, POSITION pos);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	void DrawSelection(CDC* pDC, CView* pV);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHJUNC)
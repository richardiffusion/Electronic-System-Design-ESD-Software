#if !defined(EDA_SELF_DEFINATION_COB_SCHTEXTFRAME)
#define EDA_SELF_DEFINATION_COB_SCHTEXTFRAME

class  COb_schtextframe: public CObject
{
	public:
	COb_schtextframe();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schtextframe)

// Attributes
public:

    double m_fX1, m_fY1, m_fX2, m_fY2;

	int m_nBorderWidth;
	CStringArray m_arrayText;

	int m_nFont;

	BOOL m_bClipToArea;
	BOOL m_bWordWrap;
	int m_nAlignment;
	COLORREF m_nTextColor;	

	BOOL m_bDrawBorder;
	COLORREF m_nBorderColor;
	BOOL m_bFillSolid;
	COLORREF m_nFillColor;

	int m_nOwnerPartID;
	int m_nDisplayMode;
	POSITION m_posParent;

	int m_nDiagram;
	BOOL m_bSelection;
	int m_nFlag;

// Operations
public:
	CDocument* pDocument;
	void Copy(COb_schtextframe* pschtextframe);
	void Draw(CDC* pdc, CView* pV);
	//void DrawOverwrite(CDC* pdc);
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

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHTEXTFRAME)
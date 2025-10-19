
#if !defined(EDA_SELF_DEFINATION_COB_SCHGRAPHIC)
#define EDA_SELF_DEFINATION_COB_SCHGRAPHIC

class  COb_schgraphic: public CObject
{
	public:
	COb_schgraphic();

protected:
	DECLARE_SERIAL(COb_schgraphic)

// Attributes
public:
    double m_fX1, m_fY1, m_fX2, m_fY2;

	int m_nBorderWidth;
	COLORREF m_nBorderColor;
	BOOL m_bAspect;
	BOOL m_bBorderOn;
	BOOL m_bEmbedded;
	CString m_csFileName;

	int m_nOwnerPartID;
	int m_nDisplayMode;
	POSITION m_posParent;

	int m_nDiagram;
	BOOL m_bSelection;
	int m_nFlag;
	
// Operations
public:
	CDocument* pDocument;
	void Copy(COb_schgraphic* pschgraphic);
	void Draw(CDC* pdc, CView* pV);
	void LoadBitmap();
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

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHGRAPHIC)

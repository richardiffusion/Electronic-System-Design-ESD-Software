
#if !defined(EDA_SELF_DEFINATION_COB_SCHPORT)
#define EDA_SELF_DEFINATION_COB_SCHPORT

class  COb_schport: public CObject
{
	public:
	COb_schport();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schport)

// Attributes
public:
	double m_fX, m_fY;
	double m_fSize;

	int m_nStyle;	
	int m_nType;	
	CString m_csUniqueID;
	
	COLORREF m_nFillColor;
	COLORREF m_nBorderColor;
	COLORREF m_nTextColor;

	CString m_csName;
	int m_nAlignment;

	int m_nDiagram;
	BOOL m_bSelection;
	int m_nFlag;


// Operations
public:
	CDocument* pDocument;
	void Copy(COb_schport* pschport);
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

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHPORT)
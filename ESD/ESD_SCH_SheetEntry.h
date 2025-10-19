
#if !defined(EDA_SELF_DEFINATION_COB_SCHSHEETENTRY)
#define EDA_SELF_DEFINATION_COB_SCHSHEETENTRY

class  COb_schsheetentry: public CObject
{
	public:
	COb_schsheetentry();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schsheetentry)

// Attributes
public:
	int m_nSide;	// Left, Right, Top, Bottom
	int m_nStyle;	// No(Horizontal),Left, Right, Left&Right,  No(Vertical), Top, Bottom, Top&Bottom
	int m_nType;	// unspecified output input bidirectional
	double m_fOffset;
	
	COLORREF m_nFillColor;
	COLORREF m_nBorderColor;
	COLORREF m_nTextColor;

	CString m_csName;

	int m_nDiagram;
	BOOL m_bSelection;
	int m_nFlag;

	POSITION m_posParent;

// Operations
public:
	CDocument* pDocument;
	void Copy(COb_schsheetentry* pschsheetentry);
	void Draw(CDC* pdc, CView* pV);
	BOOL Is_On_Me(double fx, double fy);
	void Change(POSITION pos);
	void Move(CClientDC* pDC, CView* pV, POSITION pos);
	void DrawSelection(CDC* pDC, CView* pV);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHSHEETENTRY)
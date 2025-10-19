
#if !defined(EDA_SELF_DEFINATION_COB_SCHPOWER)
#define EDA_SELF_DEFINATION_COB_SCHPOWER

class  COb_schpower: public CObject
{
	public:
	COb_schpower();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schpower)

// Attributes
public:
	double m_fX, m_fY;
	int m_nStyle;
	int m_nOrientation;	
	BOOL m_bShowNetName;
	BOOL m_bIsCrossSheetConnector;
	CString m_csNetName;
	
	COLORREF m_nColor;

	int m_nDiagram;
	BOOL m_bSelection;
	int m_nFlag;


// Operations
public:
	CDocument* pDocument;
	void Copy(COb_schpower* pschpower);
	void Draw(CDC* pdc, CView* pV);
	BOOL Is_On_Me(double ffx, double ffy, CView* pV);
	void Change(POSITION pos);
	void Move(CClientDC* pDC, CView* pV, POSITION pos);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	void DrawSelection(CDC* pDC, CView* pV);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHPOWER)
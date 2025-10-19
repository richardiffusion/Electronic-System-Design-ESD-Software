
#if !defined(EDA_SELF_DEFINATION_COB_SCHSHEETSYMBOL)
#define EDA_SELF_DEFINATION_COB_SCHSHEETSYMBOL

class  COb_schsheetsymbol: public CObject
{
	public:
	COb_schsheetsymbol();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schsheetsymbol)

// Attributes
public:
    double m_fX, m_fY;
	double m_fXsize;
	double m_fYsize;

	int m_nBorderWidth;
	COLORREF m_nFillColor;
	COLORREF m_nBorderColor;
	BOOL m_bFillSolid;
	BOOL m_bShowHiddenTextFields;

	CArray <POSITION, POSITION&> m_arrayPos;
	//CString m_csDesignator;
	//CString m_csFilename;
	CString m_csUniqueID;

	int m_nDiagram;
	BOOL m_bSelection;
	int m_nFlag;

	
	//POSITION pos;
// Operations
public:
	CDocument* pDocument;
	void Copy(COb_schsheetsymbol* pschsheetsymbol);
	void Draw(CDC* pdc, CView* pV);
	BOOL Is_On_Me(double fx, double fy);
	void Change(POSITION pos);
	CString GetDesignator(void);
	CString GetFilename(void);
	void SetSheetSymbolDesignator(CString& cstring);
	void SetSheetSymbolFilename(CString& cstring);

	void Move(CClientDC* pDC, CView* pV, POSITION pos);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	void DrawSelection(CDC* pDC, CView* pV);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHSHEETSYMBOL)
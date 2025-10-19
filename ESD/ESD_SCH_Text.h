#if !defined(EDA_SELF_DEFINATION_COB_SCHTEXT)
#define EDA_SELF_DEFINATION_COB_SCHTEXT

class  COb_schtext: public CObject
{
	public:
	COb_schtext();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schtext)

// Attributes
public:
	int m_nID; 	

	double m_fSx, m_fSy;
	int m_nOrientation;

	CString m_csText;
	BOOL m_bTextVisible;
	BOOL m_bTextLocked;
	
	CString m_csNameComment;
	BOOL m_bNameVisible;
	BOOL m_bNameLocked;

	int m_nFont;

	COLORREF m_nColor;
	
	BOOL m_bMirror; 
	BOOL m_bAutoPosition;
	int  m_nParamType; 
	CString m_csUniqueID;
	BOOL m_bAllowDataBaseSynchronize;
	BOOL m_bAllowLibrarySynchronize;

	int m_nH_Justification;
	int m_nV_Justification;

	int m_nAnChorH;
	int m_nAnChorV;

	int m_nOwnerPartID;
	int m_nDisplayMode;
	POSITION m_posParent;

	int m_nDiagram;
	BOOL m_bSelection;
	int m_nFlag;

// Operations
public:
	CDocument* pDocument;
	void Copy(COb_schtext* pschtext);

	void Draw(CDC* pdc, CView* pV);
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
	CSize GetExtent(CDC* pDC, CView* pV);
	void ShowTrueTypeString(CDC* pDC, CView* pV);
	
	void DrawSignal(CDC* pdc, CView* pV);
	CSize GetSignalExtent(CDC* pDC, CView* pV);
	void ShowSignalTrueTypeString(COLORREF color, CDC* pDC, CView* pV);
	
	int GetTabbedExtent(CDC* pDC, CView* pV);
	void DrawTabbed(CDC* pdc, CView* pV);
	void ShowTrueTypeTabbedString(COLORREF color, CDC* pDC, CView* pV);
	
	BOOL PickMe(CDC* pDC, CView* pV);
	BOOL Is_On_Me(double fx, double fy, int diagram, CView* pV);

	void Change(POSITION pos);
	void Move(CClientDC* pDC, CView* pV, POSITION pos);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);

	CRect GetMoveTextRect(CDC* pdc, CView* pV);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	void DrawSelection(CDC* pDC, CView* pV);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHTEXT)
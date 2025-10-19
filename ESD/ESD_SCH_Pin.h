#if !defined(EDA_SELF_DEFINATION_COB_SCHPIN)
#define EDA_SELF_DEFINATION_COB_SCHPIN

class  COb_schpin: public CObject
{
	public:
	COb_schpin();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schpin)

// Attributes
public:
	double m_fX, m_fY;
	double m_fPinLength;
	int m_nOrientation;
	int m_nColor;

	CString m_csName;
	BOOL m_bNameVisible;
	
	CString m_csNumber;
	BOOL m_bNumberVisible;
	
	int m_nElectricalType;
	CString m_csDescription;

	BOOL m_bHide;
	CString m_csConnectTo;
	CString m_csUniqueID;

	
	BOOL m_bPinLocked;

	int m_nInside;
	int m_nInsideEdge;
	int m_nOutside;
	int m_nOutsideEdge;

	int m_nOwnerPartID;
	int m_nDisplayMode;	
	POSITION m_posParent;

	int m_nDiagram;
	BOOL m_bSelection;
	int m_nFlag;

// Operations
public:
	CDocument* pDocument;
	void Copy(COb_schpin* pschpin);
	void Draw(CDC* pdc, CView* pV);
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
	void DrawOverwrite(CDC* pdc, CView* pV);
	void Move(CClientDC* pDC, CView* pV, POSITION pos);
	BOOL Is_On_Me(double fx, double fy);
	void Change(POSITION pos);
	void DrawHighlight(CDC* pDC, CView* pV, COLORREF color);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHPIN)
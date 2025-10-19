#if !defined(EDA_SELF_DEFINATION_COB_SCHCOMP)
#define EDA_SELF_DEFINATION_COB_SCHCOMP

class  COb_schcomp: public CObject
{
	public:
	COb_schcomp();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schcomp)

// Attributes
public:
	virtual ~COb_schcomp();


	double m_fX,m_fY;
	int m_nOrientation;
	BOOL m_bMirrored;

	int m_nPartCount;
	int m_nPart;	

	int m_nDisplayModeCount;
	int m_nDisplayMode;		

	BOOL m_bPartLocked;		
	BOOL m_bShowHidePins;	

	BOOL m_bPinsMoveable;

	
	BOOL m_bColorLocked;	
	COLORREF m_nColor;
	COLORREF m_nFillColor;
	COLORREF m_nPinColor;


	CArray <POSITION, POSITION&> m_arrayPos;

    int m_nGraphicalType;		
	BOOL m_bDesignatorLocked;	

	CString m_csDescription;	
	CString m_csLibraryReference;
	CString m_csLibraryFileName;
	CString m_csTargetFileName;	
	CString m_csLibraryPath;	


	CString m_csUniqueID;	

	int m_nFlag;
	BOOL m_bSelection;
	int m_nDiagram;

// Operations
public:
	CDocument* pDocument;	
	void Copy(COb_schcomp* pschcomp);
	void Draw(CDC* pdc, CView* pV);
	struct Struct_Multi_Selection Is_On_Me(double fx, double fy, CDocument* pDocument, CView* pV, int diagram, POSITION position);
	void Change(POSITION pos);
    
	CString GetDesignator(void);
	CString GetComment(void);
	CString GetPackage(void);
	BOOL GetDesignatorVisible(void);
	BOOL GetCommentVisible(void);
	void SetDesignator(CString& cstring, BOOL bVisible);
	void SetComment(CString& cstring, BOOL bVisible);
	void SetPackage(CString& cstring);

	CSize GetCompXYSize();
	RECT GetCompRect();

	BOOL  Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	void DrawSelection(CDC* pDC, CView* pV);
	void Move(CClientDC* pDC, CView* pV, POSITION pos);
	BOOL MovePlaceComp(CClientDC* pDC, CView* pV);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHCOMP)
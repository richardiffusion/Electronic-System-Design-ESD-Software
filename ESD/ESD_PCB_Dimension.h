#if !defined(EDA_SELF_DEFINATION_COB_PCBDIMENSION)
#define EDA_SELF_DEFINATION_COB_PCBDIMENSION

class  COb_pcbdimension: public CObject
{
	public:
	COb_pcbdimension();

protected:
	DECLARE_SERIAL(COb_pcbdimension)

// Attributes
public:
	double m_fLx, m_fLy, m_fHx, m_fHy;  
	double m_fXs, m_fYs, m_fXe, m_fYe;	
	double m_fHeight;
	double m_fLineWidth;
	double m_fRotation;

	CArray<Fpoint, Fpoint&> m_cRefPoint;

	double m_fArrowSize;
	double m_fArrowLineWidth;
	double m_fArrowLength;
	int m_nArrowPosition;

	double m_fExtensionOffset;
	double m_fExtensionLineWidth;
	double m_fExtensionPickGap;

	double m_fTextSx, m_fTextSy;
	double m_fTextHeight;
	double m_fTextLineWidth;
	double m_fTextRotation;
	
	int m_nTextPosition;		
	double m_fTextGap;			
	int m_nTextFormat;			
	int m_nTextDimensionUnit;	
	int m_nTextPrecision;		

	CString m_csPrefix;			
	CString m_csSuffix;			

	int m_nLayer;
	int m_nUnitStyle;
	int m_nDimensionKind;

	BOOL m_bLocked;
	BOOL m_bPrimitiveLock;

	int m_nStrokeFont;
	BOOL m_bUseTTFont;
	BOOL m_bBold;
	BOOL m_bItalic;
	CString m_csTTFontName;
	
	BOOL m_bPolygonOutline;
	BOOL m_bUserRouted;
	int m_nUnionIndex;


	BOOL m_bSelection;	
	int m_nFlag;


// Operations
public:
	CDocument* pDocument;
	void Draw(CDC* pDC, CView* pV, int mode);
	void DrawSelection(CDC* pDC, CView* pV);
	virtual void Serialize(CArchive& ar);
	BOOL Is_On_Me(double fx, double fy, CView* pV);
	void Copy(COb_pcbdimension* pdimension);
	void Change(int index, CView* pV);
	void Change_Linear(int index, CView* pV);
	void Move(CClientDC* pDC, CView* pV,int index);

	void UpdateDimensionText(CView* pV);

};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCBDIMENSION)
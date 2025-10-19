#if !defined(EDA_SELF_DEFINATION_COB_PCBCOORDINATE)
#define EDA_SELF_DEFINATION_COB_PCBCOORDINATE

class  COb_pcbcoordinate: public CObject
{
	public:
	COb_pcbcoordinate();

protected:
	DECLARE_SERIAL(COb_pcbcoordinate)

// Attributes
public:
	double m_fX, m_fY;
	double m_fSize;
	double m_fLineWidth;
	double m_fTextHeight;
	double m_fTextWidth;
	double m_fRotation; 

	int m_nLayer;
	int m_nUnitStyle;

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
	void Change(int index);
	void Copy(COb_pcbcoordinate* pcoordinate);
	void Move(CClientDC* pDC, CView* pV,int index);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCBCOORDINATE)
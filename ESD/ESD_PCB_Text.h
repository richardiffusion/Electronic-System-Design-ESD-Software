#if !defined(EDA_SELF_DEFINATION_COB_PCBTEXT)
#define EDA_SELF_DEFINATION_COB_PCBTEXT

class  COb_pcbtext: public CObject
{
	public:
	COb_pcbtext();

protected:
	DECLARE_SERIAL(COb_pcbtext)

// Attributes
public:
	double m_fSx, m_fSy;
	double m_fHeight;
	double m_fWidth;
	double m_fRotation;

	int m_nLeft, m_nRight, m_nTop, m_nBottom;
	int m_nAscent, m_nDescent, m_nInternalLeading;

	double m_fX1, m_fY1, m_fX2, m_fY2;
	int m_nLayer;

	CString m_csText;
	BOOL m_bMirror;
	BOOL m_bLocked;
	
	int m_nComponent;
	BOOL m_bDesignator;
	BOOL m_bComment;

	int m_nStrokeFont;
	BOOL m_bUseTTFont;
	BOOL m_bBold;
	BOOL m_bItalic;
	BOOL m_bInverted;
	CString m_csTTFontName;
	double m_fInvertedBorder;	

	BOOL m_bPolygonOutline;
	BOOL m_bUserRouted;
	int m_nUnionIndex;

	BOOL m_bSelection;	
	int m_nFlag;

// Operations
public:
	CDocument* pDocument;
	void Copy(COb_pcbtext* ppcbtext);
	void Change(int index, CView* pV);
	void Draw(CDC* pdc, CView* pV);
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
	void DrawSelection(CDC* pDC, CView* pV);
	void DrawNotEditSelection(CDC* pDC, CView* pV);
	void ShowTTString(COLORREF color, CDC* pDC, CView* pV);
	void ShowMirrorTTString(COLORREF color, CDC* pDC, CView* pV);
	void ShowStrokeString(COLORREF color, CDC* pDC, CView* pV);

	
	BOOL PickMe(CDC* pDC, CView* pV);
	BOOL Is_On_Me(double fx, double fy, CView* pV);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);

	void GetTextCorner(double *fx11, double *fy11, double *fx22, double *fy22, double *fx33, double *fy33, double *fx44, double *fy44);
	void GetRotationPoint(double *fxr, double *fyr, double fx, double fy);
	void GetMirrorRotationPoint(double *fxr, double *fyr, double fx, double fy);
	void GetTextBitmapMargin(CDC* pDC, char* str, LOGFONT* logfont);
	void GetGDIPlusTextBitmapMargin(CDC* pDC, WCHAR* str, LOGFONT* logfont,int cx, int cy);

	double GetStringWidth(CView* pV);
	void GetStrokeStringRect(CView* pV, double *dx, double *dy1, double *dy2);

	void Move(CClientDC* pDC, CView* pV, int index);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);
	void UpdateStringBorder(CView* pV, BOOL refreshbitmap);
	void DrawStringRect(CDC* pDC, CView* pV);
	void GetRotatedTextBorder(double angle);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCBTEXT)
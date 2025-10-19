#if !defined(EDA_SELF_DEFINATION_COB_SCHSHEET)
#define EDA_SELF_DEFINATION_COB_SCHSHEET


class  COb_schsheet: public CObject
{
	public:
	COb_schsheet();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schsheet)

// Attributes
public:
	int m_nStandardStyle;
	BOOL m_bCustom;
	double m_fCustomWidth, m_fCustomHeight, m_fCustomMargin;
	int m_nCustomXRegionCount, m_nCustomYRegionCount;
	int m_nOrientation;
	COLORREF m_nBorderColor;
	COLORREF m_nSheetColor;
	BOOL m_bTitleBlock;
	int m_nTitleBlockStyle;
	int m_nSheetNumberSpace;
	BOOL m_bShowReferenceZones;
	BOOL m_bShowBorder;
	BOOL m_bShowTemplateGraphics;
	int m_nFont;

	int m_nDiagram;
	//Title lines&strings
	
	// Operations
public:
	CDocument* pDocument;
	void Draw(CDC* pdc, CView* pV);
	void Change(void);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHSHEET)
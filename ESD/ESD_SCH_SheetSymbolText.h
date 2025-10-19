#if !defined(EDA_SELF_DEFINATION_COB_SCHSHEETSYMBOLTEXT)
#define EDA_SELF_DEFINATION_COB_SCHSHEETSYMBOLTEXT

class  COb_schsheetsymboltext: public CObject
{
	public:
	COb_schsheetsymboltext();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schsheetsymboltext)

// Attributes
public:
	float m_fSx, m_fSy;
	CString m_csText;
	int m_nOrientation;

	int m_nFont;
	COLORREF m_nColor;
	int m_nID; //Text = 4;  NetLabel = 25;  Sheet Symbol Designator = 32;   Sheet Symbol File Name = 33
	
	//用于特殊字符串
	BOOL m_bHidden;    //=TRUE 不显示  =FALSE 显示
	BOOL m_bNotAutoPosition;
	int m_nAnChorH;
	int m_nAnChorV;

	int m_nDiagram;
	int m_nSelection;
	int m_nFlag;

	POSITION m_posParent;
// Operations
public:

	void Draw(CDC* pdc, CView* pV);
	//void DrawOverwrite(CDC* pdc);
	int GetExtent(CDC* pDC, CView* pV);
	void ShowTrueTypeString(COLORREF color, CDC* pDC, CView* pV);
	//BOOL OutOfBorder(CDC* pDC);
	BOOL PickMe(CDC* pDC, CView* pV);
	//BOOL Is_On_Text(int x, int y);
public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHSHEETSYMBOLTEXT)
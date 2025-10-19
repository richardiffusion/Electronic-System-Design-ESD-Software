
#if !defined(EDA_SELF_DEFINATION_COB_SCHIEEE)
#define EDA_SELF_DEFINATION_COB_SCHIEEE

class  COb_schieee: public CObject
{
	public:
	COb_schieee();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_schieee)

// Attributes
public:
	double m_fX, m_fY;
	double m_fSize;
	int m_nOrientation;	
	int m_nOwnerPartID;
	int m_nDisplayMode;

	int m_nWidth;
	BOOL m_bMirrored;
	int m_nIeeeSymbol;
	
	COLORREF m_nColor;

	int m_nDiagram;
	BOOL m_bSelection;
	int m_nFlag;

	POSITION m_posParent;

// Operations
public:
	CDocument* pDocument;
	void Copy(COb_schieee* pschieee);
	void Draw(CDC* pdc, CView* pV);
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
	void Change(void);
	void Get_Rotate_XY(double* fx, double* fy, double old_fx, double old_fy, int orientation, BOOL mirrored);
	void Get_Rotate_Angle(double* sangle, double* eangle, double old_sangle, double old_eangle, int orientation, BOOL mirrored);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHIEEE)
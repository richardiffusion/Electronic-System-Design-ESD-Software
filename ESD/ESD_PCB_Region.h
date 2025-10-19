#if !defined(EDA_SELF_DEFINATION_COB_PCBREGION)
#define EDA_SELF_DEFINATION_COB_PCBREGION

#include "struct.h"

class  COb_pcbregion: public CObject
{
	public:
	COb_pcbregion();
	virtual ~COb_pcbregion();

protected:
	DECLARE_SERIAL(COb_pcbregion)

// Attributes
public:
	CArray<Struct_RegionVertex, Struct_RegionVertex&> m_cVertex;
	
	CTypedPtrArray<CObArray, CArray<Fpoint, Fpoint>*> m_arrayHoles;

	int m_nLayer;
	int m_nNet;
	int m_nComponent;
	int m_nPolygon;

	BOOL m_bLocked;
	BOOL m_bKeepout;
	BOOL m_bPolygonCutout;
	BOOL m_bBoardCutout;

	BOOL m_bPolygonOutline;
	BOOL m_bUserRouted;
	int m_nUnionIndex;
	int m_nSubPolyIndex;

	BOOL m_bSelection;	
	int m_nFlag;

	// Operations
public:
	double area;
	CDocument* pDocument;
	void Draw(CDC* pDC, CView* pV);
	void DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy);
	void DrawSelection(CDC* pDC, CView* pV);
	BOOL PickMe(CView* pV);
	void Copy(COb_pcbregion* pregion);
	BOOL Is_On_Me(double fx, double fy);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	void Change(int index);
	RECT GetRegionRect();

	void GetRegionVertex(CArray<Fpoint, Fpoint&>& arrayVertex);
	void FillRegion(CDC* pDC, CView* pV, COLORREF color);
	void FrameRegion(CDC* pDC, CView* pV, int width, COLORREF color);
	void DottedFrameRegion(CDC* pDC, CView* pV, COLORREF color);
	void Move(CClientDC* pDC, CView* pV, int index);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);
	void MoveAndRotate(CView* pV, double x0, double y0,  double deltax, double deltay, int rotation, BOOL mirror, int index);

	void DrawHighlighted(CDC* pDC, CView* pV, int layer);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCBREGION)


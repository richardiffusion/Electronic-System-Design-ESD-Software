#if !defined(EDA_SELF_DEFINATION_COB_PCBPOLYGON)
#define EDA_SELF_DEFINATION_COB_PCBPOLYGON

#include "struct.h"

class  COb_pcbpolygon: public CObject
{
	public:
	COb_pcbpolygon();

protected:
	DECLARE_SERIAL(COb_pcbpolygon)

// Attributes
public:

	int m_nNet;	
	int m_nLayer;

	BOOL m_bLocked;
	int m_nPolygonType;  
	BOOL m_bPrimitiveLocked;
	BOOL m_bPourOver;
	BOOL m_bRemoveDead;

	double m_fGridSize;
	double m_fTrackWidth;
	int m_nHatchStyle;	
	BOOL m_bUseOctagons;
	double m_fMinPrimLength;
	
	CArray<Struct_RegionVertex, Struct_RegionVertex&> m_cVertex;

	BOOL m_bShelved;
	int m_nRestoreLayer;
	BOOL m_bRemoveIslandsByArea;
	BOOL m_bRemoveNecks;
	double m_fAreaThreshold;
	double m_fArcResolution;
	double m_fNeckWidthThreshold;
	int m_nPourOverStyle;	//Don't Pour Over Same Net Objects
							//Pour Over All Same Net Objects
							//Pour Over Same Net Polygons Only
	CString m_csName;
	int m_nPourIndex;
	BOOL m_bIgnoreViolations;

	BOOL m_bPolygonOutline;
	BOOL m_bUserRouted;
	int m_nUnionIndex;
	int m_nSubPolyIndex;

	double m_fPourGap;	

	BOOL m_bSelection;	
	int m_nFlag;

	// Operations
public:
	CDocument* pDocument;
	void Copy(COb_pcbpolygon* ppolygon);
	BOOL Is_On_Me(double fx, double fy);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	void ChangePolygon(CView* pV, int index);
	void ChangeSplitPlane(int index);
	RECT GetPolygonRect();
	void Move(CClientDC* pDC, CView* pV, int index);
	BOOL MovePlaced(CClientDC* pDC, CView* pV);
	BOOL ShowPolygonOutline(int index);
	void Draw(CDC* pDC, CView* pV);
	void DrawSelection(CDC* pDC, CView* pV);
	void GetPolygonVertex(CArray<Fpoint, Fpoint&>& arrayVertex);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCBPOLYGON)


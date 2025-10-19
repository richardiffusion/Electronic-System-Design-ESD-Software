#if !defined(EDA_SELF_DEFINATION_SCHUNDO)
#define EDA_SELF_DEFINATION_SCHUNDO

class  CSchUndo: public CObject
{
	public:
	CSchUndo();
	~CSchUndo();

// Attributes
public:

	int m_nOp; //DELETE, MOVE, EDIT ...
	int m_nOb; //LINE, JUNC, ....
	int m_nDiagram;
	POSITION m_nPos;
	
	COb_schcomp			schcomp;
	COb_schpin			schpin;
	COb_schieee			schieee;
	COb_schtext			schtext;
	COb_schbezier		schbezier;
	COb_schpolyline		schpolyline;
	COb_schpolygon		schpolygon;
	COb_schellipsefill	schellipsefill;
	COb_schpie			schpie;
	COb_schroundrect	schroundrect;
	COb_schellipse		schellipse;
	COb_scharc			scharc;
	COb_schline			schline;
	COb_schrect			schrect;
	COb_schsheetsymbol	schsheetsymbol;
	COb_schsheetentry	schsheetentry;
	COb_schpower		schpower;
	COb_schport			schport;
	COb_schtextframe	schtextframe;
	COb_schjunc			schjunc;
	COb_schgraphic		schgraphic;
	
	double m_fX1;
	double m_fY1;
	double m_fX2;
	double m_fY2;
	int m_nOrientation;
	double m_fRx;
	double m_fRy;

	CArray<Fpoint, Fpoint&> m_cVertex;
	
	BOOL changeviewpoint;

	// Operations
public:
	void Undo(void);
	void Redo(void);
public:

};

#endif // !defined(EDA_SELF_DEFINATION_SCHUNDO)
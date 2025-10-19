#if !defined(EDA_SELF_DEFINATION_COB_PCBSHEET)
#define EDA_SELF_DEFINATION_COB_PCBSHEET


class  COb_pcbsheet: public CObject
{
	public:
	COb_pcbsheet();
	virtual ~COb_pcbsheet();

protected:
	DECLARE_SERIAL(COb_pcbsheet)

// Attributes
public:

	double m_fOriginX, m_fOriginY;

	double m_fBigVisibleGridSize;
	double m_fVisibleGridSize;
	double m_fElecGridRange;
	BOOL  m_bElecGridEnable;
	BOOL  m_bAllLayersElecGrid;
	double m_fSnapGridSize;
	double m_fSnapGridSizeX;
	double m_fSnapGridSizeY;
	double m_fTrackGridSize;
	double m_fViaGridSize;
	double m_fComponentGridSize;
	double m_fComponentGridSizeX;
	double m_fComponentGridSizeY;
	

	BOOL m_bDotGrid;
	

	int m_nUnit;


	int m_nDesignatorDisplayMode;


	BOOL m_bPrimitiveLock;

	CArray<Struct_RegionVertex, Struct_RegionVertex&> m_cVertex;

	double m_fSheetX, m_fSheetY, m_fSheetWidth, m_fSheetHeight;
	BOOL m_bShowSheet;
	BOOL m_bLockSheet;

	CTypedPtrArray <CObArray, COb_pcbtrack*> m_arraySheetTrack;
	CTypedPtrArray <CObArray, COb_pcbtext*> m_arraySheetText;	
	
	// Operations
public:
	virtual void Serialize(CArchive& ar);
	void Draw(CDC* pdc, CView* pV);
	void GetBoardRect(double* fx1, double* fy1, double* fx2, double* fy2);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCBSHEET)
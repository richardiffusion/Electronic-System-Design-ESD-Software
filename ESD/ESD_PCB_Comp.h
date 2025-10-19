#if !defined(EDA_SELF_DEFINATION_COB_PCBCOMP)
#define EDA_SELF_DEFINATION_COB_PCBCOMP

class  COb_pcbcomp: public CObject
{
	public:
	COb_pcbcomp();

protected:
	DECLARE_SERIAL(COb_pcbcomp)

// Attributes
public:
	virtual ~COb_pcbcomp();

	double m_fX,m_fY;
	double m_fRotation;
	double m_fHeight;

	int m_nLayer; //TOP/BOTTOM

	BOOL m_bNameOn;
	int m_nNamePosition;
	BOOL m_bCommentOn;
	int m_nCommentPosition;

	BOOL m_bPrimitiveLock;
	BOOL m_bLocked;
	BOOL m_bLockStrings;

	//swapping options
	BOOL m_bEnablePinSwapping;
	BOOL m_bEnablePartSwapping;

	CString m_csPattern;
	CString m_csSourceFootprintLibrary;
	CString m_csFootprintDescription;

	//int m_nIndexForSave;	
	BOOL m_bPolygonOutline;
	BOOL m_bUserRouted;
	int m_nUnionIndex;

	int m_nGroupNum;
	int m_nCount;
	int m_nComponentKind;
	int m_nComponentKindVersion2;
	int m_nChannelOffset;
	CString m_csSourceDesignator;
	CString m_csSourceUniqueID;
	CString m_csSourceHierarchicalPath;
	CString m_csSourceComponentLibrary;
	CString m_csSourceLibReference;
	CString m_csSourceDescription;
	CString m_csDefaultPcb3dModel;

	//CArray <POSITION, POSITION&> m_arrayPos;	//保存属于元件的图形和文字信息

	int m_nFlag;
	int m_bSelection;

// Operations
public:
	CDocument* pDocument;
	void Draw(CDC* pdc, CView* pV);
	void DrawSelection(CDC* pDC, CView* pV, int index);
	BOOL Is_On_Me(double fx, double fy, int index);
	BOOL Is_Inside_Area(double fx1, double fy1, double fx2, double fy2);
	BOOL IsVisible(int index);
	void GetPcbCompRect(double *fx1, double *fy1, double *fx2, double *fy2, double *fx3, double *fy3, double *fx4, double *fy4, int index);
	CString GetPcbCompDesignator(int index);
	CString GetPcbCompComment(int index);
	void Change(int index, CView* pV);
	void Copy(COb_pcbcomp* pcomp);
	void Move(CClientDC* pDC, CView* pV, int index);
	BOOL MovePlacePackage(CClientDC* pDC, CView* pV, int index);

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCBCOMP)
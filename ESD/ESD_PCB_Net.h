#if !defined(EDA_SELF_DEFINATION_COB_PCB_NET)
#define EDA_SELF_DEFINATION_COB_PCB_NET

class  COb_pcbnet: public CObject
{
	public:
	COb_pcbnet();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_pcbnet)

// Attributes
public:

	CString m_csNetName;
	BOOL m_bVisible;
	BOOL m_bLoopRemove;
	COLORREF m_nColor;
	
	double m_fTrackWidth[SIGNAL_LAYER_NUM];
	double m_fViaDia;
	double m_fViaHoleDia;
	
	BOOL m_bSelection;	
	int m_nFlag;

// Operations
public:


public:
	CDocument* pDocument;
	void Copy(COb_pcbnet* pnet);
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCB_NET)


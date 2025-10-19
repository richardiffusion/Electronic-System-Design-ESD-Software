#if !defined(EDA_SELF_DEFINATION_COB_PCB_RULES)
#define EDA_SELF_DEFINATION_COB_PCB_RULES

class  COb_pcbrules: public CObject
{
	public:
	COb_pcbrules();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_pcbrules)

// Attributes
public:

	int m_nKind;
	int m_nNetScope;
	int m_nLayerKind;

	BOOL m_bEnagle;
	CString m_csName;
	CString m_csComment;


	
	BOOL m_bLocked;
	
	BOOL m_bPolygonOutline;
	BOOL m_bUserRouted;
	int m_nUnionIndex;

	BOOL m_bSelection;
	int m_nFlag;

// Operations
public:


public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCB_RULES)


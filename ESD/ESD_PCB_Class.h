#if !defined(EDA_SELF_DEFINATION_COB_PCB_CLASS)
#define EDA_SELF_DEFINATION_COB_PCB_CLASS

class  COb_pcbclass: public CObject
{
	public:
	COb_pcbclass();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_pcbclass)

// Attributes
public:

	CString m_csName;
	int m_nLayer;
	int m_nKind;
	BOOL m_bSuperClass;
	CStringArray m_arrayMember;
	
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

#endif // !defined(EDA_SELF_DEFINATION_COB_PCB_NET)


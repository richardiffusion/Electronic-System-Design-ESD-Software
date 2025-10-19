#if !defined(EDA_SELF_DEFINATION_COB_SCH_NET)
#define EDA_SELF_DEFINATION_COB_SCH_NET

class  COb_schnet: public CObject
{
	public:
	COb_schnet();

protected:
	DECLARE_SERIAL(COb_schnet)

// Attributes
public:
	CString m_csNetName;
	CStringArray m_arrayCompDesignator;
	CStringArray m_arrayPinNumber;

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCH_NET)


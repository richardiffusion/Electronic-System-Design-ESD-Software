#if !defined(EDA_SELF_DEFINATION_COB_libcompheadHEAD)
#define EDA_SELF_DEFINATION_COB_libcompheadHEAD

class  COb_libcomphead: public CObject
{
	public:
	COb_libcomphead();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_libcomphead)

// Attributes
public:
	virtual ~COb_libcomphead();

	int m_nPartCount;
	int m_nSharedCompNum;

	CString m_csDescription;
	CStringArray m_arrayPackage;
	CString m_csRef;
	CStringArray m_arrayCompName;

public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_libcompheadHEAD)
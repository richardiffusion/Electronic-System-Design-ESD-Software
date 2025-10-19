#if !defined(EDA_SELF_DEFINATION_COB_SCHLIBRARY)
#define EDA_SELF_DEFINATION_COB_SCHLIBRARY

class  CLibrarySch: public CObject
{
	public:
	CLibrarySch();

protected:
	DECLARE_SERIAL(CLibrarySch)

// Attributes
public:

	char compname[64];
	char description[128];
	int length;
	int offset;

// Operations
public:
	CDocument* pDocument;


public:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_SCHLIBRARY)
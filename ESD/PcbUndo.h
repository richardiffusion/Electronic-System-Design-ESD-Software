#if !defined(EDA_SELF_DEFINATION_PCBUNDO)
#define EDA_SELF_DEFINATION_PCBUNDO

class  CPcbUndo: public CObject
{
	public:
	CPcbUndo();
	~CPcbUndo();

// Attributes
public:

	int m_nOp; //DELETE, MOVE, EDIT ...
	int m_nOb; //LINE, JUNC, ....
	//int pcbelement;
	int m_nIndex;
	
	COb_pcbcomp			pcbcomp;
	COb_pcbtrack		pcbtrack;
	COb_pcbarc			pcbarc;
	COb_pcbtext			pcbtext;
	COb_pcbvia			pcbvia;
	COb_pcbpad			pcbpad;
	COb_pcbfill			pcbfill;
	COb_pcbregion		pcbregion;
	COb_pcbpolygon		pcbpolygon;
	COb_pcbcoordinate	pcbcoordinate;
	COb_pcbdimension	pcbdimension;
	COb_pcbnet			pcbnet;
	

	BOOL changeviewpoint;

	// Operations
public:
	void Undo(void);
	void Redo(void);
public:

};

#endif // !defined(EDA_SELF_DEFINATION_PCBUNDO)
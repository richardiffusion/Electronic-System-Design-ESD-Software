#if !defined(EDA_SELF_DEFINATION_COB_PCB_CONNECTION)
#define EDA_SELF_DEFINATION_COB_PCB_CONNECTION

class  COb_pcbconnection: public CObject
{
	public:
	COb_pcbconnection();

protected:
	//CStroke();
	DECLARE_SERIAL(COb_pcbconnection)

// Attributes
public:

	double m_fX1, m_fY1, m_fX2, m_fY2;
	int m_nLayer1, m_nLayer2;
	int m_nNet;
	
	int m_nLayer;
	BOOL m_bLocked;
	BOOL m_bPolygonOutline;
	BOOL m_bUserRouted;
	int m_nUnionIndex;

	BOOL m_bSelection;
	int m_nFlag;

// Operations
public:

	void Draw(CDC* pdc, CView* pV);

public:
	virtual void Serialize(CArchive& ar);
	void Copy(COb_pcbconnection* pconnection);
};

#endif // !defined(EDA_SELF_DEFINATION_COB_PCB_NET)


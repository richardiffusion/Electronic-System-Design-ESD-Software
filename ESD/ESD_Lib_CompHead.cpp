#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"


IMPLEMENT_SERIAL(COb_libcomphead, CObject, 0)
COb_libcomphead::COb_libcomphead()
{
	m_nPartCount = 1;
	m_nSharedCompNum = 1;

	m_csDescription.Empty();
	m_arrayPackage.RemoveAll();
	m_csRef.Empty();
	m_arrayCompName.RemoveAll();
}

COb_libcomphead::~COb_libcomphead()
{

}

void COb_libcomphead::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nPartCount;
		ar << m_nSharedCompNum;
		
		int namenum = m_arrayCompName.GetCount();
		ar << namenum;
		for(int i=0; i<namenum; i++)
		{
			ar << m_arrayCompName.GetAt(i);
		}
		ar << m_csDescription;
		ar << m_csRef;

		int packagenum = m_arrayPackage.GetCount();
		ar << packagenum;
		for(int i=0; i<packagenum; i++)
		{
			ar << m_arrayPackage.GetAt(i);
		}	

	}
	else
	{
		ar >> m_nPartCount;
		ar >> m_nSharedCompNum;
		
		int namenum;
		ar >> namenum;
		m_arrayCompName.RemoveAll();
		for(int i=0; i<namenum; i++)
		{
			CString cstring;
			ar >> cstring;
			m_arrayCompName.Add(cstring);
		}

		ar >> m_csDescription;
		ar >> m_csRef;

		int packagenum;
		ar >> packagenum;
		m_arrayPackage.RemoveAll();
		for(int i=0; i<packagenum; i++)
		{
			CString cstring;
			ar >> cstring;
			m_arrayPackage.Add(cstring);
		}	
	}
}


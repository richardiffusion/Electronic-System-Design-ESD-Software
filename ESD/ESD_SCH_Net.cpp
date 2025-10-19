#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"


IMPLEMENT_SERIAL(COb_schnet, CObject, 0)
COb_schnet::COb_schnet()
{
	m_csNetName.Empty();
	m_arrayCompDesignator.RemoveAll();
	m_arrayPinNumber.RemoveAll();
}

void COb_schnet::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_csNetName;

		int n = m_arrayCompDesignator.GetCount();
		ar << n;
		for(int i=0; i<n; i++)
		{
			CString cstring = m_arrayCompDesignator.GetAt(i);
			ar << cstring;
		}
		for(int i=0; i<n; i++)
		{
			CString cstring = m_arrayPinNumber.GetAt(i);
			ar << cstring;
		}
	}
	else
	{
		ar >> m_csNetName;

		int n;
		ar >> n;
		for(int i=0; i<n; i++)
		{
			CString cstring;
			ar >> cstring;
			m_arrayCompDesignator.Add(cstring);
		}
		for(int i=0; i<n; i++)
		{
			CString cstring;
			ar >> cstring;
			m_arrayPinNumber.Add(cstring);
		}
	}
}



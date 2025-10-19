#include "stdafx.h"

#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditArc.h"

IMPLEMENT_SERIAL(CLibrarySch, CObject, 0)
CLibrarySch::CLibrarySch()
{
	// This empty constructor should be used by serialization only

}



void CLibrarySch::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}


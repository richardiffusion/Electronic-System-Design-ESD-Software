#if !defined(AFX_DRAWBUTTON_H__A921B002_64D3_11D2_AA46_444553540000__INCLUDED_PINDEMO)
#define AFX_DRAWBUTTON_H__A921B002_64D3_11D2_AA46_444553540000__INCLUDED_PINDEMO

#include "struct.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DrawButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatic_PinDemo window

class CStatic_PinDemo : public CStatic
{
// Construction
public:
	CStatic_PinDemo();

// Attributes
public:
	COb_schpin* m_pPin;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatic_PinDemo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStatic_PinDemo();
	COLORREF m_nColor;
	// Generated message map functions
protected:
	//{{AFX_MSG(CStatic_PinDemo)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWBUTTON_H__A921B002_64D3_11D2_AA46_444553540000__INCLUDED_PINDEMO)

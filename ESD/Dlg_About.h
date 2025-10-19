// AboutDlg.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2007 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(__ABOUTDLG_H__)
#define __ABOUTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (_MSC_VER > 1310) // VS2005
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlg_About dialog
#include "Resource.h"

class CDlg_About : public CDialog
{
// Construction
public:
	CDlg_About(CWnd* pParent = NULL)
		: CDialog(IDD_ABOUTBOX, pParent)
	{
		NONCLIENTMETRICS ncm;
		::ZeroMemory(&ncm, sizeof(NONCLIENTMETRICS));
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		
		VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
			sizeof(NONCLIENTMETRICS), &ncm, 0));

		ncm.lfMenuFont.lfWeight = FW_BLACK; //FW_BOLD;
		ncm.lfMenuFont.lfHeight = 50;
		ncm.lfMenuFont.lfUnderline = FALSE;
		m_fontBold.CreateFontIndirect(&ncm.lfMenuFont);
	}

// Dialog Data
	//{{AFX_DATA(CDlg_About)
	//enum { IDD = IDD_ABOUTBOX };

	CString	m_szSerial;
	CString	m_szUnit;
	CString	m_szUser;
	CXTButton m_btnOk;
	CXTHyperLink m_txtURL;
	//CXTHyperLink m_txtURL2;
	CXTHyperLink m_txtEmail;
	//CXTHyperLink m_txtEmail2;
	//}}AFX_DATA

	CFont m_fontBold;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_About)
	protected:

	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(CDlg_About)
		DDX_Control(pDX, IDOK, m_btnOk);
		DDX_Control(pDX, IDC_TXT_URL, m_txtURL);
		//DDX_Control(pDX, IDC_TXT_URL2, m_txtURL2);
		DDX_Control(pDX, IDC_TXT_EMAIL, m_txtEmail);
		//DDX_Control(pDX, IDC_TXT_EMAIL2, m_txtEmail2);
		DDX_Text(pDX, IDC_SN, m_szSerial);
		DDX_Text(pDX, IDC_UNIT, m_szUnit);
		DDX_Text(pDX, IDC_USER, m_szUser);
		//}}AFX_DATA_MAP
	}

	virtual BOOL OnInitDialog()
	{
		CDialog::OnInitDialog();

		// get a pointer to CWinApp.
		CWinApp* pApp = AfxGetApp( );
		ASSERT( pApp != NULL );




		// set the title text to bold font.
//		m_txtPackageVersion.SetFont( &m_fontBold );


		// define the url for our hyperlinks.
		m_txtURL.SetURL( _T( "https://github.com/richardiffusion" ) );
		//m_txtURL.SetUnderline( false );
		
		//m_txtURL2.SetURL( _T( "https://github.com/richardiffusion" ) );
		//m_txtURL2.SetUnderline( false );
		
		m_txtEmail.SetURL( _T( "mailto:cashismore@gmail.com" ) );
		//m_txtEmail.SetUnderline( false );
		
		//m_txtEmail2.SetURL( _T( "mailto:cashismore@gmail.com" ) );
		//m_txtEmail2.SetUnderline( false );

		// set OK button style.
		m_btnOk.SetXButtonStyle( BS_XT_SEMIFLAT | BS_XT_HILITEPRESSED | BS_XT_WINXP_COMPAT );

		return TRUE;  // return TRUE unless you set the focus to a control
		              // EXCEPTION: OCX Property Pages should return FALSE
	}
};

#ifdef ENABLE_MANIFESTEDITOR


#if !defined(_UNICODE) && (_MSC_VER < 1400)

#if _MSC_VER > 1200 //MFC 7.0
#include <..\src\mfc\afximpl.h> // MFC Global data
#else
#include <..\src\afximpl.h>     // MFC Global data
#endif

#endif

AFX_INLINE void EnableManifestEditor()
{
#if !defined(_UNICODE) && (_MSC_VER < 1400)
	OSVERSIONINFO ovi = {sizeof(OSVERSIONINFO)};
	::GetVersionEx(&ovi);

	if ((ovi.dwPlatformId >= VER_PLATFORM_WIN32_NT) && (ovi.dwMajorVersion >= 5))
	{
#if (_MSC_VER >= 1200)
		afxData.bWin95 = TRUE;
#else
		afxData.bWin32s = TRUE;
#endif
	}
#endif
}

#else

AFX_INLINE void EnableManifestEditor()
{
}

#endif


AFX_INLINE void ShowSampleHelpPopup(CWnd* pParentWnd, UINT nIDResource)
{
	CXTPPopupControl* pPopup = new CXTPPopupControl();
	pPopup->SetTransparency(200);
	pPopup->SetTheme(xtpPopupThemeOffice2003);
	pPopup->AllowMove(TRUE);
	pPopup->SetAnimateDelay(500);
	pPopup->SetPopupAnimation();
	pPopup->SetShowDelay(5000);
	pPopup->SetAutoDelete(TRUE);
	
	CXTPPopupItem* pItemText = (CXTPPopupItem*)pPopup->AddItem(new CXTPPopupItem(CRect(8, 12, 500, 130)));
	pItemText->SetRTFText(nIDResource);
	pItemText->FitToContent();
	CSize sz(pItemText->GetRect().Size());

	// close icon.
	CXTPPopupItem* pItemIcon = (CXTPPopupItem*)pPopup->AddItem(
		new CXTPPopupItem(CRect(sz.cx + 2, 10, sz.cx + 2 + 16, 10 + 16)));
	
	pPopup->GetImageManager()->SetIcon(IDI_POPUP_CLOSE, IDI_POPUP_CLOSE);
	pItemIcon->SetIconIndex(IDI_POPUP_CLOSE);

	pItemIcon->SetButton(TRUE);
	pItemIcon->SetID(XTP_ID_POPUP_CLOSE);

	pPopup->SetPopupSize(CSize(sz.cx + 20, sz.cy + 20));
	pPopup->Show(pParentWnd);
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ABOUTDLG_H__)

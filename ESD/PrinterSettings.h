// PrinterSettings.h: Interface of CPrinterSettings.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTIT_H__2B9969C1_0872_11D3_9A02_0000B45414CC__INCLUDED_)
#define AFX_PRINTIT_H__2B9969C1_0872_11D3_9A02_0000B45414CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winspool.h>	// for: ClosePrinter,GetPrinter,PRINTER_INFO_2


#define PRINTERNAME_UNDEFINED			"printer not set..."
#define PRINTERSETTINGS_DIRECTORYNAME	"printer settings"

class CPrinterSettings : public CObject  
{
public:
	CPrinterSettings();
	CPrinterSettings(const CString dirname );
	virtual ~CPrinterSettings();
// for use e.g. in CArray
public:
	void operator=(const CPrinterSettings* src);
	void operator=(const CPrinterSettings& src);

// general interface
public:
	// get the settings for the default MFC Printer
	void CopyDefaultMfcPrinter(void);
	// Set the default MFC Printer to this
	void SetThisPrinter(void);
	// Set the default MFC Printer by Name
	BOOL SetPrinterDevice(LPCTSTR pszDeviceName);
	// Show the common dialog and get the settings
	CString PrinterSetup( CWnd* pWnd);
	// restore previous printer settings (if safed )
	void RestorePrinter(void);
	CString GetFileName( void ) const
		{ return m_strfilename; }
	CString SetDirName( const CString neu) 
		{ CString temp = m_strdirname; m_strdirname = neu; return temp; }
	CString GetDirName( void ) const
		{ return m_strdirname; }
	// save all settings to disk
	//int Save(LPCTSTR strFilename);
	// load all settings
	//int Load(LPCTSTR strFilename);
	// check to see if our printer is still available
	BOOL IsPrinterAvailable(LPCTSTR pszDeviceName);
	// retrieve the human readable printername :)
	CString GetPrinterName(void) const
		{ return m_strPrinterName; }
public:	// but not useless:  :o)
	CString DevmodePrinterName(void);
	HANDLE CopyHandle(HANDLE h); 
	CString SetFileName( const CString neu) 
		{ CString temp = m_strfilename; m_strfilename = neu; return temp; }
	HANDLE m_hDevMode;
	HANDLE m_hDevNames;
	HANDLE m_hSaveDevMode;
	HANDLE m_hSaveDevNames;
	CString m_strPrinterName;
	CString m_strfilename;
	CString m_strdirname;
};

#endif // !defined(AFX_PRINTIT_H__2B9969C1_0872_11D3_9A02_0000B45414CC__INCLUDED_)

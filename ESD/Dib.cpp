/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Jorge Lodos
// All rights reserved
//
// Distribute and use freely, except:
// 1. Don't alter or remove this notice.
// 2. Mark the changes you made
//
// Send bug reports, bug fixes, enhancements, requests, etc. to:
//    lodos@cigb.edu.cu
/////////////////////////////////////////////////////////////////////////////

//  dib.cpp
//

#include "stdafx.h"
#include "dib.h"
#include <windowsx.h>
#include <afxadv.h>
#include <io.h>
#include <errno.h>

/////////////////////////////////////////////////////////////////////////////
// CDib

IMPLEMENT_DYNAMIC(CDib, CObject)

CDib::CDib()
{
	m_pBMI = NULL;
	m_pBits = NULL;
	m_pPalette = NULL;
}

CDib::~CDib()
{
	Free();
}

void CDib::Free()
{
	// Make sure all member data that might have been allocated is freed.
	if (m_pBits)
	{
		GlobalFreePtr(m_pBits);
		m_pBits = NULL;
	}
	if (m_pBMI)
	{
		GlobalFreePtr(m_pBMI);
		m_pBMI = NULL;
	}
	if (m_pPalette)
	{
		m_pPalette->DeleteObject();
		delete m_pPalette;
		m_pPalette = NULL;
	}
}

/*************************************************************************
 *
 * Paint()
 *
 * Parameters:
 *
 * HDC hDC          - DC to do output to
 *
 * LPRECT lpDCRect  - rectangle on DC to do output to
 *
 * LPRECT lpDIBRect - rectangle of DIB to output into lpDCRect
 *
 * CPalette* pPal   - pointer to CPalette containing DIB's palette
 *
 * Return Value:
 *
 * BOOL             - TRUE if DIB was drawn, FALSE otherwise
 *
 * Description:
 *   Painting routine for a DIB.  Calls StretchDIBits() or
 *   SetDIBitsToDevice() to paint the DIB.  The DIB is
 *   output to the specified DC, at the coordinates given
 *   in lpDCRect.  The area of the DIB to be output is
 *   given by lpDIBRect.
 *
 ************************************************************************/

BOOL CDib::Paint(HDC hDC, LPRECT lpDCRect, LPRECT lpDIBRect) const
{
	if (!m_pBMI)
		return FALSE;

	HPALETTE hPal = NULL;           // Our DIB's palette
	HPALETTE hOldPal = NULL;        // Previous palette

	// Get the DIB's palette, then select it into DC
	if (m_pPalette != NULL)
	{
		hPal = (HPALETTE) m_pPalette->m_hObject;

		// Select as background since we have
		// already realized in forground if needed
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}

	
	/* Make sure to use the stretching mode best for color pictures */
	::SetStretchBltMode(hDC, COLORONCOLOR);

	/* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
	BOOL bSuccess;
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
		bSuccess = ::SetDIBitsToDevice(hDC,        // hDC
								   lpDCRect->left,             // DestX
								   lpDCRect->top,              // DestY
								   RECTWIDTH(lpDCRect),        // nDestWidth
								   RECTHEIGHT(lpDCRect),       // nDestHeight
								   lpDIBRect->left,            // SrcX
								   (int)Height() -
									  lpDIBRect->top -
									  RECTHEIGHT(lpDIBRect),     // SrcY
								   0,                          // nStartScan
								   (WORD)Height(),             // nNumScans
								   m_pBits,                    // lpBits
								   m_pBMI,                     // lpBitsInfo
								   DIB_RGB_COLORS);            // wUsage
   else
	  bSuccess = ::StretchDIBits(hDC,            // hDC
							   lpDCRect->left,               // DestX
							   lpDCRect->top,                // DestY
							   RECTWIDTH(lpDCRect),          // nDestWidth
							   RECTHEIGHT(lpDCRect),         // nDestHeight
							   lpDIBRect->left,              // SrcX
							   lpDIBRect->top,               // SrcY
							   RECTWIDTH(lpDIBRect),         // wSrcWidth
							   RECTHEIGHT(lpDIBRect),        // wSrcHeight
							   m_pBits,                      // lpBits
							   m_pBMI,                       // lpBitsInfo
							   DIB_RGB_COLORS,               // wUsage
							   SRCCOPY);                     // dwROP

	/* Reselect old palette */
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

   return bSuccess;
}

/*************************************************************************
 *
 * CreatePalette()
 *
 * Return Value:
 *
 * TRUE if succesfull, FALSE otherwise
 *
 * Description:
 *
 * This function creates a palette from a DIB by allocating memory for the
 * logical palette, reading and storing the colors from the DIB's color table
 * into the logical palette, creating a palette from this logical palette,
 * and then returning the palette's handle. This allows the DIB to be
 * displayed using the best possible colors (important for DIBs with 256 or
 * more colors).
 *
 ************************************************************************/

BOOL CDib::CreatePalette()
{
	if (!m_pBMI)
		return FALSE;

   //get the number of colors in the DIB
   WORD wNumColors = NumColors();

   if (wNumColors != 0)
   {
		// allocate memory block for logical palette
		HANDLE hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY)*wNumColors);

		// if not enough memory, clean up and return NULL
		if (hLogPal == 0)
			return FALSE;

		LPLOGPALETTE lpPal = (LPLOGPALETTE)::GlobalLock((HGLOBAL)hLogPal);

		// set version and number of palette entries
		lpPal->palVersion = PALVERSION;
		lpPal->palNumEntries = (WORD)wNumColors;

		for (int i = 0; i < (int)wNumColors; i++)
		{
			lpPal->palPalEntry[i].peRed = m_pBMI->bmiColors[i].rgbRed;
			lpPal->palPalEntry[i].peGreen = m_pBMI->bmiColors[i].rgbGreen;
			lpPal->palPalEntry[i].peBlue = m_pBMI->bmiColors[i].rgbBlue;
			lpPal->palPalEntry[i].peFlags = 0;
		}

		/* create the palette and get handle to it */
		if (m_pPalette)
		{
			m_pPalette->DeleteObject();
			delete m_pPalette;
		}
		m_pPalette = new CPalette;
		BOOL bResult = m_pPalette->CreatePalette(lpPal);
		::GlobalUnlock((HGLOBAL) hLogPal);
		::GlobalFree((HGLOBAL) hLogPal);
		return bResult;
	}

	return TRUE;
}

/*************************************************************************
 *
 * Width()
 *
 * Return Value:
 *
 * DWORD            - width of the DIB
 *
 * Description:
 *
 * This function gets the width of the DIB from the BITMAPINFOHEADER
 * width field 
 *
 ************************************************************************/

DWORD CDib::Width() const
{
	if (!m_pBMI)
		return 0;

	/* return the DIB width */
	return m_pBMI->bmiHeader.biWidth;
}


/*************************************************************************
 *
 * Height()
 *
 * Return Value:
 *
 * DWORD            - height of the DIB
 *
 * Description:
 *
 * This function gets the height of the DIB from the BITMAPINFOHEADER
 * height field 
 *
 ************************************************************************/

DWORD CDib::Height() const
{
	if (!m_pBMI)
		return 0;
	
	/* return the DIB height */
	return m_pBMI->bmiHeader.biHeight;
}


/*************************************************************************
 *
 * PaletteSize()
 *
 * Return Value:
 *
 * WORD             - size of the color palette of the DIB
 *
 * Description:
 *
 * This function gets the size required to store the DIB's palette by
 * multiplying the number of colors by the size of an RGBQUAD 
 *
 ************************************************************************/

WORD CDib::PaletteSize() const
{
	if (!m_pBMI)
		return 0;

	return NumColors() * sizeof(RGBQUAD);
}


/*************************************************************************
 *
 * NumColors()
 *
 * Return Value:
 *
 * WORD             - number of colors in the color table
 *
 * Description:
 *
 * This function calculates the number of colors in the DIB's color table
 * by finding the bits per pixel for the DIB (whether Win3.0 or other-style
 * DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256,
 * if 24, no colors in color table.
 *
 ************************************************************************/

WORD CDib::NumColors() const
{
	if (!m_pBMI)
		return 0;

	WORD wBitCount;  // DIB bit count

	/*  The number of colors in the color table can be less than 
	 *  the number of bits per pixel allows for (i.e. lpbi->biClrUsed
	 *  can be set to some value).
	 *  If this is the case, return the appropriate value.
	 */

	DWORD dwClrUsed;

	dwClrUsed = m_pBMI->bmiHeader.biClrUsed;
	if (dwClrUsed != 0)
		return (WORD)dwClrUsed;

	/*  Calculate the number of colors in the color table based on
	 *  the number of bits per pixel for the DIB.
	 */
	wBitCount = m_pBMI->bmiHeader.biBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}

/*************************************************************************
 *
 * Save()
 *
 * Saves the specified DIB into the specified CFile.  The CFile
 * is opened and closed by the caller.
 *
 * Parameters:
 *
 * CFile& file - open CFile used to save DIB
 *
 * Return value: Number of saved bytes or CFileException
 *
 *************************************************************************/

DWORD CDib::Save(CFile& file) const
{
	BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
	DWORD dwDIBSize;

	if (m_pBMI == NULL)
		return 0;

	// Fill in the fields of the file header

	// Fill in file type (first 2 bytes must be "BM" for a bitmap)
	bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"

	// Calculating the size of the DIB is a bit tricky (if we want to
	// do it right).  The easiest way to do this is to call GlobalSize()
	// on our global handle, but since the size of our global memory may have
	// been padded a few bytes, we may end up writing out a few too
	// many bytes to the file (which may cause problems with some apps).
	//
	// So, instead let's calculate the size manually (if we can)
	//
	// First, find size of header plus size of color table.  Since the
	// first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains
	// the size of the structure, let's use this.
	dwDIBSize = *(LPDWORD)&m_pBMI->bmiHeader + PaletteSize();  // Partial Calculation

	// Now calculate the size of the image
	if ((m_pBMI->bmiHeader.biCompression == BI_RLE8) || (m_pBMI->bmiHeader.biCompression == BI_RLE4))
	{
		// It's an RLE bitmap, we can't calculate size, so trust the
		// biSizeImage field
		dwDIBSize += m_pBMI->bmiHeader.biSizeImage;
	}
	else
	{
		DWORD dwBmBitsSize;  // Size of Bitmap Bits only

		// It's not RLE, so size is Width (DWORD aligned) * Height
		dwBmBitsSize = WIDTHBYTES((m_pBMI->bmiHeader.biWidth)*((DWORD)m_pBMI->bmiHeader.biBitCount)) * m_pBMI->bmiHeader.biHeight;
		dwDIBSize += dwBmBitsSize;

		// Now, since we have calculated the correct size, why don't we
		// fill in the biSizeImage field (this will fix any .BMP files which
		// have this field incorrect).
		m_pBMI->bmiHeader.biSizeImage = dwBmBitsSize;
	}

	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)
	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	/*
	 * Now, calculate the offset the actual bitmap bits will be in
	 * the file -- It's the Bitmap file header plus the DIB header,
	 * plus the size of the color table.
	 */
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + m_pBMI->bmiHeader.biSize + PaletteSize();

	// Write the file header
	file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
	DWORD dwBytesSaved = sizeof(BITMAPFILEHEADER); 

	// Write the DIB header
	UINT nCount = sizeof(BITMAPINFO) + (NumColors()-1)*sizeof(RGBQUAD);
	dwBytesSaved += nCount; 
	file.Write(m_pBMI, nCount);
	
	// Write the DIB bits
	DWORD dwBytes = m_pBMI->bmiHeader.biBitCount * Width();
  // Calculate the number of bytes per line
	if (dwBytes%32 == 0)
		dwBytes /= 8;
	else
		dwBytes = dwBytes/8 + (32-dwBytes%32)/8 + (((32-dwBytes%32)%8 > 0) ? 1 : 0); 
	nCount = dwBytes * Height();
	dwBytesSaved += nCount; 
	file.Write(m_pBits, nCount);

	return dwBytesSaved;
}


/*************************************************************************

  Function:  Read (CFile&)

   Purpose:  Reads in the specified DIB file into a global chunk of
			 memory.

   Returns:  Number of read bytes.

*************************************************************************/

DWORD CDib::Read(CFile& file)
{
	// Ensures no memory leaks will occur
	Free();
	
	BITMAPFILEHEADER bmfHeader;

	// Go read the DIB file header and check if it's valid.
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return 0;
	if (bmfHeader.bfType != DIB_HEADER_MARKER)
		return 0;
	DWORD dwReadBytes = sizeof(bmfHeader);

	// Allocate memory for DIB
	m_pBMI = (LPBITMAPINFO)GlobalAllocPtr(GHND, bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER) + 256*sizeof(RGBQUAD));
	if (m_pBMI == 0)
		return 0;

	// Read header.
	if (file.Read(m_pBMI, bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER)) != (UINT)(bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER)))
	{
		GlobalFreePtr(m_pBMI);
		m_pBMI = NULL;
		return 0;
	}
	dwReadBytes += bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER);

	DWORD dwLength = file.GetLength();
	// Go read the bits.
	m_pBits = (LPBYTE)GlobalAllocPtr(GHND, dwLength - bmfHeader.bfOffBits);
	if (m_pBits == 0)
	{
		GlobalFreePtr(m_pBMI);
		m_pBMI = NULL;
		return 0;
	}

	if (file.Read(m_pBits, dwLength-bmfHeader.bfOffBits) != (dwLength - bmfHeader.bfOffBits))
	{
		GlobalFreePtr(m_pBMI);
		m_pBMI = NULL;
		GlobalFreePtr(m_pBits);
		m_pBits = NULL;
		return 0;
	}
	dwReadBytes += dwLength - bmfHeader.bfOffBits;

	
	CreatePalette();
	return dwReadBytes;
}

#ifdef _DEBUG
void CDib::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif

//////////////////////////////////////////////////////////////////////////
//// Clipboard support

//---------------------------------------------------------------------
//
// Function:   CopyToHandle
//
// Purpose:    Makes a copy of the DIB to a global memory block.  Returns
//             a handle to the new memory block (NULL on error).
//
// Returns:    Handle to new global memory block.
//
//---------------------------------------------------------------------

HGLOBAL CDib::CopyToHandle() const
{
	CSharedFile file;
	try
	{
		if (Save(file)==0)
			return 0;
	}
	catch (CFileException* e)
	{
		e->Delete();
		return 0;
	}
		
	return file.Detach();
}

//---------------------------------------------------------------------
//
// Function:   ReadFromHandle
//
// Purpose:    Initializes from the given global memory block.  
//
// Returns:    Number of read bytes.
//
//---------------------------------------------------------------------

DWORD CDib::ReadFromHandle(HGLOBAL hGlobal)
{
	CSharedFile file;
	file.SetHandle(hGlobal, FALSE);
	DWORD dwResult = Read(file);
	file.Detach();
	return dwResult;
}

//////////////////////////////////////////////////////////////////////////
//// Serialization support

void CDib::Serialize(CArchive& ar) 
{
	CFile* pFile = ar.GetFile();
	ASSERT(pFile != NULL);
	if (ar.IsStoring())
	{	// storing code
		Save(*pFile);
	}
	else
	{	// loading code
		Read(*pFile);
	}
}


void CDib::myConvertToGrayScale(LPBYTE lpBits) //STR lpDIB)
{
/*	LPSTR lpDIBBits;	//指针DIB像素的指针 
	int lWidth;			//图像的宽度和高度 
	int lHeight;
	lWidth = Width();
	lHeight = Height();

	unsigned  char *ired,*igreen,*iblue; 
	int i,j;
	RGBQUAD  *lpRGBquad; 
	lpRGBquad = (RGBQUAD *)&lpDIB[sizeof(BITMAPINFOHEADER)]; //INFOHEADER后为调色板 

	lpDIBBits = (LPSTR)lpDIB + sizeof(BITMAPINFOHEADER);	//指向DIB像素 
	int lLineBytes = WIDTHBYTES(lWidth*8*3);
	for(i = 0;i < lHeight; i++) 
		for(j = 0;j < lWidth*3; j+=3) 
		{ 
			ired  =  (unsigned  char*)lpDIBBits + lLineBytes * i + j + 2; 
			igreen =  (unsigned  char*)lpDIBBits + lLineBytes * i + j + 1; 
			iblue  =  (unsigned  char*)lpDIBBits + lLineBytes * i + j ; 
			//lpdest[i*lWidth + j/3]  = (unsigned  char)((*ired)*0.299 + (*igreen)*0.588 + (*iblue)*0.114); 
			unsigned char y = (unsigned  char)((*ired)*0.299 + (*igreen)*0.588 + (*iblue)*0.114); 
			(*ired) = y;
			(*igreen) = y;
			(*iblue) = y;
		} 

	if(::DIBNumColors(lpDIB) == 256)  //256色位图不作任何处理 
	{ 
		return TRUE; 
	} */

}
/*BOOL COb_schgraphic::ConvertToGrayScale(LPSTR lpDIB)  
{ 

LPSTR lpDIBBits;                //指针DIB像素的指针 
LPSTR lpNewDIBBits;            //指向DIB灰度图像(新图像)的开始处像素的指针 
        LONG lLineBytes; 
unsigned char * lpSrc;          //指向原图像像素点的指针 
unsigned char * lpdest;        //指向目标图像的像素点的指针 

        unsigned  char *ired,*igreen,*iblue; 

        long lWidth;                    //图像的宽度和高度 
long lHeight; 

long i,j;          //循环变量 

lWidth = ::DIBWidth(lpDIB);  //DIB宽度 
lHeight = ::DIBHeight(lpDIB); //DIB 高度 
RGBQUAD  *lpRGBquad; 
lpRGBquad = (RGBQUAD *)&lpDIB[sizeof(BITMAPINFOHEADER)]; //INFOHEADER后为调色板 
        if(::DIBNumColors(lpDIB) == 256)  //256色位图不作任何处理 
{ 
return TRUE; 
        } 

if(::DIBNumColors(lpDIB) != 256)  //非256色位图将它灰度化 
{ 
lLineBytes = WIDTHBYTES(lWidth*8*3); 
lpdest=  new  BYTE[lHeight*lWidth]; 
lpDIBBits = (LPSTR)lpDIB + sizeof(BITMAPINFOHEADER);//指向DIB像素 
for(i = 0;i < lHeight; i++) 
for(j = 0;j < lWidth*3; j+=3) 
{ 
ired  =  (unsigned  char*)lpDIBBits + lLineBytes * i + j + 2; 
igreen =  (unsigned  char*)lpDIBBits + lLineBytes * i + j + 1; 
iblue  =  (unsigned  char*)lpDIBBits + lLineBytes * i + j ; 
lpdest[i*lWidth + j/3]  = (unsigned  char)((*ired)*0.299 + (*igreen)*0.588 + (*iblue)*0.114); 
} 

//需要做三件事:1修改INFOHEADER  2.增加调色板  3.修改原图像灰度值 
LPBITMAPINFOHEADER  lpBI; 
lpBI  =  (LPBITMAPINFOHEADER)lpDIB; 
lpBI->biBitCount = 8; 

//设置灰度调色板 
for(i = 0;i < 256;i++) 
{ 
lpRGBquad[i].rgbRed  = (unsigned char)i; 
lpRGBquad[i].rgbGreen = (unsigned char)i; 
lpRGBquad[i].rgbBlue  = (unsigned char)i; 
lpRGBquad[i].rgbReserved = 0; 
} 

lpNewDIBBits= ::FindDIBBits(lpDIB);  //找到DIB(新的)像素的起始位置 
lLineBytes=WIDTHBYTES(lWidth  *  8); 
//修改灰度值 
for(i = 0;i < lHeight; i++) 
for(j = 0;j < lWidth; j++) 
{ 
lpSrc = (unsigned  char*)lpNewDIBBits + lLineBytes * i+ j ; 
*lpSrc=lpdest[i*lWidth+j]; 
} 
delete  lpdest; 
} 
return true; 
} */

/*BOOL WINAPI Color2Gray(HGLOBAL hDIB,LONG lWidth,LONG lHeight)
{ 
	long i,j;
	BYTE r,g,b, newColor;
	
	LONG lLineBytes; 
　　
	LPBITMAPINFO lpbmi;
	LPBITMAPCOREINFO lpbmc;
　　
　　WORD wNumColors; 
　　 
　　BOOL bWinStyleDIB;
　　BOOL bResult=FALSE;
　　
　　LPSTR lpDIB;
　　lpDIB=(LPSTR)::GlobalLock((HGLOBAL)hDIB);
　　
　　lLineBytes = WIDTHBYTES(lWidth * 24); //
　　
　　lpbmi=(LPBITMAPINFO)lpDIB;
　　lpbmc=(LPBITMAPCOREINFO)lpDIB;
　　wNumColors=::DIBNumColors(lpDIB);
　　
　　LPSTR lpDIBBits,lpNewDIBBits;
　　lpDIBBits=::FindDIBBits(lpDIB);// 找到DIB图像象素起始位置
　　unsigned char * lpdest;
　　
　　unsigned char * ired;
　　unsigned char * igreen;
　　unsigned char * iblack; 
　　unsigned char* lpSrc; 
　　
　　if(wNumColors==24)//24位真彩色转256灰度
　　{
	  int n=0;
	  lpdest=(unsigned char *)::malloc(lLineBytes*lHeight);
	  memset(lpdest,0,lLineBytes*lHeight);//初始化 
	  for(j=0;j< lHeight; j++)
	  {
		  for(i=0;i<lWidth; i++)
		  {
			  ired= (unsigned char*)lpDIBBits + lLineBytes*j+i*3+2;
			  igreen = (unsigned char*)lpDIBBits+lLineBytes*j+i*3+1;
			  iblack = (unsigned char*)lpDIBBits +lLineBytes*j+i*3;
			  lpdest[n] =(unsigned char)(0.299*(*ired)+0.587*(*igreen)+0.114*(*iblack));
			  n++;
		  }
	  }
	  
	  //设置256色灰度调色板 

	  LPBITMAPINFOHEADER lpBI;//位图信息头 
　　  // 读取BITMAPINFO结构，初始化指针
　　 
	  lpBI = (LPBITMAPINFOHEADER)lpDIB;//[sizeof(BITMAPFILEHEADER)];
	  lpBI->biBitCount=8; 
　　  LPBITMAPFILEHEADER lpbmfHdr;
	  lpbmfHdr=(LPBITMAPFILEHEADER)lpDIB;
	  DWORD dwDIBSize;
　　 
	  // 读取BITMAPINFO结构，初始化指针 
　 
	  RGBQUAD *lpRGBquad;
	  lpRGBquad=(RGBQUAD *)&lpDIB[sizeof(BITMAPINFOHEADER)];//位图信息头后面为调色板
　　 

	  for (i = 0; i < 256; i++)
	  { 
　　 	  lpRGBquad[i].rgbRed =(unsigned char)i;// 读取红色分量 
　　 	  lpRGBquad[i].rgbGreen =(unsigned char)i;// 读取绿色分量 
　　 	  lpRGBquad[i].rgbBlue = (unsigned char)i;// 读取红色分量 
　　 	  lpRGBquad[i].rgbReserved = 0;// 保留位
　　 
	  } 
　　 
　　 
	  lpNewDIBBits=::FindDIBBits(lpDIB);// 找到DIB图像象素起始位置
　　 
	  long lNewLineBytes;
　　 
	  lNewLineBytes = WIDTHBYTES(lWidth * 8);
　　 
　　 
	  n=0;
　　 
	  for(j=0;j　　 {
　　 
		  for(i=0;i　　 {
　　 
			  lpSrc = (unsigned char*)lpNewDIBBits + lNewLineBytes * j + i; 
　　 
			  *lpSrc=lpdest[n];
　　 
			  n++; 
		  }
　　 
	  } 
　　 
　　 ::free((void *)lpdest); 
　　 }
　　
　　
　　 else if(wNumColors==24)//256彩色转256灰度
　　 {
　　 bWinStyleDIB=IS_WIN30_DIB(lpDIB);
　　 
　　 for(i=0;i<(int)wNumColors;i++)
　　 {
　　 if(bWinStyleDIB)
　　 {
　　 r=lpbmi->bmiColors[i].rgbRed;
　　 g=lpbmi->bmiColors[i].rgbGreen;
　　 b=lpbmi->bmiColors[i].rgbBlue;
　　 newColor=(0.299*r+0.587*g+0.114*b);
　　 lpbmi->bmiColors[i].rgbRed=newColor;
　　 lpbmi->bmiColors[i].rgbGreen=newColor;
　　 lpbmi->bmiColors[i].rgbBlue=newColor;
　　 lpbmi->bmiColors[i].rgbReserved=0;
　　 }
　　 else
　　 {
　　 r=lpbmc->bmciColors[i].rgbtRed;
　　 g=lpbmc->bmciColors[i].rgbtGreen;
　　 b=lpbmc->bmciColors[i].rgbtBlue;
　　 newColor=(30*r+59*g+11*b)/100;
　　 lpbmc->bmciColors[i].rgbtRed=newColor;
　　 lpbmc->bmciColors[i].rgbtGreen=newColor;
　　 lpbmc->bmciColors[i].rgbtBlue=newColor;
　　 
　　 }
　　 } 
　　 } 
　　 return bResult;
　　}*/
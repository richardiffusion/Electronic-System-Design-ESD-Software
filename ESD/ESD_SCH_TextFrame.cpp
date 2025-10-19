#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditTextFrame.h"

IMPLEMENT_SERIAL(COb_schtextframe, CObject, 0)
COb_schtextframe::COb_schtextframe()
{
	m_nFlag = 0;
	m_bSelection = FALSE;
	m_nOwnerPartID = 0;
	m_nDisplayMode = 0;
	m_arrayText.RemoveAll();
	m_posParent = NULL;

	pDocument = NULL;
}

void COb_schtextframe::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	   ar << m_fX1;
	   ar << m_fY1;
	   ar << m_fX2;
	   ar << m_fY2;

	   ar << m_nBorderWidth;

	   int num = m_arrayText.GetCount();
	   ar << num;
	   for(int i=0; i<num; i++)
	   {
		   ar << m_arrayText.GetAt(i);
	   }

	   ar << m_nFont;

	   ar << m_bClipToArea;
	   ar << m_bWordWrap;
	   ar << m_nAlignment;
	   ar << m_nTextColor;

	   ar << m_bDrawBorder;
	   ar << m_nBorderColor;
	   ar << m_bFillSolid;
	   ar << m_nFillColor;
	   
	   ar << m_nOwnerPartID;
	   ar << m_nDisplayMode;

	   ar << m_nDiagram;
	   ar << m_bSelection;
	}
	else
	{
	   ar >> m_fX1;
	   ar >> m_fY1;
	   ar >> m_fX2;
	   ar >> m_fY2;

	   ar >> m_nBorderWidth;

	   int num;
	   ar >> num;
	   CString cstring;
	   for(int i=0; i<num; i++)
	   {
		   ar >> cstring;
		   m_arrayText.Add(cstring);
	   }

	   ar >> m_nFont;

	   ar >> m_bClipToArea;
	   ar >> m_bWordWrap;
	   ar >> m_nAlignment;
	   ar >> m_nTextColor;

	   ar >> m_bDrawBorder;
	   ar >> m_nBorderColor;
	   ar >> m_bFillSolid;
	   ar >> m_nFillColor;
	   
	   ar >> m_nOwnerPartID;
	   ar >> m_nDisplayMode;

	   ar >> m_nDiagram;
	   ar >> m_bSelection;
	}
}

void COb_schtextframe::Draw(CDC* pDC, CView* pV)
{
	int bordercolor, textcolor, fillcolor;
	CStringArray stringarray;


	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(m_nFlag == FLAG_NORMAL)
	{
		bordercolor = m_nBorderColor;
		textcolor = m_nTextColor;
		fillcolor = m_nFillColor;
	}
	else if(m_nFlag == FLAG_DELETED) 
	{
		bordercolor = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor;
		textcolor = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor;
		fillcolor = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor;
	}
	
	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_Print_Color == 0) 
		{
			bordercolor = RGB(0,0,0);
			textcolor = RGB(0,0,0);
			fillcolor = RGB(255,255,255);
		}
		else if(nEDA_Print_Color == 2) 
		{
			bordercolor =  RGB(GetGray(m_nBorderColor), GetGray(m_nBorderColor), GetGray(m_nBorderColor));
			textcolor = RGB(GetGray(m_nTextColor), GetGray(m_nTextColor), GetGray(m_nTextColor));
			fillcolor = RGB(GetGray(m_nFillColor), GetGray(m_nFillColor), GetGray(m_nFillColor));
		}
		else 
		{
			bordercolor = m_nBorderColor;
			textcolor = m_nTextColor;
			fillcolor = m_nFillColor;
		}
	}

	int x1,y1,x2,y2;
	pView->Get_VPort_XY(&x1, &y1, m_fX1, m_fY1);
	pView->Get_VPort_XY(&x2, &y2, m_fX2, m_fY2);
	if(pView->m_nPrint_Flag == 1)
	{
		x1 = (int)(((double)x1*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		y1 = (int)(((double)y1*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
		x2 = (int)(((double)x2*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		y2 = (int)(((double)y2*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
	}

	int width;
	width = nEDA_Line_Width[m_nBorderWidth];

	width = (int)(width/pView->m_fScale);
	if(pView->m_nPrint_Flag == 1) width = (int)(width*nEDA_Print_Xdpi/1000*fEDA_Print_Scale);
	if(width < 1) width = 1;
	
	CPen fillpen;
	fillpen.CreatePen(PS_SOLID, 1, fillcolor);
	CPen* oldpen = pDC->SelectObject(&fillpen);

	CBrush fillbrush;
	fillbrush.CreateSolidBrush(fillcolor);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	RECT rect;
	rect.left = x1; rect.bottom = y1;
	rect.right = x2;rect.top = y2;
	if(m_bFillSolid == TRUE)
	{
		pDC->Rectangle(&rect);
	}

	CPen borderpen;
	borderpen.CreatePen(PS_SOLID, width, bordercolor);
	pDC->SelectObject(&borderpen);
	if(m_bDrawBorder == TRUE)
	{ 
		pDC->MoveTo(x1,y1);
		pDC->LineTo(x1,y2);
		pDC->LineTo(x2,y2);
		pDC->LineTo(x2,y1);
		pDC->LineTo(x1,y1);
	}

	if(m_nFlag == FLAG_DELETED) return;

	stringarray.Copy(m_arrayText);


	CRgn frameClipRgn;

	if(m_bClipToArea == TRUE)
	{

	}
	
	double height;
	LOGFONT logfont;
	logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
	height = logfont.lfHeight;
	height *= fEDA_Font_Scale;


	COb_schtext schtext;

	schtext.m_nOrientation = 0;
	schtext.m_nH_Justification = 0;
	schtext.m_nV_Justification = 0;
	schtext.m_nFont = m_nFont;
	schtext.m_nColor = textcolor;
	schtext.m_nDiagram = m_nDiagram;
	schtext.m_bMirror = FALSE;
	schtext.m_nID = TEXT_TEXT;
	schtext.m_nFlag = 0;
	
	int total = stringarray.GetSize();
	double textframe_width = m_fX2 - m_fX1 - 20; 

	for(int k=0; k<total; k++)
    {
		schtext.m_csText = stringarray.GetAt(k);
		schtext.m_fSx = 0;
		schtext.m_fSy = 0;

		double delta = schtext.GetTabbedExtent(pDC, pV)*pView->m_fScale;

		//WordWrap
		if(m_bWordWrap == TRUE)
		{
			if(delta > textframe_width)
			{
				CString str, str1,str2;
				str = schtext.m_csText;
				int length = str.GetLength();
				for(int i=1; i<=length; i++)
				{
					str1 = str.Left(i);
					schtext.m_csText = str1;
					delta = schtext.GetTabbedExtent(pDC, pV)*pView->m_fScale;
					if(delta > textframe_width) 
					{
						if((str1.Find(0x20) >= 0)||(str.Find(0x09) >= 0))  
						{
							int l = str1.GetLength();
							for(int ii=l-1; ii>=0; ii--) 
							{
								if((str1.GetAt(ii) == 0x20)||(str1.GetAt(ii) == 0x09))
								{
									str1 = str.Left(ii);
									str2 = str.Right(str.GetLength() - str1.GetLength());
									break;
								}
							}
							break;
						}
						else
						{
							if(str1.GetLength() >2) str1 = str1.Left(str1.GetLength() - 2);
							else if(str1.GetLength() >1) str1 = str1.Left(str1.GetLength() - 1);
							str2 = str.Right(str.GetLength() - str1.GetLength());
							break;
						}
					}
				}

				schtext.m_csText = str1;
				str2.TrimLeft();
				stringarray.InsertAt(k+1, str2);
				total++;
			}
		}

		delta = schtext.GetTabbedExtent(pDC, pV)*pView->m_fScale;

		double sx,sy;
		if(m_nAlignment == TEXT_ALIGNMENT_LEFT) sx = m_fX1 + 10;
		else if(m_nAlignment == TEXT_ALIGNMENT_RIGHT)	sx = m_fX1 + ((m_fX2-m_fX1) - delta) - 20;
		else sx = m_fX1 + ((m_fX2-m_fX1) - delta)/2;

		sy = m_fY2 - height*(k+1) - 10;
		schtext.m_fSx = sx;
		schtext.m_fSy = sy;

		if(m_bClipToArea == TRUE)
		{
            if((sy + height)< m_fY1) break;
		}

		schtext.DrawTabbed(pDC, pV);
	}

	if(m_bClipToArea == TRUE)
	{
	}

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void COb_schtextframe::DrawSelection(CDC* pDC, CView* pV)
{
	int x1, y1, x2, y2;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	pView->Get_VPort_XY(&x1,&y1,m_fX1,m_fY1);
	pView->Get_VPort_XY(&x2,&y2,m_fX2,m_fY2);

	pView->DrawSelectionKeyPoint(x1, y1, pDC);
	pView->DrawSelectionKeyPoint(x1, y2, pDC);
	pView->DrawSelectionKeyPoint(x2, y1, pDC);
	pView->DrawSelectionKeyPoint(x2, y2, pDC);
	pView->DrawSelectionKeyPoint((x1+x2)/2, y1, pDC);
	pView->DrawSelectionKeyPoint((x1+x2)/2, y2, pDC);
	pView->DrawSelectionKeyPoint(x1, (y1+y2)/2, pDC);
	pView->DrawSelectionKeyPoint(x2, (y1+y2)/2, pDC);

}

BOOL COb_schtextframe::PickMe(CView* pV)
{
	int wx1,wy1,wx2,wy2;

	CESDView* pView = (CESDView*)pV;

	if(pView->m_nPrint_Flag == 1)
	{
		wx1 = (int)fEDA_Print_Minx;		wy1 = (int)fEDA_Print_Miny;
		wx2 = (int)fEDA_Print_Maxx;		wy2 = (int)fEDA_Print_Maxy;
	}
	else 
	{
		wx1 = (int)pView->m_fScreenX; 
		wy1 = (int)pView->m_fScreenY;
		wx2 = (int)(pView->m_fScreenX + pView->m_fScale*pView->m_nViewport_Ex);  
		wy2 = (int)(pView->m_fScreenY + pView->m_fScale*pView->m_nViewport_Ey);

		wx1 -= 100;
		wy1 -= 100;
		wx2 += 100;
		wy2 += 100;
	}


	int x1,y1,x2,y2;
	x1 = (int)m_fX1;
	y1 = (int)m_fY1;
	x2 = (int)m_fX2;
	y2 = (int)m_fY2;

	if( ((x1 < wx1)&&(x2 < wx1)) || ((x1 > wx2)&&(x2 > wx2)) || ((y1 < wy1)&&(y2 < wy1)) || ((y1 > wy2)&&(y2 > wy2)) )	return FALSE;
	else return TRUE;
}

BOOL  COb_schtextframe::Is_On_Me(double fx, double fy)
{
	//if(diagram != m_nDiagram) return FALSE;

	CRgn   rgn;

	rgn.CreateRectRgn( (int)m_fX1, (int)m_fY1, (int)m_fX2, (int)m_fY2);

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL  COb_schtextframe::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fminx, fminy, fmaxx, fmaxy;
	fminx = MIN(m_fX1, m_fX2);
	fminy = MIN(m_fY1, m_fY2);
	fmaxx = MAX(m_fX1, m_fX2);
	fmaxy = MAX(m_fY1, m_fY2);
	
	if((fminx < fx1)||(fmaxx > fx2)||(fminy < fy1)||(fmaxy > fy2)) return FALSE;
	return TRUE;
}

void COb_schtextframe::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_EditTextFrame dlg;

	dlg.m_csEditLocationX1 = pDoc->Convert_XY_To_String(m_fX1);
	dlg.m_csEditLocationY1 = pDoc->Convert_XY_To_String(m_fY1);
	dlg.m_csEditLocationX2 = pDoc->Convert_XY_To_String(m_fX2);
	dlg.m_csEditLocationY2 = pDoc->Convert_XY_To_String(m_fY2);

	dlg.m_bCheckDrawSolid = m_bFillSolid;
	dlg.m_bCheckShowBorder = m_bDrawBorder;
	dlg.m_bCheckWordWrap = m_bWordWrap;
	dlg.m_bCheckClip = m_bClipToArea;
	
	dlg.m_nComboWidth = m_nBorderWidth;
	dlg.m_nComboAlignment = m_nAlignment;
	
	dlg.m_nFillColor = m_nFillColor;
	dlg.m_nBorderColor = m_nBorderColor;
	dlg.m_nTextColor = m_nTextColor;

	LOGFONT lf;
	lf = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
	dlg.m_csFontName = lf.lfFaceName;

	char str[100]; itoa(lf.lfHeight, str, 10);
	dlg.m_csFontSize = str;

	if(lf.lfWeight == FW_BOLD)	dlg.m_bBold = TRUE;
	else dlg.m_bBold = FALSE;
	dlg.m_bItalic = lf.lfItalic;
	dlg.m_bUnderline = lf.lfUnderline;

	dlg.m_csText.Empty();
	for(int i=0; i<m_arrayText.GetCount(); i++)
	{
		dlg.m_csText += m_arrayText.GetAt(i);
		dlg.m_csText += "\r\n";
	}

	if(dlg.DoModal() == IDOK)
	{
		if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = pos;
		pundo->schtextframe.Copy(this);
			
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

		m_fX1 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX1);
		m_fY1 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY1);
		m_fX2 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX2);
		m_fY2 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY2);

		m_bFillSolid = dlg.m_bCheckDrawSolid;
		m_bDrawBorder = dlg.m_bCheckShowBorder;
		m_bWordWrap = dlg.m_bCheckWordWrap;
		m_bClipToArea = dlg.m_bCheckClip;
	
		m_nBorderWidth = dlg.m_nComboWidth;
		m_nAlignment = dlg.m_nComboAlignment;
		
		m_nFillColor = dlg.m_nFillColor;
		m_nBorderColor = dlg.m_nBorderColor;
		m_nTextColor = dlg.m_nTextColor;

		//字型参数
		if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
		strcpy(lf.lfFaceName, dlg.m_csFontName);

		int index = dlg.m_csFontSize.Find('(');
		strcpy(str, dlg.m_csFontSize);
		if(index >= 0)	str[index] = '\0';
		lf.lfHeight = atoi(str);

		if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
		else lf.lfWeight = FW_NORMAL;
		lf.lfItalic = dlg.m_bItalic;
		lf.lfUnderline = dlg.m_bUnderline;

		int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
		if(font_num >= 0) m_nFont = font_num;
		else
		{
			m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
		}

		//文字
		m_arrayText.RemoveAll();
		int ii = dlg.m_csText.GetLength();

		int l = 0;
		char string[1000];
		for(int i=0; i<ii; i++ )
		{
			if(dlg.m_csText[i] == 0x0d) {}
			else if(dlg.m_csText[i] == 0x0a)
			{
				string[l] = '\0';
				CString cstring = string;
				m_arrayText.Add(cstring);
				l=0;
			}
			else
			{
				if(l<999)
				{
					string[l] = dlg.m_csText[i];
					l++;
				}
			}
		}
		string[l] = '\0';
		if(strlen(string) > 0) 
		{
			CString cstring = string;
			m_arrayText.Add(cstring);
		}
	}
}

void COb_schtextframe::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE; 
	
	char szHelp[] = "移动文字框: <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	pView->Invalidate();


	x0 = m_fX1;
	y0 = m_fY1;

	pView->m_fCurrentSnapX = x0;
	pView->m_fCurrentSnapY = y0;
	pView->Restore_LargeCrossXY();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schrect arect;
	arect.pDocument = pDoc;
	arect.m_fX1 = m_fX1;
	arect.m_fY1 = m_fY1;
	arect.m_fX2 = m_fX2;
	arect.m_fY2 = m_fY2;
	arect.m_nBorderWidth = m_nBorderWidth;
	arect.m_nFillColor = m_nFillColor;
	arect.m_nBorderColor = m_nBorderColor;
	arect.m_bTransparent = FALSE;
	arect.m_bFillSolid = m_bFillSolid;
	arect.m_nDiagram = m_nDiagram;

	arect.Draw(pDC, pV);
	pView->OpenMouseCursor();

	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			if(pView->m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(pView->m_bAfterSetFocus)
			{
				pView->m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				pView->CloseMouseCursor();
				arect.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			arect.m_fX1 = m_fX1 + deltax;
			arect.m_fY1 = m_fY1 + deltay;
			arect.m_fX2 = m_fX2 + deltax;
			arect.m_fY2 = m_fY2 + deltay;

			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			arect.m_fX1 = m_fX1 + deltax;
			arect.m_fY1 = m_fY1 + deltay;
			arect.m_fX2 = m_fX2 + deltax;
			arect.m_fY2 = m_fY2 + deltay;

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_fX1 = m_fX1;
			pundo->m_fY1 = m_fY1;
			pundo->m_fX2 = m_fX2;
			pundo->m_fY2 = m_fY2;
	
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fX1 = arect.m_fX1;
			m_fY1 = arect.m_fY1;
			m_fX2 = arect.m_fX2;
			m_fY2 = arect.m_fY2;
			
			break;
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	

	m_nFlag = FLAG_NORMAL;

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;


	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	pView->Write_Help(szEDA_Help_Ready);
	pView->Invalidate();
}

BOOL COb_schtextframe::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	double x1,y1, x0, y0, deltax, deltay;
	
	char szHelp[] = "复制文字框: <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	pView->Invalidate();

	x0 = m_fX1; //pView->m_fCurrentSnapX;
	y0 = m_fY1; //pView->m_fCurrentSnapY;

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schrect arect;
	arect.pDocument = pDoc;
	arect.m_fX1 = m_fX1;
	arect.m_fY1 = m_fY1;
	arect.m_fX2 = m_fX2;
	arect.m_fY2 = m_fY2;
	arect.m_nBorderWidth = m_nBorderWidth;
	arect.m_nFillColor = m_nFillColor;
	arect.m_nBorderColor = m_nBorderColor;
	arect.m_bTransparent = FALSE;
	arect.m_bFillSolid = m_bFillSolid;
	arect.m_nDiagram = m_nDiagram;

	arect.Draw(pDC, pV);
	pView->OpenMouseCursor();

	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			if(pView->m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(pView->m_bAfterSetFocus)
			{
				pView->m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				pView->CloseMouseCursor();
				arect.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			arect.m_fX1 = m_fX1 + deltax;
			arect.m_fY1 = m_fY1 + deltay;
			arect.m_fX2 = m_fX2 + deltax;
			arect.m_fY2 = m_fY2 + deltay;

			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			arect.m_fX1 = m_fX1 + deltax;
			arect.m_fY1 = m_fY1 + deltay;
			arect.m_fX2 = m_fX2 + deltax;
			arect.m_fY2 = m_fY2 + deltay;

			m_fX1 = arect.m_fX1;
			m_fY1 = arect.m_fY1;
			m_fX2 = arect.m_fX2;
			m_fY2 = arect.m_fY2;
			
			break;
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	

	m_nFlag = FLAG_NORMAL;

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	pView->Write_Help(szEDA_Help_Ready);
		
	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}

void COb_schtextframe::Copy(COb_schtextframe* pschtextframe)
{
    m_fX1 = pschtextframe->m_fX1;
	m_fY1 = pschtextframe->m_fY1;
	m_fX2 = pschtextframe->m_fX2;
	m_fY2 = pschtextframe->m_fY2;

	m_nBorderWidth = pschtextframe->m_nBorderWidth;
	
	m_arrayText.RemoveAll();
	for(int i=0; i<pschtextframe->m_arrayText.GetCount(); i++)
	{
		CString cstring;
		cstring = pschtextframe->m_arrayText.GetAt(i);
		m_arrayText.Add(cstring);
	}	
	
	m_nFont = pschtextframe->m_nFont;

	m_bClipToArea = pschtextframe->m_bClipToArea;
	m_bWordWrap = pschtextframe->m_bWordWrap;
	m_nAlignment = pschtextframe->m_nAlignment;
	m_nTextColor = pschtextframe->m_nTextColor;

	m_bDrawBorder = pschtextframe->m_bDrawBorder;
	m_nBorderColor = pschtextframe->m_nBorderColor;
	m_bFillSolid = pschtextframe->m_bFillSolid;
	m_nFillColor = pschtextframe->m_nFillColor;

	m_nOwnerPartID = pschtextframe->m_nOwnerPartID;
	m_nDisplayMode = pschtextframe->m_nDisplayMode;
	m_posParent = pschtextframe->m_posParent;

	m_nDiagram = pschtextframe->m_nDiagram;
	m_bSelection = pschtextframe->m_bSelection;
	m_nFlag = pschtextframe->m_nFlag;
	
	pDocument = pschtextframe->pDocument;
}
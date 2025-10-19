#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_PCBEditCoordinate.h"


IMPLEMENT_SERIAL(COb_pcbcoordinate, CObject, 0)
COb_pcbcoordinate::COb_pcbcoordinate()
{
	m_fX = 0;
	m_fY = 0;
	m_fSize = 10;
	m_fLineWidth = 10;
	m_fTextHeight = 60;
	m_fTextWidth = 6;
	m_fRotation = 0;

	m_nLayer = TOPLAYER;
	m_nUnitStyle = UNIT_STYLE_BRACKETS;

	m_bLocked = FALSE;
	m_bPrimitiveLock = TRUE;

	m_nStrokeFont = 0;
	m_bUseTTFont = FALSE;
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_csTTFontName = "Arial";
	
	m_bPolygonOutline = FALSE;
	m_bUserRouted = TRUE;
	m_nUnionIndex = 0;

	m_bSelection = FALSE;	
	m_nFlag = 0;

}

void COb_pcbcoordinate::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX;
		ar << m_fY;
		ar << m_fSize;
		ar << m_fLineWidth;
		ar << m_fTextHeight;
		ar << m_fTextWidth;
		ar << m_fRotation; 
		ar << m_nLayer;
		ar << m_nUnitStyle;

		ar << m_bLocked;
		ar << m_bPrimitiveLock;

		ar << m_nStrokeFont;
		ar << m_bUseTTFont;
		ar << m_bBold;
		ar << m_bItalic;
		ar << m_csTTFontName;
		
		ar << m_bPolygonOutline;
		ar << m_bUserRouted;
		ar << m_nUnionIndex;
	}
	else
	{		
		ar >> m_fX;
		ar >> m_fY;
		ar >> m_fSize;
		ar >> m_fLineWidth;
		ar >> m_fTextHeight;
		ar >> m_fTextWidth;
		ar >> m_fRotation; 
		ar >> m_nLayer;
		ar >> m_nUnitStyle;

		ar >> m_bLocked;
		ar >> m_bPrimitiveLock;

		ar >> m_nStrokeFont;
		ar >> m_bUseTTFont;
		ar >> m_bBold;
		ar >> m_bItalic;
		ar >> m_csTTFontName;
		
		ar >> m_bPolygonOutline;
		ar >> m_bUserRouted;
		ar >> m_nUnionIndex;
	}
}

void COb_pcbcoordinate::Draw(CDC* pDC, CView* pV, int mode)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();


	COb_pcbtrack pcbtrack;
	pcbtrack.m_fWidth = m_fLineWidth;
	pcbtrack.m_nLayer = m_nLayer;	
	pcbtrack.m_nFlag = m_nFlag;
	
	pcbtrack.m_fX1 = m_fX - m_fSize;
	pcbtrack.m_fY1 = m_fY;
	pcbtrack.m_fX2 = m_fX + m_fSize;
	pcbtrack.m_fY2 = m_fY;
	if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

	pcbtrack.m_fX1 = m_fX;
	pcbtrack.m_fY1 = m_fY - m_fSize;
	pcbtrack.m_fX2 = m_fX;
	pcbtrack.m_fY2 = m_fY + m_fSize;
	if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);


	if(pDoc->m_cPCB_Sheet.m_nUnit == UNIT_PCB_I)
	{
		COb_pcbtext pcbtext;
		pcbtext.m_fSx = m_fX + m_fSize;
		pcbtext.m_fSy = m_fY;
		pcbtext.m_fHeight = m_fTextHeight;
		pcbtext.m_fWidth = m_fTextWidth;
		pcbtext.m_fRotation = m_fRotation;
		pcbtext.m_nLayer = m_nLayer;
		pcbtext.m_nStrokeFont = m_nStrokeFont;
		pcbtext.m_bUseTTFont = m_bUseTTFont;
		pcbtext.m_bBold = m_bBold;
		pcbtext.m_bItalic = m_bItalic;
		pcbtext.m_csTTFontName = m_csTTFontName;
		pcbtext.m_nFlag = m_nFlag;

		char str[100];
		CString cstr_x, cstr_y;
		pView->mil_to_str(m_fX - pDoc->m_cPCB_Sheet.m_fOriginX, str);
		cstr_x = str;
		pView->mil_to_str(m_fY - pDoc->m_cPCB_Sheet.m_fOriginY, str);
		cstr_y = str;
		cstr_x.Trim("mil");
		cstr_y.Trim("mil");

		if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx,xxxx
		{
			pcbtext.m_csText = cstr_x + "," + cstr_y;
		}
		else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil,xxxxmil
		{
			pcbtext.m_csText = cstr_x + "mil," + cstr_y + "mil";
		}
		else	//xxxx,xxxx (mil)
		{
			pcbtext.m_csText = cstr_x + "," + cstr_y + " (mil)";
		}
		
		pcbtext.m_bSelection = m_bSelection;
		
		if(mode == 0)
		{
			pcbtext.UpdateStringBorder(pV, TRUE);
			if(pcbtext.PickMe(pDC, pV) == TRUE) pcbtext.Draw(pDC, pV);
		}
		else
		{
			pcbtext.UpdateStringBorder(pV, TRUE);
			if(pcbtext.m_bUseTTFont == TRUE) pcbtext.DrawStringRect(pDC, pV);
			else pcbtext.Draw(pDC, pV);
		}
	}
	else
	{
		COb_pcbtext pcbtext;
		pcbtext.m_fSx = m_fX + m_fSize;
		pcbtext.m_fSy = m_fY;
		pcbtext.m_fHeight = m_fTextHeight;
		pcbtext.m_fWidth = m_fTextWidth;
		pcbtext.m_fRotation = m_fRotation;
		pcbtext.m_nLayer = m_nLayer;
		pcbtext.m_nStrokeFont = m_nStrokeFont;
		pcbtext.m_bUseTTFont = m_bUseTTFont;
		pcbtext.m_bBold = m_bBold;
		pcbtext.m_bItalic = m_bItalic;
		pcbtext.m_csTTFontName = m_csTTFontName;
		pcbtext.m_nFlag = m_nFlag;

		char str[100];
		CString cstr_x, cstr_y;
		pView->mm_to_str(m_fX - pDoc->m_cPCB_Sheet.m_fOriginX, str);
		cstr_x = str;
		pView->mm_to_str(m_fY - pDoc->m_cPCB_Sheet.m_fOriginY, str);
		cstr_y = str;
		cstr_x.Trim("m");
		cstr_y.Trim("m");

		if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx,xxxx
		{
			pcbtext.m_csText = cstr_x + "," + cstr_y;
		}
		else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil,xxxxmil
		{
			pcbtext.m_csText = cstr_x + "mm," + cstr_y + "mm";
		}
		else	//xxxx,xxxx (mil)
		{
			pcbtext.m_csText = cstr_x + "," + cstr_y + " (mm)";
		}
		pcbtext.m_bSelection = m_bSelection;
		if(mode == 0)
		{
			pcbtext.UpdateStringBorder(pV, TRUE);
			if(pcbtext.PickMe(pDC, pV) == TRUE) pcbtext.Draw(pDC, pV);
		}
		else
		{
			pcbtext.UpdateStringBorder(pV, TRUE);
			if(pcbtext.m_bUseTTFont == TRUE) pcbtext.DrawStringRect(pDC, pV);
			else pcbtext.Draw(pDC, pV);
		}
	}
}

BOOL  COb_pcbcoordinate::Is_On_Me(double fx, double fy, CView* pV)
{
    CESDDoc* pDoc = (CESDDoc*)pDocument;
	CESDView* pView = (CESDView*)pV;
	
	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return FALSE;

	COb_pcbtext pcbtext;
	pcbtext.pDocument = pDocument;
	if(pDoc->m_cPCB_Sheet.m_nUnit == UNIT_PCB_I)
	{
		pcbtext.m_fSx = m_fX + m_fSize;
		pcbtext.m_fSy = m_fY;
		pcbtext.m_fHeight = m_fTextHeight;
		pcbtext.m_fWidth = m_fTextWidth;
		pcbtext.m_fRotation = m_fRotation;
		pcbtext.m_nLayer = m_nLayer;
		pcbtext.m_nStrokeFont = m_nStrokeFont;
		pcbtext.m_bUseTTFont = m_bUseTTFont;
		pcbtext.m_bBold = m_bBold;
		pcbtext.m_bItalic = m_bItalic;
		pcbtext.m_csTTFontName = m_csTTFontName;

		char str[100];
		CString cstr_x, cstr_y;
		pView->mil_to_str(m_fX, str);
		cstr_x = str;
		pView->mil_to_str(m_fY, str);
		cstr_y = str;
		cstr_x.Trim("mil");
		cstr_y.Trim("mil");

		if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx,xxxx
		{
			pcbtext.m_csText = cstr_x + "," + cstr_y;
		}
		else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil,xxxxmil
		{
			pcbtext.m_csText = cstr_x + "mil," + cstr_y + "mil";
		}
		else	//xxxx,xxxx (mil)
		{
			pcbtext.m_csText = cstr_x + "," + cstr_y + " (mil)";
		}
	}
	else
	{
		pcbtext.m_fSx = m_fX + m_fSize;
		pcbtext.m_fSy = m_fY;
		pcbtext.m_fHeight = m_fTextHeight;
		pcbtext.m_fWidth = m_fTextWidth;
		pcbtext.m_fRotation = m_fRotation;
		pcbtext.m_nLayer = m_nLayer;
		pcbtext.m_nStrokeFont = m_nStrokeFont;
		pcbtext.m_bUseTTFont = m_bUseTTFont;
		pcbtext.m_bBold = m_bBold;
		pcbtext.m_bItalic = m_bItalic;
		pcbtext.m_csTTFontName = m_csTTFontName;

		char str[100];
		CString cstr_x, cstr_y;
		pView->mm_to_str(m_fX, str);
		cstr_x = str;
		pView->mm_to_str(m_fY, str);
		cstr_y = str;
		cstr_x.Trim("m");
		cstr_y.Trim("m");

		if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx,xxxx
		{
			pcbtext.m_csText = cstr_x + "," + cstr_y;
		}
		else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil,xxxxmil
		{
			pcbtext.m_csText = cstr_x + "mm," + cstr_y + "mm";
		}
		else	//xxxx,xxxx (mil)
		{
			pcbtext.m_csText = cstr_x + "," + cstr_y + " (mm)";
		}
	}

	double fh = pcbtext.m_fHeight;
	double fw = pcbtext.GetStringWidth(pV);	
	pcbtext.m_fX1 = pcbtext.m_fSx;
	pcbtext.m_fY1 = pcbtext.m_fSy;
	pcbtext.m_fX2 = pcbtext.m_fSx + fw;
	pcbtext.m_fY2 = pcbtext.m_fSy + fh;
	
	return pcbtext.Is_On_Me(fx, fy, pV);

}

void COb_pcbcoordinate::Change(int index)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	CDlg_PCBEditCoordinate dlg;

	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
	}
	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		if(m_nLayer == pDoc->m_arrayPcbLayerShow.GetAt(i))
		{
			dlg.m_nComboLayer = i;
			break;
		}
	}
	
	dlg.m_csEditLineWidth = pDoc->Convert_XY_To_String(m_fLineWidth);
	dlg.m_csEditSize = pDoc->Convert_XY_To_String(m_fSize);
	dlg.m_csEditTextHeight = pDoc->Convert_XY_To_String(m_fTextHeight);
	dlg.m_csEditTextLineWidth = pDoc->Convert_XY_To_String(m_fTextWidth);
	dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fX  - pDoc->m_cPCB_Sheet.m_fOriginX);
	dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fY  - pDoc->m_cPCB_Sheet.m_fOriginY);

	dlg.m_nComboStyle = m_nUnitStyle;
	dlg.m_bCheckLocked = m_bLocked;
	
	for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
	{
		if(m_csTTFontName == arrayEDA_SystemFont.GetAt(i)) 
		{
			dlg.nTTFont = i;
			break;
		}
	}
	dlg.nStrokeFont = m_nStrokeFont;

	if(m_bUseTTFont == TRUE)
	{
		dlg.m_nRadioTrueType = 0;
		dlg.m_nComboFont = dlg.nTTFont;
	}
	else 
	{
		dlg.m_nRadioTrueType = 1;
		dlg.m_nComboFont = dlg.nStrokeFont;
	}


	dlg.m_bCheckBold = m_bBold;
	dlg.m_bCheckItalic = m_bItalic;
	
	if(dlg.DoModal() == IDOK)
	{
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = PCB_ELEMENT_COORDINATE;
		pundo->m_nIndex = index;
		pundo->pcbcoordinate.Copy(this);
			
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

		
		for(int i=0; i<=MULTILAYER; i++)
		{
			if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
			{
				m_nLayer = i;
				break;
			}
		}
		
		m_fLineWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditLineWidth);
		m_fSize = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditSize);
		m_fTextHeight = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTextHeight);
		m_fTextWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTextLineWidth);
		m_fX = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditLocationX) + pDoc->m_cPCB_Sheet.m_fOriginX;
		m_fY = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditLocationY) + pDoc->m_cPCB_Sheet.m_fOriginY;

		m_nUnitStyle = dlg.m_nComboStyle;
		m_bLocked = dlg.m_bCheckLocked;
		
		if(dlg.m_nRadioTrueType == 0)
		{
			m_bUseTTFont = TRUE;
			m_csTTFontName = arrayEDA_SystemFont.GetAt(dlg.m_nComboFont);
		}
		else
		{
			m_bUseTTFont = FALSE;
			m_nStrokeFont = dlg.m_nComboFont;
		}

		m_bBold = dlg.m_bCheckBold;
		m_bItalic = dlg.m_bCheckItalic;

	}
}

void COb_pcbcoordinate::Copy(COb_pcbcoordinate* pcoordinate)
{
	m_fX = pcoordinate->m_fX;
	m_fY = pcoordinate->m_fY;
	m_fSize = pcoordinate->m_fSize;
	m_fLineWidth = pcoordinate->m_fLineWidth;
	m_fTextHeight = pcoordinate->m_fTextHeight;
	m_fTextWidth = pcoordinate->m_fTextWidth;
	m_fRotation = pcoordinate->m_fRotation;

	m_nLayer = pcoordinate->m_nLayer;
	m_nUnitStyle = pcoordinate->m_nUnitStyle;

	m_bLocked = pcoordinate->m_bLocked;
	m_bPrimitiveLock = pcoordinate->m_bPrimitiveLock;

	m_nStrokeFont = pcoordinate->m_nStrokeFont;
	m_bUseTTFont = pcoordinate->m_bUseTTFont;
	m_bBold = pcoordinate->m_bBold;
	m_bItalic = pcoordinate->m_bItalic;
	m_csTTFontName = pcoordinate->m_csTTFontName;

	m_bPolygonOutline = pcoordinate->m_bPolygonOutline;
	m_bUserRouted = pcoordinate->m_bUserRouted;
	m_nUnionIndex = pcoordinate->m_nUnionIndex;

	m_bSelection = FALSE; //pcoordinate->m_bSelection;
	m_nFlag = pcoordinate->m_nFlag;

	pDocument = pcoordinate->pDocument;
}

void COb_pcbcoordinate::Move(CClientDC* pDC, CView* pV,int index)
{
	int ch1;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动标注: <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	

	pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV, 0);

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	


	COb_pcbcoordinate acoordinate;
	acoordinate.Copy(this);
	acoordinate.m_nFlag = FLAG_NORMAL;

	acoordinate.Draw(pDC, pV, 1);
	pView->OpenMouseCursor();

	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			acoordinate.Draw(pDC, pV, 1);
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
				acoordinate.Draw(pDC, pV, 1);
				pView->OpenMouseCursor();
			}

			acoordinate.m_fX = pView->m_fCurrentSnapX;
			acoordinate.m_fY = pView->m_fCurrentSnapY;

			pView->CloseMouseCursor();
			acoordinate.Draw(pDC, pV, 1);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			acoordinate.Draw(pDC, pV, 1);
			pView->OpenMouseCursor();

			acoordinate.m_fX = pView->m_fCurrentSnapX;
			acoordinate.m_fY = pView->m_fCurrentSnapY;

			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_COORDINATE;
			pundo->m_nIndex = index;
			pundo->pcbcoordinate.Copy(this);
			pundo->pcbcoordinate.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fX = acoordinate.m_fX;
			m_fY = acoordinate.m_fY;

			break;
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	
	pView->CloseMouseCursor();

	m_nFlag = FLAG_NORMAL;
	Draw(pDC, pV, 0);
	pView->OpenMouseCursor();

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		//if(bEDA_AutoJunction == TRUE) pView->AutoRemoveJunction();
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	pView->Write_Help(szEDA_Help_Ready);
}

void COb_pcbcoordinate::DrawSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
	COb_pcbtext pcbtext;
	pcbtext.m_fSx = m_fX + m_fSize;
	pcbtext.m_fSy = m_fY;
	pcbtext.m_fHeight = m_fTextHeight;
	pcbtext.m_fWidth = m_fTextWidth;
	pcbtext.m_fRotation = m_fRotation;
	pcbtext.m_nLayer = m_nLayer;
	pcbtext.m_nStrokeFont = m_nStrokeFont;
	pcbtext.m_bUseTTFont = m_bUseTTFont;
	pcbtext.m_bBold = m_bBold;
	pcbtext.m_bItalic = m_bItalic;
	pcbtext.m_csTTFontName = m_csTTFontName;
	pcbtext.m_nFlag = m_nFlag;

	if(pDoc->m_cPCB_Sheet.m_nUnit == UNIT_PCB_I)
	{
		char str[100];
		CString cstr_x, cstr_y;
		pView->mil_to_str(m_fX - pDoc->m_cPCB_Sheet.m_fOriginX, str);
		cstr_x = str;
		pView->mil_to_str(m_fY - pDoc->m_cPCB_Sheet.m_fOriginY, str);
		cstr_y = str;
		cstr_x.Trim("mil");
		cstr_y.Trim("mil");

		if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx,xxxx
		{
			pcbtext.m_csText = cstr_x + "," + cstr_y;
		}
		else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil,xxxxmil
		{
			pcbtext.m_csText = cstr_x + "mil," + cstr_y + "mil";
		}
		else	//xxxx,xxxx (mil)
		{
			pcbtext.m_csText = cstr_x + "," + cstr_y + " (mil)";
		}
	}
	else
	{
		char str[100];
		CString cstr_x, cstr_y;
		pView->mm_to_str(m_fX - pDoc->m_cPCB_Sheet.m_fOriginX, str);
		cstr_x = str;
		pView->mm_to_str(m_fY - pDoc->m_cPCB_Sheet.m_fOriginY, str);
		cstr_y = str;
		cstr_x.Trim("m");
		cstr_y.Trim("m");

		if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx,xxxx
		{
			pcbtext.m_csText = cstr_x + "," + cstr_y;
		}
		else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil,xxxxmil
		{
			pcbtext.m_csText = cstr_x + "mm," + cstr_y + "mm";
		}
		else	//xxxx,xxxx (mil)
		{
			pcbtext.m_csText = cstr_x + "," + cstr_y + " (mm)";
		}
	}

	pcbtext.UpdateStringBorder(pV, TRUE);
	pcbtext.GetTextCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);

	int oldbkmode = pDC->SetBkMode(TRANSPARENT);


	CPen arcpen;
	arcpen.CreatePen(PS_SOLID, 1, nEDA_PCB_LayerColor[SELECTIONS]);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	POINT point[5];
	int x, y;
	pView->Get_VPort_XY(&x, &y, fx1, fy1);
	point[0].x = x; point[0].y = y;
	pView->Get_VPort_XY(&x, &y, fx4, fy4);
	point[1].x = x; point[1].y = y;
	pView->Get_VPort_XY(&x, &y, fx2, fy2);
	point[2].x = x; point[2].y = y;
	pView->Get_VPort_XY(&x, &y, fx3, fy3);
	point[3].x = x; point[3].y = y;
	pView->Get_VPort_XY(&x, &y, fx1, fy1);
	point[4].x = x; point[4].y = y;

	// Use the bit pattern to create a bitmap.
	WORD HatchBits[8] = { 0x55, 0xaa, 0x55, 0xaa, 0x55,  0xaa, 0x55, 0xaa };
	CBitmap bm;
	bm.CreateBitmap(8,8,1,1, HatchBits);
	// Create a pattern brush from the bitmap.	

	CBrush fillbrush;
	fillbrush.CreatePatternBrush(&bm);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	int mode = pDC->SetROP2(R2_MERGEPEN );
	pDC->Polygon(point, 5);
	pDC->SetROP2(mode);
	pDC->SelectObject(oldbrush);

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}
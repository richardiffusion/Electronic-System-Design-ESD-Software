#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_PCBEditComp.h"


IMPLEMENT_SERIAL(COb_pcbcomp, CObject, 0)
COb_pcbcomp::COb_pcbcomp()
{
	m_bNameOn = TRUE;
	m_nNamePosition = 0;
	m_bCommentOn = TRUE;
	m_nCommentPosition = 0;	

	m_bPrimitiveLock = TRUE;
	m_bLocked = FALSE;
	m_bLockStrings = FALSE;

	//swapping options
	m_bEnablePinSwapping = FALSE;
	m_bEnablePartSwapping = FALSE;

	m_csPattern.Empty();
	m_csSourceFootprintLibrary.Empty();
	m_csFootprintDescription.Empty();

	//int m_nIndexForSave;	
	m_bPolygonOutline = FALSE;
	m_bUserRouted = TRUE;
	m_nUnionIndex = -1;

	int m_nGroupNum = 0;
	m_nCount = 0;
	m_nComponentKind = 0;
	m_nComponentKindVersion2 = 0;
	m_nChannelOffset = 0;
	m_csSourceDesignator.Empty();
	m_csSourceUniqueID.Empty();
	m_csSourceHierarchicalPath.Empty();
	m_csSourceComponentLibrary.Empty();
	m_csSourceLibReference.Empty();
	m_csSourceDescription.Empty();
	m_csDefaultPcb3dModel.Empty();

	m_bSelection = FALSE;
	m_nFlag = 0;
}

COb_pcbcomp::~COb_pcbcomp()
{

}

void COb_pcbcomp::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX;
		ar << m_fY;

		ar << m_fRotation;
		ar << m_fHeight;
		ar << m_nLayer;

		ar << m_bNameOn;
		ar << m_nNamePosition;
		ar << m_bCommentOn;
		ar << m_nCommentPosition;

		ar << m_bPrimitiveLock;
		ar << m_bLocked;
		ar << m_bLockStrings;

		ar << m_bEnablePinSwapping;
		ar << m_bEnablePartSwapping;

		ar << m_csPattern;
		ar << m_csSourceFootprintLibrary;
		ar << m_csFootprintDescription;

		ar << m_bPolygonOutline;
		ar << m_bUserRouted;
		ar << m_nUnionIndex;

		ar << m_nGroupNum;
		ar << m_nCount;
		ar << m_nComponentKind;
		ar << m_nComponentKindVersion2;
		ar << m_nChannelOffset;
		ar << m_csSourceDesignator;
		ar << m_csSourceUniqueID;
		ar << m_csSourceHierarchicalPath;
		ar << m_csSourceComponentLibrary;
		ar << m_csSourceLibReference;
		ar << m_csSourceDescription;
		ar << m_csDefaultPcb3dModel;
		ar << m_bSelection;
	}
	else
	{
		ar >> m_fX;
		ar >> m_fY;

		ar >> m_fRotation;
		ar >> m_fHeight;
		ar >> m_nLayer;

		ar >> m_bNameOn;
		ar >> m_nNamePosition;
		ar >> m_bCommentOn;
		ar >> m_nCommentPosition;

		ar >> m_bPrimitiveLock;
		ar >> m_bLocked;
		ar >> m_bLockStrings;

		ar >> m_bEnablePinSwapping;
		ar >> m_bEnablePartSwapping;

		ar >> m_csPattern;
		ar >> m_csSourceFootprintLibrary;
		ar >> m_csFootprintDescription;

		ar >> m_bPolygonOutline;
		ar >> m_bUserRouted;
		ar >> m_nUnionIndex;

		ar >> m_nGroupNum;
		ar >> m_nCount;
		ar >> m_nComponentKind;
		ar >> m_nComponentKindVersion2;
		ar >> m_nChannelOffset;
		ar >> m_csSourceDesignator;
		ar >> m_csSourceUniqueID;
		ar >> m_csSourceHierarchicalPath;
		ar >> m_csSourceComponentLibrary;
		ar >> m_csSourceLibReference;
		ar >> m_csSourceDescription;
		ar >> m_csDefaultPcb3dModel;
		ar >> m_bSelection;	
	}
}

void COb_pcbcomp::Draw(CDC* pDC, CView* pV)
{

}

void COb_pcbcomp::DrawSelection(CDC* pDC, CView* pV, int index)
{
	if(m_nFlag != FLAG_NORMAL) return;
	
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nComponent == index)&&((ptext->m_bComment == TRUE)||(ptext->m_bDesignator == TRUE)))
		{
			COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ptext->m_nComponent);
			if( (pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == TRUE) && \
				(((ptext->m_bComment == TRUE)&&(pcomp->m_bCommentOn == TRUE))||((ptext->m_bDesignator == TRUE)&&(pcomp->m_bNameOn == TRUE))) )
			{
				ptext->DrawNotEditSelection(pDC, pV);
			}
		}
	}

	double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
	GetPcbCompRect(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4, index);

	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen arcpen;
	arcpen.CreatePen(PS_SOLID, 1, nEDA_PCB_LayerColor[SELECTIONS]);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	POINT point[5];
	int x, y;
	pView->Get_VPort_XY(&x, &y, fx1, fy1);
	point[0].x = x;
	point[0].y = y;
	pView->Get_VPort_XY(&x, &y, fx2, fy2);
	point[1].x = x;
	point[1].y = y;
	pView->Get_VPort_XY(&x, &y, fx3, fy3);
	point[2].x = x;
	point[2].y = y;
	pView->Get_VPort_XY(&x, &y, fx4, fy4);
	point[3].x = x;
	point[3].y = y;
	pView->Get_VPort_XY(&x, &y, fx1, fy1);
	point[4].x = x;
	point[4].y = y;

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

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
	pDC->SelectObject(oldbrush);
}

void COb_pcbcomp::GetPcbCompRect(double *fx1, double *fy1, double *fx2, double *fy2, double *fx3, double *fy3, double *fx4, double *fy4, int index)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	CTypedPtrArray <CObArray, COb_pcbarc*> arrayPcbArc;
	CTypedPtrArray <CObArray, COb_pcbtrack*> arrayPcbTrack;
	CTypedPtrArray <CObArray, COb_pcbpad*> arrayPcbPad;

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* ppcbarc = pDoc->m_arrayPcbArc.GetAt(i);
		if(ppcbarc->m_nComponent == index)
		{
			COb_pcbarc* ptemparc = new COb_pcbarc();
			ptemparc->Copy(ppcbarc);
			ptemparc->pDocument = pDoc;
			arrayPcbArc.Add(ptemparc);
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if(ptrack->m_nComponent == index)
		{
			COb_pcbtrack* ptemptrack = new COb_pcbtrack();
			ptemptrack->Copy(ptrack);
			ptemptrack->pDocument = pDoc;
			arrayPcbTrack.Add(ptemptrack);
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad->m_nComponent == index)
		{
			COb_pcbpad* ptemppad = new COb_pcbpad();
			ptemppad->Copy(ppad);
			ptemppad->pDocument = pDoc;
			arrayPcbPad.Add(ptemppad);
		}
	}

	//将图形元素“恢复”到 m_fRotation = 0
	BOOL mirror = FALSE;
	BOOL fliplayer = FALSE;
	double deltax, deltay;
	double fx0, fy0, fx, fy;
						
	fx0 = m_fX; fy0 = m_fY;
	deltax = m_fX - fx0;
	deltay = m_fY - fy0;
	double delta_angle = 0 - m_fRotation;

	for(int i=0; i<arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = arrayPcbTrack.GetAt(i);

		pDoc->Get_Angle_XY(&fx, &fy, ptrack->m_fX1 - fx0, ptrack->m_fY1 - fy0, delta_angle);
		ptrack->m_fX1 = fx + fx0;
		ptrack->m_fY1 = fy + fy0;
		pDoc->Get_Angle_XY(&fx, &fy, ptrack->m_fX2 - fx0, ptrack->m_fY2 - fy0, delta_angle);
		ptrack->m_fX2 = fx + fx0;
		ptrack->m_fY2 = fy + fy0;
	}
	for(int i=0; i<arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = arrayPcbArc.GetAt(i);

		pDoc->Get_Angle_XY(&fx, &fy, parc->m_fCx - fx0, parc->m_fCy - fy0, delta_angle);
		parc->m_fCx = fx + fx0;
		parc->m_fCy = fy + fy0;
		parc->m_fSangle += delta_angle;
		parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
		parc->m_fEangle += delta_angle;
		parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
	}
	for(int i=0; i<arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = arrayPcbPad.GetAt(i);

		pDoc->Get_Angle_XY(&fx, &fy, ppad->m_fCx - fx0, ppad->m_fCy - fy0, delta_angle);
		ppad->m_fCx = fx + fx0;
		ppad->m_fCy = fy + fy0;
		ppad->m_fRotation += delta_angle;
		ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
	}

	CRect rect;
	double fmaxx=0, fmaxy=0, fminx=99999, fminy=99999;
	
	for(int i=0; i<arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = arrayPcbTrack.GetAt(i);

		double fx1, fy1, fx2, fy2;
		fx1 = MIN(ptrack->m_fX1, ptrack->m_fX2);
		fy1 = MIN(ptrack->m_fY1, ptrack->m_fY2);
		fx2 = MAX(ptrack->m_fX1, ptrack->m_fX2);
		fy2 = MAX(ptrack->m_fY1, ptrack->m_fY2);
		fmaxx = MAX(fmaxx, fx2);
		fmaxy = MAX(fmaxy, fy2);
		fminx = MIN(fminx, fx1);
		fminy = MIN(fminy, fy1);
	}
	for(int i=0; i<arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* ppcbarc = arrayPcbArc.GetAt(i);
		ppcbarc->m_fSangle = pDoc->Get_Correct_Angle(ppcbarc->m_fSangle);
		ppcbarc->m_fEangle = pDoc->Get_Correct_Angle(ppcbarc->m_fEangle);
		if((ppcbarc->m_fSangle == ppcbarc->m_fEangle)||(abs(ppcbarc->m_fSangle - ppcbarc->m_fEangle) == 360))
		{
			fmaxx = MAX(fmaxx, ppcbarc->m_fCx + ppcbarc->m_fRadius);
			fmaxy = MAX(fmaxy, ppcbarc->m_fCy + ppcbarc->m_fRadius);
			fminx = MIN(fminx, ppcbarc->m_fCx - ppcbarc->m_fRadius);
			fminy = MIN(fminy, ppcbarc->m_fCy - ppcbarc->m_fRadius);
		}
		else
		{
			if(ppcbarc->m_fEangle > ppcbarc->m_fSangle)
			{
				if((0 >= ppcbarc->m_fSangle)&&(0 <= ppcbarc->m_fEangle)) fmaxx = MAX(fmaxx, ppcbarc->m_fCx + ppcbarc->m_fRadius);
				else fmaxx = MAX(fmaxx, ppcbarc->m_fCx);

				if((90 >= ppcbarc->m_fSangle)&&(90 <= ppcbarc->m_fEangle)) fmaxy = MAX(fmaxy, ppcbarc->m_fCy + ppcbarc->m_fRadius);
				else fmaxy = MAX(fmaxy, ppcbarc->m_fCy);
				
				if((180 >= ppcbarc->m_fSangle)&&(180 <= ppcbarc->m_fEangle)) fminx = MIN(fminx, ppcbarc->m_fCx - ppcbarc->m_fRadius);
				else fminx = MIN(fminx, ppcbarc->m_fCx);

				if((270 >= ppcbarc->m_fSangle)&&(270 <= ppcbarc->m_fEangle)) fminy = MIN(fminy, ppcbarc->m_fCy - ppcbarc->m_fRadius);
				else fminy = MIN(fminy, ppcbarc->m_fCy);
			}
			else
			{
				if((0 >= ppcbarc->m_fSangle)||(0 <= ppcbarc->m_fEangle)) fmaxx = MAX(fmaxx, ppcbarc->m_fCx + ppcbarc->m_fRadius);
				else  fmaxx = MAX(fmaxx, ppcbarc->m_fCx);
				
				if((90 >= ppcbarc->m_fSangle)||(90 <= ppcbarc->m_fEangle)) fmaxy = MAX(fmaxy, ppcbarc->m_fCy + ppcbarc->m_fRadius);
				else  fmaxy = MAX(fmaxy, ppcbarc->m_fCy);
				
				if((180 >= ppcbarc->m_fSangle)||(180 <= ppcbarc->m_fEangle)) fmaxx = MIN(fminx, ppcbarc->m_fCx - ppcbarc->m_fRadius);
				else  fminx = MIN(fminx, ppcbarc->m_fCx);
				
				if((270 >= ppcbarc->m_fSangle)||(270 <= ppcbarc->m_fEangle)) fminy = MIN(fminy, ppcbarc->m_fCy - ppcbarc->m_fRadius);
				else  fminy = MIN(fminy, ppcbarc->m_fCy);
			}
		}
	}
	for(int i=0; i<arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = arrayPcbPad.GetAt(i);
		fmaxx = MAX(fmaxx, ppad->m_fCx + (abs(ppad->m_fSizeX[0]*cos(ppad->m_fRotation*fEDA_pi/180)) + abs(ppad->m_fSizeY[0]*sin(ppad->m_fRotation*fEDA_pi/180)))/2);
		fmaxy = MAX(fmaxy, ppad->m_fCy + (abs(ppad->m_fSizeY[0]*cos(ppad->m_fRotation*fEDA_pi/180)) + abs(ppad->m_fSizeX[0]*sin(ppad->m_fRotation*fEDA_pi/180)))/2);
		fminx = MIN(fminx, ppad->m_fCx - (abs(ppad->m_fSizeX[0]*cos(ppad->m_fRotation*fEDA_pi/180)) + abs(ppad->m_fSizeY[0]*sin(ppad->m_fRotation*fEDA_pi/180)))/2);
		fminy = MIN(fminy, ppad->m_fCy - (abs(ppad->m_fSizeY[0]*cos(ppad->m_fRotation*fEDA_pi/180)) + abs(ppad->m_fSizeX[0]*sin(ppad->m_fRotation*fEDA_pi/180)))/2);
	}

	while (!arrayPcbArc.IsEmpty())
	{
		delete arrayPcbArc.GetAt(0);
        arrayPcbArc.RemoveAt(0);
	}
	while (!arrayPcbTrack.IsEmpty())
	{
		delete arrayPcbTrack.GetAt(0);
        arrayPcbTrack.RemoveAt(0);
	}
	while (!arrayPcbPad.IsEmpty())
	{
		delete arrayPcbPad.GetAt(0);
        arrayPcbPad.RemoveAt(0);
	}

	rect.left = (int)fminx;
	rect.right = (int)fmaxx;
	rect.bottom = (int)fminy;
	rect.top = (int)fmaxy;

	*fx1 = fminx;
	*fy1 = fminy;
	*fx2 = fminx;
	*fy2 = fmaxy;
	*fx3 = fmaxx;
	*fy3 = fmaxy;
	*fx4 = fmaxx;
	*fy4 = fminy;
	
	pDoc->Get_Angle_XY(&fx, &fy, *fx1 - m_fX, *fy1 - m_fY, m_fRotation);
	*fx1 = fx + m_fX;
	*fy1 = fy + m_fY;
	pDoc->Get_Angle_XY(&fx, &fy, *fx2 - m_fX, *fy2 - m_fY, m_fRotation);
	*fx2 = fx + m_fX;
	*fy2 = fy + m_fY;
	pDoc->Get_Angle_XY(&fx, &fy, *fx3 - m_fX, *fy3 - m_fY, m_fRotation);
	*fx3 = fx + m_fX;
	*fy3 = fy + m_fY;
	pDoc->Get_Angle_XY(&fx, &fy, *fx4 - m_fX, *fy4 - m_fY, m_fRotation);
	*fx4 = fx + m_fX;
	*fy4 = fy + m_fY;
}

CString COb_pcbcomp::GetPcbCompDesignator(int index)
{
	CString str;
	str.Empty();
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ppcbtext = pDoc->m_arrayPcbText.GetAt(i);
		if((ppcbtext->m_nFlag == FLAG_NORMAL)&&(ppcbtext->m_nComponent == index)&&(ppcbtext->m_bDesignator == TRUE))
		{
			str = ppcbtext->m_csText;
			return str;
		}
	}

	return str;
}

CString COb_pcbcomp::GetPcbCompComment(int index)
{
	CString str;
	str.Empty();
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ppcbtext = pDoc->m_arrayPcbText.GetAt(i);
		if((ppcbtext->m_nFlag == FLAG_NORMAL)&&(ppcbtext->m_nComponent == index)&&(ppcbtext->m_bComment == TRUE))
		{
			str = ppcbtext->m_csText;
			return str;
		}
	}

	return str;
}

void COb_pcbcomp::Change(int index, CView* pV)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	COb_pcbtext* pIDtext = NULL;
	int nIDindex;
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		pIDtext = pDoc->m_arrayPcbText.GetAt(i);
		if((pIDtext->m_nFlag == FLAG_NORMAL)&&(pIDtext->m_nComponent == index)&&(pIDtext->m_bDesignator == TRUE))
		{
			nIDindex = i;
			break;
		}
	}
	
	COb_pcbtext* pCtext = NULL;
	int nCindex;
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		pCtext = pDoc->m_arrayPcbText.GetAt(i);
		if((pCtext->m_nFlag == FLAG_NORMAL)&&(pCtext->m_nComponent == index)&&(pCtext->m_bComment == TRUE))
		{
			nCindex = i;
			break;
		}
	}

	double fX_bak = m_fX;
	double fY_bak = m_fY;
	double rotation_bak = m_fRotation;
	int layer_bak = m_nLayer;	
	
	CDlg_PCBEditComp dlg;
	if(pIDtext != NULL)
	{
		dlg.m_csEdit_ID = pIDtext->m_csText;
		dlg.m_csEdit_ID_Height = pDoc->Convert_XY_To_String(pIDtext->m_fHeight);
		dlg.m_csEdit_ID_Width = pDoc->Convert_XY_To_String(pIDtext->m_fWidth);
		
		for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
		{
			dlg.arrayStringLayer.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
		}
		for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
		{
			if(pIDtext->m_nLayer == pDoc->m_arrayPcbLayerShow.GetAt(i))
			{
				dlg.m_nCombo_ID_Layer = i;
				break;
			}
		}
		
		dlg.m_fEdit_ID_Rotation = pIDtext->m_fRotation;
		dlg.m_csEdit_ID_X = pDoc->Convert_XY_To_String(pIDtext->m_fSx  - pDoc->m_cPCB_Sheet.m_fOriginX);
		dlg.m_csEdit_ID_Y = pDoc->Convert_XY_To_String(pIDtext->m_fSy  - pDoc->m_cPCB_Sheet.m_fOriginY);
		dlg.m_nCombo_ID_AutoPosition = m_nNamePosition;
		if(m_bNameOn == TRUE) dlg.m_bCheck_ID_Hide = FALSE;
		else dlg.m_bCheck_ID_Hide = TRUE;
		dlg.m_bCheck_ID_Mirror = pIDtext->m_bMirror;

		for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
		{
			if(pIDtext->m_csTTFontName == arrayEDA_SystemFont.GetAt(i)) 
			{
				dlg.nTT_ID_Font = i;
				break;
			}
		}
		dlg.nStroke_ID_Font = pIDtext->m_nStrokeFont;
		if(pIDtext->m_bUseTTFont == TRUE)
		{
			dlg.m_nRadio_ID_Font = 0;
			dlg.m_nCombo_ID_Font = dlg.nTT_ID_Font;
		}
		else 
		{
			dlg.m_nRadio_ID_Font = 1;
			dlg.m_nCombo_ID_Font = dlg.nStroke_ID_Font;
		}

		dlg.m_bCheck_ID_Bold = pIDtext->m_bBold;
		dlg.m_bCheck_ID_Italic = pIDtext->m_bItalic;
		dlg.m_bCheck_ID_Invert = pIDtext->m_bInverted;
		dlg.m_csEdit_ID_Invert = pDoc->Convert_XY_To_String(pIDtext->m_fInvertedBorder);
	}
	
	if(pCtext != NULL)
	{
		dlg.m_csEdit_C = pCtext->m_csText;
		dlg.m_csEdit_C_Height = pDoc->Convert_XY_To_String(pCtext->m_fHeight);
		dlg.m_csEdit_C_Width = pDoc->Convert_XY_To_String(pCtext->m_fWidth);
		
		//层
		for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
		{
			dlg.arrayStringLayer.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
		}
		for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
		{
			if(pCtext->m_nLayer == pDoc->m_arrayPcbLayerShow.GetAt(i))
			{
				dlg.m_nCombo_C_Layer = i;
				break;
			}
		}
		
		dlg.m_fEdit_C_Rotation = pCtext->m_fRotation;
		dlg.m_csEdit_C_X = pDoc->Convert_XY_To_String(pCtext->m_fSx  - pDoc->m_cPCB_Sheet.m_fOriginX);
		dlg.m_csEdit_C_Y = pDoc->Convert_XY_To_String(pCtext->m_fSy  - pDoc->m_cPCB_Sheet.m_fOriginY);
		dlg.m_nCombo_C_AutoPosition = m_nCommentPosition;
		if(m_bCommentOn == TRUE) dlg.m_bCheck_C_Hide = FALSE;
		else dlg.m_bCheck_C_Hide = TRUE;
		
		dlg.m_bCheck_C_Mirror = pCtext->m_bMirror;

		for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
		{
			if(pCtext->m_csTTFontName == arrayEDA_SystemFont.GetAt(i)) 
			{
				dlg.nTT_C_Font = i;
				break;
			}
		}
		dlg.nStroke_C_Font = pCtext->m_nStrokeFont;
		if(pCtext->m_bUseTTFont == TRUE)
		{
			dlg.m_nRadio_C_Font = 0;
			dlg.m_nCombo_C_Font = dlg.nTT_C_Font;
		}
		else 
		{
			dlg.m_nRadio_C_Font = 1;
			dlg.m_nCombo_C_Font = dlg.nStroke_C_Font;
		}


		dlg.m_bCheck_C_Bold = pCtext->m_bBold;
		dlg.m_bCheck_C_Italic = pCtext->m_bItalic;
		dlg.m_bCheck_C_Invert = pCtext->m_bInverted;
		dlg.m_csEdit_C_Invert = pDoc->Convert_XY_To_String(pCtext->m_fInvertedBorder);
	}

	if(m_nLayer == TOPLAYER) dlg.m_nComboCompLayer = 0;
	else dlg.m_nComboCompLayer = 1;
	dlg.m_fEditCompRotation = m_fRotation;
	dlg.m_csEditCompX = pDoc->Convert_XY_To_String(m_fX  - pDoc->m_cPCB_Sheet.m_fOriginX);
	dlg.m_csEditCompY = pDoc->Convert_XY_To_String(m_fY  - pDoc->m_cPCB_Sheet.m_fOriginY);
	dlg.m_bCheckLock = m_bLocked;
	dlg.m_bCheckLockString = m_bLockStrings;
	dlg.m_csEditPackage = m_csPattern;
	dlg.m_csEditLib = m_csSourceFootprintLibrary;
	dlg.m_csEditDescription = m_csFootprintDescription;

	if(dlg.DoModal() == IDOK)
	{
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = PCB_ELEMENT_COMP;
		pundo->m_nIndex = index;
		pundo->pcbcomp.Copy(this);
			
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					
		m_csPattern = dlg.m_csEditPackage;
		m_csSourceFootprintLibrary = dlg.m_csEditLib;
		m_csFootprintDescription = dlg.m_csEditDescription;
		
		if(dlg.m_nComboCompLayer == 0) m_nLayer = TOPLAYER;
		else m_nLayer = BOTTOMLAYER;
		m_fRotation = dlg.m_fEditCompRotation;
		m_fX = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditCompX) + pDoc->m_cPCB_Sheet.m_fOriginX;
		m_fY = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditCompY) + pDoc->m_cPCB_Sheet.m_fOriginY;
		m_bLocked = dlg.m_bCheckLock;
		m_bLockStrings = dlg.m_bCheckLockString;

		if(layer_bak != m_nLayer)
		{
			m_fRotation += 180;
			m_fRotation = pDoc->Get_Correct_Angle(m_fRotation);
		}
		
		if(pIDtext != NULL)
		{
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TEXT;
			pundo->m_nIndex = nIDindex;
			pundo->pcbtext.Copy(pIDtext);
				
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			
			pIDtext->m_csText = dlg.m_csEdit_ID;
			pIDtext->m_fHeight = pDoc->Convert_PCB_String_To_XY(dlg.m_csEdit_ID_Height);
			pIDtext->m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEdit_ID_Width);
			
			for(int i=0; i<=MULTILAYER; i++)
			{
				if(dlg.arrayStringLayer.GetAt(dlg.m_nCombo_ID_Layer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
				{
					pIDtext->m_nLayer = i;
					break;
				}
			}
			
			pIDtext->m_fRotation = dlg.m_fEdit_ID_Rotation;
			pIDtext->m_fSx = pDoc->Convert_PCB_String_To_XY(dlg.m_csEdit_ID_X) + pDoc->m_cPCB_Sheet.m_fOriginX;
			pIDtext->m_fSy = pDoc->Convert_PCB_String_To_XY(dlg.m_csEdit_ID_Y) + pDoc->m_cPCB_Sheet.m_fOriginY;
			m_nNamePosition = dlg.m_nCombo_ID_AutoPosition;
			if(dlg.m_bCheck_ID_Hide == FALSE) m_bNameOn = TRUE; 
			else m_bNameOn = FALSE;
			pIDtext->m_bMirror = dlg.m_bCheck_ID_Mirror;
			
			if(dlg.m_nRadio_ID_Font == 0)
			{
				pIDtext->m_bUseTTFont = TRUE;
				pIDtext->m_csTTFontName = arrayEDA_SystemFont.GetAt(dlg.m_nCombo_ID_Font);
			}
			else
			{
				pIDtext->m_bUseTTFont = FALSE;
				pIDtext->m_nStrokeFont = dlg.m_nCombo_ID_Font;
			}

			//True Type Font
			pIDtext->m_bBold = dlg.m_bCheck_ID_Bold;
			pIDtext->m_bItalic = dlg.m_bCheck_ID_Italic;
			pIDtext->m_bInverted = dlg.m_bCheck_ID_Invert;
			pIDtext->m_fInvertedBorder = pDoc->Convert_PCB_String_To_XY(dlg.m_csEdit_ID_Invert);
		}

		if(pCtext != NULL)
		{
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TEXT;
			pundo->m_nIndex = nCindex;
			pundo->pcbtext.Copy(pCtext);
				
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			
			pCtext->m_csText = dlg.m_csEdit_C;
			pCtext->m_fHeight = pDoc->Convert_PCB_String_To_XY(dlg.m_csEdit_C_Height);
			pCtext->m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEdit_C_Width);
			
			for(int i=0; i<=MULTILAYER; i++)
			{
				if(dlg.arrayStringLayer.GetAt(dlg.m_nCombo_C_Layer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
				{
					pCtext->m_nLayer = i;
					break;
				}
			}
			
			pCtext->m_fRotation = dlg.m_fEdit_C_Rotation;
			pCtext->m_fSx = pDoc->Convert_PCB_String_To_XY(dlg.m_csEdit_C_X) + pDoc->m_cPCB_Sheet.m_fOriginX;
			pCtext->m_fSy = pDoc->Convert_PCB_String_To_XY(dlg.m_csEdit_C_Y) + pDoc->m_cPCB_Sheet.m_fOriginY;
			m_nCommentPosition = dlg.m_nCombo_C_AutoPosition;
			if(dlg.m_bCheck_C_Hide == FALSE) m_bCommentOn = TRUE; 
			else m_bCommentOn = FALSE;
			pCtext->m_bMirror = dlg.m_bCheck_C_Mirror;
			
			if(dlg.m_nRadio_C_Font == 0)
			{
				pCtext->m_bUseTTFont = TRUE;
				pCtext->m_csTTFontName = arrayEDA_SystemFont.GetAt(dlg.m_nCombo_C_Font);
			}
			else
			{
				pCtext->m_bUseTTFont = FALSE;
				pCtext->m_nStrokeFont = dlg.m_nCombo_C_Font;
			}

			pCtext->m_bBold = dlg.m_bCheck_C_Bold;
			pCtext->m_bItalic = dlg.m_bCheck_C_Italic;
			pCtext->m_bInverted = dlg.m_bCheck_C_Invert;
			pCtext->m_fInvertedBorder = pDoc->Convert_PCB_String_To_XY(dlg.m_csEdit_C_Invert);
		}

		if((fX_bak != m_fX) || (fY_bak != m_fY) || (rotation_bak != m_fRotation) || (layer_bak != m_nLayer))
		{
			BOOL mirror = FALSE;
			BOOL fliplayer = FALSE;
			double deltax, deltay;
			double fx0, fy0, fx, fy, sangle, eangle;
						
			double to_zero = 0 - rotation_bak;
			double delta_angle = m_fRotation;// - rotation_bak;

			fx0 = fX_bak; fy0 = fY_bak;
			deltax = m_fX - fx0;
			deltay = m_fY - fy0;

			if(layer_bak != m_nLayer)
			{
				mirror = TRUE;
				fliplayer = TRUE;
			}

			for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
				if(parc->m_nComponent == index)
				{
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = i;
					pundo->pcbarc.Copy(parc);
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					if(mirror == TRUE)
					{
						pDoc->Get_Angle_XY(&fx, &fy, parc->m_fCx - fx0, parc->m_fCy - fy0, to_zero);
						parc->m_fCx = fx + fx0;
						parc->m_fCy = fy + fy0;
						parc->m_fSangle += to_zero;
						parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
						parc->m_fEangle += to_zero;
						parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
						
						pDoc->Get_Rotate_XY(&fx, &fy, parc->m_fCx - fx0, parc->m_fCy - fy0, 180, TRUE);
						parc->m_fCx = fx + fx0 + deltax;
						parc->m_fCy = fy + fy0 + deltay;

						pDoc->Get_Rotate_Angle(&sangle, &eangle, parc->m_fSangle, parc->m_fEangle, 180, TRUE);

						sangle = pDoc->Get_Correct_Angle(sangle);
						parc->m_fSangle = sangle;
						eangle = pDoc->Get_Correct_Angle(eangle);
						parc->m_fEangle = eangle;

						pDoc->Get_Angle_XY(&fx, &fy, parc->m_fCx - m_fX, parc->m_fCy - m_fY, delta_angle);
						parc->m_fCx = fx + m_fX;
						parc->m_fCy = fy + m_fY;
						parc->m_fSangle += delta_angle;
						parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
						parc->m_fEangle += delta_angle;
						parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);	
					}
					else
					{
						delta_angle = m_fRotation - rotation_bak;

						pDoc->Get_Angle_XY(&fx, &fy, parc->m_fCx - fx0, parc->m_fCy - fy0, delta_angle);
						parc->m_fCx = fx + fx0;
						parc->m_fCy = fy + fy0;
						parc->m_fSangle += delta_angle;
						parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
						parc->m_fEangle += delta_angle;
						parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);

						pDoc->Get_Rotate_XY(&fx, &fy, parc->m_fCx - fx0, parc->m_fCy - fy0, 0, FALSE);
						parc->m_fCx = fx + fx0 + deltax;
						parc->m_fCy = fy + fy0 + deltay;
					}
					
					if(fliplayer == TRUE)
					{
						if(parc->m_nLayer == TOPLAYER) parc->m_nLayer = BOTTOMLAYER;
						else if(parc->m_nLayer == BOTTOMLAYER) parc->m_nLayer = TOPLAYER;
						else if(parc->m_nLayer == TOPOVERLAY) parc->m_nLayer = BOTTOMOVERLAY;
						else if(parc->m_nLayer == BOTTOMOVERLAY) parc->m_nLayer = TOPOVERLAY;
						else if(parc->m_nLayer == TOPPASTE) parc->m_nLayer = BOTTOMPASTE;
						else if(parc->m_nLayer == BOTTOMPASTE) parc->m_nLayer = TOPPASTE;
						else if(parc->m_nLayer == TOPSOLDER) parc->m_nLayer = BOTTOMSOLDER;
						else if(parc->m_nLayer == BOTTOMSOLDER) parc->m_nLayer = TOPSOLDER;
					}
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
				if(ptrack->m_nComponent == index)
				{
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = i;
					pundo->pcbtrack.Copy(ptrack);
					pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					if(mirror == TRUE)
					{
						pDoc->Get_Angle_XY(&fx, &fy, ptrack->m_fX1 - fx0, ptrack->m_fY1 - fy0, to_zero);
						ptrack->m_fX1 = fx + fx0;
						ptrack->m_fY1 = fy + fy0;
						pDoc->Get_Angle_XY(&fx, &fy, ptrack->m_fX2 - fx0, ptrack->m_fY2 - fy0, to_zero);
						ptrack->m_fX2 = fx + fx0;
						ptrack->m_fY2 = fy + fy0;

						pDoc->Get_Rotate_XY(&fx, &fy, ptrack->m_fX1 - fx0, ptrack->m_fY1 - fy0, 180, mirror);
						ptrack->m_fX1 = fx + fx0 + deltax;
						ptrack->m_fY1 = fy + fy0 + deltay;
						pDoc->Get_Rotate_XY(&fx, &fy, ptrack->m_fX2 - fx0, ptrack->m_fY2 - fy0, 180, mirror);
						ptrack->m_fX2 = fx + fx0 + deltax;
						ptrack->m_fY2 = fy + fy0 + deltay;

						pDoc->Get_Angle_XY(&fx, &fy, ptrack->m_fX1 - m_fX, ptrack->m_fY1 - m_fY, delta_angle);
						ptrack->m_fX1 = fx + m_fX;
						ptrack->m_fY1 = fy + m_fY;
						pDoc->Get_Angle_XY(&fx, &fy, ptrack->m_fX2 - m_fX, ptrack->m_fY2 - m_fY, delta_angle);
						ptrack->m_fX2 = fx + m_fX;
						ptrack->m_fY2 = fy + m_fY;
					}
					else
					{
						delta_angle = m_fRotation - rotation_bak;

						pDoc->Get_Angle_XY(&fx, &fy, ptrack->m_fX1 - fx0, ptrack->m_fY1 - fy0, delta_angle);
						ptrack->m_fX1 = fx + fx0;
						ptrack->m_fY1 = fy + fy0;
						pDoc->Get_Angle_XY(&fx, &fy, ptrack->m_fX2 - fx0, ptrack->m_fY2 - fy0, delta_angle);
						ptrack->m_fX2 = fx + fx0;
						ptrack->m_fY2 = fy + fy0;
						
						pDoc->Get_Rotate_XY(&fx, &fy, ptrack->m_fX1 - fx0, ptrack->m_fY1 - fy0, 0, FALSE);
						ptrack->m_fX1 = fx + fx0 + deltax;
						ptrack->m_fY1 = fy + fy0 + deltay;
						pDoc->Get_Rotate_XY(&fx, &fy, ptrack->m_fX2 - fx0, ptrack->m_fY2 - fy0, 0, FALSE);
						ptrack->m_fX2 = fx + fx0 + deltax;
						ptrack->m_fY2 = fy + fy0 + deltay;
					}
					
					if(fliplayer == TRUE)
					{
						if(ptrack->m_nLayer == TOPLAYER) ptrack->m_nLayer = BOTTOMLAYER;
						else if(ptrack->m_nLayer == BOTTOMLAYER) ptrack->m_nLayer = TOPLAYER;
						else if(ptrack->m_nLayer == TOPOVERLAY) ptrack->m_nLayer = BOTTOMOVERLAY;
						else if(ptrack->m_nLayer == BOTTOMOVERLAY) ptrack->m_nLayer = TOPOVERLAY;
						else if(ptrack->m_nLayer == TOPPASTE) ptrack->m_nLayer = BOTTOMPASTE;
						else if(ptrack->m_nLayer == BOTTOMPASTE) ptrack->m_nLayer = TOPPASTE;
						else if(ptrack->m_nLayer == TOPSOLDER) ptrack->m_nLayer = BOTTOMSOLDER;
						else if(ptrack->m_nLayer == BOTTOMSOLDER) ptrack->m_nLayer = TOPSOLDER;
					}
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
				if(ppad->m_nComponent == index)
				{
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_PAD;
					pundo->m_nIndex = i;
					pundo->pcbpad.Copy(ppad);
					pundo->pcbpad.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					if(mirror == TRUE)
					{
						pDoc->Get_Angle_XY(&fx, &fy, ppad->m_fCx - fx0, ppad->m_fCy - fy0, to_zero);
						ppad->m_fCx = fx + fx0;
						ppad->m_fCy = fy + fy0;
						
						ppad->m_fRotation += to_zero;
						ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);					
						
						pDoc->Get_Rotate_XY(&fx, &fy, ppad->m_fCx - fx0, ppad->m_fCy - fy0, 180, mirror);
						ppad->m_fCx = fx + fx0 + deltax;
						ppad->m_fCy = fy + fy0 + deltay;
										
						double fangle;
						pDoc->Get_Rotate_Angle(&fangle, &fangle, ppad->m_fRotation, ppad->m_fRotation, 180, mirror);

						ppad->m_fRotation = fangle;
						ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
						
						pDoc->Get_Angle_XY(&fx, &fy, ppad->m_fCx - m_fX, ppad->m_fCy - m_fY, delta_angle);
						ppad->m_fCx = fx + m_fX;
						ppad->m_fCy = fy + m_fY;
						
						ppad->m_fRotation += delta_angle;
						ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
					}
					else
					{
						delta_angle = m_fRotation - rotation_bak;

						pDoc->Get_Angle_XY(&fx, &fy, ppad->m_fCx - fx0, ppad->m_fCy - fy0, delta_angle);
						ppad->m_fCx = fx + fx0;
						ppad->m_fCy = fy + fy0;
						
						ppad->m_fRotation += delta_angle;
						ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
						
						pDoc->Get_Rotate_XY(&fx, &fy, ppad->m_fCx - fx0, ppad->m_fCy - fy0, 0, FALSE);
						ppad->m_fCx = fx + fx0 + deltax;
						ppad->m_fCy = fy + fy0 + deltay;
					}

					if(fliplayer == TRUE)
					{
						if(ppad->m_nLayer == TOPLAYER) ppad->m_nLayer = BOTTOMLAYER;
						else if(ppad->m_nLayer == BOTTOMLAYER) ppad->m_nLayer = TOPLAYER;
						else if(ppad->m_nLayer == TOPOVERLAY) ppad->m_nLayer = BOTTOMOVERLAY;
						else if(ppad->m_nLayer == BOTTOMOVERLAY) ppad->m_nLayer = TOPOVERLAY;
						else if(ppad->m_nLayer == TOPPASTE) ppad->m_nLayer = BOTTOMPASTE;
						else if(ppad->m_nLayer == BOTTOMPASTE) ppad->m_nLayer = TOPPASTE;
						else if(ppad->m_nLayer == TOPSOLDER) ppad->m_nLayer = BOTTOMSOLDER;
						else if(ppad->m_nLayer == BOTTOMSOLDER) ppad->m_nLayer = TOPSOLDER;
					}
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
				if(pfill->m_nComponent == index)
				{
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = i;
					pundo->pcbfill.Copy(pfill);
					pundo->pcbfill.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					double dx, dy;
					double f_mx, f_my;
					double fxmin, fymin, fxmax, fymax;
					if(mirror == TRUE)
					{
						dx = (pfill->m_fX2 - pfill->m_fX1)/2;
						dy = (pfill->m_fY2 - pfill->m_fY1)/2;

						f_mx = (pfill->m_fX1 + pfill->m_fX2)/2;
						f_my = (pfill->m_fY1 + pfill->m_fY2)/2;

						pDoc->Get_Angle_XY(&fx, &fy, f_mx - fx0, f_my - fy0, to_zero);
						f_mx = fx + fx0;
						f_my = fy + fy0;
						pfill->m_fX1 = f_mx - dx;
						pfill->m_fY1 = f_my - dy;
						pfill->m_fX2 = f_mx + dx;
						pfill->m_fY2 = f_my + dy;

						pfill->m_fRotation += to_zero;
						pfill->m_fRotation = pDoc->Get_Correct_Angle(pfill->m_fRotation);
	
						fxmin = MIN(pfill->m_fX1, pfill->m_fX2);
						fymin = MIN(pfill->m_fY1, pfill->m_fY2);
						fxmax = MAX(pfill->m_fX1, pfill->m_fX2);
						fymax = MAX(pfill->m_fY1, pfill->m_fY2);
						pfill->m_fX1 = fxmin;  pfill->m_fY1 = fymin;
						pfill->m_fX2 = fxmax;  pfill->m_fY2 = fymax;					
						
						pDoc->Get_Rotate_XY(&fx, &fy, pfill->m_fX1 - fx0, pfill->m_fY1 - fy0, 180, mirror);
						pfill->m_fX1 = fx + fx0 + deltax;
						pfill->m_fY1 = fy + fy0 + deltay;
						pDoc->Get_Rotate_XY(&fx, &fy, pfill->m_fX2 - fx0, pfill->m_fY2 - fy0, 180, mirror);
						pfill->m_fX2 = fx + fx0 + deltax;
						pfill->m_fY2 = fy + fy0 + deltay;

						pDoc->Get_Rotate_Angle(&sangle, &eangle, pfill->m_fRotation, pfill->m_fRotation, 0, mirror);
						
						sangle = pDoc->Get_Correct_Angle(sangle);					
						pfill->m_fRotation = sangle;
						
						dx = (pfill->m_fX2 - pfill->m_fX1)/2;
						dy = (pfill->m_fY2 - pfill->m_fY1)/2;

						f_mx = (pfill->m_fX1 + pfill->m_fX2)/2;
						f_my = (pfill->m_fY1 + pfill->m_fY2)/2;

						pDoc->Get_Angle_XY(&fx, &fy, f_mx - m_fX, f_my - m_fY, delta_angle);
						f_mx = fx + m_fX;
						f_my = fy + m_fY;
						pfill->m_fX1 = f_mx - dx;
						pfill->m_fY1 = f_my - dy;
						pfill->m_fX2 = f_mx + dx;
						pfill->m_fY2 = f_my + dy;

						pfill->m_fRotation += delta_angle;
						pfill->m_fRotation = pDoc->Get_Correct_Angle(pfill->m_fRotation);
					}
					else
					{
						delta_angle = m_fRotation - rotation_bak;

						dx = (pfill->m_fX2 - pfill->m_fX1)/2;
						dy = (pfill->m_fY2 - pfill->m_fY1)/2;

						f_mx = (pfill->m_fX1 + pfill->m_fX2)/2;
						f_my = (pfill->m_fY1 + pfill->m_fY2)/2;

						pDoc->Get_Angle_XY(&fx, &fy, f_mx - fx0, f_my - fy0, delta_angle);
						f_mx = fx + fx0;
						f_my = fy + fy0;
						pfill->m_fX1 = f_mx - dx;
						pfill->m_fY1 = f_my - dy;
						pfill->m_fX2 = f_mx + dx;
						pfill->m_fY2 = f_my + dy;

						pfill->m_fRotation += delta_angle;
						pfill->m_fRotation = pDoc->Get_Correct_Angle(pfill->m_fRotation);

						pDoc->Get_Rotate_XY(&fx, &fy, pfill->m_fX1 - fx0, pfill->m_fY1 - fy0, 0, FALSE);
						pfill->m_fX1 = fx + fx0 + deltax;
						pfill->m_fY1 = fy + fy0 + deltay;
						pDoc->Get_Rotate_XY(&fx, &fy, pfill->m_fX2 - fx0, pfill->m_fY2 - fy0, 0, FALSE);
						pfill->m_fX2 = fx + fx0 + deltax;
						pfill->m_fY2 = fy + fy0 + deltay;
					}

					fxmin = MIN(pfill->m_fX1, pfill->m_fX2);
					fymin = MIN(pfill->m_fY1, pfill->m_fY2);
					fxmax = MAX(pfill->m_fX1, pfill->m_fX2);
					fymax = MAX(pfill->m_fY1, pfill->m_fY2);
					pfill->m_fX1 = fxmin;  pfill->m_fY1 = fymin;
					pfill->m_fX2 = fxmax;  pfill->m_fY2 = fymax;

					if(fliplayer == TRUE)
					{
						if(pfill->m_nLayer == TOPLAYER) pfill->m_nLayer = BOTTOMLAYER;
						else if(pfill->m_nLayer == BOTTOMLAYER) pfill->m_nLayer = TOPLAYER;
						else if(pfill->m_nLayer == TOPOVERLAY) pfill->m_nLayer = BOTTOMOVERLAY;
						else if(pfill->m_nLayer == BOTTOMOVERLAY) pfill->m_nLayer = TOPOVERLAY;
						else if(pfill->m_nLayer == TOPPASTE) pfill->m_nLayer = BOTTOMPASTE;
						else if(pfill->m_nLayer == BOTTOMPASTE) pfill->m_nLayer = TOPPASTE;
						else if(pfill->m_nLayer == TOPSOLDER) pfill->m_nLayer = BOTTOMSOLDER;
						else if(pfill->m_nLayer == BOTTOMSOLDER) pfill->m_nLayer = TOPSOLDER;
					}
				}
			}

			for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
				if(pvia->m_nComponent == index)
				{
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = i;
					pundo->pcbvia.Copy(pvia);
					pundo->pcbvia.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					if(mirror == TRUE)
					{
						pDoc->Get_Angle_XY(&fx, &fy, pvia->m_fCx - fx0, pvia->m_fCy - fy0, to_zero);
						pvia->m_fCx = fx + fx0;
						pvia->m_fCy = fy + fy0;

						pDoc->Get_Rotate_XY(&fx, &fy, pvia->m_fCx - fx0, pvia->m_fCy - fy0, 180, mirror);
						pvia->m_fCx = fx + fx0 + deltax;
						pvia->m_fCy = fy + fy0 + deltay;
						
						pDoc->Get_Angle_XY(&fx, &fy, pvia->m_fCx - m_fX, pvia->m_fCy - m_fY, delta_angle);
						pvia->m_fCx = fx + m_fX;
						pvia->m_fCy = fy + m_fY;
					}
					else
					{
						delta_angle = m_fRotation - rotation_bak;

						pDoc->Get_Angle_XY(&fx, &fy, pvia->m_fCx - fx0, pvia->m_fCy - fy0, delta_angle);
						pvia->m_fCx = fx + fx0;
						pvia->m_fCy = fy + fy0;
						
						pDoc->Get_Rotate_XY(&fx, &fy, pvia->m_fCx - fx0, pvia->m_fCy - fy0, 0, FALSE);
						pvia->m_fCx = fx + fx0 + deltax;
						pvia->m_fCy = fy + fy0 + deltay;
					}
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
				if(ptext->m_nComponent == index)
				{
					//----------- Undo -----------------------------------------------
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TEXT;
					pundo->m_nIndex = i;
					pundo->pcbtext.Copy(ptext);
					pundo->pcbtext.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					//----------------------------------------------------------------
					if(mirror == TRUE)
					{
						pDoc->Get_Angle_XY(&fx, &fy, ptext->m_fSx - fx0, ptext->m_fSy - fy0, to_zero);
						ptext->m_fSx = fx + fx0;
						ptext->m_fSy = fy + fy0;
						
						ptext->m_fRotation += to_zero;
						ptext->m_fRotation = pDoc->Get_Correct_Angle(ptext->m_fRotation);					
						
						pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - fx0, ptext->m_fSy - fy0, 180, mirror);
						ptext->m_fSx = fx + fx0 + deltax;
						ptext->m_fSy = fy + fy0 + deltay;

						if(mirror == TRUE)
						{
							if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
							else ptext->m_bMirror = TRUE;

							ptext->m_fRotation += 180 - 2*ptext->m_fRotation;
						}

						pDoc->Get_Angle_XY(&fx, &fy, ptext->m_fSx - m_fX, ptext->m_fSy - m_fY, delta_angle);
						ptext->m_fSx = fx + m_fX;
						ptext->m_fSy = fy + m_fY;
						
						ptext->m_fRotation += delta_angle;
						ptext->m_fRotation = pDoc->Get_Correct_Angle(ptext->m_fRotation);
					}
					else
					{
						delta_angle = m_fRotation - rotation_bak;
						pDoc->Get_Angle_XY(&fx, &fy, ptext->m_fSx - fx0, ptext->m_fSy - fy0, delta_angle);
						ptext->m_fSx = fx + fx0;
						ptext->m_fSy = fy + fy0;
						
						ptext->m_fRotation += delta_angle;
						ptext->m_fRotation = pDoc->Get_Correct_Angle(ptext->m_fRotation);
						
						pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - fx0, ptext->m_fSy - fy0, 0, FALSE);
						ptext->m_fSx = fx + fx0 + deltax;
						ptext->m_fSy = fy + fy0 + deltay;
					}

					if(fliplayer == TRUE)
					{
						if(ptext->m_nLayer == TOPLAYER) ptext->m_nLayer = BOTTOMLAYER;
						else if(ptext->m_nLayer == BOTTOMLAYER) ptext->m_nLayer = TOPLAYER;
						else if(ptext->m_nLayer == TOPOVERLAY) ptext->m_nLayer = BOTTOMOVERLAY;
						else if(ptext->m_nLayer == BOTTOMOVERLAY) ptext->m_nLayer = TOPOVERLAY;
						else if(ptext->m_nLayer == TOPPASTE) ptext->m_nLayer = BOTTOMPASTE;
						else if(ptext->m_nLayer == BOTTOMPASTE) ptext->m_nLayer = TOPPASTE;
						else if(ptext->m_nLayer == TOPSOLDER) ptext->m_nLayer = BOTTOMSOLDER;
						else if(ptext->m_nLayer == BOTTOMSOLDER) ptext->m_nLayer = TOPSOLDER;
					}
					ptext->UpdateStringBorder(pV, TRUE);
				}
			}

		}

		//------------------------------------------------------------------------------------------------------
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		//------------------------------------------------------------------------------------------------------
	}
}

void COb_pcbcomp::Copy(COb_pcbcomp* pcomp)
{
	m_fX = pcomp->m_fX;
	m_fY = pcomp->m_fY;
	m_fRotation = pcomp->m_fRotation;
	m_fHeight = pcomp->m_fHeight;

	m_nLayer = pcomp->m_nLayer;

	m_bNameOn = pcomp->m_bNameOn;
	m_nNamePosition = pcomp->m_nNamePosition;
	m_bCommentOn = pcomp->m_bCommentOn;
	m_nCommentPosition = pcomp->m_nCommentPosition;

	m_bPrimitiveLock = pcomp->m_bPrimitiveLock;
	m_bLocked = pcomp->m_bLocked;
	m_bLockStrings = pcomp->m_bLockStrings;

	//swapping options
	m_bEnablePinSwapping = pcomp->m_bEnablePinSwapping;
	m_bEnablePartSwapping = pcomp->m_bEnablePartSwapping;

	m_csPattern = pcomp->m_csPattern;
	m_csSourceFootprintLibrary = pcomp->m_csSourceFootprintLibrary;
	m_csFootprintDescription = pcomp->m_csFootprintDescription;

	m_bPolygonOutline = pcomp->m_bPolygonOutline;
	m_bUserRouted = pcomp->m_bUserRouted;
	m_nUnionIndex = pcomp->m_nUnionIndex;

	
	m_nGroupNum = pcomp->m_nGroupNum;
	m_nCount = pcomp->m_nCount;
	m_nComponentKind = pcomp->m_nComponentKind;
	m_nComponentKindVersion2 = pcomp->m_nComponentKindVersion2;
	m_nChannelOffset = pcomp->m_nChannelOffset;
	m_csSourceDesignator = pcomp->m_csSourceDesignator;
	m_csSourceUniqueID = pcomp->m_csSourceUniqueID;
	m_csSourceHierarchicalPath = pcomp->m_csSourceHierarchicalPath;
	m_csSourceComponentLibrary = pcomp->m_csSourceComponentLibrary;
	m_csSourceLibReference = pcomp->m_csSourceLibReference;
	m_csSourceDescription = pcomp->m_csSourceDescription;
	m_csDefaultPcb3dModel = pcomp->m_csDefaultPcb3dModel;

	m_bSelection = FALSE; //pcomp->m_bSelection;
	m_nFlag = pcomp->m_nFlag;

	pDocument = pcomp->pDocument;
}

void COb_pcbcomp::Move(CClientDC* pDC, CView* pV, int index)
{
	int ch1;
	double fx0, fy0, fx1, fy1, deltax, deltay;
	int flag;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动元件: <空格>旋转, <L>换层, <X>镜像, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	CTypedPtrArray <CObArray, COb_pcbarc*> arrayTempArc;
	CTypedPtrArray <CObArray, COb_pcbtrack*> arrayTempTrack;
	CTypedPtrArray <CObArray, COb_pcbtext*> arrayTempText;
	CTypedPtrArray <CObArray, COb_pcbfill*> arrayTempFill;
	CTypedPtrArray <CObArray, COb_pcbregion*> arrayTempRegion;	
	CTypedPtrArray <CObArray, COb_pcbpad*> arrayTempPad;
	CTypedPtrArray <CObArray, COb_pcbvia*> arrayTempVia;
	CTypedPtrArray <CObArray, COb_pcbarc*> arrayTempArc_bak;
	CTypedPtrArray <CObArray, COb_pcbtrack*> arrayTempTrack_bak;
	CTypedPtrArray <CObArray, COb_pcbtext*> arrayTempText_bak;
	CTypedPtrArray <CObArray, COb_pcbfill*> arrayTempFill_bak;
	CTypedPtrArray <CObArray, COb_pcbregion*> arrayTempRegion_bak;
	CTypedPtrArray <CObArray, COb_pcbpad*> arrayTempPad_bak;
	CTypedPtrArray <CObArray, COb_pcbvia*> arrayTempVia_bak;

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag == FLAG_NORMAL)&&(parc->m_nComponent == index))
		{
			arrayTempArc.Add(parc);
			
			COb_pcbarc* ptemparc_bak = new COb_pcbarc();
			ptemparc_bak->Copy(parc);
			arrayTempArc_bak.Add(ptemparc_bak);

			parc->m_nFlag = FLAG_PROCESSING;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag == FLAG_NORMAL)&&(ptrack->m_nComponent == index))
		{
			arrayTempTrack.Add(ptrack);
			
			COb_pcbtrack* ptemptrack_bak = new COb_pcbtrack();
			ptemptrack_bak->Copy(ptrack);
			arrayTempTrack_bak.Add(ptemptrack_bak);
			
			ptrack->m_nFlag = FLAG_PROCESSING;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag == FLAG_NORMAL)&&(pfill->m_nComponent == index))
		{
			arrayTempFill.Add(pfill);

			COb_pcbfill* ptempfill_bak = new COb_pcbfill();
			ptempfill_bak->Copy(pfill);
			arrayTempFill_bak.Add(ptempfill_bak);

			pfill->m_nFlag = FLAG_PROCESSING;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag == FLAG_NORMAL)&&(ptext->m_nComponent == index))
		{
			arrayTempText.Add(ptext);
			
			COb_pcbtext* ptemptext_bak = new COb_pcbtext();
			ptemptext_bak->Copy(ptext);
			arrayTempText_bak.Add(ptemptext_bak);

			ptext->m_nFlag = FLAG_PROCESSING;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag == FLAG_NORMAL)&&(pvia->m_nComponent == index))
		{
			arrayTempVia.Add(pvia);
			
			COb_pcbvia* ptempvia_bak = new COb_pcbvia();
			ptempvia_bak->Copy(pvia);
			arrayTempVia_bak.Add(ptempvia_bak);

			pvia->m_nFlag = FLAG_PROCESSING;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == index))
		{
			arrayTempPad.Add(ppad);

			COb_pcbpad* ptemppad_bak = new COb_pcbpad();
			ptemppad_bak->Copy(ppad);
			arrayTempPad_bak.Add(ptemppad_bak);

			ppad->m_nFlag = FLAG_PROCESSING;		
		}
	}

	pView->Refresh_Package_Ratsnest(index);

	m_nFlag = FLAG_PROCESSING;
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	pView->Invalidate();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	fx0 = m_fX;
	fy0 = m_fY;
	pView->m_fCurrentSnapX = fx0;
	pView->m_fCurrentSnapY = fy0;
	pView->Restore_LargeCrossXY();
	deltax = pView->m_fCurrentSnapX - fx0;
	deltay = pView->m_fCurrentSnapY - fy0;
	
	pView->OpenMouseCursor();
	

	pView->Write_Help(szHelp);
	flag = 0; 
	BOOL mirror = FALSE;
	BOOL fliplayer = FALSE;
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();

			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);
			}

			pView->DrawPcbConnections(pDC);
			pView->OpenMouseCursor();

			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				COb_pcbarc* parc_bak = arrayTempArc_bak.GetAt(i);
				parc->Copy(parc_bak);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				COb_pcbtrack* ptrack_bak = arrayTempTrack_bak.GetAt(i);
				ptrack->Copy(ptrack_bak);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				COb_pcbvia* pvia_bak = arrayTempVia_bak.GetAt(i);
				pvia->Copy(pvia_bak);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				COb_pcbfill* pfill_bak = arrayTempFill_bak.GetAt(i);
				pfill->Copy(pfill_bak);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				COb_pcbpad* ppad_bak = arrayTempPad_bak.GetAt(i);
				ppad->Copy(ppad_bak);
			}
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
				
				pView->DrawPcbConnections(pDC);
			}
			else  
			{
				pView->ElecHotPointsControl(pDC);

				pView->CloseMouseCursor();
				for(int i=0; i<arrayTempArc.GetCount(); i++) 
				{
					COb_pcbarc* parc = arrayTempArc.GetAt(i);
					parc->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempTrack.GetCount(); i++) 
				{
					COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
					ptrack->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempVia.GetCount(); i++) 
				{
					COb_pcbvia* pvia = arrayTempVia.GetAt(i);
					pvia->Draw(pDC, pV, 1);
				}
				for(int i=0; i<arrayTempFill.GetCount(); i++) 
				{
					COb_pcbfill* pfill = arrayTempFill.GetAt(i);
					pfill->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempPad.GetCount(); i++) 
				{
					COb_pcbpad* ppad = arrayTempPad.GetAt(i);
					ppad->Draw(pDC, pV, DRAW_DRAFT);

				}

				pView->DrawPcbConnections(pDC);
				pView->OpenMouseCursor();
			}

			fx1 = pView->m_fCurrentSnapX;
			fy1 = pView->m_fCurrentSnapY;
			deltax = fx1 - fx0;
			deltay = fy1 - fy0;

			for(int i=0; i<arrayTempArc.GetCount(); i++)
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				COb_pcbarc* parc_bak = arrayTempArc_bak.GetAt(i);

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, parc_bak->m_fCx - fx0, parc_bak->m_fCy - fy0, flag*90, mirror);
				parc->m_fCx = fx + fx0 + deltax;
				parc->m_fCy = fy + fy0 + deltay;

				double sangle, eangle;
				pDoc->Get_Rotate_Angle(&sangle, &eangle, parc_bak->m_fSangle, parc_bak->m_fEangle, flag*90, mirror);

				sangle = pDoc->Get_Correct_Angle(sangle);
				parc->m_fSangle = sangle;
				eangle = pDoc->Get_Correct_Angle(eangle);
				parc->m_fEangle = eangle;
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				COb_pcbtrack* ptrack_bak = arrayTempTrack_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX1 - fx0, ptrack_bak->m_fY1 - fy0, flag*90, mirror);
				ptrack->m_fX1 = fx + fx0 + deltax;
				ptrack->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX2 - fx0, ptrack_bak->m_fY2 - fy0, flag*90, mirror);
				ptrack->m_fX2 = fx + fx0 + deltax;
				ptrack->m_fY2 = fy + fy0 + deltay;				
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				COb_pcbvia* pvia_bak = arrayTempVia_bak.GetAt(i);
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pvia_bak->m_fCx - fx0, pvia_bak->m_fCy - fy0, flag*90, mirror);
				pvia->m_fCx = fx + fx0 + deltax;
				pvia->m_fCy = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				COb_pcbfill* pfill_bak = arrayTempFill_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX1 - fx0, pfill_bak->m_fY1 - fy0, flag*90, mirror);
				pfill->m_fX1 = fx + fx0 + deltax;
				pfill->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX2 - fx0, pfill_bak->m_fY2 - fy0, flag*90, mirror);
				pfill->m_fX2 = fx + fx0 + deltax;
				pfill->m_fY2 = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				COb_pcbpad* ppad_bak = arrayTempPad_bak.GetAt(i);
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppad_bak->m_fCx - fx0, ppad_bak->m_fCy - fy0, flag*90, mirror);
				ppad->m_fCx = fx + fx0 + deltax;
				ppad->m_fCy = fy + fy0 + deltay;
								
				double fangle;
				pDoc->Get_Rotate_Angle(&fangle, &fangle, ppad_bak->m_fRotation, ppad_bak->m_fRotation, flag*90, mirror);

				ppad->m_fRotation = fangle;
				ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
			}

			pView->Refresh_Package_Ratsnest(index);
			
			pView->CloseMouseCursor();
			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);
			}

			pView->DrawPcbConnections(pDC);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();

			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);
				//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
				//else ppad->DrawSingleLayerPad(pDC, pV, 1);
			}

			pView->DrawPcbConnections(pDC);
			pView->OpenMouseCursor();

			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_COMP;
			pundo->m_nIndex = index;
			pundo->pcbcomp.Copy(this);
			pundo->pcbcomp.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			
			m_fX = pView->m_fCurrentSnapX;
			m_fY = pView->m_fCurrentSnapY;

			double fangle;
			pDoc->Get_Rotate_Angle(&fangle, &fangle, m_fRotation, m_fRotation, flag*90, mirror);
			m_fRotation = fangle;
			m_fRotation = pDoc->Get_Correct_Angle(m_fRotation);

			fx1 = pView->m_fCurrentSnapX;
			fy1 = pView->m_fCurrentSnapY;
			deltax = fx1 - fx0;
			deltay = fy1 - fy0;

			for(int i=0; i<arrayTempArc.GetCount(); i++)
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				COb_pcbarc* parc_bak = arrayTempArc_bak.GetAt(i);

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, parc_bak->m_fCx - fx0, parc_bak->m_fCy - fy0, flag*90, mirror);
				parc->m_fCx = fx + fx0 + deltax;
				parc->m_fCy = fy + fy0 + deltay;

				double sangle, eangle;
				pDoc->Get_Rotate_Angle(&sangle, &eangle, parc_bak->m_fSangle, parc_bak->m_fEangle, flag*90, mirror);

				sangle = pDoc->Get_Correct_Angle(sangle);
				parc->m_fSangle = sangle;
				eangle = pDoc->Get_Correct_Angle(eangle);
				parc->m_fEangle = eangle;
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				COb_pcbtrack* ptrack_bak = arrayTempTrack_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX1 - fx0, ptrack_bak->m_fY1 - fy0, flag*90, mirror);
				ptrack->m_fX1 = fx + fx0 + deltax;
				ptrack->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX2 - fx0, ptrack_bak->m_fY2 - fy0, flag*90, mirror);
				ptrack->m_fX2 = fx + fx0 + deltax;
				ptrack->m_fY2 = fy + fy0 + deltay;				
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				COb_pcbvia* pvia_bak = arrayTempVia_bak.GetAt(i);
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pvia_bak->m_fCx - fx0, pvia_bak->m_fCy - fy0, flag*90, mirror);
				pvia->m_fCx = fx + fx0 + deltax;
				pvia->m_fCy = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				COb_pcbfill* pfill_bak = arrayTempFill_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX1 - fx0, pfill_bak->m_fY1 - fy0, flag*90, mirror);
				pfill->m_fX1 = fx + fx0 + deltax;
				pfill->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX2 - fx0, pfill_bak->m_fY2 - fy0, flag*90, mirror);
				pfill->m_fX2 = fx + fx0 + deltax;
				pfill->m_fY2 = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				COb_pcbpad* ppad_bak = arrayTempPad_bak.GetAt(i);
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppad_bak->m_fCx - fx0, ppad_bak->m_fCy - fy0, flag*90, mirror);
				ppad->m_fCx = fx + fx0 + deltax;
				ppad->m_fCy = fy + fy0 + deltay;
								
				double fangle;
				pDoc->Get_Rotate_Angle(&fangle, &fangle, ppad_bak->m_fRotation, ppad_bak->m_fRotation, flag*90, mirror);

				ppad->m_fRotation = fangle;
				ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
			}

			int tempindex;
			tempindex = 0;
			for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
				if(parc->m_nComponent == index)
				{
					COb_pcbarc* ptemparc_bak = arrayTempArc_bak.GetAt(tempindex);	tempindex++;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = i;
					pundo->pcbarc.Copy(ptemparc_bak);
					pundo->pcbarc.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					parc->m_nFlag = FLAG_NORMAL;
				}
			}
			tempindex = 0;
			for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
				if(ptrack->m_nComponent == index)
				{
					COb_pcbtrack* ptemptrack_bak = arrayTempTrack_bak.GetAt(tempindex);	tempindex++;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = i;
					pundo->pcbtrack.Copy(ptemptrack_bak);
					pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					ptrack->m_nFlag = FLAG_NORMAL;
				}
			}
			tempindex = 0;
			for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
				if(ppad->m_nComponent == index)
				{
					COb_pcbpad* ptemppad_bak = arrayTempPad_bak.GetAt(tempindex);	tempindex++;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_PAD;
					pundo->m_nIndex = i;
					pundo->pcbpad.Copy(ptemppad_bak);
					pundo->pcbpad.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					ppad->m_nFlag = FLAG_NORMAL;
				}
			}
			tempindex = 0;
			for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
				if(pfill->m_nComponent == index)
				{
					COb_pcbfill* ptempfill_bak = arrayTempFill_bak.GetAt(tempindex);	tempindex++;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = i;
					pundo->pcbfill.Copy(ptempfill_bak);
					pundo->pcbfill.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					pfill->m_nFlag = FLAG_NORMAL;
				}
			}
			tempindex = 0;
			for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
				if(pvia->m_nComponent == index)
				{
					COb_pcbvia* ptempvia_bak = arrayTempVia_bak.GetAt(tempindex);	tempindex++;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = i;
					pundo->pcbvia.Copy(ptempvia_bak);
					pundo->pcbvia.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

					pvia->m_nFlag = FLAG_NORMAL;
				}
			}
			
			double fcomp_x1, fcomp_y1, fcomp_x2, fcomp_y2, fcomp_x3, fcomp_y3, fcomp_x4, fcomp_y4;
			GetPcbCompRect(&fcomp_x1, &fcomp_y1, &fcomp_x2, &fcomp_y2, &fcomp_x3, &fcomp_y3, &fcomp_x4, &fcomp_y4, index);
			CRect rect;
			rect.left = MIN(MIN(fcomp_x1, fcomp_x2), MIN(fcomp_x3, fcomp_x4));
			rect.right = MAX(MAX(fcomp_x1, fcomp_x2), MAX(fcomp_x3, fcomp_x4));
			rect.bottom = MIN(MIN(fcomp_y1, fcomp_y2), MIN(fcomp_y3, fcomp_y4));
			rect.top = MAX(MAX(fcomp_y1, fcomp_y2), MAX(fcomp_y3, fcomp_y4));

			tempindex = 0;
			for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
				if(ptext->m_nComponent == index)
				{
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TEXT;
					pundo->m_nIndex = i;
					pundo->pcbtext.Copy(ptext);
					pundo->pcbtext.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					if(ptext->m_bDesignator == TRUE)
					{
						if(m_nNamePosition == MANUAL)
						{
							double fx, fy;
							pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - fx0, ptext->m_fSy - fy0, flag*90, mirror);
							ptext->m_fSx = fx + fx0 + deltax;
							ptext->m_fSy = fy + fy0 + deltay;
						
							double angle;
							pDoc->Get_Rotate_Angle(&angle, &angle, ptext->m_fRotation, ptext->m_fRotation, flag*90, FALSE);
							angle = pDoc->Get_Correct_Angle(angle);
							ptext->m_fRotation = angle;
						}
						else
						{
							if(m_nNamePosition == LEFTTOP)
							{
								ptext->m_fSx = rect.left;
								ptext->m_fSy = rect.top + 150;
							}
							else if(m_nNamePosition == LEFTCENTER)
							{
								ptext->m_fSx = rect.left - ptext->GetStringWidth(pV) - 50;
								ptext->m_fSy = rect.top - (rect.top - rect.bottom)/2 + 50;
							}
							else if(m_nNamePosition == LEFTBOTTOM)
							{
								ptext->m_fSx = rect.left;
								ptext->m_fSy = rect.bottom - 80;
							}
							else if(m_nNamePosition == CENTERTOP)
							{
								ptext->m_fSx = rect.left + (rect.right - rect.left)/2 - ptext->GetStringWidth(pV)/2;
								ptext->m_fSy = rect.top + 150;
							}
							else if(m_nNamePosition == CENTERCENTER)
							{
								ptext->m_fSx = rect.left + (rect.right - rect.left)/2 - ptext->GetStringWidth(pV)/2;
								ptext->m_fSy = rect.top - (rect.top - rect.bottom)/2 + 50;
							}
							else if(m_nNamePosition == CENTERBOTTOM)
							{
								ptext->m_fSx = rect.left + (rect.right - rect.left)/2 - ptext->GetStringWidth(pV)/2;
								ptext->m_fSy = rect.bottom - 80;
							}
							else if(m_nNamePosition == RIGHTTOP)
							{
								ptext->m_fSx = rect.right + 50;
								ptext->m_fSy = rect.top - 80;
							}
							else if(m_nNamePosition == RIGHTCENTER)
							{
								ptext->m_fSx = rect.right + 50;
								ptext->m_fSy = rect.top - (rect.top - rect.bottom)/2 + 50;
							}
							else //if(m_nNamePosition == RIGHTBOTTOM)
							{
								ptext->m_fSx = rect.right + 50;
								ptext->m_fSy = rect.bottom + 100;
							}
							ptext->m_fRotation = 0;
						}
					}
					else if(ptext->m_bComment == TRUE)
					{
						if(m_nCommentPosition == MANUAL)
						{
							double fx, fy;
							pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - fx0, ptext->m_fSy - fy0, flag*90, mirror);
							ptext->m_fSx = fx + fx0 + deltax;
							ptext->m_fSy = fy + fy0 + deltay;

							double angle;
							pDoc->Get_Rotate_Angle(&angle, &angle, ptext->m_fRotation, ptext->m_fRotation, flag*90, FALSE);
							angle = pDoc->Get_Correct_Angle(angle);
							ptext->m_fRotation = angle;
						}
						else
						{
							if(m_nNamePosition == LEFTTOP)
							{
								ptext->m_fSx = rect.left;
								ptext->m_fSy = rect.top + 50;
							}
							else if(m_nNamePosition == LEFTCENTER)
							{
								ptext->m_fSx = rect.left - ptext->GetStringWidth(pV) - 50;
								ptext->m_fSy = rect.top - (rect.top - rect.bottom)/2 - 50;
							}
							else if(m_nNamePosition == LEFTBOTTOM)
							{
								ptext->m_fSx = rect.left;
								ptext->m_fSy = rect.bottom - 180;
							}
							else if(m_nNamePosition == CENTERTOP)
							{
								ptext->m_fSx = rect.left + (rect.right - rect.left)/2 - ptext->GetStringWidth(pV)/2;
								ptext->m_fSy = rect.top + 50;
							}
							else if(m_nNamePosition == CENTERCENTER)
							{
								ptext->m_fSx = rect.left + (rect.right - rect.left)/2 - ptext->GetStringWidth(pV)/2;
								ptext->m_fSy = rect.top - (rect.top - rect.bottom)/2 - 50;
							}
							else if(m_nNamePosition == CENTERBOTTOM)
							{
								ptext->m_fSx = rect.left + (rect.right - rect.left)/2 - ptext->GetStringWidth(pV)/2;
								ptext->m_fSy = rect.bottom - 180;
							}
							else if(m_nNamePosition == RIGHTTOP)
							{
								ptext->m_fSx = rect.right + 50;
								ptext->m_fSy = rect.top - 180;
							}
							else if(m_nNamePosition == RIGHTCENTER)
							{
								ptext->m_fSx = rect.right + 50;
								ptext->m_fSy = rect.top - (rect.top - rect.bottom)/2 - 50;
							}
							else //if(m_nNamePosition == RIGHTBOTTOM)
							{
								ptext->m_fSx = rect.right + 50;
								ptext->m_fSy = rect.bottom;
							}
							ptext->m_fRotation = 0;
						}
					}
					else 
					{
						double fx, fy;
						pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - fx0, ptext->m_fSy - fy0, flag*90, mirror);
						ptext->m_fSx = fx + fx0 + deltax;
						ptext->m_fSy = fy + fy0 + deltay;

						double angle;
						pDoc->Get_Rotate_Angle(&angle, &angle, ptext->m_fRotation, ptext->m_fRotation, flag*90, FALSE);
						angle = pDoc->Get_Correct_Angle(angle);
						ptext->m_fRotation = angle;
					}
	
					if(mirror == TRUE)
					{
						if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
						else ptext->m_bMirror = TRUE;

						ptext->m_fRotation = 0 - ptext->m_fRotation;
						ptext->m_fRotation = pDoc->Get_Correct_Angle(ptext->m_fRotation);
					}

					if(fliplayer == TRUE)
					{
						if(ptext->m_nLayer == TOPLAYER) ptext->m_nLayer = BOTTOMLAYER;
						else if(ptext->m_nLayer == BOTTOMLAYER) ptext->m_nLayer = TOPLAYER;
						else if(ptext->m_nLayer == TOPOVERLAY) ptext->m_nLayer = BOTTOMOVERLAY;
						else if(ptext->m_nLayer == BOTTOMOVERLAY) ptext->m_nLayer = TOPOVERLAY;
						else if(ptext->m_nLayer == TOPPASTE) ptext->m_nLayer = BOTTOMPASTE;
						else if(ptext->m_nLayer == BOTTOMPASTE) ptext->m_nLayer = TOPPASTE;
						else if(ptext->m_nLayer == TOPSOLDER) ptext->m_nLayer = BOTTOMSOLDER;
						else if(ptext->m_nLayer == BOTTOMSOLDER) ptext->m_nLayer = TOPSOLDER;
					}

					ptext->UpdateStringBorder(pV, FALSE);
				}
			}

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			
			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);
				//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
				//else ppad->DrawSingleLayerPad(pDC, pV, 1);
			}

			pView->DrawPcbConnections(pDC);

			if(mirror == FALSE)
			{
				flag++;
				if(flag>3) flag=0;
			}
			else
			{
				flag--;
				if(flag < 0) flag = 3;
			}

			fx1 = pView->m_fCurrentSnapX;
			fy1 = pView->m_fCurrentSnapY;
			deltax = fx1 - fx0;
			deltay = fy1 - fy0;

			for(int i=0; i<arrayTempArc.GetCount(); i++)
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				COb_pcbarc* parc_bak = arrayTempArc_bak.GetAt(i);

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, parc_bak->m_fCx - fx0, parc_bak->m_fCy - fy0, flag*90, mirror);
				parc->m_fCx = fx + fx0 + deltax;
				parc->m_fCy = fy + fy0 + deltay;

				double sangle, eangle;
				pDoc->Get_Rotate_Angle(&sangle, &eangle, parc_bak->m_fSangle, parc_bak->m_fEangle, flag*90, mirror);

				sangle = pDoc->Get_Correct_Angle(sangle);
				parc->m_fSangle = sangle;
				eangle = pDoc->Get_Correct_Angle(eangle);
				parc->m_fEangle = eangle;
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				COb_pcbtrack* ptrack_bak = arrayTempTrack_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX1 - fx0, ptrack_bak->m_fY1 - fy0, flag*90, mirror);
				ptrack->m_fX1 = fx + fx0 + deltax;
				ptrack->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX2 - fx0, ptrack_bak->m_fY2 - fy0, flag*90, mirror);
				ptrack->m_fX2 = fx + fx0 + deltax;
				ptrack->m_fY2 = fy + fy0 + deltay;				
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				COb_pcbvia* pvia_bak = arrayTempVia_bak.GetAt(i);
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pvia_bak->m_fCx - fx0, pvia_bak->m_fCy - fy0, flag*90, mirror);
				pvia->m_fCx = fx + fx0 + deltax;
				pvia->m_fCy = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				COb_pcbfill* pfill_bak = arrayTempFill_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX1 - fx0, pfill_bak->m_fY1 - fy0, flag*90, mirror);
				pfill->m_fX1 = fx + fx0 + deltax;
				pfill->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX2 - fx0, pfill_bak->m_fY2 - fy0, flag*90, mirror);
				pfill->m_fX2 = fx + fx0 + deltax;
				pfill->m_fY2 = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				COb_pcbpad* ppad_bak = arrayTempPad_bak.GetAt(i);
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppad_bak->m_fCx - fx0, ppad_bak->m_fCy - fy0, flag*90, mirror);
				ppad->m_fCx = fx + fx0 + deltax;
				ppad->m_fCy = fy + fy0 + deltay;
				double fangle;
				pDoc->Get_Rotate_Angle(&fangle, &fangle, ppad_bak->m_fRotation, ppad_bak->m_fRotation, flag*90, mirror);

				ppad->m_fRotation = fangle;
				ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
			}
			

			pView->Refresh_Package_Ratsnest(index);
			
			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);
				//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
				//else ppad->DrawSingleLayerPad(pDC, pV, 1);
			}

			pView->DrawPcbConnections(pDC);
			pView->OpenMouseCursor();
		}
		else if((ch1 == KEY_x)||(ch1 == KEY_X))
		{
			if(AfxMessageBox("使用\"L\"键将元件放置到另外一面, 同层翻转元件可能会造成设计错误! 确认翻转?", MB_YESNO|MB_ICONWARNING) == IDYES)
			{
				pView->CloseMouseCursor();
		
				for(int i=0; i<arrayTempArc.GetCount(); i++) 
				{
					COb_pcbarc* parc = arrayTempArc.GetAt(i);
					parc->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempTrack.GetCount(); i++) 
				{
					COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
					ptrack->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempVia.GetCount(); i++) 
				{
					COb_pcbvia* pvia = arrayTempVia.GetAt(i);
					pvia->Draw(pDC, pV, 1);
				}
				for(int i=0; i<arrayTempFill.GetCount(); i++) 
				{
					COb_pcbfill* pfill = arrayTempFill.GetAt(i);
					pfill->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempPad.GetCount(); i++) 
				{
					COb_pcbpad* ppad = arrayTempPad.GetAt(i);
					ppad->Draw(pDC, pV, DRAW_DRAFT);
					//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
					//else ppad->DrawSingleLayerPad(pDC, pV, 1);
				}

				pView->DrawPcbConnections(pDC);

				if(mirror == FALSE) mirror = TRUE;
				else mirror = FALSE;
				
				fx1 = pView->m_fCurrentSnapX;
				fy1 = pView->m_fCurrentSnapY;
				deltax = fx1 - fx0;
				deltay = fy1 - fy0;

				for(int i=0; i<arrayTempArc.GetCount(); i++)
				{
					COb_pcbarc* parc = arrayTempArc.GetAt(i);
					COb_pcbarc* parc_bak = arrayTempArc_bak.GetAt(i);

					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, parc_bak->m_fCx - fx0, parc_bak->m_fCy - fy0, flag*90, mirror);
					parc->m_fCx = fx + fx0 + deltax;
					parc->m_fCy = fy + fy0 + deltay;

					double sangle, eangle;
					pDoc->Get_Rotate_Angle(&sangle, &eangle, parc_bak->m_fSangle, parc_bak->m_fEangle, flag*90, mirror);

					sangle = pDoc->Get_Correct_Angle(sangle);
					parc->m_fSangle = sangle;
					eangle = pDoc->Get_Correct_Angle(eangle);
					parc->m_fEangle = eangle;
				}
				for(int i=0; i<arrayTempTrack.GetCount(); i++)
				{
					COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
					COb_pcbtrack* ptrack_bak = arrayTempTrack_bak.GetAt(i);
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX1 - fx0, ptrack_bak->m_fY1 - fy0, flag*90, mirror);
					ptrack->m_fX1 = fx + fx0 + deltax;
					ptrack->m_fY1 = fy + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX2 - fx0, ptrack_bak->m_fY2 - fy0, flag*90, mirror);
					ptrack->m_fX2 = fx + fx0 + deltax;
					ptrack->m_fY2 = fy + fy0 + deltay;				
				}
				for(int i=0; i<arrayTempVia.GetCount(); i++) 
				{
					COb_pcbvia* pvia = arrayTempVia.GetAt(i);
					COb_pcbvia* pvia_bak = arrayTempVia_bak.GetAt(i);
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pvia_bak->m_fCx - fx0, pvia_bak->m_fCy - fy0, flag*90, mirror);
					pvia->m_fCx = fx + fx0 + deltax;
					pvia->m_fCy = fy + fy0 + deltay;
				}
				for(int i=0; i<arrayTempFill.GetCount(); i++) 
				{
					COb_pcbfill* pfill = arrayTempFill.GetAt(i);
					COb_pcbfill* pfill_bak = arrayTempFill_bak.GetAt(i);
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX1 - fx0, pfill_bak->m_fY1 - fy0, flag*90, mirror);
					pfill->m_fX1 = fx + fx0 + deltax;
					pfill->m_fY1 = fy + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX2 - fx0, pfill_bak->m_fY2 - fy0, flag*90, mirror);
					pfill->m_fX2 = fx + fx0 + deltax;
					pfill->m_fY2 = fy + fy0 + deltay;
				}
				for(int i=0; i<arrayTempPad.GetCount(); i++)
				{
					COb_pcbpad* ppad = arrayTempPad.GetAt(i);
					COb_pcbpad* ppad_bak = arrayTempPad_bak.GetAt(i);
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ppad_bak->m_fCx - fx0, ppad_bak->m_fCy - fy0, flag*90, mirror);
					ppad->m_fCx = fx + fx0 + deltax;
					ppad->m_fCy = fy + fy0 + deltay;
					
					double fangle;
					pDoc->Get_Rotate_Angle(&fangle, &fangle, ppad_bak->m_fRotation, ppad_bak->m_fRotation, flag*90, mirror);

					ppad->m_fRotation = fangle;
					ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
				}
			

				pView->Refresh_Package_Ratsnest(index);

				for(int i=0; i<arrayTempArc.GetCount(); i++) 
				{
					COb_pcbarc* parc = arrayTempArc.GetAt(i);
					parc->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempTrack.GetCount(); i++) 
				{
					COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
					ptrack->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempVia.GetCount(); i++) 
				{
					COb_pcbvia* pvia = arrayTempVia.GetAt(i);
					pvia->Draw(pDC, pV, 1);
				}
				for(int i=0; i<arrayTempFill.GetCount(); i++) 
				{
					COb_pcbfill* pfill = arrayTempFill.GetAt(i);
					pfill->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempPad.GetCount(); i++) 
				{
					COb_pcbpad* ppad = arrayTempPad.GetAt(i);
					ppad->Draw(pDC, pV, DRAW_DRAFT);
					//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
					//else ppad->DrawSingleLayerPad(pDC, pV, 1);
				}

				pView->DrawPcbConnections(pDC);
				pView->OpenMouseCursor();
			}
		}
		else if((ch1 == KEY_l)||(ch1 == KEY_L))
		{
			pView->CloseMouseCursor();
			
			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);
				//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
				//else ppad->DrawSingleLayerPad(pDC, pV, 1);
			}

			pView->DrawPcbConnections(pDC);

			if(fliplayer == TRUE) fliplayer = FALSE;
			else fliplayer = TRUE;
			if(mirror == TRUE) mirror = FALSE;
			else mirror = TRUE;
				
			fx1 = pView->m_fCurrentSnapX;
			fy1 = pView->m_fCurrentSnapY;
			deltax = fx1 - fx0;
			deltay = fy1 - fy0;

			for(int i=0; i<arrayTempArc.GetCount(); i++)
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				COb_pcbarc* parc_bak = arrayTempArc_bak.GetAt(i);

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, parc_bak->m_fCx - fx0, parc_bak->m_fCy - fy0, flag*90, mirror);
				parc->m_fCx = fx + fx0 + deltax;
				parc->m_fCy = fy + fy0 + deltay;

				double sangle, eangle;
				pDoc->Get_Rotate_Angle(&sangle, &eangle, parc_bak->m_fSangle, parc_bak->m_fEangle, flag*90, mirror);

				sangle = pDoc->Get_Correct_Angle(sangle);
				parc->m_fSangle = sangle;
				eangle = pDoc->Get_Correct_Angle(eangle);
				parc->m_fEangle = eangle;

				if(parc->m_nLayer == TOPLAYER) parc->m_nLayer = BOTTOMLAYER;
				else if(parc->m_nLayer == BOTTOMLAYER) parc->m_nLayer = TOPLAYER;
				else if(parc->m_nLayer == TOPOVERLAY) parc->m_nLayer = BOTTOMOVERLAY;
				else if(parc->m_nLayer == BOTTOMOVERLAY) parc->m_nLayer = TOPOVERLAY;
				else if(parc->m_nLayer == TOPPASTE) parc->m_nLayer = BOTTOMPASTE;
				else if(parc->m_nLayer == BOTTOMPASTE) parc->m_nLayer = TOPPASTE;
				else if(parc->m_nLayer == TOPSOLDER) parc->m_nLayer = BOTTOMSOLDER;
				else if(parc->m_nLayer == BOTTOMSOLDER) parc->m_nLayer = TOPSOLDER;
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				COb_pcbtrack* ptrack_bak = arrayTempTrack_bak.GetAt(i);

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX1 - fx0, ptrack_bak->m_fY1 - fy0, flag*90, mirror);
				ptrack->m_fX1 = fx + fx0 + deltax;
				ptrack->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX2 - fx0, ptrack_bak->m_fY2 - fy0, flag*90, mirror);
				ptrack->m_fX2 = fx + fx0 + deltax;
				ptrack->m_fY2 = fy + fy0 + deltay;
				
				if(ptrack->m_nLayer == TOPLAYER) ptrack->m_nLayer = BOTTOMLAYER;
				else if(ptrack->m_nLayer == BOTTOMLAYER) ptrack->m_nLayer = TOPLAYER;
				else if(ptrack->m_nLayer == TOPOVERLAY) ptrack->m_nLayer = BOTTOMOVERLAY;
				else if(ptrack->m_nLayer == BOTTOMOVERLAY) ptrack->m_nLayer = TOPOVERLAY;
				else if(ptrack->m_nLayer == TOPPASTE) ptrack->m_nLayer = BOTTOMPASTE;
				else if(ptrack->m_nLayer == BOTTOMPASTE) ptrack->m_nLayer = TOPPASTE;
				else if(ptrack->m_nLayer == TOPSOLDER) ptrack->m_nLayer = BOTTOMSOLDER;
				else if(ptrack->m_nLayer == BOTTOMSOLDER) ptrack->m_nLayer = TOPSOLDER;
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				COb_pcbvia* pvia_bak = arrayTempVia_bak.GetAt(i);
					
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pvia_bak->m_fCx - fx0, pvia_bak->m_fCy - fy0, flag*90, mirror);
				pvia->m_fCx = fx + fx0 + deltax;
				pvia->m_fCy = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				COb_pcbfill* pfill_bak = arrayTempFill_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX1 - fx0, pfill_bak->m_fY1 - fy0, flag*90, mirror);
				pfill->m_fX1 = fx + fx0 + deltax;
				pfill->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX2 - fx0, pfill_bak->m_fY2 - fy0, flag*90, mirror);
				pfill->m_fX2 = fx + fx0 + deltax;
				pfill->m_fY2 = fy + fy0 + deltay;
				
				if(pfill->m_nLayer == TOPLAYER) pfill->m_nLayer = BOTTOMLAYER;
				else if(pfill->m_nLayer == BOTTOMLAYER) pfill->m_nLayer = TOPLAYER;
				else if(pfill->m_nLayer == TOPOVERLAY) pfill->m_nLayer = BOTTOMOVERLAY;
				else if(pfill->m_nLayer == BOTTOMOVERLAY) pfill->m_nLayer = TOPOVERLAY;
				else if(pfill->m_nLayer == TOPPASTE) pfill->m_nLayer = BOTTOMPASTE;
				else if(pfill->m_nLayer == BOTTOMPASTE) pfill->m_nLayer = TOPPASTE;
				else if(pfill->m_nLayer == TOPSOLDER) pfill->m_nLayer = BOTTOMSOLDER;
				else if(pfill->m_nLayer == BOTTOMSOLDER) pfill->m_nLayer = TOPSOLDER;
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				COb_pcbpad* ppad_bak = arrayTempPad_bak.GetAt(i);
					
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppad_bak->m_fCx - fx0, ppad_bak->m_fCy - fy0, flag*90, mirror);
				ppad->m_fCx = fx + fx0 + deltax;
				ppad->m_fCy = fy + fy0 + deltay;
					
				double fangle;
				pDoc->Get_Rotate_Angle(&fangle, &fangle, ppad_bak->m_fRotation, ppad_bak->m_fRotation, flag*90, mirror);

				ppad->m_fRotation = fangle;
				ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
				
				if(ppad->m_nLayer == TOPLAYER) ppad->m_nLayer = BOTTOMLAYER;
				else if(ppad->m_nLayer == BOTTOMLAYER) ppad->m_nLayer = TOPLAYER;
				else if(ppad->m_nLayer == TOPOVERLAY) ppad->m_nLayer = BOTTOMOVERLAY;
				else if(ppad->m_nLayer == BOTTOMOVERLAY) ppad->m_nLayer = TOPOVERLAY;
				else if(ppad->m_nLayer == TOPPASTE) ppad->m_nLayer = BOTTOMPASTE;
				else if(ppad->m_nLayer == BOTTOMPASTE) ppad->m_nLayer = TOPPASTE;
				else if(ppad->m_nLayer == TOPSOLDER) ppad->m_nLayer = BOTTOMSOLDER;
				else if(ppad->m_nLayer == BOTTOMSOLDER) ppad->m_nLayer = TOPSOLDER;
			}
			
			pView->Refresh_Package_Ratsnest(index);

			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);
				//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
				//else ppad->DrawSingleLayerPad(pDC, pV, 1);
			}

			pView->DrawPcbConnections(pDC);
			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	while (!arrayTempArc_bak.IsEmpty())
	{
		delete arrayTempArc_bak.GetAt(0);
        arrayTempArc_bak.RemoveAt(0);
	}
	while (!arrayTempTrack_bak.IsEmpty())
	{
		delete arrayTempTrack_bak.GetAt(0);
        arrayTempTrack_bak.RemoveAt(0);
	}
	while (!arrayTempPad_bak.IsEmpty())
	{
		delete arrayTempPad_bak.GetAt(0);
        arrayTempPad_bak.RemoveAt(0);
	}
	while (!arrayTempText_bak.IsEmpty())
	{
		delete arrayTempText_bak.GetAt(0);
        arrayTempText_bak.RemoveAt(0);
	}
	while (!arrayTempFill_bak.IsEmpty())
	{
		delete arrayTempFill_bak.GetAt(0);
        arrayTempFill_bak.RemoveAt(0);
	}
	while (!arrayTempVia_bak.IsEmpty())
	{
		delete arrayTempVia_bak.GetAt(0);
        arrayTempVia_bak.RemoveAt(0);
	}
	while (!arrayTempRegion_bak.IsEmpty())
	{
		delete arrayTempRegion_bak.GetAt(0);
        arrayTempRegion_bak.RemoveAt(0);
	}

	pDC->SetROP2(drawmode);

	m_nFlag = FLAG_NORMAL;
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag == FLAG_PROCESSING)&&(parc->m_nComponent == index))	parc->m_nFlag = FLAG_NORMAL;
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag == FLAG_PROCESSING)&&(ptrack->m_nComponent == index)) ptrack->m_nFlag = FLAG_NORMAL;
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag == FLAG_PROCESSING)&&(ppad->m_nComponent == index)) ppad->m_nFlag = FLAG_NORMAL;
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag == FLAG_PROCESSING)&&(pfill->m_nComponent == index)) pfill->m_nFlag = FLAG_NORMAL;
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag == FLAG_PROCESSING)&&(ptext->m_nComponent == index)) ptext->m_nFlag = FLAG_NORMAL;
	}
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag == FLAG_PROCESSING)&&(pvia->m_nComponent == index)) pvia->m_nFlag = FLAG_NORMAL;
	}
	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag == FLAG_PROCESSING)&&(pregion->m_nComponent == index)) pregion->m_nFlag = FLAG_NORMAL;
	}

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	
	ClipCursor(NULL); pView->m_bClipState = FALSE;

	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
	

	pView->Refresh_Ratsnest();	

	pView->Write_Help(szEDA_Help_Ready);
	pView->Invalidate();
}

BOOL COb_pcbcomp::MovePlacePackage(CClientDC* pDC, CView* pV, int index)
{
	int ch1;
	double fx0, fy0, fx1, fy1, deltax, deltay;
	int flag;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动元件: <空格>旋转, <L>换层, <X>镜像, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	CTypedPtrArray <CObArray, COb_pcbarc*> arrayTempArc;
	CTypedPtrArray <CObArray, COb_pcbtrack*> arrayTempTrack;
	CTypedPtrArray <CObArray, COb_pcbtext*> arrayTempText;
	CTypedPtrArray <CObArray, COb_pcbfill*> arrayTempFill;
	CTypedPtrArray <CObArray, COb_pcbregion*> arrayTempRegion;	
	CTypedPtrArray <CObArray, COb_pcbpad*> arrayTempPad;
	CTypedPtrArray <CObArray, COb_pcbvia*> arrayTempVia;
	CTypedPtrArray <CObArray, COb_pcbarc*> arrayTempArc_bak;
	CTypedPtrArray <CObArray, COb_pcbtrack*> arrayTempTrack_bak;
	CTypedPtrArray <CObArray, COb_pcbtext*> arrayTempText_bak;
	CTypedPtrArray <CObArray, COb_pcbfill*> arrayTempFill_bak;
	CTypedPtrArray <CObArray, COb_pcbregion*> arrayTempRegion_bak;
	CTypedPtrArray <CObArray, COb_pcbpad*> arrayTempPad_bak;
	CTypedPtrArray <CObArray, COb_pcbvia*> arrayTempVia_bak;


	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag == FLAG_NORMAL)&&(parc->m_nComponent == index))
		{
			arrayTempArc.Add(parc);
			
			COb_pcbarc* ptemparc_bak = new COb_pcbarc();
			ptemparc_bak->Copy(parc);
			arrayTempArc_bak.Add(ptemparc_bak);

			parc->m_nFlag = FLAG_PROCESSING;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag == FLAG_NORMAL)&&(ptrack->m_nComponent == index))
		{
			arrayTempTrack.Add(ptrack);
			
			COb_pcbtrack* ptemptrack_bak = new COb_pcbtrack();
			ptemptrack_bak->Copy(ptrack);
			arrayTempTrack_bak.Add(ptemptrack_bak);
			
			ptrack->m_nFlag = FLAG_PROCESSING;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == index))
		{
			arrayTempPad.Add(ppad);

			COb_pcbpad* ptemppad_bak = new COb_pcbpad();
			ptemppad_bak->Copy(ppad);
			arrayTempPad_bak.Add(ptemppad_bak);

			ppad->m_nFlag = FLAG_PROCESSING;		
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag == FLAG_NORMAL)&&(pfill->m_nComponent == index))
		{
			arrayTempFill.Add(pfill);

			COb_pcbfill* ptempfill_bak = new COb_pcbfill();
			ptempfill_bak->Copy(pfill);
			arrayTempFill_bak.Add(ptempfill_bak);

			pfill->m_nFlag = FLAG_PROCESSING;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag == FLAG_NORMAL)&&(ptext->m_nComponent == index))
		{
			arrayTempText.Add(ptext);
			
			COb_pcbtext* ptemptext_bak = new COb_pcbtext();
			ptemptext_bak->Copy(ptext);
			arrayTempText_bak.Add(ptemptext_bak);

			ptext->m_nFlag = FLAG_PROCESSING;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag == FLAG_NORMAL)&&(pvia->m_nComponent == index))
		{
			arrayTempVia.Add(pvia);
			
			COb_pcbvia* ptempvia_bak = new COb_pcbvia();
			ptempvia_bak->Copy(pvia);
			arrayTempVia_bak.Add(ptempvia_bak);

			pvia->m_nFlag = FLAG_PROCESSING;
		}
	}

	pView->Refresh_Package_Ratsnest(index);

	pView->CloseMouseCursor();
	pView->Invalidate();
	pView->OpenMouseCursor();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	fx0 = m_fX;
	fy0 = m_fY;
	deltax = pView->m_fCurrentSnapX - fx0;
	deltay = pView->m_fCurrentSnapY - fy0;


	pView->Write_Help(szHelp);
	flag = 0; 
	BOOL mirror = FALSE;
	BOOL fliplayer = FALSE;
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{

			pView->CloseMouseCursor();

			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);

			}
			for(int i=0; i<arrayTempText.GetCount(); i++) 
			{
				COb_pcbtext* ptext = arrayTempText.GetAt(i);
				ptext->Draw(pDC, pV);
			}

			pView->DrawPcbConnections(pDC);
			pView->OpenMouseCursor();
			
			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				COb_pcbarc* parc_bak = arrayTempArc_bak.GetAt(i);
				parc->Copy(parc_bak);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				COb_pcbtrack* ptrack_bak = arrayTempTrack_bak.GetAt(i);
				ptrack->Copy(ptrack_bak);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				COb_pcbvia* pvia_bak = arrayTempVia_bak.GetAt(i);
				pvia->Copy(pvia_bak);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				COb_pcbfill* pfill_bak = arrayTempFill_bak.GetAt(i);
				pfill->Copy(pfill_bak);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				COb_pcbpad* ppad_bak = arrayTempPad_bak.GetAt(i);
				ppad->Copy(ppad_bak);
			}
		}
		else if(ch1 == TAB)
		{

			bEDA_LoadPackageByName = TRUE;
			pView->CloseMouseCursor();

			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);
				//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
				//else ppad->DrawSingleLayerPad(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempText.GetCount(); i++) 
			{
				COb_pcbtext* ptext = arrayTempText.GetAt(i);
				ptext->Draw(pDC, pV);
			}

			pView->DrawPcbConnections(pDC);
			pView->OpenMouseCursor();


			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				COb_pcbarc* parc_bak = arrayTempArc_bak.GetAt(i);
				parc->Copy(parc_bak);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				COb_pcbtrack* ptrack_bak = arrayTempTrack_bak.GetAt(i);
				ptrack->Copy(ptrack_bak);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				COb_pcbvia* pvia_bak = arrayTempVia_bak.GetAt(i);
				pvia->Copy(pvia_bak);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				COb_pcbfill* pfill_bak = arrayTempFill_bak.GetAt(i);
				pfill->Copy(pfill_bak);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				COb_pcbpad* ppad_bak = arrayTempPad_bak.GetAt(i);
				ppad->Copy(ppad_bak);
			}
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
				
				pView->DrawPcbConnections(pDC);
			}
			else  
			{
				pView->ElecHotPointsControl(pDC);

				pView->CloseMouseCursor();
				for(int i=0; i<arrayTempArc.GetCount(); i++) 
				{
					COb_pcbarc* parc = arrayTempArc.GetAt(i);
					parc->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempTrack.GetCount(); i++) 
				{
					COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
					ptrack->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempVia.GetCount(); i++) 
				{
					COb_pcbvia* pvia = arrayTempVia.GetAt(i);
					pvia->Draw(pDC, pV, 1);
				}
				for(int i=0; i<arrayTempFill.GetCount(); i++) 
				{
					COb_pcbfill* pfill = arrayTempFill.GetAt(i);
					pfill->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempPad.GetCount(); i++) 
				{
					COb_pcbpad* ppad = arrayTempPad.GetAt(i);
					ppad->Draw(pDC, pV, DRAW_DRAFT);
					//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
					//else ppad->DrawSingleLayerPad(pDC, pV, 1);
				}
				
				pView->DrawPcbConnections(pDC);
				pView->OpenMouseCursor();
			}

			fx1 = pView->m_fCurrentSnapX;
			fy1 = pView->m_fCurrentSnapY;
			deltax = fx1 - fx0;
			deltay = fy1 - fy0;

			for(int i=0; i<arrayTempArc.GetCount(); i++)
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				COb_pcbarc* parc_bak = arrayTempArc_bak.GetAt(i);

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, parc_bak->m_fCx - fx0, parc_bak->m_fCy - fy0, flag*90, mirror);
				parc->m_fCx = fx + fx0 + deltax;
				parc->m_fCy = fy + fy0 + deltay;

				double sangle, eangle;
				pDoc->Get_Rotate_Angle(&sangle, &eangle, parc_bak->m_fSangle, parc_bak->m_fEangle, flag*90, mirror);

				sangle = pDoc->Get_Correct_Angle(sangle);
				parc->m_fSangle = sangle;
				eangle = pDoc->Get_Correct_Angle(eangle);
				parc->m_fEangle = eangle;
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				COb_pcbtrack* ptrack_bak = arrayTempTrack_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX1 - fx0, ptrack_bak->m_fY1 - fy0, flag*90, mirror);
				ptrack->m_fX1 = fx + fx0 + deltax;
				ptrack->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX2 - fx0, ptrack_bak->m_fY2 - fy0, flag*90, mirror);
				ptrack->m_fX2 = fx + fx0 + deltax;
				ptrack->m_fY2 = fy + fy0 + deltay;				
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				COb_pcbvia* pvia_bak = arrayTempVia_bak.GetAt(i);
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pvia_bak->m_fCx - fx0, pvia_bak->m_fCy - fy0, flag*90, mirror);
				pvia->m_fCx = fx + fx0 + deltax;
				pvia->m_fCy = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				COb_pcbfill* pfill_bak = arrayTempFill_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX1 - fx0, pfill_bak->m_fY1 - fy0, flag*90, mirror);
				pfill->m_fX1 = fx + fx0 + deltax;
				pfill->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX2 - fx0, pfill_bak->m_fY2 - fy0, flag*90, mirror);
				pfill->m_fX2 = fx + fx0 + deltax;
				pfill->m_fY2 = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				COb_pcbpad* ppad_bak = arrayTempPad_bak.GetAt(i);
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppad_bak->m_fCx - fx0, ppad_bak->m_fCy - fy0, flag*90, mirror);
				ppad->m_fCx = fx + fx0 + deltax;
				ppad->m_fCy = fy + fy0 + deltay;
								
				double fangle;
				pDoc->Get_Rotate_Angle(&fangle, &fangle, ppad_bak->m_fRotation, ppad_bak->m_fRotation, flag*90, mirror);

				ppad->m_fRotation = fangle;
				ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
				//ppad->m_fRotation += flag*90;
				//ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
			}
						
			pView->Refresh_Package_Ratsnest(index);
			
			pView->CloseMouseCursor();

			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);
				//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
				//else ppad->DrawSingleLayerPad(pDC, pV, 1);
			}

			pView->DrawPcbConnections(pDC);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();

			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);
				//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
				//else ppad->DrawSingleLayerPad(pDC, pV, 1);
			}

			pView->DrawPcbConnections(pDC);
			pView->OpenMouseCursor();
			

			m_fX = pView->m_fCurrentSnapX;
			m_fY = pView->m_fCurrentSnapY;

			double fangle;
			pDoc->Get_Rotate_Angle(&fangle, &fangle, m_fRotation, m_fRotation, flag*90, mirror);
			m_fRotation = fangle;
			m_fRotation = pDoc->Get_Correct_Angle(m_fRotation);
			
			fx1 = pView->m_fCurrentSnapX;
			fy1 = pView->m_fCurrentSnapY;
			deltax = fx1 - fx0;
			deltay = fy1 - fy0;

			for(int i=0; i<arrayTempArc.GetCount(); i++)
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				COb_pcbarc* parc_bak = arrayTempArc_bak.GetAt(i);

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, parc_bak->m_fCx - fx0, parc_bak->m_fCy - fy0, flag*90, mirror);
				parc->m_fCx = fx + fx0 + deltax;
				parc->m_fCy = fy + fy0 + deltay;

				double sangle, eangle;
				pDoc->Get_Rotate_Angle(&sangle, &eangle, parc_bak->m_fSangle, parc_bak->m_fEangle, flag*90, mirror);

				sangle = pDoc->Get_Correct_Angle(sangle);
				parc->m_fSangle = sangle;
				eangle = pDoc->Get_Correct_Angle(eangle);
				parc->m_fEangle = eangle;
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				COb_pcbtrack* ptrack_bak = arrayTempTrack_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX1 - fx0, ptrack_bak->m_fY1 - fy0, flag*90, mirror);
				ptrack->m_fX1 = fx + fx0 + deltax;
				ptrack->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX2 - fx0, ptrack_bak->m_fY2 - fy0, flag*90, mirror);
				ptrack->m_fX2 = fx + fx0 + deltax;
				ptrack->m_fY2 = fy + fy0 + deltay;				
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				COb_pcbvia* pvia_bak = arrayTempVia_bak.GetAt(i);
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pvia_bak->m_fCx - fx0, pvia_bak->m_fCy - fy0, flag*90, mirror);
				pvia->m_fCx = fx + fx0 + deltax;
				pvia->m_fCy = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				COb_pcbfill* pfill_bak = arrayTempFill_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX1 - fx0, pfill_bak->m_fY1 - fy0, flag*90, mirror);
				pfill->m_fX1 = fx + fx0 + deltax;
				pfill->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX2 - fx0, pfill_bak->m_fY2 - fy0, flag*90, mirror);
				pfill->m_fX2 = fx + fx0 + deltax;
				pfill->m_fY2 = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				COb_pcbpad* ppad_bak = arrayTempPad_bak.GetAt(i);
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppad_bak->m_fCx - fx0, ppad_bak->m_fCy - fy0, flag*90, mirror);
				ppad->m_fCx = fx + fx0 + deltax;
				ppad->m_fCy = fy + fy0 + deltay;
								
				double fangle;
				pDoc->Get_Rotate_Angle(&fangle, &fangle, ppad_bak->m_fRotation, ppad_bak->m_fRotation, flag*90, mirror);

				ppad->m_fRotation = fangle;
				ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
			}
			
			double fcomp_x1, fcomp_y1, fcomp_x2, fcomp_y2, fcomp_x3, fcomp_y3, fcomp_x4, fcomp_y4;
			GetPcbCompRect(&fcomp_x1, &fcomp_y1, &fcomp_x2, &fcomp_y2, &fcomp_x3, &fcomp_y3, &fcomp_x4, &fcomp_y4, index);
			CRect rect;
			rect.left = MIN(MIN(fcomp_x1, fcomp_x2), MIN(fcomp_x3, fcomp_x4));
			rect.right = MAX(MAX(fcomp_x1, fcomp_x2), MAX(fcomp_x3, fcomp_x4));
			rect.bottom = MIN(MIN(fcomp_y1, fcomp_y2), MIN(fcomp_y3, fcomp_y4));
			rect.top = MAX(MAX(fcomp_y1, fcomp_y2), MAX(fcomp_y3, fcomp_y4));

			int tempindex = 0;
			for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
				if(ptext->m_nComponent == index)
				{
					if(ptext->m_bDesignator == TRUE)
					{
						if(m_nNamePosition == MANUAL)
						{
							double fx, fy;
							pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - fx0, ptext->m_fSy - fy0, flag*90, mirror);
							ptext->m_fSx = fx + fx0 + deltax;
							ptext->m_fSy = fy + fy0 + deltay;
						
							double angle;
							pDoc->Get_Rotate_Angle(&angle, &angle, ptext->m_fRotation, ptext->m_fRotation, flag*90, FALSE);
							angle = pDoc->Get_Correct_Angle(angle);
							ptext->m_fRotation = angle;
						}
						else
						{
							if(m_nNamePosition == LEFTTOP)
							{
								ptext->m_fSx = rect.left;
								ptext->m_fSy = rect.top + 150;
							}
							else if(m_nNamePosition == LEFTCENTER)
							{
								ptext->m_fSx = rect.left - ptext->GetStringWidth(pV) - 50;
								ptext->m_fSy = rect.top - (rect.top - rect.bottom)/2 + 50;
							}
							else if(m_nNamePosition == LEFTBOTTOM)
							{
								ptext->m_fSx = rect.left;
								ptext->m_fSy = rect.bottom - 80;
							}
							else if(m_nNamePosition == CENTERTOP)
							{
								ptext->m_fSx = rect.left + (rect.right - rect.left)/2 - ptext->GetStringWidth(pV)/2;
								ptext->m_fSy = rect.top + 150;
							}
							else if(m_nNamePosition == CENTERCENTER)
							{
								ptext->m_fSx = rect.left + (rect.right - rect.left)/2 - ptext->GetStringWidth(pV)/2;
								ptext->m_fSy = rect.top - (rect.top - rect.bottom)/2 + 50;
							}
							else if(m_nNamePosition == CENTERBOTTOM)
							{
								ptext->m_fSx = rect.left + (rect.right - rect.left)/2 - ptext->GetStringWidth(pV)/2;
								ptext->m_fSy = rect.bottom - 80;
							}
							else if(m_nNamePosition == RIGHTTOP)
							{
								ptext->m_fSx = rect.right + 50;
								ptext->m_fSy = rect.top - 80;
							}
							else if(m_nNamePosition == RIGHTCENTER)
							{
								ptext->m_fSx = rect.right + 50;
								ptext->m_fSy = rect.top - (rect.top - rect.bottom)/2 + 50;
							}
							else //if(m_nNamePosition == RIGHTBOTTOM)
							{
								ptext->m_fSx = rect.right + 50;
								ptext->m_fSy = rect.bottom + 100;
							}
							ptext->m_fRotation = 0;
						}
					}
					else if(ptext->m_bComment == TRUE)
					{
						if(m_nCommentPosition == MANUAL)
						{
							double fx, fy;
							pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - fx0, ptext->m_fSy - fy0, flag*90, mirror);
							ptext->m_fSx = fx + fx0 + deltax;
							ptext->m_fSy = fy + fy0 + deltay;

							double angle;
							pDoc->Get_Rotate_Angle(&angle, &angle, ptext->m_fRotation, ptext->m_fRotation, flag*90, FALSE);
							angle = pDoc->Get_Correct_Angle(angle);
							ptext->m_fRotation = angle;
						}
						else
						{
							if(m_nNamePosition == LEFTTOP)
							{
								ptext->m_fSx = rect.left;
								ptext->m_fSy = rect.top + 50;
							}
							else if(m_nNamePosition == LEFTCENTER)
							{
								ptext->m_fSx = rect.left - ptext->GetStringWidth(pV) - 50;
								ptext->m_fSy = rect.top - (rect.top - rect.bottom)/2 - 50;
							}
							else if(m_nNamePosition == LEFTBOTTOM)
							{
								ptext->m_fSx = rect.left;
								ptext->m_fSy = rect.bottom - 180;
							}
							else if(m_nNamePosition == CENTERTOP)
							{
								ptext->m_fSx = rect.left + (rect.right - rect.left)/2 - ptext->GetStringWidth(pV)/2;
								ptext->m_fSy = rect.top + 50;
							}
							else if(m_nNamePosition == CENTERCENTER)
							{
								ptext->m_fSx = rect.left + (rect.right - rect.left)/2 - ptext->GetStringWidth(pV)/2;
								ptext->m_fSy = rect.top - (rect.top - rect.bottom)/2 - 50;
							}
							else if(m_nNamePosition == CENTERBOTTOM)
							{
								ptext->m_fSx = rect.left + (rect.right - rect.left)/2 - ptext->GetStringWidth(pV)/2;
								ptext->m_fSy = rect.bottom - 180;
							}
							else if(m_nNamePosition == RIGHTTOP)
							{
								ptext->m_fSx = rect.right + 50;
								ptext->m_fSy = rect.top - 180;
							}
							else if(m_nNamePosition == RIGHTCENTER)
							{
								ptext->m_fSx = rect.right + 50;
								ptext->m_fSy = rect.top - (rect.top - rect.bottom)/2 - 50;
							}
							else //if(m_nNamePosition == RIGHTBOTTOM)
							{
								ptext->m_fSx = rect.right + 50;
								ptext->m_fSy = rect.bottom;
							}
							ptext->m_fRotation = 0;
						}
					}
					else 
					{
						double fx, fy;
						pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - fx0, ptext->m_fSy - fy0, flag*90, mirror);
						ptext->m_fSx = fx + fx0 + deltax;
						ptext->m_fSy = fy + fy0 + deltay;


						double angle;
						pDoc->Get_Rotate_Angle(&angle, &angle, ptext->m_fRotation, ptext->m_fRotation, flag*90, FALSE);
						angle = pDoc->Get_Correct_Angle(angle);
						ptext->m_fRotation = angle;
					}

					
					if(mirror == TRUE)
					{
						if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
						else ptext->m_bMirror = TRUE;
					}

					if(fliplayer == TRUE)
					{
						if(ptext->m_nLayer == TOPLAYER) ptext->m_nLayer = BOTTOMLAYER;
						else if(ptext->m_nLayer == BOTTOMLAYER) ptext->m_nLayer = TOPLAYER;
						else if(ptext->m_nLayer == TOPOVERLAY) ptext->m_nLayer = BOTTOMOVERLAY;
						else if(ptext->m_nLayer == BOTTOMOVERLAY) ptext->m_nLayer = TOPOVERLAY;
						else if(ptext->m_nLayer == TOPPASTE) ptext->m_nLayer = BOTTOMPASTE;
						else if(ptext->m_nLayer == BOTTOMPASTE) ptext->m_nLayer = TOPPASTE;
						else if(ptext->m_nLayer == TOPSOLDER) ptext->m_nLayer = BOTTOMSOLDER;
						else if(ptext->m_nLayer == BOTTOMSOLDER) ptext->m_nLayer = TOPSOLDER;
					}

					ptext->UpdateStringBorder(pV, FALSE);
				}
			}

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();

			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);
				//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
				//else ppad->DrawSingleLayerPad(pDC, pV, 1);
			}

			pView->DrawPcbConnections(pDC);

			if(mirror == FALSE)
			{
				flag++;
				if(flag>3) flag=0;
			}
			else
			{
				flag--;
				if(flag < 0) flag = 3;
			}

			fx1 = pView->m_fCurrentSnapX;
			fy1 = pView->m_fCurrentSnapY;
			deltax = fx1 - fx0;
			deltay = fy1 - fy0;

			for(int i=0; i<arrayTempArc.GetCount(); i++)
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				COb_pcbarc* parc_bak = arrayTempArc_bak.GetAt(i);

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, parc_bak->m_fCx - fx0, parc_bak->m_fCy - fy0, flag*90, mirror);
				parc->m_fCx = fx + fx0 + deltax;
				parc->m_fCy = fy + fy0 + deltay;

				double sangle, eangle;
				pDoc->Get_Rotate_Angle(&sangle, &eangle, parc_bak->m_fSangle, parc_bak->m_fEangle, flag*90, mirror);

				sangle = pDoc->Get_Correct_Angle(sangle);
				parc->m_fSangle = sangle;
				eangle = pDoc->Get_Correct_Angle(eangle);
				parc->m_fEangle = eangle;
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				COb_pcbtrack* ptrack_bak = arrayTempTrack_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX1 - fx0, ptrack_bak->m_fY1 - fy0, flag*90, mirror);
				ptrack->m_fX1 = fx + fx0 + deltax;
				ptrack->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX2 - fx0, ptrack_bak->m_fY2 - fy0, flag*90, mirror);
				ptrack->m_fX2 = fx + fx0 + deltax;
				ptrack->m_fY2 = fy + fy0 + deltay;				
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				COb_pcbvia* pvia_bak = arrayTempVia_bak.GetAt(i);
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pvia_bak->m_fCx - fx0, pvia_bak->m_fCy - fy0, flag*90, mirror);
				pvia->m_fCx = fx + fx0 + deltax;
				pvia->m_fCy = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				COb_pcbfill* pfill_bak = arrayTempFill_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX1 - fx0, pfill_bak->m_fY1 - fy0, flag*90, mirror);
				pfill->m_fX1 = fx + fx0 + deltax;
				pfill->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX2 - fx0, pfill_bak->m_fY2 - fy0, flag*90, mirror);
				pfill->m_fX2 = fx + fx0 + deltax;
				pfill->m_fY2 = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				COb_pcbpad* ppad_bak = arrayTempPad_bak.GetAt(i);
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppad_bak->m_fCx - fx0, ppad_bak->m_fCy - fy0, flag*90, mirror);
				ppad->m_fCx = fx + fx0 + deltax;
				ppad->m_fCy = fy + fy0 + deltay;
				double fangle;
				pDoc->Get_Rotate_Angle(&fangle, &fangle, ppad_bak->m_fRotation, ppad_bak->m_fRotation, flag*90, mirror);

				ppad->m_fRotation = fangle;
				ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
			}
			
			pView->Refresh_Package_Ratsnest(index);

			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);
				//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
				//else ppad->DrawSingleLayerPad(pDC, pV, 1);
			}
			
			pView->DrawPcbConnections(pDC);
			pView->OpenMouseCursor();
		}
		else if((ch1 == KEY_x)||(ch1 == KEY_X))
		{
			if(AfxMessageBox("使用\"L\"键将元件放置到另外一面, 同层翻转元件可能会造成设计错误! 确认翻转?", MB_YESNO|MB_ICONWARNING) == IDYES)
			{
				pView->CloseMouseCursor();
		
				for(int i=0; i<arrayTempArc.GetCount(); i++) 
				{
					COb_pcbarc* parc = arrayTempArc.GetAt(i);
					parc->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempTrack.GetCount(); i++) 
				{
					COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
					ptrack->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempVia.GetCount(); i++) 
				{
					COb_pcbvia* pvia = arrayTempVia.GetAt(i);
					pvia->Draw(pDC, pV, 1);
				}
				for(int i=0; i<arrayTempFill.GetCount(); i++) 
				{
					COb_pcbfill* pfill = arrayTempFill.GetAt(i);
					pfill->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempPad.GetCount(); i++) 
				{
					COb_pcbpad* ppad = arrayTempPad.GetAt(i);
					ppad->Draw(pDC, pV, DRAW_DRAFT);
					//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
					//else ppad->DrawSingleLayerPad(pDC, pV, 1);
				}

				pView->DrawPcbConnections(pDC);

				if(mirror == FALSE) mirror = TRUE;
				else mirror = FALSE;
				
				fx1 = pView->m_fCurrentSnapX;
				fy1 = pView->m_fCurrentSnapY;
				deltax = fx1 - fx0;
				deltay = fy1 - fy0;

				for(int i=0; i<arrayTempArc.GetCount(); i++)
				{
					COb_pcbarc* parc = arrayTempArc.GetAt(i);
					COb_pcbarc* parc_bak = arrayTempArc_bak.GetAt(i);

					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, parc_bak->m_fCx - fx0, parc_bak->m_fCy - fy0, flag*90, mirror);
					parc->m_fCx = fx + fx0 + deltax;
					parc->m_fCy = fy + fy0 + deltay;

					double sangle, eangle;
					pDoc->Get_Rotate_Angle(&sangle, &eangle, parc_bak->m_fSangle, parc_bak->m_fEangle, flag*90, mirror);

					sangle = pDoc->Get_Correct_Angle(sangle);
					parc->m_fSangle = sangle;
					eangle = pDoc->Get_Correct_Angle(eangle);
					parc->m_fEangle = eangle;
				}
				for(int i=0; i<arrayTempTrack.GetCount(); i++)
				{
					COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
					COb_pcbtrack* ptrack_bak = arrayTempTrack_bak.GetAt(i);
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX1 - fx0, ptrack_bak->m_fY1 - fy0, flag*90, mirror);
					ptrack->m_fX1 = fx + fx0 + deltax;
					ptrack->m_fY1 = fy + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX2 - fx0, ptrack_bak->m_fY2 - fy0, flag*90, mirror);
					ptrack->m_fX2 = fx + fx0 + deltax;
					ptrack->m_fY2 = fy + fy0 + deltay;				
				}
				for(int i=0; i<arrayTempVia.GetCount(); i++) 
				{
					COb_pcbvia* pvia = arrayTempVia.GetAt(i);
					COb_pcbvia* pvia_bak = arrayTempVia_bak.GetAt(i);
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pvia_bak->m_fCx - fx0, pvia_bak->m_fCy - fy0, flag*90, mirror);
					pvia->m_fCx = fx + fx0 + deltax;
					pvia->m_fCy = fy + fy0 + deltay;
				}
				for(int i=0; i<arrayTempFill.GetCount(); i++) 
				{
					COb_pcbfill* pfill = arrayTempFill.GetAt(i);
					COb_pcbfill* pfill_bak = arrayTempFill_bak.GetAt(i);
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX1 - fx0, pfill_bak->m_fY1 - fy0, flag*90, mirror);
					pfill->m_fX1 = fx + fx0 + deltax;
					pfill->m_fY1 = fy + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX2 - fx0, pfill_bak->m_fY2 - fy0, flag*90, mirror);
					pfill->m_fX2 = fx + fx0 + deltax;
					pfill->m_fY2 = fy + fy0 + deltay;
				}
				for(int i=0; i<arrayTempPad.GetCount(); i++)
				{
					COb_pcbpad* ppad = arrayTempPad.GetAt(i);
					COb_pcbpad* ppad_bak = arrayTempPad_bak.GetAt(i);
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ppad_bak->m_fCx - fx0, ppad_bak->m_fCy - fy0, flag*90, mirror);
					ppad->m_fCx = fx + fx0 + deltax;
					ppad->m_fCy = fy + fy0 + deltay;
					
					double fangle;
					pDoc->Get_Rotate_Angle(&fangle, &fangle, ppad_bak->m_fRotation, ppad_bak->m_fRotation, flag*90, mirror);

					ppad->m_fRotation = fangle;
					ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
				}
				

				pView->Refresh_Package_Ratsnest(index);

				for(int i=0; i<arrayTempArc.GetCount(); i++) 
				{
					COb_pcbarc* parc = arrayTempArc.GetAt(i);
					parc->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempTrack.GetCount(); i++) 
				{
					COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
					ptrack->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempVia.GetCount(); i++) 
				{
					COb_pcbvia* pvia = arrayTempVia.GetAt(i);
					pvia->Draw(pDC, pV, 1);
				}
				for(int i=0; i<arrayTempFill.GetCount(); i++) 
				{
					COb_pcbfill* pfill = arrayTempFill.GetAt(i);
					pfill->Draw(pDC, pV);
				}
				for(int i=0; i<arrayTempPad.GetCount(); i++) 
				{
					COb_pcbpad* ppad = arrayTempPad.GetAt(i);
					ppad->Draw(pDC, pV, DRAW_DRAFT);
					//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
					//else ppad->DrawSingleLayerPad(pDC, pV, 1);
				}

				pView->DrawPcbConnections(pDC);

				pView->OpenMouseCursor();
			}
		}
		else if((ch1 == KEY_l)||(ch1 == KEY_L))
		{
			pView->CloseMouseCursor();

			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);

			}

			pView->DrawPcbConnections(pDC);

			if(fliplayer == TRUE) fliplayer = FALSE;
			else fliplayer = TRUE;
			if(mirror == TRUE) mirror = FALSE;
			else mirror = TRUE;
				
			fx1 = pView->m_fCurrentSnapX;
			fy1 = pView->m_fCurrentSnapY;
			deltax = fx1 - fx0;
			deltay = fy1 - fy0;

			for(int i=0; i<arrayTempArc.GetCount(); i++)
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				COb_pcbarc* parc_bak = arrayTempArc_bak.GetAt(i);

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, parc_bak->m_fCx - fx0, parc_bak->m_fCy - fy0, flag*90, mirror);
				parc->m_fCx = fx + fx0 + deltax;
				parc->m_fCy = fy + fy0 + deltay;

				double sangle, eangle;
				pDoc->Get_Rotate_Angle(&sangle, &eangle, parc_bak->m_fSangle, parc_bak->m_fEangle, flag*90, mirror);

				sangle = pDoc->Get_Correct_Angle(sangle);
				parc->m_fSangle = sangle;
				eangle = pDoc->Get_Correct_Angle(eangle);
				parc->m_fEangle = eangle;

				if(parc->m_nLayer == TOPLAYER) parc->m_nLayer = BOTTOMLAYER;
				else if(parc->m_nLayer == BOTTOMLAYER) parc->m_nLayer = TOPLAYER;
				else if(parc->m_nLayer == TOPOVERLAY) parc->m_nLayer = BOTTOMOVERLAY;
				else if(parc->m_nLayer == BOTTOMOVERLAY) parc->m_nLayer = TOPOVERLAY;
				else if(parc->m_nLayer == TOPPASTE) parc->m_nLayer = BOTTOMPASTE;
				else if(parc->m_nLayer == BOTTOMPASTE) parc->m_nLayer = TOPPASTE;
				else if(parc->m_nLayer == TOPSOLDER) parc->m_nLayer = BOTTOMSOLDER;
				else if(parc->m_nLayer == BOTTOMSOLDER) parc->m_nLayer = TOPSOLDER;
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				COb_pcbtrack* ptrack_bak = arrayTempTrack_bak.GetAt(i);

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX1 - fx0, ptrack_bak->m_fY1 - fy0, flag*90, mirror);
				ptrack->m_fX1 = fx + fx0 + deltax;
				ptrack->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, ptrack_bak->m_fX2 - fx0, ptrack_bak->m_fY2 - fy0, flag*90, mirror);
				ptrack->m_fX2 = fx + fx0 + deltax;
				ptrack->m_fY2 = fy + fy0 + deltay;
				
				if(ptrack->m_nLayer == TOPLAYER) ptrack->m_nLayer = BOTTOMLAYER;
				else if(ptrack->m_nLayer == BOTTOMLAYER) ptrack->m_nLayer = TOPLAYER;
				else if(ptrack->m_nLayer == TOPOVERLAY) ptrack->m_nLayer = BOTTOMOVERLAY;
				else if(ptrack->m_nLayer == BOTTOMOVERLAY) ptrack->m_nLayer = TOPOVERLAY;
				else if(ptrack->m_nLayer == TOPPASTE) ptrack->m_nLayer = BOTTOMPASTE;
				else if(ptrack->m_nLayer == BOTTOMPASTE) ptrack->m_nLayer = TOPPASTE;
				else if(ptrack->m_nLayer == TOPSOLDER) ptrack->m_nLayer = BOTTOMSOLDER;
				else if(ptrack->m_nLayer == BOTTOMSOLDER) ptrack->m_nLayer = TOPSOLDER;
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				COb_pcbvia* pvia_bak = arrayTempVia_bak.GetAt(i);
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pvia_bak->m_fCx - fx0, pvia_bak->m_fCy - fy0, flag*90, mirror);
				pvia->m_fCx = fx + fx0 + deltax;
				pvia->m_fCy = fy + fy0 + deltay;
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				COb_pcbfill* pfill_bak = arrayTempFill_bak.GetAt(i);
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX1 - fx0, pfill_bak->m_fY1 - fy0, flag*90, mirror);
				pfill->m_fX1 = fx + fx0 + deltax;
				pfill->m_fY1 = fy + fy0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pfill_bak->m_fX2 - fx0, pfill_bak->m_fY2 - fy0, flag*90, mirror);
				pfill->m_fX2 = fx + fx0 + deltax;
				pfill->m_fY2 = fy + fy0 + deltay;

				if(pfill->m_nLayer == TOPLAYER) pfill->m_nLayer = BOTTOMLAYER;
				else if(pfill->m_nLayer == BOTTOMLAYER) pfill->m_nLayer = TOPLAYER;
				else if(pfill->m_nLayer == TOPOVERLAY) pfill->m_nLayer = BOTTOMOVERLAY;
				else if(pfill->m_nLayer == BOTTOMOVERLAY) pfill->m_nLayer = TOPOVERLAY;
				else if(pfill->m_nLayer == TOPPASTE) pfill->m_nLayer = BOTTOMPASTE;
				else if(pfill->m_nLayer == BOTTOMPASTE) pfill->m_nLayer = TOPPASTE;
				else if(pfill->m_nLayer == TOPSOLDER) pfill->m_nLayer = BOTTOMSOLDER;
				else if(pfill->m_nLayer == BOTTOMSOLDER) pfill->m_nLayer = TOPSOLDER;
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				COb_pcbpad* ppad_bak = arrayTempPad_bak.GetAt(i);
					
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppad_bak->m_fCx - fx0, ppad_bak->m_fCy - fy0, flag*90, mirror);
				ppad->m_fCx = fx + fx0 + deltax;
				ppad->m_fCy = fy + fy0 + deltay;
					
				double fangle;
				pDoc->Get_Rotate_Angle(&fangle, &fangle, ppad_bak->m_fRotation, ppad_bak->m_fRotation, flag*90, mirror);

				ppad->m_fRotation = fangle;
				ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
				
				if(ppad->m_nLayer == TOPLAYER) ppad->m_nLayer = BOTTOMLAYER;
				else if(ppad->m_nLayer == BOTTOMLAYER) ppad->m_nLayer = TOPLAYER;
				else if(ppad->m_nLayer == TOPOVERLAY) ppad->m_nLayer = BOTTOMOVERLAY;
				else if(ppad->m_nLayer == BOTTOMOVERLAY) ppad->m_nLayer = TOPOVERLAY;
				else if(ppad->m_nLayer == TOPPASTE) ppad->m_nLayer = BOTTOMPASTE;
				else if(ppad->m_nLayer == BOTTOMPASTE) ppad->m_nLayer = TOPPASTE;
				else if(ppad->m_nLayer == TOPSOLDER) ppad->m_nLayer = BOTTOMSOLDER;
				else if(ppad->m_nLayer == BOTTOMSOLDER) ppad->m_nLayer = TOPSOLDER;
			}

			pView->Refresh_Package_Ratsnest(index);

			for(int i=0; i<arrayTempArc.GetCount(); i++) 
			{
				COb_pcbarc* parc = arrayTempArc.GetAt(i);
				parc->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempTrack.GetCount(); i++) 
			{
				COb_pcbtrack* ptrack = arrayTempTrack.GetAt(i);
				ptrack->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempVia.GetCount(); i++) 
			{
				COb_pcbvia* pvia = arrayTempVia.GetAt(i);
				pvia->Draw(pDC, pV, 1);
			}
			for(int i=0; i<arrayTempFill.GetCount(); i++) 
			{
				COb_pcbfill* pfill = arrayTempFill.GetAt(i);
				pfill->Draw(pDC, pV);
			}
			for(int i=0; i<arrayTempPad.GetCount(); i++) 
			{
				COb_pcbpad* ppad = arrayTempPad.GetAt(i);
				ppad->Draw(pDC, pV, DRAW_DRAFT);
				//if(ppad->m_nLayer == MULTILAYER) ppad->DrawMultiLayerPad(pDC, pV, 1);
				//else ppad->DrawSingleLayerPad(pDC, pV, 1);
			}

			pView->DrawPcbConnections(pDC);
			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC )&&(ch1 != TAB));

	while (!arrayTempArc_bak.IsEmpty())
	{
		delete arrayTempArc_bak.GetAt(0);
        arrayTempArc_bak.RemoveAt(0);
	}
	while (!arrayTempTrack_bak.IsEmpty())
	{
		delete arrayTempTrack_bak.GetAt(0);
        arrayTempTrack_bak.RemoveAt(0);
	}
	while (!arrayTempPad_bak.IsEmpty())
	{
		delete arrayTempPad_bak.GetAt(0);
        arrayTempPad_bak.RemoveAt(0);
	}
	while (!arrayTempText_bak.IsEmpty())
	{
		delete arrayTempText_bak.GetAt(0);
        arrayTempText_bak.RemoveAt(0);
	}
	while (!arrayTempFill_bak.IsEmpty())
	{
		delete arrayTempFill_bak.GetAt(0);
        arrayTempFill_bak.RemoveAt(0);
	}
	while (!arrayTempVia_bak.IsEmpty())
	{
		delete arrayTempVia_bak.GetAt(0);
        arrayTempVia_bak.RemoveAt(0);
	}
	while (!arrayTempRegion_bak.IsEmpty())
	{
		delete arrayTempRegion_bak.GetAt(0);
        arrayTempRegion_bak.RemoveAt(0);
	}


	pDC->SetROP2(drawmode);

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if(parc->m_nComponent == index) parc->m_nFlag = FLAG_NORMAL;
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if(ptrack->m_nComponent == index) ptrack->m_nFlag = FLAG_NORMAL;
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad->m_nComponent == index) ppad->m_nFlag = FLAG_NORMAL;
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if(pfill->m_nComponent == index) pfill->m_nFlag = FLAG_NORMAL;
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if(ptext->m_nComponent == index) ptext->m_nFlag = FLAG_NORMAL;
	}
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia->m_nComponent == index) pvia->m_nFlag = FLAG_NORMAL;
	}
	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if(pregion->m_nComponent == index) pregion->m_nFlag = FLAG_NORMAL;
	}

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	
	ClipCursor(NULL); pView->m_bClipState = FALSE;

	pView->Write_Help(szEDA_Help_Ready);
	pView->Invalidate();

	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)||(ch1 == TAB)) return FALSE;
	else return TRUE;
}

BOOL  COb_pcbcomp::Is_On_Me(double fx, double fy, int index)
{
    CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	if(IsVisible(index) == FALSE) return FALSE;

	double fcomp_x1, fcomp_y1, fcomp_x2, fcomp_y2, fcomp_x3, fcomp_y3, fcomp_x4, fcomp_y4;
	GetPcbCompRect(&fcomp_x1, &fcomp_y1, &fcomp_x2, &fcomp_y2, &fcomp_x3, &fcomp_y3, &fcomp_x4, &fcomp_y4, index);

	CRgn rgn;
	POINT point[5];
	point[0].x = (int)fcomp_x1;	point[0].y = (int)fcomp_y1;
	point[1].x = (int)fcomp_x2;	point[1].y = (int)fcomp_y2;
	point[2].x = (int)fcomp_x3;	point[2].y = (int)fcomp_y3;
	point[3].x = (int)fcomp_x4;	point[3].y = (int)fcomp_y4;
	point[4].x = (int)fcomp_x1;	point[4].y = (int)fcomp_y1;

	rgn.CreatePolygonRgn(point, 5, ALTERNATE);
			
	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL  COb_pcbcomp::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fx, fy;
	fx = m_fX;
	fy = m_fY;

	if((fx < fx1)||(fx > fx2)||(fy < fy1)||(fy > fy2)) return FALSE;
	return TRUE;
}

BOOL  COb_pcbcomp::IsVisible(int index)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag == FLAG_NORMAL)&&(parc->m_nComponent == index))
		{
			if(pDoc->m_sPCB_LayerParam[parc->m_nLayer].bLayerShow == TRUE) return TRUE;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag == FLAG_NORMAL)&&(ptrack->m_nComponent == index))
		{
			if(pDoc->m_sPCB_LayerParam[ptrack->m_nLayer].bLayerShow == TRUE) return TRUE;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag == FLAG_NORMAL)&&(ptext->m_nComponent == index))
		{
			if(pDoc->m_sPCB_LayerParam[ptext->m_nLayer].bLayerShow == TRUE) return TRUE;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == index))
		{
			if(ppad->IsVisible() == TRUE) return TRUE;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag == FLAG_NORMAL)&&(pfill->m_nComponent == index))
		{
			if(pDoc->m_sPCB_LayerParam[pfill->m_nLayer].bLayerShow == TRUE) return TRUE;
		}
	}

	return FALSE;
}
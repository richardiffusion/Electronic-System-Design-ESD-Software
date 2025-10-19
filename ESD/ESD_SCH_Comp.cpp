#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditComp.h"
#include "dlg_SelectPin.h"


IMPLEMENT_SERIAL(COb_schcomp, CObject, 0)
COb_schcomp::COb_schcomp()
{
	m_nFlag = 0;
	m_bPinsMoveable = FALSE;
	m_bSelection = FALSE;
	m_nDisplayModeCount = 1;
	m_nDisplayMode = 0;

	m_csDescription.Empty();
	m_csLibraryReference.Empty();
	m_csLibraryFileName.Empty();
	m_csTargetFileName.Empty();
	m_csUniqueID.Empty();
	m_csLibraryPath.Empty();

	pDocument = NULL;
}

COb_schcomp::~COb_schcomp()
{

}

void COb_schcomp::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX;
		ar << m_fY;
		ar << m_nOrientation;
		ar << m_bMirrored;
		
		ar << m_nPartCount;
		ar << m_nPart;
		
		ar << m_nDisplayModeCount;
		ar << m_nDisplayMode;
		
		ar << m_bPartLocked;
		ar << m_bShowHidePins;
		
		ar << m_bPinsMoveable;
		
		ar << m_bColorLocked;
		ar << m_nColor;
		ar << m_nFillColor;
		ar << m_nPinColor;
		
		ar << m_nGraphicalType;
		ar << m_bDesignatorLocked;
		
		ar << m_csDescription;
		ar << m_csLibraryReference;
		ar << m_csLibraryFileName;
		ar << m_csTargetFileName;
		ar << m_csLibraryPath;
		
		ar << m_csUniqueID;
		
		ar << m_nDiagram;
		ar << m_bSelection;
	}
	else
	{
		ar >> m_fX;
		ar >> m_fY;
		ar >> m_nOrientation;
		ar >> m_bMirrored;
		
		ar >> m_nPartCount;
		ar >> m_nPart;
		
		ar >> m_nDisplayModeCount;
		ar >> m_nDisplayMode;
		
		ar >> m_bPartLocked;
		ar >> m_bShowHidePins;
		
		ar >> m_bPinsMoveable;
		
		ar >> m_bColorLocked;
		ar >> m_nColor;
		ar >> m_nFillColor;
		ar >> m_nPinColor;
		
		ar >> m_nGraphicalType;
		ar >> m_bDesignatorLocked;
		
		ar >> m_csDescription;
		ar >> m_csLibraryReference;
		ar >> m_csLibraryFileName;
		ar >> m_csTargetFileName;
		ar >> m_csLibraryPath;
		
		ar >> m_csUniqueID;
		
		ar >> m_nDiagram;
		ar >> m_bSelection;	
	}
}

void COb_schcomp::Draw(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	CObject* pobject;
	for(int j=0; j< m_arrayPos.GetSize(); j++)
	{
		POSITION pos = m_arrayPos.GetAt(j);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
		{
			COb_schpin* ppin = (COb_schpin*)pobject;
			if(ppin->m_nFlag == FLAG_DELETED) continue;

			if(( m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  m_nPart))
			{

				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ppin->m_nDisplayMode))
				{
					COLORREF old_Color = ppin->m_nColor;
					if( m_bColorLocked == TRUE)
					{
						ppin->m_nColor =  m_nPinColor;
					}
					
					BOOL old_Hide = ppin->m_bHide;
					BOOL old_NameVisible = ppin->m_bNameVisible;
					BOOL old_NumberVisible = ppin->m_bNumberVisible;
					if( m_bShowHidePins == TRUE)
					{
						ppin->m_bHide = FALSE;
						ppin->m_bNameVisible = TRUE;
						ppin->m_bNumberVisible = TRUE;
					}
					ppin->Draw(pDC, pV);

					ppin->m_nColor = old_Color;
					ppin->m_bHide = old_Hide;
					ppin->m_bNameVisible = old_NameVisible;
					ppin->m_bNumberVisible = old_NumberVisible;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
		{
			COb_schieee* pieee = (COb_schieee*)pobject;
			if(( m_nPartCount <= 1)||(pieee->m_nOwnerPartID == 0)||(pieee->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == pieee->m_nDisplayMode))
				{
					COLORREF old_Color = pieee->m_nColor;
					if( m_bColorLocked == TRUE)
					{
						pieee->m_nColor =  m_nColor;
					}
					
					pieee->Draw(pDC, pV);
							
					pieee->m_nColor = old_Color;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc = (COb_scharc*)pobject;
			if(( m_nPartCount <= 1)||(parc->m_nOwnerPartID == 0)||(parc->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == parc->m_nDisplayMode))
				{
					COLORREF old_Color = parc->m_nColor;
					if( m_bColorLocked == TRUE)
					{
						parc->m_nColor =  m_nColor;
					}
							
					if(parc->PickMe(pV) == TRUE) parc->Draw(pDC, pV);	

					parc->m_nColor = old_Color;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse = (COb_schellipse*)pobject;
			if(( m_nPartCount <= 1)||(pellipse->m_nOwnerPartID == 0)||(pellipse->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == pellipse->m_nDisplayMode))
				{
					COLORREF old_Color = pellipse->m_nColor;
					if( m_bColorLocked == TRUE)
					{
						pellipse->m_nColor =  m_nColor;
					}

					if(pellipse->PickMe(pV) == TRUE) pellipse->Draw(pDC, pV);	

					pellipse->m_nColor = old_Color;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
			if(( m_nPartCount <= 1)||(pellipsefill->m_nOwnerPartID == 0)||(pellipsefill->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == pellipsefill->m_nDisplayMode))
				{
					COLORREF old_BorderColor = pellipsefill->m_nBorderColor;
					COLORREF old_FillColor = pellipsefill->m_nFillColor;					
					if( m_bColorLocked == TRUE)
					{
						pellipsefill->m_nBorderColor =  m_nColor;
						pellipsefill->m_nFillColor =  m_nFillColor;
					}
							
					if(pellipsefill->PickMe(pV) == TRUE) pellipsefill->Draw(pDC, pV);	
							
					pellipsefill->m_nBorderColor = old_BorderColor;
					pellipsefill->m_nFillColor = old_FillColor;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie = (COb_schpie*)pobject;
			if(( m_nPartCount <= 1)||(ppie->m_nOwnerPartID == 0)||(ppie->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ppie->m_nDisplayMode))
				{
					COLORREF old_BorderColor = ppie->m_nBorderColor;
					COLORREF old_FillColor = ppie->m_nFillColor;						
					if( m_bColorLocked == TRUE)
					{
						ppie->m_nBorderColor =  m_nColor;
						ppie->m_nFillColor =  m_nFillColor;
					}
							
					if(ppie->PickMe(pV) == TRUE) ppie->Draw(pDC, pV);	

					ppie->m_nBorderColor = old_BorderColor;
					ppie->m_nFillColor = old_FillColor;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if(( m_nPartCount <= 1)||(ppolyline->m_nOwnerPartID == 0)||(ppolyline->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ppolyline->m_nDisplayMode))
				{
					COLORREF old_Color = ppolyline->m_nColor;
					if( m_bColorLocked == TRUE)
					{
						ppolyline->m_nColor =  m_nColor;
					}
							
					ppolyline->Draw(pDC, pV);

					ppolyline->m_nColor = old_Color;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect = (COb_schrect*)pobject;
			if(( m_nPartCount <= 1)||(prect->m_nOwnerPartID == 0)||(prect->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == prect->m_nDisplayMode))
				{
					COLORREF old_BorderColor = prect->m_nBorderColor;
					COLORREF old_FillColor = prect->m_nFillColor;			
					if( m_bColorLocked == TRUE)
					{
						prect->m_nBorderColor =  m_nColor;
						prect->m_nFillColor =  m_nFillColor;
					}
					
					prect->Draw(pDC, pV);

					prect->m_nBorderColor = old_BorderColor;
					prect->m_nFillColor = old_FillColor;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
			if(( m_nPartCount <= 1)||(proundrect->m_nOwnerPartID == 0)||(proundrect->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == proundrect->m_nDisplayMode))
				{
					COLORREF old_BorderColor = proundrect->m_nBorderColor;
					COLORREF old_FillColor = proundrect->m_nFillColor;
					if( m_bColorLocked == TRUE)
					{
						proundrect->m_nBorderColor =  m_nColor;
						proundrect->m_nFillColor =  m_nFillColor;
					}
							
					if(proundrect->PickMe(pV) == TRUE) proundrect->Draw(pDC, pV);

					proundrect->m_nBorderColor = old_BorderColor;
					proundrect->m_nFillColor = old_FillColor;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
			if(( m_nPartCount <= 1)||(ppolygon->m_nOwnerPartID == 0)||(ppolygon->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ppolygon->m_nDisplayMode))
				{
					COLORREF old_BorderColor = ppolygon->m_nBorderColor;
					COLORREF old_FillColor = ppolygon->m_nFillColor;
					if( m_bColorLocked == TRUE)
					{
						ppolygon->m_nBorderColor =  m_nColor;
						ppolygon->m_nFillColor =  m_nFillColor;
					}
							
					if(ppolygon->PickMe(pV) == TRUE) ppolygon->Draw(pDC, pV);	

					ppolygon->m_nBorderColor = old_BorderColor;
					ppolygon->m_nFillColor = old_FillColor;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier = (COb_schbezier*)pobject;
			if(( m_nPartCount <= 1)||(pbezier->m_nOwnerPartID == 0)||(pbezier->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == pbezier->m_nDisplayMode))
				{
					COLORREF old_Color = pbezier->m_nColor;
					if( m_bColorLocked == TRUE)
					{
						pbezier->m_nColor =  m_nColor;
					}
							
					if(pbezier->PickMe(pV) == TRUE) pbezier->Draw(pDC, pV);

					pbezier->m_nColor = old_Color;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext = (COb_schtext*)pobject;

			if(ptext->m_nID == TEXT_COMPDESIGNATOR) //元件ID
			{
				if(ptext->m_bTextVisible == TRUE)
				{
					if(ptext->PickMe(pDC, pV) == TRUE) 
					{
						if(m_nPartCount > 1)
						{
							char s[100];
							s[0] = '\0';
							if(nEDA_PartID_Suffix == 0)
							{
								s[0] = 64 + m_nPart;	
								s[1] = '\0';
							}
							else if(nEDA_PartID_Suffix == 1)
							{
								char s1[100];
								_itoa(m_nPart, s1, 10);
								sprintf(s, ":%s", s1);
							}
							else
							{
								s[0] = ':';
								s[1] = 64 + m_nPart;
								s[2] = '\0';
							}
							COb_schtext atext;
							atext.Copy(ptext);
							atext.m_csText += s;
							atext.Draw(pDC, pV);
							if((atext.m_bSelection == TRUE)&&(nEDA_Print_Flag == 0)) atext.DrawSelection(pDC, pV);
						}
						else
						{
							ptext->Draw(pDC, pV);	
							if((ptext->m_bSelection == TRUE)&&(nEDA_Print_Flag == 0)) ptext->DrawSelection(pDC, pV);
						}
					}
				}
			}
			else if(ptext->m_nID == TEXT_TEXT)  
			{
				if(( m_nPartCount <= 1)||(ptext->m_nOwnerPartID == 0)||(ptext->m_nOwnerPartID ==  m_nPart))
				{
					if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ptext->m_nDisplayMode))
					{
						COLORREF old_Color = ptext->m_nColor;
						if( m_bColorLocked == TRUE)
						{
							ptext->m_nColor =  m_nColor;
						}

						CString bakstr = ptext->m_csText;
						if(ptext->m_bTextVisible == TRUE)
						{
							if(ptext->m_bNameVisible == TRUE)	ptext->m_csText = ptext->m_csNameComment + ":" + ptext->m_csText;
							if(ptext->PickMe(pDC, pV) == TRUE) 
							{
								ptext->Draw(pDC, pV);
								if((ptext->m_bSelection == TRUE)&&(nEDA_Print_Flag == 0)) ptext->DrawSelection(pDC, pV);
							}
						}
						ptext->m_csText = bakstr;

						ptext->m_nColor = old_Color;
					}
				}
			}
			else 
			{
				CString bakstr = ptext->m_csText;
				if(ptext->m_bTextVisible == TRUE)
				{
					if(ptext->m_bNameVisible == TRUE)	ptext->m_csText = ptext->m_csNameComment + ":" + ptext->m_csText;
					if(ptext->PickMe(pDC, pV) == TRUE) 
					{
						ptext->Draw(pDC, pV);
						if((ptext->m_bSelection == TRUE)&&(nEDA_Print_Flag == 0)) ptext->DrawSelection(pDC, pV);
					}
				}
				ptext->m_csText = bakstr;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
			if(( m_nPartCount <= 1)||(ptextframe->m_nOwnerPartID == 0)||(ptextframe->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ptextframe->m_nDisplayMode))
				{
					COLORREF old_BorderColor = ptextframe->m_nBorderColor;
					COLORREF old_FillColor = ptextframe->m_nFillColor;
					COLORREF old_TextColor = ptextframe->m_nTextColor;					
					if( m_bColorLocked == TRUE)
					{
						ptextframe->m_nBorderColor =  m_nColor;
						ptextframe->m_nFillColor =  m_nFillColor;
						ptextframe->m_nTextColor =  m_nColor;
					}
					if(ptextframe->PickMe(pV) == TRUE) ptextframe->Draw(pDC, pV);

					ptextframe->m_nBorderColor =old_BorderColor;
					ptextframe->m_nFillColor =old_FillColor;
					ptextframe->m_nTextColor =old_TextColor;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;

			if(( m_nPartCount <= 1)||(pgraphic->m_nOwnerPartID == 0)||(pgraphic->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == pgraphic->m_nDisplayMode))
				{
					COLORREF old_BorderColor = pgraphic->m_nBorderColor;
					if( m_bColorLocked == TRUE)
					{
						pgraphic->m_nBorderColor =  m_nColor;
					}
					if(pgraphic->PickMe(pV) == TRUE) pgraphic->Draw(pDC, pV);

					pgraphic->m_nBorderColor = old_BorderColor;
				}
			}
		}
	}

	if(m_csUniqueID == "NEWCOMP") 
	{
		RECT rect;
		rect = GetCompRect();
		COb_schline aline;
		aline.pDocument = pDoc;
		aline.m_nAttrib = LINE_LINE;
		aline.m_nLineWidth = 0;
		aline.m_nLineType = LINE_DOTTED;
		aline.m_nColor = RGB(150,150,150);
		aline.m_nDiagram = m_nDiagram;
		aline.m_cVertex.SetSize(5);
		aline.m_cVertex[0].fx = rect.left;
		aline.m_cVertex[0].fy = rect.bottom;
		aline.m_cVertex[1].fx = rect.left;
		aline.m_cVertex[1].fy = rect.top;
		aline.m_cVertex[2].fx = rect.right;
		aline.m_cVertex[2].fy = rect.top;
		aline.m_cVertex[3].fx = rect.right;
		aline.m_cVertex[3].fy = rect.bottom;
		aline.m_cVertex[4].fx = rect.left;
		aline.m_cVertex[4].fy = rect.bottom;
		aline.Draw(pDC, pV);
	}
}

void COb_schcomp::DrawSelection(CDC* pDC, CView* pV)
{
	int x1, y1, x2, y2;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen arcpen;
	arcpen.CreatePen(PS_DOT, 1, nEDA_SelectionColor);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	RECT rect = GetCompRect();

	Fpoint fp[4];
	fp[0].fx = (double)rect.left;
	fp[0].fy = (double)rect.bottom;
	fp[1].fx = (double)rect.left;
	fp[1].fy = (double)rect.top;
	fp[2].fx = (double)rect.right;
	fp[2].fy = (double)rect.top;
	fp[3].fx = (double)rect.right;
	fp[3].fy = (double)rect.bottom;

	pView->Get_VPort_XY(&x1, &y1, fp[0].fx, fp[0].fy);
	pView->Get_VPort_XY(&x2, &y2, fp[2].fx, fp[2].fy);

	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y1);
	pDC->LineTo(x2,y2);
	pDC->LineTo(x1,y2);
	pDC->LineTo(x1,y1);

	pView->DrawSelectionKeyPoint(x1, y1, pDC);
	pView->DrawSelectionKeyPoint(x1, y2, pDC);
	pView->DrawSelectionKeyPoint(x2, y1, pDC);
	pView->DrawSelectionKeyPoint(x2, y2, pDC);

	CObject* pobject;
	for(int k=0; k<m_arrayPos.GetCount(); k++)
	{
		POSITION subpos = m_arrayPos.GetAt(k);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext = (COb_schtext*)pobject;
			if((ptext->m_nID == TEXT_COMPDESIGNATOR)||(ptext->m_nID == TEXT_COMPCOMMENT)||(ptext->m_nID == TEXT_COMPPACKAGE)||(ptext->m_nID == TEXT_COMPTEXT))
			{
				if(ptext->m_bTextVisible == TRUE)
				{
					double dis = 10000000;
					int index;
					for(int m=0; m<4; m++)
					{
						double d = sqrt((ptext->m_fSx - fp[m].fx)*(ptext->m_fSx - fp[m].fx) + (ptext->m_fSy - fp[m].fy)*(ptext->m_fSy - fp[m].fy));
						if(d < dis)
						{
							dis = d;
							index = m;
						}
					}

					pView->Get_VPort_XY(&x1, &y1, fp[index].fx, fp[index].fy);
					pView->Get_VPort_XY(&x2, &y2, ptext->m_fSx, ptext->m_fSy);
					pDC->MoveTo(x1,y1);
					pDC->LineTo(x2,y2);
				}
			}
		}
	}
	
	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

struct Struct_Multi_Selection COb_schcomp::Is_On_Me(double fx, double fy, CDocument* pDocument, CView* pV, int diagram, POSITION position)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	CESDView* pView = (CESDView*)pV;

	Struct_Multi_Selection  multi_selection;
	multi_selection.element = 0;
	multi_selection.element = NULL;

	POSITION bak_pos;
	POSITION pos = position; 
	pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
	double fx1=100000, fy1=100000, fx2=0, fy2=0;
	for(int j=0; j<m_arrayPos.GetSize(); j++)
	{
		bak_pos = pos;
		pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
		{
			COb_schpin* ppin = (COb_schpin*)pobject;

			if(( m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ppin->m_nDisplayMode))
				{
					if(ppin->Is_On_Me(fx, fy) == TRUE) 
					{
						if( (m_bPinsMoveable == TRUE)&&((m_bShowHidePins == TRUE)||(ppin->m_bHide == FALSE)) ) 
						{
							multi_selection.element = SCH_ELEMENT_PIN;
							multi_selection.pos = bak_pos;
							return multi_selection;
						}
						else
						{
							multi_selection.element = SCH_ELEMENT_COMPONENT;
							multi_selection.pos = position;
							return multi_selection;
						}
					}
					else
					{
						fx1 = MIN(fx1, ppin->m_fX);
						fy1 = MIN(fy1, ppin->m_fY);
						fx2 = MAX(fx2, ppin->m_fX);
						fy2 = MAX(fy2, ppin->m_fY);
					}
				}

			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc = (COb_scharc*)pobject;
			if(( m_nPartCount <= 1)||(parc->m_nOwnerPartID == 0)||(parc->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == parc->m_nDisplayMode))
				{
					fx1 = MIN(fx1, (parc->m_fCx - parc->m_fRadius));
					fy1 = MIN(fy1, (parc->m_fCy - parc->m_fRadius));
					fx2 = MAX(fx2, (parc->m_fCx + parc->m_fRadius));
					fy2 = MAX(fy2, (parc->m_fCy + parc->m_fRadius));
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse = (COb_schellipse*)pobject;
			if(( m_nPartCount <= 1)||(pellipse->m_nOwnerPartID == 0)||(pellipse->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == pellipse->m_nDisplayMode))
				{
					fx1 = MIN(fx1, (pellipse->m_fCx - pellipse->m_fRadius_x));
					fy1 = MIN(fy1, (pellipse->m_fCy - pellipse->m_fRadius_y));
					fx2 = MAX(fx2, (pellipse->m_fCx + pellipse->m_fRadius_x));
					fy2 = MAX(fy2, (pellipse->m_fCy + pellipse->m_fRadius_y));
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
			if(( m_nPartCount <= 1)||(pellipsefill->m_nOwnerPartID == 0)||(pellipsefill->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == pellipsefill->m_nDisplayMode))
				{
					fx1 = MIN(fx1, (pellipsefill->m_fCx - pellipsefill->m_fRadius_x));
					fy1 = MIN(fy1, (pellipsefill->m_fCy - pellipsefill->m_fRadius_y));
					fx2 = MAX(fx2, (pellipsefill->m_fCx + pellipsefill->m_fRadius_x));
					fy2 = MAX(fy2, (pellipsefill->m_fCy + pellipsefill->m_fRadius_y));
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie = (COb_schpie*)pobject;
			if(( m_nPartCount <= 1)||(ppie->m_nOwnerPartID == 0)||(ppie->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ppie->m_nDisplayMode))
				{
					fx1 = MIN(fx1, (ppie->m_fCx - ppie->m_fRadius));
					fy1 = MIN(fy1, (ppie->m_fCy - ppie->m_fRadius));
					fx2 = MAX(fx2, (ppie->m_fCx + ppie->m_fRadius));
					fy2 = MAX(fy2, (ppie->m_fCy + ppie->m_fRadius));
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if(( m_nPartCount <= 1)||(ppolyline->m_nOwnerPartID == 0)||(ppolyline->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ppolyline->m_nDisplayMode))
				{
					for(int i=0; i<ppolyline->m_cVertex.GetCount(); i++)
					{
						fx1 = MIN(fx1, ppolyline->m_cVertex[i].fx);
						fy1 = MIN(fy1, ppolyline->m_cVertex[i].fy);
						fx2 = MAX(fx2, ppolyline->m_cVertex[i].fx);
						fy2 = MAX(fy2, ppolyline->m_cVertex[i].fy);
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect = (COb_schrect*)pobject;
			if(( m_nPartCount <= 1)||(prect->m_nOwnerPartID == 0)||(prect->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == prect->m_nDisplayMode))
				{
					fx1 = MIN(fx1, prect->m_fX1);
					fy1 = MIN(fy1, prect->m_fY1);
					fx2 = MAX(fx2, prect->m_fX1);
					fy2 = MAX(fy2, prect->m_fY1);
					fx1 = MIN(fx1, prect->m_fX2);
					fy1 = MIN(fy1, prect->m_fY2);
					fx2 = MAX(fx2, prect->m_fX2);
					fy2 = MAX(fy2, prect->m_fY2);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
			if(( m_nPartCount <= 1)||(proundrect->m_nOwnerPartID == 0)||(proundrect->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == proundrect->m_nDisplayMode))
				{
					fx1 = MIN(fx1, proundrect->m_fX1);
					fy1 = MIN(fy1, proundrect->m_fY1);
					fx2 = MAX(fx2, proundrect->m_fX1);
					fy2 = MAX(fy2, proundrect->m_fY1);
					fx1 = MIN(fx1, proundrect->m_fX2);
					fy1 = MIN(fy1, proundrect->m_fY2);
					fx2 = MAX(fx2, proundrect->m_fX2);
					fy2 = MAX(fy2, proundrect->m_fY2);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
			if(( m_nPartCount <= 1)||(ppolygon->m_nOwnerPartID == 0)||(ppolygon->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ppolygon->m_nDisplayMode))
				{
					for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
					{
						fx1 = MIN(fx1, ppolygon->m_cVertex[i].fx);
						fy1 = MIN(fy1, ppolygon->m_cVertex[i].fy);
						fx2 = MAX(fx2, ppolygon->m_cVertex[i].fx);
						fy2 = MAX(fy2, ppolygon->m_cVertex[i].fy);
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier = (COb_schbezier*)pobject;
			if(( m_nPartCount <= 1)||(pbezier->m_nOwnerPartID == 0)||(pbezier->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == pbezier->m_nDisplayMode))
				{
					for(int i=0; i<pbezier->m_cVertex.GetCount(); i++)
					{
						fx1 = MIN(fx1, pbezier->m_cVertex[i].fx);
						fy1 = MIN(fy1, pbezier->m_cVertex[i].fy);
						fx2 = MAX(fx2, pbezier->m_cVertex[i].fx);
						fy2 = MAX(fy2, pbezier->m_cVertex[i].fy);
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
			if(( m_nPartCount <= 1)||(ptextframe->m_nOwnerPartID == 0)||(ptextframe->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ptextframe->m_nDisplayMode))
				{
					fx1 = MIN(fx1, ptextframe->m_fX1);
					fy1 = MIN(fy1, ptextframe->m_fY1);
					fx2 = MAX(fx2, ptextframe->m_fX1);
					fy2 = MAX(fy2, ptextframe->m_fY1);
					fx1 = MIN(fx1, ptextframe->m_fX2);
					fy1 = MIN(fy1, ptextframe->m_fY2);
					fx2 = MAX(fx2, ptextframe->m_fX2);
					fy2 = MAX(fy2, ptextframe->m_fY2);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
			if(( m_nPartCount <= 1)||(pgraphic->m_nOwnerPartID == 0)||(pgraphic->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == pgraphic->m_nDisplayMode))
				{
					fx1 = MIN(fx1, pgraphic->m_fX1);
					fy1 = MIN(fy1, pgraphic->m_fY1);
					fx2 = MAX(fx2, pgraphic->m_fX1);
					fy2 = MAX(fy2, pgraphic->m_fY1);
					fx1 = MIN(fx1, pgraphic->m_fX2);
					fy1 = MIN(fy1, pgraphic->m_fY2);
					fx2 = MAX(fx2, pgraphic->m_fX2);
					fy2 = MAX(fy2, pgraphic->m_fY2);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext = (COb_schtext*)pobject;

			CString bakstr = ptext->m_csText;
			if((ptext->m_nID == TEXT_COMPDESIGNATOR)||(ptext->m_nID == TEXT_COMPCOMMENT)||(ptext->m_nID == TEXT_COMPPACKAGE)||(ptext->m_nID == TEXT_COMPTEXT))
			{
				if(ptext->m_bTextVisible == TRUE)
				{
					if(ptext->m_bNameVisible == TRUE)	ptext->m_csText = ptext->m_csNameComment + ":" + ptext->m_csText;
					if(ptext->Is_On_Me(fx, fy, m_nDiagram, pView) == TRUE)
					{
						ptext->m_csText = bakstr;

						multi_selection.element = SCH_ELEMENT_TEXT; 
						multi_selection.pos = bak_pos;
						return multi_selection;
					}
				}
			}

			ptext->m_csText = bakstr;
		}
	}

	if((fx1 == 100000)||(fy1 == 100000)||(fx2 == 0)||(fy2 == 0))
	{
		fx1 = m_fX - 100;
		fy1 = m_fY - 100;
		fx2 = m_fX + 100;
		fy2 = m_fY + 100;
	}

	CRgn   rgn;
	rgn.CreateRectRgn((int)fx1, (int)fy1, (int)fx2, (int)fy2);
	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE)
	{
		multi_selection.element = SCH_ELEMENT_COMPONENT;
		multi_selection.pos = position;
	}

	return multi_selection;
}

BOOL  COb_schcomp::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fx, fy;
	fx = m_fX;
	fy = m_fY;

	if((fx < fx1)||(fx > fx2)||(fy < fy1)||(fy > fy2)) return FALSE;
	return TRUE;
}

void COb_schcomp::Change(POSITION pos)
{
	BOOL undo_flag = FALSE; 
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_EditComp dlg;
	dlg.pDocument = pDocument;

	double fX_bak = m_fX;
	double fY_bak = m_fY;
	int orientation_bak = m_nOrientation;
	BOOL mirrored_bak = m_bMirrored;

	dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fX);
	dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fY);
	dlg.m_nComboOrientation = m_nOrientation/90;
	dlg.m_bCheckShowAllPins = m_bShowHidePins;
	if(m_bPinsMoveable == TRUE)	dlg.m_bCheckLockPins = FALSE;
	else dlg.m_bCheckLockPins = TRUE;
	dlg.m_bCheckMirrored = m_bMirrored;
	dlg.m_bCheckLocalColors = m_bColorLocked;
	dlg.m_nFillColor = m_nFillColor;
	dlg.m_nLineColor = m_nColor;
	dlg.m_nPinColor = m_nPinColor;
	dlg.m_nDisplayModeCount = m_nDisplayModeCount;
	dlg.m_nComboDisplayMode = m_nDisplayMode;

	dlg.m_csEditLibrary = m_csLibraryFileName;
	dlg.m_csEditModelName = m_csLibraryReference;

	dlg.m_csEditDesignator = GetDesignator();
	dlg.m_csEditComment = GetComment();
	dlg.m_csEditPackage = GetPackage();
	dlg.m_csEditDescription = m_csDescription;
	dlg.m_csEditUniqueID = m_csUniqueID;
	
	dlg.m_bCheckDesignatorVisible = GetDesignatorVisible();
	dlg.m_bCheckCommentVisible = GetCommentVisible();

	CObject* pobject;
	if(pDoc != NULL)
	{
		POSITION position;
		for(int i=0; i<m_arrayPos.GetSize(); i++)
		{
			position = m_arrayPos.GetAt(i);
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext;
				ptext = (COb_schtext*)pobject;
				if(ptext->m_nID == TEXT_COMPTEXT)
				{
					dlg.onetext.visible = ptext->m_bTextVisible;
					dlg.onetext.value = ptext->m_csText;
					dlg.onetext.item = ptext->m_csNameComment;
					dlg.m_arrayTextOthers.Add(dlg.onetext);
				}
			}
		}
	}

	dlg.m_nPartCount = m_nPartCount;
	dlg.m_nPart = m_nPart;

	int nReturn = dlg.DoModal();
	if((nReturn == IDOK) || (nReturn == 100) || (nReturn == 101) || (nReturn == 102))  
	{
		if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = SCH_ELEMENT_COMPONENT;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = pos;
		pundo->schcomp.Copy(this);
			
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;


		m_fX = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
		m_fY = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
		m_nOrientation = dlg.m_nComboOrientation*90;
		m_bShowHidePins = dlg.m_bCheckShowAllPins;
		
		if(dlg.m_bCheckLockPins == TRUE) m_bPinsMoveable = FALSE;
		else m_bPinsMoveable = TRUE;

		m_bMirrored	= dlg.m_bCheckMirrored;
		m_bColorLocked = dlg.m_bCheckLocalColors;
		m_nFillColor = dlg.m_nFillColor;
		m_nColor = dlg.m_nLineColor;
		m_nPinColor = dlg.m_nPinColor;
		m_nDisplayMode = dlg.m_nComboDisplayMode;
	
		m_csLibraryFileName = dlg.m_csEditLibrary;
		m_csLibraryReference = dlg.m_csEditModelName;
	
		SetDesignator(dlg.m_csEditDesignator, dlg.m_bCheckDesignatorVisible);
		SetComment(dlg.m_csEditComment, dlg.m_bCheckCommentVisible);
		SetPackage(dlg.m_csEditPackage);

		m_csDescription = dlg.m_csEditDescription;
		m_csUniqueID = dlg.m_csEditUniqueID;
		
		CObject* pobject;
		if(pDoc != NULL)
		{
			POSITION position;
			int index = 0;
			for(int i=0; i<m_arrayPos.GetSize(); i++)
			{
				position = m_arrayPos.GetAt(i);
				pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext;
					ptext = (COb_schtext*)pobject;
					if(ptext->m_nID == TEXT_COMPTEXT)
					{
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_TEXT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = position;
						pundo->schtext.Copy(ptext);
							
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

						ptext->m_bTextVisible = dlg.m_arrayTextOthers[index].visible;
						ptext->m_csText = dlg.m_arrayTextOthers[index].value;
						ptext->m_csNameComment = dlg.m_arrayTextOthers[index].item;
						index++;
					}
				}
			}
		}

		m_nPart = dlg.m_nPart;

		if((fX_bak != m_fX) || (fY_bak != m_fY) || (orientation_bak != m_nOrientation) || (mirrored_bak != m_bMirrored))
		{
			int rotation = m_nOrientation - orientation_bak;
			rotation = pDoc->Get_Correct_Orientation(rotation);
			BOOL mirror;
			if(mirrored_bak == m_bMirrored) mirror = FALSE;
			else mirror = TRUE;
			double fx0, fy0, fx1, fy1, deltax, deltay;
			fx0 = fX_bak;
			fy0 = fY_bak;
			deltax = m_fX - fx0;
			deltay = m_fY - fy0;

			for(int i=0; i<m_arrayPos.GetSize(); i++)
			{
				CObject* pobject;
				POSITION subpos;
				subpos = m_arrayPos.GetAt(i);
				pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
				{
					COb_schpin* ppin = (COb_schpin*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_PIN;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schpin.Copy(ppin);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, ppin->m_fX - fx0, ppin->m_fY - fy0, rotation, mirror);
					ppin->m_fX = fx1 + fx0 + deltax;
					ppin->m_fY = fy1 + fy0 + deltay;

					int orientation;
					pDoc->Get_Rotate_Orientation(&orientation, ppin->m_nOrientation, rotation, mirror);
					ppin->m_nOrientation = orientation;
					ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);					
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
				{
					COb_schieee* pieee = (COb_schieee*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_IEEE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schieee.Copy(pieee);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, pieee->m_fX - fx0, pieee->m_fY - fy0, rotation, mirror);
					pieee->m_fX = fx1 + fx0 + deltax;
					pieee->m_fY = fy1 + fy0 + deltay;
					int orientation;
					pDoc->Get_Rotate_Orientation(&orientation, pieee->m_nOrientation, rotation, mirror);
					pieee->m_nOrientation = orientation;
					pieee->m_nOrientation = pDoc->Get_Correct_Orientation(pieee->m_nOrientation);
		
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext = (COb_schtext*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_TEXT;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schtext.Copy(ptext);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, ptext->m_fSx - fx0, ptext->m_fSy - fy0, rotation, mirror);
					ptext->m_fSx = fx1 + fx0 + deltax;
					ptext->m_fSy = fy1 + fy0 + deltay;
					int orientation;
					pDoc->Get_Rotate_Orientation(&orientation, ptext->m_nOrientation, rotation, FALSE); //mirror);
					ptext->m_nOrientation = orientation;
					ptext->m_nOrientation = pDoc->Get_Correct_Orientation(ptext->m_nOrientation);
		
					if(mirror == TRUE)
					{
						if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
						else ptext->m_bMirror = TRUE;
						if((ptext->m_nOrientation == 0)||(ptext->m_nOrientation == 180))
						{
							if(ptext->m_nH_Justification == 0) ptext->m_nH_Justification = 2;
							else if(ptext->m_nH_Justification == 2) ptext->m_nH_Justification = 0;
						}
						else
						{
							if(ptext->m_nV_Justification == 0) ptext->m_nV_Justification = 2;
							else if(ptext->m_nV_Justification == 2) ptext->m_nV_Justification = 0;
						}
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
				{
					COb_schbezier* pbezier = (COb_schbezier*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_BEZIER;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schbezier.Copy(pbezier);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
					{
						pDoc->Get_Rotate_XY(&fx1, &fy1, pbezier->m_cVertex[k].fx-fx0, pbezier->m_cVertex[k].fy-fy0, rotation, mirror);
						pbezier->m_cVertex[k].fx = fx1 + fx0 + deltax;
						pbezier->m_cVertex[k].fy = fy1 + fy0 + deltay;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
				{
					COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_POLYLINE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schpolyline.Copy(ppolyline);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
					{
						pDoc->Get_Rotate_XY(&fx1, &fy1, ppolyline->m_cVertex[k].fx-fx0, ppolyline->m_cVertex[k].fy-fy0, rotation, mirror);
						ppolyline->m_cVertex[k].fx = fx1 + fx0 + deltax;
						ppolyline->m_cVertex[k].fy = fy1 + fy0 + deltay;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
				{
					COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_POLYGON;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schpolygon.Copy(ppolygon);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
					{
						pDoc->Get_Rotate_XY(&fx1, &fy1, ppolygon->m_cVertex[k].fx-fx0, ppolygon->m_cVertex[k].fy-fy0, rotation, mirror);
						ppolygon->m_cVertex[k].fx = fx1 + fx0 + deltax;
						ppolygon->m_cVertex[k].fy = fy1 + fy0 + deltay;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
				{
					COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schellipsefill.Copy(pellipsefill);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, pellipsefill->m_fCx - fx0, pellipsefill->m_fCy - fy0, rotation, mirror);
					pellipsefill->m_fCx = fx1 + fx0 + deltax;
					pellipsefill->m_fCy = fy1 + fy0 + deltay;
					if((rotation == 90)||(rotation == 270))
					{
						double ftemp;
						ftemp = pellipsefill->m_fRadius_y;
						pellipsefill->m_fRadius_y = pellipsefill->m_fRadius_x;
						pellipsefill->m_fRadius_x = ftemp;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
				{
					COb_schpie* ppie = (COb_schpie*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_PIE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schpie.Copy(ppie);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, ppie->m_fCx - fx0, ppie->m_fCy - fy0, rotation, mirror);
					ppie->m_fCx = fx1 + fx0 + deltax;
					ppie->m_fCy = fy1 + fy0 + deltay;
					
					double salpha, ealpha;
					pDoc->Get_Rotate_Angle(&salpha, &ealpha, ppie->m_fSangle, ppie->m_fEangle, rotation, mirror);
					ppie->m_fSangle = salpha;
					ppie->m_fSangle = pDoc->Get_Correct_Angle(ppie->m_fSangle);
					ppie->m_fEangle = ealpha;
					ppie->m_fEangle = pDoc->Get_Correct_Angle(ppie->m_fEangle);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
				{
					COb_schroundrect* proundrect = (COb_schroundrect*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schroundrect.Copy(proundrect);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX1 - fx0, proundrect->m_fY1 - fy0, rotation, mirror);
					proundrect->m_fX1 = fx1 + fx0 + deltax;
					proundrect->m_fY1 = fy1 + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX2 - fx0, proundrect->m_fY2 - fy0, rotation, mirror);
					proundrect->m_fX2 = fx1 + fx0 + deltax;
					proundrect->m_fY2 = fy1 + fy0 + deltay;
					
					double fminx, fminy, fmaxx, fmaxy;
					fminx = MIN(proundrect->m_fX1, proundrect->m_fX2);
					fmaxx = MAX(proundrect->m_fX1, proundrect->m_fX2);
					fminy = MIN(proundrect->m_fY1, proundrect->m_fY2);
					fmaxy = MAX(proundrect->m_fY1, proundrect->m_fY2);
					proundrect->m_fX1 = fminx;
					proundrect->m_fY1 = fminy;
					proundrect->m_fX2 = fmaxx;
					proundrect->m_fY2 = fmaxy;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
				{
					COb_schellipse* pellipse = (COb_schellipse*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schellipse.Copy(pellipse);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, pellipse->m_fCx - fx0, pellipse->m_fCy - fy0, rotation, mirror);
					pellipse->m_fCx = fx1 + fx0 + deltax;
					pellipse->m_fCy = fy1 + fy0 + deltay;
					
					double salpha, ealpha;
					pDoc->Get_Rotate_Angle(&salpha, &ealpha, pellipse->m_fSangle, pellipse->m_fEangle, rotation, mirror);
					pellipse->m_fSangle = salpha;
					pellipse->m_fSangle = pDoc->Get_Correct_Angle(pellipse->m_fSangle);
					pellipse->m_fEangle = ealpha;
					pellipse->m_fEangle = pDoc->Get_Correct_Angle(pellipse->m_fEangle);
					if((rotation == 90)||(rotation == 270))
					{
						double ftemp;
						ftemp = pellipse->m_fRadius_y;
						pellipse->m_fRadius_y = pellipse->m_fRadius_x;
						pellipse->m_fRadius_x = ftemp;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
				{
					COb_scharc* parc = (COb_scharc*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_ARC;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->scharc.Copy(parc);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, parc->m_fCx - fx0, parc->m_fCy - fy0, rotation, mirror);
					parc->m_fCx = fx1 + fx0 + deltax;
					parc->m_fCy = fy1 + fy0 + deltay;
					
					double salpha, ealpha;
					pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, rotation, mirror);
					parc->m_fSangle = salpha;
					parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
					parc->m_fEangle = ealpha;
					parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
				{
					COb_schrect* prect = (COb_schrect*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_RECT;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schrect.Copy(prect);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX1 - fx0, prect->m_fY1 - fy0, rotation, mirror);
					prect->m_fX1 = fx1 + fx0 + deltax;
					prect->m_fY1 = fy1 + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX2 - fx0, prect->m_fY2 - fy0, rotation, mirror);
					prect->m_fX2 = fx1 + fx0 + deltax;
					prect->m_fY2 = fy1 + fy0 + deltay;
					
					double fminx, fminy, fmaxx, fmaxy;
					fminx = MIN(prect->m_fX1, prect->m_fX2);
					fmaxx = MAX(prect->m_fX1, prect->m_fX2);
					fminy = MIN(prect->m_fY1, prect->m_fY2);
					fmaxy = MAX(prect->m_fY1, prect->m_fY2);
					prect->m_fX1 = fminx;
					prect->m_fY1 = fminy;
					prect->m_fX2 = fmaxx;
					prect->m_fY2 = fmaxy;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
				{
					COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schtextframe.Copy(ptextframe);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX1 - fx0, ptextframe->m_fY1 - fy0, rotation, mirror);
					ptextframe->m_fX1 = fx1 + fx0 + deltax;
					ptextframe->m_fY1 = fy1 + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX2 - fx0, ptextframe->m_fY2 - fy0, rotation, mirror);
					ptextframe->m_fX2 = fx1 + fx0 + deltax;
					ptextframe->m_fY2 = fy1 + fy0 + deltay;
					
					double fminx, fminy, fmaxx, fmaxy;
					fminx = MIN(ptextframe->m_fX1, ptextframe->m_fX2);
					fmaxx = MAX(ptextframe->m_fX1, ptextframe->m_fX2);
					fminy = MIN(ptextframe->m_fY1, ptextframe->m_fY2);
					fmaxy = MAX(ptextframe->m_fY1, ptextframe->m_fY2);
					ptextframe->m_fX1 = fminx;
					ptextframe->m_fY1 = fminy;
					ptextframe->m_fX2 = fmaxx;
					ptextframe->m_fY2 = fmaxy;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
				{
					COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schgraphic.Copy(pgraphic);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX1 - fx0, pgraphic->m_fY1 - fy0, rotation, mirror);
					pgraphic->m_fX1 = fx1 + fx0 + deltax;
					pgraphic->m_fY1 = fy1 + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX2 - fx0, pgraphic->m_fY2 - fy0, rotation, mirror);
					pgraphic->m_fX2 = fx1 + fx0 + deltax;
					pgraphic->m_fY2 = fy1 + fy0 + deltay;
					
					double fminx, fminy, fmaxx, fmaxy;
					fminx = MIN(pgraphic->m_fX1, pgraphic->m_fX2);
					fmaxx = MAX(pgraphic->m_fX1, pgraphic->m_fX2);
					fminy = MIN(pgraphic->m_fY1, pgraphic->m_fY2);
					fmaxy = MAX(pgraphic->m_fY1, pgraphic->m_fY2);
					pgraphic->m_fX1 = fminx;
					pgraphic->m_fY1 = fminy;
					pgraphic->m_fX2 = fmaxx;
					pgraphic->m_fY2 = fmaxy;
				}
			}			
		}
	}
	
	if( nReturn == 100) 
	{
		CDlg_SelectPin dlg_pin;
		
		dlg_pin.m_nPartCount = m_nPartCount;
		dlg_pin.m_nPart = m_nPart;
		CObject* pobject;
		for(int i=0; i<m_arrayPos.GetSize(); i++)
		{
			POSITION position = m_arrayPos.GetAt(i);
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
			{
				COb_schpin* ppin = (COb_schpin*)pobject;
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ppin->m_nDisplayMode))
				{
					COb_schpin* ppin_copy = new COb_schpin;

					ppin_copy->Copy(ppin);

					dlg_pin.m_arrayPins.Add(ppin_copy);
				}
			}
		}
		dlg_pin.m_arrayRecords.SetSize(dlg_pin.m_arrayPins.GetCount());

		if(dlg_pin.DoModal() == IDOK)
		{
			
			int index = 0;
			for(int k=0; k<m_arrayPos.GetSize(); k++)
			{
				POSITION position = m_arrayPos.GetAt(k);
				pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
				{
                    COb_schpin* ppin = (COb_schpin*)pobject;
					if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ppin->m_nDisplayMode))
					{
						COb_schpin* ppin_edit = dlg_pin.m_arrayPins.GetAt(index);

						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_PIN;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = position;
						pundo->schpin.Copy(ppin);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						ppin->Copy(ppin_edit);
						index++;
					}
				}
			}

		}

		for(int i=0; i<dlg_pin.m_arrayPins.GetCount(); i++)
		{
			COb_schpin* ppin_copy = (COb_schpin*)dlg_pin.m_arrayPins.GetAt(i);
			delete ppin_copy;
		}
	}
	
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	if(nReturn == 101)
	{
		POSITION position;
		for(int i=0; i<m_arrayPos.GetSize(); i++)
		{
			position = m_arrayPos.GetAt(i);
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext;
				ptext = (COb_schtext*)pobject;
				if(ptext->m_nID == TEXT_COMPDESIGNATOR)
				{
					ptext->Change(position);
				}
			}
		}
	}
	
	if(nReturn == 102)
	{
		POSITION position;
		for(int i=0; i<m_arrayPos.GetSize(); i++)
		{
			position = m_arrayPos.GetAt(i);
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext;
				ptext = (COb_schtext*)pobject;
				if(ptext->m_nID == TEXT_COMPCOMMENT)
				{
					ptext->Change(position);
				}
			}
		}
	}
}

CString COb_schcomp::GetDesignator(void)
{
	CObject* pobject;
	CString str="";
	CESDDoc* pESDDoc;

	pESDDoc = (CESDDoc*)pDocument;
	if(pESDDoc == NULL) return str;

	POSITION pos;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		pos = m_arrayPos.GetAt(i);
		pobject = pESDDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if(ptext->m_nID == TEXT_COMPDESIGNATOR) return ptext->m_csText;
		}
	}

	return str;
}

CString COb_schcomp::GetComment(void)
{
	CObject* pobject;
	CString str="";
	CESDDoc* pESDDoc;

	pESDDoc = (CESDDoc*)pDocument;
	if(pESDDoc == NULL) return str;

	POSITION pos;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		pos = m_arrayPos.GetAt(i);
		pobject = pESDDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if(ptext->m_nID == TEXT_COMPCOMMENT) return ptext->m_csText;
		}
	}

	return str;
}

BOOL COb_schcomp::GetCommentVisible(void)
{
	CObject* pobject;
	CESDDoc* pESDDoc;

	pESDDoc = (CESDDoc*)pDocument;
	if(pESDDoc == NULL) return FALSE;

	POSITION pos;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		pos = m_arrayPos.GetAt(i);
		pobject = pESDDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if(ptext->m_nID == TEXT_COMPCOMMENT) return ptext->m_bTextVisible;
		}
	}

	return FALSE;
}

BOOL COb_schcomp::GetDesignatorVisible(void)
{
	CObject* pobject;
	CESDDoc* pESDDoc;

	pESDDoc = (CESDDoc*)pDocument;
	if(pESDDoc == NULL) return FALSE;

	POSITION pos;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		pos = m_arrayPos.GetAt(i);
		pobject = pESDDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if(ptext->m_nID == TEXT_COMPDESIGNATOR) return ptext->m_bTextVisible;
		}
	}

	return FALSE;
}

CString COb_schcomp::GetPackage(void)
{
	CObject* pobject;
	CString str="";
	CESDDoc* pESDDoc;

	pESDDoc = (CESDDoc*)pDocument;
	if(pESDDoc == NULL) return str;

	POSITION pos;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		pos = m_arrayPos.GetAt(i);
		pobject = pESDDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if(ptext->m_nID == TEXT_COMPPACKAGE) return ptext->m_csText;
		}
	}

	return str;
}

void COb_schcomp::SetDesignator(CString& cstring, BOOL bVisible)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	if(pDoc == NULL) return;

	POSITION pos;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		pos = m_arrayPos.GetAt(i);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext = (COb_schtext*)pobject;
			if(ptext->m_nID == TEXT_COMPDESIGNATOR)
			{

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schtext.Copy(ptext);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				ptext->m_csText = cstring;
				ptext->m_bTextVisible = bVisible;
				break;
			}
		}
	}
}

void COb_schcomp::SetComment(CString& cstring, BOOL bVisible)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	if(pDoc == NULL) return;

	POSITION pos;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		pos = m_arrayPos.GetAt(i);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if(ptext->m_nID == TEXT_COMPCOMMENT)
			{
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schtext.Copy(ptext);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				ptext->m_csText = cstring;
				ptext->m_bTextVisible = bVisible;
				break;
			}
		}
	}
}

void COb_schcomp::SetPackage(CString& cstring)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	if(pDoc == NULL) return;

	POSITION pos;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		pos = m_arrayPos.GetAt(i);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if(ptext->m_nID == TEXT_COMPPACKAGE)
			{
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schtext.Copy(ptext);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				ptext->m_csText = cstring;
				break;
			}
		}
	}
}

CSize COb_schcomp::GetCompXYSize()
{
	int xsize, ysize;
	int xmin = 99999, xmax = 0;
	int ymin = 99999, ymax = 0;
	CSize csize; csize.cx = 10; csize.cy = 10;

	CObject* pobject;
	CESDDoc* pESDDoc;

	pESDDoc = (CESDDoc*)pDocument;
	if(pESDDoc == NULL) return csize;

	POSITION pos;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		pos = m_arrayPos.GetAt(i);
		pobject = pESDDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
		{
			COb_schpin* ppin;
			ppin = (COb_schpin*)pobject;

			if(xmin > (int)ppin->m_fX) xmin = (int)ppin->m_fX;
			if(xmax < (int)ppin->m_fX) xmax = (int)ppin->m_fX;
			if(ymin > (int)ppin->m_fY) ymin = (int)ppin->m_fY;
			if(ymax < (int)ppin->m_fY) ymax = (int)ppin->m_fY;
		}
	}

	if(xmin >= xmax) xsize = 10;
	else xsize = (xmax - xmin)/10;
	if(ymin >= ymax) ysize = 10;
	else ysize = (ymax - ymin)/10;
	if((m_nOrientation == 0)||(m_nOrientation == 180)) 
	{
		csize.cx = xsize;
		csize.cy = ysize;
	}
	else 
	{
		csize.cx = ysize;
		csize.cy = xsize;
	}

	return csize;
}

RECT COb_schcomp::GetCompRect()
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	double fminx, fminy, fmaxx, fmaxy;
	fminx = fminy = 100000;
	fmaxx = fmaxy = -100000;

	POSITION pos;
	for(int j=0; j<m_arrayPos.GetSize(); j++)
	{
		pos = m_arrayPos.GetAt(j);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
		{
			COb_schpin* ppin = (COb_schpin*)pobject;
			if(( m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( ppin->m_nDisplayMode == m_nDisplayMode))
				{
					if(ppin->m_bHide != TRUE)
					{
						fminx = MIN(fminx, ppin->m_fX);
						fminy = MIN(fminy, ppin->m_fY);
						fmaxx = MAX(fmaxx, ppin->m_fX);
						fmaxy = MAX(fmaxy, ppin->m_fY);
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc = (COb_scharc*)pobject;
			if(( m_nPartCount <= 1)||(parc->m_nOwnerPartID == 0)||(parc->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == parc->m_nDisplayMode))
				{
					if((parc->m_fSangle == parc->m_fEangle)||((parc->m_fEangle - parc->m_fSangle) == 360))
					{
						fminx = MIN(fminx, (parc->m_fCx - parc->m_fRadius));
						fminy = MIN(fminy, (parc->m_fCy - parc->m_fRadius));
						fmaxx = MAX(fmaxx, (parc->m_fCx + parc->m_fRadius));
						fmaxy = MAX(fmaxy, (parc->m_fCy + parc->m_fRadius));
					}
					else if(parc->m_fEangle > parc->m_fSangle)
					{
						if((180 > parc->m_fSangle)&&(180 < parc->m_fEangle)) fminx = MIN(fminx, (parc->m_fCx - parc->m_fRadius));
						else fminx = MIN(fminx, parc->m_fCx);

						if((270 > parc->m_fSangle)&&(270 < parc->m_fEangle)) fminy = MIN(fminy, (parc->m_fCy - parc->m_fRadius));
						else fminy = MIN(fminy, parc->m_fCy);

						fmaxx = MAX(fmaxx, parc->m_fCx);

						if((90 > parc->m_fSangle)&&(90 < parc->m_fEangle)) fmaxy = MAX(fmaxy, (parc->m_fCy + parc->m_fRadius));
						else fmaxy = MAX(fmaxy, parc->m_fCy);
					}
					else
					{
						if((180 > parc->m_fSangle)||(180 < parc->m_fEangle)) fminx = MIN(fminx, (parc->m_fCx - parc->m_fRadius));
						else fminx = MIN(fminx, parc->m_fCx);

						if((270 > parc->m_fSangle)||(270 < parc->m_fEangle)) fminy = MIN(fminy, (parc->m_fCy - parc->m_fRadius));
						else fminy = MIN(fminy, parc->m_fCy);

						fmaxx = MAX(fmaxx, (parc->m_fCx + parc->m_fRadius));

						if((90 > parc->m_fSangle)||(90 < parc->m_fEangle)) fmaxy = MAX(fmaxy, (parc->m_fCy + parc->m_fRadius));
						else fmaxy = MAX(fmaxy, parc->m_fCy);
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse = (COb_schellipse*)pobject;
			if(( m_nPartCount <= 1)||(pellipse->m_nOwnerPartID == 0)||(pellipse->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == pellipse->m_nDisplayMode))
				{
					if((pellipse->m_fSangle == pellipse->m_fEangle)||((pellipse->m_fEangle - pellipse->m_fSangle) == 360))
					{
						fminx = MIN(fminx, (pellipse->m_fCx - pellipse->m_fRadius_x));
						fminy = MIN(fminy, (pellipse->m_fCy - pellipse->m_fRadius_y));
						fmaxx = MAX(fmaxx, (pellipse->m_fCx + pellipse->m_fRadius_x));
						fmaxy = MAX(fmaxy, (pellipse->m_fCy + pellipse->m_fRadius_y));
					}
					else if(pellipse->m_fEangle > pellipse->m_fSangle)
					{
						if((180 > pellipse->m_fSangle)&&(180 < pellipse->m_fEangle)) fminx = MIN(fminx, (pellipse->m_fCx - pellipse->m_fRadius_x));
						else fminx = MIN(fminx, pellipse->m_fCx);

						if((270 > pellipse->m_fSangle)&&(270 < pellipse->m_fEangle)) fminy = MIN(fminy, (pellipse->m_fCy - pellipse->m_fRadius_y));
						else fminy = MIN(fminy, pellipse->m_fCy);

						fmaxx = MAX(fmaxx, pellipse->m_fCx);

						if((90 > pellipse->m_fSangle)&&(90 < pellipse->m_fEangle)) fmaxy = MAX(fmaxy, (pellipse->m_fCy + pellipse->m_fRadius_y));
						else fmaxy = MAX(fmaxy, pellipse->m_fCy);
					}
					else
					{
						if((180 > pellipse->m_fSangle)||(180 < pellipse->m_fEangle)) fminx = MIN(fminx, (pellipse->m_fCx - pellipse->m_fRadius_x));
						else fminx = MIN(fminx, pellipse->m_fCx);

						if((270 > pellipse->m_fSangle)||(270 < pellipse->m_fEangle)) fminy = MIN(fminy, (pellipse->m_fCy - pellipse->m_fRadius_y));
						else fminy = MIN(fminy, pellipse->m_fCy);

						fmaxx = MAX(fmaxx, (pellipse->m_fCx + pellipse->m_fRadius_x));

						if((90 > pellipse->m_fSangle)||(90 < pellipse->m_fEangle)) fmaxy = MAX(fmaxy, (pellipse->m_fCy + pellipse->m_fRadius_y));
						else fmaxy = MAX(fmaxy, pellipse->m_fCy);
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
			if(( m_nPartCount <= 1)||(pellipsefill->m_nOwnerPartID == 0)||(pellipsefill->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == pellipsefill->m_nDisplayMode))
				{
					fminx = MIN(fminx, (pellipsefill->m_fCx - pellipsefill->m_fRadius_x));
					fminy = MIN(fminy, (pellipsefill->m_fCy - pellipsefill->m_fRadius_y));
					fmaxx = MAX(fmaxx, (pellipsefill->m_fCx + pellipsefill->m_fRadius_x));
					fmaxy = MAX(fmaxy, (pellipsefill->m_fCy + pellipsefill->m_fRadius_y));
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie = (COb_schpie*)pobject;
			if(( m_nPartCount <= 1)||(ppie->m_nOwnerPartID == 0)||(ppie->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ppie->m_nDisplayMode))
				{
					fminx = MIN(fminx, (ppie->m_fCx - ppie->m_fRadius));
					fminy = MIN(fminy, (ppie->m_fCy - ppie->m_fRadius));
					fmaxx = MAX(fmaxx, (ppie->m_fCx + ppie->m_fRadius));
					fmaxy = MAX(fmaxy, (ppie->m_fCy + ppie->m_fRadius));
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if(( m_nPartCount <= 1)||(ppolyline->m_nOwnerPartID == 0)||(ppolyline->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ppolyline->m_nDisplayMode))
				{
					for(int i=0; i<ppolyline->m_cVertex.GetCount(); i++)
					{
						fminx = MIN(fminx, ppolyline->m_cVertex[i].fx);
						fminy = MIN(fminy, ppolyline->m_cVertex[i].fy);
						fmaxx = MAX(fmaxx, ppolyline->m_cVertex[i].fx);
						fmaxy = MAX(fmaxy, ppolyline->m_cVertex[i].fy);
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect = (COb_schrect*)pobject;
			if(( m_nPartCount <= 1)||(prect->m_nOwnerPartID == 0)||(prect->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == prect->m_nDisplayMode))
				{
					fminx = MIN(fminx, prect->m_fX1);
					fminy = MIN(fminy, prect->m_fY1);
					fmaxx = MAX(fmaxx, prect->m_fX1);
					fmaxy = MAX(fmaxy, prect->m_fY1);
					fminx = MIN(fminx, prect->m_fX2);
					fminy = MIN(fminy, prect->m_fY2);
					fmaxx = MAX(fmaxx, prect->m_fX2);
					fmaxy = MAX(fmaxy, prect->m_fY2);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
			if(( m_nPartCount <= 1)||(proundrect->m_nOwnerPartID == 0)||(proundrect->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == proundrect->m_nDisplayMode))
				{
					fminx = MIN(fminx, proundrect->m_fX1);
					fminy = MIN(fminy, proundrect->m_fY1);
					fmaxx = MAX(fmaxx, proundrect->m_fX1);
					fmaxy = MAX(fmaxy, proundrect->m_fY1);
					fminx = MIN(fminx, proundrect->m_fX2);
					fminy = MIN(fminy, proundrect->m_fY2);
					fmaxx = MAX(fmaxx, proundrect->m_fX2);
					fmaxy = MAX(fmaxy, proundrect->m_fY2);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
			if(( m_nPartCount <= 1)||(ppolygon->m_nOwnerPartID == 0)||(ppolygon->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ppolygon->m_nDisplayMode))
				{
					for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
					{
						fminx = MIN(fminx, ppolygon->m_cVertex[i].fx);
						fminy = MIN(fminy, ppolygon->m_cVertex[i].fy);
						fmaxx = MAX(fmaxx, ppolygon->m_cVertex[i].fx);
						fmaxy = MAX(fmaxy, ppolygon->m_cVertex[i].fy);
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier = (COb_schbezier*)pobject;
			if(( m_nPartCount <= 1)||(pbezier->m_nOwnerPartID == 0)||(pbezier->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == pbezier->m_nDisplayMode))
				{
					for(int i=0; i<pbezier->m_cVertex.GetCount(); i++)
					{
						fminx = MIN(fminx, pbezier->m_cVertex[i].fx);
						fminy = MIN(fminy, pbezier->m_cVertex[i].fy);
						fmaxx = MAX(fmaxx, pbezier->m_cVertex[i].fx);
						fmaxy = MAX(fmaxy, pbezier->m_cVertex[i].fy);
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
			if(( m_nPartCount <= 1)||(ptextframe->m_nOwnerPartID == 0)||(ptextframe->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == ptextframe->m_nDisplayMode))
				{
					fminx = MIN(fminx, ptextframe->m_fX1);
					fminy = MIN(fminy, ptextframe->m_fY1);
					fmaxx = MAX(fmaxx, ptextframe->m_fX1);
					fmaxy = MAX(fmaxy, ptextframe->m_fY1);
					fminx = MIN(fminx, ptextframe->m_fX2);
					fminy = MIN(fminy, ptextframe->m_fY2);
					fmaxx = MAX(fmaxx, ptextframe->m_fX2);
					fmaxy = MAX(fmaxy, ptextframe->m_fY2);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
			if(( m_nPartCount <= 1)||(pgraphic->m_nOwnerPartID == 0)||(pgraphic->m_nOwnerPartID ==  m_nPart))
			{
				if(( m_nDisplayModeCount <= 1)||( m_nDisplayMode == pgraphic->m_nDisplayMode))
				{
					fminx = MIN(fminx, pgraphic->m_fX1);
					fminy = MIN(fminy, pgraphic->m_fY1);
					fmaxx = MAX(fmaxx, pgraphic->m_fX1);
					fmaxy = MAX(fmaxy, pgraphic->m_fY1);
					fminx = MIN(fminx, pgraphic->m_fX2);
					fminy = MIN(fminy, pgraphic->m_fY2);
					fmaxx = MAX(fmaxx, pgraphic->m_fX2);
					fmaxy = MAX(fmaxy, pgraphic->m_fY2);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext = (COb_schtext*)pobject;
			if((m_nPartCount <= 1)||(ptext->m_nOwnerPartID == 0)||(ptext->m_nOwnerPartID == m_nPart))
			{
				if((m_nDisplayModeCount <= 1)||(m_nDisplayMode == ptext->m_nDisplayMode))
				{
					
				}
			}
		}
	}
	
	if((fminx == 100000)||(fminy == 100000)||(fmaxx == -100000)||(fmaxy == -100000))
	{
		fminx = m_fX;
		fmaxx = m_fX + 100;
		fminy = m_fY - 100;
		fmaxy = m_fY;
	}

	RECT rect;
	rect.left = (int)fminx;
	rect.right = (int)fmaxx;
	rect.bottom = (int)fminy;
	rect.top = (int)fmaxy;

	return rect;
}

void COb_schcomp::Copy(COb_schcomp* pschcomp)
{
	m_fX = pschcomp->m_fX;
	m_fY = pschcomp->m_fY;
	m_nOrientation = pschcomp->m_nOrientation;
	m_bMirrored = pschcomp->m_bMirrored;

	m_nPartCount = pschcomp->m_nPartCount;
	m_nPart = pschcomp->m_nPart;

	m_nDisplayModeCount = pschcomp->m_nDisplayModeCount;
	m_nDisplayMode = pschcomp->m_nDisplayMode;

	m_bPartLocked = pschcomp->m_bPartLocked;
	m_bShowHidePins = pschcomp->m_bShowHidePins;

	m_bPinsMoveable = pschcomp->m_bPinsMoveable;
	m_bColorLocked = pschcomp->m_bColorLocked;

	m_nColor = pschcomp->m_nColor;
	m_nFillColor = pschcomp->m_nFillColor;
	m_nPinColor = pschcomp->m_nPinColor;

	m_arrayPos.RemoveAll();
	for(int i=0; i<pschcomp->m_arrayPos.GetCount(); i++)
	{
		POSITION pos;
		pos = pschcomp->m_arrayPos.GetAt(i);
		m_arrayPos.Add(pos);
	}

    m_nGraphicalType = pschcomp->m_nGraphicalType;
	m_bDesignatorLocked = pschcomp->m_bDesignatorLocked;

	m_csDescription = pschcomp->m_csDescription;
	m_csLibraryReference = pschcomp->m_csLibraryReference;
	m_csLibraryFileName = pschcomp->m_csLibraryFileName;
	m_csTargetFileName = pschcomp->m_csTargetFileName;
	m_csLibraryPath = pschcomp->m_csLibraryPath;

	m_csUniqueID = pschcomp->m_csUniqueID;

	m_nFlag = pschcomp->m_nFlag;
	m_bSelection = pschcomp->m_bSelection;
	m_nDiagram = pschcomp->m_nDiagram;

	pDocument = pschcomp->pDocument;
}

void COb_schcomp::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	double fx0, fy0, fx1, fy1, deltax, deltay;
	int flag;
	BOOL mirror;
	BOOL clear_undo_buffer_flag = FALSE;
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动元件: <空格>旋转, <X>镜像, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	pView->Invalidate();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	
	RECT rect = GetCompRect();

	COb_schrect arect;
	arect.pDocument = pDoc;
	arect.m_fX1 = rect.left;
	arect.m_fY1 = rect.bottom;
	arect.m_fX2 = rect.right;
	arect.m_fY2 = rect.top;
	arect.m_nBorderWidth = 1;
	arect.m_nFillColor = m_nFillColor;
	arect.m_nBorderColor = m_nColor;
	arect.m_bTransparent = FALSE;
	arect.m_bFillSolid = TRUE;
	arect.m_nDiagram = m_nDiagram;
	arect.Draw(pDC, pV);

	CTypedPtrArray <CObArray, COb_schpin*> arrayPins;
	CTypedPtrArray <CObArray, COb_schpin*> arrayPinsBak;
	double pin_x, pin_y, fd = 100000;
	pin_x = arect.m_fX1;
	pin_y = arect.m_fY2;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		CObject* pobject;
		POSITION subpos;
		subpos = m_arrayPos.GetAt(i);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
		{
			COb_schpin* ppin = (COb_schpin*)pobject;
			if((ppin->m_nFlag != FLAG_NORMAL)||(ppin->m_bHide == TRUE)) continue;
			if((m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID == m_nPart))
			{
				if((m_nDisplayModeCount <= 1)||(m_nDisplayMode == ppin->m_nDisplayMode))
				{
					COb_schpin* ppin_copy;
					ppin_copy = new COb_schpin;
					ppin_copy->Copy(ppin);
					arrayPins.Add(ppin_copy);
					ppin_copy = new COb_schpin;
					ppin_copy->Copy(ppin);
					arrayPinsBak.Add(ppin_copy);

					double px, py;
					if(ppin->m_nOrientation == 0)
					{
						px = ppin->m_fX + ppin->m_fPinLength;
						py = ppin->m_fY;
						double dis = sqrt((px - pView->m_fCurrentSnapX)*(px - pView->m_fCurrentSnapX) + (py - pView->m_fCurrentSnapY)*(py - pView->m_fCurrentSnapY));
						if(fd > dis )
						{
							pin_x = px;
							pin_y = py;
							fd = dis;
						}
					}
					else if(ppin->m_nOrientation == 90)
					{
						px = ppin->m_fX;
						py = ppin->m_fY + ppin->m_fPinLength;
						double dis = sqrt((px - pView->m_fCurrentSnapX)*(px - pView->m_fCurrentSnapX) + (py - pView->m_fCurrentSnapY)*(py - pView->m_fCurrentSnapY));
						if(fd > dis )
						{
							pin_x = px;
							pin_y = py;
							fd = dis;
						}
					}
					else if(ppin->m_nOrientation == 180)
					{
						px = ppin->m_fX - ppin->m_fPinLength;
						py = ppin->m_fY;
						double dis = sqrt((px - pView->m_fCurrentSnapX)*(px - pView->m_fCurrentSnapX) + (py - pView->m_fCurrentSnapY)*(py - pView->m_fCurrentSnapY));
						if(fd > dis )
						{
							pin_x = px;
							pin_y = py;
							fd = dis;
						}
					}
					else if(ppin->m_nOrientation == 270)
					{
						px = ppin->m_fX;
						py = ppin->m_fY - ppin->m_fPinLength;
						double dis = sqrt((px - pView->m_fCurrentSnapX)*(px - pView->m_fCurrentSnapX) + (py - pView->m_fCurrentSnapY)*(py - pView->m_fCurrentSnapY));
						if(fd > dis )
						{
							pin_x = px;
							pin_y = py;
							fd = dis;
						}
					}
				}
			}
		}
	}

	for(int i=0; i<arrayPins.GetCount(); i++)
	{
		COb_schpin* ppin = arrayPins.GetAt(i);
		ppin->DrawOverwrite(pDC, pV);
	}

	fx0 = pin_x;
	fy0 = pin_y;
	pView->m_fCurrentSnapX = fx0;
	pView->m_fCurrentSnapY = fy0;
	pView->Restore_LargeCrossXY();
	deltax = pView->m_fCurrentSnapX - fx0;
	deltay = pView->m_fCurrentSnapY - fy0;

	pView->OpenMouseCursor();

	bEDA_NeedElectricalGrid = TRUE;

	pView->Write_Help(szHelp);
	flag = 0; 
	mirror = FALSE;
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}
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
				pView->ElecHotPointsControl(pDC);

				pView->CloseMouseCursor();
				arect.Draw(pDC, pV);
				for(int i=0; i<arrayPins.GetCount(); i++)
				{
					COb_schpin* ppin = arrayPins.GetAt(i);
					ppin->DrawOverwrite(pDC, pV);
				}
				pView->OpenMouseCursor();
			}

			fx1 = pView->m_fCurrentSnapX;
			fy1 = pView->m_fCurrentSnapY;
			deltax = fx1 - fx0;
			deltay = fy1 - fy0;

			pDoc->Get_Rotate_XY(&fx1, &fy1, rect.left - fx0, rect.bottom - fy0, flag*90, mirror);
			arect.m_fX1 = fx1 + fx0 + deltax;
			arect.m_fY1 = fy1 + fy0 + deltay;
			pDoc->Get_Rotate_XY(&fx1, &fy1, rect.right - fx0, rect.top - fy0, flag*90, mirror);
			arect.m_fX2 = fx1 + fx0 + deltax;
			arect.m_fY2 = fy1 + fy0 + deltay;

			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				COb_schpin* ppinbak = arrayPinsBak.GetAt(i);

				pDoc->Get_Rotate_XY(&fx1, &fy1, ppinbak->m_fX - fx0, ppinbak->m_fY - fy0, flag*90, mirror);
				ppin->m_fX = fx1 + fx0 + deltax;
				ppin->m_fY = fy1 + fy0 + deltay;

				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ppinbak->m_nOrientation, flag*90, mirror);
				ppin->m_nOrientation = orientation;
				ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);
			}

			
			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}			
			pView->OpenMouseCursor();

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = SCH_ELEMENT_COMPONENT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->schcomp.Copy(this);
			pundo->schcomp.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			pDoc->Get_Rotate_XY(&fx1, &fy1, m_fX - fx0, m_fY - fy0, flag*90, mirror);
			m_fX = fx1 + fx0 + deltax;
			m_fY = fy1 + fy0 + deltay;
			m_nOrientation += flag*90;
			m_nOrientation = pDoc->Get_Correct_Orientation(m_nOrientation);
			if(mirror == TRUE)
			{
				if(m_bMirrored == TRUE) m_bMirrored = FALSE;
				else m_bMirrored = TRUE;
			}

			for(int i=0; i<m_arrayPos.GetSize(); i++)
			{
				CObject* pobject;
				POSITION subpos;
				subpos = m_arrayPos.GetAt(i);
				pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
				{
					COb_schpin* ppin = (COb_schpin*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_PIN;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schpin.Copy(ppin);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, ppin->m_fX - fx0, ppin->m_fY - fy0, flag*90, mirror);
					ppin->m_fX = fx1 + fx0 + deltax;
					ppin->m_fY = fy1 + fy0 + deltay;

					int orientation;
					pDoc->Get_Rotate_Orientation(&orientation, ppin->m_nOrientation, flag*90, mirror);
					ppin->m_nOrientation = orientation;
					ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);					
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
				{
					COb_schieee* pieee = (COb_schieee*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_IEEE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schieee.Copy(pieee);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, pieee->m_fX - fx0, pieee->m_fY - fy0, flag*90, mirror);
					pieee->m_fX = fx1 + fx0 + deltax;
					pieee->m_fY = fy1 + fy0 + deltay;
					int orientation;
					pDoc->Get_Rotate_Orientation(&orientation, pieee->m_nOrientation, flag*90, mirror);
					pieee->m_nOrientation = orientation;
					pieee->m_nOrientation = pDoc->Get_Correct_Orientation(pieee->m_nOrientation);
		
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext = (COb_schtext*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_TEXT;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schtext.Copy(ptext);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, ptext->m_fSx - fx0, ptext->m_fSy - fy0, flag*90, mirror);
					ptext->m_fSx = fx1 + fx0 + deltax;
					ptext->m_fSy = fy1 + fy0 + deltay;
					int orientation;
					pDoc->Get_Rotate_Orientation(&orientation, ptext->m_nOrientation, flag*90, FALSE); //mirror);
					ptext->m_nOrientation = orientation;
					ptext->m_nOrientation = pDoc->Get_Correct_Orientation(ptext->m_nOrientation);
		
					if(mirror == TRUE)
					{
						if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
						else ptext->m_bMirror = TRUE;
						if((ptext->m_nOrientation == 0)||(ptext->m_nOrientation == 180))
						{
							if(ptext->m_nH_Justification == 0) ptext->m_nH_Justification = 2;
							else if(ptext->m_nH_Justification == 2) ptext->m_nH_Justification = 0;
						}
						else
						{
							if(ptext->m_nV_Justification == 0) ptext->m_nV_Justification = 2;
							else if(ptext->m_nV_Justification == 2) ptext->m_nV_Justification = 0;
						}
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
				{
					COb_schbezier* pbezier = (COb_schbezier*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_BEZIER;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schbezier.Copy(pbezier);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
					{
						pDoc->Get_Rotate_XY(&fx1, &fy1, pbezier->m_cVertex[k].fx-fx0, pbezier->m_cVertex[k].fy-fy0, flag*90, mirror);
						pbezier->m_cVertex[k].fx = fx1 + fx0 + deltax;
						pbezier->m_cVertex[k].fy = fy1 + fy0 + deltay;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
				{
					COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_POLYLINE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schpolyline.Copy(ppolyline);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
					{
						pDoc->Get_Rotate_XY(&fx1, &fy1, ppolyline->m_cVertex[k].fx-fx0, ppolyline->m_cVertex[k].fy-fy0, flag*90, mirror);
						ppolyline->m_cVertex[k].fx = fx1 + fx0 + deltax;
						ppolyline->m_cVertex[k].fy = fy1 + fy0 + deltay;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
				{
					COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_POLYGON;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schpolygon.Copy(ppolygon);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
					{
						pDoc->Get_Rotate_XY(&fx1, &fy1, ppolygon->m_cVertex[k].fx-fx0, ppolygon->m_cVertex[k].fy-fy0, flag*90, mirror);
						ppolygon->m_cVertex[k].fx = fx1 + fx0 + deltax;
						ppolygon->m_cVertex[k].fy = fy1 + fy0 + deltay;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
				{
					COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schellipsefill.Copy(pellipsefill);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, pellipsefill->m_fCx - fx0, pellipsefill->m_fCy - fy0, flag*90, mirror);
					pellipsefill->m_fCx = fx1 + fx0 + deltax;
					pellipsefill->m_fCy = fy1 + fy0 + deltay;
					if((flag == 1)||(flag == 3))
					{
						double ftemp;
						ftemp = pellipsefill->m_fRadius_y;
						pellipsefill->m_fRadius_y = pellipsefill->m_fRadius_x;
						pellipsefill->m_fRadius_x = ftemp;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
				{
					COb_schpie* ppie = (COb_schpie*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_PIE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schpie.Copy(ppie);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, ppie->m_fCx - fx0, ppie->m_fCy - fy0, flag*90, mirror);
					ppie->m_fCx = fx1 + fx0 + deltax;
					ppie->m_fCy = fy1 + fy0 + deltay;
					
					double salpha, ealpha;
					pDoc->Get_Rotate_Angle(&salpha, &ealpha, ppie->m_fSangle, ppie->m_fEangle, flag*90, mirror);
					ppie->m_fSangle = salpha;
					ppie->m_fSangle = pDoc->Get_Correct_Angle(ppie->m_fSangle);
					ppie->m_fEangle = ealpha;
					ppie->m_fEangle = pDoc->Get_Correct_Angle(ppie->m_fEangle);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
				{
					COb_schroundrect* proundrect = (COb_schroundrect*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schroundrect.Copy(proundrect);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX1 - fx0, proundrect->m_fY1 - fy0, flag*90, mirror);
					proundrect->m_fX1 = fx1 + fx0 + deltax;
					proundrect->m_fY1 = fy1 + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX2 - fx0, proundrect->m_fY2 - fy0, flag*90, mirror);
					proundrect->m_fX2 = fx1 + fx0 + deltax;
					proundrect->m_fY2 = fy1 + fy0 + deltay;
					
					double fminx, fminy, fmaxx, fmaxy;
					fminx = MIN(proundrect->m_fX1, proundrect->m_fX2);
					fmaxx = MAX(proundrect->m_fX1, proundrect->m_fX2);
					fminy = MIN(proundrect->m_fY1, proundrect->m_fY2);
					fmaxy = MAX(proundrect->m_fY1, proundrect->m_fY2);
					proundrect->m_fX1 = fminx;
					proundrect->m_fY1 = fminy;
					proundrect->m_fX2 = fmaxx;
					proundrect->m_fY2 = fmaxy;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
				{
					COb_schellipse* pellipse = (COb_schellipse*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schellipse.Copy(pellipse);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, pellipse->m_fCx - fx0, pellipse->m_fCy - fy0, flag*90, mirror);
					pellipse->m_fCx = fx1 + fx0 + deltax;
					pellipse->m_fCy = fy1 + fy0 + deltay;
					
					double salpha, ealpha;
					pDoc->Get_Rotate_Angle(&salpha, &ealpha, pellipse->m_fSangle, pellipse->m_fEangle, flag*90, mirror);
					pellipse->m_fSangle = salpha;
					pellipse->m_fSangle = pDoc->Get_Correct_Angle(pellipse->m_fSangle);
					pellipse->m_fEangle = ealpha;
					pellipse->m_fEangle = pDoc->Get_Correct_Angle(pellipse->m_fEangle);
					if((flag == 1)||(flag == 3))
					{
						double ftemp;
						ftemp = pellipse->m_fRadius_y;
						pellipse->m_fRadius_y = pellipse->m_fRadius_x;
						pellipse->m_fRadius_x = ftemp;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
				{
					COb_scharc* parc = (COb_scharc*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_ARC;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->scharc.Copy(parc);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, parc->m_fCx - fx0, parc->m_fCy - fy0, flag*90, mirror);
					parc->m_fCx = fx1 + fx0 + deltax;
					parc->m_fCy = fy1 + fy0 + deltay;
					
					double salpha, ealpha;
					pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, flag*90, mirror);
					parc->m_fSangle = salpha;
					parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
					parc->m_fEangle = ealpha;
					parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
				{
					COb_schrect* prect = (COb_schrect*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_RECT;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schrect.Copy(prect);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX1 - fx0, prect->m_fY1 - fy0, flag*90, mirror);
					prect->m_fX1 = fx1 + fx0 + deltax;
					prect->m_fY1 = fy1 + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX2 - fx0, prect->m_fY2 - fy0, flag*90, mirror);
					prect->m_fX2 = fx1 + fx0 + deltax;
					prect->m_fY2 = fy1 + fy0 + deltay;
					
					double fminx, fminy, fmaxx, fmaxy;
					fminx = MIN(prect->m_fX1, prect->m_fX2);
					fmaxx = MAX(prect->m_fX1, prect->m_fX2);
					fminy = MIN(prect->m_fY1, prect->m_fY2);
					fmaxy = MAX(prect->m_fY1, prect->m_fY2);
					prect->m_fX1 = fminx;
					prect->m_fY1 = fminy;
					prect->m_fX2 = fmaxx;
					prect->m_fY2 = fmaxy;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
				{
					COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schtextframe.Copy(ptextframe);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX1 - fx0, ptextframe->m_fY1 - fy0, flag*90, mirror);
					ptextframe->m_fX1 = fx1 + fx0 + deltax;
					ptextframe->m_fY1 = fy1 + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX2 - fx0, ptextframe->m_fY2 - fy0, flag*90, mirror);
					ptextframe->m_fX2 = fx1 + fx0 + deltax;
					ptextframe->m_fY2 = fy1 + fy0 + deltay;
					
					double fminx, fminy, fmaxx, fmaxy;
					fminx = MIN(ptextframe->m_fX1, ptextframe->m_fX2);
					fmaxx = MAX(ptextframe->m_fX1, ptextframe->m_fX2);
					fminy = MIN(ptextframe->m_fY1, ptextframe->m_fY2);
					fmaxy = MAX(ptextframe->m_fY1, ptextframe->m_fY2);
					ptextframe->m_fX1 = fminx;
					ptextframe->m_fY1 = fminy;
					ptextframe->m_fX2 = fmaxx;
					ptextframe->m_fY2 = fmaxy;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
				{
					COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = subpos;
					pundo->schgraphic.Copy(pgraphic);
							
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX1 - fx0, pgraphic->m_fY1 - fy0, flag*90, mirror);
					pgraphic->m_fX1 = fx1 + fx0 + deltax;
					pgraphic->m_fY1 = fy1 + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX2 - fx0, pgraphic->m_fY2 - fy0, flag*90, mirror);
					pgraphic->m_fX2 = fx1 + fx0 + deltax;
					pgraphic->m_fY2 = fy1 + fy0 + deltay;
					
					double fminx, fminy, fmaxx, fmaxy;
					fminx = MIN(pgraphic->m_fX1, pgraphic->m_fX2);
					fmaxx = MAX(pgraphic->m_fX1, pgraphic->m_fX2);
					fminy = MIN(pgraphic->m_fY1, pgraphic->m_fY2);
					fmaxy = MAX(pgraphic->m_fY1, pgraphic->m_fY2);
					pgraphic->m_fX1 = fminx;
					pgraphic->m_fY1 = fminy;
					pgraphic->m_fX2 = fmaxx;
					pgraphic->m_fY2 = fmaxy;
				}
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}
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

			pDoc->Get_Rotate_XY(&fx1, &fy1, rect.left - fx0, rect.bottom - fy0, flag*90, mirror);
			arect.m_fX1 = fx1 + fx0 + deltax;
			arect.m_fY1 = fy1 + fy0 + deltay;
			pDoc->Get_Rotate_XY(&fx1, &fy1, rect.right - fx0, rect.top - fy0, flag*90, mirror);
			arect.m_fX2 = fx1 + fx0 + deltax;
			arect.m_fY2 = fy1 + fy0 + deltay;

			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				COb_schpin* ppinbak = arrayPinsBak.GetAt(i);

				pDoc->Get_Rotate_XY(&fx1, &fy1, ppinbak->m_fX - fx0, ppinbak->m_fY - fy0, flag*90, mirror);
				ppin->m_fX = fx1 + fx0 + deltax;
				ppin->m_fY = fy1 + fy0 + deltay;

				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ppinbak->m_nOrientation, flag*90, mirror);
				ppin->m_nOrientation = orientation;
				ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);
			}

			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}
			pView->OpenMouseCursor();
		}
		else if((ch1 == KEY_x)||(ch1 == KEY_X))
		{
			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}

			if(mirror == FALSE) mirror = TRUE;
			else mirror = FALSE;

			pDoc->Get_Rotate_XY(&fx1, &fy1, rect.left - fx0, rect.bottom - fy0, flag*90, mirror);
			arect.m_fX1 = fx1 + fx0 + deltax;
			arect.m_fY1 = fy1 + fy0 + deltay;
			pDoc->Get_Rotate_XY(&fx1, &fy1, rect.right - fx0, rect.top - fy0, flag*90, mirror);
			arect.m_fX2 = fx1 + fx0 + deltax;
			arect.m_fY2 = fy1 + fy0 + deltay;

			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				COb_schpin* ppinbak = arrayPinsBak.GetAt(i);

				pDoc->Get_Rotate_XY(&fx1, &fy1, ppinbak->m_fX - fx0, ppinbak->m_fY - fy0, flag*90, mirror);
				ppin->m_fX = fx1 + fx0 + deltax;
				ppin->m_fY = fy1 + fy0 + deltay;

				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ppinbak->m_nOrientation, flag*90, mirror);
				ppin->m_nOrientation = orientation;
				ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);
			}

			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}
			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	for(int i=0; i<arrayPins.GetCount(); i++)
	{
		COb_schpin* ppin_copy;
		ppin_copy =	(COb_schpin*)arrayPins.GetAt(i);
		delete ppin_copy;
		ppin_copy = (COb_schpin*)arrayPinsBak.GetAt(i);
		delete ppin_copy;
	}
	
	pDC->SetROP2(drawmode);
	
	pView->CloseMouseCursor();

	m_nFlag = FLAG_NORMAL;
	pView->OpenMouseCursor();

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	if(undo_flag == TRUE)
	{
		if(bEDA_AutoJunction == TRUE)
		{
			pView->AutoPlaceJunction();
			pView->AutoRemoveJunction();
		}

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
	
	bEDA_NeedElectricalGrid = FALSE;
	pView->m_fpHotPoint1.fx = pView->m_fpHotPoint1.fy = pView->m_fpHotPoint2.fx = pView->m_fpHotPoint2.fy = 0;
	
	pView->Write_Help(szEDA_Help_Ready);
	pView->Invalidate();
}

BOOL COb_schcomp::MovePlaceComp(CClientDC* pDC, CView* pV)
{
	int ch1;
	double fx0, fy0, fx1, fy1, deltax, deltay;
	int flag;
	BOOL mirror;
	char szHelp[] = "放置元件: <空格>旋转, <X>镜像, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;


	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	
	
	RECT rect = GetCompRect();

	COb_schrect arect;
	arect.pDocument = pDoc;
	arect.m_fX1 = rect.left;
	arect.m_fY1 = rect.bottom;
	arect.m_fX2 = rect.right;
	arect.m_fY2 = rect.top;
	arect.m_nBorderWidth = 1;
	arect.m_nFillColor = m_nFillColor;
	arect.m_nBorderColor = m_nColor;
	arect.m_bTransparent = FALSE;
	arect.m_bFillSolid = TRUE;
	arect.m_nDiagram = m_nDiagram;
	arect.Draw(pDC, pV);

	CTypedPtrArray <CObArray, COb_schpin*> arrayPins;
	CTypedPtrArray <CObArray, COb_schpin*> arrayPinsBak;
	double pin_x, pin_y, fd = 100000;
	pin_x = arect.m_fX1;
	pin_y = arect.m_fY2;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		CObject* pobject;
		POSITION subpos;
		subpos = m_arrayPos.GetAt(i);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
		{
			COb_schpin* ppin = (COb_schpin*)pobject;
			if(ppin->m_nFlag != FLAG_NORMAL) continue;
			if((m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID == m_nPart))
			{
				if((m_nDisplayModeCount <= 1)||(m_nDisplayMode == ppin->m_nDisplayMode))
				{
					COb_schpin* ppin_copy;
					ppin_copy = new COb_schpin;
					ppin_copy->Copy(ppin);
					arrayPins.Add(ppin_copy);
					ppin_copy = new COb_schpin;
					ppin_copy->Copy(ppin);
					arrayPinsBak.Add(ppin_copy);
					double px, py;
					if(ppin->m_nOrientation == 0)
					{
						px = ppin->m_fX + ppin->m_fPinLength;
						py = ppin->m_fY;
						double dis = sqrt((px - pView->m_fCurrentSnapX)*(px - pView->m_fCurrentSnapX) + (py - pView->m_fCurrentSnapY)*(py - pView->m_fCurrentSnapY));
						if(fd > dis )
						{
							pin_x = px;
							pin_y = py;
							fd = dis;
						}
					}
					else if(ppin->m_nOrientation == 90)
					{
						px = ppin->m_fX;
						py = ppin->m_fY + ppin->m_fPinLength;
						double dis = sqrt((px - pView->m_fCurrentSnapX)*(px - pView->m_fCurrentSnapX) + (py - pView->m_fCurrentSnapY)*(py - pView->m_fCurrentSnapY));
						if(fd > dis )
						{
							pin_x = px;
							pin_y = py;
							fd = dis;
						}
					}
					else if(ppin->m_nOrientation == 180)
					{
						px = ppin->m_fX - ppin->m_fPinLength;
						py = ppin->m_fY;
						double dis = sqrt((px - pView->m_fCurrentSnapX)*(px - pView->m_fCurrentSnapX) + (py - pView->m_fCurrentSnapY)*(py - pView->m_fCurrentSnapY));
						if(fd > dis )
						{
							pin_x = px;
							pin_y = py;
							fd = dis;
						}
					}
					else if(ppin->m_nOrientation == 270)
					{
						px = ppin->m_fX;
						py = ppin->m_fY - ppin->m_fPinLength;
						double dis = sqrt((px - pView->m_fCurrentSnapX)*(px - pView->m_fCurrentSnapX) + (py - pView->m_fCurrentSnapY)*(py - pView->m_fCurrentSnapY));
						if(fd > dis )
						{
							pin_x = px;
							pin_y = py;
							fd = dis;
						}
					}
				}
			}
		}
	}

	for(int i=0; i<arrayPins.GetCount(); i++)
	{
		COb_schpin* ppin = arrayPins.GetAt(i);
		ppin->DrawOverwrite(pDC, pV);
	}

	pView->OpenMouseCursor();

	fx0 = pin_x;
	fy0 = pin_y;
	deltax = pView->m_fCurrentSnapX - fx0;
	deltay = pView->m_fCurrentSnapY - fy0;
	
	bEDA_NeedElectricalGrid = TRUE;
	
	pView->Write_Help(szHelp);
	flag = 0; 
	mirror = FALSE;
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}
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
				pView->ElecHotPointsControl(pDC);

				pView->CloseMouseCursor();
				arect.Draw(pDC, pV);
				for(int i=0; i<arrayPins.GetCount(); i++)
				{
					COb_schpin* ppin = arrayPins.GetAt(i);
					ppin->DrawOverwrite(pDC, pV);
				}
				pView->OpenMouseCursor();
			}

			fx1 = pView->m_fCurrentSnapX;
			fy1 = pView->m_fCurrentSnapY;
			deltax = fx1 - fx0;
			deltay = fy1 - fy0;

			pDoc->Get_Rotate_XY(&fx1, &fy1, rect.left - fx0, rect.bottom - fy0, flag*90, mirror);
			arect.m_fX1 = fx1 + fx0 + deltax;
			arect.m_fY1 = fy1 + fy0 + deltay;
			pDoc->Get_Rotate_XY(&fx1, &fy1, rect.right - fx0, rect.top - fy0, flag*90, mirror);
			arect.m_fX2 = fx1 + fx0 + deltax;
			arect.m_fY2 = fy1 + fy0 + deltay;

			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				COb_schpin* ppinbak = arrayPinsBak.GetAt(i);

				pDoc->Get_Rotate_XY(&fx1, &fy1, ppinbak->m_fX - fx0, ppinbak->m_fY - fy0, flag*90, mirror);
				ppin->m_fX = fx1 + fx0 + deltax;
				ppin->m_fY = fy1 + fy0 + deltay;

				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ppinbak->m_nOrientation, flag*90, mirror);
				ppin->m_nOrientation = orientation;
				ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);
			}

			
			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}			
			pView->OpenMouseCursor();

			pDoc->Get_Rotate_XY(&fx1, &fy1, m_fX - fx0, m_fY - fy0, flag*90, mirror);
			m_fX = fx1 + fx0 + deltax;
			m_fY = fy1 + fy0 + deltay;
			m_nOrientation += flag*90;
			m_nOrientation = pDoc->Get_Correct_Orientation(m_nOrientation);
			if(mirror == TRUE)
			{
				if(m_bMirrored == TRUE) m_bMirrored = FALSE;
				else m_bMirrored = TRUE;
			}
			for(int i=0; i<m_arrayPos.GetSize(); i++)
			{
				CObject* pobject;
				POSITION subpos;
				subpos = m_arrayPos.GetAt(i);
				pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
				{
					COb_schpin* ppin = (COb_schpin*)pobject;
					pDoc->Get_Rotate_XY(&fx1, &fy1, ppin->m_fX - fx0, ppin->m_fY - fy0, flag*90, mirror);
					ppin->m_fX = fx1 + fx0 + deltax;
					ppin->m_fY = fy1 + fy0 + deltay;

					int orientation;
					pDoc->Get_Rotate_Orientation(&orientation, ppin->m_nOrientation, flag*90, mirror);
					ppin->m_nOrientation = orientation;
					ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);					
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
				{
					COb_schieee* pieee = (COb_schieee*)pobject;
					pDoc->Get_Rotate_XY(&fx1, &fy1, pieee->m_fX - fx0, pieee->m_fY - fy0, flag*90, mirror);
					pieee->m_fX = fx1 + fx0 + deltax;
					pieee->m_fY = fy1 + fy0 + deltay;
					int orientation;
					pDoc->Get_Rotate_Orientation(&orientation, pieee->m_nOrientation, flag*90, mirror);
					pieee->m_nOrientation = orientation;
					pieee->m_nOrientation = pDoc->Get_Correct_Orientation(pieee->m_nOrientation);
		
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext = (COb_schtext*)pobject;

					pDoc->Get_Rotate_XY(&fx1, &fy1, ptext->m_fSx - fx0, ptext->m_fSy - fy0, flag*90, mirror);
					ptext->m_fSx = fx1 + fx0 + deltax;
					ptext->m_fSy = fy1 + fy0 + deltay;
					int orientation;
					pDoc->Get_Rotate_Orientation(&orientation, ptext->m_nOrientation, flag*90, FALSE); //mirror);
					ptext->m_nOrientation = orientation;
					ptext->m_nOrientation = pDoc->Get_Correct_Orientation(ptext->m_nOrientation);
		
					if(mirror == TRUE)
					{
						if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
						else ptext->m_bMirror = TRUE;
						if((ptext->m_nOrientation == 0)||(ptext->m_nOrientation == 180))
						{
							if(ptext->m_nH_Justification == 0) ptext->m_nH_Justification = 2;
							else if(ptext->m_nH_Justification == 2) ptext->m_nH_Justification = 0;
						}
						else
						{
							if(ptext->m_nV_Justification == 0) ptext->m_nV_Justification = 2;
							else if(ptext->m_nV_Justification == 2) ptext->m_nV_Justification = 0;
						}
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
				{
					COb_schbezier* pbezier = (COb_schbezier*)pobject;

					for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
					{
						pDoc->Get_Rotate_XY(&fx1, &fy1, pbezier->m_cVertex[k].fx-fx0, pbezier->m_cVertex[k].fy-fy0, flag*90, mirror);
						pbezier->m_cVertex[k].fx = fx1 + fx0 + deltax;
						pbezier->m_cVertex[k].fy = fy1 + fy0 + deltay;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
				{
					COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;

					for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
					{
						pDoc->Get_Rotate_XY(&fx1, &fy1, ppolyline->m_cVertex[k].fx-fx0, ppolyline->m_cVertex[k].fy-fy0, flag*90, mirror);
						ppolyline->m_cVertex[k].fx = fx1 + fx0 + deltax;
						ppolyline->m_cVertex[k].fy = fy1 + fy0 + deltay;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
				{
					COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;

					for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
					{
						pDoc->Get_Rotate_XY(&fx1, &fy1, ppolygon->m_cVertex[k].fx-fx0, ppolygon->m_cVertex[k].fy-fy0, flag*90, mirror);
						ppolygon->m_cVertex[k].fx = fx1 + fx0 + deltax;
						ppolygon->m_cVertex[k].fy = fy1 + fy0 + deltay;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
				{
					COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;

					pDoc->Get_Rotate_XY(&fx1, &fy1, pellipsefill->m_fCx - fx0, pellipsefill->m_fCy - fy0, flag*90, mirror);
					pellipsefill->m_fCx = fx1 + fx0 + deltax;
					pellipsefill->m_fCy = fy1 + fy0 + deltay;
					if((flag == 1)||(flag == 3))
					{
						double ftemp;
						ftemp = pellipsefill->m_fRadius_y;
						pellipsefill->m_fRadius_y = pellipsefill->m_fRadius_x;
						pellipsefill->m_fRadius_x = ftemp;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
				{
					COb_schpie* ppie = (COb_schpie*)pobject;

					pDoc->Get_Rotate_XY(&fx1, &fy1, ppie->m_fCx - fx0, ppie->m_fCy - fy0, flag*90, mirror);
					ppie->m_fCx = fx1 + fx0 + deltax;
					ppie->m_fCy = fy1 + fy0 + deltay;
					
					double salpha, ealpha;
					pDoc->Get_Rotate_Angle(&salpha, &ealpha, ppie->m_fSangle, ppie->m_fEangle, flag*90, mirror);
					ppie->m_fSangle = salpha;
					ppie->m_fSangle = pDoc->Get_Correct_Angle(ppie->m_fSangle);
					ppie->m_fEangle = ealpha;
					ppie->m_fEangle = pDoc->Get_Correct_Angle(ppie->m_fEangle);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
				{
					COb_schroundrect* proundrect = (COb_schroundrect*)pobject;

					pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX1 - fx0, proundrect->m_fY1 - fy0, flag*90, mirror);
					proundrect->m_fX1 = fx1 + fx0 + deltax;
					proundrect->m_fY1 = fy1 + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX2 - fx0, proundrect->m_fY2 - fy0, flag*90, mirror);
					proundrect->m_fX2 = fx1 + fx0 + deltax;
					proundrect->m_fY2 = fy1 + fy0 + deltay;
					
					double fminx, fminy, fmaxx, fmaxy;
					fminx = MIN(proundrect->m_fX1, proundrect->m_fX2);
					fmaxx = MAX(proundrect->m_fX1, proundrect->m_fX2);
					fminy = MIN(proundrect->m_fY1, proundrect->m_fY2);
					fmaxy = MAX(proundrect->m_fY1, proundrect->m_fY2);
					proundrect->m_fX1 = fminx;
					proundrect->m_fY1 = fminy;
					proundrect->m_fX2 = fmaxx;
					proundrect->m_fY2 = fmaxy;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
				{
					COb_schellipse* pellipse = (COb_schellipse*)pobject;

					pDoc->Get_Rotate_XY(&fx1, &fy1, pellipse->m_fCx - fx0, pellipse->m_fCy - fy0, flag*90, mirror);
					pellipse->m_fCx = fx1 + fx0 + deltax;
					pellipse->m_fCy = fy1 + fy0 + deltay;
					
					double salpha, ealpha;
					pDoc->Get_Rotate_Angle(&salpha, &ealpha, pellipse->m_fSangle, pellipse->m_fEangle, flag*90, mirror);
					pellipse->m_fSangle = salpha;
					pellipse->m_fSangle = pDoc->Get_Correct_Angle(pellipse->m_fSangle);
					pellipse->m_fEangle = ealpha;
					pellipse->m_fEangle = pDoc->Get_Correct_Angle(pellipse->m_fEangle);
					if((flag == 1)||(flag == 3))
					{
						double ftemp;
						ftemp = pellipse->m_fRadius_y;
						pellipse->m_fRadius_y = pellipse->m_fRadius_x;
						pellipse->m_fRadius_x = ftemp;
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
				{
					COb_scharc* parc = (COb_scharc*)pobject;

					pDoc->Get_Rotate_XY(&fx1, &fy1, parc->m_fCx - fx0, parc->m_fCy - fy0, flag*90, mirror);
					parc->m_fCx = fx1 + fx0 + deltax;
					parc->m_fCy = fy1 + fy0 + deltay;
					
					double salpha, ealpha;
					pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, flag*90, mirror);
					parc->m_fSangle = salpha;
					parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
					parc->m_fEangle = ealpha;
					parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
				{
					COb_schrect* prect = (COb_schrect*)pobject;

					pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX1 - fx0, prect->m_fY1 - fy0, flag*90, mirror);
					prect->m_fX1 = fx1 + fx0 + deltax;
					prect->m_fY1 = fy1 + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX2 - fx0, prect->m_fY2 - fy0, flag*90, mirror);
					prect->m_fX2 = fx1 + fx0 + deltax;
					prect->m_fY2 = fy1 + fy0 + deltay;
					
					double fminx, fminy, fmaxx, fmaxy;
					fminx = MIN(prect->m_fX1, prect->m_fX2);
					fmaxx = MAX(prect->m_fX1, prect->m_fX2);
					fminy = MIN(prect->m_fY1, prect->m_fY2);
					fmaxy = MAX(prect->m_fY1, prect->m_fY2);
					prect->m_fX1 = fminx;
					prect->m_fY1 = fminy;
					prect->m_fX2 = fmaxx;
					prect->m_fY2 = fmaxy;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
				{
					COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;

					pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX1 - fx0, ptextframe->m_fY1 - fy0, flag*90, mirror);
					ptextframe->m_fX1 = fx1 + fx0 + deltax;
					ptextframe->m_fY1 = fy1 + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX2 - fx0, ptextframe->m_fY2 - fy0, flag*90, mirror);
					ptextframe->m_fX2 = fx1 + fx0 + deltax;
					ptextframe->m_fY2 = fy1 + fy0 + deltay;
					
					double fminx, fminy, fmaxx, fmaxy;
					fminx = MIN(ptextframe->m_fX1, ptextframe->m_fX2);
					fmaxx = MAX(ptextframe->m_fX1, ptextframe->m_fX2);
					fminy = MIN(ptextframe->m_fY1, ptextframe->m_fY2);
					fmaxy = MAX(ptextframe->m_fY1, ptextframe->m_fY2);
					ptextframe->m_fX1 = fminx;
					ptextframe->m_fY1 = fminy;
					ptextframe->m_fX2 = fmaxx;
					ptextframe->m_fY2 = fmaxy;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
				{
					COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;

					pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX1 - fx0, pgraphic->m_fY1 - fy0, flag*90, mirror);
					pgraphic->m_fX1 = fx1 + fx0 + deltax;
					pgraphic->m_fY1 = fy1 + fy0 + deltay;
					pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX2 - fx0, pgraphic->m_fY2 - fy0, flag*90, mirror);
					pgraphic->m_fX2 = fx1 + fx0 + deltax;
					pgraphic->m_fY2 = fy1 + fy0 + deltay;
					
					double fminx, fminy, fmaxx, fmaxy;
					fminx = MIN(pgraphic->m_fX1, pgraphic->m_fX2);
					fmaxx = MAX(pgraphic->m_fX1, pgraphic->m_fX2);
					fminy = MIN(pgraphic->m_fY1, pgraphic->m_fY2);
					fmaxy = MAX(pgraphic->m_fY1, pgraphic->m_fY2);
					pgraphic->m_fX1 = fminx;
					pgraphic->m_fY1 = fminy;
					pgraphic->m_fX2 = fmaxx;
					pgraphic->m_fY2 = fmaxy;
				}
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}
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

			pDoc->Get_Rotate_XY(&fx1, &fy1, rect.left - fx0, rect.bottom - fy0, flag*90, mirror);
			arect.m_fX1 = fx1 + fx0 + deltax;
			arect.m_fY1 = fy1 + fy0 + deltay;
			pDoc->Get_Rotate_XY(&fx1, &fy1, rect.right - fx0, rect.top - fy0, flag*90, mirror);
			arect.m_fX2 = fx1 + fx0 + deltax;
			arect.m_fY2 = fy1 + fy0 + deltay;

			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				COb_schpin* ppinbak = arrayPinsBak.GetAt(i);

				pDoc->Get_Rotate_XY(&fx1, &fy1, ppinbak->m_fX - fx0, ppinbak->m_fY - fy0, flag*90, mirror);
				ppin->m_fX = fx1 + fx0 + deltax;
				ppin->m_fY = fy1 + fy0 + deltay;

				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ppinbak->m_nOrientation, flag*90, mirror);
				ppin->m_nOrientation = orientation;
				ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);
			}

			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}
			pView->OpenMouseCursor();
		}
		else if((ch1 == KEY_x)||(ch1 == KEY_X))
		{
			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}

			if(mirror == FALSE) mirror = TRUE;
			else mirror = FALSE;

			pDoc->Get_Rotate_XY(&fx1, &fy1, rect.left - fx0, rect.bottom - fy0, flag*90, mirror);
			arect.m_fX1 = fx1 + fx0 + deltax;
			arect.m_fY1 = fy1 + fy0 + deltay;
			pDoc->Get_Rotate_XY(&fx1, &fy1, rect.right - fx0, rect.top - fy0, flag*90, mirror);
			arect.m_fX2 = fx1 + fx0 + deltax;
			arect.m_fY2 = fy1 + fy0 + deltay;

			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				COb_schpin* ppinbak = arrayPinsBak.GetAt(i);

				pDoc->Get_Rotate_XY(&fx1, &fy1, ppinbak->m_fX - fx0, ppinbak->m_fY - fy0, flag*90, mirror);
				ppin->m_fX = fx1 + fx0 + deltax;
				ppin->m_fY = fy1 + fy0 + deltay;

				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ppinbak->m_nOrientation, flag*90, mirror);
				ppin->m_nOrientation = orientation;
				ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);
			}

			arect.Draw(pDC, pV);
			for(int i=0; i<arrayPins.GetCount(); i++)
			{
				COb_schpin* ppin = arrayPins.GetAt(i);
				ppin->DrawOverwrite(pDC, pV);
			}
			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	for(int i=0; i<arrayPins.GetCount(); i++)
	{
		COb_schpin* ppin_copy;
		ppin_copy =	(COb_schpin*)arrayPins.GetAt(i);
		delete ppin_copy;
		ppin_copy = (COb_schpin*)arrayPinsBak.GetAt(i);
		delete ppin_copy;
	}
	
	pDC->SetROP2(drawmode);
	
	pView->CloseMouseCursor();

	m_nFlag = FLAG_NORMAL;
	pView->OpenMouseCursor();

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	bEDA_NeedElectricalGrid = FALSE;
	pView->m_fpHotPoint1.fx = pView->m_fpHotPoint1.fy = pView->m_fpHotPoint2.fx = pView->m_fpHotPoint2.fy = 0;

	pView->Write_Help(szEDA_Help_Ready);

	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}
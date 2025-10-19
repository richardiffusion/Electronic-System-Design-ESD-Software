#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_PCBEditDimension.h"
#include "Dlg_PCBEditDimensionLinear.h"

IMPLEMENT_SERIAL(COb_pcbdimension, CObject, 0)
COb_pcbdimension::COb_pcbdimension()
{
	m_nUnitStyle = UNIT_STYLE_BRACKETS;

	m_bLocked = FALSE;
	m_bPrimitiveLock = TRUE;

	m_nStrokeFont = 0;
	m_bUseTTFont = FALSE;
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_csTTFontName = "Arial";
	
	//------ 下面参数的意义有待明确
	//m_nIndexForSave = -1;
	m_bPolygonOutline = FALSE;
	m_bUserRouted = TRUE;
	m_nUnionIndex = 0;

	m_bSelection = FALSE;	
	m_nFlag = 0;
}



void COb_pcbdimension::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fLx;
		ar << m_fLy;
		ar << m_fHx;
		ar << m_fHy;
		
		ar << m_fXs;
		ar << m_fYs;
		ar << m_fXe;
		ar << m_fYe;
		
		ar << m_fHeight;
		ar << m_fLineWidth;
		ar << m_fRotation;

		int num = m_cRefPoint.GetCount();
		ar << num;
		for(int i=0; i<num; i++)
		{
			Fpoint point = m_cRefPoint.GetAt(i);
			ar << point.fx;
			ar << point.fy;
		}

		ar << m_fArrowSize;
		ar << m_fArrowLineWidth;
		ar << m_fArrowLength;
		ar << m_nArrowPosition;

		ar << m_fExtensionOffset;
		ar << m_fExtensionLineWidth;
		ar << m_fExtensionPickGap;

		ar << m_fTextSx;
		ar << m_fTextSy;
		ar << m_fTextHeight;
		ar << m_fTextLineWidth;
		ar << m_fTextRotation;
		
		ar << m_nTextPosition;
		ar << m_fTextGap;
		ar << m_nTextFormat;
		ar << m_nTextDimensionUnit;
		ar << m_nTextPrecision;

		ar << m_csPrefix;
		ar << m_csSuffix;

		ar << m_nLayer;
		ar << m_nUnitStyle;
		ar << m_nDimensionKind;

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
		ar >> m_fLx;
		ar >> m_fLy;
		ar >> m_fHx;
		ar >> m_fHy;
		
		ar >> m_fXs;
		ar >> m_fYs;
		ar >> m_fXe;
		ar >> m_fYe;
		
		ar >> m_fHeight;
		ar >> m_fLineWidth;
		ar >> m_fRotation;

		int num;
		ar >> num;
		for(int i=0; i<num; i++)
		{
			Fpoint point;
			ar >> point.fx;
			ar >> point.fy;
			m_cRefPoint.Add(point);
		}

		ar >> m_fArrowSize;
		ar >> m_fArrowLineWidth;
		ar >> m_fArrowLength;
		ar >> m_nArrowPosition;

		ar >> m_fExtensionOffset;
		ar >> m_fExtensionLineWidth;
		ar >> m_fExtensionPickGap;

		ar >> m_fTextSx;
		ar >> m_fTextSy;
		ar >> m_fTextHeight;
		ar >> m_fTextLineWidth;
		ar >> m_fTextRotation;
		
		ar >> m_nTextPosition;
		ar >> m_fTextGap;
		ar >> m_nTextFormat;
		ar >> m_nTextDimensionUnit;
		ar >> m_nTextPrecision;

		ar >> m_csPrefix;
		ar >> m_csSuffix;

		ar >> m_nLayer;
		ar >> m_nUnitStyle;
		ar >> m_nDimensionKind;

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

void COb_pcbdimension::Draw(CDC* pDC, CView* pV, int mode)
{
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();	

	if(m_nDimensionKind == 8)  
	{
		double fw;
		double d;

		COb_pcbtext pcbtext;
		pcbtext.m_fSx = m_fTextSx;
		pcbtext.m_fSy = m_fTextSy;
		pcbtext.m_fHeight = m_fTextHeight;
		pcbtext.m_fWidth = m_fTextLineWidth;
		pcbtext.m_fRotation = m_fTextRotation;
		pcbtext.m_nLayer = m_nLayer;
		pcbtext.m_nStrokeFont = m_nStrokeFont;
		pcbtext.m_bUseTTFont = m_bUseTTFont;
		pcbtext.m_bBold = m_bBold;
		pcbtext.m_bItalic = m_bItalic;
		pcbtext.m_csTTFontName = m_csTTFontName;
		pcbtext.m_nFlag = m_nFlag;

		d = sqrt((m_fXs - m_fXe)*(m_fXs - m_fXe) + (m_fYs - m_fYe)*(m_fYs - m_fYe));

		if(pDoc->m_cPCB_Sheet.m_nUnit == UNIT_PCB_I)
		{
			char str[100];
			CString cstr;
			pView->mil_to_str(d, str);
			cstr = str;
			cstr.Trim("mil");

			if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx
			{
				pcbtext.m_csText = cstr;
			}
			else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil
			{
				pcbtext.m_csText = cstr + "mil";
			}
			else	//xxxx (mil)
			{
				pcbtext.m_csText = cstr + " (mil)";
			}
		}
		else
		{
			char str[100];
			CString cstr;
			pView->mm_to_str(d, str);
			cstr = str;
			cstr.Trim("m");

			if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx
			{
				pcbtext.m_csText = cstr;
			}
			else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil
			{
				pcbtext.m_csText = cstr + "mm";
			}
			else	//xxxx (mil)
			{
				pcbtext.m_csText = cstr + " (mm)";
			}
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

		fw = pcbtext.GetStringWidth(pV);

		if( (d-fw) > 80.0f)
		{
			double alpha;
			if(m_fXs == m_fXe)
			{
				if(m_fYs < m_fYe) alpha = fEDA_pi/2;
				else alpha = -1.0f*fEDA_pi/2;
			}
			else if(m_fYs == m_fYe) alpha = 0;
			else alpha = atan((m_fYe - m_fYs)/(m_fXe - m_fXs));

			COb_pcbtrack pcbtrack;
			pcbtrack.m_fWidth = m_fLineWidth;
			pcbtrack.m_nLayer = m_nLayer;	
			pcbtrack.m_nFlag = m_nFlag;

			pcbtrack.m_fX1 = m_fXs + m_fHeight*sin(alpha);
			pcbtrack.m_fY1 = m_fYs - m_fHeight*cos(alpha);
			pcbtrack.m_fX2 = m_fXs - m_fHeight*sin(alpha);
			pcbtrack.m_fY2 = m_fYs + m_fHeight*cos(alpha);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXs;
			pcbtrack.m_fY1 = m_fYs;
			pcbtrack.m_fX2 = m_fXs + ((d-fw-25)/2)*cos(alpha);
			pcbtrack.m_fY2 = m_fYs + ((d-fw-25)/2)*sin(alpha);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXs;
			pcbtrack.m_fY1 = m_fYs;
			pcbtrack.m_fX2 = m_fXs + 63.246f*cos(alpha + 0.3218f);
			pcbtrack.m_fY2 = m_fYs + 63.246f*sin(alpha + 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXs;
			pcbtrack.m_fY1 = m_fYs;
			pcbtrack.m_fX2 = m_fXs + 63.246f*cos(alpha - 0.3218f);
			pcbtrack.m_fY2 = m_fYs + 63.246f*sin(alpha - 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXe + m_fHeight*sin(alpha);
			pcbtrack.m_fY1 = m_fYe - m_fHeight*cos(alpha);
			pcbtrack.m_fX2 = m_fXe - m_fHeight*sin(alpha);
			pcbtrack.m_fY2 = m_fYe + m_fHeight*cos(alpha);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXe;
			pcbtrack.m_fY1 = m_fYe;
			pcbtrack.m_fX2 = m_fXe - ((d-fw-25)/2)*cos(alpha);
			pcbtrack.m_fY2 = m_fYe - ((d-fw-25)/2)*sin(alpha);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXe;
			pcbtrack.m_fY1 = m_fYe;
			pcbtrack.m_fX2 = m_fXe - 63.246f*cos(alpha + 0.3218f);
			pcbtrack.m_fY2 = m_fYe - 63.246f*sin(alpha + 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXe;
			pcbtrack.m_fY1 = m_fYe;
			pcbtrack.m_fX2 = m_fXe - 63.246f*cos(alpha - 0.3218f);
			pcbtrack.m_fY2 = m_fYe - 63.246f*sin(alpha - 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);
		}
		else
		{
			double alpha;
			if(m_fXs == m_fXe)
			{
				if(m_fYs < m_fYe) alpha = fEDA_pi/2;
				else alpha = -1.0f*fEDA_pi/2;
			}
			else if(m_fYs == m_fYe) alpha = 0;
			else alpha = atan((m_fYe - m_fYs)/(m_fXe - m_fXs));

			COb_pcbtrack pcbtrack;
			pcbtrack.m_fWidth = m_fLineWidth;
			pcbtrack.m_nLayer = m_nLayer;	
			pcbtrack.m_nFlag = m_nFlag;
			
			pcbtrack.m_fX1 = m_fXe + m_fHeight*sin(alpha);
			pcbtrack.m_fY1 = m_fYe - m_fHeight*cos(alpha);
			pcbtrack.m_fX2 = m_fXe - m_fHeight*sin(alpha);
			pcbtrack.m_fY2 = m_fYe + m_fHeight*cos(alpha);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXe;
			pcbtrack.m_fY1 = m_fYe;
			pcbtrack.m_fX2 = m_fXe + 100*cos(alpha);
			pcbtrack.m_fY2 = m_fYe + 100*sin(alpha);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXe;
			pcbtrack.m_fY1 = m_fYe;
			pcbtrack.m_fX2 = m_fXe + 63.246f*cos(alpha + 0.3218f);
			pcbtrack.m_fY2 = m_fYe + 63.246f*sin(alpha + 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXe;
			pcbtrack.m_fY1 = m_fYe;
			pcbtrack.m_fX2 = m_fXe + 63.246f*cos(alpha - 0.3218f);
			pcbtrack.m_fY2 = m_fYe + 63.246f*sin(alpha - 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXs + m_fHeight*sin(alpha);
			pcbtrack.m_fY1 = m_fYs - m_fHeight*cos(alpha);
			pcbtrack.m_fX2 = m_fXs - m_fHeight*sin(alpha);
			pcbtrack.m_fY2 = m_fYs + m_fHeight*cos(alpha);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXs;
			pcbtrack.m_fY1 = m_fYs;
			pcbtrack.m_fX2 = m_fXs - 100*cos(alpha);
			pcbtrack.m_fY2 = m_fYs - 100*sin(alpha);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXs;
			pcbtrack.m_fY1 = m_fYs;
			pcbtrack.m_fX2 = m_fXs - 63.246f*cos(alpha + 0.3218f);
			pcbtrack.m_fY2 = m_fYs - 63.246f*sin(alpha + 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXs;
			pcbtrack.m_fY1 = m_fYs;
			pcbtrack.m_fX2 = m_fXs - 63.246f*cos(alpha - 0.3218f);
			pcbtrack.m_fY2 = m_fYs - 63.246f*sin(alpha - 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);
		}
	}
	else if(m_nDimensionKind == 1)	//Embedded --- Linear
	{
		double fw;
		double d;

		COb_pcbtext pcbtext;
		pcbtext.m_fSx = m_fTextSx;
		pcbtext.m_fSy = m_fTextSy;
		pcbtext.m_fHeight = m_fTextHeight;
		pcbtext.m_fWidth = m_fTextLineWidth;
		pcbtext.m_fRotation = m_fTextRotation;
		pcbtext.m_nLayer = m_nLayer;
		pcbtext.m_nStrokeFont = m_nStrokeFont;
		pcbtext.m_bUseTTFont = m_bUseTTFont;
		pcbtext.m_bBold = m_bBold;
		pcbtext.m_bItalic = m_bItalic;
		pcbtext.m_csTTFontName = m_csTTFontName;
		//pcbtext.m_nFlag = m_nFlag;

		double alpha;
		if(m_cRefPoint[0].fx == m_cRefPoint[1].fx) 
		{
			if(m_cRefPoint[1].fy > m_cRefPoint[0].fy) alpha = fEDA_pi/2;
			else alpha = -1.0f*fEDA_pi/2;
		}
		else if(m_cRefPoint[0].fy == m_cRefPoint[1].fy) alpha = 0;
		else alpha = atan((m_cRefPoint[1].fy - m_cRefPoint[0].fy)/(m_cRefPoint[1].fx - m_cRefPoint[0].fx));

		d = abs(cos(sqrt((alpha - m_fRotation*fEDA_pi/180)*(alpha - m_fRotation*fEDA_pi/180)))*sqrt((m_cRefPoint[0].fx - m_cRefPoint[1].fx)*(m_cRefPoint[0].fx - m_cRefPoint[1].fx) + (m_cRefPoint[0].fy - m_cRefPoint[1].fy)*(m_cRefPoint[0].fy - m_cRefPoint[1].fy)));

		char str[100];
		pView->size_to_unitstr(d, str, m_nTextFormat, m_nTextPrecision, m_nTextDimensionUnit);
		

		pcbtext.m_csText = m_csPrefix + str + m_csSuffix;

		pcbtext.m_bSelection = m_bSelection;
		if(pcbtext.m_csText.GetLength() > 0)
		{
			//if(pcbtext.PickMe(pDC, pV) == TRUE) 
			pcbtext.Draw(pDC, pV);
			fw = pcbtext.GetStringWidth(pV);
		}
		else fw = 0;

		COb_pcbtrack pcbtrack;
		pcbtrack.m_nLayer = m_nLayer;
		pcbtrack.m_nFlag = m_nFlag;
			
		pcbtrack.m_fWidth = m_fExtensionLineWidth;

		double fxs, fys, fxe, fye;


		if(m_fYs < m_cRefPoint[0].fy)
		{
			fxs = m_fXs + m_fExtensionOffset*sin(m_fRotation*fEDA_pi/180);
			fys = m_fYs - m_fExtensionOffset*cos(m_fRotation*fEDA_pi/180);
		}
		else
		{
			fxs = m_fXs - m_fExtensionOffset*sin(m_fRotation*fEDA_pi/180);
			fys = m_fYs + m_fExtensionOffset*cos(m_fRotation*fEDA_pi/180);
		}
		
		int angle = (int)m_fRotation;
		angle = angle%180;
		if((angle%180) == 0) 
		{
			m_fXe = m_cRefPoint[1].fx;
			m_fYe = m_fYs;
		}
		else if((angle%90) == 0) 
		{
			m_fXe = m_fXs;
			m_fYe = m_cRefPoint[1].fy;	
		}
		else
		{
			double k = tan((90.0f + m_fRotation)*fEDA_pi/180);

			m_fXe = ( m_fXs/k + m_fYs + k*m_cRefPoint[1].fx - m_cRefPoint[1].fy )*k/(1+k*k);
			m_fYe = ( m_cRefPoint[1].fy/k + m_fXs + k*m_fYs - m_cRefPoint[1].fx )*k/(1+k*k);
		}

		if(m_fYs < m_cRefPoint[1].fy)
		{
			fxe = m_fXe + m_fExtensionOffset*sin(m_fRotation*fEDA_pi/180);
			fye = m_fYe - m_fExtensionOffset*cos(m_fRotation*fEDA_pi/180);
		}
		else
		{
			fxe = m_fXe - m_fExtensionOffset*sin(m_fRotation*fEDA_pi/180);
			fye = m_fYe + m_fExtensionOffset*cos(m_fRotation*fEDA_pi/180);
		}

		pcbtrack.m_fX1 = m_cRefPoint[0].fx;
		pcbtrack.m_fY1 = m_cRefPoint[0].fy;
		pcbtrack.m_fX2 = fxs;
		pcbtrack.m_fY2 = fys;
		if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);
		
		pcbtrack.m_fX1 = m_cRefPoint[1].fx;
		pcbtrack.m_fY1 = m_cRefPoint[1].fy;
		pcbtrack.m_fX2 = fxe;
		pcbtrack.m_fY2 = fye;
		if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

		pcbtrack.m_fWidth = m_fArrowLineWidth;
		if( ((d-fw) > 110.0f)&&(m_nTextPosition == 0) || ((m_nTextPosition != 0)&&(m_nArrowPosition == 0)) )
		{
			double alpha;
			if(m_fXs == m_fXe)
			{
				if(m_fYs < m_fYe) alpha = fEDA_pi/2;
				else alpha = -1.0f*fEDA_pi/2;
			}
			else if(m_fYs == m_fYe) alpha = 0;
			else alpha = atan((m_fYe - m_fYs)/(m_fXe - m_fXs));

			pcbtrack.m_fX1 = m_fXs;
			pcbtrack.m_fY1 = m_fYs;
			pcbtrack.m_fX2 = m_fXs + ((d-fw-50)/2)*cos(alpha);
			pcbtrack.m_fY2 = m_fYs + ((d-fw-50)/2)*sin(alpha);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXs;
			pcbtrack.m_fY1 = m_fYs;
			pcbtrack.m_fX2 = m_fXs + (m_fArrowSize/cos(0.3218f))*cos(alpha + 0.3218f);
			pcbtrack.m_fY2 = m_fYs + (m_fArrowSize/cos(0.3218f))*sin(alpha + 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXs;
			pcbtrack.m_fY1 = m_fYs;
			pcbtrack.m_fX2 = m_fXs + (m_fArrowSize/cos(0.3218f))*cos(alpha - 0.3218f);
			pcbtrack.m_fY2 = m_fYs + (m_fArrowSize/cos(0.3218f))*sin(alpha - 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXe;
			pcbtrack.m_fY1 = m_fYe;
			pcbtrack.m_fX2 = m_fXe - ((d-fw-25)/2)*cos(alpha);
			pcbtrack.m_fY2 = m_fYe - ((d-fw-25)/2)*sin(alpha);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXe;
			pcbtrack.m_fY1 = m_fYe;
			pcbtrack.m_fX2 = m_fXe - (m_fArrowSize/cos(0.3218f))*cos(alpha + 0.3218f);
			pcbtrack.m_fY2 = m_fYe - (m_fArrowSize/cos(0.3218f))*sin(alpha + 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXe;
			pcbtrack.m_fY1 = m_fYe;
			pcbtrack.m_fX2 = m_fXe - (m_fArrowSize/cos(0.3218f))*cos(alpha - 0.3218f);
			pcbtrack.m_fY2 = m_fYe - (m_fArrowSize/cos(0.3218f))*sin(alpha - 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);
		}
		else
		{
			double alpha;
			if(m_fXs == m_fXe)
			{
				if(m_fYs < m_fYe) alpha = fEDA_pi/2;
				else alpha = -1.0f*fEDA_pi/2;
			}
			else if(m_fYs == m_fYe) alpha = 0;
			else alpha = atan((m_fYe - m_fYs)/(m_fXe - m_fXs));

			pcbtrack.m_fX1 = m_fXe;
			pcbtrack.m_fY1 = m_fYe;
			pcbtrack.m_fX2 = m_fXe + m_fArrowLength*cos(alpha);
			pcbtrack.m_fY2 = m_fYe + m_fArrowLength*sin(alpha);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXe;
			pcbtrack.m_fY1 = m_fYe;
			pcbtrack.m_fX2 = m_fXe + (m_fArrowSize/cos(0.3218f))*cos(alpha + 0.3218f);
			pcbtrack.m_fY2 = m_fYe + (m_fArrowSize/cos(0.3218f))*sin(alpha + 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXe;
			pcbtrack.m_fY1 = m_fYe;
			pcbtrack.m_fX2 = m_fXe + (m_fArrowSize/cos(0.3218f))*cos(alpha - 0.3218f);
			pcbtrack.m_fY2 = m_fYe + (m_fArrowSize/cos(0.3218f))*sin(alpha - 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXs;
			pcbtrack.m_fY1 = m_fYs;
			pcbtrack.m_fX2 = m_fXs - m_fArrowLength*cos(alpha);
			pcbtrack.m_fY2 = m_fYs - m_fArrowLength*sin(alpha);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXs;
			pcbtrack.m_fY1 = m_fYs;
			pcbtrack.m_fX2 = m_fXs - (m_fArrowSize/cos(0.3218f))*cos(alpha + 0.3218f);
			pcbtrack.m_fY2 = m_fYs - (m_fArrowSize/cos(0.3218f))*sin(alpha + 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);

			pcbtrack.m_fX1 = m_fXs;
			pcbtrack.m_fY1 = m_fYs;
			pcbtrack.m_fX2 = m_fXs - (m_fArrowSize/cos(0.3218f))*cos(alpha - 0.3218f);
			pcbtrack.m_fY2 = m_fYs - (m_fArrowSize/cos(0.3218f))*sin(alpha - 0.3218f);
			if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);
		}
	}
}

BOOL  COb_pcbdimension::Is_On_Me(double fx, double fy, CView* pV)
{
    CESDDoc* pDoc = (CESDDoc*)pDocument;
	CESDView* pView = (CESDView*)pV;
	
	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return FALSE;

	COb_pcbtext pcbtext;
	pcbtext.pDocument = pDocument;
	if(m_nDimensionKind == 8)   //Dimension
	{
		double d;

		pcbtext.m_fSx = m_fTextSx;
		pcbtext.m_fSy = m_fTextSy;
		pcbtext.m_fHeight = m_fTextHeight;
		pcbtext.m_fWidth = m_fTextLineWidth;
		pcbtext.m_fRotation = m_fRotation;
		pcbtext.m_nLayer = m_nLayer;
		pcbtext.m_nStrokeFont = m_nStrokeFont;
		pcbtext.m_bUseTTFont = m_bUseTTFont;
		pcbtext.m_bBold = m_bBold;
		pcbtext.m_bItalic = m_bItalic;
		pcbtext.m_csTTFontName = m_csTTFontName;

		d = sqrt((m_fXs - m_fXe)*(m_fXs - m_fXe) + (m_fYs - m_fYe)*(m_fYs - m_fYe));

		if(pDoc->m_cPCB_Sheet.m_nUnit == UNIT_PCB_I)
		{
			char str[100];
			CString cstr;
			pView->mil_to_str(d, str);
			cstr = str;
			cstr.Trim("mil");

			if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx
			{
				pcbtext.m_csText = cstr;
			}
			else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil
			{
				pcbtext.m_csText = cstr + "mil";
			}
			else	//xxxx (mil)
			{
				pcbtext.m_csText = cstr + " (mil)";
			}
		}
		else
		{
			char str[100];
			CString cstr;
			pView->mm_to_str(d, str);
			cstr = str;
			cstr.Trim("m");

			if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx
			{
				pcbtext.m_csText = cstr;
			}
			else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil
			{
				pcbtext.m_csText = cstr + "mm";
			}
			else	//xxxx (mil)
			{
				pcbtext.m_csText = cstr + " (mm)";
			}
		}
	}
	else if(m_nDimensionKind == 1)	//Embedded --- Linear
	{
		double d;

		pcbtext.m_fSx = m_fTextSx;
		pcbtext.m_fSy = m_fTextSy;
		pcbtext.m_fHeight = m_fTextHeight;
		pcbtext.m_fWidth = m_fTextLineWidth;
		pcbtext.m_fRotation = m_fTextRotation;
		pcbtext.m_nLayer = m_nLayer;
		pcbtext.m_nStrokeFont = m_nStrokeFont;
		pcbtext.m_bUseTTFont = m_bUseTTFont;
		pcbtext.m_bBold = m_bBold;
		pcbtext.m_bItalic = m_bItalic;
		pcbtext.m_csTTFontName = m_csTTFontName;

		double alpha;
		if(m_cRefPoint[0].fx == m_cRefPoint[1].fx) 
		{
			if(m_cRefPoint[1].fy > m_cRefPoint[0].fy) alpha = fEDA_pi/2;
			else alpha = -1.0f*fEDA_pi/2;
		}
		else if(m_cRefPoint[0].fy == m_cRefPoint[1].fy) alpha = 0;
		else alpha = atan((m_cRefPoint[1].fy - m_cRefPoint[0].fy)/(m_cRefPoint[1].fx - m_cRefPoint[0].fx));

		d = abs(cos(sqrt((alpha - m_fRotation*fEDA_pi/180)*(alpha - m_fRotation*fEDA_pi/180)))*sqrt((m_cRefPoint[0].fx - m_cRefPoint[1].fx)*(m_cRefPoint[0].fx - m_cRefPoint[1].fx) + (m_cRefPoint[0].fy - m_cRefPoint[1].fy)*(m_cRefPoint[0].fy - m_cRefPoint[1].fy)));

		char str[100];
		pView->size_to_unitstr(d, str, m_nTextFormat, m_nTextPrecision, m_nTextDimensionUnit);
		
		
		pcbtext.m_csText = m_csPrefix + str + m_csSuffix;
	}

	double fh = pcbtext.m_fHeight;
	double fw = pcbtext.GetStringWidth(pV);	

	if(fw == 0) return FALSE;
	
	pcbtext.m_fX1 = pcbtext.m_fSx;
	pcbtext.m_fY1 = pcbtext.m_fSy;
	pcbtext.m_fX2 = pcbtext.m_fSx + fw;
	pcbtext.m_fY2 = pcbtext.m_fSy + fh;	

	double alpha, r;
	alpha = atan(fh/fw);
	r = sqrt(fw*fw + fh*fh)/2;

	double deltax = r*cos(alpha) - r*cos(alpha + pcbtext.m_fRotation*fEDA_pi/180);
	double deltay = r*sin(alpha + pcbtext.m_fRotation*fEDA_pi/180) - r*sin(alpha);

	pcbtext.m_fX1 -= deltax;
	pcbtext.m_fY1 += deltay;
	pcbtext.m_fX2 -= deltax;
	pcbtext.m_fY2 += deltay;

	return pcbtext.Is_On_Me(fx, fy, pV);
}

void COb_pcbdimension::Change(int index, CView* pV)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	CDlg_PCBEditDimension dlg;

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

	dlg.m_csEditStartX = pDoc->Convert_XY_To_String(m_fXs  - pDoc->m_cPCB_Sheet.m_fOriginX);
	dlg.m_csEditStartY = pDoc->Convert_XY_To_String(m_fYs  - pDoc->m_cPCB_Sheet.m_fOriginY);
	dlg.m_csEditEndX = pDoc->Convert_XY_To_String(m_fXe  - pDoc->m_cPCB_Sheet.m_fOriginX);
	dlg.m_csEditEndY = pDoc->Convert_XY_To_String(m_fYe  - pDoc->m_cPCB_Sheet.m_fOriginY);
	dlg.m_csEditLineHeight = pDoc->Convert_XY_To_String(m_fHeight);
	dlg.m_csEditLineWidth = pDoc->Convert_XY_To_String(m_fLineWidth);
	dlg.m_csEditTextHeight = pDoc->Convert_XY_To_String(m_fTextHeight);
	dlg.m_csEditTextWidth = pDoc->Convert_XY_To_String(m_fTextLineWidth);

	dlg.m_nComboStyle = m_nUnitStyle;
	dlg.m_bCheckLocked = m_bLocked;
	
	//TTFont
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

	//True Type Font
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
		pundo->m_nOb = PCB_ELEMENT_DIMENSION;
		pundo->m_nIndex = index;
		pundo->pcbdimension.Copy(this);
			
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
		
		m_fXs = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditStartX) + pDoc->m_cPCB_Sheet.m_fOriginX;
		m_fYs = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditStartY) + pDoc->m_cPCB_Sheet.m_fOriginY;
		m_fXe = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditEndX) + pDoc->m_cPCB_Sheet.m_fOriginX;
		m_fYe = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditEndY) + pDoc->m_cPCB_Sheet.m_fOriginY;
		m_fHeight = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditLineHeight);
		m_fLineWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditLineWidth);
		m_fTextHeight = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTextHeight);
		m_fTextLineWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTextWidth);

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

		//True Type Font
		m_bBold = dlg.m_bCheckBold;
		m_bItalic = dlg.m_bCheckItalic;

		if(m_fXs > m_fXe)
		{
			double ftemp = m_fXs;
			m_fXs = m_fXe;
			m_fXe = ftemp;
			ftemp = m_fYs;
			m_fYs = m_fYe;
			m_fYe = ftemp;
		}

		UpdateDimensionText( pV);
	}
}

void COb_pcbdimension::Change_Linear(int index, CView* pV)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	CDlg_PCBEditDimensionLinear dlg;

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

	dlg.m_csEditArrowLength = pDoc->Convert_XY_To_String(m_fArrowLength);
	dlg.m_csEditArrowWidth = pDoc->Convert_XY_To_String(m_fArrowLineWidth);
	dlg.m_csEditExtWidth =  pDoc->Convert_XY_To_String(m_fExtensionLineWidth);
	dlg.m_csEditGap = pDoc->Convert_XY_To_String(m_fExtensionPickGap);
	dlg.m_csEditLineWidth = pDoc->Convert_XY_To_String(m_fLineWidth);
	dlg.m_csEditOffset = pDoc->Convert_XY_To_String(m_fExtensionOffset);
	dlg.m_csEditPrefix = m_csPrefix;
	dlg.m_csEditSuffix = m_csSuffix;

	dlg.m_csEditTextGap = pDoc->Convert_XY_To_String(m_fTextGap);
	dlg.m_csEditTextHeight = pDoc->Convert_XY_To_String(m_fTextHeight);
	dlg.m_csEditTextWidth = pDoc->Convert_XY_To_String(m_fTextLineWidth);
	dlg.m_fEditRotation = m_fTextRotation;

	dlg.m_nComboTextPosition = 0; //m_nTextPosition; 总是 = "Auto"
	dlg.m_nComboArrowPosition = m_nArrowPosition;
	dlg.m_nComboPrecision = m_nTextPrecision;
	dlg.m_nComboUnit = m_nTextDimensionUnit;   if(dlg.m_nComboUnit > 4) dlg.m_nComboUnit = 4;
	dlg.m_nComboFormat = m_nTextFormat - 1;
	dlg.m_bCheckLocked = m_bLocked;

	//TTFont
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

	//True Type Font
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
		pundo->m_nOb = PCB_ELEMENT_DIMENSION;
		pundo->m_nIndex = index;
		pundo->pcbdimension.Copy(this);
			
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
		
		m_fArrowLength = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditArrowLength);
		m_fArrowLineWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditArrowWidth);
		m_fExtensionLineWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditExtWidth);
		m_fExtensionPickGap = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditGap);
		m_fLineWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditLineWidth);
		m_fExtensionOffset = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditOffset);
		m_csPrefix = dlg.m_csEditPrefix;
		m_csSuffix = dlg.m_csEditSuffix;

		m_fTextGap = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTextGap);
		m_fTextHeight = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTextHeight);
		m_fTextLineWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTextWidth);
		m_fTextRotation = dlg.m_fEditRotation;

		m_nTextPosition = dlg.m_nComboTextPosition;
		m_nArrowPosition = dlg.m_nComboArrowPosition;
		m_nTextPrecision = dlg.m_nComboPrecision;
		m_nTextDimensionUnit = dlg.m_nComboUnit;  if(m_nTextDimensionUnit >= 4) m_nTextDimensionUnit = UNIT_AUTO;
		m_nTextFormat = dlg.m_nComboFormat + 1;	

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

		//True Type Font
		m_bBold = dlg.m_bCheckBold;
		m_bItalic = dlg.m_bCheckItalic;

	}
}

void COb_pcbdimension::Copy(COb_pcbdimension* pdimension)
{
	pDocument = pdimension->pDocument;
	m_fLx = pdimension->m_fLx;
	m_fLy = pdimension->m_fLy;
	m_fHx = pdimension->m_fHx;
	m_fHy = pdimension->m_fHy;
	m_fXs = pdimension->m_fXs;
	m_fYs = pdimension->m_fYs;
	m_fXe = pdimension->m_fXe;
	m_fYe = pdimension->m_fYe;
	m_fHeight = pdimension->m_fHeight;
	m_fLineWidth = pdimension->m_fLineWidth;
	m_fRotation = pdimension->m_fRotation;

	for(int i=0; i<pdimension->m_cRefPoint.GetCount(); i++)
	{
		Fpoint fpoint = pdimension->m_cRefPoint.GetAt(i);
		m_cRefPoint.Add(fpoint);
	}

	m_fArrowSize = pdimension->m_fArrowSize;
	m_fArrowLineWidth = pdimension->m_fArrowLineWidth;
	m_fArrowLength = pdimension->m_fArrowLength;
	m_nArrowPosition = pdimension->m_nArrowPosition;

	m_fExtensionOffset = pdimension->m_fExtensionOffset;
	m_fExtensionLineWidth = pdimension->m_fExtensionLineWidth;
	m_fExtensionPickGap = pdimension->m_fExtensionPickGap;

	m_fTextSx = pdimension->m_fTextSx;
	m_fTextSy = pdimension->m_fTextSy;
	m_fTextHeight = pdimension->m_fTextHeight;
	m_fTextLineWidth = pdimension->m_fTextLineWidth;
	m_fTextRotation = pdimension->m_fTextRotation;
	
	m_nTextPosition = pdimension->m_nTextPosition;
	m_fTextGap = pdimension->m_fTextGap;
	m_nTextFormat = pdimension->m_nTextFormat;
	m_nTextDimensionUnit = pdimension->m_nTextDimensionUnit;
	m_nTextPrecision = pdimension->m_nTextPrecision;

	m_csPrefix = pdimension->m_csPrefix;
	m_csSuffix = pdimension->m_csSuffix;

	m_nLayer = pdimension->m_nLayer;
	m_nUnitStyle = pdimension->m_nUnitStyle;
	m_nDimensionKind = pdimension->m_nDimensionKind;

	m_bLocked = pdimension->m_bLocked;
	m_bPrimitiveLock = pdimension->m_bPrimitiveLock;

	m_nStrokeFont = pdimension->m_nStrokeFont;
	m_bUseTTFont = pdimension->m_bUseTTFont;
	m_bBold = pdimension->m_bBold;
	m_bItalic = pdimension->m_bItalic;
	m_csTTFontName = pdimension->m_csTTFontName;
	
	m_bPolygonOutline = pdimension->m_bPolygonOutline;
	m_bUserRouted = pdimension->m_bUserRouted;
	m_nUnionIndex = pdimension->m_nUnionIndex;

	m_bSelection = FALSE;//pdimension->m_bSelection;
}

void COb_pcbdimension::UpdateDimensionText(CView* pV)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	CESDView* pView = (CESDView*)pV;
	
	COb_pcbtext atext;
	atext.pDocument = pDocument;
	atext.m_fHeight = m_fTextHeight;
	atext.m_fWidth = m_fTextLineWidth;
	atext.m_nLayer = m_nLayer;
	atext.m_nStrokeFont = m_nStrokeFont;
	atext.m_bUseTTFont = m_bUseTTFont;
	atext.m_bBold = m_bBold;
	atext.m_bItalic = m_bItalic;
	atext.m_csTTFontName = m_csTTFontName;
	
	double d = sqrt((m_fXs - m_fXe)*(m_fXs - m_fXe) + (m_fYs - m_fYe)*(m_fYs - m_fYe));

	if(pDoc->m_cPCB_Sheet.m_nUnit == UNIT_PCB_I)
	{
		char str[100];
		CString cstr;
		pView->mil_to_str(d, str);
		cstr = str;
		cstr.Trim("mil");

		if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx
		{
			atext.m_csText = cstr;
		}
		else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil
		{
			atext.m_csText = cstr + "mil";
		}
		else	//xxxx (mil)
		{
			atext.m_csText = cstr + " (mil)";
		}
	}
	else
	{
		char str[100];
		CString cstr;
		pView->mm_to_str(d, str);
		cstr = str;
		cstr.Trim("m");

		if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx
		{
			atext.m_csText = cstr;
		}
		else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil
		{
			atext.m_csText = cstr + "mm";
		}
		else	//xxxx (mil)
		{
			atext.m_csText = cstr + " (mm)";
		}
	}
	
	atext.m_fSx = atext.m_fSy = 0;
	atext.m_fRotation = 0;

	atext.UpdateStringBorder(pV, TRUE);
	//double fw = atext.GetStringWidth(pV);
	double fw = abs(atext.m_fX2 - atext.m_fX1);
	double fh = abs(atext.m_fY2 - atext.m_fY1);
	
	double alpha;
	if(m_fXs == m_fXe)
	{
		if(m_fYs < m_fYe) alpha = fEDA_pi/2;
		else alpha = -1.0f*fEDA_pi/2;
	}
	else if(m_fYs == m_fYe) alpha = 0;
	else alpha = atan((m_fYe - m_fYs)/(m_fXe - m_fXs));

	if( (d-fw) > 80.0f)
	{
		double xs, ys;
		xs = m_fXs + ((d-fw)/2)*cos(alpha);
		ys = m_fYs + ((d-fw)/2)*sin(alpha);

		atext.m_fSx = xs + fh*sin(alpha)/2;
		atext.m_fSy = ys - fh*cos(alpha)/2;
		atext.m_fRotation = alpha*180/fEDA_pi;

		m_fTextSx = atext.m_fSx;
		m_fTextSy = atext.m_fSy;
		m_fRotation = m_fTextRotation = atext.m_fRotation;
	}
	else
	{
		double xs, ys;
		xs = m_fXs + ((d-fw)/2)*cos(alpha);
		ys = m_fYs + ((d-fw)/2)*sin(alpha);

		atext.m_fSx = xs + (1.5*fh+m_fHeight)*sin(alpha);
		atext.m_fSy = ys - (1.5*fh+m_fHeight)*cos(alpha);
		atext.m_fRotation = alpha*180/fEDA_pi;

		m_fTextSx = atext.m_fSx;
		m_fTextSy = atext.m_fSy;
		m_fRotation = m_fTextRotation = atext.m_fRotation;
	}
}

void COb_pcbdimension::Move(CClientDC* pDC, CView* pV,int index)
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

	COb_pcbdimension adimension;
	adimension.Copy(this);
	adimension.m_nFlag = FLAG_NORMAL;

	adimension.Draw(pDC, pV, 1);
	pView->OpenMouseCursor();

	double fx0, fy0;
	fx0 = adimension.m_fTextSx;
	fy0 = adimension.m_fTextSy;

	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	int flag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			adimension.Draw(pDC, pV, 1);
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
				adimension.Draw(pDC, pV, 1);
				pView->OpenMouseCursor();
			}

			adimension.m_fTextSx = pView->m_fCurrentSnapX;
			adimension.m_fTextSy = pView->m_fCurrentSnapY;
			double deltax, deltay;
			deltax = adimension.m_fTextSx - fx0;
			deltay = adimension.m_fTextSy - fy0;
			
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fXs - fx0, m_fYs - fy0, flag*90,  FALSE);
			adimension.m_fXs = fx + fx0 + deltax;
			adimension.m_fYs = fy + fy0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fXe - fx0, m_fYe - fy0, flag*90,  FALSE);
			adimension.m_fXe = fx + fx0 + deltax;
			adimension.m_fYe = fy + fy0 + deltay;

			if(adimension.m_fXs > adimension.m_fXe)
			{
				double ftemp = adimension.m_fXs;
				adimension.m_fXs = adimension.m_fXe;
				adimension.m_fXe = ftemp;
				ftemp = adimension.m_fYs;
				adimension.m_fYs = adimension.m_fYe;
				adimension.m_fYe = ftemp;
			}
			adimension.UpdateDimensionText(pV);

			pView->CloseMouseCursor();
			adimension.Draw(pDC, pV, 1);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			adimension.Draw(pDC, pV, 1);
			pView->OpenMouseCursor();

			//----------- Undo ----------------------------------------------------------------------------------
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_DIMENSION;
			pundo->m_nIndex = index;
			pundo->pcbdimension.Copy(this);
			pundo->pcbdimension.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			//---------------------------------------------------------------------------------------------------

			m_fTextSx = adimension.m_fTextSx;
			m_fTextSy = adimension.m_fTextSy;
			m_fTextRotation = adimension.m_fTextRotation;
			m_fXs = adimension.m_fXs;
			m_fYs = adimension.m_fYs;
			m_fXe = adimension.m_fXe;
			m_fYe = adimension.m_fYe;
			UpdateDimensionText(pV);

			break;
		}
		else if(ch1 == VK_SPACE)
		{
			pView->CloseMouseCursor();
			adimension.Draw(pDC, pV, 1);
			pView->OpenMouseCursor();
			
			flag++;
			if(flag>3) flag=0;

			adimension.m_fTextSx = pView->m_fCurrentSnapX;
			adimension.m_fTextSy = pView->m_fCurrentSnapY;
			double deltax, deltay;
			deltax = adimension.m_fTextSx - fx0;
			deltay = adimension.m_fTextSy - fy0;
			
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fXs - fx0, m_fYs - fy0, flag*90,  FALSE);
			adimension.m_fXs = fx + fx0 + deltax;
			adimension.m_fYs = fy + fy0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fXe - fx0, m_fYe - fy0, flag*90,  FALSE);
			adimension.m_fXe = fx + fx0 + deltax;
			adimension.m_fYe = fy + fy0 + deltay;

			if(adimension.m_fXs > adimension.m_fXe)
			{
				double ftemp = adimension.m_fXs;
				adimension.m_fXs = adimension.m_fXe;
				adimension.m_fXe = ftemp;
				ftemp = adimension.m_fYs;
				adimension.m_fYs = adimension.m_fYe;
				adimension.m_fYe = ftemp;
			}
			adimension.UpdateDimensionText(pV);

			pView->CloseMouseCursor();
			adimension.Draw(pDC, pV, 1);
			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	
	pView->CloseMouseCursor();

	m_nFlag = FLAG_NORMAL;
	Draw(pDC, pV, 0);
	pView->OpenMouseCursor();

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	pView->Write_Help(szEDA_Help_Ready);
}

void COb_pcbdimension::DrawSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;

	COb_pcbtext pcbtext;
	pcbtext.m_fSx = m_fTextSx;
	pcbtext.m_fSy = m_fTextSy;
	pcbtext.m_fHeight = m_fTextHeight;
	pcbtext.m_fWidth = m_fTextLineWidth;
	pcbtext.m_fRotation = m_fTextRotation;
	pcbtext.m_nLayer = m_nLayer;
	pcbtext.m_nStrokeFont = m_nStrokeFont;
	pcbtext.m_bUseTTFont = m_bUseTTFont;
	pcbtext.m_bBold = m_bBold;
	pcbtext.m_bItalic = m_bItalic;
	pcbtext.m_csTTFontName = m_csTTFontName;
	pcbtext.m_nFlag = m_nFlag;

	if(m_nDimensionKind == 8)   //Dimension
	{
		//double fw;
		double d;

		d = sqrt((m_fXs - m_fXe)*(m_fXs - m_fXe) + (m_fYs - m_fYe)*(m_fYs - m_fYe));

		if(pDoc->m_cPCB_Sheet.m_nUnit == UNIT_PCB_I)
		{
			char str[100];
			CString cstr;
			pView->mil_to_str(d, str);
			cstr = str;
			cstr.Trim("mil");

			if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx
			{
				pcbtext.m_csText = cstr;
			}
			else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil
			{
				pcbtext.m_csText = cstr + "mil";
			}
			else	//xxxx (mil)
			{
				pcbtext.m_csText = cstr + " (mil)";
			}
		}
		else
		{
			char str[100];
			CString cstr;
			pView->mm_to_str(d, str);
			cstr = str;
			cstr.Trim("m");

			if(m_nUnitStyle == UNIT_STYLE_NONE)   //xxxx
			{
				pcbtext.m_csText = cstr;
			}
			else if(m_nUnitStyle == UNIT_STYLE_NORMAL)	//xxxxmil
			{
				pcbtext.m_csText = cstr + "mm";
			}
			else	//xxxx (mil)
			{
				pcbtext.m_csText = cstr + " (mm)";
			}
		}
	}
	else if(m_nDimensionKind == 1)	//Embedded --- Linear
	{
		//double fw;
		double d;

		double alpha;
		if(m_cRefPoint[0].fx == m_cRefPoint[1].fx) 
		{
			if(m_cRefPoint[1].fy > m_cRefPoint[0].fy) alpha = fEDA_pi/2;
			else alpha = -1.0f*fEDA_pi/2;
		}
		else if(m_cRefPoint[0].fy == m_cRefPoint[1].fy) alpha = 0;
		else alpha = atan((m_cRefPoint[1].fy - m_cRefPoint[0].fy)/(m_cRefPoint[1].fx - m_cRefPoint[0].fx));

		d = abs(cos(sqrt((alpha - m_fRotation*fEDA_pi/180)*(alpha - m_fRotation*fEDA_pi/180)))*sqrt((m_cRefPoint[0].fx - m_cRefPoint[1].fx)*(m_cRefPoint[0].fx - m_cRefPoint[1].fx) + (m_cRefPoint[0].fy - m_cRefPoint[1].fy)*(m_cRefPoint[0].fy - m_cRefPoint[1].fy)));

		char str[100];
		pView->size_to_unitstr(d, str, m_nTextFormat, m_nTextPrecision, m_nTextDimensionUnit);

		pcbtext.m_csText = m_csPrefix + str + m_csSuffix;
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
// Dlg_PCBBoardOptions.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBBoardOptions.h"
#include ".\dlg_pcbboardoptions.h"
#include "systemext.h"

// CDlg_PCBBoardOptions 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBBoardOptions, CDialog)
CDlg_PCBBoardOptions::CDlg_PCBBoardOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBBoardOptions::IDD, pParent)
	, m_nComboUnit(0)
	, m_csComboSnapX(_T(""))
	, m_csComboSnapY(_T(""))
	, m_csComboCompX(_T(""))
	, m_csComboCompY(_T(""))
	, m_bCheckEGridEnable(FALSE)
	, m_csComboEGridRange(_T(""))
	, m_nComboVGridStyle(0)
	, m_csComboVGrid1(_T(""))
	, m_csComboVGrid2(_T(""))
	, m_csEditSheetX(_T(""))
	, m_csEditSheetY(_T(""))
	, m_csEditSheetWidth(_T(""))
	, m_csEditSheetHeight(_T(""))
	, m_bCheckShowSheet(FALSE)
	, m_bCheckLockSheet(FALSE)
	, m_nComboDesignatorDisplay(0)
	, m_bCheckAllLayer(FALSE)
{
}

CDlg_PCBBoardOptions::~CDlg_PCBBoardOptions()
{
}

void CDlg_PCBBoardOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_UNIT, m_nComboUnit);
	DDX_CBString(pDX, IDC_COMBO_SNAP_X, m_csComboSnapX);
	DDX_CBString(pDX, IDC_COMBO_SNAP_Y, m_csComboSnapY);
	DDX_CBString(pDX, IDC_COMBO_COMP_X, m_csComboCompX);
	DDX_CBString(pDX, IDC_COMBO_COMP_Y, m_csComboCompY);
	DDX_Check(pDX, IDC_CHECK_EGRID_ENABLE, m_bCheckEGridEnable);
	DDX_CBString(pDX, IDC_COMBO_EGRID_RANGE, m_csComboEGridRange);
	DDX_CBIndex(pDX, IDC_COMBO_VGRID_STYLE, m_nComboVGridStyle);
	DDX_CBString(pDX, IDC_COMBO_VGRID1, m_csComboVGrid1);
	DDX_CBString(pDX, IDC_COMBO_VGRID2, m_csComboVGrid2);
	DDX_Text(pDX, IDC_EDIT_SHEET_X, m_csEditSheetX);
	DDX_Text(pDX, IDC_EDIT_SHEET_Y, m_csEditSheetY);
	DDX_Text(pDX, IDC_EDIT_SHEET_WIDTH, m_csEditSheetWidth);
	DDX_Text(pDX, IDC_EDIT_SHEET_HEIGHT, m_csEditSheetHeight);
	DDX_Check(pDX, IDC_CHECK_SHOW_SHEET, m_bCheckShowSheet);
	DDX_Check(pDX, IDC_CHECK_LOCK_SHEET, m_bCheckLockSheet);
	DDX_CBIndex(pDX, IDC_COMBO_DESIGNATOR_DISPLAY, m_nComboDesignatorDisplay);
	DDX_Check(pDX, IDC_CHECK_ALL_LAYER, m_bCheckAllLayer);
}


BEGIN_MESSAGE_MAP(CDlg_PCBBoardOptions, CDialog)
	ON_BN_CLICKED(IDC_SHEET_SELECT, OnBnClickedSheetSelect)
	ON_BN_CLICKED(IDC_SHEET_CLEAR, OnBnClickedSheetClear)
	//ON_BN_CLICKED(IDC_BUTTON_XY, OnBnClickedButtonXy)
END_MESSAGE_MAP()


// CDlg_PCBBoardOptions 消息处理程序


void CDlg_PCBBoardOptions::OnBnClickedSheetSelect()
{
	POSITION pos = m_pDoc->GetFirstViewPosition();
	CView* pFirstView = NULL;
	if(pos != NULL) pFirstView = m_pDoc->GetNextView( pos );
	if(pFirstView == NULL) return;

	char filename[MAX_PATH];
	strcpy(filename, csEDA_System_Path);
	strcat(filename,"\\utility\\*.eda");

	char szFileFilter[] = "图框样板文件(*.eda)|*.eda;|所有文件 (*.*) |*.*||";
	CFileDialog importdialog(TRUE,NULL,filename, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_ENABLESIZING,szFileFilter,NULL);

	if(importdialog.DoModal() == IDOK)
	{
		CFile myfile;
		if(myfile.Open(importdialog.GetPathName(), CFile::modeRead) == TRUE)
		{
			CArchive ar(&myfile, CArchive::load);
			CString onelinestr;
			
			if(ar.ReadString(onelinestr) == TRUE)
			{
				if((onelinestr.Find(csESDFileHead_V1_1) < 0) && (onelinestr.Find(csESDFileHead_V1_2) < 0))
				{
					AfxMessageBox("文件格式错误, 无法读入图框样板文件。");
					ar.Close();
					myfile.Close();
					return;
				}
			}
			else
			{
				AfxMessageBox("文件格式错误, 无法读入图框样板文件。");
				ar.Close();
				myfile.Close();
				return;
			}

			
			CArray <LOGFONT, LOGFONT&>  arrayFont[SCH_SHEET_NUM];
			double fSCH_ScreenX[SCH_SHEET_NUM];
			double fSCH_ScreenY[SCH_SHEET_NUM];
			double fSCH_Scale[SCH_SHEET_NUM];
	
			
			COb_schsheet cSCH_Sheet[SCH_SHEET_NUM];

			
			BOOL bSCH_SheetShow[SCH_SHEET_NUM];
			CString csSCH_SheetName[SCH_SHEET_NUM];	
	
			int nSCH_Unit;
			BOOL bSCH_VisibleGridShow;
			double fSCH_VisibleGrid;
			BOOL bSCH_SnapGridEnable;
			double fSCH_SnapGrid;
			BOOL bSCH_ElecGridEnable;
			double fSCH_ElecGrid;

			
			for(int nCurrentDiagram=0; nCurrentDiagram<SCH_SHEET_NUM; nCurrentDiagram++)
			{
				int fontNum;
				ar >> fontNum;
				arrayFont[nCurrentDiagram].RemoveAll();
				for(int fontNo=0; fontNo<fontNum; fontNo++)
				{
					LOGFONT logfont;
					logfont.lfHeight = 10;
					logfont.lfWidth = 0;
					logfont.lfEscapement = logfont.lfOrientation = 0;
					logfont.lfWeight = FW_NORMAL;
					logfont.lfItalic = 0;
					logfont.lfUnderline = 0;
					logfont.lfStrikeOut = 0;
					logfont.lfCharSet = DEFAULT_CHARSET;
					logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
					logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
					logfont.lfQuality = PROOF_QUALITY;
					logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
					strcpy(logfont.lfFaceName, "Times New Roman");
					
					ar >> logfont.lfHeight;
					ar >> logfont.lfOrientation;
					ar >> logfont.lfUnderline;
					ar >> logfont.lfItalic;
					ar >> logfont.lfWeight;
					ar >> logfont.lfStrikeOut;
					CString csFaceName;
					ar >> csFaceName;
					strcpy(logfont.lfFaceName, csFaceName);
					arrayFont[nCurrentDiagram].Add(logfont);
				}
			}
			
			for(int nCurrentDiagram=0; nCurrentDiagram<SCH_SHEET_NUM; nCurrentDiagram++)
			{
				ar >> fSCH_ScreenX[nCurrentDiagram];		
				ar >> fSCH_ScreenY[nCurrentDiagram];		
				ar >> fSCH_Scale[nCurrentDiagram];			
				ar >> bSCH_SheetShow[nCurrentDiagram];		
				ar >> csSCH_SheetName[nCurrentDiagram];		
				cSCH_Sheet[nCurrentDiagram].Serialize(ar);	
			}
			
			ar >> nSCH_Unit;
			ar >> bSCH_SnapGridEnable;
			ar >> fSCH_SnapGrid;
			ar >> bSCH_VisibleGridShow;
			ar >> fSCH_VisibleGrid;
			ar >> bSCH_ElecGridEnable;
			ar >> fSCH_ElecGrid;


			while(!m_pDoc->m_cPCB_Sheet.m_arraySheetTrack.IsEmpty())
			{
				delete m_pDoc->m_cPCB_Sheet.m_arraySheetTrack.GetAt(0);
				m_pDoc->m_cPCB_Sheet.m_arraySheetTrack.RemoveAt(0);
			}
			while(!m_pDoc->m_cPCB_Sheet.m_arraySheetText.IsEmpty())
			{
				delete m_pDoc->m_cPCB_Sheet.m_arraySheetText.GetAt(0);
				m_pDoc->m_cPCB_Sheet.m_arraySheetText.RemoveAt(0);
			}
			//图形元素
			for(int nCurrentDiagram=0; nCurrentDiagram<1; nCurrentDiagram++)
			{
				int num;
				ar >> num;
				for(int i=0; i<num; i++)
				{	
					int elementNo;
					ar >> elementNo;
					if(elementNo == SCH_ELEMENT_LINE)
					{
						COb_schline aline;
						aline.Serialize(ar);
						for(int j=0; j<aline.m_cVertex.GetCount()-1; j++)
						{
							Fpoint fp1, fp2;
							fp1 = aline.m_cVertex.GetAt(j);
							fp2 = aline.m_cVertex.GetAt(j+1);
							COb_pcbtrack* pnewtrack = new COb_pcbtrack();
							pnewtrack->pDocument = m_pDoc;
							pnewtrack->m_nLayer = SELECTIONS;
							pnewtrack->m_fWidth =  nEDA_Line_Width[aline.m_nLineWidth];
							pnewtrack->m_fX1 = fp1.fx;
							pnewtrack->m_fY1 = fp1.fy;
							pnewtrack->m_fX2 = fp2.fx;
							pnewtrack->m_fY2 = fp2.fy;

							m_pDoc->m_cPCB_Sheet.m_arraySheetTrack.Add(pnewtrack);
						}
					}
					else if(elementNo == SCH_ELEMENT_POLYLINE)
					{
						COb_schpolyline apolyline;
						apolyline.Serialize(ar);
						for(int j=0; j<apolyline.m_cVertex.GetCount()-1; j++)
						{
							Fpoint fp1, fp2;
							fp1 = apolyline.m_cVertex.GetAt(j);
							fp2 = apolyline.m_cVertex.GetAt(j+1);
							COb_pcbtrack* pnewtrack = new COb_pcbtrack();
							pnewtrack->pDocument = m_pDoc;
							pnewtrack->m_nLayer = SELECTIONS;
							pnewtrack->m_fWidth =  nEDA_Line_Width[apolyline.m_nLineWidth];
							pnewtrack->m_fX1 = fp1.fx;
							pnewtrack->m_fY1 = fp1.fy;
							pnewtrack->m_fX2 = fp2.fx;
							pnewtrack->m_fY2 = fp2.fy;

							m_pDoc->m_cPCB_Sheet.m_arraySheetTrack.Add(pnewtrack);
						}
					}
					else if(elementNo == SCH_ELEMENT_TEXT)
					{
						COb_schtext atext;
						atext.Serialize(ar);
						LOGFONT logfont = arrayFont[0].GetAt(atext.m_nFont);

						COb_pcbtext* pnewtext = new COb_pcbtext();
						pnewtext->pDocument = m_pDoc;
						pnewtext->m_fSx = atext.m_fSx;
						pnewtext->m_fSy = atext.m_fSy;
						pnewtext->m_fHeight = logfont.lfHeight*10;
						pnewtext->m_fWidth = 1;
						pnewtext->m_fRotation = atext.m_nOrientation;
						pnewtext->m_nLayer = SELECTIONS;
						pnewtext->m_csText = atext.m_csText;
						pnewtext->m_nStrokeFont = 0;
						pnewtext->m_bUseTTFont = TRUE;
						pnewtext->m_csTTFontName = logfont.lfFaceName;

						if(atext.m_nOrientation == 0) pnewtext->m_fSy += pnewtext->m_fHeight/4;
						else if(atext.m_nOrientation == 90) pnewtext->m_fSx -= pnewtext->m_fHeight/4;
						else if(atext.m_nOrientation == 180) pnewtext->m_fSy -= pnewtext->m_fHeight/4;
						else if(atext.m_nOrientation == 270) pnewtext->m_fSx += pnewtext->m_fHeight/4;
						
						pnewtext->UpdateStringBorder(pFirstView, TRUE);
						m_pDoc->m_cPCB_Sheet.m_arraySheetText.Add(pnewtext);
					}
					//------------------------------------------------------------
					else if(elementNo == SCH_ELEMENT_POLYGON)
					{
						COb_schpolygon polygon;
						polygon.Serialize(ar);
					}
					else if(elementNo == SCH_ELEMENT_JUNCTION)
					{
						COb_schjunc junc;
						junc.Serialize(ar);
					}
					else if(elementNo == SCH_ELEMENT_ELLIPSEFILL)
					{
						COb_schellipsefill ellipsefill;
						ellipsefill.Serialize(ar);
					}
					else if(elementNo == SCH_ELEMENT_ELLIPSE)
					{
						COb_schellipse ellipse;
						ellipse.Serialize(ar);
					}
					else if(elementNo == SCH_ELEMENT_ARC)
					{
						COb_scharc arc;
						arc.Serialize(ar);
					}
					else if(elementNo == SCH_ELEMENT_PIE)
					{
						COb_schpie pie;
						pie.Serialize(ar);
					}
					else if(elementNo == SCH_ELEMENT_RECT)
					{
						COb_schrect rect;
						rect.Serialize(ar);
					}
					else if(elementNo == SCH_ELEMENT_ROUNDRECT)
					{
						COb_schroundrect roundrect;
						roundrect.Serialize(ar);
					}
					else if(elementNo == SCH_ELEMENT_PORT)
					{
						COb_schport port;
						port.Serialize(ar);
					}		
					else if(elementNo == SCH_ELEMENT_POWER)
					{
						COb_schpower power;
						power.Serialize(ar);
					}	
					else if(elementNo == SCH_ELEMENT_BEZIER)
					{
						COb_schbezier bezier;
						bezier.Serialize(ar);
					}	
					else if(elementNo == SCH_ELEMENT_GRAPHIC)
					{
						COb_schgraphic graphic;
						graphic.Serialize(ar);
					}	
					else if(elementNo == SCH_ELEMENT_TEXTFRAME)
					{
						COb_schtextframe textframe;
						textframe.Serialize(ar);
					}
					else if(elementNo == SCH_ELEMENT_SHEETSYMBOL)
					{
						COb_schsheetsymbol sheetsymbol;
						sheetsymbol.Serialize(ar);

						int subelementNum;
						ar >> subelementNum;
						for(int k=0; k<subelementNum; k++)
						{
							int subelementNo;
							ar >> subelementNo;
							if(subelementNo == SCH_ELEMENT_SHEETENTRY)
							{
								COb_schsheetentry sheetentry;
								sheetentry.Serialize(ar);
							}
							else if(subelementNo == SCH_ELEMENT_TEXT)
							{
								COb_schtext text;
								text.Serialize(ar);
							}
						}

						i += subelementNum;
					}
					else if(elementNo == SCH_ELEMENT_COMPONENT)
					{
						COb_schcomp comp;
						comp.Serialize(ar);
					
						int subelementNum;
						ar >> subelementNum;
						for(int k=0; k<subelementNum; k++)
						{
							ar >> elementNo;
							if(elementNo == SCH_ELEMENT_PIN)
							{
								COb_schpin pin;
								pin.Serialize(ar);
							}
							else if(elementNo == SCH_ELEMENT_IEEE)
							{
								COb_schieee ieee;
								ieee.Serialize(ar);
							}
							else if(elementNo == SCH_ELEMENT_ARC)
							{
								COb_scharc arc;
								arc.Serialize(ar);
							}
							else if(elementNo == SCH_ELEMENT_ELLIPSE)
							{
								COb_schellipse ellipse;
								ellipse.Serialize(ar);
							}
							else if(elementNo == SCH_ELEMENT_ELLIPSEFILL)
							{
								COb_schellipsefill ellipsefill;
								ellipsefill.Serialize(ar);
							}
							else if(elementNo == SCH_ELEMENT_PIE)
							{
								COb_schpie pie;
								pie.Serialize(ar);
							}
							else if(elementNo == SCH_ELEMENT_POLYLINE)
							{
								COb_schpolyline polyline;
								polyline.Serialize(ar);
							}
							else if(elementNo == SCH_ELEMENT_RECT)
							{
								COb_schrect rect;
								rect.Serialize(ar);
							}
							else if(elementNo == SCH_ELEMENT_ROUNDRECT)
							{
								COb_schroundrect roundrect;
								roundrect.Serialize(ar);
							}
							else if(elementNo == SCH_ELEMENT_POLYGON)
							{
								COb_schpolygon polygon;
								polygon.Serialize(ar);
							}
							else if(elementNo == SCH_ELEMENT_BEZIER)
							{
								COb_schbezier bezier;
								bezier.Serialize(ar);
							}
							else if(elementNo == SCH_ELEMENT_TEXTFRAME)
							{
								COb_schtextframe textframe;
								textframe.Serialize(ar);
							}		
							else if(elementNo == SCH_ELEMENT_GRAPHIC)
							{
								COb_schgraphic graphic;
								graphic.Serialize(ar);
							}					
							else if(elementNo == SCH_ELEMENT_TEXT)
							{
								COb_schtext text;
								text.Serialize(ar);
							}
						}
						i += subelementNum;
					}
				}
			}

			ar.Flush();
			ar.Close();
			myfile.Close();


			m_csEditSheetWidth = m_pDoc->Convert_XY_To_String(cSCH_Sheet[0].m_fCustomWidth);
			m_csEditSheetHeight = m_pDoc->Convert_XY_To_String(cSCH_Sheet[0].m_fCustomHeight);
			m_bCheckShowSheet = TRUE;

			EndDialog(IDOK);
		}
	}
}

void CDlg_PCBBoardOptions::OnBnClickedSheetClear()
{
	if(AfxMessageBox("图框数据将被清除并且不可恢复, 确认?", MB_YESNO|MB_ICONWARNING) == IDYES)
	{
		while(!m_pDoc->m_cPCB_Sheet.m_arraySheetTrack.IsEmpty())
		{
			delete m_pDoc->m_cPCB_Sheet.m_arraySheetTrack.GetAt(0);
			m_pDoc->m_cPCB_Sheet.m_arraySheetTrack.RemoveAt(0);
		}
		while(!m_pDoc->m_cPCB_Sheet.m_arraySheetText.IsEmpty())
		{
			delete m_pDoc->m_cPCB_Sheet.m_arraySheetText.GetAt(0);
			m_pDoc->m_cPCB_Sheet.m_arraySheetText.RemoveAt(0);
		}
	}
}


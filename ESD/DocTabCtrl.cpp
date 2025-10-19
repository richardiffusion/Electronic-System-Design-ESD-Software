// DocTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "DocTabCtrl.h"
#include ".\doctabctrl.h"


// CDocTabCtrl

IMPLEMENT_DYNAMIC(CDocTabCtrl, CXTPTabClientWnd)
CDocTabCtrl::CDocTabCtrl()
{
}

CDocTabCtrl::~CDocTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CDocTabCtrl, CXTPTabClientWnd)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_TAB_FILE_CLOSE, OnTabFileClose)
END_MESSAGE_MAP()



// CDocTabCtrl 消息处理程序


void CDocTabCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CXTPTabManagerItem* pItemSelected = GetSelectedItem();
	CXTPTabManagerItem* pItem = HitTest(point);
	if(pItemSelected == pItem) 
	{
		//CMenu menu;
		CMenu Menu;
		CPoint pt = point;
		GetCursorPos(&pt);
		if(Menu.LoadMenu(IDR_FILE_TAB_CLOSE))
		{
			CMenu* pSubMenu = Menu.GetSubMenu(0);
			if(pSubMenu != NULL)
			{
				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

			}
		}
	}
	/*if(pItem != NULL)
	{
		TRACE1("%d\n", pItem->GetIndex());
		if(pItem->IsFocused() == TRUE) TRACE("haha\n");
	}*/

	CXTPTabClientWnd::OnRButtonDown(nFlags, point);
}

void CDocTabCtrl::OnTabFileClose()
{
	AfxMessageBox("hi");
}

#include "MyMenu.h"

//void CMyMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) {
//    lpMeasureItemStruct->itemHeight = 64; // 项高
//    lpMeasureItemStruct->itemWidth = 280; // 项宽
//}
//
//void CMyMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) {
//    CRect rect = lpDrawItemStruct->rcItem;
//
//    CDC dc;
//    dc.Attach(lpDrawItemStruct->hDC);
//    dc.FillSolidRect(rect, RGB(145, 201, 64)); // (18, 142, 255)   143 223 246
//    CFont Font;
//    Font.CreatePointFont(200, L"微软雅黑"); // 创建字体
//    dc.SelectObject(&Font);
//
//    if (lpDrawItemStruct->itemState&ODS_SELECTED) {
//        dc.FillSolidRect(rect, RGB(80, 89, 202)); // 菜单被选中  // RGB(80, 89, 202)
//    }
//    dc.SetTextColor(RGB(255, 255, 255)); // 设置文本颜色
//    CRect rt = rect;
//    rt.OffsetRect(40, 0);
//    dc.DrawText((LPCTSTR)lpDrawItemStruct->itemData, rt, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
//    dc.Detach();
//}
//
//void CMyMenu::ChangeMenuItem(CMenu *pMenu) {
//    int itemCount = pMenu->GetMenuItemCount();
//    for (int i = 0; i < itemCount; i++) {
//        CString *pText = new CString;
//        UINT itemID = pMenu->GetMenuItemID(i); // 获取菜单项ID号
//        pMenu->GetMenuString(i, *pText, MF_BYPOSITION); // 获取菜单文本
//
//        // ModifyMenu函数最后一个参数对应DRAWITEMSTRUCT结构里的itemData变量
//        pMenu->ModifyMenu(i, MF_OWNERDRAW | MF_BYPOSITION | MF_STRING, itemID, pText->GetBuffer(255));
//        pText->ReleaseBuffer();
//        free(pText);
//        if (itemID == -1) { // 如果是一个弹出式菜单
//            ChangeMenuItem(pMenu->GetSubMenu(i));
//        }
//    }
//}

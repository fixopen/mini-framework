#include "MyMenu.h"

//void CMyMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) {
//    lpMeasureItemStruct->itemHeight = 64; // ���
//    lpMeasureItemStruct->itemWidth = 280; // ���
//}
//
//void CMyMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) {
//    CRect rect = lpDrawItemStruct->rcItem;
//
//    CDC dc;
//    dc.Attach(lpDrawItemStruct->hDC);
//    dc.FillSolidRect(rect, RGB(145, 201, 64)); // (18, 142, 255)   143 223 246
//    CFont Font;
//    Font.CreatePointFont(200, L"΢���ź�"); // ��������
//    dc.SelectObject(&Font);
//
//    if (lpDrawItemStruct->itemState&ODS_SELECTED) {
//        dc.FillSolidRect(rect, RGB(80, 89, 202)); // �˵���ѡ��  // RGB(80, 89, 202)
//    }
//    dc.SetTextColor(RGB(255, 255, 255)); // �����ı���ɫ
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
//        UINT itemID = pMenu->GetMenuItemID(i); // ��ȡ�˵���ID��
//        pMenu->GetMenuString(i, *pText, MF_BYPOSITION); // ��ȡ�˵��ı�
//
//        // ModifyMenu�������һ��������ӦDRAWITEMSTRUCT�ṹ���itemData����
//        pMenu->ModifyMenu(i, MF_OWNERDRAW | MF_BYPOSITION | MF_STRING, itemID, pText->GetBuffer(255));
//        pText->ReleaseBuffer();
//        free(pText);
//        if (itemID == -1) { // �����һ������ʽ�˵�
//            ChangeMenuItem(pMenu->GetSubMenu(i));
//        }
//    }
//}

#include <atlimage.h>

#include "Control.h"

// #include "pch.h"

//#include <wingdi.h> // for CreateFont
// FIXME #include <afxwin.h> // for CWnd CStatic
//#include <wx-3.0/wx/wx.h>

namespace utils {
    Image Image::fromPath(std::wstring const& imagePath) {
        return Image();
    }

    Image Image::formResourceId(int resourceId) {
        return Image();
    }

    Font Font::of(std::wstring const& familyName) {
        return Font();
    }

    void Control::drawBackground(int resId, std::wstring const& filename) {
        Image image;
        if (resId) {
            image = Image::formResourceId(resId);
        } else if (!filename.empty()) {
            image = Image::fromPath(filename);
        }
        DisplayUtil::drawImage(image, mapToScreen(GetPosition()), GetSize(), ImageDrawMode(ImageDrawMode::AspectRatio::Keep, ImageDrawMode::Scale::NonScale, ImageDrawMode::Filling::NonFull, ImageDrawMode::Tiled::NonTiled));
        //CImage img;
        //if (resId) {
        //    HINSTANCE hInst = AfxGetResourceHandle();
        //    img.LoadFromResource(hInst, resId);
        //} else if (!filename.empty()) {
        //    img.Load(filename.c_str());
        //}
        //HDC hdc = parent_->GetHDC();
        //if (hdc != nullptr) {
        //    img.StretchBlt(hdc, bounds_);
        //}
    }

    void Control::drawText() {
        if (!text_.empty()) {
            if (fontName_.empty()) {
                // parent_;
            } else {
                // self;
            }
            DisplayUtil::drawText(text_, mapToScreen(GetPosition()), GetSize(), Font::of(fontName_), TextLayout());
            //HFONT font = parent_->GetFont(static_cast<int>(text_.length()), isBold_, fontName_);
            //HGDIOBJ oldFont = SelectObject(parent_->GetHDC(), font);
            //int oldBackMode = ::SetBkMode(parent_->GetHDC(), TRANSPARENT);
            //SetTextColor(parent_->GetHDC(), textColor_);
            //CRect textBounds = bounds_;
            //textBounds.MoveToXY(textX_, textY_);
            //DrawText(parent_->GetHDC(), text_.c_str(), static_cast<int>(text_.length()), &textBounds, textStyle_);
            //::SetBkMode(parent_->GetHDC(), oldBackMode);
            //SelectObject(parent_->GetHDC(), oldFont);
        }
    }

    void DisplayUtil::drawImage(Image const& image, Point const& position, Size const& size, ImageDrawMode layout) {
        //
    }

    void DisplayUtil::drawText(std::wstring const& text, Point const& position, Size const& size, Font const& font, TextLayout layout) {
        //
    }

    Control* Container::FindControl(Point point) {
        Control* result = nullptr;
        for (auto c : children_) {
            if (!c->IsHide()) {
                Rect b = c->GetBounds();
                if (b.In(point)) {
                    result = c;
                    break;
                }
            }
        }
        return result;
    }

    void Container::RefreshItems() {
        /*
        CDC *dc = GetDC();

        GetClientRect(&bounds_);

        CDC memDC;
        memDC.CreateCompatibleDC(dc); // memory DC
        CBitmap memBitmap; // memory bitmap
        memBitmap.CreateCompatibleBitmap(dc, bounds_.right - bounds_.left + 1, bounds_.bottom - bounds_.top + 1);
        CBitmap* pOldBmp = memDC.SelectObject(&memBitmap);

        memDC.SetStretchBltMode(COLORONCOLOR);
        dc->SetStretchBltMode(COLORONCOLOR);

        if (backgroundImageId_) {
            CImage img;
            HINSTANCE hInst = AfxGetResourceHandle();
            img.LoadFromResource(hInst, backgroundImageId_);
            img.StretchBlt(memDC.m_hDC, bounds_);
        } else if (!backgroundImageFilename_.empty()) {
            if (hdc_) {
                BitBlt(memDC.m_hDC, bounds_.left, bounds_.top, bounds_.Width(), bounds_.Height(), hdc_, 0, 0, SRCCOPY);
            } else {
                CImage img;
                img.Load(backgroundImageFilename_.c_str());
                img.StretchBlt(memDC.m_hDC, bounds_);
            }
        }

        for (auto c : children_) {
            c->Draw();
        }

        dc->BitBlt(bounds_.left, bounds_.top, bounds_.right, bounds_.bottom, &memDC, 0, 0, SRCCOPY); // reality

        memDC.SelectObject(pOldBmp);
        memDC.DeleteDC();
        memBitmap.DeleteObject();
        ReleaseDC(dc);
        */
    }

    void Container::ShowItem(Control* item) {
        /*
        CDC *dc = GetDC();
        dc->SetStretchBltMode(COLORONCOLOR);
        item->Show();
        ReleaseDC(dc);
        */
    }

    void Container::HideItem(Control* item) {
        /*
        InvalidateRect(&item->GetBounds());

        CDC *dc = GetDC();
        CBitmap memBitmap; // memory bitmap
        CBitmap* pOldBmp = nullptr;

        item->Hide(); // FIXME do all thing??
        CRect rt_ = item->GetBounds();

        CRect rect = rt_;
        rect.MoveToXY(0, 0);

        CDC memDC;
        memDC.CreateCompatibleDC(dc); // memory DC
        memBitmap.CreateCompatibleBitmap(dc, rect.right - rect.left + 1, rect.bottom - rect.top + 1);
        pOldBmp = memDC.SelectObject(&memBitmap);

        //::SetStretchBltMode(memDC.m_hDC,HALFTONE);  
        //::SetBrushOrgEx(memDC.m_hDC,0,0,NULL);
        memDC.SetStretchBltMode(COLORONCOLOR);
        dc->SetStretchBltMode(COLORONCOLOR);

        dc->BitBlt(rt_.left, rt_.top, rt_.right - rt_.left + 1, rt_.bottom - rt_.top + 1, &memDC, rect.left, rect.top, SRCCOPY); // reality

        memDC.SelectObject(pOldBmp);
        memDC.DeleteDC();
        memBitmap.DeleteObject();
        ReleaseDC(dc);
        */
    }

    //BEGIN_MESSAGE_MAP(Form, CStatic)
    //    //{{AFX_MSG_MAP(Form)
    //    ON_WM_SHOWWINDOW()
    //    ON_WM_PAINT()
    //    ON_WM_LBUTTONUP()
    //    ON_WM_LBUTTONDOWN()
    //    ON_WM_MOUSEMOVE()
    //    ON_WM_LBUTTONDBLCLK()
    //    //}}AFX_MSG_MAP
    //END_MESSAGE_MAP()

    //namespace {
    //    int const WM_BUTTON_CLICK = 3000;
    //    int const WM_BUTTON_CLICKBOTTOM = 3001;
    //}

    //void Form::OnShowWindow(BOOL isShow, UINT status) {
    //    if (isShow) {
    //        if (onBeforeShow_) {
				//auto c = Context(GetParent(), this, currentChild_);
    //            onBeforeShow_(c);
    //        }
    //    }
    //    CStatic::OnShowWindow(isShow, status);
    //}

    //void Form::OnPaint() {
    //    CPaintDC dc(this); // device context for painting
    //    RefreshItems();
    //}

   // void Form::OnLButtonDown(UINT nFlags, CPoint point) {
   //     if (onAction_) {
   //         onAction_(point.x, point.y);
   //     }
   //     currentChild_ = FindControl(point);
   //     if (currentChild_) {
			//auto c = Context(GetParent(), this, currentChild_);
   //         currentChild_->OnLButtonDown(c);
   //     }
   //     CStatic::OnLButtonDown(nFlags, point);
   // }

    //void Form::OnLButtonUp(UINT nFlags, CPoint point) {
    //    static long preTimer = 0;
    //    long nowTimer = GetTickCount(); // get start time (ms)
    //    if (nowTimer - preTimer > 400) {
    //        currentChild_ = FindControl(point);
    //        if (currentChild_) {
				//auto c = Context(GetParent(), this, currentChild_);
    //            currentChild_->OnLButtonUp(c);
    //        }
    //        if (onActionFinally_) {
    //            onActionFinally_(point.x, point.y);
    //        }
    //    }
    //    preTimer = nowTimer;
    //    CStatic::OnLButtonUp(nFlags, point);
    //}

    //void Form::OnMouseMove(UINT nFlags, CPoint point) {
    //    static CPoint prevPoint = CPoint(0, 0);
    //    static Control* prevChild = nullptr;
    //    if (abs(point.x - prevPoint.x) > 5 || abs(point.y - prevPoint.y) > 5) {
    //        currentChild_ = FindControl(point);
    //        if (currentChild_) {
    //            if (currentChild_ != prevChild) {
    //                //if (prevChild) {
    //                //    prevChild->SetState(sNormal);
    //                //}
    //                //currentChild_->SetState(sPressed);
    //                prevChild = currentChild_;
    //            }
    //        } else {
    //            if (prevChild) {
    //                //prevChild->SetState(sNormal);
    //                prevChild = nullptr;
    //            }
    //        }
    //        prevPoint = point;
    //    }
    //    CStatic::OnMouseMove(nFlags, point);
    //}

    //void Form::OnLButtonDblClk(UINT nFlags, CPoint point) {
    //    CStatic::OnLButtonDblClk(nFlags, point);
    //}
}

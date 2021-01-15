#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <WinUser.h> // for CreateWindow

// DisplayUtil: drawImage(image, position, size, layout), drawText(text, position, size, font, layout)
// Control: size, draw[color, image, font(text layout), DC], click[select], input[keyboard], cursorOver[mouse, point, touch], hide, show, disable, enable, resize
// Container: children(position) findControl, layout(absolute, border, flow, flex, list, grid, tile, tab, frame)
// Scrollable: viewPort scrollTo(position)
// Tree: shrink expand
// RichEditor, Editor
// DateTime Color File Progress Slider Check Select Menu ContextMenu Image Flash Video Screen Audio Speaker Recorder Recognizer TouchPanel Gesture Camera
// Presenter + Data(Multimedia) Codec
// WebView
// ActiveX Combo DateTime Edit IPAddress

namespace utils {
    enum UnitStatus { sNormal, sFocus, sPressed };

    enum Event {
        eLButtonDown,
        eLButtonUp,
        eMouseMove
    };

    class Control;
    class Container;

    struct Context {
        Context(HWND aOwner, Container* aParent, Control* aControl) : owner(aOwner), parent(aParent), control(aControl) {}
        HWND owner;
        Container* parent;
        Control* control;
    };

    using Handler = std::function<void(Context& context)>;

    class Image {
    public:
        static Image fromPath(std::wstring const& imagePath);
        static Image formResourceId(int resourceId);
    };

    class Point {
    public:
        static Point move(Point self, int dx, int dy) {
            return self.move(dx, dy);
        }

        Point(int x, int y): x_(x), y_(y) {}
        int x() const {
            return x_;
        }
        int y() const {
            return y_;
        }
        Point move(int dx, int dy) const {
            return Point(x_ + dx, y_ + dy);
        }
    private:
        int x_, y_;
    };

    class Size {
    public:
        Size(int width, int height) : width_(width), height_(height) {}
        int const& width() const {
            return width_;
        }
        int const& height() const {
            return height_;
        }
    private:
        int width_, height_;
    };

    class Rect {
    public:
        Rect(Point position, Size size) : position_(position), size_(size) {}
        bool In(Point point) {
            return point.x() > position_.x() && point.x() < position_.x() + size_.width()
                && point.y() > position_.y() && point.y() < position_.y() + size_.height();
        }
        Point const& position() const {
            return position_;
        }
        Size const& size() const {
            return size_;
        }
    private:
        Point position_;
        Size size_;
    };

    class ImageDrawMode {
    public:
        enum class AspectRatio : char {
            Keep,
            NonKeep
        };

        enum class Scale : char {
            NonScale,
            Scale
        };

        enum class Filling : char {
            Full,
            FullX,
            FullY,
            NonFull
        };

        enum class Tiled : char {
            NonTiled,
            Default,
            Center,
            TiledX,
            TiledY,
            TiledXY
        };
    public:
        ImageDrawMode(AspectRatio aspectRatio, Scale scale, Filling filling, Tiled tiled) : aspectRatio_(aspectRatio), scale_(scale), filling_(filling), tiled_(tiled) {}
    private:
        AspectRatio aspectRatio_;
        Scale scale_;
        Filling filling_;
        Tiled tiled_;
    };

    class Font {
    public:
        static Font of(std::wstring const& familyName);
    };

    class TextLayout {
        //
    };

    namespace DisplayUtil {
        void drawImage(Image const& image, Point const& position, Size const& size, ImageDrawMode layout);
        void drawText(std::wstring const& text, Point const& position, Size const& size, Font const& font, TextLayout layout);
    }

    class Control {
    public:
        Control() = default;
        Control(int x, int y) : position_(0, 0), size_(x, y) {};

        virtual ~Control() {}

        virtual void Draw() {
            if (!isHide_) {
                drawBackground(backgroundImageId_, backgroundImageFilename_);
                drawText();
            }
        }

        void RegisterEventHandler(Event event, Handler&& handler) {
            eventHandlers_.insert(std::make_pair(event, handler));
        }

        void OnLButtonDown(Context& context) {
            fireEvent(eLButtonDown, context);
        }

        void OnLButtonUp(Context& context) {
            fireEvent(eLButtonUp, context);
        }

        void OnMouseMove(Context& context) {
            fireEvent(eMouseMove, context);
        }

        Point GetPosition() {
            return Point(position_.x(), position_.y());
        }

        Size const& GetSize() {
            // return Size(bounds_.right - bounds_.left, bounds_.bottom - bounds_.top);
            return size_;
        }

        Rect GetBounds() {
            return Rect(position_, size_);
        }

        bool IsHide() {
            return isHide_;
        }

        void Hide() {
            isHide_ = true;
            // FIXME parent_->Draw();
        }

        void Show() {
            isHide_ = false;
            Draw();
        }

        void SetParent(Container* parent) {
            parent_ = parent;
        }

        void SetBounds(Rect const& bounds) {
            // bounds_ = bounds;
            position_ = bounds.position();
            size_ = bounds.size();
        }

        void SetText(std::wstring const& text, int x, int y, int style, int size, bool isBold, COLORREF color, std::wstring const& fontName) {
            text_ = text;
            textX_ = x;
            textY_ = y;
            textStyle_ = style;
            textSize_ = size;
            isBold_ = isBold;
            textColor_ = color;
            fontName_ = fontName;
        }

        void SetBackground(int backgroundId) {
            backgroundImageId_ = backgroundId;
        }

        void SetBackground(std::wstring const& background) {
            backgroundImageFilename_ = background;
        }

        void* Info() {
            return info_.data();
        }

        template<typename T>
        T const& Info() {
            return *reinterpret_cast<T*>(info_.data());
        }

        template<typename T>
        void SetInfo(T const& v) {
            info_.resize(sizeof v);
            memcpy(info_.data(), &v, sizeof v);
        }

        Point mapToScreen(Point selfPosition) {
            if (parent_) {
                Point parentPosition = { 0, 0 }; // FIXME parent_->mapToScreen(parent_->GetPosition());
                return selfPosition.move(parentPosition.x(), parentPosition.y());
            } else {
                return selfPosition;
            }
        }
    protected:
        void fireEvent(Event event, Context& context) {
            auto it = eventHandlers_.find(event);
            if (it != eventHandlers_.end()) {
                (it->second)(context);
            }
        }
        void drawBackground(int resId, std::wstring const& filename);
        void drawText();

        Container* parent_ = nullptr;
        int ID_{ 0 };
        bool isHide_ = false;

        // CRect bounds_; // regions
        mutable Point position_;
        Size size_{ 0, 0 };
        std::map<Event, Handler> eventHandlers_;

        std::wstring text_;
        int textX_ = 0;
        int textY_ = 0;
        int textStyle_ = DT_CENTER | DT_SINGLELINE;
        int textSize_ = 24;
        bool isBold_ = false;
        COLORREF textColor_ = RGB(0, 0, 0);
        std::wstring fontName_;

        std::wstring backgroundImageFilename_;
        int backgroundImageId_ = 0;

        std::vector<unsigned char> info_;
    };

    class Static : public Control {
    public:
        Static() = default;
        ~Static() = default;
    };

    class Button : public Control {
    public:
        Button() = default;
        ~Button() = default;

        void Draw() final {
            switch (state_) {
            case sNormal:
                drawBackground(backgroundImageId_, backgroundImageFilename_);
                break;
            case sFocus:
                drawBackground(focusImageId_, focusImageFilename_);
                break;
            case sPressed:
                drawBackground(pressedImageId_, pressedImageFilename_);
                break;
            }
            drawText();
        }

        UnitStatus GetState() {
            return state_;
        }

        void SetState(UnitStatus state) {
            if (state_ != state) {
                state_ = state;
                Draw();
            }
        }
    private:
        UnitStatus state_ = sNormal;
        std::wstring focusImageFilename_;
        int focusImageId_ = 0;
        std::wstring pressedImageFilename_;
        int pressedImageId_ = 0;
    };

    class ToggleButton : public Button {
    public:
        ToggleButton() = default;
        ~ToggleButton() = default;
        // TODO find same group pressed button, reset it
    private:
        int toggleGroupId_ = 0;
    };

    class Container : public Control {
    public:
        Container() : Control(0, 0) {}

        void AddControl(Control* child, Rect const& bounds) {
            child->SetParent(this);
            child->SetBounds(bounds);
            children_.push_back(child);
        }

        void SetActionHandler(std::function<LRESULT(WPARAM wParam, LPARAM IParam)>&& onAction) {
            onAction_ = onAction;
        }

        void SetActionFinallyHandler(std::function<LRESULT(WPARAM wParam, LPARAM IParam)>&& onActionFinally) {
            onActionFinally_ = onActionFinally;
        }

        void SetOnShowHandler(std::function<LRESULT(Context& context)>&& onBeforeShow) {
            onBeforeShow_ = onBeforeShow;
        }

        Control* FindControl(Point point);
        // refresh views
        void RefreshItems();

        void ShowItem(Control* item);

        void HideItem(Control* item);
    protected:
        std::vector<Control*> children_;
        std::function<LRESULT(WPARAM wParam, LPARAM IParam)> onAction_ = nullptr;
        std::function<LRESULT(WPARAM wParam, LPARAM IParam)> onActionFinally_ = nullptr;
        std::function<LRESULT(Context& context)> onBeforeShow_ = nullptr;
    };

    class Scrollable : public Control {
    public:
        Scrollable(Control* pointee) : Control(0, 0), pointee_(pointee), viewPortPosition_(0, 0), viewPortSize_(pointee->GetSize()) {}
    private:
        Control* pointee_;
        Point viewPortPosition_;
        Size viewPortSize_;
    };

    class Form : public Container {
    public:
        Form() : Container() {}

        ~Form() {
            for (auto& f : fonts_) {
                ReleaseFont(f.second);
            }
            for (auto c : children_) {
                delete c;
            }
        }

        void Create(Rect const& bounds, HWND parent, int ID) {
            handle_ = parent;
            ID_ = ID;
            CreateWindowEx(0/*extra-style*/, L"className", L"windowName",
                WS_CHILD | WS_VISIBLE | SS_NOTIFY | WS_EX_TRANSPARENT,
                bounds.position().x(), bounds.position().y(), bounds.size().width(), bounds.size().height(),
                parent, nullptr/*menu*/, nullptr/*instance*/, this);
        }

        HWND GetHandler() {
            return handle_;
        }

        HDC GetHDC() {
            return hdc_;
        }

        HFONT GetFont(int height, bool isBold, std::wstring const& fontName) {
            HFONT result;
            auto it = fonts_.find(fontName);
            if (it == fonts_.end()) {
                result = CreateFont(height, 0, 0, 0, isBold, FALSE, FALSE,
                    FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS,
                    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                    FIXED_PITCH | FF_MODERN, fontName.c_str());
            } else {
                result = it->second;
            }
            return result;
        }

        void ReleaseFont(HFONT font) {
            DeleteObject(font);
        }
    protected:
        /*
        //{{AFX_MSG(Form)
        afx_msg void OnShowWindow(BOOL isShow, UINT status);
        afx_msg void OnPaint();
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg	void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
        */
    private:
        HWND handle_ = nullptr;
        HDC hdc_ = nullptr;
        HBITMAP hBitmap_ = nullptr;
        HBITMAP hOldBitmap_ = nullptr;
        Control* currentChild_ = nullptr;
        std::map<std::wstring, HFONT> fonts_;
    };
}

#pragma once
#include "core/gui_type.h"

namespace ui
{
    class INativeScreen
    {
    public:
        virtual NativeRect GetBounds() = 0;
        virtual NativeRect GetClientBounds() = 0;
        virtual std::wstring GetName() = 0;
        virtual bool IsPrimary() = 0;
        virtual double GetScalingX() = 0;
        virtual double GetScalingY() = 0;
    };

    class INativeCursor
    {
    public:
        enum SystemCursorType
        {
            SmallWaiting,
            LargeWaiting,
            Arrow,
            Cross,
            Hand,
            Help,
            IBeam,
            SizeAll,
            SizeNESW,
            SizeNS,
            SizeNWSE,
            SizeWE,
            LastSystemCursor = SizeWE,
        };

        static const int SystemCursorCount = LastSystemCursor + 1;

    public:
        virtual bool IsSystemCursor() = 0;
        virtual SystemCursorType GetSystemCursorType() = 0;
    };

    class INativeScreenService
    {
    public:
        virtual int GetScreenCount() = 0;

        virtual INativeScreen* GetScreen(vint index) = 0;

        virtual INativeScreen* GetScreen(INativeWindow* window) = 0;
    };

    class INativeWindowService
    {
    public:
        virtual INativeWindow* CreateNativeWindow() = 0;

        virtual void DestroyNativeWindow(INativeWindow* window) = 0;

        virtual INativeWindow* GetMainWindow() = 0;

        virtual INativeWindow* GetWindow(NativePoint location) = 0;

        virtual void Run(INativeWindow* window) = 0;
    };

    class INativeWindow
    {
    public:
        virtual Point Convert(NativePoint value) = 0;

        virtual NativePoint Convert(Point value) = 0;

        virtual Size Convert(NativeSize value) = 0;

        virtual NativeSize Convert(Size value) = 0;

        virtual Margin Convert(NativeMargin value) = 0;

        virtual NativeMargin Convert(Margin value) = 0;

        virtual NativeRect GetBounds() = 0;

        virtual void SetBounds(const NativeRect& bounds) = 0;

        virtual NativeSize GetClientSize() = 0;

        virtual void SetClientSize(NativeSize size) = 0;

        virtual NativeRect GetClientBoundsInScreen() = 0;

        virtual std::wstring GetTitle() = 0;

        virtual void SetTitle(std::wstring title) = 0;

        virtual INativeCursor* GetWindowCursor() = 0;

        virtual void SetWindowCursor(INativeCursor* cursor) = 0;

        virtual NativePoint GetCaretPoint() = 0;

        virtual void SetCaretPoint(NativePoint point) = 0;

        virtual INativeWindow* GetParent() = 0;

        virtual void SetParent(INativeWindow* parent) = 0;

        virtual bool GetAlwaysPassFocusToParent() = 0;

        virtual void SetAlwaysPassFocusToParent(bool value) = 0;

        virtual void EnableCustomFrameMode() = 0;

        virtual void DisableCustomFrameMode() = 0;

        virtual bool IsCustomFrameModeEnabled() = 0;

        virtual NativeMargin GetCustomFramePadding() = 0;

        enum class WindowSizeState
        {

            Minimized,

            Restored,

            Maximized,
        };

        virtual std::unique_ptr<GuiImageData> GetIcon() = 0;

        virtual void SetIcon(std::unique_ptr<GuiImageData> icon) = 0;

        virtual WindowSizeState GetSizeState() = 0;

        virtual void Show() = 0;

        virtual void ShowDeactivated() = 0;

        virtual void ShowRestored() = 0;

        virtual void ShowMaximized() = 0;

        virtual void ShowMinimized() = 0;

        virtual void Hide(bool closeWindow) = 0;

        virtual bool IsVisible() = 0;

        virtual void Enable() = 0;

        virtual void Disable() = 0;

        virtual bool IsEnabled() = 0;

        virtual void SetFocus() = 0;

        virtual bool IsFocused() = 0;

        virtual void SetActivate() = 0;

        virtual bool IsActivated() = 0;

        virtual void ShowInTaskBar() = 0;

        virtual void HideInTaskBar() = 0;

        virtual bool IsAppearedInTaskBar() = 0;

        virtual void EnableActivate() = 0;

        virtual void DisableActivate() = 0;

        virtual bool IsEnabledActivate() = 0;

        virtual bool RequireCapture() = 0;

        virtual bool ReleaseCapture() = 0;

        virtual bool IsCapturing() = 0;

        virtual bool GetMaximizedBox() = 0;

        virtual void SetMaximizedBox(bool visible) = 0;

        virtual bool GetMinimizedBox() = 0;

        virtual void SetMinimizedBox(bool visible) = 0;

        virtual bool GetBorder() = 0;

        virtual void SetBorder(bool visible) = 0;

        virtual bool GetSizeBox() = 0;

        virtual void SetSizeBox(bool visible) = 0;

        virtual bool GetIconVisible() = 0;

        virtual void SetIconVisible(bool visible) = 0;

        virtual bool GetTitleBar() = 0;

        virtual void SetTitleBar(bool visible) = 0;

        virtual bool GetTopMost() = 0;

        virtual void SetTopMost(bool topmost) = 0;

        /// Supress the system's Alt+X hot key
        virtual void SupressAlt() = 0;

        virtual bool InstallListener(INativeWindowListener* listener) = 0;

        virtual bool UninstallListener(INativeWindowListener* listener) = 0;

        virtual void RedrawContent() = 0;
    };

    template <typename T>
    struct WindowMouseInfo_
    {
        bool ctrl;
        bool shift;
        bool left;
        bool middle;
        bool right;
        T x;
        T y;
        int wheel;
        bool nonClient;
    };

    using WindowMouseInfo = WindowMouseInfo_<GuiCoordinate>;
    using NativeWindowMouseInfo = WindowMouseInfo_<NativeCoordinate>;


    struct NativeWindowKeyInfo
    {
        VKEY code;
        bool ctrl;
        bool shift;
        bool alt;
        bool capslock;
        bool autoRepeatKeyDown;
    };

    using WindowKeyInfo = NativeWindowKeyInfo;

    struct NativeWindowCharInfo
    {
        wchar_t code;
        bool ctrl;
        bool shift;
        bool alt;
        bool capslock;
    };

    using WindowCharInfo = NativeWindowCharInfo;

    class INativeWindowListener
    {
    public:
        enum class HitTestResult
        {
            BorderNoSizing,
            BorderLeft,
            BorderRight,
            BorderTop,
            BorderBottom,
            BorderLeftTop,
            BorderRightTop,
            BorderLeftBottom,
            BorderRightBottom,
            Title,
            ButtonMinimum,
            ButtonMaximum,
            ButtonClose,
            Client,
            Icon,
            NoDecision,
        };

        virtual HitTestResult HitTest(NativePoint location);

        virtual void Moving(NativeRect& bounds, bool fixSizeOnly);

        virtual void Moved();

        virtual void DpiChanged();

        virtual void Enabled();

        virtual void Disabled();

        virtual void GotFocus();

        virtual void LostFocus();

        virtual void Activated();

        virtual void Deactivated();

        virtual void Opened();

        virtual void Closing(bool& cancel);

        virtual void Closed();

        virtual void Paint();

        virtual void Destroying();

        virtual void Destroyed();

        virtual void LeftButtonDown(const NativeWindowMouseInfo& info);

        virtual void LeftButtonUp(const NativeWindowMouseInfo& info);

        virtual void LeftButtonDoubleClick(const NativeWindowMouseInfo& info);

        virtual void RightButtonDown(const NativeWindowMouseInfo& info);

        virtual void RightButtonUp(const NativeWindowMouseInfo& info);

        virtual void RightButtonDoubleClick(const NativeWindowMouseInfo& info);

        virtual void MiddleButtonDown(const NativeWindowMouseInfo& info);

        virtual void MiddleButtonUp(const NativeWindowMouseInfo& info);

        virtual void MiddleButtonDoubleClick(const NativeWindowMouseInfo& info);

        virtual void HorizontalWheel(const NativeWindowMouseInfo& info);

        virtual void VerticalWheel(const NativeWindowMouseInfo& info);

        virtual void MouseMoving(const NativeWindowMouseInfo& info);

        virtual void MouseEntered();

        virtual void MouseLeaved();

        virtual void KeyDown(const NativeWindowKeyInfo& info);

        virtual void KeyUp(const NativeWindowKeyInfo& info);

        virtual void SysKeyDown(const NativeWindowKeyInfo& info);

        virtual void SysKeyUp(const NativeWindowKeyInfo& info);

        virtual void Char(const NativeWindowCharInfo& info);
    };

}  // namespace ui
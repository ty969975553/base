#pragma once
#include "core/gui_abstract.h"
#include "precompile_header.h"

namespace ui
{
    class WindowsScreen : public INativeScreen
    {
        friend class WindowsScreenService;

    protected:
        HMONITOR monitor;

    public:
        WindowsScreen();

        NativeRect GetBounds() override;
        NativeRect GetClientBounds() override;
        std::wstring GetName() override;
        bool IsPrimary() override;
        double GetScalingX() override;
        double GetScalingY() override;
    };

    class WindowsScreenService : public INativeScreenService
    {
        typedef HWND (*HandleRetriver)(INativeWindow*);

    protected:
        std::vector<std::unique_ptr<WindowsScreen>> screens;
        HandleRetriver handleRetriver;

    public:
        struct MonitorEnumProcData
        {
            WindowsScreenService* screenService;
            int currentScreen;
        };

        WindowsScreenService(HandleRetriver _handleRetriver);

        static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor,
                                             LPRECT lprcMonitor, LPARAM dwData);
        void RefreshScreenInformation();
        int GetScreenCount() override;
        INativeScreen* GetScreen(vint index) override;
        INativeScreen* GetScreen(INativeWindow* window) override;
    };
}
}

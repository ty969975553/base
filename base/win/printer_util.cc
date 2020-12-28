#include "printer_util.h"

#include <time.h>
#include <algorithm>
#define IDD_PAGESETUP 12

namespace base
{
    PrintManager* PrintManager::GetInstance()
    {
        static PrintManager manager;
        return &manager;
    }

    UINT_PTR PrintManager::PageSetupHookProc(HWND hWnd, UINT Message,
                                             WPARAM wParam, LPARAM lParam)
    {
        switch (Message)
        {
            case WM_INITDIALOG:
                return TRUE;

            case WM_DESTROY:
                break;

            case WM_CONTEXTMENU:
                return TRUE;
        }

        return FALSE;
    }

    void PrintManager::ShowPrintSetupDlg() {}

    void PrintManager::PreparePrinter()
    {
        hWaitCursor = LoadCursor(NULL, IDC_WAIT);
    }

    void PrintManager::PrintGivenDC(HDC hPrintDC)
    {
        HANDLE hText = NULL;      // handle to MLE text
        HFONT hPrintFont = NULL;  // font to print with
        HANDLE hPrevFont = NULL;  // previous font in hPrintDC

        BOOL fPageStarted = FALSE;  // true if StartPage called for this page
        BOOL fDocStarted = FALSE;   // true if StartDoc called
        PTCHAR pStartText = NULL;   // start of edit text (locked hText)
        TEXTMETRIC Metrics;
        std::wstring msgbuf;
        //int nLinesPerPage;  // not inc. header and footer

        int iErr = 0;  // error return
        //DOCINFO DocInfo;
        LOGFONT lfPrintFont;  // local version of FontStruct
        LCID lcid;            // locale id

        SetCursor(static_cast<HCURSOR>(hWaitCursor));
        GetResolutions(hPrintDC);
        GetLocalTime(&PrintTime);  // use local, not gmt

        lcid = GetUserDefaultLCID();

        GetDateFormat(lcid, DATE_LONGDATE, &PrintTime, NULL, szFormattedDate,
                      260);

        GetTimeFormat(lcid, 0, &PrintTime, NULL, szFormattedTime, 260);

        //lfPrintFont = FontStruct;  // make local copy
        //lfPrintFont.lfHeight = -(iPointSize * yPixInch) / (72 * 10);
        //lfPrintFont.lfWidth = 0;
        SetMapMode(hPrintDC, MM_ANISOTROPIC);

        SetViewportExtEx(hPrintDC, xPhysRes, yPhysRes, NULL);

        SetWindowExtEx(hPrintDC, pageSetupDlg.ptPaperSize.x,
                       pageSetupDlg.ptPaperSize.y, NULL);
        
        rtMargin = pageSetupDlg.rtMargin;

        LPtoDP(hPrintDC, (LPPOINT)&rtMargin, 2);

        SetMapMode(hPrintDC, MM_TEXT);  // restore to mm_text mode

        hPrintFont = CreateFontIndirect(&lfPrintFont);

        hPrevFont = SelectObject(hPrintDC, hPrintFont);

        SetBkMode(hPrintDC, TRANSPARENT);

        if (!GetTextMetrics(hPrintDC, (LPTEXTMETRIC)&Metrics))
        {
            //goto ErrorExit;
        }

        if (!(Metrics.tmPitchAndFamily & (TMPF_VECTOR | TMPF_TRUETYPE)))
        {
            // remove just created font

            hPrintFont = static_cast<HFONT>(SelectObject(hPrintDC, hPrevFont));  // get old font
            DeleteObject(hPrintFont);

            memset(lfPrintFont.lfFaceName, 0, LF_FACESIZE * sizeof(TCHAR));

            hPrintFont = CreateFontIndirect(&lfPrintFont);
            if (!hPrintFont)
            {
                //goto ErrorExit;
            }

            hPrevFont = SelectObject(hPrintDC, hPrintFont);
            if (!hPrevFont)
            {
                //goto ErrorExit;
            }

            if (!GetTextMetrics(hPrintDC, (LPTEXTMETRIC)&Metrics))
            {
                //goto ErrorExit;
            }
        }
        yPrintChar =
            Metrics.tmHeight + Metrics.tmExternalLeading; /* the height */

        tabSize =
            Metrics.tmAveCharWidth * 8; /* 8 ave char width pixels for tabs */

        // compute margins in pixels

        dxLeft = std::max((int)rtMargin.left - xPhysOff, 0);
        dxRight = std::max(
            (int)rtMargin.right - (xPhysRes - xPrintRes - xPhysOff), 0);
        dyTop = std::max((int)rtMargin.top - yPhysOff, 0);
        dyBottom = std::max(
            (int)rtMargin.bottom - (yPhysRes - yPrintRes - yPhysOff), 0);

    }

    int PrintManager::PrintHeaderFooter(HDC hDC, int nHF)
    {
        SIZE Size;  // to compute the width of each string
        int yPos;   // y position to print
        int xPos;   // x position to print

        if (*chPageText[nHF] == 0)  // see if anything to do
            return 0;               // we didn't print

        // TranslateString(chPageText[nHF]);

        // figure out the y position we are printing

        if (nHF == static_cast<int>(PrintPos::HEADER))
            yPos = dyTop;
        else
            yPos = yPrintRes - dyBottom - yPrintChar;

        // print out the various strings
        // N.B. could overprint which seems ok for now

        if (*chBuff[LEFT])  // left string
        {
            TextOut(hDC, dxLeft, yPos, chBuff[LEFT], lstrlen(chBuff[LEFT]));
        }

        if (*chBuff[CENTER])  // center string
        {
            GetTextExtentPoint32(hDC, chBuff[CENTER], lstrlen(chBuff[CENTER]),
                                 &Size);
            xPos = (xPrintRes - dxRight + dxLeft) / 2 - Size.cx / 2;
            TextOut(hDC, xPos, yPos, chBuff[CENTER], lstrlen(chBuff[CENTER]));
        }

        if (*chBuff[RIGHT])  // right string
        {
            GetTextExtentPoint32(hDC, chBuff[RIGHT], lstrlen(chBuff[RIGHT]),
                                 &Size);
            xPos = xPrintRes - dxRight - Size.cx;
            TextOut(hDC, xPos, yPos, chBuff[RIGHT], lstrlen(chBuff[RIGHT]));
        }
        return 1;  // we did print something
    }

    void PrintManager::GetResolutions(HDC hPrintDC)
    {
        xPrintRes = GetDeviceCaps(hPrintDC, HORZRES);
        yPrintRes = GetDeviceCaps(hPrintDC, VERTRES);
        yPixInch = GetDeviceCaps(hPrintDC, LOGPIXELSY);

        xPhysRes = GetDeviceCaps(hPrintDC, PHYSICALWIDTH);
        yPhysRes = GetDeviceCaps(hPrintDC, PHYSICALHEIGHT);

        xPhysOff = GetDeviceCaps(hPrintDC, PHYSICALOFFSETX);
        yPhysOff = GetDeviceCaps(hPrintDC, PHYSICALOFFSETY);
    }

    void PrintManager::SetPageSetupDefault()
    {
        TCHAR szIMeasure[2];

        pageSetupDlg.lpfnPageSetupHook = PageSetupHookProc;
        pageSetupDlg.lpPageSetupTemplateName = MAKEINTRESOURCE(IDD_PAGESETUP);

        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IMEASURE, szIMeasure, 2);

        pageSetupDlg.Flags =
            PSD_MARGINS | PSD_ENABLEPAGESETUPHOOK | PSD_ENABLEPAGESETUPTEMPLATE;

        if (szIMeasure[0] == TEXT('1'))
        {
            //  English measure (in thousandths of inches).
            pageSetupDlg.Flags |= PSD_INTHOUSANDTHSOFINCHES;
            pageSetupDlg.rtMargin.top = 1000;
            pageSetupDlg.rtMargin.bottom = 1000;
            pageSetupDlg.rtMargin.left = 750;
            pageSetupDlg.rtMargin.right = 750;
        }
        else
        {
            //  Metric measure (in hundreths of millimeters).
            pageSetupDlg.Flags |= PSD_INHUNDREDTHSOFMILLIMETERS;
            pageSetupDlg.rtMargin.top = 2500;
            pageSetupDlg.rtMargin.bottom = 2500;
            pageSetupDlg.rtMargin.left = 2000;
            pageSetupDlg.rtMargin.right = 2000;
        }
    }

    HDC PrintManager::GetPrinterDC(HWND hwndOwner)
    {
        LPDEVMODE lpDevMode;
        LPDEVNAMES lpDevNames;
        HDC hDC;

        if (!pageSetupDlg
                 .hDevNames) /* Retrieve default printer if none selected. */
        {
            pageSetupDlg.Flags |= PSD_RETURNDEFAULT;
            PageSetupDlg(&pageSetupDlg);
            pageSetupDlg.Flags &= ~PSD_RETURNDEFAULT;
        }

        if (!pageSetupDlg.hDevNames)
        {
            MessageBox(hwndOwner, L"无法访问打印机", L"提示",
                       MB_OK | MB_ICONWARNING);
            return nullptr;
        }

        lpDevNames = (LPDEVNAMES)GlobalLock(pageSetupDlg.hDevNames);

        lpDevMode = NULL;

        if (pageSetupDlg.hDevMode)
            lpDevMode = (LPDEVMODE)GlobalLock(pageSetupDlg.hDevMode);

        /*  For pre 3.0 Drivers,hDevMode will be null  from Commdlg so lpDevMode
         *  will be NULL after GlobalLock()
         */

        /* The lpszOutput name is null so CreateDC will use the current setting
         * from PrintMan.
         */
        hDC = CreateDC(((LPTSTR)lpDevNames) + lpDevNames->wDriverOffset,
                       ((LPTSTR)lpDevNames) + lpDevNames->wDeviceOffset, NULL,
                       lpDevMode);

        GlobalUnlock(pageSetupDlg.hDevNames);

        if (pageSetupDlg.hDevMode) GlobalUnlock(pageSetupDlg.hDevMode);

        if (hDC == NULL)
        {
            MessageBox(hwndOwner, L"无法访问打印机", L"提示",
                       MB_OK | MB_ICONWARNING);
            return nullptr;
        }

        return hDC;
    }

}  // namespace base
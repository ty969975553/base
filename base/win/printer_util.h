#pragma once

#include <windows.h>

//#define STDMETHOD
#include <commdlg.h>
//#undef STDMETHOD

#include <string>
#include <vector>

namespace base
{
    static const int PT_LEN = 40;
    static const int LEFT = 0;
    static const int CENTER = 1;
    static const int RIGHT = 2;
    enum class PrintPos :  int
    {
        HEADER = 0, FOOTER = 1      
    };

    class PrintManager
    {
    public:
        static PrintManager* GetInstance();
        static UINT_PTR CALLBACK PageSetupHookProc(HWND hWnd, UINT Message,
                                            WPARAM wParam, LPARAM lParam);
        void ShowPrintSetupDlg();
        void PreparePrinter();
        HDC GetPrinterDC(HWND hwndOwner);
        //HDC GetPrinterDCviaDialog(HWND hwndOwner);
        void PrintGivenDC(HDC hPrintDC);
    private:
        HANDLE hWaitCursor;

        RECT rtMargin;

        TCHAR szFormattedDate[256] =
            TEXT("Y");  // formatted date (may be internationalized)
        TCHAR szFormattedTime[256] =
            TEXT("Y");         // formatted time (may be internaltionalized)
        SYSTEMTIME PrintTime;  // time we started printing
        int xPrintRes;         // printer resolution in x direction
        int yPrintRes;         // printer resolution in y direction
        int yPixInch;          // pixels/inch
        int xPhysRes;          // physical resolution x of paper
        int yPhysRes;          // physical resolution y of paper

        int xPhysOff;  // physical offset x
        int yPhysOff;  // physical offset y

        int dyTop;     // width of top border (pixels)
        int dyBottom;  // width of bottom border
        int dxLeft;    // width of left border
        int dxRight;   // width of right border

        int iPageNum;  // global page number currently being printed

        TCHAR chPageText[2][PT_LEN]; /* Strings to hold PageSetup items. */
        TCHAR chPageTextTemp[2][PT_LEN];
        TCHAR szPrinterName[260]; /* String to hold printername for PrintTo verb
                                   */
        PAGESETUPDLG pageSetupDlg;
        INT yPrintChar; /* height of a character          */
        TCHAR chBuff[RIGHT + 1][260];

        INT tabSize; /* Size of a tab for print device in device units*/
    private:
        int PrintHeaderFooter(HDC hDC, int nHF);
        void GetResolutions(HDC hPrintDC);
        void SetPageSetupDefault();

    };
}  // namespace base
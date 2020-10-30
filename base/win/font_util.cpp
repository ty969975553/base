#include "font_util.h"

namespace base {

    // reserve lobyte for charset
#define PRINTER_FONT 0x0100
#define TT_FONT 0x0200
#define DEVICE_FONT 0x0400
#define PS_OPENTYPE_FONT  0x800
#define TT_OPENTYPE_FONT  0x1000
#define TYPE1_FONT  0x2000

    static int nFontSizes[] =
    { 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72 };

    FontDesc::FontDesc(const std::wstring lpszName, const std::wstring lpszScript, BYTE nCharSet,
        BYTE nPitchAndFamily, DWORD dwFlags)
    {
        name = lpszName;
        script = lpszScript;
        charSet = nCharSet;
        pitchAndFamily = nPitchAndFamily;
        flags = dwFlags;
    }

    FontDesc::FontDesc(ENUMLOGFONTEX* pelf, DWORD dwFlags) 
    {
        name = pelf->elfFullName;
        script = pelf->elfScript;
        charSet = pelf->elfLogFont.lfCharSet;
        pitchAndFamily = pelf->elfLogFont.lfPitchAndFamily;
        flags = dwFlags;
    }

	FontManager* FontManager::GetInstance()
	{
		static FontManager manager;
		return &manager;
	}

    void  FontManager::vGetFontType(NEWTEXTMETRICEX* lpntm, int FontType, DWORD* pdwData)
    {
        DWORD  ntmFlags = lpntm->ntmTm.ntmFlags;

        if (ntmFlags & NTM_PS_OPENTYPE)
        {
            *pdwData |= PS_OPENTYPE_FONT;
        }
        else if (ntmFlags & NTM_TYPE1)
        {
            *pdwData |= TYPE1_FONT;
        }
        else
        {
            if (FontType & TRUETYPE_FONTTYPE)
            {
                if (ntmFlags & NTM_TT_OPENTYPE)
                    *pdwData |= TT_OPENTYPE_FONT;
                else
                    *pdwData |= TT_FONT;
            }
            else if (FontType & DEVICE_FONTTYPE)
                *pdwData |= DEVICE_FONT;
        }
    }

    bool FontManager::EnumFamScreenCallBackEx(ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* lpntm, int FontType, LPVOID pThis)
    {
        if (FontType & RASTER_FONTTYPE)
            return 1;

        DWORD dwData = 0;

        vGetFontType(lpntm, FontType, &dwData);
        FontDesc desc(pelf,dwData);

        FontManager::GetInstance()->screen_font_vec.push_back(desc);
        return 1;
    }

    bool FontManager::EnumFamPrinterCallBackEx(ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* lpntm, int FontType, LPVOID pThis)
    {
        DWORD dwData = PRINTER_FONT;

        vGetFontType(lpntm, FontType, &dwData);
        FontDesc desc(pelf, dwData);
        FontManager::GetInstance()->printer_font_vec.push_back(desc);
        return 1;
    }

    void FontManager::EnumFontFamiliesEx(HDC dc, BYTE nCharSet, bool print)
    {
        LOGFONT lf;
        memset(&lf, 0, sizeof(lf));
        lf.lfCharSet = nCharSet;
        if (print) {
            ::EnumFontFamiliesEx(dc, &lf,
                (FONTENUMPROC)EnumFamPrinterCallBackEx, NULL, NULL);
        }
        else {
            ::EnumFontFamiliesEx(dc, &lf,
                (FONTENUMPROC)EnumFamScreenCallBackEx, NULL, NULL);
        }
    }
}


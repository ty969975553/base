#pragma once
#include <windows.h>
#include <vector>
#include <string>

namespace base
{
	class FontDesc
	{
	public:
		FontDesc(const std::wstring lpszName, const std::wstring lpszScript, BYTE nCharSet,
			BYTE nPitchAndFamily, DWORD dwFlags);
		FontDesc(ENUMLOGFONTEX* pelf, DWORD dwFlags);
		std::wstring name;
		std::wstring script;
		BYTE charSet;
		BYTE pitchAndFamily;
		DWORD flags;
	};

	class FontManager {
	public:
		static FontManager* GetInstance();
		static void vGetFontType(NEWTEXTMETRICEX* lpntm, int FontType, DWORD* pdwData);
		static bool CALLBACK  EnumFamScreenCallBackEx(
			ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis);
		static bool CALLBACK  EnumFamPrinterCallBackEx(
			ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis);

	public:
		void EnumFontFamiliesEx(HDC dc, BYTE nCharSet = DEFAULT_CHARSET, bool print = false);

	public:
		std::vector<FontDesc> screen_font_vec;
		std::vector<FontDesc> printer_font_vec;
	};
}
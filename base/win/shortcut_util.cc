#include "base/win/shortcut_util.h"
namespace base
{
    namespace win
    {
        bool CreateShortCut(const std::wstring& target_file,
                            const std::wstring& target_args,
                            const std::wstring& link_file,
                            const std::wstring& description, int show_mode,
                            const std::wstring& cur_dir,
                            const std::wstring& icon_file, int icon_index)
        {
            // if ((target_file == NULL) || (target_file[0] == 0) ||
            //     (link_file == NULL) || (link_file[0] == 0))
            // {
            //     return false;
            // }
            // CComPtr<IShellLink> pShellLink;
            // HRESULT hRes = pShellLink.CoCreateInstance(CLSID_ShellLink, NULL,
            //                                            CLSCTX_INPROC_SERVER);
            // if (FAILED(hRes)) return hRes;

            // hRes = pShellLink->SetPath(pszTargetfile);
            // hRes = pShellLink->SetArguments(pszTargetargs);
            // if (pszDescription[0] != 0)
            // {
            //     hRes = pShellLink->SetDescription(pszDescription);
            // }
            // if (iShowmode > 0)
            // {
            //     hRes = pShellLink->SetShowCmd(iShowmode);
            // }
            // if (pszCurdir[0] != 0)
            // {
            //     hRes = pShellLink->SetWorkingDirectory(pszCurdir);
            // }
            // if (pszIconfile[0] != 0 && iIconindex >= 0)
            // {
            //     hRes = pShellLink->SetIconLocation(pszIconfile, iIconindex);
            // }

            // // Use the IPersistFile object to save the shell link
            // CComPtr<IPersistFile> pPersistFile;
            // hRes = pShellLink.QueryInterface(&pPersistFile);
            // if (SUCCEEDED(hRes))
            // {
            //     hRes = pPersistFile->Save(pszLinkfile, TRUE);
            // }
            // return hRes;
        }

        bool CreateShortCutToURL(const std::wstring& url,
                                 const std::wstring& link_file)
        {
        }
    }  // namespace win
}  // namespace base
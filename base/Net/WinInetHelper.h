#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <list>
#include <WinInet.h>
#pragma comment(lib, "wininet.lib")

using namespace std;
namespace WinInet
{
#pragma region HttpGet
    static  __inline
        bool ParserUrl(string Url, string& Host, string& Filename, WORD& Port)
    {
        Port = INTERNET_DEFAULT_HTTP_PORT;
        if (Url.substr(0U, 7) != "http://")
        {
            return false;
        }
        Url = Url.substr(7);
        int  HostLen = 0;
        for (int i = 0; i < (int)Url.size(); i++)
        {
            if (Url[i] == (':') || Url[i] == ('/'))
            {
                break;
            }
            HostLen++;
        }
        Host = Url.substr(0, HostLen);
        Url = Url.substr(HostLen);
        if (Url[0] == ':') {
            for (int i = 1; i < (int)Url.size(); i++)
            {
                if (Url[i] == ('/'))
                {
                    string strPort = Url.substr(0, i);
                    Port = atoi(strPort.c_str());
                    Url = Url.substr(i);
                    break;
                }
            }
        }
        Filename = Url;
        return true;
    }
    static _inline
        string  HttpGet(string Url, DWORD& httpStatus)
    {

        string              ret;
        DWORD               byteread = 0;
        HINTERNET           m_hInternetSession = NULL;
        HINTERNET           m_hHttpConnection = NULL;
        HINTERNET           m_hHttpFile = NULL;

        char                buffer[1024] = { 0 };
        //
        LPCSTR              ppszAcceptTypes[2];

        DWORD   dwContentLength = 0, dwIndex = 0, filesize = 0xffffffff, totalSize = 0;
        DWORD   dwBuffLen = sizeof(dwContentLength);

        DWORD dwFlags = INTERNET_FLAG_RELOAD;
        DWORD TimeOut = 15000;

        string  m_sServer, m_sObject;
        WORD m_nPort = INTERNET_DEFAULT_HTTP_PORT;
        httpStatus = DWORD(-1);
        if (!ParserUrl(Url, m_sServer, m_sObject, m_nPort))
            return "";
        LPCSTR      headers = ("\r\n\r\n");



        m_hInternetSession = ::InternetOpenA(("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:14.0) Gecko/20100101 Firefox/14.0.1"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);


        InternetSetOption(m_hInternetSession, INTERNET_OPTION_CONNECT_TIMEOUT, &TimeOut, sizeof(DWORD));
        InternetSetOption(m_hInternetSession, INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, &TimeOut, sizeof(DWORD));
        InternetSetOption(m_hInternetSession, INTERNET_OPTION_DATA_SEND_TIMEOUT, &TimeOut, sizeof(DWORD));

        if (m_hInternetSession == NULL)
            goto EXIT;



        m_hHttpConnection = ::InternetConnectA(m_hInternetSession, m_sServer.c_str(), m_nPort, NULL,
            NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (m_hHttpConnection == NULL)
            goto EXIT;
        //Issue the request to read the file

        ppszAcceptTypes[0] = ("text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");  //We support accepting any mime file type since this is a simple download of a file
        ppszAcceptTypes[1] = NULL;


        m_hHttpFile = HttpOpenRequestA(m_hHttpConnection, "GET", m_sObject.c_str(), "HTTP/1.0", NULL, ppszAcceptTypes, dwFlags, 0);
        if (m_hHttpFile == NULL)
            goto EXIT;


        if (!::HttpSendRequestA(m_hHttpFile, headers, strlen(headers), NULL, 0))
            goto EXIT;



        dwBuffLen = sizeof(httpStatus);
        if ((!HttpQueryInfoA(m_hHttpFile, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &httpStatus, &dwBuffLen,
            &dwIndex)) || (httpStatus != HTTP_STATUS_OK))
            goto EXIT;
        dwBuffLen = sizeof(dwContentLength);
        if ((HttpQueryInfoA(m_hHttpFile, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &dwContentLength, &dwBuffLen,
            &dwIndex)))
        {
            filesize = dwContentLength;
            ret.reserve(dwContentLength);
        }
        while (true)
        {
            DWORD byteread = 0;
            if (!InternetReadFile(m_hHttpFile, buffer, sizeof(buffer), &byteread) || (byteread == 0))
                break;
            totalSize += byteread;
            ULONG written = 0;
            ret += string(buffer, byteread);
            //ret->Write(buffer, byteread, &written);
            if (totalSize >= filesize)
                break;

        }

    EXIT:
        if (m_hInternetSession)
            InternetCloseHandle(m_hInternetSession);
        if (m_hHttpConnection)
            InternetCloseHandle(m_hHttpConnection);
        if (m_hHttpFile)
            InternetCloseHandle(m_hHttpFile);
        return  ret;
    }

#pragma endregion HttpGet
}
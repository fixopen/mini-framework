#include <Windows.h>

#include "Settings.h"

//#include <afxwin.h>

namespace utils {
    void Settings::Load() {
		port_ = ::GetPrivateProfileIntW(L"AppSettings", L"Port", 80, fileName_);
		std::size_t const bufferLength = 256;
		wchar_t buffer[bufferLength] = {};
        auto r = ::GetPrivateProfileStringW(L"AppSettings", L"Password", L"", buffer, bufferLength * sizeof(wchar_t), fileName_);
		password_ = buffer;
        r = ::GetPrivateProfileStringW(L"AppSettings", L"UserName", L"admin", buffer, bufferLength * sizeof(wchar_t), fileName_);
		username_ = buffer;
		r = ::GetPrivateProfileStringW(L"AppSettings", L"ServerIP", L"www.bjct.net.cn", buffer, bufferLength * sizeof(wchar_t), fileName_);
		serverIP_ = buffer;
		r = ::GetPrivateProfileStringW(L"AppSettings", L"Pdf2SwfParam", L" -G -s poly2bitmap -S", buffer, bufferLength * sizeof(wchar_t), fileName_);
		pdf2swfParams_ = buffer;

        proxyEnable_ = ::GetPrivateProfileIntW(L"AppSettings", L"ProxyEnable", 0, fileName_);
        proxyPort_ = ::GetPrivateProfileIntW(L"AppSettings", L"ProxyPort", 80, fileName_);
        r = ::GetPrivateProfileStringW(L"AppSettings", L"ProxyIP", L"", buffer, bufferLength * sizeof(wchar_t), fileName_);
		proxyIP_ = buffer;
		r = ::GetPrivateProfileStringW(L"AppSettings", L"ProxyPassword", L"", buffer, bufferLength * sizeof(wchar_t), fileName_);
		proxyPassword_ = buffer;
		r = ::GetPrivateProfileStringW(L"AppSettings", L"ProxyUser", L"", buffer, bufferLength * sizeof(wchar_t), fileName_);
		proxyUser_ = buffer;

        r = ::GetPrivateProfileStringW(L"AppSettings", L"SourceDir", L"", buffer, bufferLength * sizeof(wchar_t), fileName_);
		sourceDir_ = buffer;
		r = ::GetPrivateProfileStringW(L"AppSettings", L"DestinationDir", L"", buffer, bufferLength * sizeof(wchar_t), fileName_);
		destinationDir_ = buffer;

        limitPreviewSize_ = ::GetPrivateProfileIntW(L"AppSettings", L"LimitPreviewSize", 10 * 1024 * 1024, fileName_);
        flvLimitSize_ = ::GetPrivateProfileIntW(L"AppSettings", L"FlvLimitSize", 10 * 1024 * 1024, fileName_);
    }

    void Settings::Save() {
        ::WritePrivateProfileStringW(L"AppSettings", L"Pdf2SwfParam", pdf2swfParams_.c_str(), fileName_);
        ::WritePrivateProfileStringW(L"AppSettings", L"ServerIP", serverIP_.c_str(), fileName_);
        ::WritePrivateProfileStringW(L"AppSettings", L"Port", std::to_wstring(port_).c_str(), fileName_);

        ::WritePrivateProfileStringW(L"AppSettings", L"ProxyEnable", std::to_wstring(proxyEnable_).c_str(), fileName_);
        ::WritePrivateProfileStringW(L"AppSettings", L"ProxyPort", std::to_wstring(proxyPort_).c_str(), fileName_);
        ::WritePrivateProfileStringW(L"AppSettings", L"ProxyIP", proxyIP_.c_str(), fileName_);
        ::WritePrivateProfileStringW(L"AppSettings", L"ProxyPassword", proxyPassword_.c_str(), fileName_);
        ::WritePrivateProfileStringW(L"AppSettings", L"ProxyUser", proxyUser_.c_str(), fileName_);

        ::WritePrivateProfileStringW(L"AppSettings", L"SourceDir", sourceDir_.c_str(), fileName_);
        ::WritePrivateProfileStringW(L"AppSettings", L"DestinationDir", destinationDir_.c_str(), fileName_);

        ::WritePrivateProfileStringW(L"AppSettings", L"LimitPreviewSize", std::to_wstring(limitPreviewSize_).c_str(), fileName_);
        ::WritePrivateProfileStringW(L"AppSettings", L"FlvLimitSize", std::to_wstring(flvLimitSize_).c_str(), fileName_);
    }
}

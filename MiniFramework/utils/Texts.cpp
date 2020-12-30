#include <Windows.h>
//#include <stringapiset.h>
//#include <atlsimpstr.h>
//#include <atlstr.h>
//#include <afxstr.h>
//#include <combaseapi.h>
//#include <WinBase.h>
//#include <sysinfoapi.h>
//#include <wchar.h>
#include <WinNls.h>

#include "Texts.h"
#include <cstdlib>
#include <clocale>

#include <sstream>
#include <regex>

namespace utils::Texts {
    namespace {
        int charcode = CP_UTF8;
    }

    std::wstring from(std::string const& origin) {
        /*
        // std::size_t mbsrtowcs(wchar_t* dst, const char** src, std::size_t len, std::mbstate_t* ps);
        std::mbstate_t state = std::mbstate_t();
        char const* data = origin.data();
        size_t len = 0;
        / *std::* /mbsrtowcs_s(&len, nullptr, 0, &data, origin.length(), &state);
        wchar_t* unicode = new wchar_t[len + 1];
        / *std::* /mbsrtowcs_s(&len, unicode, len, &data, origin.length(), &state);
        std::wstring result = unicode;
        delete[] unicode;
        return result;
        */
        DWORD charCount = MultiByteToWideChar(charcode, 0, origin.data(), -1, NULL, 0);
        wchar_t* unicode = new wchar_t[charCount + 1];
        memset(unicode, 0, (charCount + 1) * sizeof(wchar_t));
        MultiByteToWideChar(charcode, 0, origin.data(), -1, unicode, charCount);
        std::wstring result = unicode;
        delete[] unicode;
        return result;
    }

    std::string const to(std::wstring const& unicode) {
        /*
        // std::size_t wcsrtombs(char* dst, const wchar_t** src, std::size_t len, std::mbstate_t* ps);
        std::mbstate_t state = std::mbstate_t();
        wchar_t const* data = unicode.data();
        size_t len = 0;
        / *std::* /wcsrtombs_s(&len, nullptr, 0, &data, unicode.length(), &state);
        char* native = new char[len + 1];
        / *std::* /wcsrtombs_s(&len, native, len, &data, unicode.length(), &state);
        std::string result = native;
        delete[] native;
        return result;
        */
        DWORD byteCount = WideCharToMultiByte(charcode, NULL, unicode.c_str(), -1, NULL, 0, NULL, FALSE);
        char* native = new char[byteCount + 1];
        memset(native, 0, (byteCount + 1) * sizeof(char));
        WideCharToMultiByte(charcode, NULL, unicode.c_str(), -1, native, byteCount, NULL, FALSE);
        std::string result = native;
        delete[] native;
        return result;
    }

    std::wstring const fromNative(std::string const& native) {
        // std::setlocale(LC_ALL, "en_US.UTF-8");
        // std::setlocale(LC_NUMERIC, "de_DE.UTF-8");
        // std::setlocale(LC_TIME, "ja_JP.UTF-8");
        // CP_ACP | CP_UTF8
        // std::setlocale(LC_ALL, "");
        charcode = CP_ACP;
        return from(native);
    }

    std::string const toNative(std::wstring const& unicode) {
        // std::setlocale(LC_ALL, "");
        charcode = CP_ACP;
        return to(unicode);
    }

    std::wstring const fromUtf8(std::string const& utf8) {
        // std::setlocale(LC_ALL, "en_US.utf8");
        charcode = CP_UTF8;
        return from(utf8);
    }

    std::string const toUtf8(std::wstring const& unicode) {
        // std::setlocale(LC_ALL, "en_US.utf8");
        charcode = CP_UTF8;
        return to(unicode);
    }

    namespace {
        inline char TOHEX(char x) { return x > 9 ? x + 55 : x + 48; }
    }

    std::string const encodeUrl(std::string const& originUrl) {
        std::string result;
        for (auto c : originUrl) {
            if (::isalnum(c)) {
                result += c;
            } else {
                if (::isspace(c)) {
                    result += '+';
                } else {
                    result += '%';
                    result += TOHEX(c >> 4);
                    result += TOHEX(c & 0xF);
                }
            }
        }
        return result;
        //std::transform(originUrl.begin(), originUrl.end(), result.begin(), [](auto c) {
        //    auto result = c;
        //    if (!::isalnum(c)) {
        //        if (::isspace(c)) {
        //            result = '+';
        //        } else {
        //            char buffer[3] = { 0 };
        //            sprintf_s(buffer, "%02X", c);
        //            result = '%' + std::string(buffer, 2);
        //        }
        //    }
        //});
        //return result;
    }

    std::string const decodeUrl(std::string const& encodeUrl) {
        return ""; // @@
    }

    std::vector<std::wstring> const split(std::wstring const& data, wchar_t const delimitation) {
        std::vector<std::wstring> result;
        std::wistringstream ss(data);
        std::wstring s;
        while (std::getline(ss, s, delimitation)) {
            result.push_back(s);
        }
        //std::copy(std::wistream_iterator<std::wstring>(data),
        //    std::wistream_iterator<std::wstring>(),
        //    std::back_inserter(result));
        return result;
    }

    std::vector<std::string> const split(std::string const& data, char const delimitation) {
        std::vector<std::string> result;
        std::istringstream ss(data);
        std::string s;
        while (std::getline(ss, s, delimitation)) {
            result.push_back(s);
        }
        //std::copy(std::wistream_iterator<std::wstring>(data),
        //    std::wistream_iterator<std::wstring>(),
        //    std::back_inserter(result));
        return result;
    }

    std::vector<std::wstring> const split(std::wstring const& data, std::wstring const& delimitation) {
        std::wregex re(delimitation);
        //std::vector<std::wstring> result;

        //std::wstring::size_type begin = 0, end = 0;
        //while ((end = data.find(delimter, begin)) != std::wstring::npos) {
        //    result.push_back(data.substr(begin, end - begin));
        //    begin = end + delimter.length();
        //}
        //result.push_back(data.substr(begin));

        //std::copy(std::wsregex_token_iterator(data.begin(), data.end(), re, -1),
        //    std::wsregex_token_iterator(),
        //    std::back_inserter(result));

        return std::vector<std::wstring>(std::wsregex_token_iterator(data.begin(), data.end(), re, -1),
            std::wsregex_token_iterator());

        //typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        //std::string str("denmark;sweden;india;us");
        //boost::char_separator<char> sep(";");
        //tokenizer tokens(str, sep);
        //BOOST_FOREACH(std::string const& token, tokens) {
        //    std::cout << "<" << *tok_iter << "> " << "\n";
        //}

        //return result;
    }

    std::vector<std::string> const split(std::string const& data, std::string const& delimitation) {
        std::regex re(delimitation);
        //std::vector<std::wstring> result;

        //std::wstring::size_type begin = 0, end = 0;
        //while ((end = data.find(delimter, begin)) != std::wstring::npos) {
        //    result.push_back(data.substr(begin, end - begin));
        //    begin = end + delimter.length();
        //}
        //result.push_back(data.substr(begin));

        //std::copy(std::wsregex_token_iterator(data.begin(), data.end(), re, -1),
        //    std::wsregex_token_iterator(),
        //    std::back_inserter(result));

        return std::vector<std::string>(std::sregex_token_iterator(data.begin(), data.end(), re, -1),
            std::sregex_token_iterator());

        //typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        //std::string str("denmark;sweden;india;us");
        //boost::char_separator<char> sep(";");
        //tokenizer tokens(str, sep);
        //BOOST_FOREACH(std::string const& token, tokens) {
        //    std::cout << "<" << *tok_iter << "> " << "\n";
        //}

        //return result;
    }

    //std::wstring const fromCString(CString const& unicode) {
    //	return static_cast<LPCTSTR>(unicode);
    //}

    //CString const toCString(std::wstring const& unicode) {
    //	return CString(unicode.c_str(), unicode.length());
    //}

    //BSTR toBSTR(std::wstring const& unicode) {
    //	return SysAllocStringLen(unicode.data(), static_cast<UINT>(unicode.length()));
    //}

    //std::wstring const fromBSTR(BSTR unicode) {
    //	return std::wstring(unicode);
    //}

    //void freeBSTR(BSTR unicode) {
    //	SysFreeString(unicode);
    //}

    std::wstring const toLower(std::wstring const& str) {
        std::wstring result;
        std::transform(str.begin(), str.end(), std::back_inserter(result), [](wchar_t v) { return v >= 0b10000000 ? v : v | 0b00100000; }/*::tolower*/);
        return result;
    }

    std::wstring const toUpper(std::wstring const& str) {
        std::wstring result;
        std::transform(str.begin(), str.end(), std::back_inserter(result), [](wchar_t v) { return v >= 0b10000000 ? v : v & 0b11011111; }/*::toupper*/);
        return result;
    }

    std::wstring const encodeXmlEntity(std::wstring const& str) {
        std::wstring result;
        auto e = str.end();
        for (auto i = str.begin(); i != e; ++i) {
            wchar_t chr = *i;
            switch (chr) {
            case L'&':
                result += L"&amp;";
                break;
            case L'<':
                result += L"&lt;";
                break;
            case L'>':
                result += L"&gt;";
                break;
            case L'\'':
                result += L"&apos;";
                break;
            case L'"':
                result += L"&quot;";
                break;
            default:
                result.push_back(chr);
                break;
            }
        }
        return result;
    }

    //std::wstring const GenGuidString() {
    //    std::wstring result;
    //    GUID guid;
    //    HRESULT hResult = CoCreateGuid(&guid);
    //    if (S_OK == hResult) {
    //        CComBSTR bstrGuid(guid);
    //        result = bstrGuid.Detach();
    //    }
    //    result.erase(result.find(L'-'), 1);
    //    result.erase(result.find(L'-'), 1);
    //    result.erase(result.find(L'{'), 1);
    //    result.erase(result.find(L'}'), 1);
    //    return result;
    //}

    //std::wstring const FormatMessageFor(HRESULT hr) {
    //    std::wstring result;
    //    LPVOID pvMsgBuf = NULL;
    //    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    //        NULL, (DWORD)hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pvMsgBuf, 0, NULL)) {
    //        result = (LPWSTR)pvMsgBuf;
    //        LocalFree(pvMsgBuf);
    //    }
    //    return result;
    //}

    std::wstring const FormatSize(std::size_t const bytes) {
        int v = static_cast<int>(bytes);
        std::wstring units = L" KMG";
        size_t unitIndex = 0;
        int p = 0;
        while (v > 1024) {
            auto d = std::div(v, 1024);
            v = d.quot;
            p = d.rem;
            ++unitIndex;
        }
        std::wstring result = std::to_wstring(v) + L"B";
        if (v != bytes) {
            std::wstring prec;
            int pre = static_cast<int>(p / 1024.0 * 10);
            if (pre) {
                prec = L"." + std::to_wstring(pre);
            }
            result = std::to_wstring(v) + prec + units[unitIndex] + L"iB";
        }
        return result;
    }

    std::string const FormatDateTime(short year, short month, short date, short hour, short minute, short second) {
        char dateTime[32] = { 0 };
        //SYSTEMTIME tm;
        //GetLocalTime(&tm);
        //sprintf_s(dateTime, "%04d-%02d-%02d %02d:%02d:%02d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
        sprintf_s(dateTime, "%04d-%02d-%02d %02d:%02d:%02d", year, month, date, hour, minute, second);
        return std::string(dateTime);
    }
}

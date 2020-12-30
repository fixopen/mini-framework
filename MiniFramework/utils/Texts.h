#include <string>
#include <vector>

namespace utils::Texts {
    std::wstring const fromNative(std::string const &native);

    std::string const toNative(std::wstring const &unicode);

    std::wstring const fromUtf8(std::string const &utf8);

    std::string const toUtf8(std::wstring const &unicode);

    std::string const encodeUrl(std::string const &originUrl);

    std::string const decodeUrl(std::string const &encodeUrl);

    std::vector <std::wstring> const split(std::wstring const &data, wchar_t const delimitation);

    std::vector <std::string> const split(std::string const &data, char const delimitation);

    std::vector <std::wstring> const split(std::wstring const &data, std::wstring const &delimitation);

    std::vector <std::string> const split(std::string const &data, std::string const &delimitation);

    //std::wstring const fromCString(CString const& unicode);
    //CString const toCString(std::wstring const& unicode);
    //std::wstring const fromBSTR(BSTR unicode);
    //BSTR toBSTR(std::wstring const& unicode);
    //void freeBSTR(BSTR unicode);
    std::wstring const toLower(std::wstring const &str);

    std::wstring const toUpper(std::wstring const &str);

    std::wstring const encodeXmlEntity(std::wstring const &str);

    //std::wstring const GenGuidString();
    //std::wstring const FormatMessageFor(HRESULT hr);
    std::wstring const FormatSize(std::size_t const bytes);

    std::string const FormatDateTime(short year, short month, short date, short hour, short minute, short second);
}

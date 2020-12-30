//#include <combaseapi.h> // for CLSIDFromProgID CLSID HRESULT SUCCEEDED LPOLESTR StringFromCLSID
#include <ObjBase.h>

#include "Utils.h"
#include "Texts.h"

namespace utils::Utils {
    std::vector<unsigned short> const ToLittleEndian(unsigned short* pWords, int nFirstIndex, int nLastIndex) {
        std::vector<unsigned short> result;
        result.reserve(nLastIndex - nFirstIndex);
        for (int index = nFirstIndex; index <= nLastIndex; ++index) {
            unsigned short v = (pWords[index] >> 8) | (pWords[index] & 0xFF);
            result.push_back(v);
        }
        return result;
    }

    std::vector<unsigned short> const TrimSpace(std::vector<unsigned short> const& v) {
        auto left = v.begin();
        while (*left == 0x20 && left != v.end()) {
            ++left;
        }
        auto right = v.rbegin();
        while (*right == 0x20 && right != v.rend()) {
            ++right;
        }
        return std::vector<unsigned short>(left, right.base());
    }

    std::wstring const ToWString(std::vector<unsigned short> const& v) {
        std::wstring result;
        result.reserve(v.size());
        for (auto c : v) {
            result.push_back(static_cast<wchar_t>(c));
        }
        return result;
    }

    std::wstring const ProgramIdToClassId(std::wstring const& programId) {
        std::wstring result;
        CLSID clsid;
        HRESULT hr = CLSIDFromProgID(programId.c_str(), &clsid);
        if (SUCCEEDED(hr)) {
            LPOLESTR unicodeClsid;
            hr = StringFromCLSID(clsid, &unicodeClsid);
            if (SUCCEEDED(hr)) {
                result = unicodeClsid;
            }
        }
        return result;
    }

    namespace {
        int const C1 = 44;
        int const C2 = 44;
        int const C3 = 65;
        int const C4 = 26;
    }

    std::string const Encrypt(std::string const& plain, unsigned short key) {
        std::string result, str;
        for (auto i = 0u; i < plain.length(); i++) {
            char chr = plain.at(i) ^ (key >> 8);
            str.push_back(chr);
            key = (chr + key) * C1 + C2;
        }
        for (auto i = 0u; i < str.length(); i++) {
            char j = (BYTE)str.at(i);
            char first = C3 + j / C4;
            char second = C3 + j % C4;
            result.push_back(first);
            result.push_back(second);
        }
        return result;
    }

    std::string const Decrypt(std::string const& cipher, unsigned short key) {
        std::string result, str;
        for (auto i = 0u; i < cipher.length() / 2; i += 2) {
            char j = ((BYTE)cipher.at(2 * i) - C3) * C4;
            j += ((BYTE)cipher.at(2 * i + 1) - C3);
            str += j;
        }
        for (auto i = 0u; i < str.length(); i++) {
            char chr = (BYTE)str.at(i) ^ (key >> 8);
            result.push_back(chr);
            key = (chr + key) * C1 + C2;
        }
        return result;
    }
}

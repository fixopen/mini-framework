#pragma once

#include <vector>
#include <string>
#include <functional>
#include <Guiddef.h>

namespace utils::Utils {
    std::vector<unsigned short> const ToLittleEndian(unsigned short* pWords, int nFirstIndex, int nLastIndex);
    std::vector<unsigned short> const TrimSpace(std::vector<unsigned short> const& v);
    std::wstring const ToWString(std::vector<unsigned short> const& v);
    std::wstring const ProgramIdToClassId(std::wstring const& programId);
    std::string const Encrypt(std::string const& S, unsigned short key);
    std::string const Decrypt(std::string const& S, unsigned short key);

    template <typename TInterface>
    bool COMProcess(IID const& classId, int classContext, IID const& interfaceId, std::function<bool(TInterface* anInterface)>&& process) {
        auto result = false;
        HRESULT hr = CoInitialize(nullptr);
        if (SUCCEEDED(hr)) {
            TInterface* anInterface = nullptr;
            hr = CoCreateInstance(classId, nullptr, classContext, interfaceId, (void**)&anInterface);
            if (SUCCEEDED(hr) && anInterface != nullptr) {
                result = process(anInterface);
                anInterface->Release();
            }
            CoUninitialize();
        }
        return result;
    }
}

#pragma once

#include <string>

namespace utils {
    class Settings {
    public:
        void Load();
        void Save();
    private:
        std::wstring currentPath_;
        std::wstring appUserPath_;
		std::wstring userId_;
		std::wstring homePath_;

		std::wstring username_;
        std::wstring password_;
        std::wstring serverIP_;
        int port_;
		std::wstring pdf2swfParams_;
		int proxyEnable_;
		std::wstring proxyUser_;
		std::wstring proxyPassword_;
		std::wstring proxyIP_;
        int proxyPort_;
        std::wstring sourceDir_;
        std::wstring destinationDir_;
		int limitPreviewSize_;
		int flvLimitSize_;
		static constexpr wchar_t const* const fileName_ = L"D:\\happychildren\\program\\config.ini";
    };
}

#pragma once

#include <string> // for std::wstring
#include <vector> // for std::vector
#include <map> // for std::map
//#include <span> // for std::span
#include <cstddef> // for std::byte

namespace utils::zip {
    bool Unzip(std::wstring const &zipFile, std::wstring const &folder);

    bool Zip(std::wstring const &zipFile, std::wstring const &folder);

    using ProgressCallback = void(std::wstring const& pathName, std::size_t total, std::size_t current);
    ProgressCallback* setProgressCallback(ProgressCallback* callback);

    //bool Unzip(std::span<std::byte> const zipContent, std::map<std::wstring, std::span<std::byte>>& contents);
    //bool Zip(std::span<std::byte> zipContent, std::map<std::wstring, std::span<std::byte> const& contents);

    bool UnzipViaZlib(std::wstring const& zipFile, std::wstring const& folder);

    bool ZipViaZlib(std::wstring const& zipFileName, std::wstring const& folder);
}

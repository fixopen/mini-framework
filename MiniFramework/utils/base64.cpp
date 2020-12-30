#include "base64.h"

#include <atlenc.h>
//#include <atlstr.h>

namespace utils::base64 {
    std::string const encode(std::string const &data) {
        int base64Length = Base64EncodeGetRequiredLength(static_cast<int>(data.length()));
        LPSTR bytes = (LPSTR) malloc(base64Length);
        Base64Encode(reinterpret_cast<const BYTE *>(data.data()), static_cast<int>(data.length()), bytes,
                     &base64Length);
        auto r = std::string(bytes, base64Length);
        free(bytes);
        return r;
    }

    std::string const decode(std::string const &data) {
        int originLength = Base64DecodeGetRequiredLength(static_cast<int>(data.length()));
        BYTE *bytes = (BYTE *) malloc(originLength);
        Base64Decode(reinterpret_cast<LPCSTR>(data.data()), static_cast<int>(data.length()), bytes, &originLength);
        auto r = std::string(reinterpret_cast<char *>(bytes), originLength);
        free(bytes);
        return r;
    }
}

/*
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <iostream>

using namespace boost::archive::iterators;
using namespace std;

int main(int argc, char **argv) {
    typedef transform_width< binary_from_base64<remove_whitespace<string::const_iterator> >, 8, 6 > it_binary_t;
    typedef insert_linebreaks<base64_from_binary<transform_width<string::const_iterator, 6, 8> >, 72 > it_base64_t;
    string s;
    getline(cin, s, '\n');
    cout << "Your string is: '" << s << "'" << endl;

    // Encode
    unsigned int writePaddChars = (3 - s.length() % 3) % 3;
    string base64(it_base64_t(s.begin()), it_base64_t(s.end()));
    base64.append(writePaddChars, '=');

    cout << "Base64 representation: " << base64 << endl;

    // Decode
    unsigned int paddChars = count(base64.begin(), base64.end(), '=');
    std::replace(base64.begin(), base64.end(), '=', 'A'); // replace '=' by base64 encoding of '\0'
    string result(it_binary_t(base64.begin()), it_binary_t(base64.end())); // decode
    result.erase(result.end() - paddChars, result.end());  // erase padding '\0' characters
    cout << "Decoded: " << result << endl;
    return 0;
}
*/
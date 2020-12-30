#include <string>

namespace utils {
    /**
     * Usage:
     * MD5Base d;
     * d.Init();
     * d.Update(input, inputlen);
     * unsigned char digest[16];
     * d.Final(digest);
     */
    class MD5Base {
    private:
    public:
        MD5Base(): context_(new Context()) {}
        ~MD5Base() {
            delete context_;
        }
        void Init();
        void Update(unsigned char* input, unsigned int inputLen);
        void Final(unsigned char digest[16]);
    private:
        /* MD5 context. */
        struct Context {
            unsigned int state[4]; /* state (ABCD) */
            unsigned int count[2]; /* number of bits, modulo 2^64 (lsb first) */
            unsigned char buffer[64]; /* input buffer */
        };

        Context* context_;
        void Encode(unsigned char* output, unsigned int* input, unsigned int len);
        void Decode(unsigned int* output, unsigned char* input, unsigned int len);
        void MD5Transform(unsigned int state[4], unsigned char block[64]);
    };

    std::string md5hex(std::string const& data) {
        std::string result;
        MD5Base d;
        d.Init();
        d.Update((unsigned char*) data.c_str(), (unsigned int) data.length());
        unsigned char digest[16];
        d.Final(digest);
        for (int i = 0; i < 16; ++i) {
            char bytes[3] = {};
            sprintf_s(bytes, "%02X", digest[i] & 0xFF);
            result += bytes;
        }
        return result;
    }

    std::wstring md5whex(std::string const& data) {
        std::wstring result;
        MD5Base d;
        d.Init();
        d.Update((unsigned char*) data.c_str(), (unsigned int) data.length());
        unsigned char digest[16];
        d.Final(digest);
        for (int i = 0; i < 16; ++i) {
            wchar_t bytes[3] = {};
            swprintf(bytes, 3, L"%02X", digest[i] & 0xFF);
            result += bytes;
        }
        return result;
    }
}

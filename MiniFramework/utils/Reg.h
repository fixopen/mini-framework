#ifndef __UTIL_REG_H__
#define __UTIL_REG_H__

#include <string>
#include <functional>
#include <winreg.h>

namespace utils {
	template<typename HKEY>
    class Reg {
    public:
		Reg(HKEY rootKey, std::wstring const& path) {
			state_ = ::RegOpenKeyEx(rootKey, path.c_str(), 0, KEY_READ | KEY_WRITE, &key_) == ERROR_SUCCESS;
		}
		~Reg() {
			if (key_) {
				::RegCloseKey(key_);
			}
		}
		long Write(std::wstring const& name, unsigned int value) {
			return ::RegSetValueEx(key_, name.c_str(), 0L, REG_DWORD, (const unsigned char*)&value, sizeof(unsigned int));
		}
		long Write(std::wstring const& name, std::string const& value) {
			return ::RegSetValueEx(key_, name.c_str(), 0L, REG_BINARY, reinterpret_cast<const unsigned char*>(value.data()), static_cast<unsigned int>(value.length()));
		}
        long Write(std::wstring const& name, std::wstring const& value) {
			return ::RegSetValueEx(key_, name.c_str(), 0L, REG_SZ, reinterpret_cast<const unsigned char*>(value.c_str()), static_cast<unsigned int>(value.length() * sizeof(wchar_t)));
		}
		unsigned int ReadDword(std::wstring const& name) {
			unsigned int result = 0;
			unsigned int type; // REG_BINARY REG_DWORD REG_DWORD_LITTLE_ENDIAN REG_DWORD_BIG_ENDIAN REG_EXPAND_SZ REG_LINK REG_MULTI_SZ REG_NONE REG_QWORD REG_QWORD_LITTLE_ENDIAN REG_SZ
			unsigned int tempValue;
			unsigned int typeSize = sizeof(unsigned int);
			long r = ::RegQueryValueEx(key_, name.c_str(), nullptr, &type, (unsigned char*)&tempValue, &typeSize);

			if (r == ERROR_SUCCESS) {
				result = tempValue;
			}

			return result;
		}
		std::wstring ReadString(std::wstring const& name) {
			return Read<std::wstring>(key_, name, [](BYTE const* data, DWORD length) -> std::wstring {
				return std::wstring(reinterpret_cast<wchar_t const*>(data), length);
			});
		}
		std::string ReadBinary(std::wstring const& name) {
			return Read<std::string>(key_, name, [](BYTE const* data, DWORD length) -> std::string {
				return std::string(reinterpret_cast<char const*>(data), length);
			});
		}
        operator bool() {
            return state_;
        }
    protected:
		template<typename T>
		T Read(HKEY key, std::wstring const& name, std::function<T(BYTE* data, DWORD length)> postProcess) {
			T result;
			DWORD type;
			DWORD length = 0;
			LONG r = ::RegQueryValueEx(key, (name.empty() ? nullptr : name.c_str()), NULL, &type, nullptr, &length);
			if (r == ERROR_SUCCESS) {
				auto value = (BYTE*)std::malloc(length);
				r = ::RegQueryValueEx(key, (name.empty() ? nullptr : name.c_str()), NULL, &type, value, &length);
				if (r == ERROR_SUCCESS) {
					result = postProcess(value, length);
				}
				std::free(value);
			}
			return result;
		}

        HKEY key_ = nullptr;
        bool state_ = false;
    };
}

#endif //__UTIL_REG_H__

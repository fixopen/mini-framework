#include <Windows.h>

#include "Mutex.h"

//#include <WinBase.h>
//#include <synchapi.h>
//#include <handleapi.h>

namespace utils {
    Mutex::Mutex(std::wstring const& name) {
        mutex_ = CreateMutexW(NULL, false, name.c_str());
    }

    Mutex::~Mutex() {
        CloseHandle(mutex_);
    }

    BOOL Mutex::Lock() {
        BOOL result = FALSE;
        if (mutex_ != nullptr) {
            DWORD r = WaitForSingleObject(mutex_, INFINITE);
            if (r == WAIT_OBJECT_0) {
                result = TRUE;
            }
        }
        return result;
    }

    BOOL Mutex::UnLock() {
        return ReleaseMutex(mutex_);
    }
}

#pragma once

#include <string>
#include <map>
#include <vector>
#include <functional>
//#include <minwindef.h> // for HANDLE, BOOL

namespace utils {
    class Mutex {
    public:
        Mutex(std::wstring const& name);
        ~Mutex();
        BOOL Lock();
        BOOL UnLock();
    private:
        HANDLE mutex_;
    };

    // RWMUTEX
    class ReadWriteMutex {
        ReadWriteMutex(const ReadWriteMutex &) = delete;

        ReadWriteMutex(ReadWriteMutex &&) = delete;

        ReadWriteMutex const &operator=(const ReadWriteMutex &) = delete;

    private:
        HANDLE mutex_ = nullptr;
        std::map<DWORD, HANDLE> threads_;
        DWORD wi = INFINITE;
    public:
        explicit ReadWriteMutex(bool D = false) {
            if (D) {
                wi = 10000;
            } else {
                wi = INFINITE;
            }
            mutex_ = CreateMutex(0, 0, 0);
        }

        ~ReadWriteMutex() {
            CloseHandle(mutex_);
            mutex_ = 0;
            for (auto &a : threads_) {
                CloseHandle(a.second);
            }
            threads_.clear();
        }

        HANDLE CreateIf(bool KeepReaderLocked = false) {
            auto tim = WaitForSingleObject(mutex_, INFINITE);
            if (tim == WAIT_TIMEOUT && wi != INFINITE) {
                OutputDebugStringW(L"LockRead debug timeout!");
            }
            DWORD id = GetCurrentThreadId();
            if (threads_[id] == 0) {
                HANDLE e0 = CreateMutex(0, 0, 0);
                threads_[id] = e0;
            }
            HANDLE e = threads_[id];
            if (!KeepReaderLocked) {
                ReleaseMutex(mutex_);
            }
            return e;
        }

        HANDLE LockRead() {
            auto z = CreateIf();
            auto tim = WaitForSingleObject(z, wi);
            if (tim == WAIT_TIMEOUT && wi != INFINITE) {
                OutputDebugStringW(L"LockRead debug timeout!");
            }
            return z;
        }

        void LockWrite() {
            CreateIf(true);

            // Wait for all
            std::vector<HANDLE> allThreads;
            allThreads.reserve(threads_.size());
            for (auto &a : threads_) {
                allThreads.push_back(a.second);
            }

            auto tim = WaitForMultipleObjects((DWORD) allThreads.size(), allThreads.data(), TRUE, wi);
            if (tim == WAIT_TIMEOUT && wi != INFINITE) {
                OutputDebugStringW(L"LockWrite debug timeout!");
            }

            // We don't want to keep threads, the hChangeMap is enough
            for (auto &a : threads_) {
                ReleaseMutex(a.second);
            }

            // Reader is locked
        }

        void ReleaseWrite() {
            ReleaseMutex(mutex_);
        }

        void ReleaseRead(HANDLE f) {
            ReleaseMutex(f);
        }
    };

    template<typename T>
    class lock {
    private:
        mutable T t;
        mutable ReadWriteMutex m;

        class proxy {
            T *const p;
            ReadWriteMutex *m;
            int me;
            // append
            HANDLE f; // @@
        public:
            proxy(T *const _p, ReadWriteMutex *_m, int _me) : p(_p), m(_m), me(_me) {
                if (me == 2) {
                    m->LockWrite();
                } else {
                    f = m->LockRead(); // @@
                }
            }

            ~proxy() {
                if (me == 2) {
                    m->ReleaseWrite();
                } else {
                    m->ReleaseRead(f); // @@
                }
            }

            T *operator->() {
                return p;
            }

            const T *operator->() const {
                return p;
            }

            T *get() {
                return p;
            }

            const T *get() const {
                return p;
            }
        };

    public:
        template<typename ...Args>
        explicit lock(Args ... args) : t(args...) {}

        proxy r() const {
            return proxy(&t, &m, 1);
        }

        proxy w() {
            return proxy(&t, &m, 2);
        }

        void readlock(std::function<void(const T &)> f) const {
            proxy mx(&t, &m, 1);
            f(*mx.get());
        }

        void writelock(std::function<void(T &)> f) {
            proxy mx(&t, &m, 2);
            f(*mx.get());
        }

        proxy operator->() {
            return w();
        }

        proxy operator->() const {
            return r();
        }
    };
}

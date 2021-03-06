// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE

#ifdef _DEBUG
#		pragma comment(lib, "..\\Debug\\libcrypto32MT.lib")
#       pragma comment(lib, "..\\Debug\\libssl32MT.lib")
#       pragma comment(lib, "..\\Debug\\libuv_d.lib")
#       pragma comment(lib, "..\\Debug\\Crypt32.lib")
#       pragma comment(lib, "..\\Debug\\zlib.lib")
#else
#		pragma comment(lib, "..\\Release\\libcrypto32MT.lib")
#       pragma comment(lib, "..\\Release\\libssl32MT.lib")
#       pragma comment(lib, "..\\Release\\libuv.lib")
#       pragma comment(lib, "..\\Release\\Crypt32.lib")
#       pragma comment(lib, "..\\Release\\zlib.lib")
#endif

// TODO: 在此处引用程序需要的其他头文件

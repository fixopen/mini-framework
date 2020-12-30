// FIXME #include <afxinet.h>
#include <WS2tcpip.h>
#include <WinInet.h>
#include <iphlpapi.h>
//#include <assert.h>
//#include <WS2tcpip.h>

#include "Communication.h"
#include "Texts.h"

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Wininet.lib")

namespace utils::Communications {
    int ProcessNetworkInfo(std::function<int(PIP_ADAPTER_INFO adapterInfo, void* args)>&& process, void* args) {
        int result = 0;
        ULONG adapterInfoLength = sizeof(IP_ADAPTER_INFO);
        PIP_ADAPTER_INFO adapterInfo = (IP_ADAPTER_INFO*) malloc(sizeof(IP_ADAPTER_INFO));
        if (adapterInfo == nullptr) {
            // printf("Error allocating memory needed to call GetAdaptersinfo\n");
            result = -1;
        } else {
            // Make an initial call to GetAdaptersInfo to get
            // the necessary size into the ulOutBufLen variable
            if (GetAdaptersInfo(adapterInfo, &adapterInfoLength) == ERROR_BUFFER_OVERFLOW) {
                free(adapterInfo);
                adapterInfo = (IP_ADAPTER_INFO*)malloc(adapterInfoLength);
                if (adapterInfo == nullptr) {
                    // printf("Error allocating memory needed to call GetAdaptersinfo\n");
                    result = -1;
                } else {
                    DWORD r = 0;
                    if ((r = GetAdaptersInfo(adapterInfo, &adapterInfoLength)) == NO_ERROR) {
                        PIP_ADAPTER_INFO currentAdapter = adapterInfo;
                        while (currentAdapter) {
                            result = process(currentAdapter, args);
                            if (result == -1) {
                                break;
                            }
                            // 下一个网卡信息
                            currentAdapter = currentAdapter->Next;
                        }
                    } else {
                        result = r;
                        // printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
                    }

                    if (adapterInfo) {
                        free(adapterInfo);
                    }
                }
            }
        }

        return result;
    }

    std::vector<IP_ADAPTER_INFO> GetAdapterInfo() {
        std::vector<IP_ADAPTER_INFO> result;
        ProcessNetworkInfo([](PIP_ADAPTER_INFO adapter, void* args) -> int {
            std::vector<IP_ADAPTER_INFO>* a = static_cast<std::vector<IP_ADAPTER_INFO>*>(args);
            a->push_back(static_cast<IP_ADAPTER_INFO>(*adapter));
            return 0;
        }, (void*) &result);
        return result;
    }

    std::map<int, NetworkInfo> GetNetworkInfo() {
        std::map<int, NetworkInfo> result;
        ProcessNetworkInfo([](PIP_ADAPTER_INFO adapter, void* args) -> int {
            int result = 0;
            NetworkInfo item;
            auto r = FormatMac(std::string(reinterpret_cast<char*>(adapter->Address), adapter->AddressLength));
            strcpy_s(item.mac, r.c_str());
            if (strstr(adapter->IpAddressList.IpAddress.String, "0.0.0.0") == 0) {
                strcpy_s(item.ip, 16, adapter->IpAddressList.IpAddress.String);
            }
            strcpy_s(item.gateway, 16, adapter->GatewayList.IpAddress.String);
            strcpy_s(item.description, 64, adapter->Description);
            std::map<int, NetworkInfo>* a = (std::map<int, NetworkInfo>*)args;
            a->insert(std::make_pair(adapter->Type, item));
            return result;
            }, (void*) &result);
        return result;
    }

    std::wstring const GetMacAddress(int type) {
        // type: IF_TYPE_IEEE80211 MIB_IF_TYPE_ETHERNET
        std::wstring result;
        auto info = GetNetworkInfo();
        auto it = info.find(type);
        if (it != info.end()) {
            result = FormatMacW(it->second.mac);
        }
        return result;
    }

    std::string const DirectGetMacAddress() {
        std::string result;
        char args[6] = { 0 };
        int r = ProcessNetworkInfo([](PIP_ADAPTER_INFO adapter, void* args) -> int {
            int ret = 0;
            if ((strstr(adapter->Description, "PCI") > 0 && strstr(adapter->Description, "802") == 0) || strstr(adapter->Description, "Ethernet") > 0) {
                memcpy(args, adapter->Address, adapter->AddressLength);
                ret = -1;
            }
            return ret;
        }, (void*) args);
        return std::string(args, 6);
    }

    std::string const FormatMac(std::string const& mac, char join) {
        std::string result;
        auto macLength = mac.length();
        bool first = true;
        for (std::size_t i = 0; i < macLength; ++i) {
            char s[6] = { 0 };
            sprintf_s(s, "%02X", static_cast<unsigned int>(static_cast<unsigned char>(mac[i])));
            if (join) {
                if (!first) {
                    result += join;
                } else {
                    first = false;
                }
            }
            result += s;
        }
        return result;
    }

    std::wstring const FormatMacW(std::string const& mac, char join) {
        std::wstring result;
        auto macLength = mac.length();
        bool first = true;
        for (std::size_t i = 0; i < macLength; ++i) {
            wchar_t s[6] = { 0 };
            swprintf_s(s, L"%02X", (int)mac[i]);
            if (join) {
                if (!first) {
                    result += join;
                } else {
                    first = false;
                }
            }
            result += s;
        }
        return result;
    }

    namespace {
        bool isNetLibInit = false;
    }

    int ClientExecutor(std::string const& host, std::string const& port, std::function<int(unsigned long long sfd, void* const args)>&& process, void* const context) {
        if (!isNetLibInit) {
            WSADATA wsData;
            WSAStartup(0x0202, &wsData);
        }

        int result = 0;
        // Obtain address(es) matching host/port
        struct addrinfo hints;
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM; // SOCK_DGRAM
        hints.ai_flags = AI_PASSIVE; // For wildcard IP address
        hints.ai_flags = 0;
        hints.ai_protocol = 0; // Any protocol

        struct addrinfo* r = nullptr;
        int s = getaddrinfo(host.c_str(), port.c_str(), &hints, &r);
        if (s == 0) {
            struct addrinfo* currentAddr;
            for (currentAddr = r; currentAddr != nullptr; currentAddr = currentAddr->ai_next) {
                SOCKET socketId = socket(currentAddr->ai_family, currentAddr->ai_socktype, currentAddr->ai_protocol);
                if (socketId == -1)
                    continue;

                if (connect(socketId, currentAddr->ai_addr, static_cast<int>(currentAddr->ai_addrlen)) != -1) {
                    result = process(static_cast<unsigned long long>(socketId), context);
                    break;  // Success
                }

                closesocket(socketId);
            }

            if (currentAddr == nullptr) {  // No address succeeded
                fprintf(stderr, "Could not connect\n");
                result = EXIT_FAILURE;
            }

            freeaddrinfo(r);  // No longer needed
        } else {
            fprintf(stderr, "getaddrinfo: %ws\n", gai_strerror(s));
            result = EXIT_FAILURE;
        }
        return result;
    }

    namespace {
        std::string decodeChunked(std::string const chunked) {
            std::string result;
            size_t lengthContentPos = 0;
            auto len = 0L;
            do {
                auto currentLengthContentPos = chunked.find("\r\n", lengthContentPos);
                if (currentLengthContentPos != std::string::npos) {
                    auto length = chunked.substr(lengthContentPos, currentLengthContentPos);
                    size_t index = 0;
                    len = std::stol(length, &index, 16);
                    if (len) {
                        lengthContentPos += 2;
                        auto content = chunked.substr(currentLengthContentPos + 2, len);
                        result += content;
                        lengthContentPos = currentLengthContentPos + 2 + len + 2;
                    }
                }
            } while (len);
            return result;
        }
    }

    std::shared_ptr<ResponseMessage> parseHeader(std::string const& head) {
        std::shared_ptr<ResponseMessage> response = std::make_shared<ResponseMessage>();
        auto heads = Texts::split(head, std::string("\r\n"));
        auto statusLine = heads[0];
        auto sdp = statusLine.find(" ", 0);
        if (sdp != std::string::npos) {
            auto sedp = statusLine.find(" ", sdp + 1);
            if (sedp != std::string::npos) {
                response->statusCode = std::stol(statusLine.substr(sdp + 1, sedp));
                response->phrase = statusLine.substr(sedp + 1);
            }
        }
        response->headers.clear();
        for (size_t i = 1; i < heads.size(); ++i) {
            auto fdp = heads[i].find(": ", 0);
            if (fdp != std::string::npos) {
                auto name = heads[i].substr(0, fdp);
                auto value = heads[i].substr(fdp + 2);
                response->headers.insert(std::make_pair(name, value));
            }
        }
        return response;
    }

    std::shared_ptr<ResponseMessage> HttpExecutor(std::shared_ptr<RequestMessage> request) {
        struct TArgs {
            std::shared_ptr<RequestMessage> request;
            std::shared_ptr<ResponseMessage> response;
        };
        TArgs args;
        args.request = request;
        args.response = std::make_shared<ResponseMessage>();

        auto hostIt = request->headers.find("Host");
        if (hostIt != request->headers.end()) {
            auto host = hostIt->second;
            std::string port = "http";
            auto nameAndPort = Texts::split(host, ':');
            if (nameAndPort.size() == 2) {
                host = nameAndPort[0];
                port = nameAndPort[1];
            }
            ClientExecutor(host, port, [](SOCKET s, void* const args) -> int {
                int result = 0;
                TArgs* context = (TArgs*)args;
                std::string sendData = context->request->method + " " + context->request->path + " HTTP/1.1\r\n";
                for (auto it = context->request->headers.begin(); it != context->request->headers.end(); ++it) {
                    sendData += it->first + ": " + it->second + "\r\n";
                }
                if (!context->request->body.empty()) {
                    sendData += "Content-Length: " + std::to_string(context->request->body.length());
                }
                sendData += "\r\n";
                sendData += context->request->body;
                int ret = send(s, sendData.data(), static_cast<int>(sendData.length()), 0);
                if (ret == sendData.length()) {
                    auto headerParsed = false;
                    std::string receive;
                    char buffer[4096] = { 0 };
                    do {
                        memset(buffer, 0, 4096);
                        ret = ::recv(s, buffer, sizeof(buffer), 0);
                        if (ret > 0 && ret != WSAECONNRESET) {
                            receive += std::string(buffer, ret);
                        }
                        if (!receive.empty()) {
                            std::string head = receive;
                            auto dp = receive.find("\r\n\r\n", 0);
                            if (dp != std::string::npos) {
                                context->response->body = receive.substr(dp + 4);
                                head = receive.substr(0, dp);
                            }
                            if (!headerParsed) {
                                context->response = parseHeader(head);
                                headerParsed = true;
                            }
                            // Content-Length
                            auto cli = context->response->headers.find("Content-Length");
                            if (cli != context->response->headers.end()) {
                                if (std::stol(cli->second) == context->response->body.length()) {
                                    break; // finally
                                }
                            }

                            // Transfer-Encoding == chunked
                            auto tei = context->response->headers.find("Transfer-Encoding");
                            if (tei != context->response->headers.end()) {
                                if (tei->second == "chunked") {
                                    static const std::string chunkedEnd = "\r\n0\r\n\r\n";
                                    if (context->response->body.find(chunkedEnd) == context->response->body.length() - chunkedEnd.length()) {
                                        context->response->body = decodeChunked(context->response->body);
                                        break; // finally
                                    }
                                }
                            }
                        }
                    } while (ret != 0 && ret != WSAECONNRESET && ret != SOCKET_ERROR);
                }
                return result;
                }, &args);
        }
        return args.response;
    }

    struct EmptyBody : Body {
        EmptyBody() : Body() {}

        size_t GetLength() const override {
            return 0;
        }

        std::string GetContent() const override {
            return "";
        }
    };

    Body* Body::NullBody = new EmptyBody();

    namespace {
        wchar_t const* agent = L"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)"; // DOWNUP-HTTP-AGENT

        //std::wstring to_wstring(int v) {
        //	wchar_t buffer[64] = {};
        //	wsprintf(buffer, L"%d", v);
        //	return buffer;
        //}

        //void clearHttp(CInternetSession* session, CHttpConnection* connection, CHttpFile* file) {
        //    // 关闭句柄
        //    file->Close();
        //    connection->Close();
        //    session->Close();
        //}

        void clearHttp(HINTERNET hInternet, HINTERNET hSession, HINTERNET hRequest) {
            // 关闭句柄
            InternetCloseHandle(hRequest);
            InternetCloseHandle(hSession);
            InternetCloseHandle(hInternet);
        }
    }

    //int HttpExecutorViaInet(std::wstring const& method, std::wstring const& url, std::wstring const& token, std::vector<std::wstring> const& headers, Body* body, std::function<int(Type type, std::string const& data)>&& processor) {
    //    int result = 0;
    //    // 初始化 WinInet 环境
    //    CInternetSession* session = new CInternetSession(agent);
    //    // 打开 http session
    //    DWORD serviceType;
    //    CString server;
    //    CString object;
    //    INTERNET_PORT port;
    //    AfxParseURL(url.c_str(), serviceType, server, object, port);
    //    CHttpConnection* connection = session->GetHttpConnection(server,
    //        /*flag = */serviceType == AFX_INET_SERVICE_HTTP ? INTERNET_FLAG_KEEP_CONNECTION : INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_SECURE,
    //        port);
    //    // 打开 http 请求的句柄
    //    LPCTSTR szAccept[] = { L"*/*", NULL };
    //    CHttpFile* file = connection->OpenRequest(method.c_str(), object,
    //        /*referer = */nullptr, /*context = */1, /*accept = */nullptr, /*version = */nullptr,
    //        /*flag = */serviceType == AFX_INET_SERVICE_HTTP ? INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE : INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID);

    //    BOOL r = FALSE;
    //    // 添加 header 信息
    //    for (auto& header = headers.begin(); header != headers.end(); ++header) {
    //        r = file->AddRequestHeaders(header->c_str());
    //        if (r == FALSE) {
    //            result = HeaderSendFailure; // throw "HEADER error";
    //            break;
    //        }
    //    }
    //    if (!token.empty()) {
    //        std::wstring s = L"Cookie: sessionId=" + token;
    //        r = file->AddRequestHeaders(s.c_str());
    //        if (r == FALSE) {
    //            result = HeaderSendFailure; // throw "HEADER error";
    //        }
    //    }
    //    r = file->AddRequestHeaders((L"Host: " + std::wstring(static_cast<LPCTSTR>(server)) + L":" + std::to_wstring(port)).c_str());
    //    if (r == FALSE) {
    //        result = HeaderSendFailure; // throw "HEADER error";
    //    }
    //    r = file->AddRequestHeaders((L"Content-Length: " + std::to_wstring(body->GetLength())).c_str());
    //    if (r == FALSE) {
    //        result = HeaderSendFailure; // throw "HEADER error";
    //    }
    //    if (result != 0) {
    //        clearHttp(session, connection, file);
    //        return result;
    //    }

    //    if (body->SupportPartical()) {
    //        constexpr size_t chunkSize = 8192;
    //        r = file->SendRequestEx(static_cast<DWORD>(body->GetLength()));
    //        std::string content = body->GetParticalContent(chunkSize);
    //        do {
    //            file->Write(content.data(), static_cast<UINT>(content.length()));
    //            content = body->GetParticalContent(chunkSize);
    //        } while (content.size() == chunkSize);
    //        r = file->EndRequest();
    //    } else {
    //        if (body->GetContent() == "") {
    //            r = file->SendRequest(nullptr, 0, nullptr, 0);
    //        } else {
    //            r = file->SendRequest(nullptr, 0, (void*)body->GetContent().data(), static_cast<DWORD>(body->GetLength()));
    //        }
    //    }

    //    if (r == FALSE) {
    //        result = BodySendFailure; // throw "END request error";
    //    }
    //    if (result != 0) {
    //        clearHttp(session, connection, file);
    //        return result;
    //    }

    //    DWORD recvSize = 0; // size of the data available

    //    DWORD statusCode;
    //    r = file->QueryInfoStatusCode(statusCode);
    //    if (r == FALSE) {
    //        result = QueryHeaderSizeFailure; // throw "QueryInfoStatusCode error";
    //    }
    //    if (result != 0) {
    //        clearHttp(session, connection, file);
    //        return result;
    //    }
    //    CString recvHeaders;
    //    file->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, recvHeaders);
    //    result = processor(HEADERS, utils::Texts::toUtf8(static_cast<LPCTSTR>(recvHeaders)));
    //    if (result != 0) {
    //        clearHttp(session, connection, file);
    //        return result;
    //    }
    //    if (statusCode == HTTP_STATUS_OK) {
    //        size_t readed = 0;
    //        char* buffer = (char*)malloc(64 * 1024 + 1);
    //        while ((readed = file->Read(buffer, 64 * 1024)) > 0) {
    //            result = processor(BODY, std::string(buffer, readed));
    //            if (result != 0) {
    //                clearHttp(session, connection, file);
    //                return result;
    //            }
    //        }
    //        result = processor(FINALLY, std::string());
    //        if (result != 0) {
    //            clearHttp(session, connection, file);
    //            return result;
    //        }
    //    } else {
    //        result = ResponseStatusCodeFailure;
    //        clearHttp(session, connection, file);
    //    }
    //    return result;
    //}

    //int HttpExecutorViaInet(std::wstring const& method, std::wstring const& url, std::wstring const& token, std::vector<std::wstring> const& headers, void* body, int bodyLength, std::function<int(Type type, std::string const& data)>&& processor) {
    //    return HttpExecutorViaInet(method, url, token, headers, new RawBody(body, bodyLength), std::move(processor));
    //}

    int HttpExecutorViaInet(std::wstring const& server, int port, std::wstring const& method, std::wstring const& url, std::wstring const& token, std::vector<std::wstring> const& headers, Body* body, std::function<int(Type type, std::string const& data)>&& processor) {
        int result = 0;
        // 初始化 WinInet 环境
        HINTERNET hInternet = InternetOpenW(agent, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
        // 打开 http session
        HINTERNET hSession = InternetConnectW(hInternet, server.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        // 打开 http post 请求的句柄
        LPCWSTR szAccept[] = { L"*/*", NULL };
        HINTERNET hRequest = HttpOpenRequestW(hSession, method.c_str(), url.c_str(), NULL, NULL, szAccept, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_COOKIES, 0);

        BOOL r = FALSE;
        // 添加 header 信息
        for (auto& header : headers) {
            r = HttpAddRequestHeadersW(hRequest, header.c_str(), -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
            if (r == FALSE) {
                result = HeaderSendFailure; // throw "HEADER error";
            }
        }
        if (!token.empty()) {
            std::wstring s = L"Cookie: sessionId=" + token;
            r = HttpAddRequestHeadersW(hRequest, s.c_str(), -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
            if (r == FALSE) {
                result = HeaderSendFailure; // throw "HEADER error";
            }
        }
        r = HttpAddRequestHeadersW(hRequest, (L"Host: " + server + L":" + std::to_wstring(port) + L"\r\n").c_str(), -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
        if (r == FALSE) {
            result = HeaderSendFailure; // throw "HEADER error";
        }
        r = HttpAddRequestHeadersW(hRequest, (L"Content-Length: " + std::to_wstring(body->GetLength()) + L"\r\n").c_str(), -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
        if (r == FALSE) {
            result = HeaderSendFailure; // throw "HEADER error";
        }
        if (result != 0) {
            clearHttp(hInternet, hSession, hRequest);
            return result;
        }

        if (body && body->SupportPartical()) {
            INTERNET_BUFFERSW BufferIn = { 0 };

            BufferIn.dwStructSize = sizeof(INTERNET_BUFFERSW);
            BufferIn.dwBufferTotal = static_cast<DWORD>(body->GetLength());

            r = HttpSendRequestExW(hRequest, &BufferIn, nullptr, 0, 0);
            if (r == FALSE) {
                result = BodySendFailure; // throw "SEND request error";
            }

            constexpr size_t chunkSize = 8192;
            DWORD written = 0;
            do {
                std::string chunk = body->GetParticalContent(chunkSize);
                r = InternetWriteFile(hRequest, chunk.data(), static_cast<DWORD>(chunkSize), &written);
                if (r == FALSE) {
                    result = BodySendFailure; // throw "SEND body error";
                }
                if (chunk.length() < chunkSize) {
                    break;
                }
            } while (written == chunkSize);
            r = HttpEndRequestW(hRequest, nullptr, 0, 0);
            if (r == FALSE) {
                result = BodySendFailure; // throw "END request error";
            }
        } else {
            if (body->GetContent() == "") {
                r = HttpSendRequestW(hRequest, nullptr, 0, nullptr, 0);
            } else {
                r = HttpSendRequestW(hRequest, nullptr, 0, (void*)body->GetContent().data(), static_cast<DWORD>(body->GetLength()));
            }
            if (r == FALSE) {
                result = BodySendFailure; // throw "SEND request error";
            }
        }

        if (result != 0) {
            clearHttp(hInternet, hSession, hRequest);
            return result;
        }

        DWORD recvSize = 0; // size of the data available

        // 请求 header 的大小，注意这里的 bResult 为 FALSE
        r = HttpQueryInfoW(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, nullptr, &recvSize, nullptr);
        if (r == FALSE) {
            result = QueryHeaderSizeFailure; // throw "QueryInfo for headers size error";
        }

        // 为接收 header 分配内存空间
        char* recvHeaders = new char[recvSize];
        memset(recvHeaders, 0, recvSize);
        // 接收 http response 中的 header
        r = HttpQueryInfoW(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, (LPVOID)recvHeaders, &recvSize, nullptr);
        if (r == FALSE) {
            result = QueryHeaderFailure; // throw "QueryInfo for headers error";
        } else {
            result = processor(HEADERS, std::string(recvHeaders, recvSize));
        }
        delete[] recvHeaders;
        if (result != 0) {
            clearHttp(hInternet, hSession, hRequest);
            return result;
        }
        DWORD readSize = 0; // size of the downloaded data
        // 循环读取数据
        do {
            // 检查在 http response 还有多少字节可以读取
            if (InternetQueryDataAvailable(hRequest, &recvSize, 0, 0)) {
                char* recvData = new char[recvSize]; // buffer for the data
                // 读取数据
                if (!InternetReadFile(hRequest, (LPVOID)recvData, recvSize, &readSize)) {
                    result = ReceiveBodyFailure;
                    delete[] recvData;
                    break;
                } else {
                    if (readSize) {
                        result = processor(BODY, std::string(recvData, readSize));
                    }
                    delete[] recvData;
                }
                if (result != 0) {
                    clearHttp(hInternet, hSession, hRequest);
                    return result;
                }
            } else {
                break;
            }
        } while (readSize != 0);
        result = processor(FINALLY, std::string());
        clearHttp(hInternet, hSession, hRequest);
        return result;
    }

    int HttpExecutorViaInet(std::wstring const& server, int port, std::wstring const& method, std::wstring const& url, std::wstring const& token, std::vector<std::wstring> const& headers, void* body, int bodyLength, std::function<int(Type type, std::string const& data)>&& processor) {
        return HttpExecutorViaInet(server, port, method, url, token, headers, new RawBody(body, bodyLength), std::move(processor));
    }

    int FileReceiver::headersProcess(std::string const& data) {
        return 0;
    }

    int FileReceiver::bodyProcess(std::string const& data) {
        int result = 0;
        if (fwrite(data.data(), 1, data.length(), ((FileBody*)body_)->GetFile()) != data.length()) {
            result = -1;
        }
        return result;
    }

    int FileReceiver::finallyProcess() {
        fflush(((FileBody*)body_)->GetFile());
        return 0;
    }

    int StreamReceiver::headersProcess(std::string const& data) {
        return 0;
    }

    int StreamReceiver::bodyProcess(std::string const& data) {
        int result = 0;
        ((StringBody*)body_)->GetData() += data;
        return result;
    }

    int StreamReceiver::finallyProcess() {
        return 0;
    }
}

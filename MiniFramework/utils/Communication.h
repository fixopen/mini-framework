#include <string>
#include <map>
#include <vector>
#include <memory>
#include <functional>

#include <WinSock2.h> // for SOCKET
#include <IPTypes.h> // for PIP_ADAPTER_INFO

namespace utils::Communications {
    int ProcessNetworkInfo(std::function<int(PIP_ADAPTER_INFO adapterInfo, void *args)>&& process, void *args);

    std::vector <IP_ADAPTER_INFO> GetAdapterInfo();

    struct NetworkInfo {
        char mac[16];
        char ip[16];
        char gateway[16];
        char description[64];
    };

    std::map<int, NetworkInfo> GetNetworkInfo();

    std::wstring const GetMacAddress(int type);

    std::string const DirectGetMacAddress();

    std::string const FormatMac(std::string const &mac, char join = 0);

    std::wstring const FormatMacW(std::string const &mac, char join = 0);

    int ClientExecutor(std::string const &host, std::string const &port,
                       std::function<int(unsigned long long sfd, void *const args)> &&process, void *context);

    struct HttpMessage {
        std::map <std::string, std::string> headers;
        std::string body;
    };

    struct RequestMessage : HttpMessage {
        std::string method;
        std::string path;
    };

    struct ResponseMessage : HttpMessage {
        int statusCode = 0;
        std::string phrase;
    };

    std::shared_ptr<ResponseMessage> parseHeader(std::string const &head);

    std::shared_ptr<ResponseMessage> HttpExecutor(std::shared_ptr<RequestMessage> request);

    struct Body {
        static Body *NullBody;

        virtual size_t GetLength() const = 0;

        virtual std::string GetContent() const = 0;

        virtual bool SupportPartical() const {
            return false;
        }

        virtual std::string GetParticalContent(size_t length) const {
            throw new std::exception("not support");
        }
    };

    struct FileBody : Body {
        FileBody(FILE *file) : Body(), file_(file) {}

        size_t GetLength() const override {
            fseek(file_, 0, SEEK_END);
            return ftell(file_);
        }

        std::string GetContent() const override {
            auto len = static_cast<size_t>(GetLength());
            char *buffer = new char[len];
            memset(buffer, 0, len);
            fseek(file_, 0, SEEK_SET);
            size_t readCount = fread(buffer, sizeof(char), len, file_);
            std::string result = std::string(buffer, readCount);
            delete[] buffer;
            return result;
        }

        bool SupportPartical() const override {
            return true;
        }

        std::string GetParticalContent(size_t len) const override {
            char *buffer = new char[len];
            memset(buffer, 0, len);
            fseek(file_, static_cast<long>(currentPosition_), SEEK_SET);
            fread(buffer, sizeof(char), len, file_);
            std::string result = std::string(buffer, len);
            delete[] buffer;
            currentPosition_ += len;
            return result;
        }

        FILE *GetFile() const {
            return file_;
        }

    private:
        mutable FILE *file_;
        mutable size_t currentPosition_ = 0;
    };

    struct RawBody : Body {
        RawBody(void *content, size_t length) : Body(), content_(content), length_(length) {}

        size_t GetLength() const override {
            return length_;
        }

        std::string GetContent() const override {
            return std::string((char *) content_, length_);
        }

    private:
        void *content_;
        size_t length_;
    };

    struct StringBody : Body {
        StringBody() : Body() {}

        StringBody(std::string &content) : Body(), content_(content) {}

        size_t GetLength() const override {
            return content_.length();
        }

        std::string GetContent() const override {
            return content_;
        }

        std::string &GetData() {
            return content_;
        }

    private:
        std::string content_;
    };

    enum Type {
        HEADERS,
        BODY,
        FINALLY
    };

    enum ErrorCode {
        HeaderSendFailure = 1,
        BodySendFailure = 2,
        QueryHeaderSizeFailure = 3,
        QueryHeaderFailure = 4,
        ReceiveBodyFailure = 5,
        ResponseStatusCodeFailure = 6,
        ProcessHeaderFailure = 7,
        ProcessBodyFailure = 8,
        ProcessFinallyFaiure = 9,
    };

    //int HttpExecutorViaInet(std::wstring const &method, std::wstring const &url, std::wstring const &token,
    //                        std::vector <std::wstring> const &headers, Body *body,
    //                        std::function<int(Type type, std::string const &data)> &&processor);

    int HttpExecutorViaInet(std::wstring const &server, int nPort, std::wstring const &method, std::wstring const &url,
                            std::wstring const &token, std::vector <std::wstring> const &headers, Body *body,
                            std::function<int(Type type, std::string const &data)> &&processor);

    //int HttpExecutorViaInet(std::wstring const &method, std::wstring const &url, std::wstring const &token,
    //                        std::vector <std::wstring> const &headers, void *body, int bodyLength,
    //                        std::function<int(Type type, std::string const &data)> &&processor);

    int HttpExecutorViaInet(std::wstring const &server, int nPort, std::wstring const &method, std::wstring const &url,
                            std::wstring const &token, std::vector <std::wstring> const &headers, void *body,
                            int bodyLength, std::function<int(Type type, std::string const &data)> &&processor);

    struct Receiver {
        Receiver(Body *const body, std::function<void(Body const *const body)> postProcessor)
                : body_(body), postProcessor_(postProcessor) {}

        Receiver(Receiver const &other) : body_(other.body_), postProcessor_(other.postProcessor_) {};

        Receiver(Receiver &&other) = default;

        int operator()(Type type, std::string const &data) {
            int result = 0;
            switch (type) {
                case utils::Communications::HEADERS:
                    result = headersProcess(data);
                    break;
                case utils::Communications::BODY:
                    result = bodyProcess(data);
                    break;
                case utils::Communications::FINALLY:
                    result = finallyProcess();
                    if (result == 0) {
                        postProcessor_(body_);
                    }
                    break;
            }
            return result;
        }

    protected:
        Body *body_;
    private:
        virtual int headersProcess(std::string const &data) = 0;

        virtual int bodyProcess(std::string const &data) = 0;

        virtual int finallyProcess() = 0;

        std::function<void(Body const *const body)> postProcessor_;
    };

    struct FileReceiver : Receiver {
        //auto en = fopen_s(&file, filename.c_str(), "wb");
        //if (file == nullptr) {
        //	result = -1;
        //}
        FileReceiver(FILE *file, std::function<void(Body const *const body)> postProcessor)
                : Receiver(new FileBody(file), postProcessor) {}

        FileReceiver(FileReceiver const &other) : Receiver(other) {};

        FileReceiver(FileReceiver &&other) = default;
        //fclose(file_);
    private:
        int headersProcess(std::string const &data) override;

        int bodyProcess(std::string const &data) override;

        int finallyProcess() override;
        //FILE* file_;
    };

    struct StreamReceiver : Receiver {
        StreamReceiver(std::function<void(Body const *const body)> postProcessor)
                : Receiver(new StringBody(), postProcessor) {}

        StreamReceiver(StreamReceiver const &other) : Receiver(other) {};

        StreamReceiver(StreamReceiver &&other) = default;

    private:
        int headersProcess(std::string const &data) override;

        int bodyProcess(std::string const &data) override;

        int finallyProcess() override;
        //std::string data_;
    };
}

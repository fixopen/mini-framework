#include "Log.h"
#include "Communication.h"

#include <map>
#include <memory>
#include <thread>

namespace utils::Log {
    void log(std::wstring const& content) {
        FILE* l = fopen("log.txt", "a+");
        fwprintf(l, L"%s\n", content.c_str());
        fclose(l);
    }

    void log(std::string const& content) {
        FILE* l = fopen("log.txt", "a+");
        fprintf(l, "%s\n", content.c_str());
        fclose(l);
    }

    namespace Manager {
        namespace {
            // std::vector<Source*> sources;
            Processor* firstNode = nullptr;
            int currentId = 0;
        }

        int Init(std::function<int(Processor* root)> init) {
            firstNode = new Processor(0);
            auto r = new Processor(1);
            firstNode->AddNext(r);
            return init(r);
        }

        int const AddSource(Source* const processor) {
            ++currentId;
            processor->id_ = currentId;
            firstNode->AddNext(processor);
            return currentId;
        }

        Processor* const FindNodeById(Processor* const node, int const id) {
            Processor* result = nullptr;
            if (node->Id() == id) {
                result = node;
            } else {
                for (auto c : node->Next()) {
                    result = FindNodeById(c, id);
                }
            }
            return result;
        }

        int const AddProcess(Processor* const processor, int const previousNodeId) {
            bool find = false;
            for (auto p : firstNode->nexts_) {
                auto n = FindNodeById(p, previousNodeId);
                if (n != nullptr) {
                    ++currentId;
                    processor->id_ = currentId;
                    n->AddNext(processor);
                    find = true;
                    break;
                }
            }
            return currentId;
        }

        void Log(Entry const& entry, int sourceId) {
            auto s = firstNode->Next();
            for (auto i : s) {
                if (i->Id() == sourceId) {
                    i->Process(entry);
                    break;
                }
            }
        }

        int PutLog(char* ahost, int port, char* buffer) {
            Init([](Processor* root) {
                Processor* contentDecorate = new Processor();
                auto dId = AddProcess(contentDecorate, root->Id());
                Sink* network = new Sink();
                network->SetDestination(new NetworkDestination("host", 80));
                network->SetFormatter(new Formatter());
                AddProcess(network, dId);
                return dId;
            });

            std::shared_ptr<Communications::RequestMessage> request = std::make_shared<Communications::RequestMessage>();
            request->method = "GET";
            request->path = "/server/apps/touchbook2?cmd=statis_bat&s=" + std::string(buffer) + "&mid=XYCESHI0001&v=xy1&crc=ecc2860d0a97f285f1ececdbd03b18cb";
            std::string host = ahost;
            if (port != 80) {
                host += ":" + std::to_string(port);
            }
            std::string boundary = "----------------------------64b23e4066ed";
            request->headers.insert(std::make_pair("Host", host));
            request->headers.insert(std::make_pair("Content-Type", "text/html; charset=utf-8"));
            request->headers.insert(std::make_pair("Accept", "application/json"));
            request->headers.insert(std::make_pair("Content-Type", "multipart/form-data; charset=\"gb2312\"; boundary=" + boundary + "\r\n"));

            std::string entityHeaders = boundary + "\r\n";
            entityHeaders += "Content-Disposition: form-data; name=\"file\"; filename=\"大论文和实验材料.rar\"\r\n";
            entityHeaders += "Content-Type: aapplication/octet-stream\r\n\r\n";

            //post entity尾
            std::string endContent("\r\n" + boundary + "\r\n");
            char temp[64] = { 0 };
            //注意下面这个参数Content-Length，这个参数值是：http请求头长度+文件总长度+请求尾长度
            auto fileLength = 0;
            request->headers.insert(std::make_pair("Content-Length", std::to_string(entityHeaders.length() + fileLength + endContent.length())));

            auto response = Communications::HttpExecutor(request);
            // std::shared_ptr<ResponseMessage> HttpExecutor(std::shared_ptr<RequestMessage> request);
            return 0;
        }

        void WriteLog(std::wstring const& content) {
            unsigned int result = 0;
            std::wstring* copy = new std::wstring(content);
            std::thread([&copy, &result] {
                std::wstring* data = (std::wstring*)copy;
                std::string native = Texts::toNative(*data);
                // type: app|read
                // name:
                // detailid:
                // booknum:
                // title:
                // cataid:
                // deviceid:
                // datetime:

                // total:
                // infos: []

                // "book.jieyueji.cn", 80
                // File
                result = 0;
            }).detach();
        }
    }

    Entry const Source::trigger() {
        Entry entry = parser_->Parse(receiver_->Read());
        return Process(entry);
    }

    Source::~Source() {
        delete receiver_;
        delete parser_;
    }

    Sink::~Sink() {
        delete formatter_;
        delete destination_;
    }

    Entry const Sink::selfProcess(Entry const& entry) {
        destination_->Write(formatter_->Format(entry));
        return entry;
    }

    bool const Filter::IsAccept(Entry const& entry) {
        return IsLevelAccept(entry);
    }

    std::string const Receiver::Read() {
        std::string result;
        //read from FILE*, fd, socket, Handle, and so on...
        return result;
    }

    NetworkReceiver::NetworkReceiver(std::string const& ip, unsigned short const port) {
        socket_ = 0;
    }

    std::string const NetworkReceiver::Read() {
        std::string result;
        //read from socket_
        return result;
    }

    std::string const Formatter::Format(Entry const& entry) {
        std::string result = entry.what;
        //convert to string
        return result;
    }

    std::string const SQLFormatter::Format(Entry const& entry) {
        std::string result;
        //convert to sql insert command
        return result;
    }

    Entry const Parser::Parse(std::string const& value) {
        Entry result(value);
        //parse from string
        return result;
    }

    Entry const SQLParser::Parse(std::string const& sql) {
        Entry result(sql);
        //parse from sql insert command
        return result;
    }

    void Destination::Write(std::string const& value) {
        //write string to destination, include FILE*, fd, socket, Handle, and so on...
    }

    FileDestination::FileDestination(std::string const& filename) : file_(0), isConsole_(false) {
        if (filename != "") {
            //file_ = fopen(filename.c_str(), "w+");
            fopen_s(&file_, filename.c_str(), "w+");
        } else {
            isConsole_ = true;
            file_ = stdout;
        }
    }

    FileDestination::~FileDestination() {
        if (!isConsole_) {
            fclose(file_);
        }
    }

    void FileDestination::Write(std::string const& value) {
        fprintf(file_, "%s\n", value.c_str());
        fflush(file_);
    }

    void TraceDestination::Write(std::string const& value) {
        //TRACE(Texts::fromUtf8(value).c_str());
        (void)Texts::fromUtf8(value).c_str();
    }

    void UIDestination::Write(std::string const& value) {
        //::AfxMessageBox(Texts::fromUtf8(value).c_str());
        ::MessageBoxW(nullptr, Texts::fromUtf8(value).c_str(), L"Log", 0);
    }

    MemoryDestination::MemoryDestination(size_t count) {
        //limit
    }

    void MemoryDestination::Write(std::string const& value) {
        contents_.push_back(value);
    }

    NetworkDestination::NetworkDestination(std::string const& ip, unsigned short const port) {
        socket_ = 0; //socket();
        //...
    }

    void NetworkDestination::Write(std::string const& value) {
        //send(socket_, value.c_str(), value.length(), 0, 0);
    }

    DatabaseDestination::DatabaseDestination(std::string const& databaseName, std::string const& tableName) {
        database_ = 0;
    }

    void DatabaseDestination::Write(std::string const& value) {
        //writeToDb(database_, ...);
    }
}

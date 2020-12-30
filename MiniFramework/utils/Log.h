#ifndef __UTIL_LOG_H__
#define __UTIL_LOG_H__

#include <vector>
#include <string>
#include <functional>
#include <cstdarg>
#include "Texts.h"

namespace utils::Log {
    void log(std::wstring const& content);
    void log(std::string const& content);

    struct Entry; // log content
    class Formatter; // log content serialize to Sink
        class SQLFormatter;
    class Parser; // log content unserialize from Source
        class SQLParser;
    class Processor; // abstract node
        class Source;
        class Sink;
        class Filter;
        //class Forker;
        //class Decorate;
            class TimeDecorate;
    class Receiver; // append Parser, used by Source
        class NetworkReceiver;
    class Destination; // append Formatter, used by Sink
        class FileDestination;
        class TraceDestination;
        class UIDestination;
        class MemoryDestination;
        class NetworkDestination;
        class DatabaseDestination;

    namespace Manager {
        int const AddSource(Source* const processor);
        int const AddProcess(Processor* const processor, int const previousNodeId);
        int Init(std::function<int(Processor* root)> init);
        void Log(Entry const& entry, int sourceId = 1);
    }

    struct Entry {
        enum Level {
            lFatalError,
            lError,
            lWarning,
            lNotice,
            lInfo,
        };

        Entry(std::string const& w, ...)
        : level(lNotice) {
            std::string r = w;
            char const* format = r.c_str();
            char buffer[4096] = {0};
            va_list ap;
            va_start(ap, format);
            int len = vsprintf_s(buffer, 4096, format, ap);
            //@@va_end(ap);
            r = buffer;
            what = r;
        }

        Entry(Level const lvl, std::string const& w, ...)
        : level(lvl) {
            std::string r = w;
            char const* format = r.c_str();
            char buffer[4096] = {0};
            va_list ap;
            va_start(ap, format);
            int len = vsprintf_s(buffer, 4096, format, ap);
            va_end(ap);
            r = buffer;
            what = r;
        }

        Entry(std::wstring const& w)
        : level(lNotice)
        , what(Texts::toUtf8(w)) {
        }

        Entry(char const* const w)
        : level(lNotice)
        , what(w) {
        }

        Entry(wchar_t const* const w)
        : level(lNotice)
        , what(Texts::toUtf8(w)) {
        }

        Level level;
        std::string what;
    };
    class Processor {
        friend int const Manager::AddSource(Source* const processor);
        friend int const Manager::AddProcess(Processor* const processor, int const previousNodeId);
    public:
        Processor() = default;
        Processor(int id) : id_(id) {}
        int Id() {
            return id_;
        }
        std::vector<Processor*>& Next() {
            return nexts_;
        }
        Entry const Process(Entry const& entry) {
            Entry e = selfProcess(entry);
            for (size_t i = 0; i < nexts_.size(); ++i) {
                nexts_[i]->Process(e);
            }
            return e;
        }
        void AddNext(Processor* const next) {
            nexts_.push_back(next);
        }
        virtual ~Processor() {}
    protected:
        int id_;
        std::vector<Processor*> nexts_;
    private:
        virtual Entry const selfProcess(Entry const& entry) { return entry; }
    };
    class Source : public Processor {
    public:
        void SetParser(Parser* const parser) {
            parser_ = parser;
        }
        void SetReceiver(Receiver* const receiver) {
            receiver_ = receiver;
        }
        Entry const trigger();
        virtual ~Source();
    private:
        Entry const selfProcess(Entry const& entry) override {
            return entry;
        }
        Parser* parser_;
        Receiver* receiver_;
    };
    class Sink : public Processor {
    public:
        void SetFormatter(Formatter* const formatter) {
            formatter_ = formatter;
        }
        void SetDestination(Destination* const destination) {
            destination_ = destination;
        }
        virtual ~Sink();
    private:
        Entry const selfProcess(Entry const& entry) override;
        Formatter* formatter_;
        Destination* destination_;
    };
    class Filter : public Processor {
    public:
        virtual bool const IsAccept(Entry const& entry);
        void SetLevel(Entry::Level const level) {
            level_ = level;
        }
        virtual ~Filter() {}
    protected:
        bool const IsLevelAccept(Entry const& entry) {
            bool result = false;
            if (entry.level < level_) {
                result = true;
            }
            return result;
        }
    private:
        Entry::Level level_;
    };
    //class Forker : public Processor { //same as processor
    //public:
    //    virtual ~Forker();
    //};
    class Receiver {
    public:
        virtual ~Receiver() {}
        virtual std::string const Read();
    };
    class NetworkReceiver : public Receiver {
    public:
        NetworkReceiver(std::string const& ip, unsigned short const port);
        std::string const Read() override;
    private:
        typedef int Socket;
        Socket socket_;
    };
    class Formatter {
    public:
        virtual std::string const Format(Entry const& entry);
    };
    class SQLFormatter : public Formatter {
    public:
        std::string const Format(Entry const& entry) override;
    };
    class Parser {
    public:
        virtual Entry const Parse(std::string const& value);
    };
    class SQLParser : public Parser {
    public:
        Entry const Parse(std::string const& sql) override;
    };
    class Destination {
    public:
        virtual void Write(std::string const& value);
    };
    class FileDestination : public Destination {
    public:
        FileDestination(std::string const& filename = "");
        ~FileDestination();
        void Write(std::string const& value) override;
    private:
        FILE* file_;
        bool isConsole_;
    };
    class TraceDestination : public Destination {
    public:
        void Write(std::string const& value) override;
    };
    class UIDestination : public Destination {
    public:
        void Write(std::string const& value) override;
    };
    class MemoryDestination : public Destination {
    public:
        MemoryDestination(size_t count);
        void Write(std::string const& value) override;
    private:
        std::vector<std::string> contents_;
    };
    class NetworkDestination : public Destination {
    public:
        NetworkDestination(std::string const& ip, unsigned short const port);
        void Write(std::string const& value) override;
    private:
        typedef int Socket;
        Socket* socket_;
    };
    class DatabaseDestination : public Destination {
    public:
        DatabaseDestination(std::string const& databaseName, std::string const& tableName);
        void Write(std::string const& value) override;
    private:
        typedef int DB;
        DB* database_;
    };
}

#endif //__UTIL_LOG_H__

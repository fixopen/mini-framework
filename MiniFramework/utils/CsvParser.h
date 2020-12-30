#pragma once

#include <vector>
#include <string>

namespace utils {
    class CsvParser {
        enum State {
            sFieldEnd,
            sField,
            sQuoteField,
            sQuoteFieldQuote,
            sCR,
            sError
        };
    public:
        CsvParser(std::wstring const& content, bool hasHeader) {
            parse(content);
        }
        template<typename InputIterator>
        CsvParser(InputIterator first, InputIterator last) {
            parse(first, last);
        }
        ~CsvParser() = default;
        std::vector<std::vector<std::wstring>> const& data() const {
            return data_;
        }
    protected:
        void parse(std::wstring const& content) {
            parse(content.begin(), content.end());
        }
        template<typename InputIterator>
        void parse(InputIterator first, InputIterator last) {
            for (auto i = first; i != last; ++i) {
                switch (state_) {
                case sFieldEnd:
                    switch (*i) {
                    case QUOTE:
                        state_ = sQuoteField;
                        break;
                    case COMMA:
                        record_.push_back(L"");
                        start_ = last;
                        state_ = sFieldEnd;
                        break;
                    case CR:
                    case LF:
                        state_ = sError;
                        break;
                    default:
                        start_ = i;
                        state_ = sField;
                        break;
                    }
                    break;
                case sField:
                    switch (*i) {
                    case QUOTE:
                        start_ = i;
                        break;
                    case COMMA:
                        record_.push_back(std::wstring(start_, i));
                        start_ = last;
                        state_ = sFieldEnd;
                        break;
                    case CR:
                    case LF:
                        record_.push_back(std::wstring(start_, i));
                        data_.push_back(record_);
                        record_.clear();
                        start_ = last;
                        state_ = sFieldEnd;
                        break;
                    default:
                        break;
                    }
                    break;
                case sQuoteField:
                    switch (*i) {
                    case QUOTE:
                        state_ = sQuoteFieldQuote;
                        break;
                    case COMMA:
                        break;
                    case CR:
                    case LF:
                        break;
                    default:
                        break;
                    }
                    break;
                case sQuoteFieldQuote:
                    switch (*i) {
                    case QUOTE:
                        part_ += std::wstring(start_, i - 1);
                        start_ = i + 1;
                        state_ = sQuoteField;
                        break;
                    case COMMA:
                        record_.push_back(part_ + std::wstring(start_, i - 1));
                        start_ = last;
                        state_ = sFieldEnd;
                        break;
                    case CR:
                    case LF:
                        record_.push_back(part_ + std::wstring(start_, i - 1));
                        data_.push_back(record_);
                        record_.clear();
                        start_ = last;
                        state_ = sFieldEnd;
                        break;
                    default:
                        state_ = sError;
                        break;
                    }
                    break;
                case sError:
                    break;
                }
            }
        }
    private:
        State state_ = sFieldEnd;
        std::vector<std::vector<std::wstring>> data_;
        std::vector<std::wstring> record_;
        std::wstring part_;
        std::wstring::const_iterator start_;
        static const wchar_t CR = 0x0D;
        static const wchar_t LF = 0x0A;
        static const wchar_t COMMA = 0x2C;
        static const wchar_t QUOTE = 0x22;
    };
}

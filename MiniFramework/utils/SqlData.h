#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <optional>
#include "../nlohmann/json.hpp"

namespace utils::SqlData {
    void Init(std::wstring const& dbFilename);
    void Finally();
    bool IsOk();
    std::wstring const LastError();
    void ExecuteNonQuery(std::wstring const& command);

    struct DataTable {
        DataTable() = default;
        DataTable(std::wstring const& n, std::vector<std::wstring> const& cns, std::wstring const& other = L"");
        std::optional<std::wstring> const& getValue(std::vector<std::optional<std::wstring>> const& row, size_t columnIndex) const;
        std::optional<std::wstring> const& getValue(std::vector<std::optional<std::wstring>> const& row, std::wstring const& columnName) const;
        void setValue(std::vector<std::optional<std::wstring>>& row, size_t columnIndex, std::wstring const& value);
        void setValue(std::vector< std::optional<std::wstring>>& row, std::wstring const& columnName, std::wstring const& value);
        void sync(long id, std::vector< std::optional<std::wstring>> const& row);
        void insert(long id, std::vector< std::optional<std::wstring>>& row);
        void batchInsert(std::vector<std::vector<std::optional<std::wstring>>>& rows);
        void sync(std::function<bool(std::wstring const& tableName, std::vector<std::optional<std::wstring>>& row)>&& isNew);
        std::optional<std::vector<std::optional<std::wstring>>> selectOne(std::function<bool(std::vector<std::optional<std::wstring>> const& row)>&& accept) const;
        std::vector<std::vector<std::optional<std::wstring>>> select(std::function<bool(std::vector<std::optional<std::wstring>> const& row)>&& accept) const;
        size_t getIndexByName(std::wstring const& name) const;
        size_t count(std::function<bool(std::vector<std::optional<std::wstring>> const& row)>&& accept) const;
        size_t remove(std::function<bool(std::vector<std::optional<std::wstring>>& row)>&& accept);
        std::map<std::wstring, size_t> getColumnIndex() const;
        void findMaxId();

        std::wstring name;
        std::vector<std::wstring> columnNames;
        std::vector<std::vector<std::optional<std::wstring>>> rows;
        long maxId = -1;
        std::map<std::wstring, size_t> columnIndex;

        size_t getIndexByName(std::wstring const& name) {
            size_t result = -1;
            auto const& it = columnIndex.find(name);
            if (it != columnIndex.end()) {
                result = it->second;
            }
            return result;
        }
        std::optional<std::wstring> const& getId(std::vector<std::optional<std::wstring>> const& row) const {
            return getValue(row, L"_id");
        }
        long const getIdValue(std::vector<std::optional<std::wstring>> const& row) const {
            auto id = getId(row);
            return std::stol(id.has_value() ? id.value() : L"0");
        }
        long getMaxId() {
            return maxId++;
        }
        void sync(std::vector<std::optional<std::wstring>> const& row) {
            sync(getIdValue(row), row);
        }
        size_t remove(std::wstring const& whereCause, std::function<bool(std::vector<std::optional<std::wstring>>& row)>&& accept) {
            std::wstring removeCommand = L"DELETE FROM " + name + L" WHERE " + whereCause + L";";
            ExecuteNonQuery(removeCommand);
            return remove(std::move(accept));
        }
        void insert(std::vector<std::optional<std::wstring>>& row) {
            insert(getMaxId(), row);
        }
    private:
        std::wstring const columnList() const;
        std::wstring const valueList(std::vector<std::optional<std::wstring>> const& row) const;
        std::optional<std::wstring> value_;
    };

    std::wstring const ExecuteScalar(std::wstring const& query);
    std::vector<std::vector<std::optional<std::wstring>>> const ExecuteQueryTable(std::wstring const& query);
    bool const IsTableExist(std::wstring const& tableName);
    inline std::wstring const quote(std::wstring const& value) {
        return L"'" + value + L"'";
    }
    DataTable const ParseCSV(std::wstring const& filename);

    struct Setting {
        static DataTable data;

        static std::vector<Setting> select(std::function<bool(std::vector<std::optional<std::wstring>> const& row)>&& accept);
        static Setting fromRow(std::vector<std::optional<std::wstring>> const& row);

        int id = 0;
        std::wstring name; // active-state, active-code, device-no, {software-version | machine-no}, protected, password, admin-password, show-english, background-music, update-site, last-check-time, main-version, local-version, video-version, class-version, tool-version, resource-version
        std::wstring value; // {-1 | 0 | 1 | 2}, , 00E04C68004D, 25A3416AA0F840B7A0D7CADCB1CFF81C, {0 | 1}, chaoxing1993, chaoxing1993, 1, 0, update.kid.chaoxing.com, 20190305T123813Z, 2.2.0, 1.92, 1.0, 1.2, 1.0, 2019011011100000

        std::vector<std::optional<std::wstring>> toRow() const;
    };

    enum OpenerTypes {
        Unknown, // unused
        Self, // open by this application, is form name
        Show, // open by this application, is directory
        Categories, // Category container
        Category, // open by this application, is filter, auto append ALL & RECENTS category
        Application, // self execute, is executable file
        Document, // open by office, is data file
        Media, // open by media player, is file
        Browser, // is relative url
        ExternalBrowser, // open by system browser, is absolute url
        MultimediaBook, // open by local(book) application.
    };

    struct Item {
        static DataTable data;

        static std::vector<Item> select(std::function<bool(std::vector<std::optional<std::wstring>> const& row)>&& accept);
        static Item fromRow(std::vector<std::optional<std::wstring>> const& row);

        int id = 0;
        std::wstring name;
        int parentId = 0;
        OpenerTypes type = Unknown;
        int order = 0;
        int state = 0;
        std::wstring drm;
        std::wstring icon;
        std::wstring args;
        std::wstring description;

        std::vector<std::optional<std::wstring>> toRow() const;
    };

    struct Recent {
        static DataTable data;

        static std::vector<Recent> select(std::function<bool(std::vector<std::optional<std::wstring>> const& row)>&& accept);
        static Recent fromRow(std::vector<std::optional<std::wstring>> const& row);

        int id = 0;
        int itemId = 0; // the show file item id
        int parentId = 0; // the show<Category> id
        std::wstring lastTime;

        std::vector<std::optional<std::wstring>> toRow() const;
    };

    // user auth
    // import : csv parse -> db
    // export : db -> csv
    // file copy
    // file destroy
    // zip & unzip
    // send & receiver : beast
    // online upgrade : meta json(xml) parse compare download process kill override file restart
    // log : log to server
    // digest : md5
    // machine access
    struct ColumnMeta {
        enum Type {
            UNKNOWN,
            TEXT,
            INTEGER,
            REAL,
            BLOB
        };
        std::wstring name;
        Type type;
    };

    std::vector<ColumnMeta> getColumns(std::wstring const& tableName);

    inline std::string jsonEscape(std::string const& v) {
        std::string result;
        for (auto chr : v) {
            result += chr == '\\' ? "\\\\" : (chr == '\"' ? "\\\"" : (chr == '\n' ? "\\n": std::string() + chr));
        }
        return result;
    }

    inline std::wstring jsonEscape(std::wstring const& v) {
        std::wstring result;
        for (auto chr : v) {
            result += chr == L'\\' ? L"\\\\" : (chr == L'\"' ? L"\\\"" : (chr == L'\n' ? L"\\n" : std::wstring() + chr));
        }
        return result;
    }

    inline std::wstring jsonQuote(std::wstring const& v) {
        return L"\"" + v + L"\"";
    }

    inline std::wstring sqlQuote(std::wstring const& v) {
        return L"\'" + v + L"\'";
    }

    std::wstring toJson(std::vector<ColumnMeta> const& columns, std::vector<std::optional<std::wstring>> const& row);

    // inline std::wstring toJson(std::vector<ColumnMeta> const& columns, std::vector<std::optional<std::wstring>> const& row) {
    //     // use nlohmann::json to convert
    //     std::wstring result = L"{";
    //     size_t columnCount = columns.size();
    //     std::vector<std::wstring> items(columnCount);
    //     int difference = 0;
    //     for (size_t i = 0; i < columnCount; ++i) {
    //         auto& cell = row[i - difference];
    //         switch (columns[i].type) {
    //         case ColumnMeta::TEXT:
    //             items[i] = jsonQuote(columns[i].name) + L": " + (cell.has_value() ? jsonQuote(jsonEscape(cell.value())) : L"null");
    //             break;
    //         case ColumnMeta::INTEGER:
    //             items[i] = jsonQuote(columns[i].name) + L": " + (cell.has_value() ? jsonEscape(cell.value()) : L"null");
    //             break;
    //         case ColumnMeta::BLOB:
    //             difference++;
    //             items[i] = L"";
    //             break;
    //         }
    //     }
    //     for (size_t i = 0; i < columnCount; ++i) {
    //         if (items[i] != L"") {
    //             if (i == 0) [[unlikely]] {
    //                 // do nothing
    //             } else [[likely]] {
    //                 result += L", ";
    //             }
    //             result += items[i];
    //         }
    //     }
    //     result += L"}";
    //     return result; // LR"json({})json";
    // }

    inline std::wstring toJson(std::vector<ColumnMeta> const& columns, std::vector<std::vector<std::optional<std::wstring>>> const& rows) {
        // use nlohmann::json to convert
        std::wstring res = L"[";
        bool first = true;
        for (auto& row : rows) {
            if (first) {
                first = false;
            } else {
                res += L", ";
            }
            res += toJson(columns, row);
        }
        res += L"]";
        return res;
    }

    std::wstring constructColumnList(std::vector<ColumnMeta> const& columns);

    std::string readBlob(std::wstring const& tableName, int id, std::wstring const& columnName);

    void fillBlob(std::wstring const& tableName, int id, std::wstring const& columnName, void const* data, int size);

    std::wstring query(std::wstring const& tableName, std::wstring const& whereCause, std::wstring const& orderBy);
    int insert(std::wstring const& tableName, nlohmann::json const& data);
    void update(std::wstring const& tableName, nlohmann::json const& data);
    void remove(std::wstring const& tableName, int id);

    long getLastId();

    int backup(std::wstring const& name);
    int restore(std::wstring const& name);

    void constructTestData();
}

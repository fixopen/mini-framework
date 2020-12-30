#include "SqlData.h"

#include "Texts.h"
#include "Folders.h"
#include "Log.h"

#include "db/sqlite3.h"
#include <assert.h>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <iostream>
#include <vector>

namespace utils::SqlData {
    DataTable::DataTable(std::wstring const& n, std::vector<std::wstring> const& cns, std::wstring const& other) : name(n), columnNames(cns) {
        rows = ExecuteQueryTable(L"SELECT " + columnList() + L" FROM " + name + L" " + other + L";");
        columnIndex = getColumnIndex();
    }

    std::optional<std::wstring> const& DataTable::getValue(std::vector<std::optional<std::wstring>> const& row, size_t columnIndex) const {
        if (columnIndex >= 0 && columnIndex < row.size()) {
            return row[columnIndex];
        } else {
            return value_;
        }
    }

    std::optional<std::wstring> const& DataTable::getValue(std::vector<std::optional<std::wstring>> const& row, std::wstring const& columnName) const {
        return getValue(row, getIndexByName(columnName));
    }

    void DataTable::setValue(std::vector<std::optional<std::wstring>>& row, size_t columnIndex, std::wstring const& value) {
        if (columnIndex >= 0 && columnIndex < row.size()) {
            row[columnIndex] = value;
        }
    }

    void DataTable::setValue(std::vector<std::optional<std::wstring>>& row, std::wstring const& columnName, std::wstring const& value) {
        setValue(row, getIndexByName(columnName), value);
    }

    void DataTable::sync(long id, std::vector<std::optional<std::wstring>> const& row) {
        std::wstring command = L"UPDATE " + name + L" SET ";
        auto count = row.size();
        for (auto i = 0u; i < count; ++i) {
            auto cell = row[i];
            command += columnNames[i] + L" = '" + (cell.has_value() ? cell.value() : L"NULL") + L"'";
            if (i != count - 1) {
                command += L", \n";
            }
        }
        command += L" WHERE id = " + std::to_wstring(id);
        ExecuteNonQuery(command);
    }

    void DataTable::insert(long id, std::vector<std::optional<std::wstring>>& row) {
        if (id != -1) {
            auto idIndex = getIndexByName(L"id");
            row[idIndex] = std::to_wstring(id);
        }
        std::wstring command = L"INSERT INTO " + name + L" ( " + columnList() + L" ) VALUES ( " + valueList(row) + L" );\n";
        ExecuteNonQuery(command);
    }

    void DataTable::batchInsert(std::vector<std::vector<std::optional<std::wstring>>>& rows) {
        auto idIndex = getIndexByName(L"id");
        findMaxId();
        std::wstring co = L"INSERT INTO " + name + L" (" + columnList() + L") VALUES (";
        for (auto r : rows) {
            auto id = getMaxId();
            r[idIndex] = std::to_wstring(id);
            std::wstring insert = co + valueList(r) + L");\n";
            ExecuteNonQuery(insert);
        }
    }

    void DataTable::sync(std::function<bool(std::wstring const& tableName, std::vector<std::optional<std::wstring>>& row)>&& isNew) {
        for (auto& row : rows) {
            if (isNew(name, row)) {
                insert(-1, row);
            } else {
                auto id = getValue(row, L"id");
                sync(std::stol(id.has_value() ? id.value() : L"0"), row);
            }
        }
    }

    std::optional<std::vector<std::optional<std::wstring>>> DataTable::selectOne(std::function<bool(std::vector<std::optional<std::wstring>> const& row)>&& accept) const {
        std::optional<std::vector<std::optional<std::wstring>>> result;
        for (auto& row : rows) {
            if (accept(row)) {
                result = row;
                break;
            }
        }
        return result;
    }

    std::vector<std::vector<std::optional<std::wstring>>> DataTable::select(std::function<bool(std::vector<std::optional<std::wstring>> const& row)>&& accept) const {
        std::vector<std::vector<std::optional<std::wstring>>> result;
        for (auto& row : rows) {
            if (accept(row)) {
                result.push_back(row);
            }
        }
        return result;
    }

    size_t DataTable::getIndexByName(std::wstring const& name) const {
        auto result = -1;
        for (auto& n : columnNames) {
            result++;
            if (n == name) {
                break;
            }
        }
        return result;
    }

    size_t DataTable::count(std::function<bool(std::vector<std::optional<std::wstring>> const& row)>&& accept) const {
        size_t result = 0;
        for (auto& r : rows) {
            if (accept(r)) {
                ++result;
            }
        }
        return result;
    }

    size_t DataTable::remove(std::function<bool(std::vector<std::optional<std::wstring>>& row)>&& accept) {
        size_t result = 0;
        auto const& endIter = rows.end();
        for (auto i = rows.begin(); i != endIter; ++i) {
            if (accept(*i)) {
                rows.erase(i++);
                ++result;
            }
        }
        return result;
    }

    std::map<std::wstring, size_t> DataTable::getColumnIndex() const {
        std::map<std::wstring, size_t> result;
        auto count = columnNames.size();
        for (auto i = 0u; i < count; ++i) {
            result.insert(std::make_pair(columnNames[i], i));
        }
        return result;
    }

    void DataTable::findMaxId() {
        auto idIndex = getIndexByName(L"id");
        for (auto& r : rows) {
            auto id = getValue(r, idIndex);
            auto nowId = std::stol(id.has_value() ? id.value() : L"0");
            if (nowId > maxId) {
                maxId = nowId + 1;
            }
        }
    }

    std::wstring const DataTable::columnList() const {
        std::wstring result;
        bool isRest = false;
        for (auto& c : columnNames) {
            if (isRest) {
                result += L", ";
            } else {
                isRest = true;
            }
            result += c;
        }
        return result;
    }

    std::wstring const DataTable::valueList(std::vector<std::optional<std::wstring>> const& row) const {
        std::wstring result;
        bool isRest = false;
        for (auto& c : row) {
            if (isRest) {
                result += L", \n";
            } else {
                isRest = true;
            }
            result += L"'" + (c.has_value() ? c.value() : L"NULL") + L"'";
        }
        return result;
    }

    namespace {
        auto runFolder = utils::Folders::GetCurrentPath();
        int r = SQLITE_OK;
        sqlite3* db = nullptr;

        void prepareTables(sqlite3* db) {
            std::wstring ddl = L"CREATE TABLE IF NOT EXISTS settings (id INTEGER PRIMARY KEY NOT NULL, name TEXT, value TEXT) WITHOUT ROWID;\n";
            ExecuteNonQuery(ddl);
            ddl = L"CREATE TABLE IF NOT EXISTS items (id INTEGER PRIMARY KEY NOT NULL, name TEXT, parent_id INTEGER, type INTEGER, \"order\" INTEGER, state INTEGER, drm TEXT, icon TEXT, args TEXT, description TEXT) WITHOUT ROWID;\n";
            ExecuteNonQuery(ddl);
            ddl = L"CREATE TABLE IF NOT EXISTS recents (id INTEGER PRIMARY KEY NOT NULL, item_id INTEGER, parent_id INTEGER, last_time TEXT) WITHOUT ROWID;\n";
            ExecuteNonQuery(ddl);
            // active-state, active-code, device-no, {software-version | machine-no}, protected, password, admin-password, show-english, background-music, update-site, last-check-time, main-version, local-version, video-version, class-version, tool-version, resource-version
            // {-1 | 0 | 1 | 2}, , 00E04C68004D, 25A3416AA0F840B7A0D7CADCB1CFF81C, {0 | 1}, chaoxing1993, chaoxing1993, 1, 0, update.kid.chaoxing.com, 20190305T123813Z, 2.2.0, 1.92, 1.0, 1.2, 1.0, 2019011011100000
            std::wstring dml = L"INSERT INTO settings (id, name, value) VALUES (1, 'active-state', '2'), (2, 'device-no', '00E04C68004D'), (3, 'machine-no', '25A3416AA0F840B7A0D7CADCB1CFF81C'), (4, 'protected', '0'), (5, 'password', 'chaoxing1993'), (6, 'admin-password', 'chaoxing1993');\n";
            ExecuteNonQuery(dml);
            dml = L"INSERT INTO items (id, name, parent_id, type, \"order\", state, drm, icon, args, description) VALUES \n"
                  "(1, '幼儿资源', 0, 1, 1, 0, null, 'static/br.jpg', null, null), \n"
                  "(2, '教学工具', 0, 1, 2, 0, null, 'static/tt.jpg', null, null), \n"
                  "(3, '教师资源', 0, 1, 3, 0, null, 'static/tr.jpg', null, null), \n"
                  "(4, '绘本阅读', 1, 3, 1, 0, null, 'static/multimedia.png', 'D:\\happychildren\\book\\', null), \n"
                  "(5, '儿童歌谣', 1, 8, 2, 0, null, 'static/music.jpg', 'http://localhost/book/erge/index.html', null), \n"
                  "(6, '动画绘本', 1, 8, 3, 0, null, 'static/web.png', 'http://localhost/book/flashbook/index.html', null), \n"
                  "(7, '国学诵读', 1, 8, 4, 0, null, 'static/classical.png', 'http://127.0.0.1/book/poembook/index.html', null), \n"
                  "(8, '安全教育', 1, 8, 5, 0, null, 'static/safe.png', 'http://localhost/book/seaqsp/index.html', null), \n"
                  "(9, '古诗欣赏', 1, 8, 6, 0, null, 'static/poetry.png', 'http://127.0.0.1/book/古诗/gushiquanbu.html', null), \n"
                  "(10, '益智游戏', 1, 8, 7, 0, null, 'static/game.png', 'http://127.0.0.1/book/益智游戏/index.html', null), \n"
                  "(11, '投屏', 2, 9, 1, 0, null, 'static/project.png', 'http://x.chaoxing.com', null), \n"
                  "(12, '电子白板', 2, 5, 2, 0, null, 'static/whiteboard.png', 'D:\\happychildren\\Whiteboard\\Software\\Board.exe', null), \n"
                  "(13, '自有课件', 2, 2, 3, 0, null, 'static/owncourseware.png', 'D:\\happychildren\\myshelf\\', null), \n"
                  "(14, '期刊杂志', 3, 8, 1, 0, null, 'static/magazine.png', 'http://localhost/book/magazines/index.html', null), \n"
                  "(15, '数字图书', 3, 8, 2, 0, null, 'static/book.png', 'http://localhost/book/borrbooks/index.html', null), \n"
                  "(16, '科学', 4, 4, 1, 0, null, null, null, null), \n"
                  "(17, '小脚印', 4, 4, 2, 0, null, null, null, null), \n"
                  "(18, '会飞的幼儿园', 4, 4, 3, 0, null, null, null, null), \n"
                  "(19, '英语', 4, 4, 4, 1, null, null, null, null), \n"
                  "(20, 'My First Phonics 1', 19, 4, 1, 0, null, '/happychildren/books/My First Phonics 1/cover/My First Phonics 1.jpg', '/happychildren/books/My First Phonics 1/', null), \n"
                  "(21, 'My First Phonics 2', 19, 4, 2, 0, null, '/happychildren/books/My First Phonics 2/cover/My First Phonics 2.jpg', '/happychildren/books/My First Phonics 2/', null), \n"
                  "(22, 'My First Phonics 3', 19, 4, 3, 0, null, '/happychildren/books/My First Phonics 3/cover/My First Phonics 3.jpg', '/happychildren/books/My First Phonics 3/', null), \n"
                  "(23, 'My First Phonics 4', 19, 4, 4, 0, null, '/happychildren/books/My First Phonics 4/cover/My First Phonics 4.jpg', '/happychildren/books/My First Phonics 4/', null), \n"
                  "(24, 'My First Phonics 5', 19, 4, 5, 0, null, '/happychildren/books/My First Phonics 5/cover/My First Phonics 5.jpg', '/happychildren/books/My First Phonics 5/', null), \n"
                  "(25, 'My First Phonics 6', 19, 4, 6, 0, null, '/happychildren/books/My First Phonics 6/cover/My First Phonics 6.jpg', '/happychildren/books/My First Phonics 6/', null), \n"
                  "(26, 'My First Phonics 7', 19, 4, 7, 0, null, '/happychildren/books/My First Phonics 7/cover/My First Phonics 7.jpg', '/happychildren/books/My First Phonics 7/', null), \n"
                  "(27, 'My First Phonics 8', 19, 4, 8, 0, null, '/happychildren/books/My First Phonics 8/cover/My First Phonics 8.jpg', '/happychildren/books/My First Phonics 8/', null), \n"
                  "(28, 'My First Phonics 9', 19, 4, 9, 0, null, '/happychildren/books/My First Phonics 9/cover/My First Phonics 9.jpg', '/happychildren/books/My First Phonics 9/', null), \n"
                  "(29, 'My First Phonics 10', 19, 4, 10, 0, null, '/happychildren/books/My First Phonics 10/cover/My First Phonics 10.jpg', '/happychildren/books/My First Phonics 10/', null), \n"
                  "(30, 'My First Phonics 11', 19, 4, 11, 0, null, '/happychildren/books/My First Phonics 11/cover/My First Phonics 11.jpg', '/happychildren/books/My First Phonics 11/', null), \n"
                  "(31, 'My First Phonics 12', 19, 4, 12, 0, null, '/happychildren/books/My First Phonics 12/cover/My First Phonics 12.jpg', '/happychildren/books/My First Phonics 12/', null), \n"
                  "(32, 'My First Phonics 13', 19, 4, 13, 0, null, '/happychildren/books/My First Phonics 13/cover/My First Phonics 13.jpg', '/happychildren/books/My First Phonics 13/', null), \n"
                  "(33, 'My First Phonics 14', 19, 4, 14, 0, null, '/happychildren/books/My First Phonics 14/cover/My First Phonics 14.jpg', '/happychildren/books/My First Phonics 14/', null), \n"
                  "(34, 'My First Phonics 15', 19, 4, 15, 0, null, '/happychildren/books/My First Phonics 15/cover/My First Phonics 15.jpg', '/happychildren/books/My First Phonics 15/', null), \n"
                  "(35, 'My First Phonics 16', 19, 4, 16, 0, null, '/happychildren/books/My First Phonics 16/cover/My First Phonics 16.jpg', '/happychildren/books/My First Phonics 16/', null), \n"
                  "(36, 'My family', 19, 4, 17, 0, null, '/happychildren/books/Book 1_My family/cover/Book 1_My family.jpg', '/happychildren/books/Book 1_My family/', null), \n"
                  "(37, 'A polite child', 19, 4, 18, 0, null, '/happychildren/books/Book 2_A polite child/cover/Book 2_A polite child.jpg', '/happychildren/books/Book 2_A polite child/', null), \n"
                  "(38, 'Winter clothes', 19, 4, 19, 0, null, '/happychildren/books/Book 3_Winter clothes/cover/Book 3_Winter clothes.jpg', '/happychildren/books/Book 3_Winter clothes/', null), \n"
                  "(39, 'Bear is body', 19, 4, 20, 0, null, '/happychildren/books/Book 4_Bear is body/cover/Book 4_Bear is body.jpg', '/happychildren/books/Book 4_Bear is body/', null), \n"
                  "(40, 'What fruit can you find', 19, 4, 21, 0, null, '/happychildren/books/Book 5_What fruit can you find/cover/Book 5_What fruit can you find.jpg', '/happychildren/books/Book 5_What fruit can you find/', null), \n"
                  "(41, 'Can you see some colors', 19, 4, 22, 0, null, '/happychildren/books/Book 6_Can you see some colors/cover/Book 6_Can you see some colors.jpg', '/happychildren/books/Book 6_Can you see some colors/', null), \n"
                  "(42, 'A maggic pencil case', 19, 4, 23, 0, null, '/happychildren/books/Book 7_A maggic pencil case/cover/Book 7_A maggic pencil case.jpg', '/happychildren/books/Book 7_A maggic pencil case/', null), \n"
                  "(43, 'Different shapes_final', 19, 4, 24, 0, null, '/happychildren/books/Book 8_Different shapes_final/cover/Book 8_Different shapes_final.jpg', '/happychildren/books/Book 8_Different shapes_final/', null), \n"
                  "(44, 'How many balloons', 19, 4, 25, 0, null, '/happychildren/books/Book 9_How many balloons/cover/Book 9_How many balloons.jpg', '/happychildren/books/Book 9_How many balloons/', null), \n"
                  "(45, 'What can they be', 19, 4, 26, 0, null, '/happychildren/books/Book 10_What can they be/cover/Book 10_What can they be.jpg', '/happychildren/books/Book 10_What can they be/', null), \n"
                  "(46, 'Go on a picnic', 19, 4, 27, 0, null, '/happychildren/books/Book 11_Go on a picnic/cover/Book 11_Go on a picnic.jpg', '/happychildren/books/Book 11_Go on a picnic/', null), \n"
                  "(47, 'What is the dog doing', 19, 4, 28, 0, null, '/happychildren/books/Book 12_What is the dog doing/cover/Book 12_What is the dog doing.jpg', '/happychildren/books/Book 12_What is the dog doing/', null), \n"
                  "(48, 'Jack Jack what do you see', 19, 4, 29, 0, null, '/happychildren/books/Book 13_ Jack Jack what do you see/cover/Book 13_ Jack Jack what do you see.jpg', '/happychildren/books/Book 13_ Jack Jack what do you see/', null), \n"
                  "(49, 'A day of Amy', 19, 4, 30, 0, null, '/happychildren/books/Book 14_A day of Amy/cover/Book 14_A day of Amy.jpg', '/happychildren/books/Book 14_A day of Amy/', null), \n"
                  "(50, 'Which one is your face today', 19, 4, 31, 0, null, '/happychildren/books/Book 15_Which one is your face today/cover/Book 15_Which one is your face today.jpg', '/happychildren/books/Book 15_Which one is your face today/', null), \n"
                  "(51, 'We are having fun', 19, 4, 32, 0, null, '/happychildren/books/Book 16_We are having fun/cover/Book 16_We are having fun.jpg', '/happychildren/books/Book 16_We are having fun/', null), \n"
                  "(52, '麻雀不怕电', 16, 4, 1, 0, null, '/happychildren/books/麻雀不怕电/cover/麻雀不怕电.jpg', '/happychildren/books/麻雀不怕电/', null), \n"
                  "(53, '马路上不走马却走人', 16, 4, 2, 0, null, '/happychildren/books/马路上不走马却走人/cover/马路上不走马却走人.jpg', '/happychildren/books/马路上不走马却走人/', null), \n"
                  "(54, '屁好臭', 16, 4, 3, 0, null, '/happychildren/books/屁好臭/cover/屁好臭.jpg', '/happychildren/books/屁好臭/', null), \n"
                  "(55, '天空那么蓝', 16, 4, 4, 0, null, '/happychildren/books/天空那么蓝/cover/天空那么蓝.jpg', '/happychildren/books/天空那么蓝/', null), \n"
                  "(56, '阳台很危险', 16, 4, 5, 0, null, '/happychildren/books/阳台很危险/cover/阳台很危险.jpg', '/happychildren/books/阳台很危险/', null), \n"
                  "(57, '用猫来上网', 16, 4, 6, 0, null, '/happychildren/books/用猫来上网/cover/用猫来上网.jpg', '/happychildren/books/用猫来上网/', null), \n"
                  "(58, '又不让我看电视', 16, 4, 7, 0, null, '/happychildren/books/又不让我看电视/cover/又不让我看电视.jpg', '/happychildren/books/又不让我看电视/', null), \n"
                  "(59, '植物会哭', 16, 4, 8, 0, null, '/happychildren/books/植物会哭/cover/植物会哭.jpg', '/happychildren/books/植物会哭/', null), \n"
                  "(60, '变来变去的变色龙', 17, 4, 1, 0, null, '/happychildren/books/变来变去的变色龙/cover/变来变去的变色龙.jpg', '/happychildren/books/变来变去的变色龙/', null), \n"
                  "(61, '不一样的大扫除', 17, 4, 2, 0, null, '/happychildren/books/不一样的大扫除/cover/不一样的大扫除.jpg', '/happychildren/books/不一样的大扫除/', null), \n"
                  "(62, '藏起来，藏起来', 17, 4, 3, 0, null, '/happychildren/books/藏起来，藏起来/cover/藏起来，藏起来.jpg', '/happychildren/books/藏起来，藏起来/', null), \n"
                  "(63, '咔嚓咔嚓，剪一剪', 17, 4, 4, 0, null, '/happychildren/books/咔嚓咔嚓，剪一剪/cover/咔嚓咔嚓，剪一剪.jpg', '/happychildren/books/咔嚓咔嚓，剪一剪/', null), \n"
                  "(64, '魔法变变变', 17, 4, 5, 0, null, '/happychildren/books/魔法变变变/cover/魔法变变变.jpg', '/happychildren/books/魔法变变变/', null), \n"
                  "(65, '我们的形状小镇', 17, 4, 6, 0, null, '/happychildren/books/我们的形状小镇/cover/我们的形状小镇.jpg', '/happychildren/books/我们的形状小镇/', null), \n"
                  "(66, '小老鼠和大象', 17, 4, 7, 0, null, '/happychildren/books/小老鼠和大象/cover/小老鼠和大象.jpg', '/happychildren/books/小老鼠和大象/', null), \n"
                  "(67, '长颈鹿要出门', 17, 4, 8, 0, null, '/happychildren/books/长颈鹿要出门/cover/长颈鹿要出门.jpg', '/happychildren/books/长颈鹿要出门/', null), \n"
                  "(68, '星星狂欢节', 18, 4, 1, 0, null, '/happychildren/books/星星狂欢节/cover/星星狂欢节.jpg', '/happychildren/books/星星狂欢节/', null), \n"
                  "(69, '身体里的旅行', 18, 4, 2, 0, null, '/happychildren/books/身体里的旅行/cover/身体里的旅行.jpg', '/happychildren/books/身体里的旅行/', null), \n"
                  "(70, '重返恐龙世纪', 18, 4, 3, 0, null, '/happychildren/books/重返恐龙世纪/cover/重返恐龙世纪.jpg', '/happychildren/books/重返恐龙世纪/', null), \n"
                  "(71, '妙妙动物园', 18, 4, 4, 0, null, '/happychildren/books/妙妙动物园/cover/妙妙动物园.jpg', '/happychildren/books/妙妙动物园/', null), \n"
                  "(72, '漫游海底世界', 18, 4, 5, 0, null, '/happychildren/books/漫游海底世界/cover/漫游海底世界.jpg', '/happychildren/books/漫游海底世界/', null), \n"
                  "(73, '追踪史前怪物', 18, 4, 6, 0, null, '/happychildren/books/追踪史前怪物/cover/追踪史前怪物.jpg', '/happychildren/books/追踪史前怪物/', null);\n";
            // /happychildren/dosh/ghos/gho_run.exe
            // /happychildren/TeamViewer/TeamViewer.exe
            // /happychildren/EasyMeeting/Main/EasyMeeting.exe
            ExecuteNonQuery(dml);
        }

        std::map<std::wstring, std::vector<ColumnMeta>> const TABLE_METAS{
                { L"templates", { { L"id", ColumnMeta::INTEGER }, { L"name", ColumnMeta::TEXT }, { L"state", ColumnMeta::INTEGER }, { L"description", ColumnMeta::TEXT } } },
                { L"types", { { L"id", ColumnMeta::INTEGER }, { L"no", ColumnMeta::INTEGER }, { L"template_id", ColumnMeta::INTEGER }, { L"name", ColumnMeta::TEXT }, { L"type", ColumnMeta::INTEGER }, { L"description", ColumnMeta::TEXT } } },
                { L"codes", { { L"id", ColumnMeta::INTEGER }, { L"template_id", ColumnMeta::INTEGER }, { L"parent_no", ColumnMeta::INTEGER }, { L"no", ColumnMeta::INTEGER }, { L"value", ColumnMeta::TEXT }, { L"description", ColumnMeta::TEXT } } },
                { L"schemas", { { L"id", ColumnMeta::INTEGER }, { L"template_id", ColumnMeta::INTEGER }, { L"parent_no", ColumnMeta::INTEGER }, { L"no", ColumnMeta::INTEGER }, { L"name", ColumnMeta::TEXT }, { L"type_no", ColumnMeta::INTEGER }, { L"is_required", ColumnMeta::INTEGER }, { L"restriction", ColumnMeta::TEXT }, { L"description", ColumnMeta::TEXT } } },
                { L"lists", { { L"id", ColumnMeta::INTEGER }, { L"template_id", ColumnMeta::INTEGER }, { L"parent_no", ColumnMeta::INTEGER }, { L"element_type_no", ColumnMeta::INTEGER }, { L"min", ColumnMeta::INTEGER }, { L"max", ColumnMeta::INTEGER }, { L"group_by", ColumnMeta::TEXT }, { L"is_hidden", ColumnMeta::INTEGER }, { L"description", ColumnMeta::TEXT } } },
                { L"unit_types", { { L"id", ColumnMeta::INTEGER }, { L"name", ColumnMeta::TEXT }, { L"description", ColumnMeta::TEXT } } },
                { L"units", { { L"id", ColumnMeta::INTEGER }, { L"type_id", ColumnMeta::INTEGER }, { L"name", ColumnMeta::TEXT }, { L"symbol", ColumnMeta::TEXT }, { L"description", ColumnMeta::TEXT } } },
                { L"组织", { { L"id", ColumnMeta::INTEGER }, { L"编号", ColumnMeta::TEXT }, { L"名称", ColumnMeta::TEXT }, { L"类型", ColumnMeta::INTEGER }, { L"地址", ColumnMeta::TEXT }, { L"电话", ColumnMeta::TEXT }, { L"联系人", ColumnMeta::TEXT }, { L"管理员id", ColumnMeta::INTEGER }, { L"备注", ColumnMeta::TEXT } } },
                { L"用户", { { L"id", ColumnMeta::INTEGER }, { L"登录名", ColumnMeta::TEXT }, { L"姓名", ColumnMeta::TEXT }, { L"性别", ColumnMeta::INTEGER }, { L"生日", ColumnMeta::TEXT }, { L"组织id", ColumnMeta::INTEGER }, { L"住址", ColumnMeta::TEXT }, { L"电话", ColumnMeta::TEXT }, { L"email", ColumnMeta::TEXT }, { L"website", ColumnMeta::TEXT }, { L"密码", ColumnMeta::TEXT }, { L"role", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                { L"机型", { { L"id", ColumnMeta::INTEGER }, { L"模板id", ColumnMeta::INTEGER }, { L"类型", ColumnMeta::INTEGER }, { L"名称", ColumnMeta::TEXT }, { L"工厂id", ColumnMeta::INTEGER }, { L"state", ColumnMeta::INTEGER }, { L"扩展", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                { L"状态", { { L"id", ColumnMeta::INTEGER }, { L"机型id", ColumnMeta::INTEGER }, { L"编号", ColumnMeta::TEXT }, { L"使命任务", ColumnMeta::TEXT }, { L"state", ColumnMeta::INTEGER }, { L"扩展", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                { L"批次", { { L"id", ColumnMeta::INTEGER }, { L"状态id", ColumnMeta::INTEGER }, { L"编号", ColumnMeta::TEXT }, { L"飞机编号范围", ColumnMeta::TEXT }, { L"发动机编号范围", ColumnMeta::TEXT }, { L"布局", ColumnMeta::TEXT }, { L"主要技术参数", ColumnMeta::TEXT }, { L"主要配备", ColumnMeta::TEXT }, { L"喷涂方案", ColumnMeta::INTEGER }, { L"state", ColumnMeta::INTEGER }, { L"扩展", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                { L"基线", { { L"id", ColumnMeta::INTEGER }, { L"适用目标", ColumnMeta::INTEGER }, { L"目标id", ColumnMeta::INTEGER }, { L"state", ColumnMeta::INTEGER }, { L"扩展", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                { L"飞机", { { L"id", ColumnMeta::INTEGER }, { L"批次id", ColumnMeta::INTEGER }, { L"编号", ColumnMeta::TEXT }, { L"承制单位id", ColumnMeta::INTEGER }, { L"接装单位id", ColumnMeta::INTEGER }, { L"移交时间", ColumnMeta::TEXT }, { L"移交编号", ColumnMeta::TEXT }, { L"使用寿命", ColumnMeta::INTEGER }, { L"剩余寿命", ColumnMeta::INTEGER }, { L"引擎型号", ColumnMeta::TEXT }, { L"引擎编号", ColumnMeta::TEXT }, { L"引擎使用寿命", ColumnMeta::INTEGER }, { L"引擎剩余寿命", ColumnMeta::INTEGER }, { L"state", ColumnMeta::INTEGER }, { L"扩展", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                { L"构型", { { L"id", ColumnMeta::INTEGER }, { L"基线id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"序号", ColumnMeta::INTEGER }, { L"名称", ColumnMeta::TEXT }, { L"型号", ColumnMeta::TEXT }, { L"承制单位id", ColumnMeta::INTEGER }, { L"extended", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                // { L"softwares", { { L"id", ColumnMeta::INTEGER }, { L"基线id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"构型节点id", ColumnMeta::INTEGER }, { L"名称", ColumnMeta::TEXT }, { L"版本", ColumnMeta::TEXT }, { L"重要度", ColumnMeta::TEXT },{ L"研制单位id", ColumnMeta::INTEGER }, { L"状态", ColumnMeta::TEXT }, { L"主管", ColumnMeta::TEXT }, { L"extended", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                { L"softwares", { { L"id", ColumnMeta::INTEGER }, { L"基线id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"构型节点id", ColumnMeta::INTEGER }, { L"名称", ColumnMeta::TEXT }, { L"版本", ColumnMeta::TEXT }, { L"重要度", ColumnMeta::TEXT },{ L"研制单位id", ColumnMeta::INTEGER }, { L"状态", ColumnMeta::TEXT }, { L"主管", ColumnMeta::TEXT }, { L"extended", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                { L"support_configs", { { L"id", ColumnMeta::INTEGER }, { L"template_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"name", ColumnMeta::TEXT }, { L"table_no", ColumnMeta::INTEGER }/*, { L"table_name", ColumnMeta::TEXT }*/ } },
                { L"随机设备工具", { { L"id", ColumnMeta::INTEGER }, { L"基线id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"专业", ColumnMeta::INTEGER }, { L"序号", ColumnMeta::TEXT }, { L"名称", ColumnMeta::TEXT }, { L"型号", ColumnMeta::TEXT }, { L"用途", ColumnMeta::TEXT }, { L"配套比例", ColumnMeta::TEXT }, { L"定价", ColumnMeta::TEXT }, { L"承制单位id", ColumnMeta::INTEGER }, { L"扩展", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                { L"机械专业随机备件", { { L"id", ColumnMeta::INTEGER }, { L"基线id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"序号", ColumnMeta::TEXT }, { L"名称", ColumnMeta::TEXT }, { L"型号(图号)", ColumnMeta::TEXT }, { L"单机数量", ColumnMeta::TEXT }, { L"比例及数量", ColumnMeta::TEXT }, { L"所属系统", ColumnMeta::TEXT }, { L"安装使用部位", ColumnMeta::TEXT }, { L"承制单位id", ColumnMeta::INTEGER }, { L"extended", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                { L"随机资料目录", { { L"id", ColumnMeta::INTEGER }, { L"基线id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"序号", ColumnMeta::TEXT }, { L"资料编号", ColumnMeta::TEXT }, { L"手册名称", ColumnMeta::TEXT }, { L"册数", ColumnMeta::TEXT }, { L"单册页数", ColumnMeta::TEXT }, { L"密级", ColumnMeta::TEXT }, { L"扩展", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                { L"随机资料配套目录", { { L"id", ColumnMeta::INTEGER }, { L"基线id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"序号", ColumnMeta::TEXT }, { L"资料名称", ColumnMeta::TEXT }, { L"手册编制的参考标准", ColumnMeta::TEXT }, { L"主要用途/使用时机/使用地点", ColumnMeta::TEXT }, { L"主要使用对象", ColumnMeta::TEXT }, { L"扩展", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                { L"技术状态文件", { { L"id", ColumnMeta::INTEGER }, { L"基线id", ColumnMeta::INTEGER }, { L"类型", ColumnMeta::INTEGER }, { L"previous_version_id", ColumnMeta::INTEGER }, { L"路径", ColumnMeta::TEXT }, { L"编号", ColumnMeta::TEXT }, { L"名称", ColumnMeta::TEXT }, { L"版本", ColumnMeta::TEXT }, { L"修订时间", ColumnMeta::TEXT }, { L"构型节点id", ColumnMeta::INTEGER }, { L"附件", ColumnMeta::INTEGER }, { L"state", ColumnMeta::INTEGER }, { L"扩展", ColumnMeta::TEXT }, { L"备注", ColumnMeta::TEXT } } },
                { L"有寿件", { { L"id", ColumnMeta::INTEGER }, { L"基线id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"扩展", ColumnMeta::TEXT } } },
                { L"双流水", { { L"id", ColumnMeta::INTEGER }, { L"基线id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"扩展", ColumnMeta::TEXT } } },
                { L"交付遗留问题及处置情况", { { L"id", ColumnMeta::INTEGER }, { L"基线id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"扩展", ColumnMeta::TEXT } } },
                { L"change_histories", { { L"id", ColumnMeta::INTEGER }, { L"基线id", ColumnMeta::INTEGER }, { L"time", ColumnMeta::TEXT }, { L"no", ColumnMeta::TEXT }, { L"control_file_id", ColumnMeta::INTEGER }, { L"description", ColumnMeta::TEXT } } },
                { L"change_history_details", { { L"id", ColumnMeta::INTEGER }, { L"change_history_id", ColumnMeta::INTEGER }, { L"from_file_id", ColumnMeta::INTEGER }, { L"to_file_id", ColumnMeta::INTEGER }, { L"description", ColumnMeta::TEXT } } },
                { L"files", { { L"id", ColumnMeta::INTEGER }, { L"type", ColumnMeta::TEXT }, { L"ext", ColumnMeta::TEXT }, { L"content", ColumnMeta::BLOB }, { L"size", ColumnMeta::INTEGER }, { L"digest", ColumnMeta::TEXT } } },
                { L"info_configs", { { L"id", ColumnMeta::INTEGER }, { L"template_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"type", ColumnMeta::INTEGER }, { L"name", ColumnMeta::TEXT }, { L"table_no", ColumnMeta::INTEGER }, { L"table_field_no", ColumnMeta::INTEGER } } },
                { L"histories", { { L"id", ColumnMeta::INTEGER }, { L"基线id", ColumnMeta::INTEGER }, { L"name", ColumnMeta::TEXT }, { L"no", ColumnMeta::TEXT }, { L"object", ColumnMeta::TEXT }, { L"abstract", ColumnMeta::TEXT }, { L"operator", ColumnMeta::TEXT }, { L"time", ColumnMeta::TEXT }, { L"control_file_id", ColumnMeta::INTEGER }, { L"description", ColumnMeta::TEXT } } }
        };
    }

    void Init(std::wstring const& dbFilename) {
        // r = sqlite3_open(Texts::toNative(dbFilename).c_str(), &db);
        r = sqlite3_open(Texts::toUtf8(dbFilename).c_str(), &db);
        //sqlite3_key(db, "password", 8);
        //sqlite3_rekey(db, "", 0);
        // prepareTables(db);
    }

    void Init() {
        Init(runFolder + L"/air.db");
        // C:/Users/fixop/source/repos/rain
        // Init(runFolder + L"/shared/db/info.sqlite"); // C:/Users/fixop/source/repos/rain
        // C:/Users/fixop/source/repos/rain/shared/db/air.sqlite
        // C:/Users/qinym/source/repos/productmanage/MiniBlinkBrowser/utils/db/air.db
        // Init(L"C:/Users/fixop/source/repos/productmanage/AirPlane/utils/db/air.db");
        // Init(L"C:/Users/qinym/source/repos/productmanage/AirPlane/utils/db/air.db");
    }

    void Finally() {
        sqlite3_close(db);
    }

    bool IsOk() {
        return r == SQLITE_OK;
    }

    std::wstring const LastError() {
        const char* e = sqlite3_errmsg(db);
        return Texts::fromUtf8(e);
    }

    void ExecuteNonQuery(std::wstring const& command) {
        if (db == nullptr) {
            Init();
        }
        sqlite3_stmt* stmt = nullptr;
        r = sqlite3_prepare_v3(db, Texts::toUtf8(command).c_str(), -1, 0, &stmt, nullptr);
        if (IsOk()) {
            r = sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }

    std::wstring const ExecuteScalar(std::wstring const& query) {
        std::wstring result;
        if (db == nullptr) {
            Init();
        }
        sqlite3_stmt* stmt = nullptr;
        r = sqlite3_prepare_v3(db, Texts::toUtf8(query).c_str(), -1, 0, &stmt, nullptr);
        if (IsOk()) {
            r = sqlite3_step(stmt);
            if (r == SQLITE_ROW) {
                int count = sqlite3_column_count(stmt);
                if (count == 1) {
                    const char *v = (char*)sqlite3_column_text(stmt, 0);
                    if (v != nullptr) {
                        result = Texts::fromUtf8(v);
                    }
                }
            }
            sqlite3_finalize(stmt);
        }
        return result;
    }

    std::vector<std::vector<std::optional<std::wstring>>> const ExecuteQueryTable(std::wstring const& query) {
        std::vector<std::vector<std::optional<std::wstring>>> result;
        if (db == nullptr) {
            Init();
        }
        sqlite3_stmt* stmt = nullptr;
        r = sqlite3_prepare_v3(db, Texts::toUtf8(query).c_str(), -1, 0, &stmt, nullptr);
        if (IsOk()) {
            int count = sqlite3_column_count(stmt);
            //result.columnNames.reserve(count);
            //for (int i = 0; i < count; ++i) {
            //    result.columnNames.push_back(Texts::fromUtf8((char*)sqlite3_column_name(stmt, i)));
            //}
            while ((r = sqlite3_step(stmt)) == SQLITE_ROW) {
                std::vector<std::optional<std::wstring>> row;
                row.reserve(count);
                for (int i = 0; i < count; ++i) {
                    const char *v = (char*)sqlite3_column_text(stmt, i);
                    if (v == nullptr) {
                        row.push_back(std::optional<std::wstring>());
                    } else {
                        row.push_back(Texts::fromUtf8(v));
                    }
                }
                result.push_back(row);
            }
            result.shrink_to_fit();
            sqlite3_finalize(stmt);
        }
        return result;
    }

    bool const IsTableExist(std::wstring const& tableName) {
        std::wstring query = L"SELECT count(*) FROM sqlite_master WHERE tbl_name='" + tableName + L"' AND type='table'";
        auto c = ExecuteScalar(query);
        return std::stoi(c) == 1;
    }

    DataTable const ParseCSV(std::wstring const& filename) {
        DataTable result;
        auto content = Folders::ReadFile(filename);
        auto lines = Texts::split(content, '\n');
        if (!lines.empty()) {
            auto columnNames = Texts::split(lines[0], ',');
            auto count = lines.size();
            for (auto i = 1u; i < count; ++i) {
                auto values = Texts::split(lines[i], ',');
            }
        }
        return result;
    }

    DataTable Setting::data = DataTable(L"settings", std::vector<std::wstring>{L"id", L"name", L"value"});

    std::vector<Setting> Setting::select(std::function<bool(std::vector<std::optional<std::wstring>> const& row)>&& accept) {
        std::vector<Setting> result;
        auto& r = data.select(std::move(accept));
        std::transform(r.begin(), r.end(), std::back_inserter(result), fromRow);
        return result;
    }

    Setting Setting::fromRow(std::vector<std::optional<std::wstring>> const& row) {
        Setting result;
        for (auto entry : data.columnIndex) {
            std::optional<std::wstring> v = row[entry.second];
            if (entry.first == L"id") {
                result.id = std::stoi(v.has_value() ? v.value() : L"0");
            } else if (entry.first == L"name") {
                result.name = v.has_value() ? v.value() : L"NULL";
            } else if (entry.first == L"value") {
                result.value = v.has_value() ? v.value() : L"";
            } else {
                assert(false); // error, do nothing
            }
        }
        return result;
    }

    std::vector<std::optional<std::wstring>> Setting::toRow() const {
        return std::vector<std::optional<std::wstring>>{std::to_wstring(id), quote(name), quote(value)};
    }

    DataTable Item::data = DataTable(L"items", std::vector<std::wstring>{L"id", L"name", L"parent_id", L"type", L"\"order\"", L"state", L"drm", L"icon", L"args", L"description"}, L"ORDER BY parent_id, \"order\"");

    std::vector<Item> Item::select(std::function<bool(std::vector<std::optional<std::wstring>> const& row)>&& accept) {
        std::vector<Item> result;
        auto& r = data.select(std::move(accept));
        std::transform(r.begin(), r.end(), std::back_inserter(result), fromRow);
        return result;
    }

    Item Item::fromRow(std::vector<std::optional<std::wstring>> const& row) {
        Item result;
        for (auto entry : data.columnIndex) {
            std::optional<std::wstring> v = row[entry.second];
            if (entry.first == L"id") {
                result.id = std::stoi(v.has_value() ? v.value() : L"0");
            } else if (entry.first == L"name") {
                result.name = v.has_value() ? v.value() : L"";
            } else if (entry.first == L"parent_id") {
                result.parentId = std::stoi(v.has_value() ? v.value() : L"0");
            } else if (entry.first == L"type") {
                result.type = (OpenerTypes) std::stoi(v.has_value() ? v.value() : L"0");
            } else if (entry.first == L"\"order\"") {
                result.order = std::stoi(v.has_value() ? v.value() : L"0");
            } else if (entry.first == L"state") {
                result.state = std::stoi(v.has_value() ? v.value() : L"0");
            } else if (entry.first == L"drm") {
                result.drm = v.has_value() ? v.value() : L"";
            } else if (entry.first == L"icon") {
                result.icon = v.has_value() ? v.value() : L"";
            } else if (entry.first == L"args") {
                result.args = v.has_value() ? v.value() : L"";
            } else if (entry.first == L"description") {
                result.description = v.has_value() ? v.value() : L"";
            } else {
                assert(false); // error, do nothing
            }
        }
        return result;
    }

    std::vector<std::optional<std::wstring>> Item::toRow() const {
        return std::vector<std::optional<std::wstring>>{std::to_wstring(id), quote(name), std::to_wstring(parentId), std::to_wstring(type), std::to_wstring(order), std::to_wstring(state), quote(drm), quote(icon), quote(args), quote(description)};
    }

    DataTable Recent::data = DataTable(L"recents", std::vector<std::wstring>{L"id", L"item_id", L"parent_id", L"last_time"});

    std::vector<Recent> Recent::select(std::function<bool(std::vector<std::optional<std::wstring>> const& row)>&& accept) {
        std::vector<Recent> result;
        auto& r = data.select(std::move(accept));
        std::transform(r.begin(), r.end(), std::back_inserter(result), fromRow);
        return result;
    }

    Recent Recent::fromRow(std::vector<std::optional<std::wstring>> const& row) {
        Recent result;
        for (auto entry : data.columnIndex) {
            std::optional<std::wstring> v = row[entry.second];
            if (entry.first == L"id") {
                result.id = std::stoi(v.has_value() ? v.value() : L"0");
            } else if (entry.first == L"item_id") {
                result.itemId = std::stoi(v.has_value() ? v.value() : L"0");
            } else if (entry.first == L"parent_id") {
                result.parentId = std::stoi(v.has_value() ? v.value() : L"0");
            } else if (entry.first == L"last_time") {
                result.lastTime = v.has_value() ? v.value() : L"";
            } else {
                assert(false); // error, do nothing
            }
        }
        return result;
    }

    std::vector<std::optional<std::wstring>> Recent::toRow() const {
        return std::vector<std::optional<std::wstring>>{std::to_wstring(id), std::to_wstring(itemId), std::to_wstring(parentId), quote(lastTime)};
    }

    std::wstring toJson(std::vector<ColumnMeta> const& columns, std::vector<std::optional<std::wstring>> const& row) {
        nlohmann::json data;
        size_t columnCount = columns.size();
        int difference = 0;
        for (size_t i = 0; i < columnCount; ++i) {
            auto& cell = row[i - difference];
            switch (columns[i].type) {
            case ColumnMeta::TEXT:
                if (cell.has_value()) {
                    data[Texts::toUtf8(columns[i].name)] = Texts::toUtf8(cell.value());
                } else {
                    data[Texts::toUtf8(columns[i].name)] = nullptr;
                }
                break;
            case ColumnMeta::INTEGER:
                if (cell.has_value()) {
                    data[Texts::toUtf8(columns[i].name)] = std::stoi(cell.value());
                } else {
                    data[Texts::toUtf8(columns[i].name)] = nullptr;
                }
                break;
            case ColumnMeta::BLOB:
                difference++;
                break;
            }
        }
        return Texts::fromUtf8(data.dump());
    }

    std::vector<ColumnMeta> getColumns(std::wstring const& tableName) {
        std::vector<ColumnMeta> result;
        auto it = TABLE_METAS.find(tableName);
        if (it != TABLE_METAS.end()) {
            result = it->second;
        }
        return result;
    }
    
    std::wstring constructColumnList(std::vector<ColumnMeta> const& columns) {
        std::wstring result = L"";
        bool isFirst = true;
        for (ColumnMeta const& column : columns) {
            if (column.type != ColumnMeta::BLOB) {
                if (isFirst) {
                    isFirst = false;
                } else {
                    result += L",";
                }
                result += jsonQuote(column.name);
            }
        }
        return result;
    }

    std::wstring query(std::wstring const& tableName, std::wstring const& whereCause, std::wstring const& orderBy) {
        auto const& columnInfos = getColumns(tableName);
        std::wstring query = L"SELECT " + constructColumnList(columnInfos) + L" FROM " + jsonQuote(tableName);
        if (whereCause != L"") {
            query += L" WHERE " + whereCause;
        }
        if (orderBy != L"") {
            query += L" ORDER BY " + orderBy;
        }
        std::vector<std::vector<std::optional<std::wstring>>> r = utils::SqlData::ExecuteQueryTable(query);
        //static std::wstring res;
        //res = L"[";
        //bool first = true;
        //for (auto& row : r) {
        //    if (first) {
        //        first = false;
        //    } else {
        //        res += L", ";
        //    }
        //    res += toJson(columnInfos, row);
        //}
        //res += L"]";
        return toJson(columnInfos, r);
    }

    ColumnMeta::Type getType(std::vector<ColumnMeta> const& columns, std::wstring const& name) {
        ColumnMeta::Type columnType = ColumnMeta::UNKNOWN;
        for (auto const& column : columns) {
            if (column.name == name) {
                columnType = column.type;
                break;
            }
        }
        return columnType;
    }

    std::optional<std::wstring> getField(nlohmann::json const& data, std::wstring const& name, ColumnMeta::Type type) {
        std::optional<std::wstring> result;
        std::string fieldName = Texts::toUtf8(name);
        if (data.contains(fieldName)) {
            switch (type) {
            case ColumnMeta::INTEGER:
                if (!data[fieldName].is_null()) {
                    if (data[fieldName].is_number()) {
                        std::int64_t value = data[fieldName].get<std::int64_t>();
                        result = std::to_wstring(value);
                    } else if (data[fieldName].is_string()) {
                        std::string value = data[fieldName].get<std::string>();
                        result = Texts::fromUtf8(value);
                    }
                }
                break;
            case ColumnMeta::TEXT:
                if (!data[fieldName].is_null()) {
                    if (data[fieldName].is_string()) {
                        std::string value = data[fieldName].get<std::string>();
                        result = sqlQuote(Texts::fromUtf8(value));
                    } else if (data[fieldName].is_number()) {
                        std::int64_t value = data[fieldName].get<std::int64_t>();
                        result = sqlQuote(std::to_wstring(value));
                    }
                }
                break;
            }
        }
        return result;
    }

    int insert(std::wstring const& tableName, nlohmann::json const& data) {
        // Log::log(data.dump());
        int id = 0;
        auto const& columnInfos = getColumns(tableName);
        if (!columnInfos.empty()) {
            std::wstring valueList = L"";
            bool isFirst = true;
            for (ColumnMeta const& column : columnInfos) {
                if (column.type != ColumnMeta::BLOB) {
                    if (isFirst) {
                        isFirst = false;
                    } else {
                        valueList += L", ";
                    }
                    // Log::log(L"name: " + column.name);
                    auto v = getField(data, column.name, column.type);
                    if (v.has_value()) {
                        valueList += v.value();
                        // Log::log(L"value: " + v.value());
                        if (column.name == L"id") {
                            id = std::stoi(v.value());
                        }
                    } else {
                        valueList += L"NULL";
                        // Log::log(L"value: NULL");
                    }
                }
            }
            std::wstring command =  L"INSERT INTO " + jsonQuote(tableName) + L" (" + constructColumnList(columnInfos) + L") VALUES (" + valueList + L");";
            // Log::log(command);
            ExecuteNonQuery(command);
            if (id == 0) {
                id = getLastId();
            }
        }
        return id;
    }

    void update(std::wstring const& tableName, nlohmann::json const& data) {
        auto const& columns = getColumns(tableName);
        if (!columns.empty()) {
            std::wstring idStr;
            std::wstring setList;
            bool isFirst = true;
            for (auto& [key, value] : data.items()) {
                std::wstring name = Texts::fromUtf8(key);
                if (name == L"id") {
                    idStr = std::to_wstring(value.get<std::int64_t>());
                } else {
                    if (isFirst) {
                        isFirst = false;
                    } else {
                        setList += L", ";
                    }
                    auto fieldValue = getField(data, name, getType(columns, name));
                    setList += jsonQuote(name) + L" = " + (fieldValue.has_value() ? fieldValue.value() : L"NULL");
                }
            }
            std::wstring command = L"UPDATE " + jsonQuote(tableName) + L" SET " + setList + L" WHERE id = " + idStr + L";";
            ExecuteNonQuery(command);
        }
    }

    void remove(std::wstring const& tableName, int id) {
        auto const& columns = getColumns(tableName);
        if (!columns.empty()) {
            std::wstring remove = L"DELETE FROM " + jsonQuote(tableName) + L" WHERE id = " + std::to_wstring(id) + L";";
            ExecuteNonQuery(remove);
        }
    }

    long getLastId() {
        return sqlite3_last_insert_rowid(db);
    }

    std::string readBlob(std::wstring const& tableName, int id, std::wstring const& columnName) {
        sqlite3_blob* blob = nullptr;
        sqlite3_blob_open(db, "main", Texts::toUtf8(tableName).c_str(), Texts::toUtf8(columnName).c_str(), id, 0, &blob);
        auto length = sqlite3_blob_bytes(blob);
        void* buffer = malloc(length);
        sqlite3_blob_read(blob, buffer, length, 0);
        sqlite3_blob_close(blob);
        std::string result = std::string(reinterpret_cast<char*>(buffer), length);
        free(buffer);
        return result;
    }

    void fillBlob(std::wstring const& tableName, int id, std::wstring const& columnName, void const* data, int size) {
        std::wstring update = L"UPDATE " + jsonQuote(tableName) + L" SET " + jsonQuote(columnName) + L" = ? WHERE id = " + std::to_wstring(id);
        sqlite3_stmt* stmt = nullptr;
        int rc = sqlite3_prepare_v2(db, Texts::toUtf8(update).c_str(),
            -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            char const* e = sqlite3_errmsg(db);
            std::cerr << "prepare failed: " << sqlite3_errmsg(db) << std::endl;
        } else {
            // SQLITE_STATIC because the statement is finalized
            // before the buffer is freed:
            rc = sqlite3_bind_blob(stmt, 1, data, size, SQLITE_STATIC);
            if (rc != SQLITE_OK) {
                std::cerr << "bind failed: " << sqlite3_errmsg(db) << std::endl;
            } else {
                rc = sqlite3_step(stmt);
                if (rc != SQLITE_DONE)
                    std::cerr << "execution failed: " << sqlite3_errmsg(db) << std::endl;
            }
        }
        sqlite3_finalize(stmt);
    }

    int backup(std::wstring const& name) {
        int result = 0;
        std::wstring path = runFolder + L"\\backup";
        if (!Folders::IsDirectoryExist(path)) {
            Folders::MakeDir(path);
        }
        std::wstring fullName = path + L"\\" + name;
        if (Folders::IsFileExist(fullName)) {
            result = -1;
        } else {
            Folders::FileCopy(runFolder + L"\\air.db", fullName);
        }
        return result;
    }

    int restore(std::wstring const& name) {
        int result = 0;
        std::wstring path = runFolder + L"\\backup";
        if (!Folders::IsDirectoryExist(path)) {
            Folders::MakeDir(path);
        }
        std::wstring fullName = path + L"\\" + name;
        if (Folders::IsFileExist(fullName)) {
            sqlite3_close(db);
            Folders::FileCopy(fullName, runFolder + L"\\air.db");
            Init();
        } else {
            result = -1;
        }
        return result;
    }

    namespace {
        int randomInt(int max) {
            int r = rand();
            return r % max;
        }

        std::string randomStr(int length) {
            static const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";
            constexpr int charLen = sizeof chars;
            std::string result;
            for (int i = 0; i < length; ++i) {
                // result += chars[randomInt(charLen)];
                result.append(1, chars[randomInt(charLen)]);
            }
            return result;
        }

        std::string randomLayout() {
            std::string pictures = u8"[";
            bool isFirst = true;
            for (int i = 0; i < 5; ++i) {
                if (isFirst) {
                    isFirst = false;
                } else {
                    pictures += u8", ";
                }
                pictures += u8"{\"名称\": \"" + randomStr(8) + u8"\"}";
            }
            pictures += u8"]";
            return u8"{\"起始布局\": \"" + randomStr(60) + u8"\", \"总体配置\": \"" + randomStr(76) + u8"\", \"图片\": " + pictures + u8"}";
        }

        std::string randomExternalForm() {
            return u8"{\"机长\": " + std::to_string(randomInt(60)) + u8", \"翼展\": " + std::to_string(randomInt(60)) + u8", \"机高\": " + std::to_string(randomInt(60)) + u8", \"机翼面积\": " + std::to_string(randomInt(60)) + u8", \"主轮距\": " + std::to_string(randomInt(60)) + u8", \"前主轮距\": " + std::to_string(randomInt(60)) + u8"}";
        }

        std::string randomFlyCapability() {
            return u8"{\"实用升限\": \"" + std::to_string(randomInt(60)) + u8"\", \"最大飞行M数\": \"" + std::to_string(randomInt(60)) + u8"\", \"最大飞行表数\": \"" + std::to_string(randomInt(60)) + u8"\", \"重量\": \"" + std::to_string(randomInt(60)) + u8"\", \"起飞滑跑距离\": \"" + std::to_string(randomInt(60)) + u8"\", \"着陆滑跑距离\": \"" + std::to_string(randomInt(60)) + u8"\", \"机动性能\": \"" + std::to_string(randomInt(60)) + u8"\", \"航程航时\": \"" + std::to_string(randomInt(60)) + u8"\"}";
        }

        std::string randomParameters() {
            return u8"{\"外型参数\": " + randomExternalForm() + u8", \"重量重心\": \"" + randomStr(60) + u8"\", \"飞行性能\": " + randomFlyCapability() + u8", \"隐身措施\": \"" + randomStr(60) + u8"\", \"高原措施\": \"" + randomStr(60) + u8"\", \"寿命\": \"" + randomStr(60) + u8"\"}";
        }

        std::string randomWeapon() {
            std::string weapons = u8"[";
            bool isFirst = true;
            for (int i = 0; i < 5; ++i) {
                if (isFirst) {
                    isFirst = false;
                } else {
                    weapons += u8", ";
                }
                weapons += u8"{\"航炮\": \"" + std::to_string(randomInt(60)) + u8"\", \"制导弹药\": \"" + std::to_string(randomInt(60)) + u8"\", \"常规弹药\": \"" + std::to_string(randomInt(60)) + u8"\", \"配套基数\": \"" + std::to_string(randomInt(60)) + u8"\", \"挂载方案\": \"" + std::to_string(randomInt(60)) + u8"\"}";
            }
            weapons += u8"]";

            std::string hangPoints = u8"[";
            isFirst = true;
            for (int i = 0; i < 5; ++i) {
                if (isFirst) {
                    isFirst = false;
                } else {
                    hangPoints += u8", ";
                }
                hangPoints += u8"{\"名称\": \"" + std::to_string(randomInt(60)) + u8"\", \"型号\": \"" + std::to_string(randomInt(60)) + u8"\", \"数量\": \"" + std::to_string(randomInt(60)) + u8"\", \"配套弹药\": \"" + std::to_string(randomInt(60)) + u8"\"}";
            }
            hangPoints += u8"]";
            return u8"{\"武器外挂\": " + weapons + u8", \"挂梁挂柱\": " + hangPoints + u8", \"挂点+外挂物种类\": \"" + randomStr(80) + u8"\"}";
        }

        std::string randomPower() {
            return u8"{\"型号\": \"" + randomStr(24) + u8"\", \"数量\": " + std::to_string(randomInt(60)) + u8", \"总寿命\": \"" + std::to_string(randomInt(60)) + u8"\", \"翻修周期\": \"" + std::to_string(randomInt(60)) + u8"\", \"翻修单位\": \"" + randomStr(60) + u8"\"}";
        }

        std::string randomItem() {
            return u8"{\"型号\": \"" + randomStr(24) + u8"\", \"主要性能\": \"" + randomStr(60) + u8"\", \"型号发展\": \"" + randomStr(60) + u8"\"}";
        }

        std::string randomHang() {
            return u8"{\"型号\": \"" + randomStr(24) + u8"\", \"数量\": " + std::to_string(randomInt(60)) + u8", \"型号发展\": \"" + randomStr(60) + u8"\"}";
        }

        std::string randomSeat() {
            return u8"{\"型号\": \"" + randomStr(18) + u8"\"}";
        }

        std::string randomCompounds() {
            return u8"{\"武器\": " + randomWeapon() + u8", \"动力装置\": " + randomPower() + u8", \"雷达配置\": " + randomItem() + u8", \"电子对抗\": " + randomItem() + u8", \"外挂吊仓\": " + randomHang() + u8", \"数据链\": " + std::to_string(randomInt(3)) + u8", \"座椅\": " + randomSeat() + u8", \"特殊使用限制\": \"" + randomStr(60) + u8"\"}";
        }

        void createBaseline(int type, int id) {
            // 5w configurations
            // create baseline
            // std::string baseline = u8"{\"适用目标\": " + std::to_string(type) + u8", \"目标id\": " + std::to_string(id) + u8", \"state\": 0}";
            nlohmann::json baseline;
            baseline[u8"适用目标"] = type;
            baseline[u8"目标id"] = id;
            baseline[u8"state"] = 0;
            int baselineId = insert(L"基线", baseline/*nlohmann::basic_json<>::parse(baseline)*/);
            // create five thousand configutations of the baseline
            for (int i = 0; i < 10; ++i) {
                // std::string baseline = u8"{\"基线id\": " + std::to_string(baselineId) + u8", \"序号\": " + std::to_string(i) + u8", \"名称\": \"" + randomStr(12) + u8"\"}";
                nlohmann::json configuration;
                configuration[u8"基线id"] = baselineId;
                configuration[u8"序号"] = i;
                configuration[u8"名称"] = randomStr(12);
                configuration.erase(u8"parent_id");
                int oneConfigurationId = insert(L"构型", configuration/*nlohmann::basic_json<>::parse(baseline)*/);
                for (int j = 0; j < 10; ++j) {
                    // baseline = u8"{\"基线id\": " + std::to_string(baselineId) + u8", \"parent_id\": " + std::to_string(oneConfigurationId) + u8", \"序号\": " + std::to_string(j) + u8", \"名称\": \"" + randomStr(12) + u8"\"}";
                    configuration[u8"基线id"] = baselineId;
                    configuration[u8"parent_id"] = oneConfigurationId;
                    configuration[u8"序号"] = i;
                    configuration[u8"名称"] = randomStr(12);
                    int twoConfigurationId = insert(L"构型", configuration/*nlohmann::basic_json<>::parse(baseline)*/);
                    for (int k = 0; k < 20; ++k) {
                        // baseline = u8"{\"基线id\": " + std::to_string(baselineId) + u8", \"parent_id\": " + std::to_string(twoConfigurationId) + u8", \"序号\": " + std::to_string(k) + u8", \"名称\": \"" + randomStr(12) + u8"\"}";
                        configuration[u8"基线id"] = baselineId;
                        configuration[u8"parent_id"] = twoConfigurationId;
                        configuration[u8"序号"] = i;
                        configuration[u8"名称"] = randomStr(12);
                        int threeConfigurationId = insert(L"构型", configuration/*nlohmann::basic_json<>::parse(baseline)*/);
                        for (int l = 0; l < 5; ++l) {
                            // baseline = u8"{\"基线id\": " + std::to_string(baselineId) + u8", \"parent_id\": " + std::to_string(threeConfigurationId) + u8", \"序号\": " + std::to_string(l) + u8", \"名称\": \"" + randomStr(12) + u8"\"}";
                            configuration[u8"基线id"] = baselineId;
                            configuration[u8"parent_id"] = threeConfigurationId;
                            configuration[u8"序号"] = i;
                            configuration[u8"名称"] = randomStr(12);
                            int fourConfigurationId = insert(L"构型", configuration/*nlohmann::basic_json<>::parse(baseline)*/);
                            for (int m = 0; m < 5; ++m) {
                                // baseline = u8"{\"基线id\": " + std::to_string(baselineId) + u8", \"parent_id\": " + std::to_string(fourConfigurationId) + u8", \"序号\": " + std::to_string(m) + u8", \"名称\": \"" + randomStr(12) + u8"\"}";
                                configuration[u8"基线id"] = baselineId;
                                configuration[u8"parent_id"] = fourConfigurationId;
                                configuration[u8"序号"] = i;
                                configuration[u8"名称"] = randomStr(12);
                                int fiveConfigurationId = insert(L"构型", configuration/*nlohmann::basic_json<>::parse(baseline)*/);
                            }
                        }
                    }
                }
            }
        }
    }

    void constructTestData() {
        srand(time(nullptr));
        // 64 baseline
        for (int i = 0; i < 2; ++i) {
            // create (2) model
            // std::string model = u8"{\"模板id\": 1, \"类型\": " + std::to_string(randomInt(8)) + u8", \"名称\": \"" + randomStr(8) + u8"\", \"state\": 0}";
            nlohmann::json model;
            model[u8"模板id"] = 1;
            model[u8"类型"] = randomInt(8);
            model[u8"名称"] = randomStr(8);
            model[u8"state"] = 0;
            // Log::log(Texts::fromUtf8(model));
            int modelId = insert(L"机型", model/*nlohmann::basic_json<>::parse(model)*/);
            for (int j = 0; j < 2; ++j) {
                // create (2) states of one model
                // std::string state = u8"{\"机型id\": " + std::to_string(modelId) + u8", \"编号\": \"00" + std::to_string(j + 1) + u8"\", \"使命任务\": \"" + randomStr(80) + u8"\", \"state\": 0}";
                nlohmann::json state;
                state[u8"机型id"] = modelId;
                state[u8"编号"] = "00" + std::to_string(j + 1);
                state[u8"使命任务"] = randomStr(80);
                state[u8"state"] = 0;
                // Log::log(Texts::fromUtf8(state));
                int stateId = insert(L"状态", state/*nlohmann::basic_json<>::parse(state)*/);
                for (int k = 0; k < 4; ++k) {
                    // create (4) batchs of one states
                    // std::string batch = u8"{\"状态id\": " + std::to_string(stateId) + u8", \"编号\": \"0" + std::to_string(k + 1) + u8"\", \"布局\": \"" + jsonEscape(randomLayout()) + u8"\", \"主要技术参数\": \"" + jsonEscape(randomParameters()) + u8"\", \"主要配备\": \"" + jsonEscape(randomCompounds()) + u8"\", \"state\": 0}";
                    nlohmann::json batch;
                    batch[u8"状态id"] = stateId;
                    batch[u8"编号"] = "0" + std::to_string(k + 1);
                    batch[u8"布局"] = randomLayout(); // jsonEscape(randomLayout());
                    batch[u8"主要技术参数"] = randomParameters(); // jsonEscape(randomParameters());
                    batch[u8"主要配备"] = randomCompounds(); // jsonEscape(randomCompounds());
                    batch[u8"state"] = 0;
                    int batchId = insert(L"批次", batch/*nlohmann::basic_json<>::parse(batch)*/);
                    createBaseline(0, batchId);
                    for (int l = 0; l < 4; ++l) {
                        // create (4) airs of one batchs
                        // std::string air = u8"{\"批次id\": " + std::to_string(batchId) + u8", \"编号\": \"A0" + std::to_string(l + 1) + u8"\", \"移交编号\": \"" + std::to_string(randomInt(80)) + u8"\", \"使用寿命\": " + std::to_string(randomInt(8000)) + u8", \"剩余寿命\": " + std::to_string(randomInt(800)) + u8", \"引擎型号\": \"" + randomStr(24) + u8"\", \"引擎编号\": \"" + std::to_string(randomInt(80)) + u8"\", \"引擎使用寿命\": " + std::to_string(randomInt(8000)) + u8", \"引擎剩余寿命\": " + std::to_string(randomInt(800)) + u8", \"state\": 0}";
                        nlohmann::json air;
                        air[u8"批次id"] = batchId;
                        air[u8"编号"] = "A0" + std::to_string(l + 1);
                        air[u8"移交编号"] = std::to_string(randomInt(80));
                        air[u8"使用寿命"] = std::to_string(randomInt(8000));
                        air[u8"剩余寿命"] = std::to_string(randomInt(80));
                        air[u8"引擎型号"] = randomStr(16);
                        air[u8"引擎编号"] = std::to_string(randomInt(80));
                        air[u8"引擎使用寿命"] = std::to_string(randomInt(8000));
                        air[u8"引擎剩余寿命"] = std::to_string(randomInt(800));
                        air[u8"state"] = 0;
                        int airId = insert(L"飞机", air/*nlohmann::basic_json<>::parse(air)*/);
                        createBaseline(1, airId);
                    }
                }
            }
        }
    }

    /*
    int InsertFile(const std::string& db_name, const std::string& file_name) {
        std::ifstream file(file_name, std::ios::in | std::ios::binary);
        if (!file) {
            std::cerr << "An error occurred opening the file\n";
            return 12345;
        }
        file.seekg(0, std::ifstream::end);
        std::streampos size = file.tellg();
        file.seekg(0);

        char* buffer = new char[size];
        file.read(buffer, size);

        sqlite3* db = nullptr;
        int rc = sqlite3_open_v2(db_name.c_str(), &db, SQLITE_OPEN_READWRITE, NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "db open failed: " << sqlite3_errmsg(db) << std::endl;
        } else {
            sqlite3_stmt* stmt = nullptr;
            rc = sqlite3_prepare_v2(db,
                // u8"INSERT INTO \"技术状态文件\"(\"附件\") VALUES(?)",
                u8"UPDATE \"技术状态文件\" SET \"附件\" = ? WHERE id = 1",
                -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                char const* e = sqlite3_errmsg(db);
                std::cerr << "prepare failed: " << sqlite3_errmsg(db) << std::endl;
                // std::cout << "prepare failed: " << sqlite3_errmsg(db) << std::endl;
            } else {
                // SQLITE_STATIC because the statement is finalized
                // before the buffer is freed:
                rc = sqlite3_bind_blob(stmt, 1, buffer, size, SQLITE_STATIC);
                if (rc != SQLITE_OK) {
                    std::cerr << "bind failed: " << sqlite3_errmsg(db) << std::endl;
                } else {
                    rc = sqlite3_step(stmt);
                    if (rc != SQLITE_DONE)
                        std::cerr << "execution failed: " << sqlite3_errmsg(db) << std::endl;
                }
            }
            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);

        delete[] buffer;
    }

    int InsertFile(std::string name) {
        constexpr int MAX = 65535;
            const char* dbname = name.c_str();
            sqlite3* database;
            int rc = sqlite3_open(dbname, &database);
            char* zErrMsg = 0;
            unsigned char* buffer = (unsigned char*) malloc(sizeof(char) * MAX);

            std::ifstream file;
            file.open("Sql.pdf", std::ios::in | std::ios::binary);

            if (!file) {
                std::cout << "An error occurred opening the file" << std::endl;
            }

            int count = 0;

            const void* fileptr = NULL;


            fileptr = buffer;

            while (file.good()) {
                char c = file.get();
                buffer[count] = c;
                count++;
            }
            file.close();


            sqlite3_stmt* stmt = NULL;

            char* statement = "INSERT INTO ONE( ID, NAME, LABEL, GRP, FILE ) VALUES ( NULL, 'fedfsdfss', NULL, NULL, ? );";

            rc = sqlite3_prepare_v2(database, statement, 0, &stmt, NULL);


            rc = sqlite3_bind_blob(stmt, 1, fileptr, sizeof(char) * count, SQLITE_TRANSIENT);


            const char* result = sqlite3_errmsg(database);


            rc = sqlite3_step(stmt);

            result = sqlite3_errmsg(database);

            sqlite3_close(database);


            free(buffer);

            fileptr = NULL;

            return 0;
    }

    int blob() {
        std::string blob = ""; // assume blob is in the string
        std::string query = "INSERT INTO foo (blob_column) VALUES (?);";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, query.c_str(), query.size(), &stmt, nullptr);
        sqlite3_bind_blob(stmt, 1, blob.data(), blob.size(),
            SQLITE_TRANSIENT); // SQLITE_STATIC

        // open sqlite3 database connection
        sqlite3* db;
        sqlite3_open("path/to/database.db", &db);

        // insert blob
        {
            sqlite3_stmt* stmtInsert = nullptr;
            sqlite3_prepare_v2(db, "INSERT INTO table_name (vector_blob) VALUES (?)", -1, &stmtInsert, nullptr);

            std::vector<float> blobData(128); // your data
            sqlite3_bind_blob(stmtInsert, 1, blobData.data(), static_cast<int>(blobData.size() * sizeof(float)), SQLITE_STATIC);

            if (sqlite3_step(stmtInsert) == SQLITE_DONE)
                std::cout << "Insert successful" << std::endl;
            else
                std::cout << "Insert failed" << std::endl;

            sqlite3_finalize(stmtInsert);
        }

        // retrieve blob
        {
            sqlite3_stmt* stmtRetrieve = nullptr;
            sqlite3_prepare_v2(db, "SELECT vector_blob FROM table_name WHERE id = ?", -1, &stmtRetrieve, nullptr);

            int id = 1; // your id
            sqlite3_bind_int(stmtRetrieve, 1, id);

            std::vector<float> blobData;
            if (sqlite3_step(stmtRetrieve) == SQLITE_ROW) {
                // retrieve blob data
                const float* pdata = reinterpret_cast<const float*>(sqlite3_column_blob(stmtRetrieve, 0));
                // query blob data size
                blobData.resize(sqlite3_column_bytes(stmtRetrieve, 0) / static_cast<int>(sizeof(float)));
                // copy to data vector
                std::copy(pdata, pdata + static_cast<int>(blobData.size()), blobData.data());
            }

            sqlite3_finalize(stmtRetrieve);
        }

        sqlite3_close(db);

        return 0;
    }
    */
}

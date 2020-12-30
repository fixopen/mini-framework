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
                  "(1, '�׶���Դ', 0, 1, 1, 0, null, 'static/br.jpg', null, null), \n"
                  "(2, '��ѧ����', 0, 1, 2, 0, null, 'static/tt.jpg', null, null), \n"
                  "(3, '��ʦ��Դ', 0, 1, 3, 0, null, 'static/tr.jpg', null, null), \n"
                  "(4, '�汾�Ķ�', 1, 3, 1, 0, null, 'static/multimedia.png', 'D:\\happychildren\\book\\', null), \n"
                  "(5, '��ͯ��ҥ', 1, 8, 2, 0, null, 'static/music.jpg', 'http://localhost/book/erge/index.html', null), \n"
                  "(6, '�����汾', 1, 8, 3, 0, null, 'static/web.png', 'http://localhost/book/flashbook/index.html', null), \n"
                  "(7, '��ѧ�ж�', 1, 8, 4, 0, null, 'static/classical.png', 'http://127.0.0.1/book/poembook/index.html', null), \n"
                  "(8, '��ȫ����', 1, 8, 5, 0, null, 'static/safe.png', 'http://localhost/book/seaqsp/index.html', null), \n"
                  "(9, '��ʫ����', 1, 8, 6, 0, null, 'static/poetry.png', 'http://127.0.0.1/book/��ʫ/gushiquanbu.html', null), \n"
                  "(10, '������Ϸ', 1, 8, 7, 0, null, 'static/game.png', 'http://127.0.0.1/book/������Ϸ/index.html', null), \n"
                  "(11, 'Ͷ��', 2, 9, 1, 0, null, 'static/project.png', 'http://x.chaoxing.com', null), \n"
                  "(12, '���Ӱװ�', 2, 5, 2, 0, null, 'static/whiteboard.png', 'D:\\happychildren\\Whiteboard\\Software\\Board.exe', null), \n"
                  "(13, '���пμ�', 2, 2, 3, 0, null, 'static/owncourseware.png', 'D:\\happychildren\\myshelf\\', null), \n"
                  "(14, '�ڿ���־', 3, 8, 1, 0, null, 'static/magazine.png', 'http://localhost/book/magazines/index.html', null), \n"
                  "(15, '����ͼ��', 3, 8, 2, 0, null, 'static/book.png', 'http://localhost/book/borrbooks/index.html', null), \n"
                  "(16, '��ѧ', 4, 4, 1, 0, null, null, null, null), \n"
                  "(17, 'С��ӡ', 4, 4, 2, 0, null, null, null, null), \n"
                  "(18, '��ɵ��׶�԰', 4, 4, 3, 0, null, null, null, null), \n"
                  "(19, 'Ӣ��', 4, 4, 4, 1, null, null, null, null), \n"
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
                  "(52, '��ȸ���µ�', 16, 4, 1, 0, null, '/happychildren/books/��ȸ���µ�/cover/��ȸ���µ�.jpg', '/happychildren/books/��ȸ���µ�/', null), \n"
                  "(53, '��·�ϲ�����ȴ����', 16, 4, 2, 0, null, '/happychildren/books/��·�ϲ�����ȴ����/cover/��·�ϲ�����ȴ����.jpg', '/happychildren/books/��·�ϲ�����ȴ����/', null), \n"
                  "(54, 'ƨ�ó�', 16, 4, 3, 0, null, '/happychildren/books/ƨ�ó�/cover/ƨ�ó�.jpg', '/happychildren/books/ƨ�ó�/', null), \n"
                  "(55, '�����ô��', 16, 4, 4, 0, null, '/happychildren/books/�����ô��/cover/�����ô��.jpg', '/happychildren/books/�����ô��/', null), \n"
                  "(56, '��̨��Σ��', 16, 4, 5, 0, null, '/happychildren/books/��̨��Σ��/cover/��̨��Σ��.jpg', '/happychildren/books/��̨��Σ��/', null), \n"
                  "(57, '��è������', 16, 4, 6, 0, null, '/happychildren/books/��è������/cover/��è������.jpg', '/happychildren/books/��è������/', null), \n"
                  "(58, '�ֲ����ҿ�����', 16, 4, 7, 0, null, '/happychildren/books/�ֲ����ҿ�����/cover/�ֲ����ҿ�����.jpg', '/happychildren/books/�ֲ����ҿ�����/', null), \n"
                  "(59, 'ֲ����', 16, 4, 8, 0, null, '/happychildren/books/ֲ����/cover/ֲ����.jpg', '/happychildren/books/ֲ����/', null), \n"
                  "(60, '������ȥ�ı�ɫ��', 17, 4, 1, 0, null, '/happychildren/books/������ȥ�ı�ɫ��/cover/������ȥ�ı�ɫ��.jpg', '/happychildren/books/������ȥ�ı�ɫ��/', null), \n"
                  "(61, '��һ���Ĵ�ɨ��', 17, 4, 2, 0, null, '/happychildren/books/��һ���Ĵ�ɨ��/cover/��һ���Ĵ�ɨ��.jpg', '/happychildren/books/��һ���Ĵ�ɨ��/', null), \n"
                  "(62, '��������������', 17, 4, 3, 0, null, '/happychildren/books/��������������/cover/��������������.jpg', '/happychildren/books/��������������/', null), \n"
                  "(63, '�������꣬��һ��', 17, 4, 4, 0, null, '/happychildren/books/�������꣬��һ��/cover/�������꣬��һ��.jpg', '/happychildren/books/�������꣬��һ��/', null), \n"
                  "(64, 'ħ������', 17, 4, 5, 0, null, '/happychildren/books/ħ������/cover/ħ������.jpg', '/happychildren/books/ħ������/', null), \n"
                  "(65, '���ǵ���״С��', 17, 4, 6, 0, null, '/happychildren/books/���ǵ���״С��/cover/���ǵ���״С��.jpg', '/happychildren/books/���ǵ���״С��/', null), \n"
                  "(66, 'С����ʹ���', 17, 4, 7, 0, null, '/happychildren/books/С����ʹ���/cover/С����ʹ���.jpg', '/happychildren/books/С����ʹ���/', null), \n"
                  "(67, '����¹Ҫ����', 17, 4, 8, 0, null, '/happychildren/books/����¹Ҫ����/cover/����¹Ҫ����.jpg', '/happychildren/books/����¹Ҫ����/', null), \n"
                  "(68, '���ǿ񻶽�', 18, 4, 1, 0, null, '/happychildren/books/���ǿ񻶽�/cover/���ǿ񻶽�.jpg', '/happychildren/books/���ǿ񻶽�/', null), \n"
                  "(69, '�����������', 18, 4, 2, 0, null, '/happychildren/books/�����������/cover/�����������.jpg', '/happychildren/books/�����������/', null), \n"
                  "(70, '�ط���������', 18, 4, 3, 0, null, '/happychildren/books/�ط���������/cover/�ط���������.jpg', '/happychildren/books/�ط���������/', null), \n"
                  "(71, '�����԰', 18, 4, 4, 0, null, '/happychildren/books/�����԰/cover/�����԰.jpg', '/happychildren/books/�����԰/', null), \n"
                  "(72, '���κ�������', 18, 4, 5, 0, null, '/happychildren/books/���κ�������/cover/���κ�������.jpg', '/happychildren/books/���κ�������/', null), \n"
                  "(73, '׷��ʷǰ����', 18, 4, 6, 0, null, '/happychildren/books/׷��ʷǰ����/cover/׷��ʷǰ����.jpg', '/happychildren/books/׷��ʷǰ����/', null);\n";
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
                { L"��֯", { { L"id", ColumnMeta::INTEGER }, { L"���", ColumnMeta::TEXT }, { L"����", ColumnMeta::TEXT }, { L"����", ColumnMeta::INTEGER }, { L"��ַ", ColumnMeta::TEXT }, { L"�绰", ColumnMeta::TEXT }, { L"��ϵ��", ColumnMeta::TEXT }, { L"����Աid", ColumnMeta::INTEGER }, { L"��ע", ColumnMeta::TEXT } } },
                { L"�û�", { { L"id", ColumnMeta::INTEGER }, { L"��¼��", ColumnMeta::TEXT }, { L"����", ColumnMeta::TEXT }, { L"�Ա�", ColumnMeta::INTEGER }, { L"����", ColumnMeta::TEXT }, { L"��֯id", ColumnMeta::INTEGER }, { L"סַ", ColumnMeta::TEXT }, { L"�绰", ColumnMeta::TEXT }, { L"email", ColumnMeta::TEXT }, { L"website", ColumnMeta::TEXT }, { L"����", ColumnMeta::TEXT }, { L"role", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                { L"����", { { L"id", ColumnMeta::INTEGER }, { L"ģ��id", ColumnMeta::INTEGER }, { L"����", ColumnMeta::INTEGER }, { L"����", ColumnMeta::TEXT }, { L"����id", ColumnMeta::INTEGER }, { L"state", ColumnMeta::INTEGER }, { L"��չ", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                { L"״̬", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"���", ColumnMeta::TEXT }, { L"ʹ������", ColumnMeta::TEXT }, { L"state", ColumnMeta::INTEGER }, { L"��չ", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                { L"����", { { L"id", ColumnMeta::INTEGER }, { L"״̬id", ColumnMeta::INTEGER }, { L"���", ColumnMeta::TEXT }, { L"�ɻ���ŷ�Χ", ColumnMeta::TEXT }, { L"��������ŷ�Χ", ColumnMeta::TEXT }, { L"����", ColumnMeta::TEXT }, { L"��Ҫ��������", ColumnMeta::TEXT }, { L"��Ҫ�䱸", ColumnMeta::TEXT }, { L"��Ϳ����", ColumnMeta::INTEGER }, { L"state", ColumnMeta::INTEGER }, { L"��չ", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                { L"����", { { L"id", ColumnMeta::INTEGER }, { L"����Ŀ��", ColumnMeta::INTEGER }, { L"Ŀ��id", ColumnMeta::INTEGER }, { L"state", ColumnMeta::INTEGER }, { L"��չ", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                { L"�ɻ�", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"���", ColumnMeta::TEXT }, { L"���Ƶ�λid", ColumnMeta::INTEGER }, { L"��װ��λid", ColumnMeta::INTEGER }, { L"�ƽ�ʱ��", ColumnMeta::TEXT }, { L"�ƽ����", ColumnMeta::TEXT }, { L"ʹ������", ColumnMeta::INTEGER }, { L"ʣ������", ColumnMeta::INTEGER }, { L"�����ͺ�", ColumnMeta::TEXT }, { L"������", ColumnMeta::TEXT }, { L"����ʹ������", ColumnMeta::INTEGER }, { L"����ʣ������", ColumnMeta::INTEGER }, { L"state", ColumnMeta::INTEGER }, { L"��չ", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                { L"����", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"���", ColumnMeta::INTEGER }, { L"����", ColumnMeta::TEXT }, { L"�ͺ�", ColumnMeta::TEXT }, { L"���Ƶ�λid", ColumnMeta::INTEGER }, { L"extended", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                // { L"softwares", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"���ͽڵ�id", ColumnMeta::INTEGER }, { L"����", ColumnMeta::TEXT }, { L"�汾", ColumnMeta::TEXT }, { L"��Ҫ��", ColumnMeta::TEXT },{ L"���Ƶ�λid", ColumnMeta::INTEGER }, { L"״̬", ColumnMeta::TEXT }, { L"����", ColumnMeta::TEXT }, { L"extended", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                { L"softwares", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"���ͽڵ�id", ColumnMeta::INTEGER }, { L"����", ColumnMeta::TEXT }, { L"�汾", ColumnMeta::TEXT }, { L"��Ҫ��", ColumnMeta::TEXT },{ L"���Ƶ�λid", ColumnMeta::INTEGER }, { L"״̬", ColumnMeta::TEXT }, { L"����", ColumnMeta::TEXT }, { L"extended", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                { L"support_configs", { { L"id", ColumnMeta::INTEGER }, { L"template_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"name", ColumnMeta::TEXT }, { L"table_no", ColumnMeta::INTEGER }/*, { L"table_name", ColumnMeta::TEXT }*/ } },
                { L"����豸����", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"רҵ", ColumnMeta::INTEGER }, { L"���", ColumnMeta::TEXT }, { L"����", ColumnMeta::TEXT }, { L"�ͺ�", ColumnMeta::TEXT }, { L"��;", ColumnMeta::TEXT }, { L"���ױ���", ColumnMeta::TEXT }, { L"����", ColumnMeta::TEXT }, { L"���Ƶ�λid", ColumnMeta::INTEGER }, { L"��չ", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                { L"��еרҵ�������", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"���", ColumnMeta::TEXT }, { L"����", ColumnMeta::TEXT }, { L"�ͺ�(ͼ��)", ColumnMeta::TEXT }, { L"��������", ColumnMeta::TEXT }, { L"����������", ColumnMeta::TEXT }, { L"����ϵͳ", ColumnMeta::TEXT }, { L"��װʹ�ò�λ", ColumnMeta::TEXT }, { L"���Ƶ�λid", ColumnMeta::INTEGER }, { L"extended", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                { L"�������Ŀ¼", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"���", ColumnMeta::TEXT }, { L"���ϱ��", ColumnMeta::TEXT }, { L"�ֲ�����", ColumnMeta::TEXT }, { L"����", ColumnMeta::TEXT }, { L"����ҳ��", ColumnMeta::TEXT }, { L"�ܼ�", ColumnMeta::TEXT }, { L"��չ", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                { L"�����������Ŀ¼", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"���", ColumnMeta::TEXT }, { L"��������", ColumnMeta::TEXT }, { L"�ֲ���ƵĲο���׼", ColumnMeta::TEXT }, { L"��Ҫ��;/ʹ��ʱ��/ʹ�õص�", ColumnMeta::TEXT }, { L"��Ҫʹ�ö���", ColumnMeta::TEXT }, { L"��չ", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                { L"����״̬�ļ�", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"����", ColumnMeta::INTEGER }, { L"previous_version_id", ColumnMeta::INTEGER }, { L"·��", ColumnMeta::TEXT }, { L"���", ColumnMeta::TEXT }, { L"����", ColumnMeta::TEXT }, { L"�汾", ColumnMeta::TEXT }, { L"�޶�ʱ��", ColumnMeta::TEXT }, { L"���ͽڵ�id", ColumnMeta::INTEGER }, { L"����", ColumnMeta::INTEGER }, { L"state", ColumnMeta::INTEGER }, { L"��չ", ColumnMeta::TEXT }, { L"��ע", ColumnMeta::TEXT } } },
                { L"���ټ�", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"��չ", ColumnMeta::TEXT } } },
                { L"˫��ˮ", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"��չ", ColumnMeta::TEXT } } },
                { L"�����������⼰�������", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"info_config_id", ColumnMeta::INTEGER }, { L"��չ", ColumnMeta::TEXT } } },
                { L"change_histories", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"time", ColumnMeta::TEXT }, { L"no", ColumnMeta::TEXT }, { L"control_file_id", ColumnMeta::INTEGER }, { L"description", ColumnMeta::TEXT } } },
                { L"change_history_details", { { L"id", ColumnMeta::INTEGER }, { L"change_history_id", ColumnMeta::INTEGER }, { L"from_file_id", ColumnMeta::INTEGER }, { L"to_file_id", ColumnMeta::INTEGER }, { L"description", ColumnMeta::TEXT } } },
                { L"files", { { L"id", ColumnMeta::INTEGER }, { L"type", ColumnMeta::TEXT }, { L"ext", ColumnMeta::TEXT }, { L"content", ColumnMeta::BLOB }, { L"size", ColumnMeta::INTEGER }, { L"digest", ColumnMeta::TEXT } } },
                { L"info_configs", { { L"id", ColumnMeta::INTEGER }, { L"template_id", ColumnMeta::INTEGER }, { L"parent_id", ColumnMeta::INTEGER }, { L"type", ColumnMeta::INTEGER }, { L"name", ColumnMeta::TEXT }, { L"table_no", ColumnMeta::INTEGER }, { L"table_field_no", ColumnMeta::INTEGER } } },
                { L"histories", { { L"id", ColumnMeta::INTEGER }, { L"����id", ColumnMeta::INTEGER }, { L"name", ColumnMeta::TEXT }, { L"no", ColumnMeta::TEXT }, { L"object", ColumnMeta::TEXT }, { L"abstract", ColumnMeta::TEXT }, { L"operator", ColumnMeta::TEXT }, { L"time", ColumnMeta::TEXT }, { L"control_file_id", ColumnMeta::INTEGER }, { L"description", ColumnMeta::TEXT } } }
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
                pictures += u8"{\"����\": \"" + randomStr(8) + u8"\"}";
            }
            pictures += u8"]";
            return u8"{\"��ʼ����\": \"" + randomStr(60) + u8"\", \"��������\": \"" + randomStr(76) + u8"\", \"ͼƬ\": " + pictures + u8"}";
        }

        std::string randomExternalForm() {
            return u8"{\"����\": " + std::to_string(randomInt(60)) + u8", \"��չ\": " + std::to_string(randomInt(60)) + u8", \"����\": " + std::to_string(randomInt(60)) + u8", \"�������\": " + std::to_string(randomInt(60)) + u8", \"���־�\": " + std::to_string(randomInt(60)) + u8", \"ǰ���־�\": " + std::to_string(randomInt(60)) + u8"}";
        }

        std::string randomFlyCapability() {
            return u8"{\"ʵ������\": \"" + std::to_string(randomInt(60)) + u8"\", \"������M��\": \"" + std::to_string(randomInt(60)) + u8"\", \"�����б���\": \"" + std::to_string(randomInt(60)) + u8"\", \"����\": \"" + std::to_string(randomInt(60)) + u8"\", \"��ɻ��ܾ���\": \"" + std::to_string(randomInt(60)) + u8"\", \"��½���ܾ���\": \"" + std::to_string(randomInt(60)) + u8"\", \"��������\": \"" + std::to_string(randomInt(60)) + u8"\", \"���̺�ʱ\": \"" + std::to_string(randomInt(60)) + u8"\"}";
        }

        std::string randomParameters() {
            return u8"{\"���Ͳ���\": " + randomExternalForm() + u8", \"��������\": \"" + randomStr(60) + u8"\", \"��������\": " + randomFlyCapability() + u8", \"�����ʩ\": \"" + randomStr(60) + u8"\", \"��ԭ��ʩ\": \"" + randomStr(60) + u8"\", \"����\": \"" + randomStr(60) + u8"\"}";
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
                weapons += u8"{\"����\": \"" + std::to_string(randomInt(60)) + u8"\", \"�Ƶ���ҩ\": \"" + std::to_string(randomInt(60)) + u8"\", \"���浯ҩ\": \"" + std::to_string(randomInt(60)) + u8"\", \"���׻���\": \"" + std::to_string(randomInt(60)) + u8"\", \"���ط���\": \"" + std::to_string(randomInt(60)) + u8"\"}";
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
                hangPoints += u8"{\"����\": \"" + std::to_string(randomInt(60)) + u8"\", \"�ͺ�\": \"" + std::to_string(randomInt(60)) + u8"\", \"����\": \"" + std::to_string(randomInt(60)) + u8"\", \"���׵�ҩ\": \"" + std::to_string(randomInt(60)) + u8"\"}";
            }
            hangPoints += u8"]";
            return u8"{\"�������\": " + weapons + u8", \"��������\": " + hangPoints + u8", \"�ҵ�+���������\": \"" + randomStr(80) + u8"\"}";
        }

        std::string randomPower() {
            return u8"{\"�ͺ�\": \"" + randomStr(24) + u8"\", \"����\": " + std::to_string(randomInt(60)) + u8", \"������\": \"" + std::to_string(randomInt(60)) + u8"\", \"��������\": \"" + std::to_string(randomInt(60)) + u8"\", \"���޵�λ\": \"" + randomStr(60) + u8"\"}";
        }

        std::string randomItem() {
            return u8"{\"�ͺ�\": \"" + randomStr(24) + u8"\", \"��Ҫ����\": \"" + randomStr(60) + u8"\", \"�ͺŷ�չ\": \"" + randomStr(60) + u8"\"}";
        }

        std::string randomHang() {
            return u8"{\"�ͺ�\": \"" + randomStr(24) + u8"\", \"����\": " + std::to_string(randomInt(60)) + u8", \"�ͺŷ�չ\": \"" + randomStr(60) + u8"\"}";
        }

        std::string randomSeat() {
            return u8"{\"�ͺ�\": \"" + randomStr(18) + u8"\"}";
        }

        std::string randomCompounds() {
            return u8"{\"����\": " + randomWeapon() + u8", \"����װ��\": " + randomPower() + u8", \"�״�����\": " + randomItem() + u8", \"���ӶԿ�\": " + randomItem() + u8", \"��ҵ���\": " + randomHang() + u8", \"������\": " + std::to_string(randomInt(3)) + u8", \"����\": " + randomSeat() + u8", \"����ʹ������\": \"" + randomStr(60) + u8"\"}";
        }

        void createBaseline(int type, int id) {
            // 5w configurations
            // create baseline
            // std::string baseline = u8"{\"����Ŀ��\": " + std::to_string(type) + u8", \"Ŀ��id\": " + std::to_string(id) + u8", \"state\": 0}";
            nlohmann::json baseline;
            baseline[u8"����Ŀ��"] = type;
            baseline[u8"Ŀ��id"] = id;
            baseline[u8"state"] = 0;
            int baselineId = insert(L"����", baseline/*nlohmann::basic_json<>::parse(baseline)*/);
            // create five thousand configutations of the baseline
            for (int i = 0; i < 10; ++i) {
                // std::string baseline = u8"{\"����id\": " + std::to_string(baselineId) + u8", \"���\": " + std::to_string(i) + u8", \"����\": \"" + randomStr(12) + u8"\"}";
                nlohmann::json configuration;
                configuration[u8"����id"] = baselineId;
                configuration[u8"���"] = i;
                configuration[u8"����"] = randomStr(12);
                configuration.erase(u8"parent_id");
                int oneConfigurationId = insert(L"����", configuration/*nlohmann::basic_json<>::parse(baseline)*/);
                for (int j = 0; j < 10; ++j) {
                    // baseline = u8"{\"����id\": " + std::to_string(baselineId) + u8", \"parent_id\": " + std::to_string(oneConfigurationId) + u8", \"���\": " + std::to_string(j) + u8", \"����\": \"" + randomStr(12) + u8"\"}";
                    configuration[u8"����id"] = baselineId;
                    configuration[u8"parent_id"] = oneConfigurationId;
                    configuration[u8"���"] = i;
                    configuration[u8"����"] = randomStr(12);
                    int twoConfigurationId = insert(L"����", configuration/*nlohmann::basic_json<>::parse(baseline)*/);
                    for (int k = 0; k < 20; ++k) {
                        // baseline = u8"{\"����id\": " + std::to_string(baselineId) + u8", \"parent_id\": " + std::to_string(twoConfigurationId) + u8", \"���\": " + std::to_string(k) + u8", \"����\": \"" + randomStr(12) + u8"\"}";
                        configuration[u8"����id"] = baselineId;
                        configuration[u8"parent_id"] = twoConfigurationId;
                        configuration[u8"���"] = i;
                        configuration[u8"����"] = randomStr(12);
                        int threeConfigurationId = insert(L"����", configuration/*nlohmann::basic_json<>::parse(baseline)*/);
                        for (int l = 0; l < 5; ++l) {
                            // baseline = u8"{\"����id\": " + std::to_string(baselineId) + u8", \"parent_id\": " + std::to_string(threeConfigurationId) + u8", \"���\": " + std::to_string(l) + u8", \"����\": \"" + randomStr(12) + u8"\"}";
                            configuration[u8"����id"] = baselineId;
                            configuration[u8"parent_id"] = threeConfigurationId;
                            configuration[u8"���"] = i;
                            configuration[u8"����"] = randomStr(12);
                            int fourConfigurationId = insert(L"����", configuration/*nlohmann::basic_json<>::parse(baseline)*/);
                            for (int m = 0; m < 5; ++m) {
                                // baseline = u8"{\"����id\": " + std::to_string(baselineId) + u8", \"parent_id\": " + std::to_string(fourConfigurationId) + u8", \"���\": " + std::to_string(m) + u8", \"����\": \"" + randomStr(12) + u8"\"}";
                                configuration[u8"����id"] = baselineId;
                                configuration[u8"parent_id"] = fourConfigurationId;
                                configuration[u8"���"] = i;
                                configuration[u8"����"] = randomStr(12);
                                int fiveConfigurationId = insert(L"����", configuration/*nlohmann::basic_json<>::parse(baseline)*/);
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
            // std::string model = u8"{\"ģ��id\": 1, \"����\": " + std::to_string(randomInt(8)) + u8", \"����\": \"" + randomStr(8) + u8"\", \"state\": 0}";
            nlohmann::json model;
            model[u8"ģ��id"] = 1;
            model[u8"����"] = randomInt(8);
            model[u8"����"] = randomStr(8);
            model[u8"state"] = 0;
            // Log::log(Texts::fromUtf8(model));
            int modelId = insert(L"����", model/*nlohmann::basic_json<>::parse(model)*/);
            for (int j = 0; j < 2; ++j) {
                // create (2) states of one model
                // std::string state = u8"{\"����id\": " + std::to_string(modelId) + u8", \"���\": \"00" + std::to_string(j + 1) + u8"\", \"ʹ������\": \"" + randomStr(80) + u8"\", \"state\": 0}";
                nlohmann::json state;
                state[u8"����id"] = modelId;
                state[u8"���"] = "00" + std::to_string(j + 1);
                state[u8"ʹ������"] = randomStr(80);
                state[u8"state"] = 0;
                // Log::log(Texts::fromUtf8(state));
                int stateId = insert(L"״̬", state/*nlohmann::basic_json<>::parse(state)*/);
                for (int k = 0; k < 4; ++k) {
                    // create (4) batchs of one states
                    // std::string batch = u8"{\"״̬id\": " + std::to_string(stateId) + u8", \"���\": \"0" + std::to_string(k + 1) + u8"\", \"����\": \"" + jsonEscape(randomLayout()) + u8"\", \"��Ҫ��������\": \"" + jsonEscape(randomParameters()) + u8"\", \"��Ҫ�䱸\": \"" + jsonEscape(randomCompounds()) + u8"\", \"state\": 0}";
                    nlohmann::json batch;
                    batch[u8"״̬id"] = stateId;
                    batch[u8"���"] = "0" + std::to_string(k + 1);
                    batch[u8"����"] = randomLayout(); // jsonEscape(randomLayout());
                    batch[u8"��Ҫ��������"] = randomParameters(); // jsonEscape(randomParameters());
                    batch[u8"��Ҫ�䱸"] = randomCompounds(); // jsonEscape(randomCompounds());
                    batch[u8"state"] = 0;
                    int batchId = insert(L"����", batch/*nlohmann::basic_json<>::parse(batch)*/);
                    createBaseline(0, batchId);
                    for (int l = 0; l < 4; ++l) {
                        // create (4) airs of one batchs
                        // std::string air = u8"{\"����id\": " + std::to_string(batchId) + u8", \"���\": \"A0" + std::to_string(l + 1) + u8"\", \"�ƽ����\": \"" + std::to_string(randomInt(80)) + u8"\", \"ʹ������\": " + std::to_string(randomInt(8000)) + u8", \"ʣ������\": " + std::to_string(randomInt(800)) + u8", \"�����ͺ�\": \"" + randomStr(24) + u8"\", \"������\": \"" + std::to_string(randomInt(80)) + u8"\", \"����ʹ������\": " + std::to_string(randomInt(8000)) + u8", \"����ʣ������\": " + std::to_string(randomInt(800)) + u8", \"state\": 0}";
                        nlohmann::json air;
                        air[u8"����id"] = batchId;
                        air[u8"���"] = "A0" + std::to_string(l + 1);
                        air[u8"�ƽ����"] = std::to_string(randomInt(80));
                        air[u8"ʹ������"] = std::to_string(randomInt(8000));
                        air[u8"ʣ������"] = std::to_string(randomInt(80));
                        air[u8"�����ͺ�"] = randomStr(16);
                        air[u8"������"] = std::to_string(randomInt(80));
                        air[u8"����ʹ������"] = std::to_string(randomInt(8000));
                        air[u8"����ʣ������"] = std::to_string(randomInt(800));
                        air[u8"state"] = 0;
                        int airId = insert(L"�ɻ�", air/*nlohmann::basic_json<>::parse(air)*/);
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
                // u8"INSERT INTO \"����״̬�ļ�\"(\"����\") VALUES(?)",
                u8"UPDATE \"����״̬�ļ�\" SET \"����\" = ? WHERE id = 1",
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

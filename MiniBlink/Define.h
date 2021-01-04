#ifndef DEFINE_H
#define DEFINE_H

#include "stdafx.h"
#include "UI/MiniControls.h"
#include <map>
#include <vector>
#include <list>

#define DUI_MSGTYPE_RECYCLE_ITEMDELETE               (_T("recycleitemdelete"))
#define DUI_MSGTYPE_RECYCLE_ITEMRESTORE           (_T("recycleitemrestore"))
#define DUI_MSGTYPE_FINISHED_ITEMDELETE           (_T("finisheditemdelete"))
#define DUI_MSGTYPE_FINISHED_ITEMSELECT           (_T("finisheditemselect"))
#define DUI_MSGTYPE_FINISHED_FILEOPEN               (_T("finishedfileopen"))
#define DUI_MSGTYPE_FINISHED_FOLDEROPEN           (_T("finishedfolderopen"))
#define DUI_MSGTYPE_DOWN_ITEMDELETE               (_T("downitemdelete"))
#define DUI_MSGTYPE_DOWN_ITEMDSTOP                   (_T("downitemstop"))
#define DUI_MSGTYPE_DOWN_ITEMDCONTINUE               (_T("downitemcontinue"))
#define DUI_MSGTYPE_DOWN_ITEMDSELECT               (_T("downitemselect"))

#define MUTEX_ID        _T("{9815E790-EC44-4D5F-A415-3D3CA0CBFF32}")
#define APP_NAME        _T("СƤ������")
#define APP_MAJOR        _T("1.0.1.0")
#define APP_CHANNEL        _T("xiaopi")

#define APP_UPDATE_URL    _T("http://xiazaiqiapi.xiaopi.com/down/bturl/?channel=update")
#define APP_VERSION_URL    _T("http://xiazaiqiapi.xiaopi.com/down/param/?channel=%s")
// ͳ�ƽӿ�
#define STAT_START_URL            _T("http://xiazaiqiapi.xiaopi.com/bt/count/stinfo/")
#define STAT_UPLOADINFO_URL        _T("http://xiazaiqiapi.xiaopi.com/bt/count/uploadinfo/")
#define GET_HEARTBEAT_TIME_URL    _T("http://xiazaiqiapi.xiaopi.com/bt/api/heartbeat/")
#define STAT_BINDAPP_URL        _T("http://xiazaiqiapi.xiaopi.com/bt/count/zsxiazai/")

// �����ؽӿ�
#define BIND_HTTP_PREURL_FORMAT        _T("http://xiazaiqiapi.xiaopi.com/down/bturl/?channel=%s")
#define BIND_BTFILE_PREURL_FORMAT    _T("http://xiazaiqiapi.xiaopi.com/down/bturl/?channel=%s")
#define REQUEST_IP_URL                _T("http://api.xiaopi.com/helper/helperapi/uip/")

#define UPDATE_TEMPDIR        _T("uptemp")
#define UPDATE_TEMPFILE        _T("upgrade.temp")
#define UPDATE_FILE            _T("upgrade.zip")
#define TEMP_DIR_NAME        _T("xpAppUpgrade")
#define TORRENT_DIR            _T("torrentfiles")
#define UP_STATE_KEY        _T("upState")

#define UIEVENT_TRAYICON                WM_USER + 0x1000
#define WM_NOTIFY_CREATEHTTPTASK        WM_USER + 0x1001
#define WM_NOTIFY_OPENBTFILE            WM_USER + 0x1002
#define WM_NOTIFY_CREATEBTTASK            WM_USER + 0x1003
#define WM_NOTIFY_COPYDATA_PROCESS        WM_USER + 0x1004
#define WM_NOTIFY_COPYDATA_BIND            WM_USER + 0x1005
#define WM_NOTIFY_GETVERSION            WM_USER + 0x1006
#define WM_NOTIFY_GETONLINETIME            WM_USER + 0x1007
#define WM_NOTIFY_REMOTE_BIND            WM_USER + 0x1008
#define WM_NOTIFY_COPYDATA_REMOTE_BIND    WM_USER + 0x1009
#define WM_NOTIFY_CREATEHTTPTASKBATCH    WM_USER + 0x1010
#define WM_NOTIFY_RECYCLE_RESTORE_TASK    WM_USER + 0x1011
#define WM_NOTIFY_RECYCLE_DELETE_TASK    WM_USER + 0x1012
#define WM_NOTIFY_RECYCLE_DELETEALLTASK    WM_USER + 0x1013

// ״̬
enum TaskState {
    ERROR_STATE,
    STARTPENDING_STATE,
    NORMAL_STATE,
    STOP_STATE,
    STOPPENDING_STATE,
    FINISHED_STATE,
    RECYCLE_STATE,
};

// ����������������
enum DownType {
    DOWNTYPE_UNKNOW,
    DOWNTYPE_HTTP,
    DOWNTYPE_BT
};

// �ļ�������ɲ���
enum OpType {
    OP_NONE,
    OP_RUN,
    OP_OPENDIR
};

// �����б� ����б� ����վ�б���Ϣ
struct TaskListItemInfo {
    TaskListItemInfo() {
        id = -1;
        memset(fileName, 0, sizeof(fileName));
        totalSize = 0;
        downSize = 0;
        downTime = 0;
        downSpeed = 0;
        isOpen = false;
        linkCnt = 0;
        totalLinkCnt = 0;
        memset(downPath, 0, sizeof(downPath));
        memset(downUrl, 0, sizeof(downUrl));
        memset(uid, 0, sizeof(uid));
        statusChangeTime = 0;
        totalUploadSize = 0;
        creationTime = 0;
        completionTime = 0;
        downloadingPeriod = 0;
        removingToRecycleTime = 0;
        memset(magnetURI, 0, sizeof(magnetURI));
        bIsMultiFile = false;
        autoRunType = OP_NONE;
        memset(channel, 0, sizeof(channel));
        memset(iconPath, 0, sizeof(iconPath));
        taskId = -1;
        memset(szFilePriorities, 0, sizeof(szFilePriorities));
        lastTotalUploadSize = 0;
        bStartSeeding = false;
        bFileNameModify = false;
    }

    int id; // ����ά����id
    TCHAR fileName[260]; // �����ļ���
    LONG64 totalSize; // �ܴ�С
    LONG64 downSize; // �Ѿ����صĴ�С
    TaskState state; // ����״̬
    int downTime; // �Ѿ����ص�ʱ��
    int downSpeed; // b/s
    bool isOpen; // ������ɺ���ļ��Ƿ�򿪹�
    int linkCnt; // ����������
    int totalLinkCnt; // ��������
    char downPath[260]; // ����Ŀ¼
    char downUrl[1024]; // ��������
    DownType downType; // ��������
    char uid[1024]; // �ļ���ʶ���� ����Ψһָ��ĳ���ļ��� http���ر�ʶ��Ϊ�������ӵ�ַ�� btֱ���������info_hash�ֶν���md5
    TaskState beforeRecycleSate; // �Ƶ�����վǮ��״̬
    LONG64 statusChangeTime; // ״̬�仯ʱ��
    LONG64 totalUploadSize; // �ϴ��ܴ�С
    LONG64 creationTime; // ����ʱ��
    LONG64 completionTime; // ���ʱ��
    LONG64 downloadingPeriod; // �������ȼ�
    LONG64 removingToRecycleTime; // �Ƶ�����վʱ��
    char magnetURI[1024]; // �������ӻ����ض�������
    bool bIsMultiFile; // �Ƿ��Ƕ��ļ�
    OpType autoRunType; // ���ؽ�����Ĳ���, 0: �޲����� 1���������У� 2������Դ�����ļ���
    char channel[260]; // ���� xiaopi��other
    char iconPath[260]; // ͼ��·��
    bool bFileNameModify; // �Ƿ����޸Ĺ��ļ���
    // ����ֵû�����ݿ�
    int taskId; // ������������ʱ����е�id
    char szFilePriorities[2049]; // �����д����ص��ļ��Ŀ��أ� 0��ʾ�����أ� 1��ʾ���أ� ��ʽ���£� 1001
    LONG64 lastTotalUploadSize; // �ϴ��ϴ��ܴ�С
    bool bStartSeeding; // �Ƿ��ǵ�һ������
};

// bt�Ƿ�����
enum BTDownType {
    BT_UNDOWN,
    BT_DOWN
};

// bt������Ϣ
struct BTItemInfo {
    BTItemInfo() {
        id = -1;
        status = 0;
        fileIndex = 0;
        visible = 1;
        download = BT_DOWN;
        memset(fileName, 0, sizeof(fileName));
        fileSize = 0;
        fileOffset = 0;
        receivedSize = 0;
    }

    int id;
    int status;
    int fileIndex;
    int visible;
    BTDownType download;
    char fileName[MAX_PATH + 1];
    LONG64 fileSize;
    LONG64 fileOffset;
    LONG64 receivedSize;
};

struct UpLoadItem {
    UpLoadItem() {
        memset(fileName, 0, sizeof(fileName));
        memset(downUrl, 0, sizeof(downUrl));
        memset(channel, 0, sizeof(channel));
        memset(info_tag, 0, sizeof(info_tag));
        type = DOWNTYPE_UNKNOW;
        upSize = 0;
        status = 1;
    }

    char fileName[MAX_PATH + 1]; // �ϴ��ļ���
    char downUrl[1024]; // �ϴ��ļ���
    char channel[MAX_PATH + 1]; // ���� xiaopi other
    DownType type; // �������ͣ�http�� bt�ȣ�
    char info_tag[128]; // �ļ���ʶ���� ����Ψһָ��ĳ���ļ��� http���ر�ʶ��Ϊ�������ӵ�ַ+��С����md5�� btֱ���������info_hash�ֶν���md5
    LONG64 upSize; // �ϴ���С
    int status; // ����״̬�� 0���������֣� 1����������
};

// http����
struct HttpTask {
    HttpTask() {
        memset(szUrl, 0, sizeof(szUrl));
        memset(szSavePath, 0, sizeof(szSavePath));
        memset(szSaveName, 0, sizeof(szSaveName));
        memset(szChannel, 0, sizeof(szChannel));
        memset(szIconPath, 0, sizeof(szIconPath));
        autoRunType = OP_NONE;
        bFileNameModify = false;
    }

    char szUrl[1024];
    char szSavePath[MAX_PATH + 1];
    char szSaveName[MAX_PATH + 1];
    char szChannel[MAX_PATH + 1];
    char szIconPath[MAX_PATH + 1];
    OpType autoRunType; // ���ؽ�����Ĳ���, 0: �޲����� 1���������У� 2������Դ�����ļ���
    bool bFileNameModify; // �Ƿ����޸Ĺ��ļ���
};

typedef std::map<int, BTItemInfo> BTInfoMap;

// bt����
struct BTTask {
    BTTask() {
        memset(szSavePath, 0, sizeof(szSavePath));
        memset(szSeedFile, 0, sizeof(szSeedFile));
        memset(szSaveName, 0, sizeof(szSaveName));
        memset(szInfoHashMd5, 0, sizeof(szInfoHashMd5));
        bIsMultiFile = false;
        memset(szChannel, 0, sizeof(szChannel));
        memset(szIconPath, 0, sizeof(szIconPath));
        autoRunType = OP_NONE;
    }

    BTInfoMap btInfo;
    char szSavePath[MAX_PATH + 1];
    char szSeedFile[MAX_PATH + 1];
    char szSaveName[MAX_PATH + 1];
    char szInfoHashMd5[128];
    bool bIsMultiFile;
    char szChannel[MAX_PATH + 1];
    char szIconPath[MAX_PATH + 1];
    OpType autoRunType; // ���ؽ�����Ĳ���, 0: �޲����� 1���������У� 2������Դ�����ļ���
};

typedef std::map<int, TaskListItemInfo> TaskInfoMap;
typedef std::vector <CDuiString> HistroyDirList;
typedef std::vector <UpLoadItem> UpLoadList;

// status = 0���������֣� 1����ʼ���� 2��ˢ���ϴ�����״̬
enum SeedStatus {
    SEED_STOP = 0,
    SEED_START,
    SEED_UPDATE
};

#define XIAOPI_FILE_DEFAULT _T("xpDown")

inline CDuiString ByteToFitFloat(LONG64 size) {
    CDuiString strSize;
    if (size / 1024 / 1024 / 1024 > 0) {
        float fRate = size / 1024.f / 1024.f / 1024.f;
        strSize.Format(_T("%0.2fG"), fRate);
    } else if (size / 1024 / 1024 > 0) {
        float fRate = size / 1024.f / 1024.f;
        strSize.Format(_T("%0.2fM"), fRate);
    } else if (size / 1024 > 0) {
        float fRate = size / 1024.f;
        strSize.Format(_T("%0.2fKB"), fRate);
    } else {
        strSize.Format(_T("%dB"), size);
    }
    return strSize;
}

inline CDuiString ByteToFitUnit(LONG64 size) {
    CDuiString strSize;
    if (size / 1024 / 1024 / 1024 > 0) {
        float fRate = size / 1024.f / 1024.f / 1024.f;
        strSize.Format(_T("%0.2fG"), fRate);
    } else if (size / 1024 / 1024 > 0) {
        strSize.Format(_T("%dM"), size / 1024 / 1024);
    } else if (size / 1024 > 0) {
        strSize.Format(_T("%dKB"), size / 1024);
    } else {
        strSize.Format(_T("%dB"), size);
    }
    return strSize;
}

#endif

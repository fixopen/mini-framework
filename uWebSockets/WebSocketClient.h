#pragma once

#include "define.h"

#include<iostream>

class UWebSocketsClientEvent;

class UWBESOCKET_EXPORTIMPL CWebSocketClient {
public:
    CWebSocketClient();

    ~CWebSocketClient();

    // ���ӵ�web������
    void Start(std::string url);

    // ������Ϣ
    void sendTextMessage(std::string, OpCode opCode = TEXT);

    // ��������
    void StartTimeEvent(int nMiliseconds);

    // ֹͣ����
    void StopTimeEvent();

    // �˳�websocket����
    void Stop();

    // ���ûص��¼�
    void SetEvent(UWebSocketsClientEvent*);

    void onTimeEvent();

    void SetExtraHeaders(std::map <std::string, std::string>& extraHeaders);

private:
    std::string m_url;
    void* m_pus;
    UWebSocketsClientEvent* m_event;

    int m_timerid;
    std::recursive_mutex m_asyncMutex;
    std::map <std::string, std::string> m_extraHeaders;
    std::atomic<bool> m_bRun;
};

// �ص���
class UWBESOCKET_EXPORTIMPL UWebSocketsClientEvent {
public:
    UWebSocketsClientEvent() {}

    ~UWebSocketsClientEvent() {}

    // ���ӳɹ��ص�
    virtual void onConnection(bool bSuccess) = 0;

    virtual void onDisconnection() = 0;

    virtual void OnMessage(std::string message, CWebSocketClient* client) = 0;

    virtual void onTimeEvent() = 0;

    virtual void onError(int port) = 0;

    virtual void onError(void* user) = 0;
};

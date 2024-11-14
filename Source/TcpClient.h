#pragma once
#include <winsock2.h>
#include <MainScene.h>
#include "math/Vec2.h"


class Actor;



class TcpClient
{
public:
  
    TcpClient(const char* ip, unsigned short port);
    ~TcpClient();

    static TcpClient* get();


    bool SendActorMessage(void* data,char msg);
    bool RecvData();
    bool Select(struct timeval timeout);

    Actor* GetActor() { return mActor; }
    int GetID() { return ID; }
    

    int GetSocketNum() { return mSocket; }


    
    int LastIdx         = 0;
    char mRecvBuf[1024] = {0};
    char mSaveBuf[1024] = {0};

     void CleanSaveBuf(int S_idx, int lenth);

private:

    Actor* mActor = nullptr;
    bool CreateClientSocket(const char* ip, unsigned short port);
    SOCKET mSocket;
    unsigned short mPort;



    short ID;
    u_short mServerPort;


};

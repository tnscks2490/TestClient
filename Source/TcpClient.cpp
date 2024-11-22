#include "TcpClient.h"
#include <stdio.h>
#include <ws2tcpip.h>
#include <conio.h>
#include "Actor.h"
#include "PrePacket.h"
#include "MoveComp.h"

#pragma comment(lib, "ws2_32")
#pragma warning(disable : 4996)

namespace
{
static TcpClient* g = nullptr;
}

bool g_TcpNetworkInited = false;

bool Init_TCP()
{
    if (g_TcpNetworkInited)
        return true;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return true;

    return false;
}

void Close_TCP()
{
    if (g_TcpNetworkInited)
        WSACleanup();

    g_TcpNetworkInited = false;
}

TcpClient::TcpClient(const char* ip, unsigned short port)
{
    g = this;
    printf("클라이언트 생성\n");
    Init_TCP();
    CreateClientSocket(ip, port);
}

TcpClient::~TcpClient()
{
    printf("클라이언트 종료\n");
    closesocket(mSocket);
    Close_TCP();
}


TcpClient* TcpClient::get()
{
    if (!g)
    {
        g = new TcpClient("172.30.1.26", 20202);
        //"172.30.1.26" 학원 IP
        //"192.168.1.105" 집 IP
        //"127.0.0.1" 로컬IP
    }
    return g;
}



bool TcpClient::SendActorMessage(PK_Data data)
{
    char buf[1024] = {0};
    int bufLen     = 0;

    PK_Head head;
    head.dataLen = sizeof(PK_Data);

    memcpy(buf + bufLen, &head, sizeof(PK_Head));
    bufLen += sizeof(PK_Head);

    memcpy(buf + bufLen, &data, sizeof(PK_Data));
    bufLen += sizeof(PK_Data);

    // 보내기
    int r = send(mSocket, buf, bufLen, 0);
    if (r == SOCKET_ERROR)
        return false;

    printf("Send Pos....\n");
    return true;
}

bool TcpClient::RecvData()
{
    int r = recv(mSocket, mSaveBuf + LastIdx, 1024, 0);
    LastIdx += r;
    int RecvIdx    = 0;
    int SaveIdx = 0;
    bool check = true;

    if (r > 0)
    {
        while (check)
        {
            int len = 0;

            PK_Head head;
            memcpy(&head.UniqueID, mSaveBuf + SaveIdx, sizeof(head.UniqueID));
            len += sizeof(PK_Head);

            if (head.UniqueID != 12)
            {
                CleanSaveBuf(SaveIdx, len);
                SaveIdx += sizeof(PK_Head) + head.dataLen;
                if (sizeof(PK_Head) + head.dataLen > LastIdx || LastIdx == 0)
                    check = false;
                continue;
            }

            PK_Data data;
            memcpy(&data, mSaveBuf + SaveIdx, sizeof(PK_Data));
            len += sizeof(PK_Data);

            if (sizeof(PK_Head) + head.dataLen > LastIdx || LastIdx == 0)
                check = false;
            else
            {
                memcpy(mRecvBuf + RecvIdx, mSaveBuf + SaveIdx, len);
                CleanSaveBuf(SaveIdx, len);
                RecvIdx += len;
                SaveIdx += len;

                LastIdx -= len;
            }     
        }
        memmove(mSaveBuf, mSaveBuf + SaveIdx, LastIdx);

        return true;
    }
    return false;
}

bool TcpClient::Select(timeval timeout)
{
    FD_SET rset;

    FD_ZERO(&rset);
    FD_SET(mSocket, &rset);

    int fd_num = select(0, &rset, nullptr, nullptr, &timeout);
    return fd_num > 0;
}

void TcpClient::CleanSaveBuf(int S_idx, int lenth)
{
    for (int i = S_idx; i < S_idx + lenth; i++)
    {
        mSaveBuf[i] = 0;
    }
}

bool TcpClient::CreateClientSocket(const char* ip, unsigned short port)
{
    mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocket == INVALID_SOCKET) return false;

    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(port);

    int r = connect(mSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (r == INVALID_SOCKET)
        return false;

    SOCKADDR_IN my_addr;
    int addr_len = sizeof(my_addr);
    getsockname(mSocket, (SOCKADDR*)&my_addr, &addr_len);
    mPort = ntohs(my_addr.sin_port);

    printf("Connect 성공  ");


    int idx = 0;
    recv(mSocket, (char*)&idx, 4, 0);

    ID = idx;
    printf("ID : %d\n", ID);

    return true;
}

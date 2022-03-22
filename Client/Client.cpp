#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "utils.h"
#include <thread>
#include <iostream>
#include <stdio.h>
#include <string>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
using namespace std;

#define MAXLINE 1024

std::string gbk2utf8(const char* strGBK)
{
    std::string strOutUTF8 = "";
    int n = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
    wchar_t* str1 = new wchar_t[n];
    MultiByteToWideChar(CP_ACP, 0, strGBK, -1, str1, n);
    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
    char* str2 = new char[n];
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
    strOutUTF8 = str2;
    delete[] str1;
    str1 = NULL;
    delete[] str2;
    str2 = NULL;
    return strOutUTF8;
}

std::string utf82gbk(const char* utf8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
    len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr) delete[] wstr;
    return str;
}


int main() {
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //向服务器发起请求
    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充

    string ip;
    u_short port;
    cout << "input your server ip: ";
    cin >> ip;
    cout << "port: ";
    cin >> port;

    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockAddr.sin_port = htons(port);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    char szBuffer[MAXLINE];
    thread([&]() {
        while (1) {
            ZeroMemory(szBuffer, MAXLINE);
            recv(sock, szBuffer, MAXLINE, NULL);
            cout << utf82gbk(szBuffer) << endl;
        }
    }).detach();

    //login pson example
    pson pon;
    cout << "input your name: ";
    cin >> pon["username"];
    pon[KEY_MODULE] = "login";
    pon[KEY_END];

    //cout << pon.toString() << endl;
    string text = gbk2utf8(pon.toString().c_str());
    send(sock, text.c_str(), text.size(), NULL);

    while (1)
    {
        string cmd;
        cin >> cmd;

        string text = gbk2utf8(cmd.c_str());
        send(sock, text.c_str(), text.size(), NULL);
    }

    //关闭套接字
    closesocket(sock);
    //终止使用 DLL
    WSACleanup();
    return 0;
}
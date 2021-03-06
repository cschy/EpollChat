#pragma once

#define MAXLINE     1024  

#include <arpa/inet.h>
#include <string>
#include <map>

struct MyData //like channel?
{
    int fd;
    sockaddr_in userAddr;
    std::string readBuf;
    std::string sendBuf;
};



MyData* recvData(MyData* data);

void sendData(MyData* data);
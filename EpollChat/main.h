#pragma once

#define MAXLINE     1024  

#include <arpa/inet.h>
#include <string>
#include <map>
#include "chat.h"

class Chat;
struct MyData
{
    Chat* business;
    int fd;
    sockaddr_in userAddr;
    std::string readBuf;
    std::string sendBuf;
    
    ~MyData() {
        delete business;
    }
};



MyData* recvData(MyData* data);

void sendData(MyData* data);
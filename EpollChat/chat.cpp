#include "chat.h"

#include "utils.h"

void Chat::accept_callback()
{
	
}

//command
#define MODULE_LOGIN "login"
#define MODULE_USERS "users"
#define MODULE_CHATONE "chatone"
#define MODULE_CHATALL "chatall"

extern std::map<int, MyData*> users;

std::vector<MyData*> Chat::read_parse()
{
	pson request(data->readBuf.c_str());

	//登录
	if (request[KEY_MODULE] == MODULE_LOGIN)
	{
		userName = request["username"];
		if (!userName.empty())
			data->sendBuf = "你好呀，" + userName;
		else
			data->sendBuf = "username字段不能为空";
		return { data };
	}

	//获取用户列表
	if (request[KEY_MODULE] == MODULE_USERS)
	{
		for (auto& i : users)
		{
			//if ((tmpName = i.second->business->getUserName()) != userName)
			data->sendBuf += (i.second->business->getUserName() + '\n');
		}
		data->sendBuf[data->sendBuf.size() - 1] = '\0';
		return { data };
	}

	//私聊
	if (request[KEY_MODULE] == MODULE_CHATONE)
	{
		std::string sendName = request["username"];
		std::string sendText = request["text"];
		if (sendName.empty() || sendText.empty()) {
			data->sendBuf = "username和text字段不能为空";
			return { data };
		}
		
		std::string tmpName;
		for (auto& i : users)
		{
			if ((tmpName = i.second->business->getUserName()) == sendName)
			{
				i.second->sendBuf = userName + ": " + sendText;
				return { i.second };
			}
		}
		data->sendBuf = "can't find user: " + sendName;
		return { data };
	}

	//广播
	if (request[KEY_MODULE] == MODULE_CHATALL)
	{
		std::string sendText = request["text"];
		if (sendText.empty()) {
			data->sendBuf = "text字段不能为空";
			return { data };
		}

		std::vector<MyData*> tmp;
		for (auto& i : users)
		{
			i.second->sendBuf = userName + ": " + sendText;
			tmp.push_back(i.second);
		}
		return tmp;
	}

	data->sendBuf = "unknown module: " + request[KEY_MODULE];
	return { data };
}

void Chat::read_callback()
{
	std::vector<MyData*> mydata = read_parse();
	data->readBuf.clear();

	for (auto& i : mydata)
	{
		sendData(i);
	}
}


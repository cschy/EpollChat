#include "chat.h"

#include "utils.h"

void Chat::accept_callback(MyData* data)
{
	addMyData(data);
}

//command
#define MODULE_LOGIN "login"
#define MODULE_USERS "users"
#define MODULE_CHATONE "chatone"
#define MODULE_CHATALL "chatall"


std::map<MyData*, std::string> Chat::MyData2Name;

std::vector<MyData*> Chat::read_parse(MyData* data)
{
	pson request(data->readBuf.c_str());

	//登录
	if (request[KEY_MODULE] == MODULE_LOGIN)
	{
		if (!request["username"].empty()) {
			for (auto& i : MyData2Name) 
			{
				if (i.second == request["username"]) {
					data->sendBuf = "当前用户名已经有人登录了";
					return { data };
				}
			}
			MyData2Name[data] = request["username"];
			data->sendBuf = "你好呀，" + MyData2Name[data];
		}
		else {
			data->sendBuf = "username字段不能为空";
		}
			
		return { data };
	}

	//获取用户列表
	if (request[KEY_MODULE] == MODULE_USERS)
	{
		for (auto& i : MyData2Name)
		{
			data->sendBuf += (i.second + '\n');
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

		for (auto& i : MyData2Name)
		{
			if (i.second == sendName) {
				MyData* res = i.first;
				res->sendBuf = MyData2Name[data] + " send to you: " + sendText;
				return { res };
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
		for (auto& i : MyData2Name)
		{
			i.first->sendBuf = MyData2Name[data] + ": " + sendText;
			tmp.emplace_back(i.first);
		}
		return tmp;
	}

	data->sendBuf = "unknown module: " + request[KEY_MODULE];
	return { data };
}

void Chat::read_callback(MyData* data)
{
	if (data->readBuf.empty()) {//logout
		MyData2Name.erase(data);
		//clear businesss data...

		deleteMyData(data);
		return;
	}
	//如果明文传输被抓包了，黑客一直发送请求会导致缓冲区爆炸
	std::vector<MyData*> datas = read_parse(data);
	data->readBuf.clear();

	for (auto& i : datas)
	{
		sendData(i);
	}
}


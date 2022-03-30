#pragma once
#include "business.h"
#include <vector>

class Chat : public Business
{
public:
	virtual void accept_callback(MyData* data);
	virtual void read_callback(MyData* data);

protected:
	virtual void addMyData(MyData* data) {
		MyData2Name[data] = std::string(inet_ntoa(data->userAddr.sin_addr)) + ":" + std::to_string(data->userAddr.sin_port);
	}
	virtual void deleteMyData(MyData* data) { delete data; };

	std::vector<MyData*> read_parse(MyData* data);
	static std::map<MyData*, std::string> MyData2Name;
};
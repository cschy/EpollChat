#pragma once
#include "business.h"
#include <vector>

class Chat : protected Business
{
public:
	Chat(MyData* _data = nullptr): Business(_data) {}
	virtual ~Chat() {}

	virtual void accept_callback();
	virtual void read_callback();

public:
	const std::string& getUserName() const {
		return userName;
	}

protected:
	std::vector<MyData*> read_parse();

private:
	std::string userName;
};
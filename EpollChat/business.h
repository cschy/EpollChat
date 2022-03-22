#pragma once
#include "main.h"

class Business
{
public:
	Business(struct MyData* _data): data(_data) {}
	virtual ~Business() {}
	
public:
	virtual void accept_callback() = 0;
	virtual void read_callback() = 0;

protected:
	struct MyData* data;
};
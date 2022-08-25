#pragma once

#include <memory>

class SDLI
{
public:
	SDLI();
	~SDLI();
	int init();
	int deinit();
	int exec();
private:
	enum
	{
		DEFAULT_WIDTH = 640,
		DEFAULT_HEIGHT = 480,
	};
private:
	struct InnerData;
	std::shared_ptr<InnerData> data_;
};
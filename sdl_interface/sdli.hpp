#pragma once

#include <memory>
#include <string>

#include "pub.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

class SDLI
{
public:
	SDLI(const std::string &filename);
	~SDLI();
	int init();
	int deinit();
	int exec();
private:

private:
	enum
	{
		DEFAULT_WIDTH = 640,
		DEFAULT_HEIGHT = 480,
	};
	
	struct InnerData;
	std::shared_ptr<InnerData> data_;

	
};

NAMESPACE_FFMPEG_DEMO_END
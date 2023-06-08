#ifndef _SOURCE_H_
#define _SOURCE_H_
#include "Config.h"

#include <string>
#include <fstream>

#ifdef USE_FILE_SOURCE
class Source
{
	std::ifstream file;
	size_t filesize;
public:
	Source(void* data = nullptr);
	~Source();

	int ReadText_lock(std::string& out, size_t maxLength = ~size_t(0));
	bool is_EOF();
};
#endif

#endif
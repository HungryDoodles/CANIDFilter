#include "Source.h"
#include <stdexcept>
#include <algorithm>

#ifdef USE_FILE_SOURCE

Source::Source(void* data)
{
    const char* filename = reinterpret_cast<const char*>(data);
    if (filename == nullptr || filename[0] == 0)
        throw std::invalid_argument("Filename cannot be null or empty (no default source filename)");

    file.open(filename);
    if (!file.is_open())
        throw std::invalid_argument("Cannot open file");

    auto thisPos = file.tellg();
    file.seekg(0, file.end);
    auto endPos = file.tellg();
    filesize = endPos - thisPos;
    file.seekg(0, file.beg);
}

Source::~Source()
{
    if (file.is_open())
        file.close();
}

int Source::ReadText_lock(std::string& out, size_t maxLength)
{
    if (!file.is_open())
        return 0;

    maxLength = std::min<size_t>(filesize, maxLength);
    out.resize(maxLength);

    char* buf = const_cast<char*>(out.c_str());

    auto oldPos = file.tellg();
    file.read(buf, maxLength);
    auto newPos = file.tellg();
    auto len = oldPos - newPos;
    if (file.eof()) len = maxLength;

    if (len < maxLength)
        buf[len - 1] = '\0';

    return oldPos - newPos;
}

bool Source::is_EOF()
{
    if (file.is_open())
        return file.eof();
    return true;
}

#endif
#include "Config.h"
#include "ArgParser.h"
#include "Source.h"
#include "Sink.h"
#include "Helpers.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdint.h>

#ifdef USE_WIN_SOCK
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif



// Could be in a separate thread if streaming is assumed
int worker(const ArgParser& args)
{
#ifdef USE_WIN_SOCK
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif

	std::string filename;
	args.AggregateAsString("f", filename);

	Source source((void*)filename.c_str());


	std::string ipAddr = "127.0.0.1";
	int16_t port;
	args.GetFirstParam("ip", ipAddr);
	port = args.GetFirstParamOrDefault_slow("port", 1337);

	Sink sink(ipAddr.c_str(), port);


	std::vector<uint32_t> IDs;
	args.TransformParams("id", IDs);

	if (IDs.size() == 0) 
	{
		std::cerr << "No CAN IDs provided (argument '-id')" << std::endl;
		return 1;
	}

#ifdef DUMP_FILTERED
	std::ofstream outf("dump_filtered.txt");
#endif

	std::istringstream ss;
	std::string accumulator = "";
	std::string line;
	std::string readBuf;
	// Loop
	while (!source.is_EOF()) 
	{
		int chars_read = source.ReadText_lock(readBuf, MAX_READ_LENGTH);
		if (chars_read == 0) 
		{
			std::cerr << "Source stream read error" << std::endl;
			return 2;
		}

		accumulator.append(readBuf);

		// Cut lines
		int newLine;
		while ((newLine = accumulator.find_first_of('\n')) != accumulator.npos) 
		{
#ifdef INCLUDE_NEWLINE_SYMBOL
			line = accumulator.substr(0, newLine + 1);
#else
			line = accumulator.substr(0, newLine);
#endif
			accumulator.erase(accumulator.begin(), accumulator.begin() + newLine + 1);

			int addrColBeg = -1, addrColEnd = -1;
			int addrColLen = SeekCol(line.c_str(), 2, addrColBeg, addrColEnd);

			if (addrColLen == 0) 
			{
				std::cerr << "Bad line: " << line << std::endl;
				continue;
			}

			uint32_t addrDword = ~0;
			auto colStr = line.substr(addrColBeg, addrColLen);

			ss.str(colStr);
			ss.seekg(0, ss.beg);
			if (!(ss >> std::hex >> addrDword))
			{
				std::cerr << "Failed reading column " << colStr << " as hex" << std::endl;
				continue;
			}

			if (addrDword == ~0) 
			{
				std::cerr << "Bad address data: " << colStr << std::endl;
				continue;
			}

			uint32_t colID = ~0;
			if (SubInt(addrDword, 20, 12) == 0x1E0)
				colID = SubInt(addrDword, 8, 8);
			else
				colID = SubInt(addrDword, 0, 7);

#ifdef DUMP_FILTERED
			if (std::find(IDs.begin(), IDs.end(), colID) != IDs.end())
			{
#ifdef INCLUDE_NEWLINE
				outf << line;
#else
				outf << line << std::endl;
#endif
				sink.Send(line.c_str(), line.length());
			}
#else
			if (std::find(IDs.begin(), IDs.end(), colID) != IDs.end())
				sink.Send(line.c_str(), line.length());
#endif
		}
	}

	return 0;
}



int main(int argc, const char** argv) 
{
	ArgParser args(argc, argv);
	worker(args);

	return 0;
}
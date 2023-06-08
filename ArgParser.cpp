#include "ArgParser.h"
#include <stdexcept>

using namespace std;

ArgParser::ArgParser(int argc, const char** argv)
	: argc(argc), argv(argv) 
{
	if (argc < 0)
		throw invalid_argument("argc is < 0");
	if (argv == nullptr)
		throw invalid_argument("argv must not be null");

	string thisArg = "";
	for (int i = 0; i < argc; ++i) 
	{
		if (argv[i][0] == '-')
		{
			thisArg = argv[i] + 1;
			if (arg_params_map.find(thisArg) == arg_params_map.end())
				arg_params_map.emplace(thisArg, vector<const char*>());
		}

		else if (thisArg != "")
			arg_params_map[thisArg].push_back(argv[i]);
	}
}

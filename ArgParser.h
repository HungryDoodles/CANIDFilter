#ifndef _ARGPARSER_H_
#define _ARGPARSER_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>

class ArgParser
{
	int argc;
	const char** argv;

	std::unordered_map<std::string, std::vector<const char*>> arg_params_map;

public:
	ArgParser() = delete;
	ArgParser(const ArgParser& other) = delete;
	ArgParser(int argc, const char** argv);

	inline bool HasArg(const char* argName) { return arg_params_map.find(argName) != arg_params_map.end(); }
	inline const std::vector<const char*>* GetParamsOfArg(const char* argName)  const
	{
		auto it = arg_params_map.find(argName);
		return (it == arg_params_map.end()) ? nullptr : &(it->second);
	}

	// out is unchanged if argName is not found
	inline void GetFirstParam(const char* argName, std::string& out) const
	{
		auto it = arg_params_map.find(argName);
		if (it != arg_params_map.end() && it->second.size() >= 1)
			out = std::string(it->second[0]);
	}

	inline void AggregateAsString(const char* argName, std::string& out) const 
	{
		auto it = arg_params_map.find(argName);
		if (it != arg_params_map.end() && it->second.size() >= 1) 
		{
			out.clear();
			for (int i = 0; i < it->second.size() - 1; ++i)
			{
				out.append(it->second[i]);
				out.append(" ");
			}
			out.append(it->second.back());
		}
	}

	template<typename T>
	inline T GetFirstParamOrDefault_slow(const char* argName, T _default = T()) const
	{
		T value = _default;
		auto it = arg_params_map.find(argName);
		if (it != arg_params_map.end() && it->second.size() >= 1) 
		{
			T newValue;
			if (std::istringstream(it->second[0]) >> newValue)
				value = newValue;
		}
		return value;
	}

	template<typename T>
	inline void TransformParams(const char* argName, std::vector<T>& out) const
	{
		auto it = arg_params_map.find(argName);
		if (it != arg_params_map.end()) 
		{
			out.clear();
			out.resize(it->second.size());
			std::transform(it->second.begin(), it->second.end(), out.begin(),
				[](const char* cstr)->T
				{
					T newValue;
					if (std::istringstream(cstr) >> newValue)
						return newValue;
					return T();
				});
		}
	}
};

#endif
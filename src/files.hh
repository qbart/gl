#pragma once
#include "types.hh"
#include <tuple>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <filesystem>

class Files
{
public:
	using ReadStatus = std::tuple<bool, string>;
	using OnRead = std::function<void(const ReadStatus&)>;

	static ReadStatus readText(const string &path)
	{
		std::stringstream ss;
		std::ifstream in;

		in.open(path.c_str(), std::ios::in);

		if (in.is_open())
		{
			string line;
			while (std::getline(in, line))
			{
				ss << line;
				ss << "\n";
			}

			return { true, string(ss.str()) };
		}

		return { false, "" };
	}

	static bool isLoaded(const Files::ReadStatus& status)
	{
		return std::get<0>(status);
	}

	static const string& text(const Files::ReadStatus& status)
	{
		return std::get<1>(status);
	}

	static bool exists(const string &path)
	{
		return std::filesystem::exists(path.c_str());
	}
};
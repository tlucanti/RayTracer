
#pragma once

#include <fstream>
#include <string_view>
#include <string>
#include <stdexcept>
#include <sstream>

#include "Exception.hpp"

namespace tlucanti
{

	class IOError : public Exception
	{
	public:
		IOError(const std::string_view &parent, const std::string_view &msg)
			: Exception(parent, msg) {}
	};

	class EOFError : public IOError
	{
	public:
		EOFError(const std::string &parent, const std::string_view &msg)
			: IOError(parent, msg) {}
	};

	class File
	{
	public:
		File(const std::string_view &fname)
		{
			fin.open(std::string(fname));
			if (not fin.is_open())
				throw IOError("fileIO", "cannot open file " + std::string(fname));
		}

		std::string readline()
		{
			std::string line = _readline_s();
			if (line.empty())
				throw EOFError("fileIO", "end of file");
			return line;
		}

		std::string text()
		{
			char buff[10000];
			fin.read(buff, 10000);
			return { buff };
			std::stringstream buffer;
			buffer << fin.rdbuf();
			return buffer.str();
		}

		~File()
		{
			fin.close();
		}

	private:
		std::ifstream fin;

		std::string _readline_s()
		{
			std::string line;
			std::getline(fin, line);
			return line;
		}

	};
}

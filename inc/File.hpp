
#pragma once

#include <fstream>
#include <string_view>
#include <string>
#include <stdexcept>

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
			fin.open(fname);
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
			char buf[10000] {};
			fin.read(buf, 10000);
			return {buf};

			std::string data;
			std::string line = _readline_s();
			while (not line.empty())
			{
				data += line + '\n';
				line = _readline_s();
			}
			return data;
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

#include "utils.h"
#include "file_stream.h"

namespace zpk::utils
{
	std::vector<uint8_t> readBytesFromFile(const std::string& name) noexcept
	{
		FileStream stream(name);
		return stream.getData();
	}

	std::pair<std::string, std::string> splitFileNameWithExtension(const std::string& path) noexcept
	{
		if (path.find_last_of('.') != std::string::npos)
		{
			const size_t position = path.find_last_of('/');
			if (position != std::string::npos)
				return { path.substr(0, position + 1), path.substr(position + 1) };
			else
				return { "", path };
		}

		return {};
	}

	void appendSlash(std::string& path) noexcept
	{
		const size_t size = path.size();
		if (size > 0 && path[size] != '/')
			path += '/';
	}

	void fixSlashes(std::string& path) noexcept
	{
		char* string = path.data();
		while (*string++ != '\0')
		{
			switch (*string)
			{
			case '\\': case '//':
				*string = '/';
			}
		}
	}

	void eraseFromPath(std::string& path, const uint32_t& count) noexcept
	{
		path = path.substr(count + 1);
	}

	bool isPath(const std::string& path) noexcept
	{
		return path.find_last_of('.') == std::string::npos;
	}

	bool isPathHasFileExtension(const std::string& path) noexcept
	{
		return path.find_last_of('.') != std::string::npos;
	}

	bool isPathHasExtension(const std::string& path, const std::string& extension) noexcept
	{
		const size_t position = path.find_last_of('.');
		return position != std::string::npos ? path.substr(position + 1) == extension : false;
	}
}


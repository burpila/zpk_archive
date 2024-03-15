#ifndef _UTILS_H
#define _UTILS_H
#include "zpk_config.h"

namespace zpk
{
	namespace utils
	{
		std::vector<uint8_t> readBytesFromFile(const std::string& name) noexcept;

		// TODO: std::pair -> FilePathInfo
		/*struct FilePathInfo
		{
			explicit FilePathInfo() noexcept = default;
			explicit FilePathInfo(std::string&& path, std::string&& file_name) noexcept
				: m_path(std::move(path))
				, m_name(std::move(file_name))
			{}

			std::string m_path;
			std::string m_name;
		};*/
		/// <summary>
		/// Splits path into two strings.<para />
		/// 
		/// Example: path = 'test/dir/data.txt'.<para />
		/// First string = 'test/dir/'.<para />
		/// Second string = 'data.txt'.<para />
		/// 
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		std::pair<std::string, std::string> splitFileNameWithExtension(const std::string& path) noexcept;
		/// <summary>
		/// Adds a slash at the end of the path, 
		/// but only if path doesn't have an extension or slash at the end
		/// </summary>
		/// <param name="path"></param>
		void appendSlash(std::string& path) noexcept;
		/// <summary>
		/// ('//' -> '/') : ('\\' -> '/')
		/// </summary>
		/// <param name="path"></param>
		void fixSlashes(std::string& path) noexcept;
		void eraseFromPath(std::string& path, const uint32_t& count) noexcept;
		bool isPath(const std::string& path) noexcept;
		bool isPathHasFileExtension(const std::string& path) noexcept;
		bool isPathHasExtension(const std::string& path, const std::string& extension) noexcept;
	}
}

#endif // !_UTILS_H

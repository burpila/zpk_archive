#include "archive_reader.h"

namespace zpk
{
	Archive* ArchiveReader::read(const std::string& path)
	{
		Archive* archive = new Archive(path);
		return read(archive, path);
	}

	Archive* ArchiveReader::read(Archive* archive, const std::string& path)
	{
		FileStream stream(path);
		archive->m_header = stream.read<ArchiveHeader_t>();

		uint32_t directory_count = stream.read<uint16_t>();
		for (int i = 0; i < directory_count; i++)
		{
			std::string directory_name = stream.read<std::string>();
			uint16_t files_count = stream.read<uint16_t>();
			for (int i = 0; i < files_count; i++)
			{
				std::string file_name = stream.read<std::string>();
				uint32_t size = stream.read<uint32_t>();
				uint32_t offset = stream.read<uint32_t>();

				FileHandle* file_handle = new FileHandle(size, offset);
				archive->writeDirectory(directory_name, file_name, file_handle);
			}
		}

		return archive;
	}
}


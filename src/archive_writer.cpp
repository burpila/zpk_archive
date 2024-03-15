#include "archive_writer.h"

namespace zpk
{
	inline uint32_t getDirectoryAndFilesSize(Archive* archive) noexcept
	{
		uint32_t size = 0;

		for (const auto& [path, directory] : archive->getPaths())
		{
			size += path.size();
			for (const auto& [name, file] : directory)
				size += name.size();
		}

		return size;
	}

	Archive* ArchiveWriter::createEmpty(Archive* archive, const std::string& name)
	{
		return nullptr;
	}

	Archive* ArchiveWriter::createFromDirectory(Archive* archive, const std::string& name)
	{
		archive->m_header = ArchiveHeader_t{ consts::ZPK_ID, consts::ZPK_VERSION };
		std::filesystem::recursive_directory_iterator files(name);
		
		for (const auto& file : files)
		{
			const auto& path = file.path();

			if (!std::filesystem::is_directory(path))
				archive->writeDirectory(name, path.string());
		}

		ArchiveWriter::finalize(name + ".zpk", archive);

		return archive;
	}

	Archive* ArchiveWriter::createEmpty(const std::string& name)
	{
		return nullptr;
	}

	Archive* ArchiveWriter::createFromDirectory(const std::string& name)
	{
		return nullptr;
	}

	bool ArchiveWriter::finalize(const std::string& name, Archive* archive)
	{
		std::ofstream file_stream(name, std::ios::trunc | std::ios::binary);
		ZPK_RETURN(!file_stream.is_open(), false);

		file_stream.write(reinterpret_cast<const char*>(&archive->m_header), sizeof(ArchiveHeader_t));

		uint16_t directories_count = archive->m_paths.size();
		uint16_t files_count = archive->getFilesCount();
		uint32_t base_offset = 
			sizeof(ArchiveHeader_t)
			+ sizeof(uint16_t)
			+ directories_count
			+ files_count * sizeof(uint16_t)
			+ files_count
			+ files_count * sizeof(uint64_t)
			+ getDirectoryAndFilesSize(archive);

		file_stream.write(reinterpret_cast<const char*>(&directories_count), sizeof(uint16_t));
		for (const auto& [path, directory] : archive->m_paths)
		{
			uint16_t files_count = directory.size();

			file_stream.write(path.data() + '\0', path.size() + 1);
			file_stream.write(reinterpret_cast<const char*>(&files_count), sizeof(uint16_t));

			for (const auto& [name, file] : directory)
			{
				uint32_t size = file->getSize();

				file_stream.write(name.data() + '\0', name.size() + 1);
				file_stream.write(reinterpret_cast<const char*>(&size), sizeof(uint32_t));
				file_stream.write(reinterpret_cast<const char*>(&base_offset), sizeof(uint32_t));
				file->setOffset(base_offset);
				base_offset += size;
			}
		}

		for (const auto& [path, directory] : archive->m_paths)
		{
			for (const auto& [name, file] : directory)
			{
				Archive::FileDirectory* file_directory = static_cast<Archive::FileDirectory*>(file);

				auto data = file_directory->getData();
				uint32_t size = file_directory->getSize();

				file_stream.write(reinterpret_cast<const char*>(data.data()), size);
				file_directory->clear();
			}
		}

		return true;
	}
}


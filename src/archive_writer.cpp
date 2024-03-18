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
		FileStream stream(name, std::ios::trunc | std::ios::binary | std::ios::out);
		ZPK_RETURN(!stream.isOpen(), false);

		stream.write(archive->m_header);

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

		stream.write(directories_count);
		for (const auto& [path, directory] : archive->m_paths)
		{
			uint16_t files_count = directory.size();

			stream.write(path);
			stream.write(files_count);

			for (const auto& [name, file] : directory)
			{
				uint32_t size = file->getSize();

				stream.write(name);
				stream.write(size);
				stream.write(base_offset);
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
				stream.write(data);
				file_directory->clear();
			}
		}

		return true;
	}
}


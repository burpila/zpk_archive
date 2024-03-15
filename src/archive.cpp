#include "archive.h"
#include "utils.h"

namespace zpk
{
	FileStream Archive::m_fileStream;

	Archive::Archive(const std::string& path) noexcept
		: m_fullPath(path)
	{}

	Archive::~Archive() noexcept
	{
		for (const auto& [name, directory] : m_paths)
		{
			for (auto& [file_name, file] : directory)
				delete file;
		}
	}

	void Archive::open() noexcept
	{
		m_fileStream.open(m_fullPath);
	}

	void Archive::close() noexcept
	{
		m_fileStream.close();
	}

	std::vector<uint8_t> Archive::loadFile(const std::string& path) const
	{
		FileHandle* file = this->findFile(path);
		return this->loadFile(file);
	}

	std::vector<uint8_t> Archive::loadFile(FileHandle* file) const
	{
		ZPK_RETURN(!file, {});

		if (m_fileStream.isOpen())
			return m_fileStream.read(file->getSize(), file->getOffset());

		FileStream stream;
		return stream.read(m_fullPath, file);
	}

	void Archive::setPath(const std::string& path) noexcept
	{
		m_fullPath = path;
	}

	FileHandle* Archive::findFile(const std::string& path) const
	{
		ZPK_RETURN(!utils::isPathHasFileExtension(path), nullptr);

		const auto& [full_path, file_name] = utils::splitFileNameWithExtension(path);
		const auto& directory_path = m_paths.find(full_path);

		if (directory_path != m_paths.end())
		{
			const auto& directory = directory_path->second;
			const auto& file = directory.find(file_name);
			return file != directory.end() ? file->second : nullptr;
		}

		return nullptr;
	}

	Archive::Directory Archive::findFiles(const std::string& path, const std::string& extension) const
	{
		ZPK_RETURN(!utils::isPath(path), {});

		const auto& directory_path = m_paths.find(path);
		if (directory_path != m_paths.end())
		{
			if (extension.size() > 0)
			{
				Directory files {};
				for (const auto& [name, file] : directory_path->second)
				{
					if (utils::isPathHasExtension(name, extension))
						files.emplace(name, file);
				}
				return files;
			}

			return directory_path->second;
		}

		return {};
	}

	bool Archive::isPathExists(const std::string& path) const
	{
		ZPK_RETURN(!utils::isPath(path), false);

		const auto& directory_path = m_paths.find(path);
		return directory_path != m_paths.end();
	}

	bool Archive::isFileExists(const std::string& path) const
	{
		ZPK_RETURN(!utils::isPathHasFileExtension(path), false);

		const auto& [full_path, file_name] = utils::splitFileNameWithExtension(path);
		const auto& directory_path = m_paths.find(full_path);

		if (directory_path != m_paths.end())
		{
			const auto& directory = directory_path->second;
			return directory.find(file_name) != directory.end();
		}

		return false;
	}

	Archive::Path Archive::getPaths() const noexcept
	{
		return m_paths;
	}

	uint32_t Archive::getFilesCount() const noexcept
	{
		uint32_t size = 0;

		size = std::accumulate(m_paths.begin(), m_paths.end(), 0, 
			[](uint32_t previous_size, const auto& directory) 
			{
				return previous_size + directory.second.size();
			});

		return size;
	}

	uint32_t Archive::getSize() const noexcept
	{
		uint32_t size = 0;

		size = std::accumulate(m_paths.begin(), m_paths.end(), 0,
			[](uint32_t previous_size, const auto& directory)
			{
				for (auto& [name, file] : directory.second)
					previous_size += file->getSize();

				return previous_size;
			});

		return size;
	}

	uint32_t Archive::getDirectorySize(Directory directory) const noexcept
	{
		uint32_t size = 0;

		size = std::accumulate(directory.begin(), directory.end(), 0,
			[](uint32_t previous_size, const auto& file)
			{
				return previous_size + file.second->getSize();
			});

		return size;
	}

	void Archive::writeDirectory(const std::string& archive_name, std::string& path)
	{
		auto bytes = utils::readBytesFromFile(path);

		utils::fixSlashes(path);
		utils::eraseFromPath(path, archive_name.size());
		const auto& [file_path, file_name] = utils::splitFileNameWithExtension(path);
		FileDirectory* file_directory = new FileDirectory(bytes);
		m_paths[file_path][file_name] = file_directory;
	}

	void Archive::writeDirectory(const std::string& path, const std::string& name, FileHandle* file)
	{
		m_paths[path][name] = file;
	}

	void Archive::FileDirectory::clear() noexcept
	{
		std::vector<uint8_t>().swap(m_data);
	}

	std::vector<uint8_t> Archive::FileDirectory::getData() const noexcept
	{
		return m_data;
	}
}

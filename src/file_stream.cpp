#include "file_stream.h"

namespace zpk
{
	FileStream::FileStream(const std::string& path) noexcept
	{
		open(path, std::ios::binary | std::ios::in);
	}

	FileStream::FileStream(const std::string& path, const OpenMode& mode) noexcept
	{
		open(path, mode);
	}

	FileStream::FileStream(const std::string& path, const uint32_t& size, const uint32_t& offset) noexcept
	{

	}

	FileStream::~FileStream() noexcept
	{
		close();
	}

	size_t FileStream::open(const std::string& path, const OpenMode& mode) noexcept
	{
		m_file.open(path, mode);
		ZPK_RETURN(!m_file.is_open(), 0);
		size_t size = 0;

		if (mode & std::ios::in)
		{
			size = m_file.tellg();
			m_file.seekg(0, std::ios::end);
			size = static_cast<size_t>(m_file.tellg()) - size;
			m_file.seekg(std::ios::beg);

			m_data.resize(size);
			m_file.read(reinterpret_cast<char*>(m_data.data()), size);
			m_file.close();
		}
		
		return size;
	}

	void FileStream::close() noexcept
	{
		std::vector<uint8_t>().swap(m_data);
		m_file.close();
	}

	void FileStream::setSeek(const uint32_t& position) noexcept
	{
		m_offset = position;
	}

	bool FileStream::isOpen() const noexcept
	{
		return m_file.is_open();
	}

	const std::vector<uint8_t> FileStream::getData() const noexcept
	{
		return m_data;
	}

	std::vector<uint8_t> FileStream::read(const uint32_t& size, const uint32_t& offset) noexcept
	{
		return std::vector<uint8_t>();
	}

	std::vector<uint8_t> FileStream::read(const std::string& path, FileHandle* file) noexcept
	{
		std::ifstream file_stream(path, std::ios::ate | std::ios::binary);
		ZPK_RETURN(!file_stream.is_open(), {});

		const uint32_t size = file->getSize();
		file_stream.seekg(file->getOffset());
		m_data.resize(size);

		file_stream.read(reinterpret_cast<char*>(m_data.data()), size);
		file_stream.close();

		return m_data;
	}
}


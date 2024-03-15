#include "file_stream.h"

namespace zpk
{
	FileStream::FileStream(const std::string& path) noexcept
	{
		open(path);
	}

	FileStream::FileStream(const std::string& path, const uint32_t& size, const uint32_t& offset) noexcept
	{

	}

	void FileStream::open(const std::string& path) noexcept
	{
		std::ifstream file_stream(path, std::ios::ate | std::ios::binary);
		ZPK_RETURN(!file_stream.is_open());

		const size_t size = file_stream.tellg();
		file_stream.seekg(std::ios::beg);

		m_data.resize(size);
		file_stream.read(reinterpret_cast<char*>(m_data.data()), size);
		file_stream.close();
	}

	void FileStream::close() noexcept
	{
		std::vector<uint8_t>().swap(m_data);
	}

	void FileStream::setSeek(const uint32_t& position) noexcept
	{
		m_offset = position;
	}

	bool FileStream::isOpen() const noexcept
	{
		return m_data.size() > 0;
	}

	const std::vector<uint8_t> FileStream::getData() const noexcept
	{
		return m_data;
	}

	std::vector<uint8_t> FileStream::read(const uint32_t& size, const uint32_t& offset) noexcept
	{
		return std::vector<uint8_t>();
	}

	std::vector<uint8_t> FileStream::read(const std::string& path,
		const uint32_t& size, const uint32_t& offset) noexcept
	{
		std::ifstream file_stream(path, std::ios::ate | std::ios::binary);
		ZPK_RETURN(!file_stream.is_open(), {});

		file_stream.seekg(offset);
		m_data.resize(size);

		file_stream.read(reinterpret_cast<char*>(m_data.data()), size);
		file_stream.close();

		return m_data;
	}
}


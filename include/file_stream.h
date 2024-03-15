#ifndef _FILE_STREAM_H
#define _FILE_STREAM_H
#include "zpk_config.h"
#include "file_handle.h"

namespace zpk
{
	class FileStream
	{
	public:
		explicit FileStream() noexcept = default;
		explicit FileStream(const std::string& path) noexcept;
		explicit FileStream(const std::string& path, const uint32_t& size, const uint32_t& offset) noexcept;
		~FileStream() noexcept = default;

		void open(const std::string& path) noexcept;

		void close() noexcept;
		void setSeek(const uint32_t& position) noexcept;

		bool isOpen() const noexcept;

		const std::vector<uint8_t> getData() const noexcept;

		template <class _T>
		_T read() noexcept
		{
			if constexpr (std::is_pointer<_T>::value)
			{
				_T* value = new _T();
				std::memcpy(&value, data.data() + offset, sizeof(_T));
				m_offset += sizeof(_T);
				return value;
			}
			else
			{
				_T value;
				std::memcpy(&value, data.data() + offset, sizeof(_T));
				m_offset += sizeof(_T);
				return value;
			}
		}

		template <>
		uint8_t read<uint8_t>() noexcept
		{
			return m_data[m_offset++];
		}

		template <>
		uint16_t read<uint16_t>() noexcept
		{
			uint16_t value = (m_data[m_offset + 1] << 8) | (m_data[m_offset]);
			m_offset += 2;
			return value;
		}

		template <>
		uint32_t read<uint32_t>() noexcept
		{
			uint32_t value = (m_data[m_offset + 3] << 24) | (m_data[m_offset + 2] << 16)
				| (m_data[m_offset + 1] << 8) | m_data[m_offset];
			m_offset += 4;
			return value;
		}

		template <>
		std::string read<std::string>() noexcept
		{
			std::string result;
			char temp = m_data[m_offset++];
			while (temp != '\0')
			{
				result += temp;
				temp = m_data[m_offset++];
			}

			return result;
		}

		std::vector<uint8_t> read(const uint32_t& size, const uint32_t& offset) noexcept;

		std::vector<uint8_t> read(const std::string& path, FileHandle* file) noexcept;
	private:
		std::vector<uint8_t> m_data {};
		uint32_t m_offset = 0;
	};
}

#endif // !_FILE_STREAM_H

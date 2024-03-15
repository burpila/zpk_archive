#ifndef _FILE_HANDLE_H
#define _FILE_HANDLE_H
#include "zpk_config.h"

namespace zpk
{
	class FileHandle
	{
	public:
		FileHandle() noexcept = default;
		FileHandle(const uint32_t& size, const uint32_t& offset) noexcept
			: m_size(size)
			, m_offset(offset)
		{}
		virtual ~FileHandle() noexcept = default;

		void setOffset(const uint32_t& offset) noexcept;
		void setSize(const uint32_t& size) noexcept;

		const uint32_t getOffset() const noexcept;
		const uint32_t getSize() const noexcept;

		virtual std::string getExtension() const noexcept
		{
			return "";
		}
	private:
		FileHandle(const FileHandle&) = delete;
		FileHandle(FileHandle&&) = delete;
	protected:
		uint32_t m_offset = 0;
		uint32_t m_size = 0;
	};
}

#endif // !_FILE_HANDLE_H

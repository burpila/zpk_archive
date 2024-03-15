#include "file_handle.h"

namespace zpk
{
	void FileHandle::setOffset(const uint32_t& offset) noexcept
	{
		m_offset = offset;
	}

	void FileHandle::setSize(const uint32_t& size) noexcept
	{
		m_size = size;
	}

	const uint32_t FileHandle::getOffset() const noexcept
	{
		return m_offset;
	}

	const uint32_t FileHandle::getSize() const noexcept
	{
		return m_size;
	}
}
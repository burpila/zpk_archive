#ifndef _ARCHIVE_READER_H
#define _ARCHIVE_READER_H
#include "zpk_config.h"
#include "archive.h"

namespace zpk
{
	class ArchiveReader
	{
	public:
		static Archive* read(const std::string& path);
		static Archive* read(Archive* archive, const std::string& path);
	};
}

#endif // !_ARCHIVE_READER_H

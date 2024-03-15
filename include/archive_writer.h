#ifndef _ARCHIVEWRITER_H
#define _ARCHIVEWRITER_H
#include "zpk_config.h"
#include "archive.h"

namespace zpk
{
	class ArchiveWriter
	{
	public:
		static Archive* createEmpty(Archive* archive, const std::string& name);
		static Archive* createFromDirectory(Archive* archive, const std::string& name);

		static Archive* createEmpty(const std::string& name);
		static Archive* createFromDirectory(const std::string& name);
	private:
		static bool finalize(const std::string& name, Archive* archive);
	};
}

#endif // !_ARCHIVEWRITER_H

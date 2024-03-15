#include <zpk.h>

namespace tools
{
	constexpr static inline std::string_view HELP_STRING(
		"Usage\n"
		"  zpk [options] <path>\n"
		"\n"
		"Options\n"
		"  -c          : Create archive by specific path.\n"
		"  -d          : Copy all files into folder with archive name.\n"
		"  -i          : Prints information about archive.\n"
		"\n");
	constexpr static inline uint32_t DEFAULT_OFFSET_BASIS = 0x811C9DC5;
	constexpr static inline uint32_t PRIME = 0x01000193;
}

/*
* Credit: https://gist.github.com/filsinger
*/
constexpr static inline uint32_t hashString(const char* string, 
	const uint32_t& offset = tools::DEFAULT_OFFSET_BASIS) noexcept
{
	return (string[0] == '\0') ? 
		offset : hashString(&string[1], (offset * tools::PRIME) ^ uint32_t(string[0]));
}

static inline std::vector<std::string> getArguments(int count, char* arguments[]) noexcept
{
	std::vector<std::string> args {};
	for (int i = 1; i < count; i++)
		args.push_back(arguments[i]);

	return args;
}

int main(int argc, char* argv[])
{
	using namespace zpk;

	const auto arguments = getArguments(argc, argv);
	Archive* archive = new Archive();

	if (arguments.size() < 1)
		std::cout << tools::HELP_STRING << std::endl;

	static bool print_archive_info = false;

	for (int i = 0; i < arguments.size(); i++)
	{
		switch (hashString(arguments[i].data()))
		{
		case hashString("-i"):
			print_archive_info = true;
			break;
		case hashString("-c"):
			{
				auto path = arguments.at(++i);
				archive->setPath(path + ".zpk");
				ArchiveWriter::createFromDirectory(archive, path);
				break;
			}
		case hashString("-d"):
			{
				auto path = arguments.at(++i);
				archive->setPath(path);
				ArchiveReader::read(archive, path);
				print_archive_info = true;
				break;
			}
		}
	}

	if (print_archive_info)
	{
		std::cout << "==========main==========" << std::endl;
		std::cout << "Files count: " << archive->getFilesCount() << std::endl;
		std::cout << "All files size: " << archive->getSize() << std::endl;
		std::cout << "Directories: " << std::endl;

		for (const auto& [name, directory] : archive->getPaths())
			std::cout << "Path: \'" << (name.empty() ? "(main)" : name) << "\'" << std::endl;
		std::cout << "========================" << std::endl;

		for (const auto& [name, directory] : archive->getPaths())
		{
			std::cout << "------------------------" << std::endl;

			std::cout << "Directory: \'" << (name.empty() ? "(main)" : name) << "\' Size: "
				<< archive->getDirectorySize(directory) << std::endl;
			for (const auto& [file_name, file] : directory)
				std::cout << "File name: \'" << file_name << "\' Size: " << file->getSize() << std::endl;

		}
		std::cout << "------------------------" << std::endl;
	}

	delete archive;
	return 0;
}
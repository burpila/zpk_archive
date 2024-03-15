#include <zpk.h>

int main(int argc, char* argv[])
{
	zpk::Archive* read_archive = new zpk::Archive("test.zpk");
	zpk::FileHandle* file_data = read_archive->findFile("resources/resource.txt");
	auto data = read_archive->loadFile(file_data);
	if (!data.empty())
		std::cout << data.data() << std::endl;

	delete read_archive;

	return 0;
}
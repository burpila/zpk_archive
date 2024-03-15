#include <zpk.h>

int main(int argc, char* argv[])
{
	zpk::Archive* create_archive = new zpk::Archive("test");
	delete create_archive;

	return 0;
}
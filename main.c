#include "utils.h"

int main(int argc, char** argv)
{
	#ifdef DEBUG
	Dutils_help(argv[0]);
	#endif

	return 0;
}
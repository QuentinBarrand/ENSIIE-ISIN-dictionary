#include "args.h"

int main(int argc, char** argv)
{
	#ifdef DEBUG
	Dargs_help(argv[0]);
	#endif

	return 0;
}
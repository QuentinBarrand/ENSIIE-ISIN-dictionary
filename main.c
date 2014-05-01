#include "Dconfig.h"
#include "Ddictionary.h"


int main(int argc, char** argv)
{
    Dconfig* config;
    config = Ddictionary_parseArgs(argc, argv);

    return 0;
}
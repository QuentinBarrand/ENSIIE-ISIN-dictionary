#include <stdlib.h>

#include "Dconfig.h"
#include "Ddictionary.h"


int main(int argc, char** argv)
{
    // Get the config from the command-line arguments
    Dconfig* config;
    config = Ddictionary_parseArgs(argc, argv);

    // Trigger functions required by configuration and get the dictionary
    Dnode* dictionary;
    dictionary = Ddictionary_processArgs(config);

    // Run the interactive mode


    // Free the config
    free(config);

    return 0;
}
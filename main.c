#include "Dconfig.h"
#include "Ddictionary.h"


int main(int argc, char** argv)
{
    // Get the config from the command-line arguments
    Dconfig* config;
    config = Dconfig_new();

    Ddictionary_parseArgs(config, argc, argv);

    // Trigger actions required by configuration
    Ddictionary_processArgs(config);

    // Get the dictionary from configuration
    Dnode* dictionary;
    dictionary = Ddictionary_create(config);

    // Run the interactive mode


    // Free the config
    Dconfig_free(config);

    free(config);

    return 0;
}
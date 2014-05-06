#include "Dconfig.h"
#include "Ddictionary.h"
#include "Dnode.h"


int main(int argc, char** argv)
{
    // Get the config from the command-line arguments
    Dconfig* config;
    config = Dconfig_new();

    if(! Ddictionary_parseArgs(config, argc, argv))
    {
        Dconfig_free(config);
        return 1;
    }

    // Trigger actions required by configuration and get the dictionary
    Dnode* dictionary;
    dictionary = Dnode_new();

    if(! Ddictionary_processArgs(config, dictionary))
    {
        Dconfig_free(config);
        return 1;
    }

    // Run the interactive mode
    Ddictionary_runInteractive(config, dictionary);

    // Free the config
    Dconfig_free(config);

    // Free the dictionary
    Ddictionary_free(dictionary);

    return 0;
}
#include <stdlib.h>

#include "application.h"
#include "Dconfig.h"
#include "Ddictionary.h"

/** The aplication's main entry point.
 *
 * \param argc the number or arguments.
 * \param argv the array of arguments.
 *
 * \returns A status code the parent process. 
 */
int main(int argc, char** argv)
{
    int returnValue;

    // Get the config from the command-line arguments
    Dconfig* config;
    config = Dconfig_new();

    if(! Dconfig_parseArgs(config, argc, argv))
    {
        Dconfig_free(config);
        return EXIT_FAILURE;
    }

    Ddictionary* dict;
    dict = Ddictionary_new();

    // Trigger actions required by configuration and initialize the dictionary
    if(processArgs(config, dict))
    {
        // Run the interactive mode
        runInteractive(config, dict);

        returnValue = EXIT_SUCCESS;
    }
    else
    {
        returnValue = EXIT_FAILURE;
    }

    // Free the config
    Dconfig_free(config);

    // Free the dictionary
    Ddictionary_free(dict);

    return returnValue;
}

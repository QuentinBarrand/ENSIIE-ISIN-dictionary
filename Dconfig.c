#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Dconfig.h"
#include "Dutils.h"

/*******************************************************************************
 * Extern functions
 */

/** Allocates a new Dconfig object with all of its attributes set to the `0`
 * value.
 *
 * \returns A new, freshly allocated Dconfig object.
 */
extern Dconfig*
Dconfig_new()
{
    return calloc(1, sizeof(Dconfig));
}

/** Frees a Dconfig object and all of its dynamically allocated attributes.
 *
 * \param config The Dconfig object to be freed.
 */
extern void
Dconfig_free(Dconfig* config)
{
    int i;

    for(i = 0; i < config->commandsNb; i++)
    {
        free(config->commands[i]);
    }

    free(config->commands);

    free(config);
}

/** Parses the program's arguments and populates a Dconfig object.
 *
 * \param config a Dconfig object to be populated.
 * \param argc the program's number of arguments.
 * \param argv the program's arguments.
 *
 * \returns `0` if there was an error, else `1`.
 */
extern int
Dconfig_parseArgs(Dconfig* config, int argc, char** argv)
{
    char* currentArg;
    int i;

    // There should be at least 2 arguments; if not, we print the help
    if(argc < 2)
    {
        fprintf(stderr, "%s : Merci: de fournir un fichier dans lequel lire "
            " les définitions de bases et de synonymes."
            "\nVoir %s -h pour l'aide."
            "\n", argv[0], argv[0]);

        return FALSE;
    }

    config->execName = argv[0];

    for(i = 1; i < argc; i++)
    {
        currentArg = argv[i];

        if(currentArg[0] != '-')
        {
            // If this is the first time we are seing a dash-less argument
            if(! config->definitionsPath)
            {
                config->definitionsPath = currentArg;
            }
            else
            {
                fprintf(stderr, "%s : Argument %s non reconnu."
                    "\n", argv[0], currentArg);
            }
        }
        else
        {
            // -if <commands> is the only argument where strlen(2) > 2
            if(strlen(currentArg) > 2)
            {
                if(strcmp(currentArg, "-if") == 0)
                {
                    // If nothing has been supplied after -if (last argument)
                    if(i == argc - 1)
                    {
                    fprintf(stderr, "%s : -if doit être suivi d'un nom de "
                        "fichier de commandes."
                        "\n", argv[0]);
                    }
                    else
                    {
                        config->commandsPath = argv[++i];
                    }
                }
                else
                {
                    fprintf(stderr, "%s : Argument %s non reconnu."
                        "\n", argv[0], currentArg);
                }

                continue;
            }

            // All the other arguments
            switch(currentArg[1])
            {
                case 'd':
                    config->d_option = TRUE;
                    break;

                case 'h':
                    config->h_option = TRUE;
                    break;

                case 'p':
                    config->p_option = TRUE;
                    break;

                default:
                    fprintf(stderr, "%s : Argument %s non reconnu."
                        "\n", argv[0], currentArg);
            }
        }
    }

    return TRUE;
}

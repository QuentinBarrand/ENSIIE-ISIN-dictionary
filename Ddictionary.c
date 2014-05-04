#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Ddictionary.h"

#include "Dconfig.h"
#include "Dnode.h"
#include "Dutils.h"


/*******************************************************************************
 * Static declarations
 */

static void
Ddictionary_help(char*);

static int
Ddictionary_readCommands(Dconfig*);

static int
Ddictionary_readDefinitions(Dconfig*, Dnode*);

/*******************************************************************************
 * Static functions
 */

static void 
Ddictionary_help(char* execName)
{
    printf("Usage : %s <options> <definitions>"
        "\n"
        "\n<options>"
        "\n\t-h            Afficher cette aide"
        "\n\t-d            Afficher pour chaque requête le nombre de cases "
            "parcourues"
        "\n\t-if <file>    Utiliser le fichier file à la place de l'entrée "
            "standard pour lire les commandes"
        "\n\t-p            Imprime sous un format lisible la structure de "
            "données"
        "\n"
        "\n<definitions>"
        "\n\tFichier de définitions de bases et de synonymes"
        "\n", execName);
}

static int
Ddictionary_readCommands(Dconfig* config)
{
    int i, j;
    char c;

    FILE* stream;

    if((stream = fopen(config->commandsPath, "r")) == NULL)
    {
        fprintf(stderr, "%s : Impossible d'ouvrir en lecture le fichier de "
            " définitions %s. Vérifiez que le fichier existe et réessayez."
            "\n", config->execName, config->definitionsPath);

        return FALSE;        
    }

    while((c = fgetc(stream)) != EOF )
    {
        if(c == '\n')
        {
            config->commandsNb++;
        }
    }

    config->commandsNb++;

    // Allocate config->commands with the correct number of lines.
    config->commands = malloc(config->commandsNb * sizeof(char*));

    stream = freopen(config->commandsPath, "r", stream);

    for(i = 0; i < config->commandsNb; i++)
    {
        /* This parameter is actually not used by getline() since 
         * config->commands[i] is NULL.
         * getline() thus ignores it, allocates the appropriate amount of memory
         * and puts a pointer to that place into config->commands[i].
         */
        size_t length;
        length = getline(&config->commands[i], &length, stream);

        // config->commands[i] contains all the line, we remove '\n'.
        config->commands[i][length - 1] = '\0';
    }

    fclose(stream);

    // Check if all the commands look like actual ones
    for(i = 0; i < config->commandsNb; i++)
    {
        if(strncmp(config->commands[i], "base ", 5) &&
            strncmp(config->commands[i], "deri ", 5) &&
            strncmp(config->commands[i], "syno ", 5) &&
            strncmp(config->commands[i], "info ", 5) &&
            strncmp(config->commands[i], "BASE ", 5) &&
            strncmp(config->commands[i], "DERI ", 5) &&
            strncmp(config->commands[i], "SYNO ", 5) &&
            strncmp(config->commands[i], "INFO ", 5))
        {
            fprintf(stderr, "%s : commande non reconnue dans le fichier "
                "de commandes %s ligne %d : \"%s\" - ignorée"
                "\n", config->execName, config->commandsPath, i + 1,
                config->commands[i]);

            // Remove the item from the array
            free(config->commands[i]);
            
            for(j = i; j < config->commandsNb - 1; j++)
            {
                config->commands[j] = config->commands[j + 1];
            }

            config->commandsNb--;
            i--;
        }
    }

    return TRUE;
}

static int
Ddictionary_readDefinitions(Dconfig* config, Dnode* dictionary)
{
    // Check if config->definitionsPath is not NULL
    if(config->definitionsPath == NULL)
    {
        fprintf(stderr, "%s : Merci: de fournir un fichier dans lequel lire "
            " les définitions de bases et de synonymes."
            "\nVoir %s -h pour l'aide."
            "\n", config->execName, config->execName);

        return FALSE;
    }

    // TODO
    return TRUE;
}

/*******************************************************************************
 * Extern functions
 */

/** Parses the program's arguments and populates a Dconfig object.
 *
 * \param config a Dconfig object to be populated.
 * \param argc the program's number of arguments.
 * \param argv the program's arguments.
 *
 * \returns `0` if there was an error, else `1`.
 */
extern int 
Ddictionary_parseArgs(Dconfig* config, int argc, char** argv)
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
            if(config->definitionsPath == NULL)
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

extern int
Ddictionary_processArgs(Dconfig* config, Dnode* dictionary)
{
    // Help ?
    if(config->h_option)
    {
        Ddictionary_help(config->execName);

        // Let's stop here if nothing else was supplied
        if(config->definitionsPath == NULL)
        {
            return FALSE;
        }
    }

    // Import the definitions file
    if(! Ddictionary_readDefinitions(config, dictionary))
    {
        return FALSE;
    }

    // Import the commands file
    if(! Ddictionary_readCommands(config))
    {
        return FALSE;
    }

    return TRUE;
}

extern void
Ddictionary_runInteractive(Dconfig* config, Dnode* dictionary)
{
    // TODO
}
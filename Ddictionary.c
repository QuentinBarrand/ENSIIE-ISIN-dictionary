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

/*******************************************************************************
 * Static functions
 */

static void 
Ddictionary_help(char* execName)
{
    printf("Usage : %s <options> <definitions>"
        "\n<definitions> : Fichier de définitions de bases et de synonymes"
        "\n"
        "\nOptions :"
        "\n\t-h            Afficher cette aide"
        "\n\t-d            Afficher pour chaque requête le nombre de cases "
            "parcourues"
        "\n\t-if <file>    Utiliser le fichier file à la place de l'entrée "
            "standard pour lire les commandes"
        "\n\t-p            Imprime sous un format lisible la structure de "
            "données"
        "\n", execName);
}

/*******************************************************************************
 * Extern functions
 */

extern Dnode*
Ddictionary_create(Dconfig* config)
{

}

extern void 
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
        exit(ARGS_ERROR);
    }

    // config->execName = argv[0];

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
}

extern void
Ddictionary_processArgs(Dconfig* config)
{
    // So here, we should have a not null config->definitionsPath
    if(config->definitionsPath == NULL)
    {
        fprintf(stderr, "%s : Merci: de fournir un fichier dans lequel lire "
            " les définitions de bases et de synonymes."
            "\nVoir %s -h pour l'aide."
            "\n", config->execName, config->execName);
        exit(ARGS_ERROR);
    }
}

extern void
Ddictionary_runInteractive(Dconfig* config, Dnode* dictionary)
{
    // TODO
}
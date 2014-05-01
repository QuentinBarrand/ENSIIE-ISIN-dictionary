#include <stdio.h>
#include <stdlib.h>

#include "Ddictionary.h"

#include "Dconfig.h"
#include "Dutils.h"


void Ddictionary_help(char* execName)
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
            "données", execName);
}


extern Dconfig*
Ddictionary_parseArgs(int argc, char** argv)
{
    char* currentArg;
    int i, definitionFileSeen;

    // There should be at least 2 arguments; if not, we print the help
    if(argc < 2)
    {
        Ddictionary_help(argv[0]);
        exit(ARGS_ERROR);
    }

    Dconfig* config;
    config = (Dconfig*)malloc(sizeof(Dconfig));

    definitionFileSeen = FALSE;

    for(i = 0; i < argc; i++)
    {
        currentArg = argv[i];

        if(currentArg[0] != '-')
        {
            if(! definitionFileSeen)
            {
                definitionFileSeen = TRUE;
                config->definitionsPath = currentArg;
            }
        }
    }

    return config;
}
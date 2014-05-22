#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "application.h"
#include "Dconfig.h"
#include "Ddictionary.h"


/*******************************************************************************
 * Static declarations
 */

static void 
help(char*);

static void
printDwordList(DwordList*, int*);

static void
printTree(Dnode*);

static bool
readCommands(Dconfig*);

static bool
readDefinitions(Dconfig*, Ddictionary*);

static void
runCommand(char*, Ddictionary*, Dconfig*);

/*******************************************************************************
 * Static functions
 */

static void 
help(char* execName)
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

/** Prints all the words in a DwordList object.
 *
 * \param list the list containing the Dword->word to be printed.
 * \param counter a pointer to an int counter to increment.
 */
static void
printDwordList(DwordList* list, int* counter)
{
    if(list)
    {
        if(list->next)
        {
            printDwordList(list->next, counter);
        }

        ++*counter;
        printf("\t%s\n", list->word->word);
    }
}

/** Prints the dictionary on the standard output.
 *
 * \param tree the Ddictionary.tree to print.
 */
static void
printTree(Dnode* tree)
{
    int i;

    if((Dword*)tree->element)
    {
        if(((Dword*)tree->element)->isWord)
        {
            printf("%s\n", ((Dword*)tree->element)->word);
        }
    }

    for(i = 0; i < ALPHABET_SIZE; i++)
    {
        if(tree->children[i])
        {
            printTree(tree->children[i]);
        }
    }
}

/** Reads the commands written beforehand in the commands file.
 * Ignores commands that are not correctly formatted.
 *
 * \param config the application's configuration.
 */
static bool
readCommands(Dconfig* config)
{
    int i, j;
    char c;

    FILE* stream;

    if(! (stream = fopen(config->commandsPath, "r")))
    {
        fprintf(stderr, "%s : Impossible d'ouvrir en lecture le fichier de "
            " commandes %s. Vérifiez que le fichier existe et réessayez."
            "\n", config->execName, config->commandsPath);

        return false;
    }

    while((c = fgetc(stream)) != EOF)
    {
        if(c == '\n')
        {
            config->commandsNb++;
        }
    }

    config->commandsNb++;

    // Allocate config->commands with the correct number of lines.
    config->commands = calloc(1, config->commandsNb * sizeof(char*));

    stream = freopen(config->commandsPath, "r", stream);

    for(i = 0; i <= config->commandsNb; i++)
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

    return true;
}

/** Reads the dictionary's configuration file.
 *
 * \param config the application's configuration.
 * \param dict the Ddictionary object to be populated.
 *
 * \returns `0` if an error occured, else `1`.
 */
static bool
readDefinitions(Dconfig* config, Ddictionary* dict)
{
    FILE* stream;

    char buffer[MAX_WORD_SIZE],
        tempBase[MAX_WORD_SIZE],
        tempSynonym[MAX_WORD_SIZE];

    int nbBases, nbSynonyms, i;

    Dword* currentWord;

    // Check if config->definitionsPath is not NULL
    if(! config->definitionsPath)
    {
        fprintf(stderr, "%s : Merci: de fournir un fichier dans lequel lire "
            " les définitions de bases et de synonymes."
            "\nVoir %s -h pour l'aide."
            "\n", config->execName, config->execName);

        return false;
    }

    // Open a file descriptor
    if(! (stream = fopen(config->definitionsPath, "r")))
    {
        fprintf(stderr, "%s : Impossible d'ouvrir en lecture le fichier de "
            "définitions %s. Vérifiez que le fichier existe et réessayez."
            "\n", config->execName, config->definitionsPath);

        return false;
    }

    // Read a base and two integers
    while(fscanf(stream, "%s %d %d", buffer, &nbBases, &nbSynonyms) != EOF)
    {
        // First, add the word to the dictionary
        currentWord = Ddictionary_getOrAddWord(dict, buffer);

        // Then, add its bases and link them together
        for(i = 0; i < nbBases; i++)
        {
            fscanf(stream, "%s", tempBase);

            // Get the base's memory address
            Dword* base;
            base = Ddictionary_getOrAddWord(dict, tempBase);

            Dword_addBase(currentWord, base);
        }

        // Finally, add the synonyms
        for(i = 0; i < nbSynonyms; i++)
        {
            fscanf(stream, "%s", tempSynonym);

            // Get the synonym's memory address
            Dword* synonym;
            synonym = Ddictionary_getOrAddWord(dict, tempSynonym);

            Dword_addSynonym(currentWord, synonym);
        }
    }

    fclose(stream);

    return true;
}

/** Runs a command in a specified dictionary.
 *
 * \param command the command to be executed.
 * \param dict the dictionary in which the command should be executed.
 * \param config the application's configuration.
 */
static void
runCommand(char* command, Ddictionary* dict, Dconfig* config)
{
    int i;
    char c1, c2;

    bool valid;
    valid = false;

    // Looks like a valid command
    if(strlen(command) > 5)
    {
        Dword* word;

        char* argument;
        argument = strndup(command + 5, 20);

        if(strncmp(command, "BASE ", 5) == 0 ||
            strncmp(command, "base ", 5) == 0)
        {
            dict->counter = 0;

            word = Ddictionary_getOrAddWord(dict, argument);

            printf("Bases du mot \"%s\" :\n", argument);
            printDwordList(word->bases, &dict->counter);
            
            if(config->d_option)
            {
                printf("Cases parcourues : %d\n", dict->counter);                            
            }
        }
        else if(strncmp(command, "DERI ", 5) == 0 ||
            strncmp(command, "deri ", 5) == 0)
        {
            if(config->d_option)
            {
                dict->counter = 0;
            }

            word = Ddictionary_getOrAddWord(dict, argument);

            printf("Dérivés du mot \"%s\" :\n", argument);
            printDwordList(word->derivatives, &dict->counter);

            if(config->d_option)
            {
                printf("Cases parcourues : %d\n", dict->counter);                            
            }
        }
        else if(strncmp(command, "SYNO ", 5) == 0 ||
            strncmp(command, "syno ", 5) == 0)
        {
            if(config->d_option)
            {
                dict->counter = 0;
            }

            word = Ddictionary_getOrAddWord(dict, argument);

            printf("Synonymes du mot \"%s\" :\n", argument);
            printDwordList(word->synonyms, &dict->counter);

            if(config->d_option)
            {
                printf("Cases parcourues : %d\n", dict->counter);                            
            }
        }
        else if(strncmp(command, "INFO ", 5) == 0 ||
            strncmp(command, "info ", 5) == 0)
        {
            DwordList* list;
            DwordList* tempList;
            DwordList* tempList_orig;

            tempList = NULL;

            i = 0;

            while(argument[i] != '\0' && argument[i + 1] != '\0')
            {
                // Check if the regex is correctly formatted
                if((argument[i] >= 'a' && argument[i] <= 'z') &&
                   (argument[i + 1] >= 'a' && argument[i + 1] <= 'z'))
                {
                    valid = true;

                    c1 = argument[i] - 'a';
                    c2 = argument[i + 1] - 'a';

                    list = dict->hashTable[c1 * 26 + c2];

                    while(list)
                    {
                        if(Dword_doesMatch(list->word, argument))
                        {
                            if(! DwordList_contains(tempList, list->word->word))
                            {
                                /* Populate a temporary list that we can filter 
                                 * to avoid printing 2 times the same word.
                                 */
                                DwordList_add(&tempList, list->word);
                            }
                        }

                        list = list->next;
                    }
                }

                i++;
            }

            if(! valid)
            {
                fprintf(stderr, "L'expression régulière n'est pas "
                    "correctement formattée.\n ");
            }
            else
            {
                printf("Mots correspondant à l'expression %s : \n", argument);

                tempList_orig = tempList;

                while(tempList)
                {
                    printf("\t%s\n", tempList->word->word);
                    tempList = tempList->next;
                }

                // Free the temporary list
                DwordList_free(tempList_orig);
            }
        }
        else
        {
            // If we're here, the command is invalid
            fprintf(stderr, "Commande non reconnue.\n ");
        }

        printf("\n");

        free(argument);
    }
}

/*******************************************************************************
 * Extern functions
 */

/** Triggers actions requested by the configuration/
 *
 * \param config the application's configuration.
 * \param dict an initialized Dnode dictionary.
 *
 * \returns `0` if there was an error, else `1`.
 */
extern bool
processArgs(Dconfig* config, Ddictionary* dict)
{
    int i;

    // Help ?
    if(config->h_option)
    {
        help(config->execName);

        // Let's stop here if nothing else was supplied
        if(! config->definitionsPath)
        {
            return false;
        }
    }

    // Import the definitions file
    if(! readDefinitions(config, dict))
    {
        return false;
    }

    // Import the commands file
    if(config->commandsPath &&
        ! readCommands(config))
    {
        return false;
    }

    // Print data structure if needed
    if(config->p_option)
    {
        printf("Affichage de la structure de données :\n");
        printTree(dict->tree);
        printf("\n");
    }

    // Run all the commands prepared in the commands file
    for(i = 0; i < config->commandsNb; i++)
    {
        printf("\nCommande : %s\n", config->commands[i]);
        runCommand(config->commands[i], dict, config);
    }

    return true;
}

/** Runs an interactive session. Starts an infinite loop waiting for commands
 * from the user.
 *
 * \param config the application's configuration.
 * \param dict the dictionary in which commands should be executed.
 */
extern void
runInteractive(Dconfig* config, Ddictionary* dict)
{
    char input[30];

    while(true)
    {
        printf("Entrez une commande (BASE, DERI, SYNO, INFO) ou q pour quitter"
        " : ");
        fgets(input, 30, stdin);

        // Remove newline ('\n') character
        input[strlen(input) - 1] = '\0';

        // Want to quit ?
        if(input[0] == 'q' && input[1] == '\0')
        {
            return;
        }

        runCommand(input, dict, config);
    }
}

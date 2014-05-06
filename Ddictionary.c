#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Ddictionary.h"

#include "Dconfig.h"
#include "Dnode.h"
#include "Dutils.h"
#include "Dword.h"

/*******************************************************************************
 * Static declarations
 */

static void
Ddictionary_help(char*);

static Dword*
Ddictionary_getOrAddWord(Dnode*, char*);

static void
Ddictionary_print(Dnode*);

static int
Ddictionary_readCommands(Dconfig*);

static int
Ddictionary_readDefinitions(Dconfig*, Dnode*);

static void
Ddictionary_runCommand(char*, Dnode*);

/*******************************************************************************
 * Static functions
 */

/** Gets the Dnode object for a given word in the given dictionary, or creates
 * it if it does not exist yet.
 *
 * \param tree the dictionary to look into.
 * \param word the word to look for in the give dictionary.
 *
 * \returns A pointer to the Dnode object that contains the queried word.
 */
static Dword*
Ddictionary_getOrAddWord(Dnode* tree, char* word)
{
    char tempString[MAX_WORD_SIZE];

    char c;
    uint i;

    Dnode* currentNode;
    currentNode = tree;

    for(i = 0; i < strlen(word); i++)
    {
        c = word[i];

        strncpy(tempString, word, i);
        
        if(! currentNode->children[c - 'a'])
        {
            currentNode->children[c - 'a'] = Dnode_new();
            currentNode->children[c - 'a']->element = Dword_new(tempString);
        }

        currentNode = currentNode->children[c - 'a'];
        // strncpy(currentNode->word, word, i);
    }

    // strncpy(currentNode->word, word, i);

    if(strcmp(((Dword*)currentNode->element)->word, word) == 0)
    {
        ((Dword*)currentNode->element)->isWord = 1;
    }

    return ((Dword*)currentNode->element);
}

/** Displays the application's help.
 *
 * \param execName the executable's name that was used to start the program.
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

/** Prints the dictionary on the standard output.
 *
 * \param tree the dictionary to print.
 */
static void
Ddictionary_print(Dnode* tree)
{
    int i;

    if(((Dword*)tree->element)->isWord)
    {
        printf("%s\n", ((Dword*)tree->element)->word);
        // #ifdef DEBUG
        // printf("Synonyms : ");
        // Dnode_getSynonyms(tree  , tree->word);
        // #endif
    }

    for(i = 0; i < 26; i++)
    {
        if(tree->children[i])
        {
            Ddictionary_print(tree->children[i]);
        }
    }
}

/** Reads the commands written beforehand in the commands file.
 * Ignores commands that are not correctly formatted.
 *
 * \param config the application's configuration.
 */
static int
Ddictionary_readCommands(Dconfig* config)
{
    int i, j;
    char c;

    FILE* stream;

    if(! (stream = fopen(config->commandsPath, "r")))
    {
        fprintf(stderr, "%s : Impossible d'ouvrir en lecture le fichier de "
            " commandes %s. Vérifiez que le fichier existe et réessayez."
            "\n", config->execName, config->commandsPath);

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

/** Reads the dictionary's configuration file.
 *
 * \param config the application's configuration.
 * \param dictionary the Dnode dictionary to be populated.
 *
 * \returns `0` if an error occured, else `1`.
 */
static int
Ddictionary_readDefinitions(Dconfig* config, Dnode* dictionary)
{
    FILE* stream;

    char buffer[MAX_WORD_SIZE],
        tempBase[MAX_WORD_SIZE],
        tempSynonym[MAX_WORD_SIZE];

    int nbBases, nbSynonyms, i;

    // Check if config->definitionsPath is not NULL
    if(! config->definitionsPath)
    {
        fprintf(stderr, "%s : Merci: de fournir un fichier dans lequel lire "
            " les définitions de bases et de synonymes."
            "\nVoir %s -h pour l'aide."
            "\n", config->execName, config->execName);

        return FALSE;
    }

    // Open a file descriptor
    if(! (stream = fopen(config->definitionsPath, "r")))
    {
        fprintf(stderr, "%s : Impossible d'ouvrir en lecture le fichier de "
            "définitions %s. Vérifiez que le fichier existe et réessayez."
            "\n", config->execName, config->definitionsPath);

        return FALSE;        
    }

    // Read a base and two integers
    while(fscanf(stream, "%s %d %d", buffer, &nbBases, &nbSynonyms) != EOF)
    {
        // First, add the base to the dictionary
        Ddictionary_getOrAddWord(dictionary, buffer);

        for(i = 0; i < nbBases; i++)
        {
            fscanf(stream, "%s", tempBase);

            // Dnode_addBaseToDerivative(dictionary, buffer, tempBase);
            // Dnode_addDerivativeToBase(dictionary, tempBase, buffer);
        }

        for(i = 0; i < nbSynonyms; i++)
        {
            fscanf(stream, "%s", tempSynonym);
            
            // Dnode_addSynonym(dictionary, buffer, tempSynonym);
        }
    }

    fclose(stream);

    return TRUE;
}

/** Runs a command in a specified dictionary.
 *
 * \param command the command to be executed.
 * \param dictionary the dictionary in which the command should be executed.
 */
static void
Ddictionary_runCommand(char* command, Dnode* dictionary)
{
    // Looks like a valid command
    if(strlen(command) > 5)
    {
        char* searchedWord;
        searchedWord = strndup(command + 5, 20);

        Dword* word;
        word = Ddictionary_getOrAddWord(dictionary, searchedWord);

        if(strncmp(command, "BASE ", 5) == 0 ||
            strncmp(command, "base ", 5) == 0)
        {
            printf("Bases du mot \"%s\" :\n", searchedWord);
            Dword_printBases(word);
        }
        else if(strncmp(command, "DERI ", 5) == 0 ||
            strncmp(command, "deri ", 5) == 0)
        {
            printf("Dérivés du mot \"%s\" :\n", searchedWord);
            Dword_printDerivatives(word);
        }
        else if(strncmp(command, "SYNO ", 5) == 0 ||
            strncmp(command, "syno ", 5) == 0)
        {
            printf("Synonymes du mot \"%s\" :\n", searchedWord);
            Dword_printSynonyms(word);
        }
        else if(strncmp(command, "INFO ", 5) == 0 ||
            strncmp(command, "info ", 5) == 0)
        {
            // Process INFO
            // TODO
        }
        else
        {
            // If we're here, the command is invalid
            fprintf(stderr, "Commande non reconnue.\n ");
        }

        printf("\n");
    }
}

/*******************************************************************************
 * Extern functions
 */

/** Frees recursively a Dnode dictionary and all of its dynamically allocated 
 * attributes.
 *
 * \param tree The Dnode dictionary to be freed.
 */
extern void 
Ddictionary_free(Dnode* tree)
{
    Dnode_free(tree, Dword_free);
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

/** Triggers actions requested by the configuration/
 *
 * \param config the application's configuration.
 * \param dictionary an initialized Dnode dictionary.
 *
 * \returns `0` if there was an error, else `1`.
 */
extern int
Ddictionary_processArgs(Dconfig* config, Dnode* dictionary)
{
    int i;

    // Help ?
    if(config->h_option)
    {
        Ddictionary_help(config->execName);

        // Let's stop here if nothing else was supplied
        if(! config->definitionsPath)
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
    if(config->commandsPath &&
        ! Ddictionary_readCommands(config))
    {
        return FALSE;
    }

    // Print data structure if needed
    if(config->p_option)
    {
        printf("Affichage de la structure de données :\n");
        Ddictionary_print(dictionary);
        printf("\n");
    }

    // Run all the commands prepared in the commands file
    for(i = 0; i < config->commandsNb; i++)
    {
        printf("\nCommande : %s\n", config->commands[i]);
        Ddictionary_runCommand(config->commands[i], dictionary);
    }

    return TRUE;
}

/** Runs an interactive session. Starts an infinite loop waiting for commands
 * from the user.
 *
 * \param config the application's configuration.
 * \param dictionary the dictionary in which commands should be executed.
 */
extern void
Ddictionary_runInteractive(Dconfig* config, Dnode* dictionary)
{
    char input[30];

    while(TRUE)
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

        Ddictionary_runCommand(input, dictionary);
    }
}

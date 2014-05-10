#include <stdbool.h>
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
Ddictionary_getOrAddWord(Ddictionary*, char*, bool);

static void
Ddictionary_printTree(Dnode*);

static int
Ddictionary_readCommands(Dconfig*);

static int
Ddictionary_readDefinitions(Dconfig*, Ddictionary*);

static void
Ddictionary_runCommand(char*, Ddictionary*);

/*******************************************************************************
 * Static functions
 */

/** Gets the Dword object for a given word in the given dictionary, or creates
 * it if it does not exist yet.
 *
 * \param dict the dictionary to look into.
 * \param word the word to look for in the give dictionary.
 * \param addToList a boolean variable. If `true`, the word is added to the
 *    Ddictionary.words list.
 *
 * \returns A pointer to the Dword object that contains the queried word.
 */
static Dword*
Ddictionary_getOrAddWord(Ddictionary* dict, char* word, bool addToList)
{
    char tempWord[MAX_WORD_SIZE];
    int c, nbChars;
    int i;

    Dnode* tree;
    tree = dict->tree;

    nbChars = strlen(word);

    for(i = 0; i < nbChars; i++)
    {
        c = word[i];

        strncpy(tempWord, word, i + 1);
        tempWord[i + 1] = '\0';

        if(! tree->children[c - 'a'])
        {
            tree->children[c - 'a'] = Dnode_new();
            tree->children[c - 'a']->element = Dword_new(tempWord);
        }

        tree = tree->children[c - 'a'];
    }

    if(addToList)
    {
        DwordList_add(&dict->words, ((Dword*)tree->element));
    }

    ((Dword*)tree->element)->isWord = true;

    return ((Dword*)tree->element);
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
 * \param tree the Ddictionary.tree to print.
 */
static void
Ddictionary_printTree(Dnode* tree)
{
    int i;

    if((Dword*)tree->element)
    {
        if(((Dword*)tree->element)->isWord)
        {
            printf("%s\n", ((Dword*)tree->element)->word);
        }
    }

    for(i = 0; i < CHILDREN_NUMBER; i++)
    {
        if(tree->children[i])
        {
            Ddictionary_printTree(tree->children[i]);
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
 * \param dictionary the Ddictionary object to be populated.
 *
 * \returns `0` if an error occured, else `1`.
 */
static int
Ddictionary_readDefinitions(Dconfig* config, Ddictionary* dict)
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
        // First, add the word to the dictionary
        currentWord = Ddictionary_getOrAddWord(dict, buffer, true);

        // Then, add its bases and link them together
        for(i = 0; i < nbBases; i++)
        {
            fscanf(stream, "%s", tempBase);

            // Get the base's memory address
            Dword* base;
            base = Ddictionary_getOrAddWord(dict, tempBase, true);

            Dword_addBase(currentWord, base);
            Dword_addDerivative(base, currentWord);
        }

        // Finally, add the synonyms
        for(i = 0; i < nbSynonyms; i++)
        {
            fscanf(stream, "%s", tempSynonym);

            // Get the synonym's memory address
            Dword* synonym;
            synonym = Ddictionary_getOrAddWord(dict, tempSynonym, true);

            Dword_addSynonym(currentWord, synonym);
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
Ddictionary_runCommand(char* command, Ddictionary* dict)
{
    // Looks like a valid command
    if(strlen(command) > 5)
    {
        char* searchedWord;
        searchedWord = strndup(command + 5, 20);

        Dword* word;
        word = Ddictionary_getOrAddWord(dict, searchedWord, false);

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

        free(searchedWord);
    }
}

/*******************************************************************************
 * Extern functions
 */

/** Frees recursively a Dnode dictionary and all of its dynamically allocated
 * attributes.
 *
 * \param dict The Dnode dictionary to be freed.
 */
extern void
Ddictionary_free(Ddictionary* dict)
{
    Dnode_free(dict->tree, Dword_free);

    /* All the members of this dynamic array have already been freed by the
     * above instruction, we only have to free the array itself.
     */
    DwordList_free(dict->words);

    free(dict);
}

extern Ddictionary*
Ddictionary_new()
{
    Ddictionary* newDic;
    newDic = calloc(1, sizeof(Ddictionary));

    newDic->tree = Dnode_new();

    return newDic;
}

/** Triggers actions requested by the configuration/
 *
 * \param config the application's configuration.
 * \param dict an initialized Dnode dictionary.
 *
 * \returns `0` if there was an error, else `1`.
 */
extern int
Ddictionary_processArgs(Dconfig* config, Ddictionary* dict)
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
    if(! Ddictionary_readDefinitions(config, dict))
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
        Ddictionary_printTree(dict->tree);
        printf("\n");
    }

    // Run all the commands prepared in the commands file
    for(i = 0; i < config->commandsNb; i++)
    {
        printf("\nCommande : %s\n", config->commands[i]);
        Ddictionary_runCommand(config->commands[i], dict);
    }

    return TRUE;
}

/** Runs an interactive session. Starts an infinite loop waiting for commands
 * from the user.
 *
 * \param config the application's configuration.
 * \param dict the dictionary in which commands should be executed.
 */
extern void
Ddictionary_runInteractive(Dconfig* config, Ddictionary* dict)
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

        Ddictionary_runCommand(input, dict);
    }
}

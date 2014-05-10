#ifndef DCONFIG_H
#define DCONFIG_H


/*******************************************************************************
 * Types declarations
 */

/** Object containing the application's configuration. */
typedef struct _Dconfig {
    /** The executable's name. */
    char* execName;

    /** The path to the file containing all the derivatives and synonyms
     * definitions.
     */
    char* definitionsPath;

    /** The path to the file containing all the commands that should be
     * executed before we start an interactive session.
     */
    char* commandsPath;

    /** The number of commands we read in the commands file. */
    int commandsNb;

    /** The actual commands array. */
    char** commands;

    /** Contains `1` if the `-d` switch was part of the arguments. */
    int d_option;

    /** Contains `1` if the `-h` switch was part of the arguments. */
    int h_option;

    /** Contains `1` if the `-p` switch was part of the arguments. */
    int p_option;
} Dconfig;


/*******************************************************************************
 * Extern function declarations
 */

extern void
Dconfig_free(Dconfig*);

extern Dconfig*
Dconfig_new();

extern int
Dconfig_parseArgs(Dconfig*, int, char**);

#endif

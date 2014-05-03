#ifndef DCONFIG_H
#define DCONFIG_H

typedef struct _Dconfig {
    char* execName;

    char* definitionsPath;
    
    char* commandsPath;
    int commandsNb;
    char** commands;

    int d_option;
    int h_option;
    int p_option;
} Dconfig;


extern Dconfig*
Dconfig_new();

extern void
Dconfig_free(Dconfig*);

#endif
#ifndef DCONFIG_H
#define DCONFIG_H

typedef struct _Dconfig {
    char* definitionsPath;
    char* commandsPath;

    int d_option;
    int h_option;
    int p_option;
} Dconfig;

#endif
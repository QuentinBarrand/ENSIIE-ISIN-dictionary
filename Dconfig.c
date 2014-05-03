#include <stdlib.h>

#include "Dconfig.h"

/*******************************************************************************
 * Extern functions
 */

extern Dconfig*
Dconfig_new()
{
	// We allocate the Dconfig object with a 0 value to all of its attributes
    return calloc(1, sizeof(Dconfig));
}

extern void
Dconfig_free(Dconfig* config)
{
	int i;

	for(i = 0; i < config->commandsNb; i++)
	{
		free(config->commands[i]);
	}

	free(config->commandsPath);
	free(config->commands);

	config->execName = NULL;

	free(config->execName);
	free(config->definitionsPath);

	free(config);
}
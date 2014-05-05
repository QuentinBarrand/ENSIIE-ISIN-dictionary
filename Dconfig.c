#include <stdlib.h>

#include "Dconfig.h"


/*******************************************************************************
 * Extern functions
 */

/** Allocates a new Dconfig object with all of its attributes set to the `0`
 * value.
 *
 * \returns A new, freshly allocated Dconfig object.
 */
extern Dconfig*
Dconfig_new()
{
    return calloc(1, sizeof(Dconfig));
}

/** Frees a Dconfig object and all of its dynamically allocated attributes.
 *
 * \param config The Dconfig object to be freed.
 */
extern void
Dconfig_free(Dconfig* config)
{
    int i;

    for(i = 0; i < config->commandsNb; i++)
    {
        free(config->commands[i]);
    }

    free(config->commands);

    free(config);
}

#include <stdio.h>

void Dutils_help(char* execName)
{
	printf("Usage : %s <options>"
		"\n-h            Afficher cette aide"
		"\n-d            Afficher pour chaque requête le nombre de cases parcourues"
		"\n-if file      Utiliser le fichier file à la place de l'entrée standard"
		"\n-p            Imprime sous un format lisible la structure de données"
		"\n", execName);
}
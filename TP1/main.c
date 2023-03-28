// dans le typedef Signe, il y a un champ start et finish. On n'a besoin de stocker que 1 start et 1 finish. Je pense que c'est plus adapte de les stocker a part
// piste pour avancer sur le projet et bien préparer la suite : Au lieu que tout ça soit appelé depuis le main, ca serait bien de creer une fonction qui retourne un tableau Signe qu'on pourrait facilement acceder depuis le main et ensuite facilement recopier pour les autres fichiers
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 100
#define MAX_LINES 200

// definir string²
typedef char string[1024];

// definir une signe avec l'enregistrement
typedef struct
{
    char start[200];
    char action[100];
    char finish[100];
    char preconds[100];
    char add[100];
    char delete[100];
} Signe;

int parseLine(char source[], string cible[])
{
    int i = 0, n = 0; // i: l'indice de source[], n: l'indice de cible[]
    while (source[i] != ':')
        i++;   // avancer jusqu'à ':'
    i++;       // avancer au debut de la premiere chaine
    int j = i; // marquer le debut de la premiere chaine avec j
    while (source[i] != '\n')
    {
        if (source[i] == ',')
        {
            memcpy(&cible[n], &source[j], i - j); // extraire les i-j caracteres a partir de j
            n++;
            j = i + 1; // marquer le debut de la chaine suivante avec j
        }
        i++;
    }
    return n;
}

int main(void)
{
    string start[MAX_LINES];
    string finish[MAX_LINES];
    string action[MAX_LINES];
    char source[MAX_LINE_LENGTH];
    string preconds[MAX_LINES];
    string add[MAX_LINES];
    string delete[MAX_LINES];
    Signe signes[MAX_LINES];
    int nbS = 0, nbM = 0;
    int i;

    // ouvrir le fichier
    FILE *monflux = fopen("school.txt", "r");

    // tester le fichier s'il existe
    if (monflux == NULL)
    {
        printf("fichier errur: \n");
        exit(0);
    }

    // lire start
    fgets(source, 100, monflux);
    nbM = parseLine(source, start);
    for (i = 0; i < nbM; i++)
        printf("start : %s\n", start[i]);

    // lire finish
    fgets(source, 100, monflux);
    nbM = parseLine(source, finish);
    for (i = 0; i < nbM; i++)
        printf("finish : %s\n", finish[i]);

    // lire les signes
    while (fgets(source, MAX_LINE_LENGTH, monflux) != NULL)
    {
        printf("\n");

        // lire les action
        nbM = parseLine(source, action);
        for (int i = 0; i < nbM; i++)
        {
            strcpy(signes[nbS].action, action[i]);
            printf("action : %s\n", signes[nbS].action);
        }

        // lire les préconditions
        nbM = parseLine(source, preconds);
        for (int i = 0; i < nbM; i++)
        {
            strcpy(signes[nbS].preconds, preconds[i]);
            printf("preconds : %s\n", signes[nbS].preconds);
        }

        // lire les ajouts
        fgets(source, MAX_LINE_LENGTH, monflux); // passer la ligne ""
        nbM = parseLine(source, add);
        for (int i = 0; i < nbM; i++)
        {
            strcpy(signes[nbS].add, add[i]);
            printf("add : %s\n", signes[nbS].add);
        }

        // lire les suppressions
        fgets(source, MAX_LINE_LENGTH, monflux); // passer la ligne "**"
        nbM = parseLine(source, delete);
        for (int i = 0; i < nbM; i++)
        {
            strcpy(signes[nbS].delete, delete[i]);
            printf("delete : %s\n", signes[nbS].delete);
        }

        nbS++;
    }

    // fermer le fichier
    fclose(monflux);
}
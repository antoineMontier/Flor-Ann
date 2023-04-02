// dans le typedef Signe, il y a un champ start et finish. On n'a besoin de stocker que 1 start et 1 finish. Je pense que c'est plus adapte de les stocker a part
// piste pour avancer sur le projet et bien préparer la suite : Au lieu que tout ça soit appelé depuis le main, ca serait bien de creer une fonction qui retourne un tableau Signe qu'on pourrait facilement acceder depuis le main et ensuite facilement recopier pour les autres fichiers
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 100
#define MAX_LINES 200
#define A 123
#define B 124
#define C 125
#define VIDE 126

// definir string²
typedef char string[1024];

// definir une signe avec l'enregistrement
typedef struct
{
    char start[200]; // -- a voir si c'est utile ?
    char action[100];
    char finish[100]; // -- a voir si c'est utile ?
    char preconds[100];
    char add[100];
    char delete[100];
} Signe;

char* resoudre_school(Signe* s, char* start, char* end);
void supprimer(char* etat, char* a_supprimer);
int rechercher(char*etat, char* a_rechercher);

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

    char test1[MAX_LINES];
    sprintf(test1,"%s", "have money,have phone book,car needs battery,son at home");
    char delete[MAX_LINES];
    sprintf(delete, "%s", "have phone book");

    printf("main: %d\n", rechercher(test1, delete));







    /*
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


    resoudre_school(signes, start, finish);
    */
    return 0;
}

/*
// initialisation des faits courants, du but et des règles en les chargeant depuis un fichier
tantque (les faits courants ne contiennent pas tous les faits du but) faire
trouver une règle applicable
appliquer la règle sur les faits courants (ajout des add, suppression des delete)
fintantque
*/

char* resoudre_school(Signe* s, char* start, char* end){

    // 1ere etape, placer A B C et (VIDE) de la maniere specifiee par le start : 
    printf("=============\n%s\n%s\n", start, end);
    printf("============ %s\n", s[7].preconds);
    int finish = 0;
    char etat[256];
    strcpy(etat, start);

    while(!finish){
        
        // pour chaque bloc, on regarde si on peut appliquer les actions en comparand les preconds actuels a notre etat
        for(int i = 0 ;  i < MAX_LINES ; ++i){
            if(rechercher(etat, s[i].preconds)){  // -- a modifier, peut etre creer notre propre fonction ?
                supprimer(etat, s[i].delete);
                sprintf(etat, "%s,%s", etat, s[i].add);
                break; // -- a voir si c'est encessaire de sortir de la boucle for
            }
        }







        if(strcmp(etat, end) == 0)
            finish = 1;
    }
}

void supprimer(char* etat, char* a_supprimer){
    if(a_supprimer[0] == '\0') return; // -- verifier la condition d'arret, peut etre tester [' ', '\0'] ?

    int i = 0, i_suppr = 0, debut_de_suppression = 0;
    while(etat[i] != '\0'){
        if(etat[i] == a_supprimer[i_suppr])
            i_suppr++;
        else {
            i_suppr = 0;
            debut_de_suppression = i;
        }
        i++;
        if(a_supprimer[i_suppr] == '\0')
            break;
    }

    // il faut supprimer entre debut_de_suppression et i dans etat
    char nouveau_etat[MAX_LINES];
    for(int k = 0; k < debut_de_suppression; k++){
        nouveau_etat[k] = etat[k];
        nouveau_etat[k+1] = '\0';
    }
    
    printf("step 1 : %s\n", nouveau_etat);

    for(int k = debut_de_suppression ; k < MAX_LINES ;++k)
        nouveau_etat[k] = etat[k + i - debut_de_suppression]; 
    printf("step 2 : %s\n", nouveau_etat);
    strcpy(etat, nouveau_etat);
}   


int rechercher(char*etat, char* a_rechercher){
    if(a_rechercher[0] == '\0') return; // -- verifier la condition d'arret, peut etre tester [' ', '\0'] ?

    int i = 0, i_rech = 0, debut_de_recherche = 0;
    while(etat[i] != '\0'){
        if(etat[i] == a_rechercher[i_rech])
            i_rech++;
        else {
            i_rech = 0;
            debut_de_recherche = i;
        }
        i++;
        if(a_rechercher[i_rech] == '\0')
            return 1;
    }
    return 0;
}

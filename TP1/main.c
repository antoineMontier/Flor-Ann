/*
17/04 : 
on a un tableau de 'signes'. pour chaque signe, il y a 4 'Cond' : add, delete, preconds, action : pour chaque 'Cond' il ya un type (ADD, DELETE, ACTION, PRECOND)
il y a aussi un nb_cond qui correspond lui au nombre de lignes pour stocker le cond, limité par NB_MAX_CONDS. il y a aussi un tableau de chaine de char, une ligne de tableau par propositions (qui sont séparées par des virgules dans le fichier txt)
*/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 100
#define MAX_LINES 32
#define NB_MAX_CONDS 5
#define A 123
#define B 124
#define C 125
#define VIDE 126

#define ACTION (-1024)
#define PRECOND (-1025)
#define ADD (-1026)
#define DELETE (-1027)

// definir string²
typedef char string[1024];

typedef struct{
    int nb_cond;
    char cond[NB_MAX_CONDS][MAX_LINE_LENGTH];
    int type; // ACTION, PRECOND, ADD ou DELETE
} Cond;

// definir une signe avec l'enregistrement
typedef struct
{
    Cond action;
    Cond preconds;
    Cond add;
    Cond delete;
} Signe;



char* resoudre_school(Signe* s, char* start, char* end);
void supprimer(char* etat, char* a_supprimer);
int rechercher(char*etat, char* a_rechercher);
char** lecture(const char* filename);
int preconds(char*c);
int add(char*c);
int action(char*c);
int delete(char*c);


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
/*
Objectif de fonction : à partir des ':' et entre chaque ',' mettre la chaine de char en question dans un tableau : 
preconds:car needs battery,shop knows problem,shop has money,
*/


void parse_cond(char* chaine, Cond *c){
    c->nb_cond = 0;
    if(chaine == NULL || chaine[0]== '\0')return;
    int indice_case_courante = 0;
    int indice_resultat = 0;
    int indice_de_copie_resultat = 0;
    // avancer jusqu'aux 2 points
    // printf("traitement de la chaine : %s\n", chaine);
    if(add(chaine)) c->type = ADD;
    else if(action(chaine)) c->type = ACTION;
    else if(delete(chaine)) c->type = DELETE;
    else if(preconds(chaine)) c->type = PRECOND;
    else return;
    while(chaine[indice_case_courante] != ':' && chaine[indice_case_courante] != '\0') indice_case_courante++;
    indice_case_courante++; // avancer pour ne pas copier les deux points
    int fini = 0;
    while(!fini){
        // copier jusqu'à la ','
        while(chaine[indice_case_courante] != ',' && chaine[indice_case_courante] != '\0'){
            c->cond[indice_resultat][indice_de_copie_resultat] = chaine[indice_case_courante];
            indice_case_courante++;
            indice_de_copie_resultat++;
        }
        if(chaine[indice_case_courante + 1] == '\0' || chaine[indice_case_courante + 1] == '\n' || indice_resultat >= NB_MAX_CONDS) fini = 1;
        c->cond[indice_resultat][indice_de_copie_resultat] = '\0';
        indice_resultat++;
        indice_case_courante++;
        indice_de_copie_resultat = 0;
        (c->nb_cond)++;
    }
}

/*
Objectif de fonction : donner un char** correspondant au fichier
*/

void to_signes(char**fichier, Signe s[MAX_LINES/4]){
    int indice_signe = 0;
    for(int i = 3 ; i < MAX_LINES ; i++){ // on lit a partir de la 3e ligne pour sauter les starts, finish et *** du début
        if(fichier[i][0] != '*'){
            // printf("indice signe = %d\nligne :%s\n", indice_signe, fichier[i]);
            Cond co;
            parse_cond(fichier[i], &co);
            // printf("type %d\n", co.type);
            if(co.type == ADD)
                parse_cond(fichier[i], &s[indice_signe].add);
            else if(co.type == DELETE)
                parse_cond(fichier[i], &s[indice_signe].delete);
            else if(co.type == PRECOND)
                parse_cond(fichier[i], &s[indice_signe].preconds);
            else if(co.type == ACTION)
                parse_cond(fichier[i], &s[indice_signe].action);
        }else indice_signe++;
    }
}


int main(void)
{

    // ===== test des fonctions supprimer et rechercher =====

    /*char test1[MAX_LINE_LENGTH];
    sprintf(test1,"%s", "have money,have phone book,car needs battery,son at home");

    supprimer(test1, "have");
    printf("main: %s\n", test1);*/

    // ====================================================
    
    char** fichier;
    fichier = lecture("school.txt");


    // for(int i = 0 ; i < MAX_LINES ; ++i)
    //     printf("%s\n", fichier[i]);

    // ================= ajout du start ===================
    char start[MAX_LINE_LENGTH];

    strcpy(start, fichier[0]);
    //supprimer(start, "start:");
    printf("start lu :  %s\n", start);

    // =============== ajout du finish ================
    char finish[MAX_LINE_LENGTH];
    strcpy(finish, fichier[1]);
    //supprimer(finish, "finish:");
    printf("finish lu : %s\n", finish);


    Signe test[MAX_LINES/4];
    to_signes(fichier, test);

    for(int i = 0 ; i < 5 ; i++){
        printf("=============== SIGNE i = %d\n", i);
        for(int j = 0 ; j  < test[i].add.nb_cond ; j++)
            printf("ADD : %s\n", test[i].add.cond[j]);
        for(int j = 0 ; j  < test[i].preconds.nb_cond ; j++)
            printf("PRECOND : %s\n", test[i].preconds.cond[j]);
        for(int j = 0 ; j  < test[i].delete.nb_cond ; j++)
            //if()
            printf("DELETE : %d\n", test[i].delete.cond[j][0]);
        for(int j = 0 ; j  < test[i].action.nb_cond ; j++)
            printf("ACTION : %s\n", test[i].action.cond[j]);
    }




















    // ============ ajouter les actions ============
    /*Signe signes[MAX_LINES/4];
    int indice_action = 0;
    for(int i = 2 ; i < MAX_LINES ; i++){
        
        if(action(fichier[i]))
            strcpy(signes[indice_action].action, fichier[i]);
        else if(add(fichier[i]))
            strcpy(signes[indice_action].add, fichier[i]);
        else if (preconds(fichier[i]))
            strcpy(signes[indice_action].preconds, fichier[i]);
        else if(delete(fichier[i]))
            strcpy(signes[indice_action].delete, fichier[i]);
        else
            indice_action++;
        printf("%s\n", fichier[i]);
    }*/



    // resoudre_school(signes, start, finish);





    /*
    char start[MAX_LINES];
    char finish[MAX_LINES];
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
    if (monflux == NULL){
        printf("fichier errur: \n");
        exit(0);
    }

    // lire start
    fgets(source, 100, monflux);
    nbM = parseLine(source, start);
    for (i = 0; i < nbM; i++)
        printf("start : %s\n", start);

    // lire finish
    fgets(source, 100, monflux);
    nbM = parseLine(source, finish);
    for (i = 0; i < nbM; i++)
        printf("finish : %s\n", finish);

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


/*
char* resoudre_school(Signe* s, char* start, char* end){

    // 1ere etape, placer A B C et (VIDE) de la maniere specifiee par le start : 
    printf("=============\n%s\n%s\n", start, end);
    printf("=============\n");
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
}*/

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
    char nouveau_etat[MAX_LINE_LENGTH];
    for(int k = 0; k < debut_de_suppression; k++){
        nouveau_etat[k] = etat[k];
        nouveau_etat[k+1] = '\0';
    }
    
    // printf("step 1 : %s\n", nouveau_etat);

    for(int k = debut_de_suppression ; k < MAX_LINE_LENGTH ;++k)
        nouveau_etat[k] = etat[k + i - debut_de_suppression]; 
    // printf("step 2 : %s\n", nouveau_etat);
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

int preconds(char*c){
    if(c == NULL) return 0;
    if(c[0] != '\0' && c[0] == 'p' && c[1] != '\0' && c[1] == 'r' && c[2] != '\0' && c[2] == 'e') return 1;
    return 0;
}

int add(char*c){
    if(c == NULL) return 0;
    if(c[0] != '\0' && c[0] == 'a' && c[1] != '\0' && c[1] == 'd' && c[2] != '\0' && c[2] == 'd') return 1;
    return 0;
}
int action(char*c){
    if(c == NULL) return 0;
    if(c[0] != '\0' && c[0] == 'a' && c[1] != '\0' && c[1] == 'c' && c[2] != '\0' && c[2] == 't') return 1;
    return 0;
}
int delete(char*c){
    if(c == NULL) return 0;
    if(c[0] != '\0' && c[0] == 'd' && c[1] != '\0' && c[1] == 'e' && c[2] != '\0' && c[2] == 'l') return 1;
    return 0;
}

char** lecture(const char* filename) {
    char buffer[MAX_LINE_LENGTH];
    char** lines = NULL;
    FILE* reader = fopen(filename, "r");

    if (reader == NULL) {
        printf("erreur d'ouverture du fichier '%s'\n", filename);
        return NULL;
    }
    lines = malloc(MAX_LINES*sizeof(char*));
    int line_count = 0;
    while (fgets(buffer, sizeof(buffer), reader) != NULL) {
        char* line = malloc(strlen(buffer) + 1);
        strcpy(line, buffer);
        lines[line_count] = line;
        line_count++;
    }

    // fermer le ficher
    fclose(reader);

    return lines;
}
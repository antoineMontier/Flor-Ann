/*
17/04 : 
on a un tableau de 'signes'. pour chaque signe, il y a 4 'Cond' : add, delete, preconds, action : pour chaque 'Cond' il ya un type (ADD, DELETE, ACTION, PRECOND)
il y a aussi un nb_cond qui correspond lui au nombre de lignes pour stocker le cond, limité par NB_MAX_CONDS. il y a aussi un tableau de chaine de char, une ligne de tableau par propositions (qui sont séparées par des virgules dans le fichier txt)
*/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 256
#define FILE_LINES 82
#define NB_MAX_CONDS 100

#define ACTION (-1024)
#define PRECOND (-1025)
#define ADD (-1026)
#define DELETE (-1027)
#define START (-1028)
#define FINISH (-1029)

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



void supprimer(char* etat, char* a_supprimer);
int rechercher(char*etat, char* a_rechercher);
char** lecture(const char* filename);
int preconds(char*c);
int add(char*c);
int action(char*c);
int delete(char*c);
int start(char*c);
int finish(char*c);
void to_signes(char**fichier, Signe s[FILE_LINES/5]);
void resoudre_naive(Signe act[FILE_LINES/5], Cond start, Cond finish);
void print_signes(Signe *s, int signe_size);
int proposition_dans_cond(char *s, Cond test);
int precond_statisfait(Cond precond, Cond etat);
void supprimer_cond(Cond delete, Cond* etat);
void ajouter_add(Cond add, Cond *etat);
void parse_cond(char* chaine, Cond *c);
void print_cond(Cond d);
void resoudre_fute(Signe act[FILE_LINES/5], Cond start, Cond finish);
void quels_preconds(Signe act[FILE_LINES/5], Cond etat, int tableau_de_verite[FILE_LINES/5]);
void copie_cond(Cond *dest, Cond src);
void resoudre_fute_rec(Signe act[FILE_LINES/5], Cond etat, Cond finish, int*fini, int actions_prises[FILE_LINES/5]);



int main(){
    
    char** fichier;
    fichier = lecture("riviere.txt");

    for(int i = 0 ; i < FILE_LINES ; ++i)
        printf("%s\n", fichier[i]);

    Cond start, finish;
    parse_cond(fichier[0], &start);
    parse_cond(fichier[1], &finish);

    print_cond(start);
    print_cond(finish);

    // afficher finish et start

    // printf("=========== START\n");
    // for(int i = 0 ; i < start.nb_cond ; i++)
    //     printf("%s\n", start.cond[i]);
    // printf("=========== FINISH\n");
    // for(int i = 0 ; i < finish.nb_cond ; i++)
    //     printf("%s\n", finish.cond[i]);

    Signe test[FILE_LINES/5];
    to_signes(fichier, test);

    print_signes(test, FILE_LINES/5);

    //resoudre_fute(test, start, finish);


    return 0;
}


void quels_preconds(Signe act[FILE_LINES/5], Cond etat, int tableau_de_verite[FILE_LINES/5]){
    for(int i=0; i < FILE_LINES/5 ; ++i)
        if(precond_statisfait(act[i].preconds, etat))
            tableau_de_verite[i] = 1;
        else 
            tableau_de_verite[i] = 0;
}



void resoudre_fute(Signe act[FILE_LINES/5], Cond start, Cond finish){
    int fini = 0;
    int action[FILE_LINES/5];
    for(int i=0; i<FILE_LINES/5 ; ++i) action[i] = 0;
    resoudre_fute_rec(act, start, finish, &fini,action);
}

void copie_cond(Cond *dest, Cond src){
    dest->nb_cond = src.nb_cond;
    dest->type = src.type;
    for(int i = 0 ; i < NB_MAX_CONDS ; i++)
        strcpy(dest->cond[i], src.cond[i]);
}


void resoudre_fute_rec(Signe act[FILE_LINES/5], Cond etat, Cond finish, int*fini, int actions_prises[FILE_LINES/5]){
    //conditions d'arret : 
    
    if(*fini == 1){
        printf("solution trouvée par une autre rec, je m'arrete\n");
        return;
    }
    if(precond_statisfait(finish, etat)){
        printf("solution trouvée mon etat : \n");
        print_cond(etat);
        *fini = 1;
        return;
    }

    // affichage : 
    printf("\n\nMon etat : \n");
    print_cond(etat);


    // connaitre tous les preconds faisables : 
    int tab_verite[FILE_LINES/5];
    quels_preconds(act, etat, tab_verite);
    // tableau de vérité rempli : 
    for(int action = 0; action < FILE_LINES/5 ; ++action){
        if(tab_verite[action] && actions_prises[action] == 0){
            // option1 : on retire le delete et on ajoute le add, on lance la recursivité avec l'etat, puis on retire le add qu'on a ajoute et on remet le delete pour lancer la rec sur la deuxieme solution possi
            // option2 : creer un nouvel etat pour chaque action, lancer la recursivite avec le nouvel etat auquel on aura retiré les delete et ajoute les add
            Cond nv_etat;
            copie_cond(&nv_etat, etat);
            // suppression des delete dans nv_etat
            supprimer_cond(act[action].delete, &nv_etat);

            // ajout des add
            ajouter_add(act[action].add, &nv_etat);

            int actions[FILE_LINES/5];
            for(int i=0; i<FILE_LINES/5 ; ++i) actions[i] = actions_prises[i];
            actions[action] = 1;
            resoudre_fute_rec(act, nv_etat, finish, fini, actions);
        }
    }
}
































/*
// initialisation des faits courants, du but et des règles en les chargeant depuis un fichier
tantque (les faits courants ne contiennent pas tous les faits du but) faire
trouver une règle applicable
appliquer la règle sur les faits courants (ajout des add, suppression des delete)
fintantque
*/



    /**
    * Objectif de fonction : donner un char** correspondant au fichier
    */
void to_signes(char**fichier, Signe s[FILE_LINES/5]){
    int indice_signe = 0;
    for(int i = 3 ; i < FILE_LINES ; i++){ // on lit a partir de la 3e ligne pour sauter les starts, finish et *** du début
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

/**
 * fonction pour afficher les propositions enregistrées dans un tableau de signes de taille signe_size
*/
void print_signes(Signe *s, int signe_size){
    for(int i = 0 ; i < signe_size ; i++){
        printf("=============== SIGNE i = %d\n", i);
        for(int j = 0 ; j  < s[i].add.nb_cond ; j++)
            printf("ADD : %s\n", s[i].add.cond[j]);
        for(int j = 0 ; j  < s[i].preconds.nb_cond ; j++)
            printf("PRECOND : %s\n", s[i].preconds.cond[j]);
        for(int j = 0 ; j  < s[i].delete.nb_cond ; j++)
            printf("DELETE : %s\n", s[i].delete.cond[j]);
        for(int j = 0 ; j  < s[i].action.nb_cond ; j++)
            printf("ACTION : %s\n", s[i].action.cond[j]);
    }
}





int proposition_dans_cond(char *s, Cond test){
    if(s == NULL) return 0;
    for(int i = 0 ; i < test.nb_cond ; i++)
        if(strcmp(s, test.cond[i]) == 0) return 1;
    return 0;
}

int precond_statisfait(Cond precond, Cond etat){
    if(precond.nb_cond  == 0) return 1;
    if(precond.nb_cond > etat.nb_cond) return 0;
    // vérifier que chaque condition du precond est contenue dans etat : 
    for(int i = 0 ; i < precond.nb_cond ; i++)
        if(!proposition_dans_cond(precond.cond[i], etat)){
            // printf("action %s non comprise dans etat\n", precond.cond[i]);
            return 0;
        } 
    return 1;
}


void supprimer_cond(Cond delete, Cond *etat){
    if(delete.nb_cond == 0) return;
    Cond nv_etat;
    nv_etat.type = etat->type;
    int indice_remplissage = 0;
    for(int i = 0 ; i  < etat->nb_cond ; ++i){
        if(!proposition_dans_cond(etat->cond[i], delete)){
            strcpy(nv_etat.cond[indice_remplissage], etat->cond[i]);
            indice_remplissage++;
        }
    }
    nv_etat.nb_cond = indice_remplissage;

    copie_cond(etat, nv_etat);
}


void ajouter_add(Cond add, Cond *etat){
    for(int i=0; i< add.nb_cond; i++){
        if(!proposition_dans_cond(add.cond[i], *etat)){
            strcpy(etat->cond[etat->nb_cond], add.cond[i]);
            etat->nb_cond++;
        }
    }
}

void resoudre_naive(Signe act[FILE_LINES/5], Cond start, Cond finish){
    if(act == NULL ) return;
    Cond etat;
    etat.nb_cond = 0;
    // initialiser 'etat' avec start : 
    for(int i = 0 ; i < start.nb_cond ; i++) 
        strcpy(etat.cond[i], start.cond[i]);
    etat.nb_cond = start.nb_cond;

    printf("\n\naffichage de 'etat' apres son init : \n");
    for(int i = 0 ; i < etat.nb_cond ; i++)
        printf("%s\n",etat.cond[i]);

    int fini = 0;
    while(!fini){
        printf("================ ETAT : \n");
        print_cond(etat);
        printf("================\n");
        // regarder les preconds de chaque bloc : 
        for(int bloc_actuel = 0 ; bloc_actuel < FILE_LINES/5 ; ++bloc_actuel){ // boucle infinie est possible
            // pour chaque bloc, si le precond est satisfait, cest a dire que toutes les propositions du precond sont contenues dans 'etat'
            if(precond_statisfait(act[bloc_actuel].preconds, etat)){
                // printf("precond trouve %d\n", bloc_actuel);
                // le precond est satisfait : on fait les delete 
                // printf("delete de :\n");
                // print_cond(act[bloc_actuel].delete);
                supprimer_cond(act[bloc_actuel].delete, &etat);
                printf("============= APRES DELETE =================\n");
                print_cond(etat);
                // printf("ajout de :\n");
                // print_cond(act[bloc_actuel].add);
                printf("================\n================APRES ajout=============\n");

                // puis les add
                ajouter_add(act[bloc_actuel].add, &etat);

                print_cond(etat);
                printf("==================\n");
                bloc_actuel = FILE_LINES/5; // stopper la boucle for
            }
        }
        if(/*finish est contenu etat.cond[i]*/precond_statisfait(finish, etat)) fini = 1;
    }
}

void print_cond(Cond d){
    for(int i=0; i<d.nb_cond; ++i) printf("%s\n", d.cond[i]);
}











/**
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
    else if(start(chaine)) c->type = START;
    else if(finish(chaine)) c->type = FINISH;
    else return;
    while(chaine[indice_case_courante] != ':' && chaine[indice_case_courante] != '\0') indice_case_courante++;
    indice_case_courante++; // avancer pour ne pas copier les deux points
    if(chaine[indice_case_courante] == '\n') return;
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
    if(a_rechercher[0] == '\0') return 0; // -- verifier la condition d'arret, peut etre tester [' ', '\0'] ?

    int i = 0, i_rech = 0;
    while(etat[i] != '\0'){
        if(etat[i] == a_rechercher[i_rech])
            i_rech++;
        else 
            i_rech = 0;
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
int start(char*c){
    if(c == NULL) return 0;
    if(c[0] != '\0' && c[0] == 's' && c[1] != '\0' && c[1] == 't' && c[2] != '\0' && c[2] == 'a') return 1;
    return 0;
}
int finish(char*c){
    if(c == NULL) return 0;
    if(c[0] != '\0' && c[0] == 'f' && c[1] != '\0' && c[1] == 'i' && c[2] != '\0' && c[2] == 'n') return 1;
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
    lines = malloc(FILE_LINES*sizeof(char*));
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCS_LENGHT 92

char** lecture(const char *);
int preconds(char*);


typedef struct{
    char add[5][128];
    char preconds[5][128];
    char delete[5][128];
    char action[5][128];
    int nb_add;
    int nb_preconds;
    int nb_delete;
    int nb_action;
} Stockage;


/// @brief ici on verifie que les 3 premiers char, peut etre utile de verifier le mot dans son entierete
/// @param c 
/// @return 
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

int delete(char*c){
    if(c == NULL) return 0;
    if(c[0] != '\0' && c[0] == 'd' && c[1] != '\0' && c[1] == 'e' && c[2] != '\0' && c[2] == 'l') return 1;
    return 0;
}

int action(char*c){
    if(c == NULL) return 0;
    if(c[0] != '\0' && c[0] == 'a' && c[1] != '\0' && c[1] == 'c' && c[2] != '\0' && c[2] == 't') return 1;
    return 0;
}



int main(){


    // ============= enregistrer dans un double tableau les lignes du fichier
    char**blocs = lecture("blocs.txt");

    Stockage st_blocs[40];
    int bloc_actuel = 0;
    char tmp;

    for(int i = 0 ; i < BLOCS_LENGHT ; ++i){
        if(preconds(blocs[i])){
            printf("la ligne %d commence par preconds\n", i);
            int char_actuel_doc = 9;
            tmp = '\0';
            while(tmp != '\n'){
                int char_actuel_preconds = 0;
                while((tmp = blocs[i][char_actuel_doc]) != ','){
                    st_blocs[bloc_actuel].preconds[  st_blocs[bloc_actuel].nb_preconds ][char_actuel_preconds] = tmp;
                    char_actuel_doc++;
                    char_actuel_preconds++;
                    printf("tmp = %c  ligne de doc = %d  colone du doc = %d  nb_preconds enregistre = %d\n", tmp, i, char_actuel_doc, st_blocs[bloc_actuel].nb_preconds);
                }
                st_blocs[bloc_actuel].preconds[  st_blocs[bloc_actuel].nb_preconds ][char_actuel_preconds] = '\0';
                printf("tmp = %c  ligne de doc = %d  colone du doc = %d  nb_preconds enregistre = %d\n", st_blocs[bloc_actuel].preconds[  st_blocs[bloc_actuel].nb_preconds ][char_actuel_preconds], i, char_actuel_doc, st_blocs[bloc_actuel].nb_preconds);
                printf("\n");
                st_blocs[bloc_actuel].nb_preconds++;
                char_actuel_doc++;
                tmp = blocs[i][char_actuel_doc];
            }
            bloc_actuel++;
        }


        else if(add(blocs[i])){
            printf("la ligne %d commence par add\n", i);
            int char_actuel_doc = 9;
            tmp = '\0';
            while(tmp != '\n'){
                int char_actuel_add = 0;
                while((tmp = blocs[i][char_actuel_doc]) != ','){
                    st_blocs[bloc_actuel].add[  st_blocs[bloc_actuel].nb_add ][char_actuel_add] = tmp;
                    char_actuel_doc++;
                    char_actuel_add++;
                    printf("tmp = %c  ligne de doc = %d  colone du doc = %d  nb_add enregistre = %d\n", tmp, i, char_actuel_doc, st_blocs[bloc_actuel].nb_add);
                }
                st_blocs[bloc_actuel].add[  st_blocs[bloc_actuel].nb_add ][char_actuel_add] = '\0';
                printf("tmp = %c  ligne de doc = %d  colone du doc = %d  nb_add enregistre = %d\n", st_blocs[bloc_actuel].add[  st_blocs[bloc_actuel].nb_add ][char_actuel_add], i, char_actuel_doc, st_blocs[bloc_actuel].nb_add);
                printf("\n");
                st_blocs[bloc_actuel].nb_add++;
                char_actuel_doc++;
                tmp = blocs[i][char_actuel_doc];
            }
            bloc_actuel++;
        }   
    }
    int i = 0;
    printf("la ligne %d commence par add\n", i);
            int char_actuel_doc = 9;
            tmp = '\0';
            while(tmp != '\n'){
                int char_actuel_add = 0;
                while((tmp = blocs[i][char_actuel_doc]) != ','){
                    st_blocs[bloc_actuel].add[  st_blocs[bloc_actuel].nb_add ][char_actuel_add] = tmp;
                    char_actuel_doc++;
                    char_actuel_add++;
                    printf("tmp = %c  ligne de doc = %d  colone du doc = %d  nb_add enregistre = %d\n", tmp, i, char_actuel_doc, st_blocs[bloc_actuel].nb_add);
                }
                st_blocs[bloc_actuel].add[  st_blocs[bloc_actuel].nb_add ][char_actuel_add] = '\0';
                printf("tmp = %c  ligne de doc = %d  colone du doc = %d  nb_add enregistre = %d\n", st_blocs[bloc_actuel].add[  st_blocs[bloc_actuel].nb_add ][char_actuel_add], i, char_actuel_doc, st_blocs[bloc_actuel].nb_add);
                printf("\n");
                st_blocs[bloc_actuel].nb_add++;
                char_actuel_doc++;
                tmp = blocs[i][char_actuel_doc];
            }
            bloc_actuel++;
        }   



















    // for(int i = 0 ; i < BLOCS_LENGHT ; ++i)
    //     if(blocs[i][0] == '*')          // supprimer les lignes avec * et retour a la ligne
    //         blocs[i][0] = '\0';


    // for(int i = 0 ; i < BLOCS_LENGHT ; ++i)
    //     printf("%s", blocs[i]);

    //============= resoudre les problemes de blocs



   // =========== liberer le double tableau
    for (int i = 0; i < BLOCS_LENGHT; i++) free(blocs[i])
    free(blocs);

    return 0;





char** lecture(const char* filename) {
    char buffer[256];
    char** lines = NULL;
    FILE* reader = fopen(filename, "r");

    if (reader == NULL) {
        printf("erreur d'ouverture du fichier '%s'\n", filename);
        return NULL;
    {
    lines = malloc(BLOCS_LENGHT*sizeof(char*));
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

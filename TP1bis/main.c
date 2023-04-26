#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ecrire_regles(char* filename, int nb_blocs);






int main(){



    return 0;
}

void ecrire_regles(char* filename, int nb_blocs){
    if(nb_blocs < 1 || nb_blocs > 'z' - 'a') return;
    FILE * f = fopen(filename,"w");

    // start : 
    fprintf(f, "start:space on a,");
    for(int i = 1 ; i < nb_blocs  ; i++)
        fprintf(f, "%c on %c,", 'a' -1 + i, 'a' + i);
    fprintf(f, "%c on table,space on table,\n", 'a'  + nb_blocs -1);

    // finish
    fprintf(f, "finish:space on %c,", 'a'  + nb_blocs -1);
    for(int i = nb_blocs -1; i >= 1  ; i--)
        fprintf(f, "%c on %c,", 'a' + i, 'a' + i-1);
    fprintf(f, "a on table,space on table,\n");


    for(int bloc_actuel = 0 ; bloc_actuel < nb_blocs ; bloc_actuel++){
        for(int bloc_src = 0 ; bloc_src < nb_blocs ; bloc_src++){
            for(int bloc_dest = 0 ; bloc_dest < nb_blocs ; bloc_dest++){
                if(bloc_actuel != bloc_src && bloc_src != bloc_dest && bloc_dest != bloc_actuel){
                    fprintf(f, "****\n");
                    fprintf(f, "action:move %c from %c to %c,\n", 'a' + bloc_actuel, 'a' + bloc_src, 'a' + bloc_dest);
                    fprintf(f, "preconds:space on %c,space on %c,%c on %c,\n", 'a' + bloc_actuel, 'a' + bloc_dest, 'a' + bloc_actuel, 'a' + bloc_src);
                    fprintf(f, "add:%c on %c,space on %c,\n", 'a' + bloc_actuel, 'a' + bloc_dest, 'a' + bloc_src);
                    fprintf(f, "delete:%c on %c,space on %c,\n",'a' + bloc_actuel, 'a' + bloc_src, 'a' + bloc_dest);
                }
            }
        }
        // table vers n'importe ou
        for(int bloc_dest = 0 ; bloc_dest < nb_blocs ; bloc_dest++)
            if(bloc_actuel != bloc_dest){
                fprintf(f, "****\n");
                fprintf(f, "action:move %c from table to %c,\n", 'a' + bloc_actuel, 'a' + bloc_dest);
                fprintf(f, "preconds:space on %c,space on %c,%c on table,\n", 'a' + bloc_actuel, 'a' + bloc_dest, 'a' + bloc_actuel);
                fprintf(f, "add:%c on %c,\n", 'a' + bloc_actuel, 'a' + bloc_dest);
                fprintf(f, "delete:%c on table,space on %c,\n",'a' + bloc_actuel, 'a' + bloc_dest);
            }
        // n'importe ou vers la table
        for(int bloc_src = 0 ; bloc_src < nb_blocs ; bloc_src++)
            if(bloc_actuel != bloc_src){
                fprintf(f, "****\n");
                fprintf(f, "action:move %c from %c to table,\n", 'a' + bloc_actuel, 'a' + bloc_src);
                fprintf(f, "preconds:space on %c,space on table,%c on %c,\n", 'a' + bloc_actuel, 'a' + bloc_actuel, 'a' + bloc_src);
                fprintf(f, "add:%c on table,space on %c,\n", 'a' + bloc_actuel, 'a' + bloc_src);
                fprintf(f, "delete:%c on %c,\n",'a' + bloc_actuel, 'a' + bloc_src);
            }

    }








    fclose(f);
}
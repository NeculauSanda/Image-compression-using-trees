/*NECULAU SANDA-ELENA GRUPA 313 CB*/
#include "tema2.h"

#define RGB_max_color 255

int main(int argc, char *argv[]) {

    Image1 img = NULL;  /* matricea de pixeli care retine RGB*/
    Tarb A = NULL;  /*arbore*/
    char p;
    int nr, x_reper = 0, y_reper = 0, nivel = 0;
    int val_max_rgb;
    int factor = 0;
    INFO_C info;  /*structura ce retine RGB si tip*/
    Tcoada *C = NULL; /*coada*/

    /*CERINTA 3*/
    if(strcmp(argv[1], "-d") == 0) {
        int i = 0;

        /*initializare coada*/
        C = Init();

        // deschidem fisierul de compresie
        FILE *in;
        in = fopen(argv[2], "rb");
        if (!in) {
            printf("Unable to upen file %s\n", argv[2]);
            return 0;
        }

        /*deschidem fisier output*/
        FILE *out;
        out = fopen(argv[3], "wb");
        if (!out){
            fprintf(stderr, "ERROR: Nu se poate deschide fisierul %s\n", argv[3]);
            return 0;
        }

        /*citim dimensiune*/
        int dimensiune = 0;
        fread(&dimensiune, sizeof(unsigned int), 1, in);
       
        int type = 0;

        /*initializare structura ce retine RGB si tip*/
        info.TIP = 0;
        info.RED = info.GREEN = info.BLUE = 0;

        /*CITIRE DATE + INSERARE CELULA IN COADA */
        while(fread(&type, sizeof(unsigned char), 1, in)) {
            info.TIP = type;
            if (info.TIP == 1){
            fread(&info.RED, sizeof(unsigned char), 1, in);
            fread(&info.GREEN, sizeof(unsigned char), 1, in);
            fread(&info.BLUE, sizeof(unsigned char), 1, in);
            } else {
                info.RED = info.GREEN = info.BLUE = 0;
            }
            Inserare_Coada(C, info);
        }

        fclose(in);

        int nr_nod = 0; // contor pt numarul de noduri interne = cele care au tipul 0

        // /*CONSTRUIM ARBORELE*/
        Refacere_arbore(&A, C, &nr_nod);
        
        /* alocam matricea pt pixeli ce ne va fi de 
        folos atunci cand trecem datele din arbore in fisier ppm */
        img = (Image1 ) malloc(sizeof(Image));
        if (!img) {
            return 0;
        }

        img->data_ = (INFO *) malloc(sizeof(INFO) * dimensiune * dimensiune );
        // nu s-a reusit alocare
        if (!img->data_){
            free(img->data_);
            free(img);
        }

        // initializare manuala matrice de pixeli
        int j = 0;
        for ( i = 0; i < dimensiune; i++) {
            for( j = 0; j < dimensiune ; j++) {
                img->data_[i * dimensiune + j].RED = 0;
                img->data_[i * dimensiune + j].GREEN = 0;
                img->data_[i * dimensiune + j].BLUE = 0;
            }
        }

        // Aplicam functia prin care trecem datele din arbore in matrice
        // x_reper , y_reper si nivel sunt 0 la inceput, 
        //deoarece incepem cu analiza radacinii
        Matrice_pixeli(A, img, x_reper, y_reper, nivel, dimensiune);

        /*scriere in fisier*/
        Write_file_PPM(img, out, dimensiune);

        /*eliberare*/
        Distrugere_arbore(&A);
        free(img->data_);
        free(img);
        Distrugere_Coada(C);
        fclose(out);

    } else {
        /*CERINTA 1 SI 2*/

        /* deschidere fisier tip binar */
        FILE *in;
        in = fopen(argv[3], "rb");
        if (!in) {
            printf("Unable to upen file %s\n", argv[3]);
            return 0;
        }

        /*creeare fisier out de tip text */
        FILE *out;
        if (strcmp(argv[1], "-c1") == 0) {
            out = fopen(argv[4], "wt");
            if (!out){
                fprintf(stderr, "ERROR: Nu se poate deschide fisierul %s\n", argv[4]);
                return 0;
            }
        }

        /* creeare fisier out de tip binar */
        if (strcmp(argv[1], "-c2") == 0) {
            out = fopen(argv[4], "wb");
            if (!out){
                fprintf(stderr, "ERROR: Nu se poate deschide fisierul %s\n", argv[4]);
                return 0;
            }
        }

        /*retinerea factorului*/
        sscanf(argv[2], "%d", &factor);

        /*citire format fisier*/
        fscanf(in, "%c%d\n", &p, &nr);

        // verificam formatul imaginii daca e P6
        if (p != 'P' || nr != 6) {
            printf("Format gresit\n");
            return 0;
        }

        /*alocam memorie pt imagine */
        img = (Image1 ) malloc(sizeof(Image));
        if (!img) {
            return 0;
        }
        /*citim dimensiunea imaginii*/
        fscanf(in, "%d %d\n", &img->w, &img->h);

        /*citire culoare maxima rgb*/
        fscanf(in, "%d", &val_max_rgb);
        fseek(in,1,SEEK_CUR);

        /*alocare matrice pe baza acesteia vom forma arborele */
        img->data_ = (INFO *) malloc(sizeof(INFO) * img->w * img->h );
        // nu s-a reusit alocare
        if (!img->data_){
            free(img->data_);
            free(img);
        }

        /*initializare manuala matrice de pixeli*/
        int  i = 0, j = 0;
        for ( i = 0; i < img->w; i++) {
            for( j = 0; j < img->h ; j++) {
                img->data_[i * img->w +j].RED = 0;
                img->data_[i * img->w +j].GREEN = 0;
                img->data_[i * img->w +j].BLUE = 0;
            }
        }

        /*citire date in matrice de pixeli*/
        for ( i = 0; i < img->w; i++) {
            for( j = 0; j < img->h ; j++) {
                fread(&img->data_[i * img->w + j], sizeof(unsigned char), 3, in);
            }
        }


        /*CONSTRUIM ARBORELE*/
        /*x_reper , y_reper = 0(initial) vor fi reperele unde se afla coltul 
        stanga sus a fiecarui patrat, nivel = nivelul curent arbore*/
        Constructie_Arbore(&A, *img, x_reper, y_reper, nivel, factor);
        
        int n = 0;
        n = Inaltime(A);

        int frunze = 0;
        frunze = Nr_frunze(A);

        int niv_min = 0;
        niv_min = NIVEL_min(A);
        int dimensiune_lat = img->w / (1<<niv_min);

        /*afisare cerinta 1*/
        if(strcmp(argv[1], "-c1") == 0) {
            fprintf(out, "%d\n", n);
            fprintf(out, "%d\n", frunze);
            fprintf(out, "%d\n", dimensiune_lat);
        }

        /*afisare cerinta 2*/
        if (strcmp(argv[1], "-c2") == 0) {
            // scriem mai intai dimensiunea fisierului
            fwrite(&img->w, sizeof(unsigned int), 1, out);
            Parcurgere_arbore_niveluri(A, out, n);  // n->inaltimea arbore
        }
    
        /*eliberare*/
        fclose(in);
        free(img->data_);
        free(img);
        Distrugere_arbore(&A);
        fclose(out);
    }

    return 0;
    
}
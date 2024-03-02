/*NECULAU SANDA-ELENA GRUPA 313 CB*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


// datele fiecarui nod din arbore
typedef struct{
    unsigned char RED, GREEN, BLUE;
} INFO;

/*structura de informatii pentru coada*/
typedef struct{
    int TIP;
    unsigned char RED, GREEN, BLUE;
} INFO_C;

// Arbore binar cu 4 fii
typedef struct nod {
    INFO data;
    struct nod *st_u, *dr_u, *dr_d, *st_d;  // u --> up, d-->down
    int TIP;
}TNod, *Tarb;

// definire imagine Arbore + dimensiuni
typedef struct {
    INFO *data_;
    int w, h;  // w-->width  h-->height;
}Image, *Image1;

typedef struct celula {
    INFO_C date;
    struct celula *urm;
}Tcelula, *Tlista;

typedef struct coada {
    Tlista inc, sf;
}Tcoada;

int Constructie_Arbore(Tarb *A, Image img, int x, int y, int nivel, int factor);
Tarb Make_frunza(unsigned char red, unsigned char green, unsigned char blue);
Tcoada *Init();
int Inserare_Coada(Tcoada *C, INFO_C info);
int Eliminare_Ele_Coada(Tcoada *C);
void Distrugere_Coada(Tcoada *C);
void Distrugere_Lista(Tlista *L);
void Distrugere_arbore(Tarb *A);
int Inaltime(Tarb A);
int Nr_frunze (Tarb A);
int NIVEL_min(Tarb A);
void Parcurgere_arbore_niveluri(Tarb A, FILE *out, int inaltime);
void Scriere_arbore_niveluri(Tarb A, FILE *out, int nivel);
void Refacere_arbore(Tarb *A, Tcoada *C, int *nr_nod);
void Construire(Tarb *A, Tcoada *C, int *nr_nod, int nivel);
int Matrice_pixeli(Tarb A, Image *img, int x, int y, int nivel, int size);
void Write_file_PPM(Image *img, FILE *out, int size);
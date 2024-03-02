/*NECULAU SANDA-ELENA GRUPA 313 CB*/
#include "tema2.h"
#define RGB_max_color 255

/*creare frunza*/
Tarb Make_frunza(unsigned char red, unsigned char green, unsigned char blue) {
    Tarb aux = (Tarb) malloc(sizeof(TNod));
    if (!aux) {
        return 0;
    }

    aux->data.RED = red;
    aux->data.GREEN = green;
    aux->data.BLUE = blue;
    aux->st_u = aux->dr_u = aux->dr_d = aux->st_d = NULL;

    return aux;
}

Tcoada *Init() {
    Tcoada *c;
    c = (Tcoada*)malloc(sizeof(Tcoada));
    if (!c) {
        return 0;
    }
    c->inc = NULL;
    c->sf = NULL;
}

int Inserare_Coada(Tcoada *C, INFO_C info) {
    
    Tlista aux = NULL;
        /*alocare celula*/ 
        aux = (Tlista) calloc(1,sizeof(Tcelula));
        if (!aux){
                return 0;
        }

        aux->date.TIP = info.TIP;
        aux->date.RED = info.RED;
        aux->date.GREEN = info.GREEN;
        aux->date.BLUE = info.BLUE;

        if (C->sf != NULL) {
                C->sf->urm = aux; /*adaugam elementul intr-o coada nevida*/
        } else {
                C->inc = aux;  /*adaugam elementul intr-o coada vida*/
        }
        C->sf = aux;  /*mutam sfarsitul cozii*/
        return 1;
}

int Eliminare_Ele_Coada(Tcoada *C) {

        Tlista aux ;
        if (C->inc == NULL && C->sf == NULL) { // coada este vida
                return 0;
        } else {
                if (C->inc == C->sf) {   // coada are o singura instructiune
                        aux = C->inc;
                        C->inc = NULL;
                        C->sf = NULL;
                        free(aux);
                } else {
                        aux = C->inc;
                        C->inc = C->inc->urm;
                        free(aux);  
                }
                return 1;
        }        

}

void Distrugere_Coada(Tcoada *C) {
        Tlista aux ;
        aux = C->inc;
        C->inc = NULL;
        C->sf = NULL;
        Distrugere_Lista(&aux);
        free(C);
}

void Distrugere_Lista(Tlista *L) {
        Tlista laux, aux;
        laux = *L;
        while (laux != NULL) {
                aux = laux;
                laux = laux->urm;
                free(aux); 
        }
}

/*img matricea de pixeli, x - index linie, y - index coloana*/
int Constructie_Arbore(Tarb *A, Image img, int x, int y, int nivel, int factor) {

    Tarb aux = NULL, aux2 = NULL, aux3 = NULL, aux4 = NULL;
    /*m_red/m_green/m_blue = suma totala pt fiecare culoare 
    din respectivul patrat*/
    unsigned long long m_red = 0, m_green = 0, m_blue = 0, scor = 0;
    /*ma_red,.... = media aritmetica*/
    unsigned long long ma_red = 0, ma_green = 0, ma_blue = 0, mean = 0;
    int size = 0, i = 0, j = 0, k = 0, t = 0;

    size = img.w ;
    size = size / (1<<nivel);  /*dimensiunea actuala*/

    /* daca imaginea inca se mai poate imparti in pixeli*/
    if (size >= 1) {
        //  sumele culorilor din respectiva arie size*size
            for (i = x; i < x + size; i++) {
                for (j = y; j < y + size; j++) {
                    m_red = m_red + (unsigned long long)img.data_[i * img.w + j].RED;
                    m_green = m_green + (unsigned long long)img.data_[i * img.w + j].GREEN;
                    m_blue = m_blue + (unsigned long long)img.data_[i * img.w + j].BLUE;
                }
            }

        /*media aritmetica a fiecarei culori */
        ma_red = m_red / (size * size);
        ma_green = m_green / (size * size);
        ma_blue = m_blue / (size * size);

        // scor = suma 
        for (k = x; k < x + size; k++) {
            for (t = y; t < y + size; t++) {
                scor = scor + ((ma_red - img.data_[k * img.w + t].RED) * (ma_red - img.data_[k * img.w + t].RED)) + 
                            ((ma_green - img.data_[k * img.w + t].GREEN) * (ma_green - img.data_[k * img.w + t].GREEN)) + 
                                    ((ma_blue - img.data_[k * img.w + t].BLUE) * (ma_blue - img.data_[k * img.w + t].BLUE));
            }
        }

        // scorul final mean
        mean = scor / (3 * size * size);

        if (mean <= factor) {
            /*patratul nu se mai poate imparti, atunci nodul din arbore va deveni 
            frunza si va salva pt fiecare canal de culoare media respectiva */
            if (*A == NULL) {
                // DACA arborele e null nu are radacina
                aux = Make_frunza(ma_red, ma_green, ma_blue);
                *A = aux;
                (*A)->TIP = 1;  //frunza
                // ne oprim deoarece toata imaginea este o singura culoare
                return 0;
            }else {
                // Daca arborele nu este nul
                (*A)->data.RED = ma_red;
                (*A)->data.GREEN = ma_green;
                (*A)->data.BLUE = ma_blue;
                (*A)->TIP = 1;  // frunza
                return 0;
            }
        }else {
            /* Daca scorul este mai mare decat factorul atunci nodul va 
            avea 4 descendenti. Pt inceput vom aloca 4 frunze unde RGB 
            va avea  valoarea 0, deoarece nu stim ce culori vor avea, pe 
            urma vom face legaturile cu radacina si vom aplica recursivitatea 
            la aceasta functie, dar cu NIVELUL si REPERELE X, Y modificate */
            /*daca arborele e vid creeam radacina la care atasam pe urma fii*/
            if(*A == NULL) {
                *A = Make_frunza(0,0,0);
            }
            aux = Make_frunza(0,0,0);
            aux2 = Make_frunza(0,0,0);
            aux3 = Make_frunza(0,0,0);
            aux4 = Make_frunza(0,0,0);
            (*A)->TIP = 0;
            (*A)->st_u = aux;
            (*A)->dr_u = aux2;
            (*A)->dr_d = aux3;
            (*A)->st_d = aux4;
            // x, y --> sunt reperele anterioare ale parintelui
            Constructie_Arbore(&(*A)->st_u, img, x + 0, y + 0, nivel + 1, factor);  
            Constructie_Arbore(&(*A)->dr_u, img, x + 0, y + (img.w / (1<<(nivel + 1))), nivel + 1, factor);
            Constructie_Arbore(&(*A)->dr_d, img, x + (img.w / (1<<(nivel + 1))), y + (img.w / (1<<(nivel + 1))), nivel + 1, factor);
            Constructie_Arbore(&(*A)->st_d, img, x + (img.w / (1<<(nivel + 1))), y + 0, nivel + 1, factor);
        }
    }else {
        return 0;
    }
    return 1;
}

/*distruge arbore*/
void Distrugere_arbore(Tarb *A) {
    if (!(*A)) {
        return;
    }

    Distrugere_arbore(&(*A)->st_u);
    Distrugere_arbore(&(*A)->dr_u);
    Distrugere_arbore(&(*A)->dr_d);
    Distrugere_arbore(&(*A)->st_d);

    free(*A);
}

/*calculeaza inaltimea*/
int Inaltime(Tarb A) {
    int nr_st_u, nr_dr_u, nr_dr_d, nr_st_d, nr;
    if (!A) {
        return 0;
    }
    nr_st_u = Inaltime(A->st_u);
    nr_dr_u = Inaltime(A->dr_u);
    nr_dr_d = Inaltime(A->dr_d);
    nr_st_d = Inaltime(A->dr_d);
    /* comparam inaltimea de pe fiecare ramura sa
    vedem care este mai mare */
    nr = nr_st_u >= nr_dr_u ? nr_st_u : nr_dr_u;
    nr = nr >= nr_dr_d ? nr : nr_dr_d;
    nr = nr >= nr_st_d ? nr : nr_st_d;
    return 1 + nr;  // numaram si nivelul radacinii
}

/*calculeaza nr de frunze =  patratele ce nu mai trebuie divizate*/
int Nr_frunze (Tarb A) {

    if (!A) {
        return 0;
    }

    if (A->st_u == NULL && A->dr_u == NULL && A->dr_d == NULL && A->st_d == NULL) {
        return 1 + Nr_frunze(A->st_u) + Nr_frunze(A->dr_u) + Nr_frunze(A->dr_d) + Nr_frunze(A->st_d);
    }

    return Nr_frunze(A->st_u) + Nr_frunze(A->dr_u) + Nr_frunze(A->dr_d) + Nr_frunze(A->st_d);
}

/*nivelul minim pe care se afla o frunza pentru
 a afla dimensiunea celei mai mari laturi*/
int NIVEL_min(Tarb A) {

    if (!A) {
        return 0;
    }
    /*cautam prima frunza de pe nivelul minim, iar dimensiunea maxima a laturii din imagine va fi
    dimensiunea imaginii impartita la 2^nivel_minim calculul ei se afla in main*/
    // daca exista doar radacina
    if (A->st_u == NULL && A->dr_u == NULL && A->dr_d == NULL && A->st_d == NULL) {
        return 0;
    }
    int val_min_st_u = 0;
    int val_min_dr_u = 0;
    int val_min_dr_d = 0;
    int val_min_st_d = 0;
        
    val_min_st_u = 1 + NIVEL_min(A->st_u);

    val_min_dr_u = 1 + NIVEL_min(A->dr_u);

    val_min_dr_d = 1 + NIVEL_min(A->dr_d);

    val_min_st_d = 1 + NIVEL_min(A->st_d);

    /*comparam valorile pe ramuri sa vedem care are cea mai mica inaltime*/
    int min = 0;
    min = val_min_st_u >= val_min_dr_u ? val_min_dr_u : val_min_st_u;
    min = min >= val_min_dr_d ? val_min_dr_d : min;
    min = min >= val_min_st_d ? val_min_st_d : min;
    return min;

}

/*mai intai parcurgem arborele pe nivele si apoi apelam o functie pentru
a scrie nodurile in fisier*/
void Parcurgere_arbore_niveluri(Tarb A, FILE *out, int inaltime) {
    int i = 0;
    for (i = 0; i < inaltime; i++) {
        Scriere_arbore_niveluri(A, out, i);
    }

}

/*Afisare cerinta 2*/
void Scriere_arbore_niveluri(Tarb A, FILE *out, int nivel) {

    if (!A) {
        return;
    }

    /* s-a ajuns la nivelul dorit */
    if (nivel == 0) {
        if (A->TIP == 0){
            fwrite(&A->TIP, sizeof(unsigned char), 1, out);
        } else if (A->TIP == 1) {
            fwrite(&A->TIP, sizeof(unsigned char), 1, out);
            fwrite(&A->data, sizeof(unsigned char), 3, out);
        }

    } else {
        Scriere_arbore_niveluri(A->st_u, out, nivel - 1);
        Scriere_arbore_niveluri(A->dr_u, out, nivel - 1);
        Scriere_arbore_niveluri(A->dr_d, out, nivel - 1);
        Scriere_arbore_niveluri(A->st_d, out, nivel - 1);
    }

}

/*nr_nod va tine stocat numarul de noduri care nu sunt frunze pana
la nivelul i. Daca nr_nod ajunge la 0 atunci arborele este generat complet*/
/*Acest algoritm se bazeaza pe numarul de noduri interne*/ 
void Refacere_arbore(Tarb *A, Tcoada *C, int *nr_nod) {

    /*la inceput arborele e null si trebuie sa-i punem radacina */
    Tarb aux = NULL;
    int i = 0;
    aux = Make_frunza(C->inc->date.RED, C->inc->date.GREEN, C->inc->date.BLUE);
    *A = aux;
    (*A)->TIP = C->inc->date.TIP;
    /*eliminam curentul din coada pe care l-am pus in arbore*/
    Eliminare_Ele_Coada(C);

    // daca este nod si nu frunza atunci se continua constructia arborelui
    if ((*A)->TIP == 0) {
        (*nr_nod)++;
        for (i = 0; *nr_nod > 0; i++) {
            Construire(A, C, nr_nod, i);
        }
    }
}

/*construirea arborelui*/
void Construire(Tarb *A, Tcoada *C, int *nr_nod, int nivel) {

    Tarb aux = NULL, aux1 = NULL, aux2 = NULL, aux3 = NULL;

    if (!A) {
        return;
    }
    /*cand s-a ajuns la nivelul dorit*/
    if (nivel == 0) {
        // daca este nod intern
        if ((*A)->TIP == 0) {
            // alocam cei 4 descendenti si le dam si valorile din vector
            aux = Make_frunza(C->inc->date.RED, C->inc->date.GREEN, C->inc->date.BLUE);
            aux->TIP = C->inc->date.TIP;
            Eliminare_Ele_Coada(C);
            /*daca este nod intern atunci nr_nod se mareste*/
            if (aux->TIP == 0) {
                (*nr_nod)++;
            }

            aux1 = Make_frunza(C->inc->date.RED, C->inc->date.GREEN, C->inc->date.BLUE);
            aux1->TIP = C->inc->date.TIP;
            Eliminare_Ele_Coada(C);
            if (aux1->TIP == 0) {
                (*nr_nod)++;
            }

            aux2 = Make_frunza(C->inc->date.RED, C->inc->date.GREEN, C->inc->date.BLUE);
            aux2->TIP = C->inc->date.TIP;
            Eliminare_Ele_Coada(C);
            if (aux2->TIP == 0) {
                (*nr_nod)++;
            }

            aux3 = Make_frunza(C->inc->date.RED, C->inc->date.GREEN, C->inc->date.BLUE);
            aux3->TIP = C->inc->date.TIP;
            Eliminare_Ele_Coada(C);
            if (aux3->TIP == 0) {
                (*nr_nod)++;
            }

            // facem legaturile
            (*A)->st_u = aux;
            (*A)->dr_u = aux1;
            (*A)->dr_d = aux2;
            (*A)->st_d = aux3;

            /*eliminam nodul la care am adaugat deja fii*/
            (*nr_nod)--;
        }

    } else {
        if ((*A)->st_u != NULL) {
            Construire(&(*A)->st_u, C, nr_nod, nivel - 1);
        }
        if ((*A)->dr_u != NULL) {
            Construire(&(*A)->dr_u, C, nr_nod, nivel - 1);
        }
        if ((*A)->dr_d != NULL) {
            Construire(&(*A)->dr_d, C, nr_nod, nivel - 1);
        }
        if ((*A)->st_d != NULL) {
            Construire(&(*A)->st_d, C, nr_nod, nivel - 1);
        }
    }


}

/*reconstruire matrice pixeli-imagine*/
/*Parcurgem arborele pe inaltime si pe baza formulei pe care am aplicat-o 
si la cerinta 1 retinem in matricea de pixeli pentru fiecare pixel codurile
de culoare corespunzatoare zonei*/
int Matrice_pixeli(Tarb A, Image *img, int x, int y, int nivel, int size) {

    int i = 0, j = 0;
    if (!A) {
        return 0;
    }
    if (A->TIP == 1) {   // daca e frunza 
        for (i = x; i < x + (size / (1<<nivel)); i++) {
            for (j = y; j < y + (size / (1<<nivel)); j++) {
                
                (*img).data_[i * size + j].RED = A->data.RED;
                (*img).data_[i * size + j].GREEN = A->data.GREEN;
                (*img).data_[i * size + j].BLUE = A->data.BLUE;
            }
        }
        return 1;
    } else {
        Matrice_pixeli(A->st_u, img, x + 0, y + 0, nivel + 1, size);
        Matrice_pixeli(A->dr_u, img, x + 0, y + (size / (1<<(nivel +1))) , nivel + 1, size);
        Matrice_pixeli(A->dr_d, img, x + (size / (1<<(nivel +1))), y + (size / (1<<(nivel +1))), nivel + 1, size);
        Matrice_pixeli(A->st_d, img, x + (size / (1<<(nivel +1))), y + 0, nivel + 1, size);
    }

}

/*Afisare cerinta 3*/
void Write_file_PPM(Image *img, FILE *out, int size) {
    fprintf(out, "P6\n");
    fprintf(out, "%d %d\n", size, size);
    fprintf(out, "%d\n", RGB_max_color);
    int i = 0, j = 0;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            fwrite(&(*img).data_[i * size + j], sizeof(unsigned char), 3, out);
        }
    }

} 
In MAIN in functie de cerinta pe care trebuie sa o rezolvam vom deschide fisierele de IN si creape cele de OUT. 

In program mai intai se verifica daca se cere cerinta a 3 a. Pe urma dupa deschiderea ambelor
fisiere se citeste dimesiunea din fisierul COMPRIMAT, apoi initializam o coada unde dupa ce citim pe rand 
informatiile din fisier adaugam cate o celula in coada prin functia "Inserare_Coada". Dupa aplicam 
functia "Refacere_arbore", prin care construim arborele pornind de la datele cozii. Functia va avea ca
parmetri pe langa arbore si coada un "nr_nod" ce va tine stocat numarul de noduri interne (care nu sunt
frunze). Cand "nr_nod" ajunge la 0 atunci stim ca arborele este generat complet si ne oprim.  Pt inceput
in functie alocam mai intai radacina arborelui, dupa verificam daca aceasta este frunza sau nod. Daca 
este nod atunci continuam constructia arborelui prin apelarea functiei "Costruire", care are aceeasi
parametri la care se mai adauga si nivelul la care o sa ne oprim ca sa adugam cei 4 fii daca nodul este
intern, actualizand in acelasi timp si "nr_nod". Dupa ce am terminat de legat cei 4 fii ai nodului
micsoram valoarea "nr_nod". Aceasta functie va fi aplicata recursiv in ordinea stanga sus, dreapta sus,
dreapta jos, stanga jos.


Dupa ce am refacut arborele vom aloca memorie pt Matricea de pixeli cu dimensiunea size^2. Si pe 
aceasta la randul ei o initializam cu zero, ca mai apoi sa aplicam functia "Matrice pixeli" care are ca
parametri arborele, matricea, reperele x, y , inaltimea si dimensiunea. La randul ei aceasta functie este
recursiva.


Daca nodul din arbore este intern atunci in functie de tipul de nod (st sus, dr sus, dr jos, st
jos) vom retine in matricea de pixeli incepand de la reperele (x,y reprezentative pt tipul nodului) RGB-
ului nodului.

Suprafata pe care se va retine acelasi RGB incepe de la (x,y) care continua pana la x +
size/(2^nivel) si y + size/(2^nivel). Atunci cand functia este apelata din nou reperele x, y se vor
modifica in functie de tipul nodului si nivelul va avansa.
	
 La sfarsitul cerintei 3 vom scrie fisierul de tip PPM cu datele din matricea de pixeli.
	
Daca se cere cerinta 1 si 2 se deschide fisierul de IN, in functie de cerinta se creaza cel de 
OUT, dupa care se retine factorul, se citeste formatul fisierului, se aloca memorie pt Matricea de
pixeli, se citeste dimensiunile width si height retinute in structura "img" care retine si Matricea,
citim val maxima a RGB-ului. Alocam memorie in matrice pt toti pixeli avand dimensiunea width*height, o
initializam cu 0 si apoi citim din fisier datele pixelilor. 

Pentru a construi arborele pe baza matricei Apelam functia "Construire_Arbore" care are ca
parametrii arborele, matricea, reperele x, y, nivel si factorul. Aceasta functie are aceeasi logica ca si
cea de la cerinta 3. In functie de reperele x, y si nivel se va calcula media aritmetica pt fiecare
culoare pe aria respectiva, ca mai apoi sa se calculeze scorul. 
		
Daca scorul este mai mic sau egal decat factorul (nodurile vor fi frunze), atunci
verificam mai intai daca arborele este null, daca este atunci ii cream radacina in care retinem media
aritmetica a fiecarei culoare si ne oprim, daca arborele nu este null, atunci cream frunza in care
retinem RGB si facem legaturile corespunzatoare.
		
Daca scorul este mai mare decat factorul, pt inceput trebuie verificat si aici daca
arborele este null sau nu pt a face radacina, care de aceasta data va fi 0, pe urma pt nodul actual cream
cei 4 descendenti tot cu valorile 0 la RGB deoarece inca nu stim daca acestea mai au descendenti sau nu,
iar pt acest lucru vom aplica recursiv functia, dar cu reperele x,y si nivelul modificat pt fiecare tip
de nod.
	
Dupa ce am construit arborele calculam inaltimea cu functia "Inaltime", calculam numarul de
frunze cu functia "Nr_frunze", iar pt a afla lungimea celei mai lungi laturi, mai intai cautam nivelul
minim din arbore pentru ca acolo se afla frunza cea mai apropriata de radacina. Acest lucru inseamna ca
patratul respectiv nu este impartit atat de des si are latura cea mai mare, iar lungimea laturii va fi
dimensiunea imaginii/2^(nivel_minim).

Pentru cerinta 1 vom afisa cele 3 rezultate mentionate anterior.
	
Pentru cerinta 2 vom apela functia de "Parcurgere_arbore_niveluri", care are ca parametri
arborele, inaltimea si fisierul in care se va scrie. Pt fiecare nivel se va apela o alta functie
recursiva care parcurge arborele, iar cand ajunge la nivelul respectiv afiseaza datele nodului respectiv
in formatul dat in cerinta.

La sfarsit memoria este eliberata.
Punctaj 80/80

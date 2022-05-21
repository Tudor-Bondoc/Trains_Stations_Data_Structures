#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define infinit INT_MAX

void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

//structura pentru graf

struct graf
{
    int V;
    int E;
    int **a;
};

typedef struct graf G;

//structura pentru lista cu statii unice

struct orase
{
    char nume[40];
    struct orase *next;
    int poz_in_lista;
};

typedef struct orase TOWNS;


//structura pentru lista cu toate statiile (cu dubluri)

struct dubluri
{
    char nume [40];
    int poz_in_lista;
    int costul_de_la_origine;
    struct dubluri *next;
};

typedef struct dubluri DUBL;

int contor = 0; //pentru pozitia in lista cu statii unice
int contor2 = 0; //pentru pozitia in lista cu dubluri

//FUNCTII

void addAtBeginning(TOWNS**head, char denumiree[40], int pozitia)
{
    TOWNS* newNode=(TOWNS*)malloc(sizeof(TOWNS));
    strcpy(newNode->nume, denumiree);
    newNode->poz_in_lista = pozitia;
    newNode->next=*head;
    *head=newNode;
}

void addAtEnd(TOWNS**head, char denumiree[40], int pozitia)
{
    TOWNS*aux=*head, *headcopy=*head;
    TOWNS* newNode=(TOWNS*)malloc(sizeof(TOWNS)); /*creez un nou nod*/
    strcpy(newNode->nume, denumiree);
    newNode->poz_in_lista = pozitia;

    while (headcopy!=NULL) {/* cat timp nu am ajuns la finalul listei*/
    aux=headcopy; /*stochez potentialul ultim element in aux – adaug dupa el*/
    headcopy=headcopy->next; /* si trec la urmatorul element*/
    }
    if(aux==NULL) addAtBeginning(&*head,denumiree, pozitia); /*daca lista era goala se modifica adresa continuta de capul listei*/
    else{
    aux->next=newNode; /*pun noul nod pe ultima pozitie*/
    newNode->next=NULL; /*si urmatorul element e NULL*/
    }
}

void addAtBeginning_dubl(DUBL**head, char denumiree[40], int pozitia, int cost)
{
    DUBL* newNode=(DUBL*)malloc(sizeof(DUBL));
    strcpy(newNode->nume, denumiree);
    newNode->poz_in_lista = pozitia;
    newNode->costul_de_la_origine = cost;
    newNode->next=*head;
    *head=newNode;
}

void addAtEnd_dubl(DUBL**head, char denumiree[40], int pozitia, int cost)
{
    DUBL*aux=*head, *headcopy=*head;
    DUBL* newNode=(DUBL*)malloc(sizeof(DUBL)); /*creez un nou nod*/
    strcpy(newNode->nume, denumiree);
    newNode->poz_in_lista = pozitia;
    newNode->costul_de_la_origine = cost;

    while (headcopy!=NULL) {/* cat timp nu am ajuns la finalul listei*/
    aux=headcopy; /*stochez potentialul ultim element in aux – adaug dupa el*/
    headcopy=headcopy->next; /* si trec la urmatorul element*/
    }
    if(aux==NULL) addAtBeginning_dubl(&*head,denumiree, pozitia, cost); /*daca lista era goala se modifica adresa continuta de capul listei*/
    else{
    aux->next=newNode; /*pun noul nod pe ultima pozitie*/
    newNode->next=NULL; /*si urmatorul element e NULL*/
    }
}

//functie ce verifica daca un oras se afla deja in lista

int se_gaseste_in_lista (TOWNS *pointer, char denumire[40])
{
    while (pointer!=NULL)
    {
        if (strcmp(pointer->nume, denumire) == 0)
            return 1;
        pointer = pointer->next;
    }
    return 0;
}

int nr_statii (TOWNS *iter)
{
    int ct = 0;
    while (iter != NULL)
    {
        ct ++;
        iter = iter->next;
    }
    return ct;
}

int pozitia_din_lista_mica (char statie[40], TOWNS *head)
{
    while (head != NULL)
    {
        if (strcmp(head->nume, statie) == 0)
            return head->poz_in_lista;
        head = head->next;
    }
    return -1; //in cazul in care nu s-a gasit
}

void afiseaza_lista (TOWNS *iter)
{
    while (iter!=NULL)
    {
        printf("'%s'\n", iter->nume);
        iter = iter->next;
    }
}

int nr_statii_care_se_repeta (TOWNS *head)
{
    int ct = 0;
    TOWNS *iter;
    while (head != NULL)
    {
        iter = head->next;
        while (iter != NULL)
        {
            if (strcmp(head->nume, iter->nume) == 0)
            {
                ct ++;
                break;
            }
            iter = iter->next;
        }
        head = head->next;
    }
    return ct;
}

int minimul (int a, int b)
{
    if (a <= b) return a;
    return b;
}

char *cauta_statie_dupa_index (TOWNS *head, int poz)
{
    while (head != NULL)
    {
        if (head->poz_in_lista == poz)
        {
            return head->nume;
        }
        head = head->next;
    }
}


int minDistance(int dist[], int sps[], int V)
{
    int v, min = infinit, min_index;
    for ( v = 0; v < V; v++)
    if (sps[v] == 0 && dist[v] <= min) {
    min = dist[v];
    min_index = v;
    }
    return min_index;
}

void printSolution(int dist[], int sursaaa, int index, int pred[], TOWNS *head)
{
    FILE *f;
    f = fopen("ruta.txt", "wt");
    fprintf(f, "Distanta de la sursa la statia selectata este %d\n", dist[index]);
    int actual = index;
    do
    {
        fprintf(f, "%s\n", cauta_statie_dupa_index(head, actual));
        actual = pred[actual];
    } while(pred[actual] != -1);
    fprintf(f, "%s\n", cauta_statie_dupa_index(head, sursaaa));
    fclose(f);

}

void dijkstra(G *g, int s, int d, TOWNS *head)
{
    int i, j, u, predecesor;
    int dist[g->V]; // vectorul cu distantele de la sursa la destinatie
    int sps[g->V]; // sps[i] e 1 daca varful i e inclus in cea mai scurta cale
    int pred[g->V];
    // initial toate distantele sunt infinit si niciun element nu e in sps
    for ( i = 0; i < g->V; i++)
    {
        dist[i] = infinit;
        sps[i] = 0;
        pred[i] = -1;
    }
    // distanta de la sursa la ea insasi e 0 si adaugam sursa in sps
    dist[s] = 0;
    //calculez pentru toate varfurile calea de cost minim
    for (j = 0; j < g->V-1; j++)
    {
        // iau varful cu distanta minima pana la sursa dintre varfurile care nu sunt in sps
        u = minDistance(dist, sps, g->V);
        //printf("%d\n",u);
        // il adaug in sps
        sps[u] = 1;
        //incep sa il procesez
        for (i = 0; i < g->V; i++)
        {
            // Updatez dist[i] daca varful i nu e in sps, daca u si i adiacente si
            //daca distanta pe noua ruta e mai mica decat cea stocata anterior
            if (sps[i] == 0 && g->a[u][i]!=infinit && dist[u] != INT_MAX && dist[u]+g->a[u][i] < dist[i])
            {
                    dist[i] = dist[u] + g->a[u][i];
                    pred[i] = u;
            }

        }
    }
    pred[s] = -1;
    // afiseaza vectorul cu distantele de la sursa la fiecare varf
    printSolution(dist, s, d, pred, head);
}


int main()
{
    TOWNS *head;
    head = (TOWNS*)malloc(sizeof(TOWNS));
    head = NULL;
    DUBL *head_dubl;
    head_dubl = (DUBL*)malloc(sizeof(DUBL));
    head_dubl = NULL;
    FILE *f, *g;
    char denumire_statie[40], rand[100], *p, nume_fisier[15];
    int cost;

    //crearea listei cu statii unice, care contine denumirea statiei si indicele corespunzator

    g = fopen("_all_files.txt", "rt");
    while (fgets(nume_fisier, 15, g))
    {
        nume_fisier[strlen(nume_fisier)] = '\0';
             if (nume_fisier[strlen(nume_fisier)-1] == '\n')
             {
                 nume_fisier[strlen(nume_fisier)-1] = '\0';
             }
             if (nume_fisier[strlen(nume_fisier)-1] == '\r')
             {
                 nume_fisier[strlen(nume_fisier)-1] = '\0';
             }
        f = fopen(nume_fisier, "rt");
        while (fgets(rand, 100, f))
        {
            rand[strlen(rand)] = '\0';
             if (rand[strlen(rand)-1] == '\n')
             {
                 rand[strlen(rand)-1] = '\0';
             }
             if (rand[strlen(rand)-1] == '\r')
             {
                 rand[strlen(rand)-1] = '\0';
             }
            p = strtok (rand, ",");
            strcpy (denumire_statie, p);
            p = strtok(NULL, " ");
            cost = atoi(p);
            if (se_gaseste_in_lista(head, denumire_statie) == 0)
                {
                    addAtEnd(&head, denumire_statie, contor);
                    contor ++;
                }
        }
        fclose (f);
    }
    fclose(g);

    //crearea listei cu dubluri, care contine denumirea statiei si costul asociat, citit din fisiere

    g = fopen("_all_files.txt", "rt");
    while (fgets(nume_fisier, 15, g))
    {
        nume_fisier[strlen(nume_fisier)] = '\0';
             if (nume_fisier[strlen(nume_fisier)-1] == '\n')
             {
                 nume_fisier[strlen(nume_fisier)-1] = '\0';
             }
             if (nume_fisier[strlen(nume_fisier)-1] == '\r')
             {
                 nume_fisier[strlen(nume_fisier)-1] = '\0';
             }
        f = fopen(nume_fisier, "rt");
        while (fgets(rand, 100, f))
        {
            rand[strlen(rand)] = '\0';
             if (rand[strlen(rand)-1] == '\n')
             {
                 rand[strlen(rand)-1] = '\0';
             }
             if (rand[strlen(rand)-1] == '\r')
             {
                 rand[strlen(rand)-1] = '\0';
             }
            p = strtok (rand, ",");
            strcpy (denumire_statie, p);
            p = strtok(NULL, " ");
            cost = atoi(p);
            addAtEnd_dubl(&head_dubl, denumire_statie, contor2, cost);
            contor2 ++;
        }
        fclose (f);
    }
    fclose(g);

    printf("\nExista %d statii unice\n", nr_statii(head));

    //crearea propriu zisa a grafurilor

    G *graf_neorientat, *graf_orientat;

    graf_neorientat = (G*)malloc(sizeof(G));
    graf_orientat = (G*)malloc(sizeof(G));
    graf_neorientat->V = nr_statii(head);
    graf_orientat->V = nr_statii(head);
    graf_neorientat->E = 0;
    graf_orientat->E = 0;

    //matricea de costuri neorientata

    graf_neorientat->a = (int**)malloc((graf_neorientat->V) * sizeof(int*));
    int i, j;
    for (i = 0; i< graf_neorientat->V; i++)
        graf_neorientat->a[i] = (int*)malloc(graf_neorientat->V*sizeof(int));
    for(i=0;i<graf_neorientat->V;i++)
    {
        for(j=0;j<graf_neorientat->V;j++)
        {
            graf_neorientat->a[i][j] = infinit;
        }
    }

    DUBL *iterator_dubl;
    iterator_dubl = (DUBL*)malloc(sizeof(DUBL));
    iterator_dubl = head_dubl;
    while (iterator_dubl->next != NULL)
    {
        if (iterator_dubl->next->costul_de_la_origine != 0)
        {
            graf_neorientat->a[pozitia_din_lista_mica(iterator_dubl->nume, head)][pozitia_din_lista_mica(iterator_dubl->next->nume, head)] = minimul (iterator_dubl->next->costul_de_la_origine - iterator_dubl->costul_de_la_origine, graf_neorientat->a[pozitia_din_lista_mica(iterator_dubl->nume, head)][pozitia_din_lista_mica(iterator_dubl->next->nume, head)]);
            graf_neorientat->a[pozitia_din_lista_mica(iterator_dubl->next->nume, head)][pozitia_din_lista_mica(iterator_dubl->nume, head)] = minimul (iterator_dubl->next->costul_de_la_origine - iterator_dubl->costul_de_la_origine, graf_neorientat->a[pozitia_din_lista_mica(iterator_dubl->next->nume, head)][pozitia_din_lista_mica(iterator_dubl->nume, head)]);
        }
        iterator_dubl = iterator_dubl->next;
    }

    //determin cate muchii sunt in graful neorientat

    for(i=0;i<graf_neorientat->V;i++)
    {
        for(j=0;j<graf_neorientat->V;j++)
        {
            if(graf_neorientat->a[i][j] != infinit)
                graf_neorientat->E = graf_neorientat->E + 1;
        }
    }
    graf_neorientat->E = graf_neorientat->E / 2;

    printf ("\nIn graful neorientat exista %d muchii\n", graf_neorientat->E);

    //matricea de costuri orientata

    graf_orientat->a = (int**)malloc((graf_orientat->V) * sizeof(int*));
    for (i = 0; i< graf_orientat->V; i++)
        graf_orientat->a[i] = (int*)malloc(graf_orientat->V*sizeof(int));
    for(i=0;i<graf_orientat->V;i++)
    {
        for(j=0;j<graf_orientat->V;j++)
        {
            graf_orientat->a[i][j] = infinit;
        }
    }

    iterator_dubl = head_dubl;
    while (iterator_dubl->next != NULL)
    {
        if (iterator_dubl->next->costul_de_la_origine != 0)
        {
            graf_orientat->a[pozitia_din_lista_mica(iterator_dubl->nume, head)][pozitia_din_lista_mica(iterator_dubl->next->nume, head)] = iterator_dubl->next->costul_de_la_origine - iterator_dubl->costul_de_la_origine;
        }
        iterator_dubl = iterator_dubl->next;
    }

    //determin cate muchii sunt in graful orientat


    for(i=0;i<graf_orientat->V;i++)
    {
        for(j=0;j<graf_orientat->V;j++)
        {
            if(graf_orientat->a[i][j] != infinit)
                graf_orientat->E = graf_orientat->E + 1;
        }
    }

    printf ("\nIn graful orientat exista %d muchii\n", graf_orientat->E);

    //determin statiile consecutive care au distanta minima intre ele

    int dist_min = graf_neorientat->a[0][0];
    int statia1 = 0, statia2 = 0;

    for (i=0; i<graf_neorientat->V; i++)
    {
        for (j=0; j<graf_neorientat->V; j++)
        {
            if (graf_neorientat->a[i][j] < dist_min)
            {
                dist_min = graf_neorientat->a[i][j];
            }
        }
    }

    f = fopen ("min.txt", "wt");
    fprintf(f, "Distanta minima este de %d si se gaseste intre urmatoarele statii:\n", dist_min);
    for (i=0; i<graf_neorientat->V; i++)
    {
        for (j=0; j<graf_neorientat->V; j++)
        {
            if (j>i && graf_neorientat->a[i][j] == dist_min)
            {
               fprintf (f, "%s-%s\n", cauta_statie_dupa_index(head, i), cauta_statie_dupa_index(head, j));
            }
        }
    }
    fclose(f);

    //determin statiile consecutive care au distanta maxima intre ele

    int dist_max = 0;

    for (i=0; i<graf_neorientat->V; i++)
    {
        for (j=0; j<graf_neorientat->V; j++)
        {
            if (j>i && graf_neorientat->a[i][j] > dist_max && graf_neorientat->a[i][j] != infinit)
            {
                dist_max = graf_neorientat->a[i][j];
            }
        }
    }


    f = fopen ("max.txt", "wt");
    fprintf(f, "Distanta maxima este de %d si se gaseste intre urmatoarele statii:\n", dist_max);
    for (i=0; i<graf_neorientat->V; i++)
    {
        for (j=0; j<graf_neorientat->V; j++)
        {
            if (j>i && graf_neorientat->a[i][j] == dist_max)
            {
               fprintf (f, "%s-%s\n", cauta_statie_dupa_index(head, i), cauta_statie_dupa_index(head, j));
            }
        }
    }
    fclose(f);

    //determin cati km de cale ferata avem in total in baza de date
    //parcurg lista mare si daca next->cost este 0, adun la suma costul de pe pozitia pe care ma aflu

    int total_km = 0;
    iterator_dubl = head_dubl;
    while (iterator_dubl->next != NULL)
    {
        if (iterator_dubl->next->costul_de_la_origine == 0)
        {
            total_km = total_km + iterator_dubl->costul_de_la_origine;
        }
        iterator_dubl = iterator_dubl->next;
    }

    f = fopen("total.txt", "wt");
    fprintf (f, "Nr. total de km de cale ferata: %d", total_km);
    fclose(f);


    //gasirea drumul minim de la Bucuresti la Ramnicu Valcea (Dijkstra)

    char sursa[40], destinatie[40];
    strcpy(sursa, "Bucuresti Nord");
    strcpy(destinatie, "Ramnicu Valcea");

    dijkstra(graf_neorientat, pozitia_din_lista_mica(sursa, head), pozitia_din_lista_mica(destinatie, head), head);




    return 0;
}

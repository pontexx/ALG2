#ifndef G_H_
#define G_H_

#include <stdio.h>
#include <stdlib.h>
#include "dimensao.h"

struct lista_de_arestas;

//DEFINI��O DO V�RTICE DO GRAFO
typedef struct vertice {
    int i; //EXEMPLO DE CONTE�DO

    struct lista_de_arestas *A; //ENDER�O DA LISTA DE ARESTAS DESTE V�RICE
    struct vertice *prox; //PR�X VERTICE DO GRAFO

    char sigla[100];

    struct dimensao *dim;
    int nvl_de_cada_dimensao[10];
} v;

typedef struct aresta {
   v* origem;
   v* destino; //DESTINO DA ARESTA (UMA DIMENS�O)
   struct aresta *prox; //PROX ARESTA DA LISTA DE ARESTAS
} a;


typedef struct lista_de_arestas{
    a* inicio; //INICIO DA LISTA
} A;

typedef struct lista_de_vertices{
    v* inicio; //INICIO DA LISTA
} V;

//GRAFO(LISTA DE VERTICES E LISTAS DE ARESTAS)
typedef struct GRAFO{
    V listaV;
} G;


int estaVazioV(V*);
v cria_vertice(int n, char *s, dimensao *dim);
void insere_vertice(V*, v*);
a cria_aresta(v* origem, v* destino);
int estaVazioA(A*);
void insere_aresta(v*, a*);
V* gera_permutacao(v *vt);
int fatorial(int a);
void GG(G *GRAFO);
void gerarSigladoVertice(v *vert, int qd);
V* permuta_dim(v *vert, int qd, int *conjuntos_vazios_encontrados);
void remove_duplicata(v *LV);
#endif

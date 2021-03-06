/// GABRIEL PONTES 9313030
/// ANDR� DAHER BENEDETTI 9622772
/// NATHALIA MORENO PEREIRA 9266560
/// RAFAEL YONEZAWA DE MELLO 9313026

#ifndef _DIMENSAO_H
#define _DIMENSAO_H

//Estrutura que receber� as dimens�es
typedef struct dimensao {
    int numAtributos;
    char nome[16];
    char sigla[4];
    struct atributo *atributos;
    int ID;
} dimensao;

//Estrutura que receber� os atributos
typedef struct atributo {
    char nome[16];
    char sigla[4];
    int nvl_hierarquia;
} atributo;

typedef struct lista_de_atributo{
    atributo *inicio;
} lista_de_atributo;

typedef struct lista_de_dimensao{
    dimensao *dimensoes;
    int tamanho;
    char **siglas;
    int totalElementos;
} lista_de_dimensao;

char * gerarSigla(dimensao *dimensoes, int n, char nome[16]);
void criarDimensao(char nome[15], dimensao * dimensoes, int n, int maxAtributos);
char * gerarSiglaAtrib(atributo * atributos, int n, char nome[16], int ID);
void inserirAtributo(char nome[16], dimensao * dim, int nvl);

#endif //_DIMENSAO_H

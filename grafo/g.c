#include "g.h"

/// VERIFICA SE A LISTA DE VERTICES ESTA VAZIA
int estaVazioV(V *LV){
    if(LV->inicio == NULL){
        //printf("LV vazia.\n");
        return 1;
    }
    //printf("LV nao vazia.\n");
    return 0;
}

/// FUN��O PARA CRIAR NOVO V�RTICE
v cria_vertice(int n, char *s, dimensao *dim){
    v novo;
    novo.i = n;
    strcpy(novo.sigla, s);
    novo.prox = NULL;
    novo.A = (A*) malloc (1 * sizeof(A));
    novo.A->inicio = NULL;
   //printf("Vertice %d criado.\n", n);

    novo.dim = dim;

    int i;
    for(i = 0; i < 10; i++){
        novo.nvl_de_cada_dimensao[i] = 0;
    }

    for(i = 0; i < 100; i++){
        novo.dimensoes_disponiveis[i] = 1;
    }

    return novo;
}

//INSERE NOVO VERTICE NA LISTA DE VERTICES
v* insere_vertice(struct lista_de_vertices *LV, v *novo){
    v *aux;
    if(estaVazioV(LV)){
        LV->inicio = (v*) malloc (1 * sizeof(v));
        *LV->inicio = *novo;
    } else {

        aux = LV->inicio;
        //printf("Comecou no vertice %d\n", aux->i);
        while(aux->prox != NULL){
           // printf("Passou pelo vertice %d\n", aux->i);
            aux = aux->prox;
        }
        //printf("Parou no vertice %d\n", aux->i);
        aux->prox = (v*) malloc (1 * sizeof(v));
        *aux->prox = *novo;
    }
    return aux->prox;
    //printf("Vertice %d inserido no grafo.\n", novo->i);
}

//FUN��O PARA CRIAR UMA NOVA ARESTA
a cria_aresta(v* origem, v *destino){
    a novo;
    novo.prox = NULL;
    novo.origem = origem;
    novo.destino = destino;
    //printf("\nAresta de %s para %s criada.\n", origem->sigla, destino->sigla);
    return novo;
}

//VERIFICA SE A LISTA DE ARESTA ESTA VAZIA
int estaVazioA(A *LA){
    if(LA->inicio == NULL){
        return 1;
    }
    return 0;
}

//FUN��O PARA INSERIR UMA ARESTA NUMA LISTA
void insere_aresta(v *origem, a *novo){
    A* LA = origem->A;
    if(estaVazioA(LA)){
        //printf("Lista de arestas estava vazia.\n");
        LA->inicio = (a*) malloc (1 * sizeof(a));
        *LA->inicio = *novo;
        //printf("Aresta foi inserida no inicio da lista.\n");
    } else {
        a *aux;
        aux = LA->inicio;
        while(aux->prox != NULL)
            aux = aux->prox;
        aux->prox  = (a*) malloc (1 * sizeof(a));
        *aux->prox = *novo;
        //printf("Aresta foi inserida no final da lista.\n");
    }
   /* printf("\nAresta de %s para %s criada.\n", novo->origem->sigla, novo->destino->sigla);
    printf("Aresta %d -> %d inserida no grafo.\n", origem->i, novo->destino->i);*/
}

/// FUN��O PARA GERAR A SIGLA DE UM V�RTICE
/// BASEADO NAS SUAS DIMENSOES E HIERARQUIA
/// qd = QNT TOTAL DE DIMENSOES + ATRIBUTOS
void gerarSigladoVertice(v *vert, int qd){
    char sigla[100];
    sigla[0] = '\0';
    int i;
    for(i = 0; i < qd; i++){
        if(vert->nvl_de_cada_dimensao[i] == 0){
            strcat(sigla, vert->dim[i].sigla);
        } else if(vert->nvl_de_cada_dimensao[i] <= vert->dim[i].numAtributos){
            strcat(sigla, vert->dim[i].atributos[vert->nvl_de_cada_dimensao[i]-1].sigla);
        }
    }
    strcpy(vert->sigla, sigla);
}

void gerarSigladoVerticeD(v *vert, int qd){
    char sigla[100];
    sigla[0] = '\0';
    int i;
    for(i = 0; i < qd; i++){
        if(vert->nvl_de_cada_dimensao[i] == 0){
            strcat(sigla, vert->dim[i].sigla);
        }
    }
    strcpy(vert->sigla, sigla);
}

/// FUN��O PARA GERAR AS COMBINA��ES "FILHAS"
/// DE UM V�RICE
/// LV = LISTA A SER ADD O V�RTICE
/// VERTICE A SER RECOMBINADO
/// qd = QNT TOTAL DE DIMENSOES + ATRIBUTOS
/// ST MATRIZ DE SIGLAS DAS DIMENSOES E ATRIBUTOS
/// FLAG_ISOLADOS SERVE COMO INTERRUP��O DA GEREA��O DO GRAFO
void permuta_dim(V *LV, v *vert, int qd, char **ST, int flag_isolados[], int total_de_elementos){
    v nv;
    a ar;

    int flag_para_gerar_vertice = 0;
    int it_dim;
    for(it_dim = 0; it_dim < qd; it_dim++){
        if(vert->nvl_de_cada_dimensao[it_dim] <= vert->dim[it_dim].numAtributos){
            flag_para_gerar_vertice++;
        }
    }

    int i;
    if(flag_para_gerar_vertice > 1){
        for(it_dim=0; it_dim < qd; it_dim++){
            nv = cria_vertice(vert->i + (it_dim+1), "", vert->dim);
            for(i = 0; i < 10; i++)
                nv.nvl_de_cada_dimensao[i] = vert->nvl_de_cada_dimensao[i];
            nv.nvl_de_cada_dimensao[it_dim]++;
            gerarSigladoVertice(&nv, qd);

            v* end = insere_vertice(LV, &nv);

            ar = cria_aresta(vert, end);
            insere_aresta(vert, &ar);

        }
    } else {
        int m;
        for(m = 0; m < total_de_elementos; m++){
            printf("%s %s\n", ST[m], vert->sigla);
            if(strcmp(ST[m], vert->sigla) == 0){
                flag_isolados[m] = 1;
            }
        }
    }


    //return LV;
}

v* cria_arestas_das_isoladas(V *LV, int qd, char **ST, int total_de_elementos, dimensao *dimensoes){
    dimensao *D_aux;
    v *buscador_atual;
    v *buscador_prox;
    v *all = (v*) calloc (1, sizeof(v));
    *all = cria_vertice(1000, "ALL", dimensoes);
    a nova_aresta;

    insere_vertice(LV, all);
    int i;
    for(i = 0; i < 3; i++){
        D_aux = &(dimensoes[i]);
        //SE N�O EXISTEM ATRIBUTOS
        if(D_aux->numAtributos == 0){
            //APONTA VERTICE DA DIMENSAO ISOLADO PRA ALL
            buscador_atual = LV->inicio;
            while(strcmp(D_aux->sigla, buscador_atual->sigla) != 0){
                buscador_atual = buscador_atual->prox;
            }
            nova_aresta = cria_aresta(buscador_atual, all);
            insere_aresta(buscador_atual, &nova_aresta);
        }
        //SE EXISTEM ATRIBUTOS
        int NH;
        if(D_aux->numAtributos > 0){
            NH = 0;
            //ENQNT NH < NRO_ATRIBUTOS
            while(NH <= D_aux->numAtributos){
                buscador_atual = LV->inicio;
                buscador_prox = LV->inicio;
                //PROCURA NH ATUAL E PROX_NH
                //CRIA ARESTA ENTRE NH E PROX_NH
                if(NH == 0){
                    //printf("CASO 0: Nvl de hierarquia: %d\n", NH);

                    //printf("Estamos procurando a sigla %s.\n", D_aux->sigla);
                    while(strcmp(buscador_atual->sigla, D_aux->sigla) != 0){
                        buscador_atual = buscador_atual->prox;
                    }
                   // printf("Encontrou o vertice %s isolado.\n", buscador_atual->sigla);
                    buscador_prox = buscador_atual->prox;

                    //printf("Estamos procurando a sigla %s.\n", D_aux->atributos[NH].sigla);
                    while(strcmp(buscador_prox->sigla, D_aux->atributos[NH].sigla) != 0){
                        buscador_prox = buscador_prox->prox;
                    }
                    //printf("Encontrou o vertice maior na h %s isolado.\n", buscador_prox->sigla);
                    nova_aresta = cria_aresta(buscador_atual, buscador_prox);
                    insere_aresta(buscador_atual, &nova_aresta);

                    NH++;
                }
                if(NH > 0 && NH < D_aux->numAtributos){
                   // printf("CASO 1: Nvl de hierarquia: %d\n", NH);

                    //printf("Estamos procurando a sigla %s.\n", D_aux->atributos[NH-1].sigla);
                    while(strcmp(buscador_atual->sigla, D_aux->atributos[NH-1].sigla) != 0){
                        buscador_atual = buscador_atual->prox;
                    }
                    //printf("Encontrou o vertice %s isolado.\n", buscador_atual->sigla);
                    buscador_prox = buscador_atual->prox;
                    while(strcmp(buscador_prox->sigla, D_aux->atributos[NH].sigla) != 0){
                        buscador_prox = buscador_prox->prox;
                    }
                    //printf("Encontrou o vertice maior na h %s isolado.\n", buscador_prox->sigla);
                    nova_aresta = cria_aresta(buscador_atual, buscador_prox);
                    insere_aresta(buscador_atual, &nova_aresta);

                    NH++;
                }
                 //QUANDO NH == NRO_ATRIBUTOS
                if(NH == D_aux->numAtributos){
                    //printf("CASO 2: Nvl de hierarquia: %d\n", NH);
                    //APONTA VERTICE DA HRQUIA MAIS ALTO ISOLADO PRA ALL
                    //printf("Estamos procurando a sigla %s.\n", D_aux->atributos[NH-1].sigla);
                    while(strcmp(D_aux->atributos[NH-1].sigla, buscador_atual->sigla) != 0){
                        buscador_atual = buscador_atual->prox;
                    }
                    nova_aresta = cria_aresta(buscador_atual, all);
                    insere_aresta(buscador_atual, &nova_aresta);

                    NH++;
                }
            }
        }
    }
    return all;
}

v* encontra_duplicata(v *LV){
    v *aux_busca_travado;
    v *aux_busca_rolando;

    aux_busca_travado = LV;
    while(aux_busca_travado != NULL){
        aux_busca_rolando = aux_busca_travado->prox;
        while(aux_busca_rolando != NULL){
            //printf("Comparou %s com %s.\n", aux_busca_travado->sigla, aux_busca_rolando->sigla);
            if(strcmp(aux_busca_travado->sigla, aux_busca_rolando->sigla) == 0){
                return aux_busca_rolando;
            }
        }
    }
    return NULL;
}

/// FUN��O QUE REMOVE TODOS OS V�RTICE DUPLICADOS
/// SEM TRATAR ARESTAS
void remove_duplicata(v *LV){
    v *aux_busca_travado;
    v *aux_busca_rolando;
    v *aux_busca_anterior;

    aux_busca_travado = LV;
    while(aux_busca_travado != NULL){
        aux_busca_anterior = aux_busca_travado;
        aux_busca_rolando = aux_busca_travado->prox;
        while(aux_busca_rolando != NULL){
            //printf("Comparou %s com %s.\n", aux_busca_travado->sigla, aux_busca_rolando->sigla);
            if(strcmp(aux_busca_travado->sigla, aux_busca_rolando->sigla) == 0){
                //printf("Removeu.\n");
                aux_busca_anterior->prox = aux_busca_rolando->prox;
                aux_busca_rolando = aux_busca_rolando->prox;
            } else {
                aux_busca_anterior = aux_busca_anterior->prox;
                aux_busca_rolando = aux_busca_rolando->prox;
            }

        }
        aux_busca_travado = aux_busca_travado->prox;
    }
}

/*void permuta_dimensao(V *LV, v *vert, int qd, char **ST, int flag_isolados[], int total_de_elementos){
    v nv;
    a ar;

    int flag_para_gerar_vertice = 0;

    int it_dim;

    for(it_dim = 0; it_dim < qd; it_dim++){
        if(strlen(vert->sigla) > 1){
            flag_para_gerar_vertice++;
        }
    }

    int i;
    if(flag_para_gerar_vertice > 1){
        for(it_dim=0; it_dim < qd; it_dim++){
            nv = cria_vertice(vert->i + (it_dim+1), "", vert->dim);
            for(i = 0; i < 20; i++)
                nv.vetor_grafo_dimensoes[i] = vert->vetor_grafo_dimensoes[i];
            nv.nvl_de_cada_dimensao[it_dim]++;
            gerarSigladoVerticeD(&nv, qd);

            printf("Sigla gerada: %s\n", nv.sigla);

            v* end = insere_vertice(LV, &nv);
            ar = cria_aresta(vert, end);
            insere_aresta(vert, &ar);
        }
    } else {
        int m;
        for(m = 0; m < qd; m++){
            if(strcmp(vert->dim[m].sigla, vert->sigla) == 0){
                flag_isolados[m] = 1;
            }
        }
    }
}*/
/*
void gerar_grafo_de_dimensao(lista_de_dimensao *LD){
    //int total_de_elementos;// = LD->TOTAL

    //SINALIZADOR DE DIM/ATRIBUTO J� ISOLADO
    int *flag_isolados;
    flag_isolados = (int*) calloc (LD->tamanho, sizeof(int));
    //VETOR DE ISOLADOS
    int it;
    for(it = 0; it < LD->tamanho; it++){
        flag_isolados[it] = 0;
    }

    //CONJUNTO VAZIO � QUANDO UMA SIGLA � ISOLADA DE TODAS AS OUTRAS.
    //PRECISA SER PASSADO COMO REF PARA SER OPERADO POR FUNCOES EXTERNAS
    int flag_continuidade = 1;
    v vert = cria_vertice(0, "", LD->dimensoes);
    vert.i = 0;
    gerarSigladoVertice(&vert, LD->tamanho);

    //VARI�VEIS QUE GERAM O GRAFO
    v *aux_v = &vert;
    v *aux_final;
    V nova_lista;
    nova_lista.inicio = &vert;
    int cnt;


    while(flag_continuidade){

        //ENCONTRA FINAL "ATUAL" DA LISTA
        int i;
        char lds[LD->tamanho][3];

        for(i = 0; i < LD->tamanho; i++){
            strcpy(lds[i], LD->dimensoes[i].sigla);
        }


        //ENCONTRA FINAL "ATUAL" DA LISTA
        v *aux_final = &vert;
        while(aux_final->prox != NULL){
            aux_final = aux_final->prox;
        }
        v* aux_v7 = &vert;


        //gera "filhos" do v�rtice atual no final da lista
        printf("vt atual: %s%d\n", aux_v->sigla, aux_v->i);
        permuta_dimensao(&nova_lista, aux_v, LD->tamanho, lds, flag_isolados, LD->totalElementos);

        //passa para o pr�ximo vertice
        aux_v = aux_v->prox;

        //TESTA CONDI��O DE PARADA
        cnt = 0;

        for(it = 0; it < LD->tamanho; it++){
            printf("%d ", flag_isolados[it]);
            if(flag_isolados[it] == 1)
                cnt++;
        } printf("\n");
        if(cnt == LD->tamanho)
            flag_continuidade = 0;
    }
        //CRIA AS ARESTAS ENTRE ISOLADOS E ALL
        //cria_arestas_das_isoladas(&nova_lista, LD->tamanho, LD->siglas, LD->totalElementos, LD->dimensoes);

        //IMPRIME VERTICES DO GRAFO
        v* aux_v7 = &vert;
        while(aux_v7 != NULL){
            puts(aux_v7->sigla);
            aux_v7=aux_v7->prox;
        }
}
*/


void gerar_grafo_de_derivacao(lista_de_dimensao *LD){
    /*int qd = LD->tamanho;*/
    //int total_de_elementos;// = LD->TOTAL

    //SINALIZADOR DE DIM/ATRIBUTO J� ISOLADO
    int *flag_isolados;
    flag_isolados = (int*) calloc (LD->totalElementos, sizeof(int));

    //VETOR DE ISOLADOS
    int it;
    printf("%d\n", LD->totalElementos);
    for(it = 0; it < LD->totalElementos; it++){
        flag_isolados[it] = 0;
    }

    //CONJUNTO VAZIO � QUANDO UMA SIGLA � ISOLADA DE TODAS AS OUTRAS.
    //PRECISA SER PASSADO COMO REF PARA SER OPERADO POR FUNCOES EXTERNAS
    int flag_continuidade = 1;
    v vert = cria_vertice(0, "", LD->dimensoes);
    vert.i = 0;
    gerarSigladoVertice(&vert, LD->tamanho);



    //VARI�VEIS QUE GERAM O GRAFO
    v *aux_v = &vert;
    v *aux_final;
    V nova_lista;
    nova_lista.inicio = &vert;
    int cnt;

    while(flag_continuidade){
        printf("ok1\n");

        //ENCONTRA FINAL "ATUAL" DA LISTA
        aux_final = &vert;
        while(aux_final->prox != NULL){
            aux_final = aux_final->prox;
        }
        v* aux_v7 = &vert;

        //gera "filhos" do v�rtice atual no final da lista
        printf("gerando filhos do vert %s\n", aux_v->sigla);

        permuta_dim(&nova_lista, aux_v, LD->tamanho, LD->siglas, flag_isolados, LD->totalElementos);
        printf("gerou.\n");

        //passa para o pr�ximo vertice
        aux_v = aux_v->prox;

        //TESTA CONDI��O DE PARADA
        cnt = 0;
        for(it = 0; it < LD->totalElementos; it++){
            printf("%d ", flag_isolados[it]);
            if(flag_isolados[it] == 1)
                cnt++;
        }
        printf("\n");

        if(cnt == LD->totalElementos)
            flag_continuidade = 0;
    }
        //CRIA AS ARESTAS ENTRE ISOLADOS E ALL
        cria_arestas_das_isoladas(&nova_lista, LD->tamanho, &(LD->siglas), LD->totalElementos, LD->dimensoes);

        //IMPRIME VERTICES DO GRAFO
        v* aux_v7 = &vert;
        while(aux_v7 != NULL){
            puts(aux_v7->sigla);
            aux_v7=aux_v7->prox;
        }
        generateDot(&vert);
}
/*
void combina_dimensoes(V* L, v *vert, int qd){
    int cnt;
    int i, j;
    /*for(i = 0; i < qd; i++){
        if(vert->dimensoes_disponiveis[i] == 0) cnt++;
    }

    int qt_dim = qd - cnt;

    v *novo_vertice;
    char nova_sigla[100];

    if(qt_dim == 1) return;
    else{
        for(i = 0; i++; i < qd; i++){
            for(j = 0; j < qd; j++){
                if((i != j) && (dimensoes_disponiveis[j] != 0)){
                    strcat(nova_sigla, )
                }
            }
        }

    }

}
*/

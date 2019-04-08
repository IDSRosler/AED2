/* 
 * File:   main.c
 * Author: Igor de Souza Rosler
 *
 * Created on 3 de Abril de 2019, 10:50
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct grafo {
    int eh_ponderado; // Indica se o grafo é ou não ponderado
    int grau_max; // Indica o grau máximo que cada vértice pode possuir
    int num_vertices; // Quantidade total de vértices
    int** arestas; // Matriz de arestas
    float** pesos; // Matriz dos pesos das arestas 
    int* grau; // Grau contido em cada vértice 
} Grafo;

Grafo* cria_grafo(int num_vertices, int grau_max, int eh_ponderado);
void inserir_aresta(Grafo* gr, int orig, int dest, int eh_digrafo, float peso);
void liberar_grafo(Grafo* gr);
int procura_menor_distancia(float* dist, int* visitado, int nv);
void menor_caminho(Grafo* gr, int ini, int* ant, float* dist);

int main() {
    int num_vertices, grau_max, eh_ponderado, inserir, orig, dest, eh_digrafo;
    int v_origem, v_destino, *v_anterior;
    float peso = 0, *distancia;
    Grafo *gr;

    /*printf("Digite a quantidade de vertices do grafo:  ");
    scanf("%d", &num_vertices);
    printf("\nDigite a quantidade maxima de arestas que os vertices podem ter:  ");
    scanf("%d", &grau_max);
    printf("\nO grafo e ponderado?(1 - SIM  |  0 - NAO):  ");
    scanf("%d", &eh_ponderado);

    gr = cria_grafo(num_vertices, grau_max, eh_ponderado);

    printf("\nInserir aresta(1 - SIM  |  0 - NAO):  ");
    scanf("%d", &inserir);
    inserir = (inserir != 1) ? 0 : 1; // Qualquer escolha diferente de 1 será considerado como alternativa 'NÃO'
    do {
        if (inserir) {
            printf("\nInforme o vertice origem e o vertice de destino da aresta:  ");
            scanf("%d %d", &orig, &dest);
            printf("\nO grafo e digrafo?(1 - SIM  |  0 - NAO):  ");
            scanf("%d", &eh_digrafo);
            eh_digrafo = (eh_digrafo != 1) ? 0 : 1; // Qualquer escolha diferente de 1 será considerado como alternativa 'NÃO'
            if (eh_ponderado) {
                printf("\nInforme o peso da aresta: ");
                scanf("%f", &peso);
            }
            inserir_aresta(gr, orig, dest, eh_digrafo, peso);
            printf("\nInserir outra aresta(1 - SIM  |  0 - NAO):  ");
            scanf("%d", &inserir);
            inserir = (inserir != 1) ? 0 : 1; // Qualquer escolha diferente de 1 será considerado como alternativa 'NÃO'
        }
    } while (inserir);*/
    
    gr = cria_grafo(4,3,1);
    inserir_aresta(gr,0,1,1,3);
    inserir_aresta(gr,1,3,1,1);
    inserir_aresta(gr,1,2,1,2);
    inserir_aresta(gr,2,0,1,2);
    inserir_aresta(gr,2,3,1,5);
    
    printf("\nInforme o vertice origem e o vertice destino para o calculo do menor caminho: ");
    scanf("%d %d", &v_origem, &v_destino);

    v_anterior = (int *) malloc(gr->num_vertices * sizeof (int));
    distancia = (float *) malloc(gr->num_vertices * sizeof (float));

    menor_caminho(gr, v_origem, v_anterior, distancia);

    printf("\n\nA menor distancia do vertice[%d] ate o vertice[%d] e: %.2f", v_origem, v_destino, distancia[v_destino]);

    return 0;
}

Grafo* cria_grafo(int num_vertices, int grau_max, int eh_ponderado) {
    Grafo *gr = (Grafo *) malloc(sizeof (Grafo));
    if (gr != NULL) {
        int i;
        gr->num_vertices = num_vertices;
        gr->grau_max = grau_max;
        gr->eh_ponderado = (eh_ponderado != 0) ? 1 : 0; // Estrutura simplificada para if(eh_ponderado != 0) então retorna 1 caso contrario 0
        gr->grau = (int *) calloc(num_vertices, sizeof (int));
        gr->arestas = (int **) malloc(num_vertices * sizeof (int));
        for (i = 0; i < num_vertices; i++) {
            gr->arestas[i] = (int *) malloc(grau_max * sizeof (int));
        }
        if (eh_ponderado) {
            gr->pesos = (float **) malloc(num_vertices * sizeof (float));
            for (i = 0; i < num_vertices; i++) {
                gr->pesos[i] = (float *) malloc(grau_max * sizeof (float));
            }
        }
    }
    return gr;
}

void liberar_grafo(Grafo* gr) {
    if (gr == NULL) return;
    int i;
    for (i = 0; i < gr->num_vertices; i++) {
        free(gr->arestas[i]);
    }
    free(gr->arestas);
    if (gr->eh_ponderado) {
        for (i = 0; i < gr->num_vertices; i++) {
            free(gr->pesos[i]);
        }
        free(gr->pesos);
    }
    free(gr->grau);
    free(gr);
}

void inserir_aresta(Grafo* gr, int orig, int dest, int eh_digrafo, float peso) {
    if (gr == NULL) return;
    if (orig < 0 || orig >= gr->num_vertices) return;
    if (dest < 0 || dest >= gr->num_vertices) return;
    gr->arestas[orig][gr->grau[orig]] = dest;
    if (gr->eh_ponderado) {
        gr->pesos[orig][gr->grau[orig]] = peso;
    }
    gr->grau[orig]++;
    if (eh_digrafo == 0) { // 1 - Indica que é dígrafo (direcional) / 0 - Indica que não é digrafo (não direcinado) 
        inserir_aresta(gr, dest, orig, 1, peso);
    }
}

void menor_caminho(Grafo* gr, int ini, int* ant, float* dist) {
    int i, cont, nv, ind, *visitado, u;
    cont = nv = gr->num_vertices;
    visitado = (int*) malloc(nv * sizeof (int));
    for (i = 0; i < nv; i++) { // Inicialização
        ant[i] = -1;
        dist[i] = -1;
        visitado = 0;
    }
    dist[ini] = 0;
    while (cont > 0) {
        u = procura_menor_distancia(dist, visitado, nv);
        if (u == -1)
            break;
        visitado[u] = 1;
        cont--;
        for (i = 0; i < gr->grau[u]; i++) {
            ind = gr->arestas[u][i];
            if (dist[ind] < 0) {
                dist[ind] = dist[u] + gr->pesos[u][i];
                ant[ind] = u;
            } else {
                if (dist[ind] > dist[u] + gr->pesos[u][i]) {
                    dist[ind] = dist[u] + gr->pesos[u][i];
                    dist[ind] = u;
                }
            }
        }
    }
    free(visitado);
}

int procura_menor_distancia(float* dist, int* visitado, int nv) {
    int i, menor = -1, primeiro = 1;
    for (i = 0; i < nv; i++) {
        if (dist[i] >= 0 && visitado[i] == 0) {
            if (primeiro) {
                menor = i;
                primeiro = 0;
            } else {
                if (dist[menor] > dist[i])
                    menor = i;
            }
        }
    }
    return menor;
}

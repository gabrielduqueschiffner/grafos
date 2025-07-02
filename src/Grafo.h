//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include "types.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

// Supondo enum para cores:
enum Cor
{
    BRANCO = 0,
    CINZA = 1,
    PRETO = 2
};

class Grafo
{
public:
    // constructor e destrutor
    Grafo();
    ~Grafo();

    vector<bool> visitado;

    // Getters e Setters
    void set_direcionado(int direcao);
    void set_ponderado_aresta(int ponderado);
    void set_ponderado_vertice(int ponderado);
    void set_ordem(int ordem);

    int get_direcionado();
    int get_ponderado_aresta();
    int get_ponderado_vertice();
    int get_ordem();
    vector<No *> get_lista_adj();

    No *encontra_no_por_id(NoId id) const; // Busca linear por ID
    

    void adiciona_no(NoId id_no, int peso = 0);
    void adiciona_aresta(NoId id_no_origem, NoId id_no_alvo, int peso);

    // Funções de impressão no console
    void imprime_grafo();
    void imprime_vector(vector<NoId> v);

    // Funções de exportação para arquivo
    void exportar_grafo_para_arquivo(Grafo *g, const string &nome_arquivo);
    void exportar_grafo_para_arquivo_csacademy(Grafo *g, const string &nome_arquivo);
    void exportar_vector_para_arquivo(const vector<NoId> &v, const string &nome_arquivo);

    // Funções principais do trabalho
    vector<NoId> fecho_transitivo_direto(NoId id_no);                 // a
    vector<NoId> fecho_transitivo_indireto(NoId id_no);               // b
    vector<NoId> caminho_minimo_dijkstra(NoId id_no_a, NoId id_no_b); // c
    vector<NoId> caminho_minimo_floyd(NoId id_no, NoId id_no_b);      // d
    Grafo *arvore_geradora_minima_prim(vector<NoId> ids_nos);         // e
    Grafo *arvore_geradora_minima_kruskal(vector<NoId> ids_nos);      // f
    Grafo *arvore_caminhamento_profundidade(NoId id_no);              // g
    int raio();                                                       // h 1
    int diametro();                                                   // h 2
    vector<NoId> centro();                                            // h 3
    vector<NoId> periferia();                                         // h 4
    vector<NoId> vertices_de_articulacao();                           // i

    

private:
    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No *> lista_adj;

    // Auxiliares
    void dfs_arvore_aux(int indice_no,
                        vector<bool> &visitado,
                        const unordered_map<NoId, int> &mapa_id_para_indice, vector<Aresta *> &tree_edges);

    void dfs_fecho_transitivo_direto(int indice_no,
                                     vector<NoId> &marcado,
                                     vector<NoId> &resultado,
                                     const unordered_map<NoId, int> &mapa_id_para_indice);
};

#endif // GRAFO_H

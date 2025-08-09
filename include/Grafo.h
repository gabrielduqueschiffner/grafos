//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include "types.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <numeric>

using namespace std;


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

    int get_direcionado();
    int get_ponderado_aresta();
    int get_ponderado_vertice();
    int get_ordem();
    vector<No *> get_lista_adj();

    unordered_map<int, NoId> get_mapa_index_id();
    unordered_map<NoId, int> get_mapa_id_index();

    No *encontra_no_por_id(NoId id); // Busca linear por ID
    
    void adiciona_no(No* no);
    void adiciona_aresta(NoId id_no_origem, NoId id_no_alvo, int peso=0);

    // Funções de impressão no console
    void imprime_grafo();
    void imprime_resumo_grafo();

    // Funções de exportação para arquivo
    void exportar_grafo_para_arquivo(Grafo *g, const string &nome_arquivo);
    void exportar_grafo_para_arquivo_csacademy(Grafo *g, const string &nome_arquivo);
    void exportar_vector_para_arquivo(const vector<NoId> &v, const string &nome_arquivo);
    void exportar_metricas_para_arquivo(Grafo *g, const string &nome_arquivo);

    // Guloso
    // FIXME: Ver se é necessário, acho q ta destaulizado
   // Grafo* guloso_random_cda(float alfa);
   // Grafo* guloso_cda();
    Grafo* clone();

    // Funções principais do trabalho
    vector<NoId> fecho_transitivo_direto(NoId id_no);                 // a
    vector<NoId> fecho_transitivo_indireto(NoId id_no);               // b
    vector<NoId> caminho_minimo_dijkstra(NoId id_no_a, NoId id_no_b); // c
    vector<NoId> caminho_minimo_floyd(NoId id_no, NoId id_no_b);      // d
    Grafo* arvore_geradora_minima_prim(vector<NoId> ids_nos);         // e
    Grafo* arvore_geradora_minima_kruskal(vector<NoId> ids_nos);      // f
    Grafo* arvore_caminhamento_profundidade(NoId id_no);              // g
    int get_raio();                                                   // h 1
    int get_diametro();                                               // h 2
    vector<NoId> get_centro();                                        // h 3
    vector<NoId> get_periferia();                                     // h 4

    /// DEBUG
    void imprime_matriz(vector<vector<int>> mat) {

        for (int i = 0; i < static_cast<int>(mat.size()); ++i) {
            for (int j = 0; j < static_cast<int>(mat[i].size()); ++j) {

                if (mat[i][j] == numeric_limits<int>::max())
                    cout << setw(3) << left << "-";
                else
                    cout << setw(3) << left << mat[i][j];
            }
            cout << endl;
        }
    }

private:
    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No *> lista_adj;

    // Auxiliares
    void dfs_arvore_aux(int indice_no,
                        vector<bool> &visitado,
                        const unordered_map<NoId, int> &mapa_id_para_indice, vector<Aresta *> &arestas_arvore);

    void dfs_fecho_transitivo_direto(int indice_no,
                                     vector<bool> &marcado,
                                     vector<NoId> &resultado,
                                     const unordered_map<NoId, int> &mapa_id_para_indice);


    void calcular_matrizes_floyd(vector<vector<int>>& dist, vector<vector<int>>& prox);
    vector<int> get_excentricidades();
    Grafo* gera_grafo_transposto();
};

#endif // GRAFO_H

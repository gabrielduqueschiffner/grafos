//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <iostream>
#include <vector>

using namespace std;
class Grafo
{
public:
    Grafo();
    ~Grafo();

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

    void adiciona_no(char id_no, int peso = 0);
    void imprime_grafo();

    vector<char> fecho_transitivo_direto(char id_no);                 // a
    vector<char> fecho_transitivo_indireto(char id_no);               // b
    vector<char> caminho_minimo_dijkstra(char id_no_a, char id_no_b); // c
    vector<char> caminho_minimo_floyd(char id_no, char id_no_b);      // d
    Grafo *arvore_geradora_minima_prim(vector<char> ids_nos);         // e
    Grafo *arvore_geradora_minima_kruskal(vector<char> ids_nos);      // f
    Grafo *arvore_caminhamento_profundidade(char id_no);              // g
    int raio();                                                       // h 1
    int diametro();                                                   // h 2
    vector<char> centro();                                            // h 3
    vector<char> periferia();                                         // h 4
    vector<char> vertices_de_articulacao();                           // i

private:
    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No *> lista_adj;
};

#endif // GRAFO_H

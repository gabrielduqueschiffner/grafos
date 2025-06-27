#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include "Grafo.h"
#include "types.h"
#include <algorithm>
#include <iostream>

using namespace std;
class Gerenciador {
  public:
    static void iniciar_menu(Grafo *grafo);
    static NoId get_id_entrada();
    // Obtém a partir da entrada, um subconjunto de ids para um grafo
    static vector<NoId> get_subconjunto_ids(Grafo *grafo);
    static bool perguntar_escrever_arquivo(string nome_arquivo);

    static void fecho_transitivo(Grafo *grafo);

    static void processa_saida_caminho(vector<NoId> sequencia, string nome_arquivo="caminho");
    static void processa_saida_grafo(Grafo *grafo, string nome_arquivo="grafo");
};

#endif // GERENCIADOR_H

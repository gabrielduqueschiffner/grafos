#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include "Grafo.h"
#include "types.h"
#include <algorithm>
#include <iostream>

using namespace std;
class Gerenciador {
public:
    static void comandos(Grafo* grafo);
    static NoId get_id_entrada();
    static vector<NoId> get_conjunto_ids(Grafo* grafo, int tam);
    static bool pergunta_imprimir_arquivo(string nome_arquivo);
};


#endif //GERENCIADOR_H

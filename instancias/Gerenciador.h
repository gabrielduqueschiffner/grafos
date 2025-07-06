#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <algorithm>
#include <iostream>
#include "../include/Grafo.h"
#include "../include/types.h"

using namespace std;
class Gerenciador {
public:
    Gerenciador(Grafo* grafo);
    void executar_menu();
    NoId get_id_entrada(int i=-1);
    vector<NoId> get_conjunto_ids(int tam);
    bool pergunta_imprimir_arquivo(string nome_arquivo);
    bool no_existe(NoId id_no);

private:
    Grafo* grafo;
    vector<NoId> ids_nos;
};


#endif //GERENCIADOR_H

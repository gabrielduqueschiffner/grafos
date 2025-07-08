#ifndef INTERFACECONSOLE_H
#define INTERFACECONSOLE_H

#include "Grafo.h"
#include "types.h"
#include <algorithm>
#include <iostream>

using namespace std;
class InterfaceConsole {
public:
    InterfaceConsole(Grafo* grafo);
    void executar_menu();
    NoId get_id_entrada(int i=-1);
    vector<NoId> get_conjunto_ids(int tam);
    vector<NoId> get_conjunto_ids();
    bool pergunta_imprimir_arquivo(string nome_arquivo);
    bool no_existe(NoId id_no);
    void imprime_vector(vector<NoId> v);
    
private:
    Grafo* grafo;
    vector<NoId> ids_nos;
};


#endif //INTERFACECONSOLE_H

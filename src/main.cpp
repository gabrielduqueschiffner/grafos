#include "Gerenciador.h"
#include "LeitorGrafo.h"
#include <iostream>


using namespace std;
int main(int argc, char *argv[]) {

    if (argc != 2) {
        cerr << "Uso correto do comando: " << argv[0]
             << " <arquivo_de_entrada>\n"
             << endl;
        return 1;
    }

    string arquivo = argv[1];
    Grafo *grafo = LeitorGrafo::lerGrafo(arquivo);

    cout << endl << "=> Grafo lido: " << endl;
    grafo->imprime_grafo();
    
    Gerenciador::iniciar_menu(grafo);

    
    // // FIXME: teste, apagar depois
    // cout << "Direcionado: " << (grafo->get_direcionado() ? "sim" : "nao")
    //      << endl;
    // cout << "Ponderado Aresta: "
    //      << (grafo->get_ponderado_aresta() ? "sim" : "nao") << endl;
    // cout << "Ponderado Vertice: "
    //      << (grafo->get_ponderado_vertice() ? "sim" : "nao") << endl;
    // cout << "Ordem: " << grafo->get_ordem() << endl;


    return 0;
}
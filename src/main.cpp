#include <cstdlib>
#include <iostream>
#include <stdexcept>
// #include "../include/InterfaceConsole.h"
#include "../include/LeitorGrafo.h"
#include "../include/types.h"

using namespace std;
int main(int argc, char *argv[])
{
    if (argc != 2)
        throw new invalid_argument("Passar arquivo a ser lido como argumento.");
    
    Grafo* grafo = LeitorGrafo::lerDeArquivo(argv[1]);

    for (No* no : grafo->get_lista_adj()) {
        for (Aresta* aresta : no->get_arestas()) {
            bool domina = rand() / (float)RAND_MAX > 0.5;
            aresta->set_domina(domina);
        }
    }

    grafo->imprime_resumo_grafo();
    cout << endl;
    grafo->imprime_grafo();


    // InterfaceConsole* gerenciador = new InterfaceConsole(grafo);
    // gerenciador->executar_menu();
    
    // TESTES
    // grafo->conjunto_dominante_arestas()->imprime_grafo();

    delete grafo;
    // delete gerenciador;

    return 0;
}
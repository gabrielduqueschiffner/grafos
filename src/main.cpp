#include <iostream>
#include <stdexcept>
#include "../include/InterfaceConsole.h"
#include "../include/LeitorGrafo.h"
#include "../include/types.h"

using namespace std;
int main(int argc, char *argv[])
{
    if (argc != 2)
        throw new invalid_argument("Passar arquivo a ser lido como argumento.");
    
    Grafo* grafo = LeitorGrafo::lerDeArquivo(argv[1]);

    grafo->imprime_resumo_grafo();
    cout << endl;
    grafo->imprime_grafo();

    InterfaceConsole* gerenciador = new InterfaceConsole(grafo);
    gerenciador->executar_menu();
    
    // // TESTES
    // cout << endl;
    // grafo->arvore_geradora_minima_prim({'d', 'b', 'e'})->imprime_grafo();
    // cout << endl;

    delete grafo;
    delete gerenciador;

    return 0;
}
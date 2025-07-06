#include <iostream>
#include <stdexcept>
#include <string>
#include "LeitorGrafo.h"
#include "types.h"

using namespace std;
int main(int argc, char *argv[])
{
    if (argc != 2)
        throw new invalid_argument("Passar arquivo a ser lido como argumento.");
    
    Grafo* grafo = LeitorGrafo::lerDeArquivo(argv[1]);

    grafo->imprime_resumo_grafo();
    cout << endl;
    grafo->imprime_grafo();

    //Gerenciador::comandos(grafo);
    
    // TESTES

    for (NoId id : grafo->get_centro())
        cout << id << " ";

    cout<< endl;

    for (NoId id : grafo->get_periferia())
        cout << id << " ";

    cout << endl;

    return 0;
}
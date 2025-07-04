#include <iostream>
#include <stdexcept>
#include <string>
#include "Gerenciador.h"
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

    Gerenciador::comandos(grafo);
    
    // TESTES

    // vector<NoId> caminho = grafo->caminho_minimo_floyd('a', 'c');
    // for (NoId id : caminho)
    //     cout << id << endl;

    return 0;
}
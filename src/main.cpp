#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "../include/LeitorGrafo.h"
#include "../include/types.h"
#include "../include/Guloso.h"

using namespace std;
int main(int argc, char *argv[])
{
    if (argc != 2)
        throw invalid_argument("Passar arquivo a ser lido como argumento.");
    
    Grafo* grafo = LeitorGrafo::lerDeArquivo(argv[1]);

    grafo->imprime_resumo_grafo();
    cout << endl;
    grafo->imprime_grafo();

    // cout << "=========================" << endl;
    
    // Guloso guloso(0.5);

    // Grafo* grafo_dominante = guloso.conjunto_dominante_arestas(grafo);
    // grafo_dominante->imprime_grafo();

    // delete grafo_dominante;
    delete grafo;

    // InterfaceConsole* gerenciador = new InterfaceConsole(grafo);
    // gerenciador->executar_menu();
    
    // TESTES    FIXME: Ta dando problema, o randomizado esta com a solução fixa, dependendo do alfa até muda a solucao se comparado com o alfa 0, por exemplo o alfa 0.4 e 0.5, mas a cada rodagem do programa n muda a resposta, teste no g_10_0.30_0_1_1.txt

    // cout << endl << "Conjunto Dominante de Arestas:" << endl;
    // grafo->conjunto_dominante_arestas(0.5)->imprime_grafo();

    // delete gerenciador;

    return 0;
}
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
   

    // InterfaceConsole* gerenciador = new InterfaceConsole(grafo);
    // gerenciador->executar_menu();
    
    // TESTES    FIXME: Ta dando problema, o randomizado esta com a solução fixa, dependendo do alfa até muda a solucao se comparado com o alfa 0, por exemplo o alfa 0.4 e 0.5, mas a cada rodagem do programa n muda a resposta, teste no g_10_0.30_0_1_1.txt

    // cout << endl << "Conjunto Dominante de Arestas:" << endl;
    //grafo->conjunto_dominante_arestas(0.5)->imprime_grafo();
    //grafo->rodar_reativo(0.5, 10, 5, 1)->imprime_grafo();
    // delete gerenciador;

// teste Estático (um único α):
    {
        float alpha = 0.5f;
        std::vector<float> alfas = { alpha };    // só um valor
        int maxIter = 1;                         // vai rodar UMA vez
        int kBloco  = 1;                         // não importa
        int seed    = 42;
        
        Guloso guloso(grafo, alfas, maxIter, kBloco, seed);
        Grafo* solEstatica = guloso.conjunto_dominante_arestas(alpha);
        
        if(alpha == 0){
        cout << "\n=== GULOSO PADRAO (alpha=0) ===\n";
        }
        else{
        cout << "\n=== GULOSO RANDOM COM (alpha=" << alpha << ") ===\n";
        }
        solEstatica->imprime_grafo();
        delete solEstatica;
    }

    // Teste Reativo(múltiplos α):
    {
        vector<float> alfas = { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f };
        int maxIter = 20;      // rodadas totais
        int kBloco  = 5;       // repensa probabilidades a cada 5 iters
        int seed    = 42;
        
        Guloso reativo(grafo, alfas, maxIter, kBloco, seed);
        Grafo* solReativa = reativo.rodar_reativo();
        
        cout << "\n=== GULOSO REATIVO ===\n";
        solReativa->imprime_grafo();
        delete solReativa;
    }
     

    delete grafo;
    return 0;








   
}
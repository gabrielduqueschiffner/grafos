#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "../include/LeitorGrafo.h"
#include "../include/types.h"
#include "../include/Guloso.h"

using namespace std;
int main(int argc, char *argv[])
{
    // vector<string> arquivos = {

    //     "g_25_0.16_0_1_0.txt",
    //     "g_25_0.16_0_1_1.txt",
    //     "g_25_0.21_0_1_0.txt",
    //     "g_25_0.21_0_1_1.txt",
    //     "g_25_0.26_0_1_0.txt",
    //     "g_25_0.26_0_1_1.txt",
    //     "g_40_0.10_0_1_0.txt",
    //     "g_40_0.10_0_1_1.txt",
    //     "g_40_0.15_0_1_0.txt",
    //     "g_40_0.15_0_1_1.txt",
    //     "g_40_0.20_0_1_0.txt",
    //     "g_40_0.20_0_1_1.txt",
    //     "g_60_0.07_0_1_0.txt",
    //     "g_60_0.07_0_1_1.txt",
    //     "g_60_0.12_0_1_0.txt",
    //     "g_60_0.12_0_1_1.txt",
    //     "g_60_0.17_0_1_0.txt",
    //     "g_60_0.17_0_1_1.txt",
    // };

    // for (string arquivo : arquivos) 
    {

        // string arquivo = "g_25_0.16_0_1_0.txt";
        
        // string caminho = "instancias/t2/" + arquivo;
        string caminho = argv[1];

        if (argc != 2)
            throw invalid_argument("Passar arquivo a ser lido como argumento.");
    
        Grafo* grafo = LeitorGrafo::lerDeArquivo(caminho);
    
        cout << caminho << endl;
        // grafo->imprime_resumo_grafo();
        // grafo->imprime_grafo();
        
        {
            // float alpha = 0.5f;
            // int seed = -1;
            
            // Guloso guloso(grafo, seed);
            
            // // FIXME: É estranho passar o alfa direto...?
            // Grafo* solEstatica = guloso.conjunto_dominante_arestas(alpha);
            
            // solEstatica->imprime_grafo();
            // cout << "Qualidade: " << guloso.qualidade_da_solucao(solEstatica);

            // if (alpha == 0)
            //     cout << "==> GULOSO PADRAO (alpha=0): " << guloso.qualidade_da_solucao(solEstatica) << endl;
            // else
            //     cout << "==> GULOSO RANDOM (alpha=" << alpha << "): " << guloso.qualidade_da_solucao(solEstatica) << endl;
            
            // solEstatica->imprime_grafo();
            // delete solEstatica;
        }

        {
            vector<float> alfas = { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f };
            int maxIter = 100;      // rodadas totais
            int kBloco  = 5;       // repensa probabilidades a cada 5 iters
            int seed    = 42;
            
            Guloso reativo(grafo, alfas, maxIter, kBloco, seed);
            
            Grafo* solReativa = reativo.rodar_reativo();
            
            if (!solReativa)
                throw runtime_error("Não foi gerada solução");

            // cout << "\n==> GULOSO REATIVO: " << reativo.qualidade_da_solucao(solReativa) << endl;
            // solReativa->imprime_grafo();
            delete solReativa;
        }

        delete grafo;
    }

    return 0;
}
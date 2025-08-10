#include <cstdlib>
#include <stdexcept>
#include <vector>

#include "../include/LeitorGrafo.h"
#include "../include/types.h"
#include "../include/Guloso.h"

using namespace std;
int main(int argc, char *argv[])
{
    vector<string> arquivos = {
        "g_25_0.16_0_1_0.txt",
        "g_25_0.16_0_1_1.txt",
        "g_25_0.21_0_1_0.txt",
        "g_25_0.21_0_1_1.txt",
        "g_25_0.26_0_1_0.txt",
        "g_25_0.26_0_1_1.txt",
        "g_40_0.10_0_1_0.txt",
        "g_40_0.10_0_1_1.txt",
        "g_40_0.15_0_1_0.txt",
        "g_40_0.15_0_1_1.txt",
        "g_40_0.20_0_1_0.txt",
        "g_40_0.20_0_1_1.txt",
        "g_60_0.07_0_1_0.txt",
        "g_60_0.07_0_1_1.txt",
        "g_60_0.12_0_1_0.txt",
        "g_60_0.12_0_1_1.txt",
        "g_60_0.17_0_1_0.txt",
        "g_60_0.17_0_1_1.txt",
    };

    for (string arquivo : { argv[1] }) // arquivos)
    {
        string caminho = arquivo; //"instancias/t2/" + arquivo;

        if (argc != 2)
            throw invalid_argument("Passar arquivo a ser lido como argumento.");
    
        Grafo* grafo = LeitorGrafo::lerDeArquivo(caminho);
    
        // cout << caminho << endl;
        // grafo->imprime_resumo_grafo();
        // grafo->imprime_grafo();
        
        if (false)
        {
            float alpha = 0.5f;
            int seed = -1;
            
            Guloso guloso(grafo, seed);
            
            // FIXME: É estranho passar o alfa direto...?
            Grafo* solEstatica = guloso.conjunto_dominante_arestas(alpha);
            
            solEstatica->imprime_grafo();
            cout << "Qualidade: " << guloso.custo_da_solucao(solEstatica);// pq o custo aqui e la embaixo? esse ta estranho

            if (alpha == 0)
                cout << "==> GULOSO PADRAO (alpha=0): ";
            else
                cout << "==> GULOSO RANDOM (alpha=" << alpha << "): "; 
            
            cout << guloso.custo_da_solucao(solEstatica) << endl;

            solEstatica->imprime_grafo();
            delete solEstatica;
        }

        if (true)
        {
            vector<float> alfas = {0.25f, 0.5f, 0.75f}; //Professora pediu 3 alfas
            int max_iter = 300;      // rodadas totais   (no minimo 300)
            int k_bloco  = 30;       // repensa probabilidades a cada x iters (de 30 a 50)
            int seed    = 42;
            
            Guloso reativo(grafo, alfas, max_iter, k_bloco, seed);
            
            // A posse da solução é do Guloso
            Grafo* solReativa = reativo.rodar_reativo();
            
            if (!solReativa)
                throw runtime_error("Não foi gerada solução");

            cout << "\n==> GULOSO REATIVO: "; 
            cout << reativo.custo_da_solucao(solReativa) << endl;
            
             solReativa->imprime_grafo();
        }

        delete grafo;
    }

    return 0;
}
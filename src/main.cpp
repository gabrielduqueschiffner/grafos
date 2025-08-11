#include <cmath>
#include <cstdlib>
#include <ostream>
#include <stdexcept>
#include <vector>

#include "../include/LeitorGrafo.h"
#include "../include/types.h"
#include "../include/Guloso.h"
#include "../include/Global.h"

const int INFINITO = numeric_limits<int>::max();

// Constantes de execução
const int SEED = -1;

// Constantes de arquivo
const string PREFIXO = "instancias/t2/";
const string SUFIXO = ".txt";
const vector<string> INSTANCIAS = {
    "g_25_0.16_0_1_0",
    "g_25_0.16_0_1_1",
    "g_25_0.21_0_1_0",
    "g_25_0.21_0_1_1",
    "g_25_0.26_0_1_0",
    "g_25_0.26_0_1_1",
    "g_40_0.10_0_1_0",
    "g_40_0.10_0_1_1",
    "g_40_0.15_0_1_0",
    "g_40_0.15_0_1_1",
    "g_40_0.20_0_1_0",
    "g_40_0.20_0_1_1",
    "g_60_0.07_0_1_0",
    "g_60_0.07_0_1_1",
    "g_60_0.12_0_1_0",
    "g_60_0.12_0_1_1",
    "g_60_0.17_0_1_0",
    "g_60_0.17_0_1_1",
};

// Parâmetros de repetição
const int REPETICOES_POR_INSTANCIA = 10;
const int REPETICOES_POR_ALFA = 30;
const int ITERACOES = 300;
const int ITERACOES_POR_BLOCO = 30;

// Critérios de aleatoriedade
const vector<float> ALFAS = {0.25f, 0.5f, 0.75f};

using namespace std;
int main(int argc, char *argv[]) {

    TimerGlobal.marcar("Início");
    
    for (string instancia : INSTANCIAS) {        
    
        cout << "INSTÂNCIA: " << instancia << endl;

        string arquivo = PREFIXO+instancia+SUFIXO;
        Grafo* grafo = LeitorGrafo::lerDeArquivo(arquivo);
        
        for (int i = 0; i < REPETICOES_POR_INSTANCIA; i++) {

            cout << "=> Repetição: " << i << endl; 

            vector<int> melhor_custo_alfas;
            melhor_custo_alfas.assign(ALFAS.size(), INFINITO);

            for (int indice_alfa = 0; indice_alfa < ALFAS.size(); indice_alfa++) {
                
                for (int j = 0; j < REPETICOES_POR_ALFA; j++) {

                    float alfa = ALFAS[indice_alfa];
                    Guloso guloso_random(grafo, SEED);
                    Grafo* solEstatica = guloso_random.conjunto_dominante_arestas(alfa);
                    
                    int custo = guloso_random.custo_da_solucao(solEstatica);
                    if (custo < melhor_custo_alfas[indice_alfa])
                        melhor_custo_alfas[indice_alfa] = custo;
                
                    delete solEstatica;
                }
            }   
            
            // Imprimir melhores custos random
            cout << "Random: ";
            for (int i=0; i<ALFAS.size(); i++)
                cout << ALFAS[i] << ": " << melhor_custo_alfas[i] << ", ";
            cout << endl;

            {
                Guloso guloso_reativo(grafo, ALFAS, ITERACOES, ITERACOES_POR_BLOCO, SEED);
                
                // A posse desse memória é do Guloso, cabe a ele liberar
                Grafo* solReativa = guloso_reativo.rodar_reativo();
                
                if (!solReativa)
                    throw runtime_error("Não foi gerada solução");

                cout << "Reativo: " << 
                guloso_reativo.custo_da_solucao(solReativa);
                cout << endl;
            }
        }

        delete grafo;

        TimerGlobal.marcar("Fim instância");
    }

    TimerGlobal.marcar("Fim global");

    TimerGlobal.imprimir_resultado();

    return 0;
}
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
const int ITERACOES = 500;
const int ITERACOES_POR_BLOCO = 50;
const float DELTA = 1.0f;

// Critérios de aleatoriedade
const vector<float> ALFAS = {
    0.2f,
    0.3f,
    0.4f,
};

template<typename T>
void printVec(vector<T> vec) {
    for (T value : vec) 
        cout << value << ", "; 
}

using namespace std;
int main(int argc, char *argv[]) {

    TimerGlobal.marcar("Início");

    printVec(ALFAS);
    cout << endl;

    for (string instancia : INSTANCIAS) {        

        string arquivo = PREFIXO+instancia+SUFIXO;
        Grafo* grafo = LeitorGrafo::lerDeArquivo(arquivo);
        
        // RANDOM
        for (int i = 0; i < REPETICOES_POR_INSTANCIA; i++) {

            Guloso guloso(grafo, SEED);
            Grafo* solEstatica = guloso.conjunto_dominante_arestas(0);

            cout << instancia << ", " << i << ", ";
            cout << "comum, " << guloso.custo_da_solucao(solEstatica);
            cout << endl;

            delete solEstatica;
        }

        TimerGlobal.marcar(instancia + ", comum");
    }
    
    for (string instancia : INSTANCIAS) {        

        string arquivo = PREFIXO+instancia+SUFIXO;
        Grafo* grafo = LeitorGrafo::lerDeArquivo(arquivo);
        
        // RANDOM
        for (int i = 0; i < REPETICOES_POR_INSTANCIA; i++) {

            vector<int> melhor_custo_alfas;
            melhor_custo_alfas.assign(ALFAS.size(), INFINITO);

            for (int indice_alfa = 0; indice_alfa < ALFAS.size(); indice_alfa++) {
                
                for (int j = 0; j < REPETICOES_POR_ALFA; j++) {

                    float alfa = ALFAS[indice_alfa];
                    Guloso guloso_random(grafo, SEED);
                    Grafo* solRandom = guloso_random.conjunto_dominante_arestas(alfa);
                    
                    int custo = guloso_random.custo_da_solucao(solRandom);
                    if (custo < melhor_custo_alfas[indice_alfa])
                        melhor_custo_alfas[indice_alfa] = custo;
                
                    delete solRandom;
                }
            }   
            
            // Imprimir melhores custos random
            cout << instancia << ", " << i << ", ";
            cout << "random, ";
            printVec(melhor_custo_alfas);
            cout << endl;
        }

        TimerGlobal.marcar(instancia + ", random");
    }

    for (string instancia : INSTANCIAS) { 

        string arquivo = PREFIXO+instancia+SUFIXO;
        Grafo* grafo = LeitorGrafo::lerDeArquivo(arquivo);

        // REATIVO
        for (int i = 0; i < REPETICOES_POR_INSTANCIA; i++) {
            
            Guloso guloso_reativo(grafo, ALFAS, ITERACOES, ITERACOES_POR_BLOCO, DELTA, SEED);
            
            // A posse desse memória é do Guloso, cabe a ele liberar
            Grafo* solReativa = guloso_reativo.rodar_reativo();
            
            if (!solReativa)
                throw runtime_error("Não foi gerada solução");

            // Imprimir melhores custos random
            cout << instancia << ", " << i << ", ";
            cout << "reativo, " << guloso_reativo.custo_da_solucao(solReativa);
            cout << endl;
        }
        
        TimerGlobal.marcar(instancia + ", reativo");

        delete grafo;
    }

    TimerGlobal.marcar("Fim global");

    TimerGlobal.imprimir_resultado();

    return 0;
}
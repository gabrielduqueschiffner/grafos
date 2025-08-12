
#include "Aleatorio.h"
#include "Grafo.h"
#include <vector>

class Guloso {

public:

    Guloso(Grafo* grafo, int seed=-1);
    Guloso(Grafo* grafo, vector<float> alfas, int max_iter, int k_bloco, float delta, int seed=-1);
    ~Guloso();
    Grafo* conjunto_dominante_arestas(float alfa);
    Grafo* rodar_reativo();

    int custo_da_solucao(Grafo* solucao);

    vector<float> get_custos_relativos() { return custo_relat_alfas; } 

private:

    Grafo* grafo;
    Aleatorio* aleatorio;
    
    vector<float> alfas;
    int max_iter;
    int k_bloco;
    int __ultima_iteracao_subst__ = -1;

    vector<float> probs_alfas; // probabilidade de cada alfa ser escolhido p_i
    vector<float> custo_alfas; // custo absoluta de cada alfa
    vector<float> custo_relat_alfas; // custo relativa de cada alfa

    vector<float> qtd_usos_alfas; // quantidade de soluções encontradas para cada alfa
    float delta;  // expoente das fórmulas qi = (F*/Mi)^δ , usado para elevar as diferenças


    float media_custo;
    Grafo* melhor_solucao;

    //metodos

    void atualizar_probs();
    int selecionar_alfa();
    
    bool atualizar_melhor_solucao(Grafo* solucao);
    void atualizar_custos(int indice_alfa, Grafo* solucao);
    
    // TODO: Depois, adicionar registro de detalhes das iterações? 

    int get_indice_eleito_comum(vector<Aresta*>& fora);
    int get_indice_eleito_random(float alfa, vector<Aresta*>& fora);
    int grau_total(Aresta* aresta);
    function<int(vector<Aresta*>)> get_heuristica(float alfa);
    
    int custo_da_solucao();
};
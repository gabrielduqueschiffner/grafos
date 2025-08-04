
#include "Aleatorio.h"
#include "Grafo.h"
#include <vector>

class Guloso {

public:

    Guloso(Grafo* grafo, int seed=-1);
    Guloso(Grafo* grafo, vector<float> alfas, int max_iter, int k_bloco, int seed=-1);
    Grafo* conjunto_dominante_arestas(float alfa);
    Grafo* rodar_reativo();

private:

    Grafo* grafo;
    Aleatorio* aleatorio;
    
    vector<float> alfas;
    int max_iter;
    int k_bloco;

    vector<float> probs_alfas;
    vector<float> qualid_alfas;
    vector<float> qualid_relat_alfas;

    float qualid_media;
    Grafo* melhor_solucao;

    // TODO: Depois, adicionar registro de detalhes das iterações? 

    int get_indice_eleito(vector<Aresta*> dentro, vector<Aresta*> fora);
    int get_indice_eleito_random(float alfa, vector<Aresta*> dentro, vector<Aresta*> fora);
    int grau_total(Aresta* aresta);
    function<int(vector<Aresta*>, vector<Aresta*>)> get_heuritica(float alfa);
    int qualidade_da_solucao();
};
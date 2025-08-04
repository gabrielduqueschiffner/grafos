
#include <random>
#include <cstdlib>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "../include/Guloso.h"
#include "../include/Grafo.h"
#include "../include/Aresta.h"
#include "../include/No.h"
#include "../include/types.h"

using namespace std;

Guloso::Guloso(Grafo* grafo, int seed) {
    
    // Só faz aletório, seed definida

    this->grafo = grafo;

    if (seed == -1) {

        random_device rd;
        this->aleatorio = new Aleatorio(rd());
    
    } else {

        this->aleatorio = new Aleatorio(seed);
    }

    this->alfas = {};
    this->max_iter = -1;
    this->k_bloco = -1;
}

Guloso::Guloso(Grafo* grafo, vector<float> alfas, int max_iter, int k_bloco, int seed) {

    this->grafo = grafo;
    this->alfas = alfas;
    this->max_iter = max_iter;
    this->k_bloco = k_bloco;
    
    if (seed == -1) {
        
        random_device rd;
        this->aleatorio = new Aleatorio(rd());
        
    } else {
        
        this->aleatorio = new Aleatorio(seed);
    }
}

Grafo* Guloso::rodar_reativo() {

    for (int i=0; i<max_iter; i++) {

        if (i % k_bloco) 
            atualizar_probs();

        int index_alfa = selecionar_alfa();
		float alfa = alfas[index_alfa];

        Grafo* solucao = conjunto_dominante_arestas(alfa);
        
        atualizar_melhor_solucao(solucao);
        atualizar_qualidades();
    }

    return nullptr;
}

Grafo* Guloso::conjunto_dominante_arestas(float alfa) {

    /*
    Algoritmo guloso random para encontrar um conjunto dominante de arestas.
    Ordena todas as arestas não consideradas ainda, e a partir dela, escolhe uma
    entre as primeiras (sendo as N primeiras, a amostra de candidatos separados,
    bastando sortear um valor de 0 a N para obter encontrar o candidato escolhido.
    */
    
    if (grafo->get_direcionado()) {
        throw std::runtime_error(
            "Conjunto dominante de arestas não se aplica para grafo não direcionado."
        );
    }
    
    if(alfa < 0 || alfa > 1) 
        throw runtime_error("Erro: alfa deve estar entre 0 e 1.");

    Grafo* grafo_marcado = grafo->clone();
    auto mapa_id_index = grafo_marcado->get_mapa_id_index();

    // Copiar arestas sem repetição
    
    vector<Aresta*> fora = {};
    
    for(No* no : grafo_marcado->get_lista_adj()) {

        int index_origem = mapa_id_index[no->get_id()];
        for(Aresta* aresta : no->get_arestas())
            if (index_origem <= mapa_id_index[aresta->get_id_no_alvo()])
                fora.push_back(aresta);
    }

    // Arestas fora e dentro do conjunto dominante

    vector<Aresta*> dentro = {};
    dentro.reserve(fora.size());

    // Decidindo heurística e armazenando em variável 

    auto heuristica = get_heuritica(alfa);
    
    // Enquanto não houver arestas a serem avaliadas, selecionar arestas

    while (fora.size() > 0) {

        int indice_eleito = heuristica(dentro, fora);
        
        Aresta* aresta_eleita = fora[indice_eleito];
        
        // Marcar arestas dos dois sentidos
        
        aresta_eleita->set_domina(true);
        No* no_alvo = grafo_marcado->encontra_no_por_id(aresta_eleita->get_id_no_alvo());
        
        for (Aresta* aresta_alvo : no_alvo->get_arestas())
            if (aresta_alvo->get_id_no_alvo() == aresta_eleita->get_id_no_origem())
                aresta_alvo->set_domina(true);
    
        // Remover do conjunto "fora" as arestas adjascentes à aresta selecionada
        
        int eleito_origem = aresta_eleita->get_id_no_origem();
        int eleito_alvo = aresta_eleita->get_id_no_alvo();
        
        // Mover aresta de acordo

        dentro.push_back(aresta_eleita);
        
        vector<Aresta*> arestas_restantes;
        
        for (Aresta* aresta : fora) {

            int origem = aresta->get_id_no_origem();
            int alvo = aresta->get_id_no_alvo();

            if (
                eleito_origem != origem 
                && eleito_origem != alvo
                && eleito_alvo != origem  
                && eleito_alvo != alvo
            ) {
                arestas_restantes.push_back(aresta);
            }
        }

        fora = arestas_restantes;
    }

    // TODO: Esse formato está de acordo com o desejado no trabalho?

    return grafo_marcado;
}

int Guloso::get_indice_eleito(vector<Aresta*> dentro, vector<Aresta*> fora) {

    // Se for guloso normal, pegar o melhor 
    // (escolha determinística: aresta com maior grau total)
    
    auto it = max_element(
        fora.begin(), fora.end(), 
        [this](Aresta* a, Aresta* b) {
            return grau_total(a) < grau_total(b);
        }
    );

    return distance(fora.begin(), it);
}

int Guloso::get_indice_eleito_random(float alfa, vector<Aresta*> dentro, vector<Aresta*> fora) {

    // Se for guloso aleatório: ordenar, calcular tamanho da amostra e escolher
        
    // Ordenar arestas por maior grau total
    // FIXME: só considerar se tiver ligado a arestas dentro???
    sort(fora.begin(), fora.end(), [this](Aresta* a, Aresta* b) { 
        return grau_total(a) > grau_total(b);
    });

    // Calcular o pior valor aceito na seleção de candidatos: 
    
    int melhor = grau_total(fora.front());
    int pior = grau_total(fora.back());
    float pior_aceito = melhor - alfa * (melhor - pior);

    // Encontrar o tamanho da amostra (contar até o pior candidato aceito)
    
    int tamanho_amostra = 0;
    for (Aresta* aresta : fora) {
        if (grau_total(aresta) >= pior_aceito)
            tamanho_amostra++;
        else
            break;
    }
    
    // Dentre os candidatos, escolher um aleatoriamente

    return aleatorio->gerar_inteiro(tamanho_amostra);
}

int Guloso::grau_total(Aresta* aresta) {

    // Calcula o grau total de uma aresta (soma do grau do de origem com alvo)
    return grafo->encontra_no_por_id(aresta->get_id_no_alvo())->get_arestas().size() 
    + grafo->encontra_no_por_id(aresta->get_id_no_origem())->get_arestas().size();
    
}

function<int(vector<Aresta*>, vector<Aresta*>)> Guloso::get_heuritica(float alfa) {

    /* Retorna um lambda com a heurística a ser empregada, baseado no valor de 
    alfa */

    if (alfa == 0) {

        return [this](vector<Aresta*> dentro, vector<Aresta*> fora) {
            return get_indice_eleito(dentro, fora);
        };

    } else {

        return [this, alfa](vector<Aresta*> dentro, vector<Aresta*> fora) {
            return get_indice_eleito_random(alfa, dentro, fora);
        };
    }
}

int Guloso::qualidade_da_solucao() {

    /* 
    Assume que arestas estão duplicadas, e ambas foram marcadas como
    dominantes.
    */

    int qtd_arestas_dominantes = 0; 

    for (No* no : grafo->get_lista_adj())
        for (Aresta* aresta : no->get_arestas())
            if (aresta->get_domina())
                qtd_arestas_dominantes++;

    return qtd_arestas_dominantes / 2;
}
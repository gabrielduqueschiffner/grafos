
#include "../include/Guloso.h"
#include <random>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include "../include/Grafo.h"
#include "../include/Aresta.h"
#include "../include/No.h"
#include "../include/types.h"

using namespace std;

Guloso::Guloso(float alfa, int seed) {
    aleatorio = new Aleatorio(seed);
    this->alfa = alfa;
}

Guloso::Guloso(float alfa) {
    random_device rd;
    aleatorio = new Aleatorio(rd());
    this->alfa = alfa;
}

Grafo* Guloso::conjunto_dominante_arestas(Grafo* grafo) {

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
    Aleatorio aleatorio; //(-406155332); // seed com erro => alfa: 0.4, e seed: -406155332
    unordered_map<NoId, int> mapa_id_index = grafo_marcado->get_mapa_id_index();

    // Copiar arestas sem repetição
    
    vector<Aresta*> fora = {};
    
    for(No* no : grafo_marcado->get_lista_adj()) {

        int index_origem = mapa_id_index[no->get_id()];
        for(Aresta* aresta : no->get_arestas())
            if (index_origem <= mapa_id_index[aresta->get_id_no_alvo()])
                fora.push_back(aresta);
    }

    // Calcula o grau total de uma aresta (soma do grau do de origem com alvo)
    auto grau_total = [grafo_marcado](Aresta* aresta){
        return grafo_marcado->encontra_no_por_id(aresta->get_id_no_alvo())->get_arestas().size() 
        + grafo_marcado->encontra_no_por_id(aresta->get_id_no_origem())->get_arestas().size();
    };

    // Arestas fora e dentro do conjunto dominante

    vector<Aresta*> dentro = {};
    dentro.reserve(fora.size());
    
    // Enquanto não houver arestas a serem avaliadas, selecionar arestas

    while (fora.size() > 0) {

        int indice_eleito;

        if (alfa == 0) {

            // Se for guloso normal, pegar o melhor 
            // (escolha determinística: aresta com maior grau total)
            
            auto it = max_element(
                fora.begin(), fora.end(), 
                [grau_total](Aresta* a, Aresta* b) {
                    return grau_total(a) < grau_total(b);
                }
            );
            indice_eleito = distance(fora.begin(), it);

        } else {

            // Se for guloso aleatório: ordenar, calcular tamanho da amostra e escolher
            
            // Ordenar arestas por maior grau total
            // FIXME: só considerar se tiver ligado a arestas dentro???
            sort(fora.begin(), fora.end(), [grau_total](Aresta* a, Aresta* b) { 
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

            indice_eleito = aleatorio.gerar_inteiro(tamanho_amostra);
        }
        
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
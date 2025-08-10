
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include "../include/Grafo.h"
#include "../include/Guloso.h"
#include "../include/No.h"
#include "../include/types.h"

using namespace std;

Guloso::Guloso(Grafo *grafo, int seed) {

    /* Inicializador simples para realização apenas do guloso não reativo */

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

Guloso::Guloso(
    Grafo *grafo, vector<float> alfas, int max_iter, int k_bloco, int seed
) {

    /* Inicializador para realizar guloso reativo */
    
    if (alfas.empty()) 
        throw runtime_error("vetor de alfas vazio.");

    this->grafo = grafo;
    this->alfas = alfas;
    this->max_iter = max_iter;
    this->k_bloco = k_bloco;
    this->melhor_solucao = nullptr;
    this->delta = 1.0f;

    int tam_alfa = alfas.size(); // Número de alfas
    
    probs_alfas.assign(tam_alfa, 1.0f / tam_alfa);     
    qualidade_alfas.assign(tam_alfa, 0.0f);         
    qualidade_relat_alfas.assign(tam_alfa, 0.0f); 
    qtd_alfas.assign(tam_alfa, 0.0f);          
    qualidade_media = 0.0f; // FIXME: Não usado

    // Valor padrão -1 pra seed, sinaliza que não foi passada uma seed
    if (seed == -1) {
        random_device rd;
        seed = rd();
    }
    
    this->aleatorio = new Aleatorio(seed);
}

Guloso::~Guloso() {
    
    delete aleatorio;
    
    // FIXME: quem desaloca isso?
    // if (melhor_solucao) 
    //     delete melhor_solucao;

    // A memória do ponteiro "grafo" é de propriedade externa
    // TODO: Deveria ser recebido e passado invés de guardado?
}

int Guloso::qualidade_da_solucao(Grafo *solucao) {

    /*
    Assume que arestas estão duplicadas, e ambas foram marcadas como
    dominantes. Não é válido para grafos direcionados.
    */

    if (solucao->get_direcionado())
        throw runtime_error("Não é valido para grafos direcionados");

    // Soma de arestas dentro da solução
    
    int qtd_arestas_dominantes = 0;
    for (No *no : solucao->get_lista_adj())
        for (Aresta* aresta : no->get_arestas())
            if (aresta->get_domina())
                qtd_arestas_dominantes++;    

    return qtd_arestas_dominantes / 2;
}

void Guloso::atualizar_probs() {
    
    /* Atualiza as probabilidades de cada alfa, baseado na qualidade média
    das soluções encontradas até o momento. */

    int tam_alfas = static_cast<int>(alfas.size());

    const float INFINITO = numeric_limits<float>::infinity();

    // FIXME: Setar como infinito mata a exploração?
    for (int i = 0; i < tam_alfas; i++)
        qualidade_relat_alfas[i] = qtd_alfas[i] > 0 ? qualidade_alfas[i] / qtd_alfas[i] : INFINITO;

    // 2) encontrar F*, melhor qualidade global
    int f_estrela = (melhor_solucao != nullptr) ? qualidade_da_solucao(melhor_solucao) : 0; 

    // 3) calcular qi
    for (int i = 0; i < tam_alfas; i++) {

        float m_i = qualidade_relat_alfas[i]; // m_i é a qualidade relativa do alfa i
        
        if (m_i > 0 && m_i != INFINITO)
            qualidade_relat_alfas[i] = pow(f_estrela / qualidade_relat_alfas[i], delta);
        else
            qualidade_relat_alfas[i] = 0;
    }

    // 4) média global de m_i
    
    float soma = 0;
    int conatgem = 0;
    for (int i = 0; i < tam_alfas; ++i) {
        if (qtd_alfas[i] > 0) {
            soma += (qualidade_alfas[i] / float(qtd_alfas[i]));
            ++conatgem;
        }
    }

    qualidade_media = (conatgem > 0) ? (soma / conatgem) : 0; 

    // 5) Normalizar qi para obter probabilidades (probs_alfas)
    
    float soma_qi = accumulate(
        qualidade_relat_alfas.begin(), 
        qualidade_relat_alfas.end(), 
        0.0f
    );

    
    if (soma_qi > 0) {
        
        for (int i = 0; i < tam_alfas; ++i) 
            probs_alfas[i] = qualidade_relat_alfas[i] / soma_qi;
        
    } else {

        // Se a soma for zero, setar probabilidades uniformes
        for (int i = 0; i < tam_alfas; i++)
            probs_alfas[i] = 1.0f / tam_alfas;
        
    }

    // Resetar variáveis
    
    fill(qualidade_alfas.begin(), qualidade_alfas.end(), 0.0f);
    fill(qtd_alfas.begin(), qtd_alfas.end(), 0.0f);
}

bool Guloso::atualizar_melhor_solucao(Grafo *solucao) {

    /* Atualiza a melhor solução encontrada, caso a nova solução for melhor. Faz
    isso ao guardar um clone da solução. */

    if (!melhor_solucao || qualidade_da_solucao(solucao) > qualidade_da_solucao(melhor_solucao)) {
        
        if (melhor_solucao)
            delete melhor_solucao; 
        
        melhor_solucao = solucao->clone();
        
        return true;
    }

    return false;
}

void Guloso::atualizar_qualidades(int indice_alfa, Grafo *solucao) {

    /* Atualiza as qualidades relativas e absolutas de cada alfa, baseado na
    qualidade da solução encontrada. */

    int qualidade = qualidade_da_solucao(solucao);
    qualidade_alfas[indice_alfa] += qualidade;
    qtd_alfas[indice_alfa] += 1;
}

int Guloso::selecionar_alfa() {

    /* Seleciona um alfa baseado nas probabilidades atualizadas. */

    float soma = accumulate(probs_alfas.begin(), probs_alfas.end(), 0.0f);

    if (fabs(soma - 1.0f) > 1e-6f)
        throw runtime_error("Soma do array de probs é diferente de 1.");

    int tam_probs = static_cast<int>(probs_alfas.size());

    float soma_minima = aleatorio->gerar_float(1);
    float soma_probs = 0.0f;

    for (int i = 0; i < tam_probs; ++i) {
        soma_probs += probs_alfas[i]; // Soma as probabilidades acumuladas

        // Verifica se o número aleatório está dentro do intervalo acumulado
        if (soma_minima <= soma_probs)
            return i; // Retorna o índice do alfa selecionado
    }

    throw runtime_error("Nenhum alfa selecionado.");
}

Grafo* Guloso::rodar_reativo() {

    if (alfas.empty())
        throw runtime_error("Vetor de alfas vazio! Não posso rodar reativo.");

    if (probs_alfas.size() != alfas.size())
        throw runtime_error("Vetores alfas e probs_alfas com tamanhos inconsistentes.");

    Grafo* solucao = nullptr;

    for (int i = 0; i < max_iter; ++i) {
        
        if ((i != 0) && (i % k_bloco == 0)) {

            atualizar_probs();
        
            if ((i % k_bloco) == 0) {
                cout << "Iteração " << i << "\n";
                for (size_t j = 0; j < alfas.size(); ++j) {
                    cout << "  alfa[" << j << "]=" << alfas[j]
                        << " prob=" << probs_alfas[j]
                        << " qualidade_media=" << (qtd_alfas[j] > 0 ? qualidade_alfas[j] / qtd_alfas[j] : 0)
                        << "\n";
                }
                cout << "Melhor qualidade global: " << qualidade_da_solucao(melhor_solucao) << "\n\n";
            }
        }

        int indice_alfa_eleito = selecionar_alfa();
        float alfa_eleito = alfas[indice_alfa_eleito];

        // Aloca um grafo, transferindo memória 
        solucao = conjunto_dominante_arestas(alfa_eleito);

        if (!solucao)
            throw runtime_error("Não foi retornada uma solução.");
        
        atualizar_qualidades(indice_alfa_eleito, solucao);

        // Só liberar memória caso a posse não tenha sido transferida
        if (!atualizar_melhor_solucao(solucao))
            delete solucao;
    }

    return melhor_solucao;
}

Grafo* Guloso::conjunto_dominante_arestas(float alfa) {

    /*
    Algoritmo guloso random para encontrar um conjunto dominante de arestas.
    Ordena todas as arestas que ainda não foram consideradas de acordo com a sua
    qualidade dfinida pela heurística, e em seguida escolhe uma entre as 
    melhores (entre as N primeiras).
    */

    if (grafo->get_direcionado()) {
        throw std::runtime_error(
            "Conjunto dominante de arestas não se aplica para grafo direcionado.");
    }

    if (alfa < 0 || alfa > 1)
        throw runtime_error("Erro: alfa deve estar entre 0 e 1.");

    // Aloca um clone no ponteiro passado
    Grafo* solucao = grafo->clone();
    
    // Copiar arestas sem repetição

    vector<Aresta*> fora = {};
    vector<Aresta*> dentro = {};

    auto mapa_id_index = solucao->get_mapa_id_index();

    for (No *no : solucao->get_lista_adj()) {

        int index_origem = mapa_id_index[no->get_id()];
        for (Aresta *aresta : no->get_arestas())
        if (index_origem <= mapa_id_index[aresta->get_id_no_alvo()])
            fora.push_back(aresta);
    }
    
    dentro.reserve(fora.size());

    // Decidindo heurística e armazenando em variável

    auto heuristica = get_heuristica(alfa);

    // Enquanto não houver arestas a serem avaliadas selecionar e incluir na solução

    while (fora.size() > 0) {

        int indice_eleito = heuristica(fora);
        Aresta *aresta_eleita = fora[indice_eleito];

        // Marcar como dominante a aresta indo, e a resta voltando

        aresta_eleita->set_domina(true);
        No *no_alvo = solucao->encontra_no_por_id(aresta_eleita->get_id_no_alvo());

        for (Aresta *aresta_alvo : no_alvo->get_arestas())
        if (aresta_alvo->get_id_no_alvo() == aresta_eleita->get_id_no_origem())
            aresta_alvo->set_domina(true);

        // Remover do conjunto "fora" as arestas adjascentes à aresta selecionada

        int eleito_origem = aresta_eleita->get_id_no_origem();
        int eleito_alvo = aresta_eleita->get_id_no_alvo();

        dentro.push_back(aresta_eleita);

        vector<Aresta*> arestas_restantes;

        for (Aresta *aresta : fora) {

            int origem = aresta->get_id_no_origem();
            int alvo = aresta->get_id_no_alvo();

            if ((eleito_origem != origem && eleito_origem != alvo) 
                && (eleito_alvo != origem && eleito_alvo != alvo)) {
                arestas_restantes.push_back(aresta);
            }
        }

        fora = arestas_restantes;
    }

    return solucao;
}

int Guloso::get_indice_eleito_comum(vector<Aresta*>& fora) {

    /* Para guloso não-random, pega sempre o elemento máximo */

    if (fora.empty())
        throw runtime_error("Não pode escolher entre nenhum item");

    auto it = max_element(fora.begin(), fora.end(), [this](Aresta *a, Aresta *b) {
        return grau_total(a) < grau_total(b);
    });

    return distance(fora.begin(), it);
}

int Guloso::get_indice_eleito_random(float alfa, vector<Aresta*>& fora) {

    /* Para o guloso random, escolhe aleatoriamente entre os melhores. 
    A quantidade de melhores é calculada a partir de alfa. */

    if (fora.empty())
        throw runtime_error("Sem aresta a ser escolhida.");

    if (alfa < 0 || alfa > 1)
        throw runtime_error("Valor de alfa impróprio: " + to_string(alfa));

    // FIXME: só considerar se tiver ligado a arestas dentro???
    sort(fora.begin(), fora.end(),
        [this](Aresta *a, Aresta *b) { return grau_total(a) > grau_total(b); });

    int melhor_qualidade = grau_total(fora.front());
    int pior_qualidade = grau_total(fora.back());

    float pior_aceito = melhor_qualidade - alfa * (melhor_qualidade - pior_qualidade);

    // Calcular quantos quandidatos serão aceitos
    int tamanho_amostra = 0;
    for (Aresta *aresta : fora) {
        if (grau_total(aresta) >= pior_aceito)
            tamanho_amostra++;
        else
            break;
    }
    
    // Sortei o índice de uma aresta dentro da amostra
    return aleatorio->gerar_inteiro(tamanho_amostra-1);
}

int Guloso::grau_total(Aresta *aresta) {
    
    /* 
    Calcula o grau total de uma aresta (a soma do grau do nó de origem 
    com o grau do nó alvo) 
    */
    
    int grau_origem = grafo->encontra_no_por_id(
        aresta->get_id_no_origem()
    )->get_arestas().size();
    
    int grau_alvo = grafo->encontra_no_por_id(
        aresta->get_id_no_alvo()
    )->get_arestas().size();

    return grau_origem + grau_alvo;
}

function<int(vector<Aresta *>)> Guloso::get_heuristica(
    float alfa
) {

    /* Retorna um lambda com a heurística a ser empregada, baseado no valor de
    alfa */

    if (alfa == 0) {

        return [this](vector<Aresta*> fora) {
            return get_indice_eleito_comum(fora);
        };

    } else {

        return [this, alfa](vector<Aresta*> fora) {
            return get_indice_eleito_random(alfa, fora);
        };
    }
}

int Guloso::qualidade_da_solucao() {

  /*
  Assume que arestas estão duplicadas, e ambas foram marcadas como
  dominantes.
  */

  int qtd_arestas_dominantes = 0;

  for (No *no : grafo->get_lista_adj())
    for (Aresta *aresta : no->get_arestas())
      if (aresta->get_domina())
        qtd_arestas_dominantes++;

  return qtd_arestas_dominantes / 2;
}
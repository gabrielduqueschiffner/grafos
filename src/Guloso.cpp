
#include <random>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "../include/Guloso.h"
#include "../include/Grafo.h"
#include "../include/Aresta.h"
#include "../include/No.h"
#include "../include/types.h"

using namespace std;

Guloso::Guloso(Grafo *grafo, int seed)
{

    cout << "[DEBUG] Construtor simples: seed=" << seed << "\n";
    // Só faz aletório, seed definida

    this->grafo = grafo;

    if (seed == -1)
    {

        random_device rd;
        this->aleatorio = new Aleatorio(rd());
    }
    else
    {

        this->aleatorio = new Aleatorio(seed);
    }

    this->alfas = {};
    this->max_iter = -1;
    this->k_bloco = -1;
}

Guloso::Guloso(Grafo *grafo, vector<float> alfas, int max_iter, int k_bloco, int seed)
{

    //
    this->grafo = grafo;
    this->alfas = alfas;
    this->max_iter = max_iter;
    this->k_bloco = k_bloco;
    this->melhor_solucao = nullptr;
    this->delta = 1.0f;

    int m = alfas.size(); // Número de alfas

    if (m == 0)
    {
        throw runtime_error("CONSTRUTOR GULOSO ERROR: vetor de alfas vazio.");
    }
    probs_alfas.assign(m, 1.0f / m);    // Inicializa com probabilidades uniformes
    qualid_alfas.assign(m, 0.0f);       // Inicializa qualidades absolutas
    qualid_relat_alfas.assign(m, 0.0f); // Inicializa qualidades relativas
    quant_alfas.assign(m, 0.0f);        // Inicializa contagem de soluções encontradas
    qualid_media = 0.0f;                // Inicializa qualidade média  FIXME: devo usar quando??????

    // Inicializa o gerador de números aleatórios
    // Se seed for -1, usa random_device para gerar um seed aleatório
    // Caso contrário, usa o seed fornecido
    if (seed == -1)
    {

        random_device rd;
        this->aleatorio = new Aleatorio(rd());
    }
    else
    {

        this->aleatorio = new Aleatorio(seed);
    }

    cout << "[DEBUG] Construtor completo: alfas.size()=" << alfas.size()
         << " max_iter=" << max_iter << " k_bloco=" << k_bloco
         << " seed=" << seed << "\n";
}

int Guloso::qualidade_da_solucao(Grafo *solucao)
{
    /*
    Assume que arestas estão duplicadas, e ambas foram marcadas como
    dominantes.
    */

    // conta todas as arestas nos vetores de adjacencia
    int qtd_arestas_dominantes = 0;
    for (No *no : solucao->get_lista_adj())
    {
        qtd_arestas_dominantes += int(no->get_arestas().size());
    }

    if (!solucao->get_direcionado())
        qtd_arestas_dominantes /= 2;
    // Se não for direcionado, cada aresta é contada duas vezes

    return qtd_arestas_dominantes;
}

void Guloso::atualizar_probs()
{
    /* Atualiza as probabilidades de cada alfa, baseado na qualidade média
    das soluções encontradas até o momento. */
    int m = int(alfas.size());

    // 1) calcular médias mi = qualid_alfas[i] / quant_alfas[i]
    for (int i = 0; i < m; i++)
    {
        if (quant_alfas[i] > 0)
        {
            qualid_relat_alfas[i] = qualid_alfas[i] / float(quant_alfas[i]);
        }
        else
        {
            qualid_relat_alfas[i] = numeric_limits<float>::infinity(); // ou algum valor padrão
        }
    }
    // 2) encontrar F* = melhor qualidade entre todas as alfas (melhor solucao global)
    int F_Estrela;

    if (melhor_solucao != nullptr)
    {
        F_Estrela = qualidade_da_solucao(melhor_solucao); // ou algum valor padrão se melhor_solucao for nulo
    }
    else
    {
        F_Estrela = 0; // ou algum valor padrão se não houver melhor solução
    }

    // 3) calcular qi = (F*/Mi)^delta
    for (int i = 0; i < m; i++)
    {
        float Mi = qualid_relat_alfas[i]; // Mi é a qualidade relativa do alfa i
        if ((Mi > 0) && (Mi < numeric_limits<float>::infinity()))
        {
            qualid_relat_alfas[i] = pow(F_Estrela / qualid_relat_alfas[i], delta);
        }
        else
        {
            qualid_relat_alfas[i] = 0.0f; // ou algum valor padrão
        }
    }

    // 4) média global de Mi
    {
        float soma = 0;
        int cnt = 0;
        for (int i = 0; i < m; ++i)
        {
            if (quant_alfas[i] > 0)
            {
                soma += (qualid_alfas[i] / float(quant_alfas[i]));
                ++cnt;
            }
        }
        if (cnt > 0)
        {
            qualid_media = soma / cnt; // Média global de Mi
        }
        else
        {
            qualid_media = 0.0f; // Se não houver soluções, média é zero
        }
    }

    // 5) Normalizar qi para obter probabilidades (probs_alfas)
    float soma_qi = accumulate(qualid_relat_alfas.begin(), qualid_relat_alfas.end(), 0.0f);
    if (soma_qi > 0)
    {
        for (int i = 0; i < m; ++i)
        {
            probs_alfas[i] = qualid_relat_alfas[i] / soma_qi;
        }
    }
    else
    {
        // Se a soma for zero, distribuir probabilidades uniformemente
        for (int i = 0; i < m; i++)
        {
            probs_alfas[i] = 1.0f / m;
        }
    }

    // 6) Resetar qualid_alfas e quant_alfas (acumuladores) para a próxima rodada(bloco)
    fill(qualid_alfas.begin(), qualid_alfas.end(), 0.0f);
    fill(quant_alfas.begin(), quant_alfas.end(), 0.0f);
}

void Guloso::atualizar_melhor_solucao(Grafo *solucao)
{
    /* Atualiza a melhor solução encontrada, se a nova solução for melhor. */

    int qualidade_nova = qualidade_da_solucao(solucao);
    // se for a primeira solução, ou se for melhor que a melhor solução atual
    if (!melhor_solucao || qualidade_nova < qualidade_da_solucao(melhor_solucao))
    {
        // atualiza a melhor solucao
        delete melhor_solucao;             // libera memória da solução anterior
        melhor_solucao = solucao->clone(); // Clona a solução para manter a original intacta
    }
    delete solucao; // Libera a memória da solução temporária
}

void Guloso::atualizar_qualidades(int indice_alfa, Grafo *solucao)
{
    /* Atualiza as qualidades relativas e absolutas de cada alfa, baseado na
    qualidade da solução encontrada. */

    int qualidade = qualidade_da_solucao(solucao);
    // Atualiza a qualidade absoluta
    qualid_alfas[indice_alfa] += qualidade;
    // Atualiza a quantidade de soluções encontradas com esse alfa
    quant_alfas[indice_alfa] += 1;

    // DELETO AQUI TBM? ESTAVA DANDO PROBLEMA DE MEMORIA
    //   delete solucao; // Libera a memória da solução temporária
}

int Guloso::selecionar_alfa()
{
    /* Seleciona um alfa baseado nas probabilidades atualizadas. */

    int m = int(probs_alfas.size());

    float roleta = aleatorio->gerar_inteiro(100) / 100.0f; // Gera um número aleatório entre 0 e 1
    float soma_probs = 0.0f;

    for (int i = 0; i < m; ++i)
    {
        soma_probs += probs_alfas[i]; // Soma as probabilidades acumuladas

        // Verifica se o número aleatório está dentro do intervalo acumulado
        if (roleta <= soma_probs)
            return i; // Retorna o índice do alfa selecionado
    }

    // Se não encontrou, retornar o último índice (deve ser o caso de alfas vazios)
    // Isso pode acontecer se todas as probabilidades forem zero, o que não deveria ocorrer
    // se a lógica de atualização de probabilidades estiver correta.
    cout << "Aviso: Nenhum alfa selecionado, retornando último índice." << endl;

    return m - 1; // Retorna o último índice
}

Grafo *Guloso::rodar_reativo()
{

    // melhor_solucao = nullptr; // Reseta a melhor solução antes de começar
    //  Me parece desnecessário, pois já é inicializado no construtor
    //  mas vou deixar comentado para não perder a referência

    cout << "[DEBUG] Entrou em rodar_reativo: max_iter="
         << max_iter << ", k_bloco=" << k_bloco
         << ", alfas.size()=" << alfas.size()
         << ", probs_alfas.size()=" << probs_alfas.size()
         << "\n";

    if (alfas.empty())
    {
        throw runtime_error("[ERROR] vetor de alfas vazio! Não posso rodar reativo.");
        return nullptr;
    }

    if (probs_alfas.size() != alfas.size())
    {
        cerr << "[ERROR] probs_alfas.size()=" << probs_alfas.size()
             << " mas alfas.size()=" << alfas.size() << "\n";

        return nullptr;
    }

    for (int i = 0; i < max_iter; ++i)
    {
        cout << "[DEBUG - FUNCREATIVA]iteração " << i << "\n";
        // recalcula probabilidades a cada k_bloco iterações
        if (i > 0 && i % k_bloco == 0)
        {
            cout << "[DEBUG] chamando atualizar_probs()\n";
            atualizar_probs();
        }

        // escolhe a segundo probs_alfa
        int indice_alfa = selecionar_alfa();
        float alfa = alfas[indice_alfa];

        cout << "[DEBUG] selecionei indice_alfa=" << indice_alfa
             << " valor=" << alfas[indice_alfa] << "\n";

        cout << "[DEBUG] chamando conjunto_dominante_arestas...\n";
        Grafo *solucao = conjunto_dominante_arestas(alfa);

        if (!solucao)
        {
            throw runtime_error("[ERROR] conjunto_dominante_arestas retornou nullptr");
            break;
        }

        cout << "[DEBUG] solução gerada com sucesso, agora atualizando qualidades\n";
        // 1) acumula a qualidade da solução

        atualizar_qualidades(indice_alfa, solucao);
        cout << "[DEBUG] qualidades atualizadas, agora atualizando melhor solução\n";
        // 2) tambem verifica se foi a melhor global
        // precisa de um clone , porque a solucao é temporária e ja foi deletada o ponteiro

        // Grafo* solucao_clone = solucao->clone();
        atualizar_melhor_solucao(solucao);
        cout << "[DEBUG] melhor solução potencialmente atualizada\n";
    }

    return melhor_solucao;
}

Grafo *Guloso::conjunto_dominante_arestas(float alfa)
{

    /*
    Algoritmo guloso random para encontrar um conjunto dominante de arestas.
    Ordena todas as arestas não consideradas ainda, e a partir dela, escolhe uma
    entre as primeiras (sendo as N primeiras, a amostra de candidatos separados,
    bastando sortear um valor de 0 a N para obter encontrar o candidato escolhido.
    */

    cout << "[DEBUG conj] **ENTRADA** conjunto_dominante_arestas(alfa="
         << alfa << ")\n";

    if (grafo->get_direcionado())
    {
        throw std::runtime_error(
            "Conjunto dominante de arestas não se aplica para grafo direcionado.");
    }

    if (alfa < 0 || alfa > 1)
        throw runtime_error("Erro: alfa deve estar entre 0 e 1.");

    cout << "[DEBUG conj] 1) antes de clonar grafo\n";
    Grafo *grafo_marcado = grafo->clone();
    cout << "[DEBUG conj] 1) depois de clonar, nós = "
         << grafo_marcado->get_lista_adj().size() << "\n";

    auto mapa_id_index = grafo_marcado->get_mapa_id_index();

    // Copiar arestas sem repetição

    vector<Aresta *> fora = {};

    cout << "[DEBUG conj] 2) coletando arestas em 'fora'\n";

    for (No *no : grafo_marcado->get_lista_adj())
    {

        int index_origem = mapa_id_index[no->get_id()];
        for (Aresta *aresta : no->get_arestas())
            if (index_origem <= mapa_id_index[aresta->get_id_no_alvo()])
                fora.push_back(aresta);
    }
    cout << "[DEBUG conj] 2) fora.size() = " << fora.size() << "\n";
    // Arestas fora e dentro do conjunto dominante

    vector<Aresta *> dentro = {};
    dentro.reserve(fora.size());

    // Decidindo heurística e armazenando em variável

    auto heuristica = get_heuritica(alfa);

    // Enquanto não houver arestas a serem avaliadas, selecionar arestas

    int passo = 0;
    cout << "[DEBUG conj] 3) entrando no while(fora)\n";
    while (fora.size() > 0)
    {
        cout << "[DEBUG conj]   loop passo=" << passo++
             << " fora.size()=" << fora.size() << "\n";

        int indice_eleito = heuristica(dentro, fora);

        cout << "[DEBUG conj]    eleito idx=" << indice_eleito
             << " em [0.." << fora.size() - 1 << "]\n";

        Aresta *aresta_eleita = fora[indice_eleito];

        // Marcar arestas dos dois sentidos

        aresta_eleita->set_domina(true);
        No *no_alvo = grafo_marcado->encontra_no_por_id(aresta_eleita->get_id_no_alvo());

        for (Aresta *aresta_alvo : no_alvo->get_arestas())
            if (aresta_alvo->get_id_no_alvo() == aresta_eleita->get_id_no_origem())
                aresta_alvo->set_domina(true);

        // Remover do conjunto "fora" as arestas adjascentes à aresta selecionada

        int eleito_origem = aresta_eleita->get_id_no_origem();
        int eleito_alvo = aresta_eleita->get_id_no_alvo();

        // Mover aresta de acordo

        dentro.push_back(aresta_eleita);

        vector<Aresta *> arestas_restantes;

        for (Aresta *aresta : fora)
        {

            int origem = aresta->get_id_no_origem();
            int alvo = aresta->get_id_no_alvo();

            if (
                eleito_origem != origem && eleito_origem != alvo && eleito_alvo != origem && eleito_alvo != alvo)
            {
                arestas_restantes.push_back(aresta);
            }
        }

        fora = arestas_restantes;
    }

    cout << "[DEBUG conj] 4) saiu do while, retornando grafo_marcado\n";

    // TODO: Esse formato está de acordo com o desejado no trabalho?

    return grafo_marcado;
}

int Guloso::get_indice_eleito(vector<Aresta *> dentro, vector<Aresta *> fora)
{

    // Se for guloso normal, pegar o melhor
    // (escolha determinística: aresta com maior grau total)

    auto it = max_element(
        fora.begin(), fora.end(),
        [this](Aresta *a, Aresta *b)
        {
            return grau_total(a) < grau_total(b);
        });

    return distance(fora.begin(), it);
}

int Guloso::get_indice_eleito_random(float alfa, vector<Aresta *> dentro, vector<Aresta *> fora)
{

    // Se for guloso aleatório: ordenar, calcular tamanho da amostra e escolher

    if (fora.empty())
    {
        return get_indice_eleito(dentro, fora); // Se não houver arestas fora, retorna o índice do melhor candidato
    }

    // 1) Ordenar arestas por maior grau total (grau total decrescente)
    // FIXME: só considerar se tiver ligado a arestas dentro???
    sort(fora.begin(), fora.end(), [this](Aresta *a, Aresta *b)
         { return grau_total(a) > grau_total(b); });

    // 2) Calcular o pior valor aceito na seleção de candidatos:

    int melhor = grau_total(fora.front());
    int pior = grau_total(fora.back());

    // 3) calcula o limite de aceitação
    float pior_aceito = melhor - alfa * (melhor - pior);

    // Encontrar o tamanho da amostra (contar até o pior candidato aceito)

    // 4) conta quantos candidatos estão acima do pior aceito
    int tamanho_amostra = 0;
    for (Aresta *aresta : fora)
    {
        if (grau_total(aresta) >= pior_aceito)
            tamanho_amostra++;
        else
            break;
    }

    // MODIFICAOCAO FEITA AQUI , faz sentido?
    //   5) se a amostra ficou vazia, volta ao determinístico
    if (tamanho_amostra <= 0)
    {
        return get_indice_eleito(dentro, fora);
    }
    // Dentre os candidatos, escolher um aleatoriamente

    // ADICIONEI O INDICE_MAX AQUI PQ ESTAVA ESTOURANDO O VETOR E A MEMORIA
    int indice_max = tamanho_amostra - 1; // Último índice da amostra
    if (indice_max < 0)
    {
        // cerr << "[ERROR] Tamanho da amostra é negativo! Retornando 0.\n";
        return 0; // Retorna 0 se a amostra for negativa, o que não deveria acontecer
    }

    // 6) sorteia um índice aleatório entre 0 e tamanho_amostra-1
    return aleatorio->gerar_inteiro(indice_max);
}

int Guloso::grau_total(Aresta *aresta)
{

    // Calcula o grau total de uma aresta (soma do grau do de origem com alvo)
    return grafo->encontra_no_por_id(aresta->get_id_no_alvo())->get_arestas().size() + grafo->encontra_no_por_id(aresta->get_id_no_origem())->get_arestas().size();
}

function<int(vector<Aresta *>, vector<Aresta *>)> Guloso::get_heuritica(float alfa)
{

    /* Retorna um lambda com a heurística a ser empregada, baseado no valor de
    alfa */

    if (alfa == 0)
    {

        return [this](vector<Aresta *> dentro, vector<Aresta *> fora)
        {
            return get_indice_eleito(dentro, fora);
        };
    }
    else
    {

        return [this, alfa](vector<Aresta *> dentro, vector<Aresta *> fora)
        {
            return get_indice_eleito_random(alfa, dentro, fora);
        };
    }
}

int Guloso::qualidade_da_solucao()
{

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
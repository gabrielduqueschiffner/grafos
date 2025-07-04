#include "Grafo.h"
#include <algorithm>
#include <stdexcept>
#include <vector>
#include "Aresta.h"
#include "No.h"
#include "types.h"

Grafo::Grafo()
{
}

Grafo::~Grafo()
{
}

void Grafo::set_direcionado(int direcao)
{
    in_direcionado = direcao;
}
void Grafo::set_ponderado_aresta(int ponderado)
{
    in_ponderado_aresta = ponderado;
}
void Grafo::set_ponderado_vertice(int ponderado)
{
    in_ponderado_vertice = ponderado;
}
void Grafo::set_ordem(int ordem_grafo)
{
    ordem = ordem_grafo;
}
int Grafo::get_direcionado()
{
    return in_direcionado;
}
int Grafo::get_ponderado_aresta()
{
    return in_ponderado_aresta;
}
int Grafo::get_ponderado_vertice()
{
    return in_ponderado_vertice;
}

int Grafo::get_ordem()
{
    return ordem;
}

vector<No*> Grafo::get_lista_adj()
{
    return lista_adj;
}

void Grafo::adiciona_no(NoId id_no, int peso)
{
    No *no = new No();
    no->set_id(id_no);

    if (in_ponderado_vertice)
    {
        no->set_peso(peso);
    }
    lista_adj.push_back(no);
}

void Grafo::imprime_grafo() // imprime grafo no formato: id(peso): -> id_alvo(peso)
{
    for (No *no : lista_adj)
    {
        cout << no->get_id() << "(" << no->get_peso() << ")" << ": ";
        for (Aresta *aresta : no->get_arestas())
        {
            cout << "-> " << aresta->get_id_no_alvo() << "(" << aresta->get_peso() << ")";
        }
        cout << endl;
    }
}

void Grafo::imprime_vector(vector<NoId> v)
{
    if (v.size() == 0)
    {
        return;
    }
    else
    {
        for (size_t i = 0; i < v.size(); ++i)
        {
            cout << v[i];
            if (i + 1 < v.size())
            {
                cout << ",";
            }
        }
        cout << endl;
    }
}

void Grafo::imprime_resumo_grafo() {
    cout << "=> Características do grafo:" << endl;
    cout << "- Direcionado: " << (get_direcionado() ? "sim" : "nao") << endl;
    cout << "- Ponderado Aresta: " << (get_ponderado_aresta() ? "sim" : "nao") << endl;
    cout << "- Ponderado Vertice: " << (get_ponderado_vertice() ? "sim" : "nao") << endl;
    cout << "- Ordem: " << get_ordem() << endl;
}

// =======================================
// A) FECHO TRANSITIVO DIRETO
// =======================================

//Gabriel
vector<NoId> Grafo::fecho_transitivo_direto(NoId id_no)
{
    int n = lista_adj.size();
    vector<NoId> resultado;
    if (n == 0) {
        cout << "Lista de adjacência vazia. Não há fecho transitivo direto." <<endl;
        return resultado;
    }

    if (!get_direcionado()) {
        cout << "Grafo não direcionado. Fecho transitivo direto não implementado." << endl;
        return resultado;
    }

    // 1) Mapeamento Id → índice

    auto mapa_id_index = get_mapa_id_index(); // cria um mapa de NoId para índice
    auto it = mapa_id_index.find(id_no); // busca pelo ID do nó inicial
    if (it == mapa_id_index.end()) {
        cout << "Vértice '" << id_no << "' não existe no grafo." <<endl; 
        return resultado;
    }
    int idx_inicio = it->second; 

    // 2) Vetor de marcados e chama a busca em profundidade auxiliar
    vector<bool> marcado(n, false);
    marcado[idx_inicio] = true;  // marca o nó inicial como visitado
    dfs_fecho_transitivo_direto(idx_inicio, marcado, resultado, mapa_id_index);

    // 3) Imprime e retorna
    cout << "Fecho transitivo a partir de '" << id_no << "': ";
    imprime_vector(resultado);
    return resultado;
}

//Gabriel
// Função auxiliar de busca em profundidade que coleta o fecho transitivo direto
void Grafo::dfs_fecho_transitivo_direto(int idx,
                                        vector<bool> &marcado,
                                        vector<NoId> &resultado,
                                        const unordered_map<NoId,int> &mapa_id_index)
{
    for (Aresta *a : lista_adj[idx]->get_arestas()) // percorre as arestas do nó atual
    {
        NoId v_id = a->get_id_no_alvo(); //
        auto it = mapa_id_index.find(v_id); // busca o ID do nó alvo no mapa
        if (it == mapa_id_index.end()) continue;
        int idx_v = it->second;
        if (!marcado[idx_v]) // se o nó alvo ainda não foi visitado
        {
            marcado[idx_v] = true; // marca como visitado
            resultado.push_back(v_id); // adiciona ao resultado
            dfs_fecho_transitivo_direto(idx_v, marcado, resultado, mapa_id_index); 
        }
    }
}

// =======================================

// Gustavo
vector<NoId> Grafo::fecho_transitivo_indireto(NoId id_no)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}


// =======================================
// C) DIJKSTRA
// =======================================

//Gabriel
vector<NoId> Grafo::caminho_minimo_dijkstra(NoId id_origem, NoId id_destino)
{
    int numero_vertices = lista_adj.size();
    if (numero_vertices == 0)
    {
        cout << "Grafo vazio!!!" << endl;
        return {};
    }

    // 1) Mapa de ID → índice
    auto mapa_id_para_indice = get_mapa_id_index();

    // Verifica existência dos vértices
    if (!mapa_id_para_indice.count(id_origem))
    {
        cout << "Vértice de origem '" << id_origem << "' não existe no grafo.\n";
        return {};
    }
    if (!mapa_id_para_indice.count(id_destino))
    {
        cout << "Vértice de destino '" << id_destino << "' não existe no grafo.\n";
        return {};
    }

    int indice_origem  = mapa_id_para_indice[id_origem];
    int indice_destino = mapa_id_para_indice[id_destino];

    // 2) Inicializa distâncias e antecessores
    const int INFINITO = numeric_limits<int>::max();
    vector<int> distancia(numero_vertices, INFINITO);
    vector<int> antecessor(numero_vertices, -1);
    distancia[indice_origem] = 0;

    // 3) Fila de prioridade de pares (distância_atual, índice_do_vertice)
    using Par = pair<int,int>;
    priority_queue<Par, vector<Par>, greater<Par>> fila_prioridade;
    fila_prioridade.emplace(0, indice_origem);

    while (!fila_prioridade.empty())
    {
        auto [dist_atual, vertice_atual] = fila_prioridade.top();
        fila_prioridade.pop();

        if (dist_atual > distancia[vertice_atual]) 
            continue;
        if (vertice_atual == indice_destino) 
            break;

        // Relaxa cada aresta de saída
        for (Aresta* aresta : lista_adj[vertice_atual]->get_arestas())
        {
            NoId id_vizinho = aresta->get_id_no_alvo();
            int indice_vizinho = mapa_id_para_indice[id_vizinho];
            int peso_aresta = get_ponderado_aresta() ? aresta->get_peso() : 1;

            int nova_dist = distancia[vertice_atual] + peso_aresta;
            if (nova_dist < distancia[indice_vizinho])
            {
                distancia[indice_vizinho] = nova_dist;
                antecessor[indice_vizinho] = vertice_atual;
                fila_prioridade.emplace(nova_dist, indice_vizinho);
            }
        }
    }

    // 4) Reconstrói o caminho de destino a origem
    if (distancia[indice_destino] == INFINITO)
    {
        cout << "Não existe caminho de '" << id_origem
             << "' até '" << id_destino << "'\n";
        return {};
    }

    vector<NoId> caminho;
    for (int atual = indice_destino; atual != -1; atual = antecessor[atual])
        caminho.push_back(lista_adj[atual]->get_id());
    reverse(caminho.begin(), caminho.end());

    return caminho;
}


// =======================================


// Gustavo - aux
unordered_map<int, NoId> Grafo::get_mapa_index_id() 
{
    unordered_map<int, NoId> mapa_index_id;

    for (int index = 0; index < ordem; index++) {
        NoId id = lista_adj[index]->get_id();
        mapa_index_id.insert(pair<int, NoId>(index, id));
    }

    return mapa_index_id;
}

// Gustavo - aux
unordered_map<NoId, int> Grafo::get_mapa_id_index() 
{

    unordered_map<NoId, int> mapa_id_index;

    for (int index = 0; index < ordem; index++) {
        NoId id = lista_adj[index]->get_id();
        mapa_id_index.insert(pair<NoId, int>(id, index));
    }

    return mapa_id_index;
}


// Gustavo
Grafo *Grafo::arvore_geradora_minima_prim(vector<NoId> ids_nos)
{
    cout << "Metodo nao implementado" << endl;
    return nullptr;
}


// =======================================
// F) KRUSKAL / AGM DE KRUSKAL
// =======================================

//Gabriel
Grafo *Grafo::arvore_geradora_minima_kruskal(vector<NoId> ids_nos)
{
    int m = ids_nos.size();
    if (m == 0)
    {
        cout << "Conjunto de nós vazio!!!" << endl;
        return nullptr;
    }

    // if(get_direcionado()){
    //     cout << "Grafo direcionado. Kruskal não implementado." << endl;
    //     return nullptr;
    // }


    // 1) Verifica se todos os IDs existem no grafo original
    unordered_map<NoId, int> mapa_id_para_indice;
    mapa_id_para_indice.reserve(m);
    for (int i = 0; i < m; ++i)
    {
        NoId id = ids_nos[i];
        // busca linear em lista_adj
        bool achou = false;
        for (No *no : lista_adj)
        {
            if (no->get_id() == id)
            {
                achou = true;
                break;
            }
        }
        if (!achou)
        {
            cout << "Vértice '" << id << "' não existe no grafo original.\n";
            return nullptr;
        }
        // mapeia para índice local [0..m-1]
        mapa_id_para_indice[id] = i;
    }

    // 2) Cria o grafo-árvore de saída e copia os nós
    Grafo *grafo_kruskal = new Grafo();
    grafo_kruskal->set_direcionado(false); // MST/AGM sempre não-direcionado
    grafo_kruskal->set_ponderado_aresta(get_ponderado_aresta());
    grafo_kruskal->set_ponderado_vertice(get_ponderado_vertice());
    for (NoId id : ids_nos)
    {
        No *orig = encontra_no_por_id(id);
        int peso_no = orig ? orig->get_peso() : 0;
        grafo_kruskal->adiciona_no(id, peso_no);
    }

    // 3) Coleta arestas internas sem duplicata (u<v)
    vector<Aresta *> all_edges;
    for (No *no : lista_adj)
    {
        NoId u = no->get_id();
        auto it_u = mapa_id_para_indice.find(u);
        if (it_u == mapa_id_para_indice.end())
            continue;
        for (Aresta *a : no->get_arestas())
        {
            NoId v = a->get_id_no_alvo();
            if (!mapa_id_para_indice.count(v))
                continue;
            // apenas uma direção para não-direcionado: exige u<v
            if (u < v)
            {
                all_edges.push_back(a);
            }
        }
    }

    // 4) Ordena por peso
    if (get_ponderado_aresta())
    {
        sort(all_edges.begin(), all_edges.end(),
             [](Aresta *a, Aresta *b)
             {
                 return a->get_peso() < b->get_peso();
             });
    }

    // 5) Union–Find
    vector<int> pai(m), rank(m, 0);
    for (int i = 0; i < m; ++i)
        pai[i] = i;
    function<int(int)> find = [&](int x)
    {
        return pai[x] == x ? x : pai[x] = find(pai[x]);
    };
    auto unite = [&](int x, int y)
    {
        x = find(x);
        y = find(y);
        if (x == y)
            return false;
        if (rank[x] < rank[y])
            swap(x, y);
        pai[y] = x;
        if (rank[x] == rank[y])
            ++rank[x];
        return true;
    };

    // 6) Kruskal: adicionar só arestas que unem componentes diferentes
    for (Aresta *a : all_edges)
    {
        NoId u = a->get_id_no_origem();
        NoId v = a->get_id_no_alvo();
        int pu = mapa_id_para_indice[u];
        int pv = mapa_id_para_indice[v];
        if (unite(pu, pv))
        {
            // insere no MST/AGM
            grafo_kruskal->adiciona_aresta(u, v, a->get_peso());
        }
    }

    return grafo_kruskal;
}

// =======================================

void Grafo::adiciona_aresta(NoId id_origem, NoId id_alvo, int peso = 0)
{
    No *no_origem = encontra_no_por_id(id_origem);
    if (!no_origem)
    {
        cerr << "Aviso: nó origem '" << id_origem << "' não existe." << endl;
        return;
    }
    // no_origem.adiciona_aresta já verifica duplicata dentro de No
    no_origem->adiciona_aresta(id_origem, id_alvo, peso);

    if (!get_direcionado())
    {
        No *no_destino = encontra_no_por_id(id_alvo);
        if (no_destino)
        {
            no_destino->adiciona_aresta(id_alvo, id_origem, peso);
        }
    }
}

// =======================================
// G) ÁRVORE DE CAMINHAMENTO (DFS)
// =======================================

//Gabriel
Grafo *Grafo::arvore_caminhamento_profundidade(NoId id_no)
{
    int n = lista_adj.size();
    if (n == 0)
    {
        cout << "Grafo vazio!!!" << endl;
        return nullptr;
    }

    // 1) Mapa ID→índice
    auto mapa_id_index = get_mapa_id_index(); // cria um mapa de NoId para índice
    auto it = mapa_id_index.find(id_no); // busca pelo ID do nó inicial
    if (it == mapa_id_index.end()) {
        cout << "Nó inicial '" << id_no << "' não encontrado." << endl;
        return nullptr;
    }
    int indice_inicio = it->second;

    // 2) Vetor de visitados e coletor de ponteiros de arestas
    vector<bool> visitado(n, false);
    vector<Aresta *> arestas_arvore;
    arestas_arvore.reserve(n - 1); // reserva espaço para arestas de árvore

    // 3) DFS recursivo que coleta as arestas de árvore
    cout << "Ordem de DFS a partir de '" << id_no << "': ";
  dfs_arvore_aux(indice_inicio, visitado, mapa_id_index, arestas_arvore);   
    cout << endl;

    // DEBUG: veja quantas arestas foram coletadas
  //  cout << "DEBUG: Coletadas " << arestas_arvore.size() << " arestas de árvore.\n";

    // 4) Montar o grafo-árvore a partir dessas arestas
    Grafo *arvore = new Grafo();
    arvore->set_direcionado(get_direcionado());
    arvore->set_ponderado_aresta(get_ponderado_aresta());
    arvore->set_ponderado_vertice(get_ponderado_vertice());

    // Copiar os nós
    for (No *no : lista_adj)
        arvore->adiciona_no(no->get_id(), no->get_peso());

    // Adicionar cada aresta de árvore, com peso
    for (Aresta *a : arestas_arvore)
    {
        NoId u = a->get_id_no_origem();
        NoId v = a->get_id_no_alvo();
        int peso = a->get_peso();
        arvore->adiciona_aresta(u, v, peso);
    }

    return arvore;
}

// Auxiliar privado
void Grafo::dfs_arvore_aux(int indice_no,
                           vector<bool> &visitado,
                           const unordered_map<NoId, int> &mapa_id_para_indice,
                           vector<Aresta *> &arestas_arvore)
{
    visitado[indice_no] = true;
    NoId id_u = lista_adj[indice_no]->get_id();
    cout << id_u << " ";

    for (Aresta *aresta : lista_adj[indice_no]->get_arestas()) // percorre arestas de saída
    {
        NoId id_v = aresta->get_id_no_alvo(); // ID do nó alvo da aresta
        auto it = mapa_id_para_indice.find(id_v);
        if (it == mapa_id_para_indice.end())  
            continue;
        int indice_v = it->second;
        if (!visitado[indice_v]) // se o nó alvo ainda não foi visitado
        {
            
            arestas_arvore.push_back(aresta); // adiciona a aresta de árvore
            dfs_arvore_aux(indice_v,
                           visitado,
                           mapa_id_para_indice,
                           arestas_arvore);
        }
    }
}


//// =======================================

void Grafo::exportar_vector_para_arquivo(const vector<NoId> &v, const string &nome_arquivo)
{
    ofstream arquivo(nome_arquivo);
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo para escrita: " << nome_arquivo << endl;
        return;
    }

    // Escreve o vetor no formato: id1,id2,id3,...
    for (size_t i = 0; i < v.size(); ++i)
    {
        arquivo << v[i];
        if (i + 1 < v.size())
        {
            arquivo << ",";
        }
    }
    arquivo << endl;

    arquivo.close();
    cout << "Vetor exportado em " << nome_arquivo << endl;
}

// exporta o grafo para arquivo no formato: id(peso vertice): -> id_alvo(peso aresta)
void Grafo::exportar_grafo_para_arquivo(Grafo *g, const string &nome_arquivo)
{
    ofstream arquivo(nome_arquivo);
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo para escrita: " << nome_arquivo << endl;
        return;
    }

    // Para cada nó do grafo
    for (No *no : g->get_lista_adj())
    {
        NoId id_u = no->get_id();
        int peso_u = no->get_peso();
        arquivo << id_u << "(" << peso_u << "):";

        // percorre as arestas de saída
        for (Aresta *a : no->get_arestas())
        {
            NoId id_v = a->get_id_no_alvo();
            int peso_av = a->get_peso();
            arquivo << " -> "
                    << id_v << "(" << peso_av << ")";
        }
        arquivo << endl; // nova linha após cada nó
    }

    arquivo.close();
    cout << "Grafo exportado em " << nome_arquivo << endl;
}

// exporta o grafo para arquivo no formado do csacademy
void Grafo::exportar_grafo_para_arquivo_csacademy(Grafo *g, const string &nome_arquivo)
{
    ofstream arquivo(nome_arquivo);
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo para escrita: " << nome_arquivo << endl;
        return;
    }

    // 1. Escreve flags na primeira linha
    arquivo << (g->get_direcionado() ? 1 : 0) << " "
            << (g->get_ponderado_aresta() ? 1 : 0) << " "
            << (g->get_ponderado_vertice() ? 1 : 0) << endl;

    // 2. Número de vértices
    int n = g->get_lista_adj().size();
    arquivo << n << endl;

    // 3. Lista de vértices
    for (No *no : g->get_lista_adj())
    {
        arquivo << no->get_id();
        if (g->get_ponderado_vertice())
        {
            arquivo << " " << no->get_peso();
        }
        arquivo << endl;
    }

    // 4. Lista de arestas
    // Arestas são escritas como id_u id_v [peso] (se ponderado)
    for (No *no : g->get_lista_adj())
    {
        NoId id_u = no->get_id();
        for (Aresta *a : no->get_arestas())
        {
            NoId id_v = a->get_id_no_alvo();

            // Em não direcionado, só escreve se id_u < id_v para não duplicar:
            bool escreve = true; // Se direcionado, escreve sempre
            if (!g->get_direcionado())
            {
                if (id_u > id_v) // Evita duplicação de arestas
                    escreve = false;
            }
            if (!escreve) // Se não direcionado e id_u > id_v, não escreve
                continue;
            arquivo << id_u << " " << id_v;
            if (g->get_ponderado_aresta())
            {
                arquivo << " " << a->get_peso();
            }
            arquivo << endl;
        }
    }

    // 5. FeNoId o arquivo
    arquivo.close();
    cout << "Grafo exportado em " << nome_arquivo << endl;
}

// Busca linear por ID
 No * Grafo::encontra_no_por_id(NoId id) const {

     for (No *no : lista_adj)
        {
            if (no->get_id() == id)
                return no;
        }
        return nullptr;
}

// Gustavo
vector<NoId> Grafo::vertices_de_articulacao()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

// ======================== FLOYD  ========================

// Gustavo - aux
void Grafo::calcular_matrizes_floyd(vector<vector<int>>& dist, vector<vector<int>>& prox) 
{
    if (!in_ponderado_aresta)
        throw new runtime_error("Grafo precisa ser ponderado nas arestas.");

    // Mapeamento de dois sentidos entre NoId e índice da matriz

    auto mapa_index_id = get_mapa_id_index();
    auto mapa_id_index = get_mapa_id_index();

    for (int index = 0; index < ordem; index++) {
        NoId id = lista_adj[index]->get_id();
        mapa_index_id.insert(pair<int, NoId>(index, id));
        mapa_id_index.insert(pair<NoId, int>(id, index));
    }

    // INICIALIZAÇÃO

    // Matriz de distância - Preencher tudo com infinito
    int INFINITO = numeric_limits<int>::max();
    dist.assign(ordem, vector<int>(ordem, INFINITO));

    // Matriz de próximo - Preencher com -1
    prox.assign(ordem, vector<int>(ordem, -1));
    
    // Preencher distância de vizinhos com o peso da aresta
    for (int i = 0; i < ordem; i++) {

        // Diagonal da matriz distância igual a 0
        dist[i][i] = 0;

        No *no_origem = lista_adj[i];
        vector<Aresta*> arestas = no_origem->get_arestas();

        for (Aresta* aresta : arestas) {

            int j = mapa_id_index[aresta->id_no_alvo];
            dist[i][j] = aresta->get_peso();
            prox[i][j] = j;
        }
    }

    // ATUALIZAÇÃO

    // Considerar o caminho passando por intermediário k
    for (int k = 0; k < ordem; k++) {

        // Para cada par de vértices i, j
        for (int i = 0; i < ordem; i++) {
            for (int j = 0; j < ordem; j++) {

                if (dist[i][k] != INFINITO && dist[k][j] != INFINITO) {

                    int atual = dist[i][j];
                    int alternativo = dist[i][k] + dist[k][j];
                
                    if (alternativo < atual) {

                        dist[i][j] = alternativo;
                        prox[i][j] = prox[i][k];
                    }
                }
            }
        }
    }

    for (int i=0; i<ordem; i++) {
        if (dist[i][i] < 0) {
            throw new runtime_error("Resultados inválidos para grafo com ciclo negativo");
        }
    }
}

// Gustavo
vector<NoId> Grafo::caminho_minimo_floyd(NoId id_no_origem, NoId id_no_alvo)
{
    // TODO: Verificar por ciclos negativos?

    int INFINITO = numeric_limits<int>::max();
    
    auto mapa_index_id = get_mapa_index_id();
    auto mapa_id_index = get_mapa_id_index();
    
    vector<vector<int>> dist;
    vector<vector<int>> prox;
    
    calcular_matrizes_floyd(dist, prox);

    // RECUPERAR CAMINHO 
    vector<NoId> caminho = {};

    int index_no_atual = mapa_id_index[id_no_origem];
    int index_no_alvo = mapa_id_index[id_no_alvo];

    // Verificar caminho impossível
    if (dist[index_no_atual][index_no_alvo] == INFINITO 
        || prox[index_no_atual][index_no_alvo] == -1)
        return {};

    // Enquanto não chegar no destino, adicionar os nós do caminho no vector
    while (index_no_atual != index_no_alvo) {
        caminho.push_back(mapa_index_id[index_no_atual]);
        index_no_atual = prox[index_no_atual][index_no_alvo];
    }

    // Adicionando nó alvo
    caminho.push_back(mapa_index_id[index_no_alvo]);

    return caminho;
}

// h1 - Gustavo
int Grafo::get_raio()
{
    auto excentricidades = get_excentricidades();

    auto it_raio = min_element(excentricidades.begin(), excentricidades.end());

    if (it_raio == excentricidades.end())
        throw new runtime_error("Raio inexistente");

    return *it_raio;
}

// h2 - Gustavo
int Grafo::get_diametro()
{
    auto excentricidades = get_excentricidades();

    auto it_diametro = max_element(excentricidades.begin(), excentricidades.end());

    if (it_diametro == excentricidades.end())
        throw new runtime_error("Raio inexistente");

    return *it_diametro;
}

// h3 - Gustavo
vector<NoId> Grafo::get_centro()
{
    auto excentricidades = get_excentricidades();
    auto mapa_index_id = get_mapa_index_id();
    vector<NoId> centro;
    int raio = get_raio();

    for (int i=0; i<ordem; i++)
        if (excentricidades[i] == raio)
            centro.push_back(mapa_index_id[i]);
    
    return centro;
}

// h4 - Gustavo
vector<NoId> Grafo::get_periferia()
{
    auto excentricidades = get_excentricidades();
    auto mapa_index_id = get_mapa_index_id();
    vector<NoId> centro;
    int diametro = get_diametro();

    for (int i=0; i<ordem; i++)
        if (excentricidades[i] == diametro)
            centro.push_back(mapa_index_id[i]);
    
    return centro;
}

// h auxiliar - Gustavo
vector<int> Grafo::get_excentricidades() 
{
    if (!in_ponderado_aresta)
        throw new runtime_error("Grafo deve ser ponderado nas arestas");

    vector<vector<int>> dist;
    vector<vector<int>> prox;

    calcular_matrizes_floyd(dist, prox);

    int MENOS_INFINITO = numeric_limits<int>::min();

    vector<int> excentricidades(ordem);

    for (int i=0; i<ordem; i++) {

        int excentricidade = MENOS_INFINITO;

        for (int j=0; j<ordem; j++) {

            if (i == j)
                continue;

            if (dist[i][j] > excentricidade)
                excentricidade = dist[i][j];
        }
        
        excentricidades[i] = excentricidade;
    }
        
    return excentricidades;
}
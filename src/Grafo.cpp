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

void Grafo::set_direcionado(int direcao) {in_direcionado = direcao;}
void Grafo::set_ponderado_aresta(int ponderado) {in_ponderado_aresta = ponderado;}
void Grafo::set_ponderado_vertice(int ponderado) {in_ponderado_vertice = ponderado;}
void Grafo::set_ordem(int ordem_grafo) {ordem = ordem_grafo;}
int Grafo::get_direcionado() {return in_direcionado;}
int Grafo::get_ponderado_aresta() {return in_ponderado_aresta;}
int Grafo::get_ponderado_vertice() {return in_ponderado_vertice;}
int Grafo::get_ordem() {return ordem;}
vector<No*> Grafo::get_lista_adj() {return lista_adj;}

void Grafo::adiciona_no(NoId id_no, int peso)
{
    No *no = new No();
    no->set_id(id_no);

    if (in_ponderado_vertice)
    {
        no->set_peso(peso);
    }
    lista_adj.push_back(no);
    ordem++;
}

void Grafo::adiciona_no(No* no)
{
    lista_adj.push_back(no);
    ordem++;
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

vector<NoId> Grafo::fecho_transitivo_direto(NoId id_no)
{
    // Método para calcular o fecho transitivo direto de um nó em um grafo
    int n = lista_adj.size();
    vector<NoId> resultado;
    if (n == 0)
        return resultado;

    if (get_direcionado())
    {

        // 1. Mapear ID -> índice
        unordered_map<NoId, int> mapa_id_para_indice;
        mapa_id_para_indice.reserve(n);
        for (int i = 0; i < n; ++i)
        {
            mapa_id_para_indice[lista_adj[i]->get_id()] = i;
        }
        auto it = mapa_id_para_indice.find(id_no);
        if (it == mapa_id_para_indice.end())
        {
            // vértice não existe
            return resultado;
        }
        int indice_inicio = it->second;

        // 2. Vetor de marcados (visitados) inicializado com 0
        vector<NoId> marcado(n, 0);
        // Marca o inicial para não reapontar a si mesmo
        marcado[indice_inicio] = 1;

        // 3. Chama DFS recursivo para ver todos alcançáveis
        dfs_fecho_transitivo_direto(indice_inicio, marcado, resultado, mapa_id_para_indice);

        // 4. Retorna vetor de IDs alcançados (ordem de descoberta)
        cout << "Fecho transitivo a partir de '" << id_no << "': ";
        imprime_vector(resultado);
        return resultado;
    }
    else
    {
        cout << "Grafo não direcionado. Fecho transitivo direto não implementado." << endl;
        return {};
    }
}

void Grafo::dfs_fecho_transitivo_direto(int indice_no,
                                        vector<NoId> &marcado,
                                        vector<NoId> &resultado,
                                        const unordered_map<NoId, int> &mapa_id_para_indice)
{
    // Para cada aresta de saída de lista_adjacencia_[indice_no]
    for (Aresta *aresta : lista_adj[indice_no]->get_arestas())
    {
        NoId id_vizinho = aresta->get_id_no_alvo();
        auto it = mapa_id_para_indice.find(id_vizinho);
        if (it == mapa_id_para_indice.end())
            continue;
        int indice_vizinho = it->second;
        if (!marcado[indice_vizinho])
        {
            marcado[indice_vizinho] = 1;
            resultado.push_back(id_vizinho);
            dfs_fecho_transitivo_direto(indice_vizinho, marcado, resultado, mapa_id_para_indice);
        }
    }
}


vector<NoId> Grafo::caminho_minimo_dijkstra(NoId id_no_a, NoId id_no_b)
{
    int n = lista_adj.size();
    if (n == 0)
    {
        cout << "Grafo vazio!!!" << endl;
        return {};
    }
    // 1) Mapear ID → índice
    unordered_map<NoId, int> mapa_id_para_indice;
    mapa_id_para_indice.reserve(n);
    for (int i = 0; i < n; ++i)
        mapa_id_para_indice[lista_adj[i]->get_id()] = i;

    // verifica nós válidos (se existem no grafo)
    if (!mapa_id_para_indice.count(id_no_a))
    {
        cout << "Vértice de origem '" << id_no_a << "' não existe no grafo.\n";
        return {};
    }
    if (!mapa_id_para_indice.count(id_no_b))
    {
        cout << "Vértice de destino '" << id_no_b << "' não existe no grafo.\n";
        return {};
    }

    int src = mapa_id_para_indice[id_no_a];
    int dst = mapa_id_para_indice[id_no_b];

    // 2) Distâncias e predecessores
    const int INF = numeric_limits<int>::max();
    vector<int> dist(n, INF);
    vector<int> prev(n, -1);
    dist[src] = 0;

    // 3) Min-heap de (dist, índice)
    using pii = pair<int, int>;
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.emplace(0, src);

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u])
            continue;
        if (u == dst)
            break; // já achou distância mínima até dst

        // relaxa todas as arestas de u
        for (Aresta *a : lista_adj[u]->get_arestas())
        {
            NoId id_v = a->get_id_no_alvo();
            int v = mapa_id_para_indice[id_v];
            int peso = get_ponderado_aresta() ? a->get_peso() : 1;
            if (dist[u] + peso < dist[v])
            {
                dist[v] = dist[u] + peso;
                prev[v] = u;
                pq.emplace(dist[v], v);
            }
        }
    }

    // 4) Reconstruir caminho
    vector<NoId> caminho;
    if (dist[dst] == INF)
    {
        // não existe caminho de A para B
        cout << "Não existe caminho de '" << id_no_a
             << "' até '" << id_no_b << endl;
        return {}; // não alcançável
    }
    for (int at = dst; at != -1; at = prev[at])
    {
        caminho.push_back(lista_adj[at]->get_id());
    }
    reverse(caminho.begin(), caminho.end());
    return caminho;
}

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

void Grafo::adiciona_aresta(NoId id_origem, NoId id_alvo, int peso)
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

Grafo *Grafo::arvore_caminhamento_profundidade(NoId id_no)
{
    int n = lista_adj.size();
    if (n == 0)
    {
        cout << "Grafo vazio; nada a percorrer." << endl;
        return nullptr;
    }

    // 1) Mapa ID→índice
    unordered_map<NoId, int> mapa_id_para_indice;
    mapa_id_para_indice.reserve(n);
    for (int i = 0; i < n; ++i)
        mapa_id_para_indice[lista_adj[i]->get_id()] = i;

    auto it = mapa_id_para_indice.find(id_no);
    if (it == mapa_id_para_indice.end())
    {
        cout << "Nó inicial '" << id_no << "' não encontrado." << endl;
        return nullptr;
    }
    int indice_inicio = it->second;

    // 2) Vetor de visitados e coletor de ponteiros de arestas
    vector<bool> visitado(n, false);
    vector<Aresta *> tree_edges;
    tree_edges.reserve(n - 1);

    // 3) DFS recursivo que coleta as arestas de árvore
    cout << "Ordem de DFS a partir de '" << id_no << "': ";
    dfs_arvore_aux(indice_inicio,
                   visitado,
                   mapa_id_para_indice,
                   tree_edges);
                   

    cout << endl;

    // DEBUG: veja quantas arestas foram coletadas
  //  cout << "DEBUG: Coletadas " << tree_edges.size() << " arestas de árvore.\n";

    // 4) Montar o grafo-árvore a partir dessas arestas
    Grafo *arvore = new Grafo();
    arvore->set_direcionado(get_direcionado());
    arvore->set_ponderado_aresta(get_ponderado_aresta());
    arvore->set_ponderado_vertice(get_ponderado_vertice());

    // Copiar os nós
    for (No *no : lista_adj)
        arvore->adiciona_no(no->get_id(), no->get_peso());

    // Adicionar cada aresta de árvore, com peso
    for (Aresta *a : tree_edges)
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
                           vector<Aresta *> &tree_edges)
{
    visitado[indice_no] = true;
    NoId id_u = lista_adj[indice_no]->get_id();
    cout << id_u << " ";

    for (Aresta *aresta : lista_adj[indice_no]->get_arestas())
    {
        NoId id_v = aresta->get_id_no_alvo();
        auto it = mapa_id_para_indice.find(id_v);
        if (it == mapa_id_para_indice.end())
            continue;
        int indice_v = it->second;
        if (!visitado[indice_v])
        {
            // guarda o ponteiro da aresta original
            tree_edges.push_back(aresta);
            dfs_arvore_aux(indice_v,
                           visitado,
                           mapa_id_para_indice,
                           tree_edges);
        }
    }
}

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

// exporta o grafo para arquivo no formato: id(peso): -> id_alvo(peso)
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

// [ ] - b - Gus - Fecho transitivo indireto (direcionado)
// [X] - d - Gus - Caminho mínimo Floyd
// [ ] - e - Gus - Árvore geradora mínima
// [X] - h - Gus - Raio, diâmetro, centro, periferia



// -=-=-=-=-=-=-=-=-=-=-=-=-=-=- GUSTAVO -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ========================= PRIM =========================

// Seja (u,v) a aresta de menor peso.
// F ¬ {(u,v)}
// Para i = 1,...,n faça
// Se c(i,u) < c(i,v) então prox(i) ¬ u
// Senão prox(i) ¬ v
// fim-para
// prox(u), prox(v) ¬ 0, contador ¬ 0
// Enquanto contador < n-2 faça
// Seja j tal que prox(j)10 e c(j,prox(j)) é mínimo.
// F ¬ F È {(j,prox(j))}
// prox(j) ¬ 0
// Para i = 1,...,n faça
// Se prox(i) 1 0 e c(i,prox(i)) > c(i,j) então
// prox(i) ¬ j

// fim-para
// contador ¬ contador + 1
// fim-enquanto

// BUG: deixei bug pra trás!? Quando adiciona a aresta, tem que adiciona nó alvo!? Onde foi?


// Gustavo
Grafo *Grafo::arvore_geradora_minima_prim(vector<NoId> ids_subconjunto)
{
    /* Obs: Grafos sem arestas ponderadas tem arestas de peso zero, funcionando 
    adequadamente com o algoritmo, embora o resultado tenham um significado um
    pouco diferente. */

    // TODO: Verificar conectividade?

    if (in_direcionado)
        throw runtime_error("Método de Prim é inválido para ávores direcionadas");

    // Verificar se o subconjunto não está vazio
    if (ids_subconjunto.empty())
        throw runtime_error("Subconjunto de nós não pode estar vazio");

    Grafo* grafo_agm = new Grafo();

    auto mapa_id_index = get_mapa_id_index();
    auto mapa_index_id = get_mapa_index_id();
    
    // Se certificar que todos os IDs existem no grafo
    for (NoId id : ids_subconjunto)
        if (mapa_id_index.find(id) == mapa_id_index.end())
            throw runtime_error("Nem todos os nós passados pertencem ao grafo");
        
    // Nós

    vector<No*> nos = lista_adj;
    vector<bool> sao_nos_incluidos(ordem, false); 
    vector<bool> sao_nos_faltantes(ordem, false);
    
    // Arestas

    vector<Aresta*> arestas;
    vector<bool> sao_arestas_incluidas;
    vector<bool> sao_arestas_possiveis;

    // Inicializando auxiliares

    for (NoId id_no : ids_subconjunto) {
        sao_nos_faltantes[mapa_id_index[id_no]] = true;
    }

    for (int index_no_origem=0; index_no_origem<nos.size(); index_no_origem++) {
        for (Aresta* aresta : nos[index_no_origem]->get_arestas()) {
            
            int index_no_alvo = mapa_id_index[aresta->get_id_no_alvo()];
            
            if (index_no_alvo >= index_no_origem)
                arestas.push_back(aresta);
        }
    }

    sao_arestas_incluidas.assign(arestas.size(), false);
    sao_arestas_possiveis.assign(arestas.size(), false);
    
    // Funções auxiliares

    function<void()> atualiza_arestas_possiveis = [&](){
        
        for (int i=0; i<arestas.size(); i++) {

            // Não elegíveis do lado de fora podem passar a ser
            if (!sao_arestas_possiveis[i] && !sao_arestas_incluidas[i]) {

                int index_origem = mapa_id_index[arestas[i]->get_id_no_origem()]; 
                int index_alvo = mapa_id_index[arestas[i]->get_id_no_alvo()];

                // Liga nós do subconjunto, dentro com fora
                if ((sao_nos_faltantes[index_origem] && sao_nos_incluidos[index_alvo]) 
                    || (sao_nos_incluidos[index_origem] && sao_nos_faltantes[index_alvo])
                ) {
                    sao_arestas_possiveis[i] = true;
                }
            }
            
        }
    };

    function<void(int)> adiciona_no_agm = [&](int no_index){
        
        No* no = nos[no_index];

        grafo_agm->adiciona_no(new No(no->get_id(), no->get_peso()));

        sao_nos_incluidos[no_index] = true;
        sao_nos_faltantes[no_index] = false;

        atualiza_arestas_possiveis();
    };

    function<void(int)> adiciona_aresta_e_no_agm = [&](int aresta_index){

        Aresta* aresta = arestas[aresta_index];
        
        int index_origem = mapa_id_index[aresta->get_id_no_origem()];
        int index_alvo = mapa_id_index[aresta->get_id_no_alvo()];
        
        int no_a_adicionar_index = -1;

        // Decidindo qual nó será adicionado
        if (sao_nos_incluidos[index_origem] && sao_nos_faltantes[index_alvo]) {

            no_a_adicionar_index = index_alvo;

        } else if (sao_nos_incluidos[index_alvo] && sao_nos_faltantes[index_origem]) {
            
            no_a_adicionar_index = index_origem;
        }

        if (no_a_adicionar_index == -1)
            throw runtime_error("Aresta não conecta nós válidos da AGM");

        // Adicionar o nó à AGM
        adiciona_no_agm(no_a_adicionar_index);
        
        // Adicionar aresta em ambas as direções
        No* no_origem = grafo_agm->encontra_no_por_id(mapa_index_id[index_origem]);
        No* no_alvo = grafo_agm->encontra_no_por_id(mapa_index_id[index_alvo]);
        
        if (!no_origem || !no_alvo)
            throw runtime_error("Nó(s) não encontrados na AGM");

        NoId id_no_origem = no_origem->get_id();
        NoId id_no_alvo = no_alvo->get_id();

        no_origem->adiciona_aresta(new Aresta(id_no_origem, id_no_alvo, aresta->get_peso()));
        no_alvo->adiciona_aresta(new Aresta(id_no_alvo, id_no_origem, aresta->get_peso()));
        
        sao_arestas_possiveis[aresta_index] = false;
        sao_arestas_incluidas[aresta_index] = true;
    };

    // Adicionando um nó qualquer do subconjunto

    auto it = find(sao_nos_faltantes.begin(), sao_nos_faltantes.end(), true);
    
    if (it == sao_nos_faltantes.end())
        throw runtime_error("Árvore AGM não inicializada");
    
    adiciona_no_agm(distance(sao_nos_faltantes.begin(), it));

    // ATUALIZAÇÕES

    while (true) {

        // Encontrar aresta de menor peso
        
        int index_menor_aresta = -1;

        for (int i=0; i<arestas.size(); i++) {
            if (sao_arestas_possiveis[i]) {

                if (index_menor_aresta == -1 
                    || arestas[i]->get_peso() 
                    < arestas[index_menor_aresta]->get_peso()
                ) {
                    index_menor_aresta = i;
                }
            }
        }
        
        if (index_menor_aresta == -1)
            throw runtime_error("Não foi encontrada aresta adequada.");

        // Adicionar aresta (também atualiza as arestas possíveis)

        adiciona_aresta_e_no_agm(index_menor_aresta);

        int nos_incluidos = count(
        
            sao_nos_incluidos.begin(), 
            sao_nos_incluidos.end(), 
            true
        );

        if (nos_incluidos == ids_subconjunto.size())
            break;
    }

    // TODO: Verificar por ciclos

    return grafo_agm;
}

// Cria novo grafo

// Adiciona um nó se for subconjunto
// Marca como dentro

// Verifica as arestas, verifi











// ========================= FECHO TRANSITIVO INDIRETO =========================

vector<NoId> Grafo::fecho_transitivo_indireto(NoId id_no) {

    Grafo* transposto = gera_grafo_transposto();
    
    vector<NoId> fecho = transposto->fecho_transitivo_direto(id_no);

    delete transposto;
    return fecho;
}

Grafo* Grafo::gera_grafo_transposto() {
    
    Grafo* transposto = new Grafo();

    for (No* no_original: lista_adj) {

        transposto->adiciona_no(new No(no_original->get_id(), no_original->get_peso()));

        for (Aresta* aresta : no_original->get_arestas()) {

            transposto->adiciona_aresta(aresta->get_id_no_alvo(), 
                                        aresta->get_id_no_origem(),
                                        aresta->get_peso());
        }
    }

    transposto->set_ordem(ordem);
    transposto->set_direcionado(in_direcionado);
    transposto->set_ponderado_aresta(in_ponderado_aresta);
    transposto->set_ponderado_vertice(in_ponderado_vertice);

    return transposto;
}

// =================================== FLOYD ===================================

// d
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

// d aux
void Grafo::calcular_matrizes_floyd(vector<vector<int>>& dist, vector<vector<int>>& prox) 
{
    if (!in_ponderado_aresta)
        throw runtime_error("Grafo precisa ser ponderado nas arestas.");

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
            throw runtime_error("Resultados inválidos para grafo com ciclo negativo");
        }
    }
}

// ================================== MÉTRICAS =================================

// h auxiliar
vector<int> Grafo::get_excentricidades() 
{
    if (!in_ponderado_aresta)
        throw runtime_error("Grafo deve ser ponderado nas arestas");

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

// h1
int Grafo::get_raio()
{
    auto excentricidades = get_excentricidades();

    auto it_raio = min_element(excentricidades.begin(), excentricidades.end());

    if (it_raio == excentricidades.end())
        throw runtime_error("Raio inexistente");

    return *it_raio;
}

// h2
int Grafo::get_diametro()
{
    auto excentricidades = get_excentricidades();

    auto it_diametro = max_element(excentricidades.begin(), excentricidades.end());

    if (it_diametro == excentricidades.end())
        throw runtime_error("Raio inexistente");

    return *it_diametro;
}

// h3
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

// h4
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

// ================================== EXTRA ====================================

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

        // cout << index << " " << ordem << endl;
        NoId id = lista_adj[index]->get_id();
        mapa_id_index.insert(pair<NoId, int>(id, index));
    }

    return mapa_id_index;
}
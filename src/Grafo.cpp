#include "Grafo.h"

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

vector<No *> Grafo::get_lista_adj()
{
    return lista_adj;
}

void Grafo::adiciona_no(char id_no, int peso)
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
    cout << "Grafo: " << endl;
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

void Grafo::imprime_vector(vector<char> v)
{
    if (v.size() == 0)
    {
        return;
    }
    else
    {
        for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i];
        if (i + 1 < v.size()) {
            cout << ",";
        }
    }
    cout << endl;
    }
}

vector<char> Grafo::fecho_transitivo_direto(char id_no)
{
    // Método para calcular o fecho transitivo direto de um nó em um grafo
    int n = lista_adj.size();
    vector<char> resultado;
    if (n == 0)
        return resultado;

    if (get_direcionado())
    {

        // 1. Mapear ID -> índice
        unordered_map<char, int> mapa_id_para_indice;
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
        vector<char> marcado(n, 0);
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
                                        vector<char> &marcado,
                                        vector<char> &resultado,
                                        const unordered_map<char, int> &mapa_id_para_indice)
{
    // Para cada aresta de saída de lista_adjacencia_[indice_no]
    for (Aresta *aresta : lista_adj[indice_no]->get_arestas())
    {
        char id_vizinho = aresta->get_id_no_alvo();
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

vector<char> Grafo::fecho_transitivo_indireto(char id_no)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b)
{
    int n = lista_adj.size();
    if (n == 0)
    {
        cout << "Grafo vazio!!!" << endl;
        return {};
    }
    // 1) Mapear ID → índice
    unordered_map<char, int> mapa_id_para_indice;
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
            char id_v = a->get_id_no_alvo();
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
    vector<char> caminho;
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

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

Grafo *Grafo::arvore_geradora_minima_prim(vector<char> ids_nos)
{
    cout << "Metodo nao implementado" << endl;
    return nullptr;
}

Grafo *Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos)
{
    int m = ids_nos.size();
    if (m == 0)
    {
        cout << "Conjunto de nós vazio. Nada a fazer.\n";
        return nullptr;
    }

    // 1) Verifica se todos os IDs existem no grafo original
    unordered_map<char, int> mapa_id_para_indice;
    mapa_id_para_indice.reserve(m);
    for (int i = 0; i < m; ++i)
    {
        char id = ids_nos[i];
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
    grafo_kruskal->set_direcionado(false); // MST sempre não-direcionado
    grafo_kruskal->set_ponderado_aresta(get_ponderado_aresta());
    grafo_kruskal->set_ponderado_vertice(get_ponderado_vertice());
    for (char id : ids_nos)
    {
        No *orig = encontra_no_por_id(id);
        int peso_no = orig ? orig->get_peso() : 0;
        grafo_kruskal->adiciona_no(id, peso_no);
    }

    // 3) Coleta arestas internas sem duplicata (u<v)
    vector<Aresta *> all_edges;
    for (No *no : lista_adj)
    {
        char u = no->get_id();
        auto it_u = mapa_id_para_indice.find(u);
        if (it_u == mapa_id_para_indice.end())
            continue;
        for (Aresta *a : no->get_arestas())
        {
            char v = a->get_id_no_alvo();
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
        char u = a->get_id_no_origem();
        char v = a->get_id_no_alvo();
        int pu = mapa_id_para_indice[u];
        int pv = mapa_id_para_indice[v];
        if (unite(pu, pv))
        {
            // insere no MST
            grafo_kruskal->adiciona_aresta(u, v, a->get_peso());
        }
    }

    return grafo_kruskal;
}

void Grafo::adiciona_aresta(char id_origem, char id_alvo, int peso = 0)
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

Grafo *Grafo::arvore_caminhamento_profundidade(char id_no)
{
    int n = lista_adj.size();
    if (n == 0)
    {
        cout << "Grafo vazio; nada a percorrer." << endl;
        return nullptr;
    }

    // 1) Mapa ID→índice
    unordered_map<char, int> mapa_id_para_indice;
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
    // Se houver componentes desconexas, cobre-as também
    for (int i = 0; i < n; ++i)
    {
        if (!visitado[i])
        {
            dfs_arvore_aux(i,
                           visitado,
                           mapa_id_para_indice,
                           tree_edges);
        }
    }
    cout << endl;

    // DEBUG: veja quantas arestas foram coletadas
    cout << "DEBUG: Coletadas " << tree_edges.size() << " arestas de árvore.\n";

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
        char u = a->get_id_no_origem();
        char v = a->get_id_no_alvo();
        int peso = a->get_peso();
        arvore->adiciona_aresta(u, v, peso);
    }

    return arvore;
}

// Auxiliar privado
void Grafo::dfs_arvore_aux(int indice_no,
                           vector<bool> &visitado,
                           const unordered_map<char, int> &mapa_id_para_indice,
                           vector<Aresta *> &tree_edges)
{
    visitado[indice_no] = true;
    char id_u = lista_adj[indice_no]->get_id();
    cout << id_u << " ";

    for (Aresta *aresta : lista_adj[indice_no]->get_arestas())
    {
        char id_v = aresta->get_id_no_alvo();
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

void Grafo::exportar_grafo_para_arquivo(Grafo *g, const string &nome_arquivo)
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
        char id_u = no->get_id();
        for (Aresta *a : no->get_arestas())
        {
            char id_v = a->get_id_no_alvo();

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

    // 5. Fechar o arquivo
    arquivo.close();
    cout << "Grafo exportado em " << nome_arquivo << endl;
}

int Grafo::raio()
{
    cout << "Metodo nao implementado" << endl;
    return 0;
}

int Grafo::diametro()
{
    cout << "Metodo nao implementado" << endl;
    return 0;
}

vector<char> Grafo::centro()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::periferia()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::vertices_de_articulacao()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

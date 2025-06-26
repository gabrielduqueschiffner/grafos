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

void Grafo::imprime_vector(vector<char> v){
cout << "[ ";
for (size_t i = 0; i < v.size(); ++i) {
    cout << "'" << v[i];
    if(v.size() > 0 && i != v.size() - 1) {
        cout << "' ,";
    }
   
}
cout << "' ]" << endl;
}

vector<char> Grafo::fecho_transitivo_direto(char id_no)
{
    // Método para calcular o fecho transitivo direto de um nó em um grafo
    int n = lista_adj.size();
    vector<char> resultado;
    if (n == 0) return resultado;

    if(get_direcionado()) {
       
   
    // 1. Mapear ID -> índice
    unordered_map<char,int> mapa_id_para_indice;
    mapa_id_para_indice.reserve(n);
    for (int i = 0; i < n; ++i) {
        mapa_id_para_indice[ lista_adj[i]->get_id() ] = i;
    }
    auto it = mapa_id_para_indice.find(id_no);
    if (it == mapa_id_para_indice.end()) {
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
    else {
        cout << "Grafo não direcionado. Fecho transitivo direto não implementado." << endl;
        return {}; 
    }

}

void Grafo::dfs_fecho_transitivo_direto(int indice_no,
                                     vector<char>& marcado,
                                     vector<char>& resultado,
                                     const unordered_map<char,int>& mapa_id_para_indice)
{
    // Para cada aresta de saída de lista_adjacencia_[indice_no]
    for (Aresta* aresta : lista_adj[indice_no]->get_arestas()) {
        char id_vizinho = aresta->get_id_no_alvo();
        auto it = mapa_id_para_indice.find(id_vizinho);
        if (it == mapa_id_para_indice.end()) continue;
        int indice_vizinho = it->second;
        if (!marcado[indice_vizinho]) {
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
    cout << "Metodo nao implementado" << endl;
    return {};
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
    cout << "Metodo nao implementado" << endl;
    return nullptr;
}

void Grafo::adiciona_aresta(char id_origem, char id_alvo, int peso = 0) {
    No* no_origem = encontra_no_por_id(id_origem);
    if (!no_origem) {
        cerr << "Aviso: nó origem '" << id_origem << "' não existe." <<endl;
        return;
    }
    // no_origem.adiciona_aresta já verifica duplicata dentro de No
    no_origem->adiciona_aresta(id_origem, id_alvo, peso);

    if (!get_direcionado()) {
        No* no_destino = encontra_no_por_id(id_alvo);
        if (no_destino) {
            no_destino->adiciona_aresta(id_alvo, id_origem, peso);
        }
    }
}


Grafo* Grafo::arvore_caminhamento_profundidade(char id_no) {
    int n = lista_adj.size();
    if (n == 0) {
        cout << "Grafo vazio; nada a percorrer." << endl;
        return nullptr;
    }
    // 1) Mapear ID → índice
    unordered_map<char,int> mapa_id_para_indice;
    mapa_id_para_indice.reserve(n);
    for (int i = 0; i < n; ++i) {
        mapa_id_para_indice[ lista_adj[i]->get_id() ] = i;
    }
    auto it = mapa_id_para_indice.find(id_no);
    if (it == mapa_id_para_indice.end()) {
        cout << "Nó inicial '" << id_no << "' não encontrado." << endl;
        return nullptr;
    }
    int indice_inicio = it->second;

    // 2) Vetor de marcados
    vector<bool> visitado(n, false);

    // 3) Coletor de tree-edges
    vector<pair<char,char>> tree_edges;

    // 4) DFS simples mas coletando arestas de árvore
    cout << "Ordem de DFS a partir de '" << id_no << "': ";
    dfs_arvore_aux(indice_inicio, visitado, mapa_id_para_indice, tree_edges);
    for (int i = 0; i < n; ++i) {
        if (!visitado[i]) {
            dfs_arvore_aux(i, visitado, mapa_id_para_indice, tree_edges);
        }
    }
    cout << endl;

    // 5) Montar grafo-árvore
    Grafo* arvore = new Grafo();
    arvore->set_direcionado(get_direcionado());
    arvore->set_ponderado_aresta(get_ponderado_aresta());
    arvore->set_ponderado_vertice(get_ponderado_vertice());

    // Adicionar nós
    for (No* no : lista_adj) {
        arvore->adiciona_no(no->get_id(), no->get_peso());
    }


    for (auto &par : tree_edges) {
    char u = par.first;
    char v = par.second;
    int peso = 0;
    // busca peso original no grafo fonte
    int idx_u = mapa_id_para_indice[u];  // use o mesmo mapa de antes
    for (Aresta* a : lista_adj[idx_u]->get_arestas()) {
        if (a->get_id_no_alvo() == v) {
            peso = a->get_peso();
            break;
        }
    }
    arvore->adiciona_aresta(u, v, peso);
}

    return arvore;
}

void Grafo::dfs_arvore_aux(int indice_no,
                           vector<bool>& visitado,
                           const unordered_map<char,int>& mapa_id_para_indice,
                           vector<pair<char,char>>& tree_edges)
{
    visitado[indice_no] = true;
    char id_u = lista_adj[indice_no]->get_id();
    cout << id_u << " ";

    for (Aresta* aresta : lista_adj[indice_no]->get_arestas()) {
        char id_v = aresta->get_id_no_alvo();
        auto it = mapa_id_para_indice.find(id_v);
        if (it == mapa_id_para_indice.end()) continue;
        int indice_v = it->second;
        if (!visitado[indice_v]) {
            // registra aresta de árvore antes de descer
            tree_edges.emplace_back(id_u, id_v);
            dfs_arvore_aux(indice_v, visitado, mapa_id_para_indice, tree_edges);
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
                if (id_u > id_v)  // Evita duplicação de arestas
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

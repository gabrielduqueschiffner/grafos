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
    dfs_transitivo_recursivo(indice_inicio, marcado, resultado, mapa_id_para_indice);

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

void Grafo::dfs_transitivo_recursivo(int indice_no,
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
            dfs_transitivo_recursivo(indice_vizinho, marcado, resultado, mapa_id_para_indice);
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

// Grafo *Grafo::arvore_caminhamento_profundidade(char id_no)
// {

//     int n = lista_adj.size();
//     if (n == 0)
//     {
//         cout << "Grafo vazio. Não é possível realizar arvore_caminhamento_profundidade." << endl;
//         return nullptr;
//     }

//     // 1. Construir mapa ID -> índice [0..n-1]
//     unordered_map<char, int> mapaIdParaIndice;
//     mapaIdParaIndice.reserve(n);
//     for (int i = 0; i < n; ++i)
//     {
//         mapaIdParaIndice[lista_adj[i]->get_id()] = i;
//     }

//     auto it = mapaIdParaIndice.find(id_no);
//     if (it == mapaIdParaIndice.end())
//     {
//         cout << "Nó inicial '" << id_no << "' não encontrado.\n";
//         return nullptr;
//     }
//     int idx_inicio = it->second;



//      // 3. Preparar cores e listas de arestas
//     vector<int> cor(n, WHITE);
//     vector<pair<int,int>> arestas_arvore;
//     vector<pair<int,int>> arestas_retorno;

//     // 2. Vetor de visitados, inicializado com 0 (falso)
//     vector<char> visitado(n, 0);

//     cout << "Iniciando DFS a partir do nó: " << id_no << "\n";
//     cout << "Caminho da busca em profundidade:\n";
//     // 3. Chamar DFS a partir do índice inicial
//     dfs_idx(idx_inicio, visitado, mapaIdParaIndice);

//     return nullptr;
// }

// void Grafo::dfs_idx(int idx, vector<char> &visitado,
//                     const unordered_map<char, int> &mapaIdParaIndice)
// {
//     visitado[idx] = 1;
//     char id_atual = lista_adj[idx]->get_id();

//     cout << id_atual << "->";

//     // Percorrer arestas de idx
//     for (Aresta *aresta : lista_adj[idx]->get_arestas())
//     {
//         char id_alvo = aresta->get_id_no_alvo();
//         auto it = mapaIdParaIndice.find(id_alvo);
//         if (it == mapaIdParaIndice.end())
//             continue; // nó alvo não existe
//         int idx_alvo = it->second;
//         if (!visitado[idx_alvo])
//         {
//             dfs_idx(idx_alvo, visitado, mapaIdParaIndice);
//         }
//     }
// }

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


// 2) DFS árvore: constrói e retorna grafo contendo só arestas de árvore, imprime também arestas de retorno
Grafo* Grafo::arvore_caminhamento_profundidade(char id_no) {
    int n = lista_adj.size();
    if (n == 0) {
        cout << "Grafo vazio; não é possível construir árvore DFS.\n";
        return nullptr;
    }
    // Mapear ID para índice
    unordered_map<char,int> mapa_id_para_indice;
    mapa_id_para_indice.reserve(n);
    for (int i = 0; i < n; ++i) {
        mapa_id_para_indice[ lista_adj[i]->get_id() ] = i;
    }
    auto it_inicio = mapa_id_para_indice.find(id_no);
    if (it_inicio == mapa_id_para_indice.end()) {
        cout << "Nó inicial '" << id_no << "' não encontrado.\n";
        return nullptr;
    }
    int indice_inicio = it_inicio->second;

    // Construir lista de adjacência em índices
    vector<vector<int>> adj_indices(n);
    for (int i = 0; i < n; ++i) {
        for (Aresta* aresta : lista_adj[i]->get_arestas()) {
            char id_viz = aresta->get_id_no_alvo();
            auto it2 = mapa_id_para_indice.find(id_viz);
            if (it2 == mapa_id_para_indice.end()) continue;
            adj_indices[i].push_back(it2->second);
        }
    }

    // Preparar cores e coleções de arestas
    vector<Cor> cor(n, BRANCO);
    vector<pair<int,int>> arestas_arvore;
    vector<pair<int,int>> arestas_retorno;

    cout << "Executando DFS para construir árvore a partir de '" << id_no << "'...\n";
    dfs_arvore_recursivo(indice_inicio, -1, cor, adj_indices, arestas_arvore, arestas_retorno);
    // Cobrir componentes desconexas
    for (int i = 0; i < n; ++i) {
        if (cor[i] == BRANCO) {
            dfs_arvore_recursivo(i, -1, cor, adj_indices, arestas_arvore, arestas_retorno);
        }
    }

    // Montar grafo-árvore para retornar
    Grafo* arvore = new Grafo();
    arvore->set_direcionado(get_direcionado());
    arvore->set_ponderado_aresta(get_ponderado_aresta());
    arvore->set_ponderado_vertice(get_ponderado_vertice());
    // Adicionar nós
    for (No* no : lista_adj) {
        arvore->adiciona_no(no->get_id(), no->get_peso());
    }
    // Adicionar apenas arestas de árvore
    for (auto& par : arestas_arvore) {
        int u = par.first;
        int v = par.second;
        char id_u = lista_adj[u]->get_id();
        char id_v = lista_adj[v]->get_id();
        if (get_ponderado_aresta()) {
            // buscar peso original
            int peso = 0;
            for (Aresta* a : lista_adj[u]->get_arestas()) {
                if (a->get_id_no_alvo() == id_v) {
                    peso = a->get_peso();
                    break;
                }
            }
            arvore->adiciona_aresta(id_u, id_v, peso);
            if (!get_direcionado()) {
                arvore->adiciona_aresta(id_v, id_u, peso);
            }
        } else {
            arvore->adiciona_aresta(id_u, id_v,0);
            if (!get_direcionado()) {
                arvore->adiciona_aresta(id_v, id_u,0);
            }
        }
    }

    // Imprimir arestas de árvore
    cout << "Arestas de árvore (tree edges):\n";
    for (auto& par : arestas_arvore) {
        char u = lista_adj[par.first]->get_id();
        char v = lista_adj[par.second]->get_id();
        cout << "  " << u << " -> " << v << "\n";
    }
    // Imprimir arestas de retorno
    cout << "Arestas de retorno (back edges):\n";
    for (auto& par : arestas_retorno) {
        char u = lista_adj[par.first]->get_id();
        char v = lista_adj[par.second]->get_id();
        cout << "  " << u << " -> " << v << "\n";
    }

    // (Opcional) imprimir hierarquia da árvore
    vector<vector<int>> filhos(n);
    for (auto& par : arestas_arvore) {
        filhos[par.first].push_back(par.second);
    }
    function<void(int,int)> imprime_hierarquia = [&](int idx, int nivel) {
        cout << string(nivel*2, ' ') << lista_adj[idx]->get_id() << "\n";
        for (int filho : filhos[idx]) {
            imprime_hierarquia(filho, nivel+1);
        }
    };
    cout << "Árvore de DFS hierárquica:\n";
    imprime_hierarquia(indice_inicio, 0);

    return arvore;
}

void Grafo::dfs_arvore_recursivo(int indice_no, int indice_pai,
                                 vector<Cor>& cor,
                                 const vector<vector<int>>& adj_indices,
                                 vector<pair<int,int>>& arestas_arvore,
                                 vector<pair<int,int>>& arestas_retorno)
{
    cor[indice_no] = CINZA;
    char id_u = lista_adj[indice_no]->get_id();
    cout << "Visitando nó: " << id_u << "\n";

    for (int indice_vizinho : adj_indices[indice_no]) {
        if (cor[indice_vizinho] == BRANCO) {
            // aresta de árvore
            arestas_arvore.emplace_back(indice_no, indice_vizinho);
            dfs_arvore_recursivo(indice_vizinho, indice_no, cor, adj_indices, arestas_arvore, arestas_retorno);
        }
        else if (cor[indice_vizinho] == CINZA) {
            // back edge: se direcionado, ou se não direcionado e não for aresta para o pai
            if (get_direcionado() || indice_vizinho != indice_pai) {
                arestas_retorno.emplace_back(indice_no, indice_vizinho);
            }
        }
        // se PRETO, ignora (forward/cross)
    }
    cor[indice_no] = PRETO;
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

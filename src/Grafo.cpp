#include "Grafo.h"
#include <unordered_set>

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

void Grafo::imprime_grafo() // ajustar como imprime o peso das arestas para melhor visualizacao
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

vector<char> Grafo::fecho_transitivo_direto(char id_no)
{
    cout << "Metodo nao implementado" << endl;
    return {};
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

Grafo *Grafo::arvore_caminhamento_profundidade(char id_no)
{

    int n = lista_adj.size();
    if (n == 0)
    {
        cout << "Grafo vazio. Não é possível realizar DFS." << endl;
        return nullptr;
    }

    // 1. Construir mapa ID -> índice [0..n-1]
    unordered_map<char, int> mapaIdParaIndice;
    mapaIdParaIndice.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        mapaIdParaIndice[lista_adj[i]->get_id()] = i;
    }

    auto it = mapaIdParaIndice.find(id_no);
    if (it == mapaIdParaIndice.end())
    {
        cout << "Nó inicial '" << id_no << "' não encontrado.\n";
        return nullptr;
    }
    int idx_inicio = it->second;

    // 2. Vetor de visitados, inicializado com 0 (falso)
    vector<char> visitado(n, 0);

    cout << "Iniciando DFS a partir do nó: " << id_no << "\n";
    cout << "Caminho da busca em profundidade:\n";
    // 3. Chamar DFS a partir do índice inicial
    dfs_idx(idx_inicio, visitado, mapaIdParaIndice);

    return nullptr;
}

void Grafo::dfs_idx(int idx, vector<char> &visitado,
                    const unordered_map<char, int> &mapaIdParaIndice)
{
    visitado[idx] = 1;
    char id_atual = lista_adj[idx]->get_id();

    cout << id_atual << "->";

    // Percorrer arestas de idx
    for (Aresta *aresta : lista_adj[idx]->get_arestas())
    {
        char id_alvo = aresta->get_id_no_alvo();
        auto it = mapaIdParaIndice.find(id_alvo);
        if (it == mapaIdParaIndice.end())
            continue; // nó alvo não existe
        int idx_alvo = it->second;
        if (!visitado[idx_alvo])
        {
            dfs_idx(idx_alvo, visitado, mapaIdParaIndice);
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
            bool escreve = true;
            if (!g->get_direcionado())
            {
                if (id_u > id_v)
                    escreve = false;
            }
            if (!escreve)
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

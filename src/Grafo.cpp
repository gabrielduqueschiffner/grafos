#include "Grafo.h"

Grafo::Grafo()
{
}

Grafo::~Grafo()
{
}

bool Grafo::set_direcionado(bool direcao)
{
    in_direcionado = direcao;
    return in_direcionado;
}
bool Grafo::set_ponderado_aresta(bool ponderado)
{
    in_ponderado_aresta = ponderado;
    return in_ponderado_aresta;
}
bool Grafo::set_ponderado_vertice(bool ponderado)
{
    in_ponderado_vertice = ponderado;
    return in_ponderado_vertice;
}
int Grafo::set_ordem(int ordem)
{
    ordem = ordem;
    return ordem;
}
bool Grafo::get_direcionado()
{
    return in_direcionado;
}
bool Grafo::get_ponderado_aresta()
{
    return in_ponderado_aresta;
}
bool Grafo::get_ponderado_vertice()
{
    return in_ponderado_vertice;
}

int Grafo::get_ordem()
{
    return ordem;
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
    cout << "Metodo nao implementado" << endl;
    return nullptr;
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

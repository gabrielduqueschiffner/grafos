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

void Grafo::imprime_grafo()
{
    cout << "Grafo: " << endl;
    for (No *no : lista_adj)
    {
        cout << "No: " << no->get_id() << " Peso: " << no->get_peso() << endl;
        for (Aresta *aresta : no->get_arestas())
        {
            cout << "  Aresta para No: " << aresta->get_id_no_alvo()
                 << " Peso: " << aresta->get_peso() << endl;
        }
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

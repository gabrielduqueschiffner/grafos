#include "Gerenciador.h"
#include "types.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
int main(int argc, char *argv[])
{

    Grafo *grafo = new Grafo();

    if (argc != 2)
    {
        cerr << "Uso correto do comando: " << argv[0] << " <arquivo_de_entrada>\n"
             << endl;
        return 1;
    }

    ifstream arquivoEntrada(argv[1]); // Lê o arquivo de entrada passado no comando

    if (!arquivoEntrada.is_open())
    {
        cerr << "Erro ao abrir o arquivo: " << argv[1] << endl;
        return 1;
    }

    bool ponderado_aresta, ponderado_vertice, direcionado;
    int ordem;
    string linha, palavra;

    // Lê a primeira linha

    if (getline(arquivoEntrada, linha))
    {
        istringstream iss(linha);
        iss >> direcionado >> ponderado_vertice >> ponderado_aresta;
        grafo->set_direcionado(direcionado);
        grafo->set_ponderado_vertice(ponderado_vertice);
        grafo->set_ponderado_aresta(ponderado_aresta);
    }
    else
    {
        cerr << "Erro ao ler a primeira linha.\n";
        return 1;
    }

    // Lê a segunda linha

    if (getline(arquivoEntrada, linha))
    {
        istringstream iss(linha);
        iss >> ordem;

        grafo->set_ordem(ordem);
    }
    else
    {
        std::cerr << "Erro ao ler a segunda linha.\n";
        return 1;
    }

    // Gerando a lista de nós do grafo e setando os seus pesos caso seja ponderado
    for (int i = 0; i < ordem; i++)
    {

        if (getline(arquivoEntrada, linha))
        {
            istringstream iss(linha);
            NoId id_no;
            int peso = 0;

            iss >> id_no >> peso; // Lê o id do nó e o peso, se houver

            grafo->adiciona_no(id_no, peso); // Método para adicionar um nó ao grafo
        }
        else
        {
            cerr << "Erro ao ler a linha " << i + 3 << ".\n";
            return 1;
        }
    }

    // Lê as arestas do grafo
    while (getline(arquivoEntrada, linha))
    {
        istringstream iss(linha);
        NoId id_no_origem, id_no_alvo;
        int peso = 0;
        iss >> id_no_origem >> id_no_alvo; // Lê os ids dos nós origem e alvo
        if (ponderado_aresta)
        {
            iss >> peso; // Lê o peso da aresta, se o grafo for ponderado
        }
        // Adiciona a aresta ao nó de origem
        for (No *no : grafo->get_lista_adj())
        {
            if (no->get_id() == id_no_origem)
            {
                no->adiciona_aresta(id_no_origem, id_no_alvo, peso);
                break; // Encontra o nó de origem e adiciona a aresta
            }
        }
        // Se o grafo for direcionado, não adiciona a aresta de volta
        if (!direcionado)
        {
            // Adiciona a aresta de volta ao nó de destino
            for (No *no : grafo->get_lista_adj())
            {
                if (no->get_id() == id_no_alvo)
                {
                    no->adiciona_aresta(id_no_alvo, id_no_origem, peso);
                    break; // Encontra o nó de destino e adiciona a aresta
                }
            }
        }
    }

    arquivoEntrada.close(); // Fecha o arquivo após a leitura

    // passar a impressão do grafo para o gerenciador depois

    grafo->imprime_grafo();

    // teste, apagar depois
    cout << "Direcionado: " << (grafo->get_direcionado() ? "sim" : "nao") << endl;
    cout << "Ponderado Aresta: " << (grafo->get_ponderado_aresta() ? "sim" : "nao") << endl;
    cout << "Ponderado Vertice: " << (grafo->get_ponderado_vertice() ? "sim" : "nao") << endl;
    cout << "Ordem: " << grafo->get_ordem() << endl;

    Gerenciador::comandos(grafo);

    return 0;
}
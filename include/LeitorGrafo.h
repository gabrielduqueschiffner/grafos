

#include <stdexcept>
#include <string>
#include "Aresta.h"
#include "Grafo.h"

class LeitorGrafo {

public:

    static Grafo* lerDeArquivo(string pathfile) {

        Grafo* grafo = new Grafo();

        // Lê o arquivo de entrada passado no comando
        ifstream arquivoEntrada(pathfile);  

        if (!arquivoEntrada.is_open()) 
            throw runtime_error("Erro ao abrir o arquivo: ");

        
        // Lê a primeira linha
        
        string linha, palavra;
        if (!getline(arquivoEntrada, linha))
            throw runtime_error("Erro ao ler a primeira linha");
    
        {
            bool ponderado_aresta, ponderado_vertice, direcionado;
            istringstream iss(linha);
            iss >> direcionado >> ponderado_aresta >> ponderado_vertice;
            
            grafo->set_direcionado(direcionado);
            grafo->set_ponderado_vertice(ponderado_vertice);
            grafo->set_ponderado_aresta(ponderado_aresta);
        }
    
        // Lê a segunda linha

        if (!getline(arquivoEntrada, linha)) 
            throw runtime_error("Erro ao ler a segunda linha");
        
        int qtd_nos; // ordem do grafo é gerenciada internamente

        {
            istringstream iss(linha);
            iss >> qtd_nos;

            if(qtd_nos == 0 || qtd_nos == 1) 
                throw runtime_error("Grafo nulo ou trivial não fazem sentido.");
        }

        // verifica se é grafo nulo ou trivial

        // Gerando a lista de nós do grafo e setando os seus pesos caso seja
        // ponderado
        for (int i = 0; i < qtd_nos; i++) { // ordem declarada no escopo "cobre" a da classe

            if (!getline(arquivoEntrada, linha)) 
                throw runtime_error("Erro ao ler a linha " + to_string(i + 3));
                
            istringstream iss(linha);
            NoId id_no;
            int peso = 0;

            iss >> id_no >> peso;  // Lê o id do nó e o peso, se houver

            grafo->adiciona_no(new No(id_no, peso));  // Método para adicionar um nó ao grafo 
        }

        // Lê as arestas do grafo
        while (getline(arquivoEntrada, linha)) {

            istringstream iss(linha);
            NoId id_no_origem, id_no_alvo;
            int peso = 0;

            iss >> id_no_origem >>
                id_no_alvo;  // Lê os ids dos nós origem e alvo
            if (grafo->get_ponderado_aresta()) {
                iss >> peso;  // Lê o peso da aresta, se o grafo for ponderado
            }

            // Adiciona a aresta ao nó de origem
            for (No *no : grafo->get_lista_adj()) {
                if (no->get_id() == id_no_origem) {
                    no->adiciona_aresta(new Aresta(id_no_origem, id_no_alvo, peso));
                    break;  // Encontra o nó de origem e adiciona a aresta
                }
            }

            // Se o grafo for direcionado, não adiciona a aresta de volta
            if (!grafo->get_direcionado()) {
                // Adiciona a aresta de volta ao nó de destino
                for (No *no : grafo->get_lista_adj()) {
                    if (no->get_id() == id_no_alvo) {
                        no->adiciona_aresta(new Aresta(id_no_alvo, id_no_origem, peso));
                        break;  // Encontra o nó de destino e adiciona a aresta
                    }
                }
            }
        }

        arquivoEntrada.close();  // Fecha o arquivo após a leitura

        return grafo;
    }
};
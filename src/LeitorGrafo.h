

#include <stdexcept>
#include <string>
#include "Grafo.h"

class LeitorGrafo {

public:

    static Grafo* lerDeArquivo(string pathfile) {

        Grafo* grafo = new Grafo();

        // Lê o arquivo de entrada passado no comando
        ifstream arquivoEntrada(pathfile);  

        if (!arquivoEntrada.is_open()) 
            throw new runtime_error("Erro ao abrir o arquivo: ");
        

        bool ponderado_aresta, ponderado_vertice, direcionado;
        int ordem;
        string linha, palavra;

        // Lê a primeira linha

        if (getline(arquivoEntrada, linha)) {
            istringstream iss(linha);
            iss >> direcionado >> ponderado_vertice >> ponderado_aresta;
            grafo->set_direcionado(direcionado);
            grafo->set_ponderado_vertice(ponderado_vertice);
            grafo->set_ponderado_aresta(ponderado_aresta);
        } else {
            throw new runtime_error("Erro ao ler a primeira linha");
        }

        // Lê a segunda linha

        if (getline(arquivoEntrada, linha)) {
            istringstream iss(linha);
            iss >> ordem;

            grafo->set_ordem(ordem);
        } else {
            throw new runtime_error("Erro ao ler a segunda linha");
        }

        // Gerando a lista de nós do grafo e setando os seus pesos caso seja
        // ponderado
        for (int i = 0; i < ordem; i++) {
            if (getline(arquivoEntrada, linha)) {
                istringstream iss(linha);
                NoId id_no;
                int peso = 0;

                iss >> id_no >> peso;  // Lê o id do nó e o peso, se houver

                grafo->adiciona_no(id_no, peso);  // Método para adicionar um nó ao grafo
            
            } else {
            
                throw new runtime_error("Erro ao ler a linha" + to_string(i + 3));
            }
        }

        // Lê as arestas do grafo
        while (getline(arquivoEntrada, linha)) {
            istringstream iss(linha);
            NoId id_no_origem, id_no_alvo;
            int peso = 0;
            iss >> id_no_origem >>
                id_no_alvo;  // Lê os ids dos nós origem e alvo
            if (ponderado_aresta) {
                iss >> peso;  // Lê o peso da aresta, se o grafo for ponderado
            }
            // Adiciona a aresta ao nó de origem
            for (No *no : grafo->get_lista_adj()) {
                if (no->get_id() == id_no_origem) {
                    no->adiciona_aresta(id_no_origem, id_no_alvo, peso);
                    break;  // Encontra o nó de origem e adiciona a aresta
                }
            }
            // Se o grafo for direcionado, não adiciona a aresta de volta
            if (!direcionado) {
                // Adiciona a aresta de volta ao nó de destino
                for (No *no : grafo->get_lista_adj()) {
                    if (no->get_id() == id_no_alvo) {
                        no->adiciona_aresta(id_no_alvo, id_no_origem, peso);
                        break;  // Encontra o nó de destino e adiciona a aresta
                    }
                }
            }
        }

        arquivoEntrada.close();  // Fecha o arquivo após a leitura

        return grafo;
    }
};
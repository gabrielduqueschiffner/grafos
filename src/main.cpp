#include <iostream>
#include <fstream>
#include <sstream>
#include "Gerenciador.h"
#include <string>

using namespace std;
int main(int argc, char *argv[])
{

    Grafo *grafo = new Grafo();

    

    if (argc != 2){
        cerr << "Uso correto do comando: " << argv[0] << " <arquivo_de_entrada>\n" << endl;
        return 1;
    }
   
    ifstream arquivoEntrada(argv[1]); //Lê o arquivo de entrada passado no comando


    if (!arquivoEntrada.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << argv[1] << endl;
        return 1;
    }

    bool ponderado_aresta, ponderado_vertice, direcionado;
    int ordem;
    string linha, palavra;
    


//Lê a primeira linha

if(getline(arquivoEntrada, linha)) {
    istringstream iss(linha);
    iss >> direcionado >> ponderado_vertice >> ponderado_aresta;
    grafo->set_direcionado(direcionado);
    grafo->set_ponderado_vertice(ponderado_vertice);
    grafo->set_ponderado_aresta(ponderado_aresta);

} else {
    std::cerr << "Erro ao ler a primeira linha.\n";
    return 1;
}

//Lê a segunda linha 

if(getline(arquivoEntrada, linha)) {
    istringstream iss(linha);
    iss >> ordem;
    cout << "Ordem do grafo: " << ordem << endl;
    grafo->set_ordem(ordem);
} else {
    std::cerr << "Erro ao ler a segunda linha.\n";
    return 1;
}

// Gerando a lista de nós do grafo e setando os seus pesos caso seja ponderado
for(int i=0; i < ordem; i++) {

    if(getline(arquivoEntrada, linha)) {
        istringstream iss(linha);
        char id_no;
        int peso = 0;

        iss >> id_no >> peso; // Lê o id do nó e o peso, se houver

        grafo->adiciona_no(id_no, peso); // Método para adicionar um nó ao grafo
        
       
    } else {
        std::cerr << "Erro ao ler a linha " << i + 3 << ".\n";
        return 1;
    }

}



    arquivoEntrada.close(); // Fecha o arquivo após a leitura

 
//passar a impressão do grafo para o gerenciador depois

  grafo->imprime_grafo();

  //teste, apagar depois
  cout << "Direcionado: " << (grafo->get_direcionado() ? "sim" : "nao") << endl;
    cout << "Ponderado Aresta: " << (grafo->get_ponderado_aresta() ? "sim" : "nao" ) << endl;
    cout << "Ponderado Vertice: " << (grafo->get_ponderado_vertice() ? "sim" : "nao") << endl;
    cout << "Ordem: " << grafo->get_ordem() << endl;
    
    Gerenciador::comandos(grafo);

    return 0;
}


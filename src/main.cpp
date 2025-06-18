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
    istringstream iss(linha);


//Lê a primeira linha

if(getline(arquivoEntrada, linha)) {
    
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
        int peso;

        iss >> id_no;

        grafo->adiciona_no(id_no); // Método para adicionar um nó ao grafo
        if(grafo->get_ponderado_vertice() ==true) {
           
            iss >> peso;
            grafo->set_peso(id_no, peso); // Método para definir o peso do nó
        } else
        
        grafo->set_peso( id_no, 1); // Defina o peso do nó como necessário
    } else {
        std::cerr << "Erro ao ler a linha " << i + 3 << ".\n";
        return 1;
    }

}

    while (getline(arquivoEntrada, linha)) {

        // Exemplo: grafo->adicionar_aresta(linha);
        while (iss >> palavra)
        {
            if(linha == "1") {
                  
                grafo->set_direcionado(true);
               
            } else if(linha == '2') {
                grafo->set_ordem();
            } else if(linha == '3') {
                direcionado = true;
           
        }
        
        cout << "Linha lida: " << linha << endl; // Apenas para demonstração
    }

    arquivoEntrada.close(); // Fecha o arquivo após a leitura

    // Configurações iniciais do grafo
    grafo->set_direcionado(false);
    grafo->set_ponderado_aresta(false);
    grafo->set_ponderado_vertice(false);
    grafo->set_ordem(0); // Defina a ordem conforme necessário
 
 


    Gerenciador::comandos(grafo);

    return 0;
}
}

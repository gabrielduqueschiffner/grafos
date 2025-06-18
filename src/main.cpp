#include <iostream>
#include <fstream>
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


    string linha;



    Gerenciador::comandos(grafo);

    return 0;
}

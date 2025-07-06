#include <algorithm>
#include <string>
#include "../include/Gerenciador.h"
#include "../include/types.h"
#include "../include/Gerenciador.h"

Gerenciador::Gerenciador(Grafo* grafo) {
    
    this->grafo = grafo;

    for (No* no : grafo->get_lista_adj())
        this->ids_nos.push_back(no->get_id());
}

void Gerenciador::executar_menu() {

    cout <<

        "\n => Digite uma das opcoes abaixo e pressione enter:"
        "\n (a) Fecho transitivo direto de um no"
        "\n (b) Fecho transitivo indireto de um no"
        "\n (c) Caminho minimo (Djikstra)"
        "\n (d) Caminho minimo (Floyd)"
        "\n (e) Arvore Geradora Minima (Algoritmo de Prim)"
        "\n (f) Arvore Geradora Minima (Algoritmo de Kruskal)"
        "\n (g) Arvore de caminhamento em profundidade"
        "\n (h) Raio, diametro, centro e periferia do grafo"
        "\n (0) Sair;"

    << endl;

    char resp;
    cin >> resp;

    // TODO: Resetar resultados
    vector<NoId> nos_resultado;
    Grafo* grafo_resultado;

    switch (resp) {
        
        case 'a': {
            nos_resultado = grafo->fecho_transitivo_direto(get_id_entrada());
            imprime_vector(nos_resultado);
            break;
        }

        case 'b':{
            nos_resultado = grafo->fecho_transitivo_indireto(get_id_entrada());
            imprime_vector(nos_resultado);
            break;
        }

        case 'c': {
            nos_resultado = grafo->caminho_minimo_dijkstra(get_id_entrada(1), get_id_entrada(2));
            imprime_vector(nos_resultado);
            break;
        }

        case 'd': {
            nos_resultado = grafo->caminho_minimo_floyd(get_id_entrada(1), get_id_entrada(2));
            imprime_vector(nos_resultado);
            break;
        }

        case 'e': {

            grafo_resultado = grafo->arvore_geradora_minima_prim(get_conjunto_ids());
            grafo_resultado->imprime_grafo();
            break;
        }

        case 'f': {
            grafo_resultado = grafo->arvore_geradora_minima_kruskal(get_conjunto_ids());
            grafo_resultado->imprime_grafo();
            break;
        }

        case 'g': {

            grafo_resultado = grafo->arvore_caminhamento_profundidade(get_id_entrada());
            grafo_resultado->imprime_grafo();
            break;
        }

        case 'h': {

            // TODO: Como escrever esses resultados em arquivo?

            cout << "Raio: " + to_string(grafo->get_raio()) << endl;
            cout << "Centro: "; grafo->get_centro();
            cout << "Diâmetro : " + to_string(grafo->get_diametro()) << endl;
            cout << "Periferia: "; grafo->get_periferia();
            break;
        }

        case '0': {
            return;
        }

        default: {

            cout << "Opção inválida" << endl << endl;
        }
    }

    // Verificar qual dos dados foi escrito, e oferecer escrita

    executar_menu();
}

bool Gerenciador::no_existe(NoId id_no) {

    return find(ids_nos.begin(), ids_nos.end(), id_no) != ids_nos.end();
}

NoId Gerenciador::get_id_entrada(int i) {
    
    NoId id_no;
    
    do {
        
        cout << "Digite um ID válido para o nó " + (i == -1 ? "" : to_string(i)) + ": ";
        cin >> id_no;

    } while (!no_existe(id_no));

    return id_no;
}

vector<NoId> Gerenciador::get_conjunto_ids(int tam) {

    vector<NoId> input_ids = {};

    for (int i=0; i<tam; i++) {

        NoId id_no = get_id_entrada(i);

        if (find(input_ids.begin(), input_ids.end(),id_no) != input_ids.end()) { 
            
            cout << "Valor repetido." << endl;
            i--;
            continue;
        }

        input_ids.push_back(id_no);
    }

    cout << "IDs inseridos: ";
    for (NoId id_no : input_ids)
        cout << id_no << " ";
    cout << endl;

    return input_ids;
}

vector<NoId> Gerenciador::get_conjunto_ids() {

    cout << "Quantidade de nós a inserir: ";
    int tam;
    cin >> tam;
    cout << endl;

    return get_conjunto_ids(tam);
}

bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo) {

    cout<<"Imprimir em arquivo externo? ("<<nome_arquivo<<")"<<endl;
    cout<<"(1) Sim;"<<endl;
    cout<<"(2) Nao."<<endl;
    int resp;
    cin>>resp;
    cout<<endl;

    switch (resp) {
        case 1:
            return true;
        case 2:
            return false;
        default:
            cout<<"Resposta invalida"<<endl;
            return pergunta_imprimir_arquivo(nome_arquivo);
    }
}

void Gerenciador::imprime_vector(vector<NoId> v) {

    if (v.size() == 0) {
        
        return;

    } else {

        for (size_t i = 0; i < v.size(); ++i) {
            
            cout << v[i];

            if (i + 1 < v.size()) {
                cout << ",";
            }
        }
        cout << endl;
    }
}
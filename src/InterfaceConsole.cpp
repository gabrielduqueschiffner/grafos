#include "../include/InterfaceConsole.h"
#include "../include/types.h"

#include <ostream>

InterfaceConsole::InterfaceConsole(Grafo* grafo) {
    
    this->grafo = grafo;

    for (No* no : grafo->get_lista_adj())
        this->ids_nos.push_back(no->get_id());
}

void InterfaceConsole::executar_menu() {

    cout <<

        "\n => Digite uma das opcoes abaixo e pressione enter:\n"
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

    cout << endl << "=> Sua opção: ";
    char resp;
    cin >> resp;
    cout << endl;

    


    // TODO: Resetar resultados
    vector<NoId> nos_resultado = {'\0'};
    Grafo* grafo_resultado = nullptr;

    switch (resp) {
        
        case 'a': {
            nos_resultado = grafo->fecho_transitivo_direto(get_id_entrada());
            imprime_vector(nos_resultado);
             if(pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                grafo->exportar_vector_para_arquivo(nos_resultado, "fecho_trans_dir.txt");
                
              
            }
            break;
        }

        case 'b':{
            nos_resultado = grafo->fecho_transitivo_indireto(get_id_entrada());
            imprime_vector(nos_resultado);
             if(pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                grafo->exportar_vector_para_arquivo(nos_resultado, "fecho_trans_indir.txt");
                
              
             }
            break;
        }

        case 'c': {
            nos_resultado = grafo->caminho_minimo_dijkstra(get_id_entrada(1), get_id_entrada(0));
            imprime_vector(nos_resultado);
             if(pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                grafo->exportar_vector_para_arquivo(nos_resultado, "caminho_minimo_dijkstra.txt");
               
               
             }
            break;
        }

        case 'd': {
            nos_resultado = grafo->caminho_minimo_floyd(get_id_entrada(1), get_id_entrada(0));
            imprime_vector(nos_resultado);
             if(pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                grafo->exportar_vector_para_arquivo(nos_resultado, "caminho_minimo_floyd.txt");
                
               
             }
            break;
        }

        case 'e': {

            grafo_resultado = grafo->arvore_geradora_minima_prim(get_conjunto_ids());
            grafo_resultado->imprime_grafo();
             if(pergunta_imprimir_arquivo("agm_prim.txt")) {
                grafo_resultado->exportar_grafo_para_arquivo(grafo_resultado, "agm_prim.txt");
               

             }
            break;
        }

        case 'f': {
            grafo_resultado = grafo->arvore_geradora_minima_kruskal(get_conjunto_ids());
            grafo_resultado->imprime_grafo();
             if(pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                grafo_resultado->exportar_grafo_para_arquivo(grafo_resultado, "agm_kruskal.txt");
                

             }
            break;
        }

        case 'g': {

            grafo_resultado = grafo->arvore_caminhamento_profundidade(get_id_entrada());
            grafo_resultado->imprime_grafo();
             if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                grafo_resultado->exportar_grafo_para_arquivo(grafo_resultado, "arvore_caminhamento_profundidade.txt");
              
           
             }
            break;
        }

        case 'h': {

            cout << "Raio: " + to_string(grafo->get_raio()) << endl;
            cout << "Centro: "; imprime_vector(grafo->get_centro());
            cout << endl;
            cout << "Diâmetro: " + to_string(grafo->get_diametro()) << endl;
            cout << "Periferia: "; imprime_vector(grafo->get_periferia());
            cout << endl;
          
            break;
        }

        case '0': {
            return;
        }

        default: {

            cout << "Opção inválida" << endl << endl;
        }
    }

    // Imprimindo resultados

    // if (grafo_resultado)
    //     grafo_resultado->imprime_grafo();

    // if (nos_resultado.size() == 1 && nos_resultado[0] == '\0')
    //     imprime_vector(nos_resultado);

    // Retornando pro menu

    executar_menu();
}

bool InterfaceConsole::no_existe(NoId id_no) {

    return find(ids_nos.begin(), ids_nos.end(), id_no) != ids_nos.end();
}

NoId InterfaceConsole::get_id_entrada(int i) {
    
    NoId id_no;
    
    do {
        
        cout << "Digite um ID válido para o nó" + (i == -1 ? "" : " " + to_string(i)) + ": ";
        cin >> id_no;
        if(id_no == '\0') {
            cout << "ID inválido. Tente novamente." << endl;
            continue;
        }
        cout << endl;

    } while (!no_existe(id_no));

    return id_no;
}

vector<NoId> InterfaceConsole::get_conjunto_ids(int tam) {

    vector<NoId> input_ids = {};

    for (int i=0; i<tam; i++) {

        NoId id_no = get_id_entrada(i);

        if (find(input_ids.begin(), input_ids.end(), id_no) != input_ids.end()) { 
            
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

vector<NoId> InterfaceConsole::get_conjunto_ids() {

    cout << "Quantidade de nós a inserir: ";
    int tam;
    cin >> tam;
    cout << endl;

    return get_conjunto_ids(tam);
}

bool InterfaceConsole::pergunta_imprimir_arquivo(string nome_arquivo) {

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

void InterfaceConsole::imprime_vector(vector<NoId> v) {

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





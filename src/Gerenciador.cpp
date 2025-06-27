#include "Gerenciador.h"
#include "types.h"

void Gerenciador::iniciar_menu(Grafo *grafo) {

    cout <<

        "\n====================== MENU ======================="
        "\n"
        "\n (a) Fecho transitivo direto de um no"
        "\n (b) Fecho transitivo indireto de um no"
        "\n (c) Caminho minimo (Djikstra)"
        "\n (d) Caminho minimo (Floyd)"
        "\n (e) Arvore Geradora Minima (Algoritmo de Prim)"
        "\n (f) Arvore Geradora Minima (Algoritmo de Kruskal)"
        "\n (g) Arvore de caminhamento em profundidade"
        "\n (h) Raio, diametro, centro e periferia do grafo"
        "\n (0) Sair"
        "\n"
    
    << endl;

    char resp;
    cout << "Opção digitada: ";
    cin >> resp;
    cout << endl;

    switch (resp) {

        case 'a': {
            grafo->fecho_transitivo_direto(get_id_entrada());
            // imprime sequencia
            break;
        }

        case 'b': {
            grafo->fecho_transitivo_indireto(get_id_entrada());
            // imprime sequencia
            break;
        }

        case 'c': {
            grafo->caminho_minimo_dijkstra(get_id_entrada(), get_id_entrada());
            // imprime sequencia
            break;
        }

        case 'd': {
            grafo->caminho_minimo_floyd(get_id_entrada(), get_id_entrada());
            // imprime sequencia
            break;
        }
        
        case 'e': {
            grafo->arvore_geradora_minima_prim(
                get_subconjunto_ids(grafo));
            // imprime árvore
            break;
        }

        case 'f': {
            grafo->arvore_geradora_minima_kruskal(
                get_subconjunto_ids(grafo));
            // imprime árvore
            break;
        }

        case 'g': {
            grafo->arvore_caminhamento_profundidade(get_id_entrada());
            // imprime árvore
            break;
        }

        case 'h': {
            grafo->vertices_de_articulacao();
            // imprime sequencia
            break;
        }

        case '0': {
            return;
        }

        default: {
            cout << "Opção inválida" << endl;
        }
    }

    // Repetir
    iniciar_menu(grafo);
}

NoId Gerenciador::get_id_entrada() {
    NoId id;
    cout << "Digite o id de um no: ";
    cin >> id;
    return id;
}

vector<NoId> Gerenciador::get_subconjunto_ids(Grafo *grafo) {

    int tam = -1;

    while (tam <= 0 || tam > grafo->get_ordem()) {
        
        string in;
        cout << "Digite um tamanho válido para o subconjunto: ";
        cin >> in;

        // Tentar converter para número
        try {
            tam = stoi(in);
        } catch (...) {
            continue;
        }
    }

    vector<NoId> ids = {};

    // Até obter a quantidade desejada
    while ((int)ids.size() < tam) {

        NoId id_no = get_id_entrada();
        bool existe = false;

        // Verificar se é parte do grafo
        for (No *no : grafo->get_lista_adj()) {
            if (no->id == id_no) {
                existe = true;
                break;
            }
        }

        if (!existe) {

            // Ignorar entrada
            cout << "Vertice nao existe" << endl;
            continue;

        } else {

            // Verificar se já foi escolhido
            bool repetido =
                find(ids.begin(), ids.end(), id_no) != ids.end();
            if (repetido) {

                // Ignorar entrada
                cout << "Valor repetido" << endl;
                continue;

            } else {

                // Incluir no resultado
                ids.push_back(id_no);
            }
        }
    }

    return ids;
}

bool Gerenciador::perguntar_escrever_arquivo(string nome_arquivo) {

    cout << "Imprimir em arquivo externo? (" << nome_arquivo << ")" << endl;
    cout << "(1) Sim;" << endl;
    cout << "(2) Nao." << endl;

    int resp;
    cin >> resp;
    cout << endl;

    switch (resp) {
    case 1:
        return true;
    case 2:
        return false;
    default:
        cout << "Resposta invalida" << endl;
        return perguntar_escrever_arquivo(nome_arquivo);
    }
}
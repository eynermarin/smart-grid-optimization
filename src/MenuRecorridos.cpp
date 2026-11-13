#include "MenuRecorridos.h"
#include "Recorridos.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

bool leerOpcion(int& opcion) {
    string texto;

    cout << "Seleccione una opcion: ";

    if (!getline(cin, texto)) {
        return false;
    }

    istringstream entrada(texto);
    char extra;

    if (!(entrada >> opcion) || (entrada >> extra)) {
        opcion = -1;
    }

    return true;
}

void ejecutarRecorrido(const Grafo& grafo, bool esBfs) {
    string nombre;
    string region;

    cout << "\n===== RECORRIDO "
         << (esBfs ? "BFS" : "DFS")
         << " =====\n";

    cout << "Nombre del nodo inicial: ";
    if (!getline(cin, nombre)) {
        return;
    }

    cout << "Region del nodo inicial: ";
    if (!getline(cin, region)) {
        return;
    }

    if (grafo.buscarNodo(nombre, region) == nullptr) {
        cout << "Nodo inicial no encontrado.\n";
        return;
    }

    vector<const Nodo*> recorrido;

    if (esBfs) {
        recorrido = Recorridos::bfs(grafo, nombre, region);
    } else {
        recorrido = Recorridos::dfs(grafo, nombre, region);
    }

    cout << "\nResultado "
         << (esBfs ? "BFS" : "DFS")
         << ":\n";

    Recorridos::mostrar(recorrido);

    cout << "Nodos visitados: "
         << recorrido.size() << '\n';
}

} // namespace

void menuRecorridos(const Grafo& grafo) {
    while (true) {
        cout << "\n===== RECORRIDOS DE LA RED =====\n";
        cout << "1. Recorrido BFS\n";
        cout << "2. Recorrido DFS\n";
        cout << "0. Volver al menu principal\n";

        int opcion;

        if (!leerOpcion(opcion)) {
            return;
        }

        switch (opcion) {
            case 1:
                ejecutarRecorrido(grafo, true);
                break;

            case 2:
                ejecutarRecorrido(grafo, false);
                break;

            case 0:
                cout << "Regresando al menu principal...\n";
                return;

            default:
                cout << "Opcion invalida.\n";
                break;
        }

        if (cin.eof()) {
            return;
        }
    }
}
#include "Recorridos.h"

#include <algorithm>
#include <iostream>
#include <queue>

using namespace std;

vector<const Nodo*> Recorridos::bfs(
    const Grafo& grafo,
    const string& nombre,
    const string& region) {

    vector<const Nodo*> resultado;

    const Nodo* inicio = grafo.buscarNodo(nombre, region);

    if (inicio == nullptr) {
        return resultado;
    }

    vector<const Nodo*> visitados;
    queue<const Nodo*> pendientes;

    visitados.push_back(inicio);
    pendientes.push(inicio);

    while (!pendientes.empty()) {
        const Nodo* actual = pendientes.front();
        pendientes.pop();

        resultado.push_back(actual);

        // Recorrer solamente las conexiones salientes
        const Arco* arco = actual->primerSaliente;

        while (arco != nullptr) {
            const Nodo* vecino = arco->destino;

            if (find(visitados.begin(), visitados.end(),
                     vecino) == visitados.end()) {

                visitados.push_back(vecino);
                pendientes.push(vecino);
            }

            arco = arco->siguienteSaliente;
        }
    }

    return resultado;
}

void Recorridos::dfsRecursivo(
    const Nodo* actual,
    vector<const Nodo*>& resultado,
    vector<const Nodo*>& visitados) {

    visitados.push_back(actual);
    resultado.push_back(actual);

    const Arco* arco = actual->primerSaliente;

    while (arco != nullptr) {
        const Nodo* vecino = arco->destino;

        if (find(visitados.begin(), visitados.end(),
                 vecino) == visitados.end()) {

            dfsRecursivo(vecino, resultado, visitados);
        }

        arco = arco->siguienteSaliente;
    }
}

vector<const Nodo*> Recorridos::dfs(
    const Grafo& grafo,
    const string& nombre,
    const string& region) {

    vector<const Nodo*> resultado;
    vector<const Nodo*> visitados;

    const Nodo* inicio = grafo.buscarNodo(nombre, region);

    if (inicio != nullptr) {
        dfsRecursivo(inicio, resultado, visitados);
    }

    return resultado;
}

void Recorridos::mostrar(
    const vector<const Nodo*>& recorrido) {

    if (recorrido.empty()) {
        cout << "No se encontraron nodos para recorrer.\n";
        return;
    }

    for (size_t i = 0; i < recorrido.size(); i++) {
        cout << recorrido[i]->nombre
             << " (" << recorrido[i]->region << ")";

        if (i + 1 < recorrido.size()) {
            cout << " -> ";
        }
    }

    cout << '\n';
}
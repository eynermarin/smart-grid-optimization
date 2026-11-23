#include "ReportesRed.h"

#include <iostream>
#include <vector>

using namespace std;

vector<const Nodo*> ReportesRed::obtenerNodosInactivos(
    const Grafo& grafo) {

    vector<const Nodo*> inactivos;

    const Nodo* actual = grafo.getPrimerNodo();

    while (actual != nullptr) {

        // Un nodo se considera inactivo cuando
        // no tiene conexiones de entrada ni salida.
        bool sinEntradas = actual->primerEntrante == nullptr;
        bool sinSalidas = actual->primerSaliente == nullptr;

        if (sinEntradas && sinSalidas) {
            inactivos.push_back(actual);
        }

        actual = actual->siguiente;
    }

    return inactivos;
}

void ReportesRed::mostrarNodosInactivos(
    const Grafo& grafo) {

    cout << "\n===== REPORTE DE NODOS INACTIVOS =====\n";

    vector<const Nodo*> inactivos =
        obtenerNodosInactivos(grafo);

    cout << "Total de nodos en la red: "
         << grafo.getCantidadNodos() << '\n';

    cout << "Nodos inactivos encontrados: "
         << inactivos.size() << '\n';

    if (inactivos.empty()) {
        cout << "No existen nodos completamente aislados.\n";
        return;
    }

    for (size_t i = 0; i < inactivos.size(); i++) {
        const Nodo* nodo = inactivos[i];

        cout << "\nNodo inactivo " << (i + 1) << ":\n";
        cout << "Nombre: " << nodo->nombre << '\n';
        cout << "Region: " << nodo->region << '\n';
        cout << "Tipo: " << nodo->tipo << '\n';
        cout << "Entradas: 0\n";
        cout << "Salidas: 0\n";
    }
}
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

std::vector<const Nodo*> ReportesRed::obtenerNodosSinClientes(
    const Grafo& grafo,
    const Clientes& clientes) {

    std::vector<const Nodo*> resultado;

    const Nodo* nodo = grafo.getPrimerNodo();

    while (nodo != nullptr) {
        bool registrado = false;

        const std::string nombreCompleto =
            nodo->nombre + " (" + nodo->region + ")";

        const Cliente* cliente = clientes.getPrimero();

        while (cliente != nullptr && !registrado) {
            for (const std::string& referencia :
                 cliente->subestacionesConectadas) {

                if (referencia == nodo->nombre ||
                    referencia == nombreCompleto) {
                    registrado = true;
                    break;
                }
            }

            cliente = cliente->siguiente;
        }

        if (!registrado) {
            resultado.push_back(nodo);
        }

        nodo = nodo->siguiente;
    }

    return resultado;
}

void ReportesRed::mostrarNodosSinClientes(
    const Grafo& grafo,
    const Clientes& clientes) {

    std::cout
        << "\n===== REPORTE DE NODOS SIN REGISTRO EN CLIENTES =====\n";

    std::vector<const Nodo*> nodos =
        obtenerNodosSinClientes(grafo, clientes);

    std::cout << "Total de nodos en la red: "
              << grafo.getCantidadNodos() << '\n';

    std::cout << "Nodos sin registro en clientes: "
              << nodos.size() << '\n';

    if (nodos.empty()) {
        std::cout << "Todos los nodos aparecen registrados "
                  << "en al menos un cliente.\n";
        return;
    }

    for (size_t i = 0; i < nodos.size(); i++) {
        const Nodo* actual = nodos[i];

        std::cout << "\nNodo " << (i + 1) << ":\n";
        std::cout << "Nombre: " << actual->nombre << '\n';
        std::cout << "Region: " << actual->region << '\n';
        std::cout << "Tipo: " << actual->tipo << '\n';
    }
}
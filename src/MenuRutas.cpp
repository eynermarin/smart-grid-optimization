#include "MenuRutas.h"
#include "Rutas.h"
#include "Dijkstra.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

bool leerTexto(const string& mensaje, string& texto) {
    cout << mensaje;
    return static_cast<bool>(getline(cin, texto));
}

bool leerOpcion(int& opcion) {
    string texto;

    if (!leerTexto("Seleccione una opcion: ", texto)) {
        return false;
    }

    istringstream flujo(texto);
    char extra;

    if (!(flujo >> opcion) || (flujo >> extra)) {
        opcion = -1;
    }

    return true;
}

bool leerExtremos(
    const Grafo& grafo,
    string& origen,
    string& regionOrigen,
    string& destino,
    string& regionDestino) {

    if (!leerTexto("Nombre del nodo origen: ", origen) ||
        !leerTexto("Region del origen: ", regionOrigen) ||
        !leerTexto("Nombre del nodo destino: ", destino) ||
        !leerTexto("Region del destino: ", regionDestino)) {
        return false;
    }

    if (grafo.buscarNodo(origen, regionOrigen) == nullptr) {
        cout << "Nodo origen no encontrado.\n";
        return false;
    }

    if (grafo.buscarNodo(destino, regionDestino) == nullptr) {
        cout << "Nodo destino no encontrado.\n";
        return false;
    }

    return true;
}

void consultarTodas(const Grafo& grafo) {
    cout << "\n===== TODAS LAS RUTAS SIMPLES =====\n";

    string origen, regionOrigen, destino, regionDestino;

    if (!leerExtremos(
        grafo, origen, regionOrigen, destino, regionDestino)) {
        return;
    }

    cout << "\nFiltrar por medio de transmision:\n";
    cout << "1. Alta tension\n";
    cout << "2. Subterraneo\n";
    cout << "3. Ambos medios\n";

    int filtro;

    if (!leerOpcion(filtro)) {
        return;
    }

    vector<string> medios;

    switch (filtro) {
        case 1:
            medios = {"alta_tension"};
            break;

        case 2:
            medios = {"subterraneo"};
            break;

        case 3:
            medios = {"alta_tension", "subterraneo"};
            break;

        default:
            cout << "Filtro invalido.\n";
            return;
    }

    vector<Ruta> rutas = Rutas::buscarTodas(
        grafo,
        origen, regionOrigen,
        destino, regionDestino,
        medios
    );

    cout << "\nRESULTADO DE LA BUSQUEDA:\n";
    Rutas::mostrar(rutas);
}

void consultarDijkstra(
    const Grafo& grafo, CriterioRuta criterio) {

    cout << "\n===== DIJKSTRA =====\n";

    string origen, regionOrigen, destino, regionDestino;

    if (!leerExtremos(
        grafo, origen, regionOrigen, destino, regionDestino)) {
        return;
    }

    Ruta resultado;

    bool encontrada = Dijkstra::calcular(
        grafo,
        origen, regionOrigen,
        destino, regionDestino,
        criterio,
        resultado
    );

    if (!encontrada) {
        cout << "No se encontro una ruta entre los nodos.\n";
        return;
    }

    if (criterio == CriterioRuta::Distancia) {
        cout << "\nRUTA DE MENOR DISTANCIA:\n";
    } else {
        cout << "\nRUTA DE MENORES PERDIDAS:\n";
    }

    Rutas::mostrar({resultado});
}

} // namespace

void menuRutas(const Grafo& grafo) {
    while (true) {
        cout << "\n===== CONSULTAS DE RUTAS =====\n";
        cout << "1. Mostrar todas las rutas simples\n";
        cout << "2. Ruta de menor distancia (Dijkstra)\n";
        cout << "3. Ruta de menores perdidas (Dijkstra)\n";
        cout << "0. Volver al menu principal\n";

        int opcion;

        if (!leerOpcion(opcion)) {
            return;
        }

        switch (opcion) {
            case 1:
                consultarTodas(grafo);
                break;

            case 2:
                consultarDijkstra(grafo, CriterioRuta::Distancia);
                break;

            case 3:
                consultarDijkstra(grafo, CriterioRuta::Perdidas);
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
#include "Dijkstra.h"

#include <iostream>
#include <string>

using namespace std;

bool verificar(bool condicion, const string& mensaje) {
    if (!condicion) {
        cerr << "FALLO - " << mensaje << '\n';
        return false;
    }

    cout << "OK - " << mensaje << '\n';
    return true;
}

bool caminoCorrecto(
    const Ruta& ruta,
    const string& primero,
    const string& segundo,
    const string& tercero) {

    return ruta.nodos.size() == 3 &&
           ruta.nodos[0]->nombre == primero &&
           ruta.nodos[1]->nombre == segundo &&
           ruta.nodos[2]->nombre == tercero &&
           ruta.arcos.size() == 2;
}

int main() {
    cout << "===== PRUEBAS DE DIJKSTRA =====\n";

    Grafo grafo;

    if (!verificar(
        grafo.agregarNodo("A", "Norte", "generador") &&
        grafo.agregarNodo("B", "Norte", "subestacion") &&
        grafo.agregarNodo("C", "Sur", "subestacion") &&
        grafo.agregarNodo("D", "Sur", "subestacion") &&
        grafo.agregarNodo("E", "Este", "subestacion"),
        "Cinco nodos creados")) {
        return 1;
    }

    if (!verificar(
        grafo.agregarArco(
            "A", "Norte", "B", "Norte",
            "alta_tension", 4, 2, 100, 5) &&
        grafo.agregarArco(
            "B", "Norte", "D", "Sur",
            "alta_tension", 4, 2, 100, 5) &&
        grafo.agregarArco(
            "A", "Norte", "C", "Sur",
            "subterraneo", 10, 3, 100, 1) &&
        grafo.agregarArco(
            "C", "Sur", "D", "Sur",
            "subterraneo", 10, 3, 100, 1) &&
        grafo.agregarArco(
            "A", "Norte", "D", "Sur",
            "alta_tension", 30, 7, 100, 0.5),
        "Cinco conexiones creadas")) {
        return 1;
    }

    Ruta distancia;

    if (!verificar(
        Dijkstra::calcular(
            grafo, "A", "Norte", "D", "Sur",
            CriterioRuta::Distancia, distancia),
        "Ruta de menor distancia encontrada")) {
        return 1;
    }

    if (!verificar(
        caminoCorrecto(distancia, "A", "B", "D") &&
        distancia.distanciaTotalKm == 8 &&
        distancia.sumaPerdidasPorcentaje == 10,
        "Mejor ruta por distancia: A-B-D")) {
        return 1;
    }

    Ruta perdidas;

    if (!verificar(
        Dijkstra::calcular(
            grafo, "A", "Norte", "D", "Sur",
            CriterioRuta::Perdidas, perdidas),
        "Ruta de menores perdidas encontrada")) {
        return 1;
    }

    if (!verificar(
        perdidas.nodos.size() == 2 &&
        perdidas.nodos[0]->nombre == "A" &&
        perdidas.nodos[1]->nombre == "D" &&
        perdidas.arcos.size() == 1 &&
        perdidas.distanciaTotalKm == 30 &&
        perdidas.sumaPerdidasPorcentaje == 0.5,
        "Mejor ruta por perdidas: A-D")) {
        return 1;
    }

    Ruta imposible;

    if (!verificar(
        !Dijkstra::calcular(
            grafo, "E", "Este", "D", "Sur",
            CriterioRuta::Distancia, imposible) &&
        imposible.nodos.empty(),
        "Nodo aislado sin ruta")) {
        return 1;
    }

    if (!verificar(
        !Dijkstra::calcular(
            grafo, "D", "Sur", "B", "Norte",
            CriterioRuta::Distancia, imposible),
        "Se respeta la direccion de los arcos")) {
        return 1;
    }

    if (!verificar(
        !Dijkstra::calcular(
            grafo, "Inexistente", "Norte", "D", "Sur",
            CriterioRuta::Distancia, imposible),
        "Nodo inexistente rechazado")) {
        return 1;
    }

    Ruta mismo;

    if (!verificar(
        Dijkstra::calcular(
            grafo, "A", "Norte", "A", "Norte",
            CriterioRuta::Distancia, mismo) &&
        mismo.nodos.size() == 1 &&
        mismo.arcos.empty() &&
        mismo.distanciaTotalKm == 0,
        "Origen igual a destino: costo cero")) {
        return 1;
    }

    if (!verificar(
        !Dijkstra::calcular(
            grafo, "A", "Norte", "D", "Sur",
            static_cast<CriterioRuta>(99), imposible),
        "Criterio invalido rechazado")) {
        return 1;
    }

    cout << "\nRUTA MINIMA POR DISTANCIA:\n";
    Rutas::mostrar({distancia});

    cout << "\nRUTA MINIMA POR PERDIDAS:\n";
    Rutas::mostrar({perdidas});

    cout << "\nPRUEBA DE DIJKSTRA EXITOSA\n";
    return 0;
}
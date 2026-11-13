#include "Rutas.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool verificar(bool condicion, const string& mensaje) {
    if (!condicion) {
        cerr << "FALLO - " << mensaje << '\n';
        return false;
    }

    cout << "OK - " << mensaje << '\n';
    return true;
}

bool contieneCamino(
    const vector<Ruta>& rutas,
    const vector<string>& nombres) {

    for (const Ruta& ruta : rutas) {
        if (ruta.nodos.size() != nombres.size()) {
            continue;
        }

        bool coincide = true;

        for (size_t i = 0; i < nombres.size(); i++) {
            if (ruta.nodos[i]->nombre != nombres[i]) {
                coincide = false;
                break;
            }
        }

        if (coincide) {
            return true;
        }
    }

    return false;
}

int main() {
    cout << "===== PRUEBAS DE RUTAS =====\n";

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
            "alta_tension", 10, 5, 100, 2) &&
        grafo.agregarArco(
            "A", "Norte", "C", "Sur",
            "subterraneo", 8, 4, 100, 1) &&
        grafo.agregarArco(
            "B", "Norte", "D", "Sur",
            "alta_tension", 7, 3, 100, 1.5) &&
        grafo.agregarArco(
            "B", "Norte", "C", "Sur",
            "subterraneo", 3, 2, 100, 0.5) &&
        grafo.agregarArco(
            "C", "Sur", "D", "Sur",
            "subterraneo", 6, 3, 100, 1) &&
        grafo.agregarArco(
            "D", "Sur", "A", "Norte",
            "alta_tension", 5, 2, 100, 1),
        "Seis conexiones dirigidas creadas")) {
        return 1;
    }

    vector<Ruta> todas = Rutas::buscarTodas(
        grafo, "A", "Norte", "D", "Sur",
        {"alta_tension", "subterraneo"}
    );

    if (!verificar(
        todas.size() == 3,
        "Tres rutas simples encontradas")) {
        return 1;
    }

    if (!verificar(
        contieneCamino(todas, {"A", "B", "D"}) &&
        contieneCamino(todas, {"A", "B", "C", "D"}) &&
        contieneCamino(todas, {"A", "C", "D"}),
        "Los tres caminos son correctos")) {
        return 1;
    }

    vector<Ruta> alta = Rutas::buscarTodas(
        grafo, "A", "Norte", "D", "Sur",
        {"alta_tension"}
    );

    if (!verificar(
        alta.size() == 1 &&
        contieneCamino(alta, {"A", "B", "D"}),
        "Filtro exclusivo de alta tension")) {
        return 1;
    }

    if (!verificar(
        alta[0].distanciaTotalKm == 17 &&
        alta[0].sumaPerdidasPorcentaje == 3.5,
        "Distancia y suma de perdidas correctas")) {
        return 1;
    }

    vector<Ruta> subterraneo = Rutas::buscarTodas(
        grafo, "A", "Norte", "D", "Sur",
        {"subterraneo"}
    );

    if (!verificar(
        subterraneo.size() == 1 &&
        contieneCamino(subterraneo, {"A", "C", "D"}),
        "Filtro exclusivo subterraneo")) {
        return 1;
    }

    if (!verificar(
        Rutas::buscarTodas(
            grafo, "D", "Sur", "B", "Norte",
            {"alta_tension", "subterraneo"}
        ).size() == 1,
        "Las rutas respetan los arcos dirigidos")) {
        return 1;
    }

    if (!verificar(
        Rutas::buscarTodas(
            grafo, "E", "Este", "D", "Sur",
            {"alta_tension", "subterraneo"}
        ).empty(),
        "Nodo aislado sin rutas")) {
        return 1;
    }

    if (!verificar(
        Rutas::buscarTodas(
            grafo, "Inexistente", "Norte", "D", "Sur",
            {"alta_tension"}
        ).empty(),
        "Origen inexistente rechazado")) {
        return 1;
    }

    if (!verificar(
        Rutas::buscarTodas(
            grafo, "A", "Norte", "D", "Sur",
            {"red_local"}
        ).empty(),
        "Medio no permitido rechazado")) {
        return 1;
    }

    if (!verificar(
        Rutas::buscarTodas(
            grafo, "A", "Norte", "D", "Sur",
            {"alta_tension", "alta_tension"}
        ).empty(),
        "Filtros duplicados rechazados")) {
        return 1;
    }

    cout << "\nRUTAS ENCONTRADAS:\n";
    Rutas::mostrar(todas);

    cout << "\nPRUEBA DE RUTAS EXITOSA\n";
    return 0;
}
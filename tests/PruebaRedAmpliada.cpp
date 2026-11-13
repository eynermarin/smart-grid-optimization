#include "Grafo.h"
#include "Persistencia.h"
#include "Recorridos.h"
#include "Rutas.h"
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

int main() {
    cout << "===== PRUEBAS DE RED AMPLIADA =====\n";

    Grafo grafo;

    if (!verificar(
        Persistencia::cargarRed(
            "data/red_electrica_ampliada_prueba.json", grafo),
        "JSON ampliado cargado correctamente")) {
        return 1;
    }

    if (!verificar(
        grafo.getCantidadNodos() == 15 &&
        grafo.getCantidadArcos() == 20,
        "15 nodos y 20 conexiones registrados")) {
        return 1;
    }

    if (!verificar(
        grafo.buscarNodo("Planta Solar Alpha", "Zona Norte") != nullptr &&
        grafo.buscarNodo("Subestacion Central", "Zona Norte") != nullptr &&
        grafo.buscarNodo("Subestacion Sur", "Zona Sur") != nullptr,
        "Nodos originales conservados")) {
        return 1;
    }

    if (!verificar(
        grafo.buscarNodo("Subestacion Sur 5", "Zona Sur") != nullptr &&
        grafo.buscarNodo("Planta Eolica Norte", "Zona Norte") != nullptr,
        "Nuevos nodos disponibles")) {
        return 1;
    }

    auto bfs = Recorridos::bfs(
        grafo, "Planta Solar Alpha", "Zona Norte");

    auto dfs = Recorridos::dfs(
        grafo, "Planta Solar Alpha", "Zona Norte");

    if (!verificar(
        !bfs.empty() && !dfs.empty() &&
        bfs.size() == dfs.size() &&
        bfs.size() >= 10,
        "BFS y DFS recorren la red ampliada")) {
        return 1;
    }

    auto rutas = Rutas::buscarTodas(
        grafo,
        "Planta Solar Alpha", "Zona Norte",
        "Subestacion Sur 2", "Zona Sur",
        {"alta_tension", "subterraneo"}
    );

    if (!verificar(
        !rutas.empty(),
        "Busqueda de rutas simples funcionando")) {
        return 1;
    }

    Ruta minima;

    if (!verificar(
        Dijkstra::calcular(
            grafo,
            "Planta Solar Alpha", "Zona Norte",
            "Subestacion Sur 5", "Zona Sur",
            CriterioRuta::Distancia,
            minima),
        "Dijkstra encuentra una ruta por distancia")) {
        return 1;
    }

    if (!verificar(
        minima.nodos.size() >= 2 &&
        minima.arcos.size() + 1 == minima.nodos.size() &&
        minima.distanciaTotalKm > 0,
        "Resultado de Dijkstra consistente")) {
        return 1;
    }

    Ruta menoresPerdidas;

    if (!verificar(
        Dijkstra::calcular(
            grafo,
            "Planta Solar Alpha", "Zona Norte",
            "Subestacion Sur 5", "Zona Sur",
            CriterioRuta::Perdidas,
            menoresPerdidas),
        "Dijkstra encuentra una ruta por perdidas")) {
        return 1;
    }

    if (!verificar(
        grafo.getCantidadNodos() == 15 &&
        grafo.getCantidadArcos() == 20,
        "Las consultas no modificaron el grafo")) {
        return 1;
    }

    cout << "\nMejor ruta por distancia:\n";
    Rutas::mostrar({minima});

    cout << "\nMejor ruta por perdidas:\n";
    Rutas::mostrar({menoresPerdidas});

    cout << "\nPRUEBA DE RED AMPLIADA EXITOSA\n";
    return 0;
}
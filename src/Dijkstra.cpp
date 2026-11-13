#include "Dijkstra.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

bool Dijkstra::calcular(
    const Grafo& grafo,
    const string& nombreOrigen,
    const string& regionOrigen,
    const string& nombreDestino,
    const string& regionDestino,
    CriterioRuta criterio,
    Ruta& resultado) {

    // Limpiar el resultado incluso si la busqueda falla.
    resultado = Ruta{};

    const Nodo* origen =
        grafo.buscarNodo(nombreOrigen, regionOrigen);

    const Nodo* destino =
        grafo.buscarNodo(nombreDestino, regionDestino);

    if (origen == nullptr || destino == nullptr) {
        return false;
    }

    if (criterio != CriterioRuta::Distancia &&
        criterio != CriterioRuta::Perdidas) {
        return false;
    }

    // Dijkstra permite un camino de costo cero
    // cuando el origen y destino son el mismo nodo.
    if (origen == destino) {
        resultado.nodos.push_back(origen);
        return true;
    }

    using Estado = pair<double, const Nodo*>;

    priority_queue<
        Estado,
        vector<Estado>,
        greater<Estado>
    > pendientes;

    unordered_map<const Nodo*, double> distancias;
    unordered_map<const Nodo*, const Arco*> anteriores;

    distancias[origen] = 0.0;
    pendientes.push({0.0, origen});

    while (!pendientes.empty()) {
        Estado estado = pendientes.top();
        pendientes.pop();

        double costoActual = estado.first;
        const Nodo* actual = estado.second;

        if (costoActual > distancias.at(actual)) {
            continue;
        }

        if (actual == destino) {
            break;
        }

        const Arco* arco = actual->primerSaliente;

        while (arco != nullptr) {
            double peso = criterio == CriterioRuta::Distancia
                ? arco->distanciaKm
                : arco->perdidaPorcentaje;

            // Dijkstra requiere pesos finitos no negativos.
            if (!isfinite(peso) || peso < 0) {
                return false;
            }

            double nuevoCosto = costoActual + peso;

            if (!isfinite(nuevoCosto)) {
                return false;
            }

            const Nodo* vecino = arco->destino;
            auto encontrado = distancias.find(vecino);

            if (encontrado == distancias.end() ||
                nuevoCosto < encontrado->second) {

                distancias[vecino] = nuevoCosto;
                anteriores[vecino] = arco;

                pendientes.push({nuevoCosto, vecino});
            }

            arco = arco->siguienteSaliente;
        }
    }

    if (distancias.find(destino) == distancias.end()) {
        return false;
    }

    vector<const Nodo*> nodosInvertidos;
    vector<const Arco*> arcosInvertidos;

    const Nodo* actual = destino;
    nodosInvertidos.push_back(actual);

    // Reconstruir desde el destino hasta el origen.
    while (actual != origen) {
        auto encontrado = anteriores.find(actual);

        if (encontrado == anteriores.end()) {
            return false;
        }

        const Arco* arco = encontrado->second;
        arcosInvertidos.push_back(arco);

        actual = arco->origen;
        nodosInvertidos.push_back(actual);
    }

    reverse(nodosInvertidos.begin(), nodosInvertidos.end());
    reverse(arcosInvertidos.begin(), arcosInvertidos.end());

    resultado.nodos = std::move(nodosInvertidos);
    resultado.arcos = std::move(arcosInvertidos);

    for (const Arco* arco : resultado.arcos) {
        resultado.distanciaTotalKm += arco->distanciaKm;
        resultado.sumaPerdidasPorcentaje +=
            arco->perdidaPorcentaje;
    }

    return true;
}
#include "OptimizadorTokens.h"

#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

long long OptimizadorTokens::calcularPuntaje(
    const Ruta& ruta,
    bool horarioValle) {

    long long bonificacion = 0;

    for (const Arco* arco : ruta.arcos) {
        if (arco->medio == "subterraneo") {
            bonificacion += 4;
        } else if (arco->medio == "alta_tension") {
            bonificacion += 2;
        } else if (arco->medio == "red_local") {
            bonificacion += 1;
        }
    }

    double valor =
        60.0
        - ceil(ruta.distanciaTotalKm / 5.0)
        - ceil(3.0 * ruta.sumaPerdidasPorcentaje)
        + bonificacion
        + (horarioValle ? 5 : 0);

    if (!isfinite(valor)) {
        return 1;
    }

    return static_cast<long long>(
        max(1.0, min(valor, 1000000.0))
    );
}

void OptimizadorTokens::explorar(
    const Nodo* actual,
    const Nodo* destino,
    bool horarioValle,
    vector<const Nodo*>& nodos,
    vector<const Arco*>& arcos,
    ResultadoEcoRuta& mejor) {

    if (actual == destino) {
        Ruta candidata;

        candidata.nodos = nodos;
        candidata.arcos = arcos;

        for (const Arco* arco : arcos) {
            candidata.distanciaTotalKm += arco->distanciaKm;
            candidata.sumaPerdidasPorcentaje +=
                arco->perdidaPorcentaje;
        }

        long long puntos = calcularPuntaje(
            candidata, horarioValle);

        bool mejora =
            !mejor.encontrada ||
            puntos > mejor.tokens ||
            (puntos == mejor.tokens &&
             candidata.distanciaTotalKm <
             mejor.ruta.distanciaTotalKm);

        if (mejora) {
            mejor.encontrada = true;
            mejor.tokens = puntos;
            mejor.ruta = candidata;
        }

        return;
    }

    const Arco* arco = actual->primerSaliente;

    while (arco != nullptr) {
        const Nodo* vecino = arco->destino;

        bool repetido = find(
            nodos.begin(), nodos.end(), vecino
        ) != nodos.end();

        if (!repetido) {
            nodos.push_back(vecino);
            arcos.push_back(arco);

            explorar(
                vecino,
                destino,
                horarioValle,
                nodos,
                arcos,
                mejor
            );

            arcos.pop_back();
            nodos.pop_back();
        }

        arco = arco->siguienteSaliente;
    }
}

ResultadoEcoRuta OptimizadorTokens::buscarMejorRuta(
    const Grafo& grafo,
    const string& nombreOrigen,
    const string& regionOrigen,
    const string& nombreDestino,
    const string& regionDestino,
    bool horarioValle) {

    ResultadoEcoRuta resultado;

    const Nodo* origen = grafo.buscarNodo(
        nombreOrigen, regionOrigen);

    const Nodo* destino = grafo.buscarNodo(
        nombreDestino, regionDestino);

    if (origen == nullptr || destino == nullptr ||
        origen == destino) {
        return resultado;
    }

    vector<const Nodo*> nodos = {origen};
    vector<const Arco*> arcos;

    explorar(
        origen,
        destino,
        horarioValle,
        nodos,
        arcos,
        resultado
    );

    return resultado;
}
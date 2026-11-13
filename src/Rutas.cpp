#include "Rutas.h"

#include <algorithm>
#include <iostream>

using namespace std;

bool Rutas::medioPermitido(
    const string& medio,
    const vector<string>& permitidos) {

    return find(
        permitidos.begin(),
        permitidos.end(),
        medio
    ) != permitidos.end();
}

void Rutas::buscarRecursivo(
    const Nodo* actual,
    const Nodo* destino,
    const vector<string>& mediosPermitidos,
    vector<const Nodo*>& caminoNodos,
    vector<const Arco*>& caminoArcos,
    vector<Ruta>& resultados) {

    if (actual == destino) {
        Ruta ruta;
        ruta.nodos = caminoNodos;
        ruta.arcos = caminoArcos;

        for (const Arco* arco : caminoArcos) {
            ruta.distanciaTotalKm += arco->distanciaKm;

            // Indicador aditivo para comparar rutas.
            ruta.sumaPerdidasPorcentaje +=
                arco->perdidaPorcentaje;
        }

        resultados.push_back(ruta);
        return;
    }

    const Arco* arco = actual->primerSaliente;

    while (arco != nullptr) {
        const Nodo* vecino = arco->destino;

        bool permitido = medioPermitido(
            arco->medio, mediosPermitidos);

        bool yaVisitado = find(
            caminoNodos.begin(),
            caminoNodos.end(),
            vecino
        ) != caminoNodos.end();

        if (permitido && !yaVisitado) {

            // Elegir la siguiente conexion
            caminoNodos.push_back(vecino);
            caminoArcos.push_back(arco);

            // Explorar el camino
            buscarRecursivo(
                vecino,
                destino,
                mediosPermitidos,
                caminoNodos,
                caminoArcos,
                resultados
            );

            // Retroceder para explorar otra ruta
            caminoArcos.pop_back();
            caminoNodos.pop_back();
        }

        arco = arco->siguienteSaliente;
    }
}

vector<Ruta> Rutas::buscarTodas(
    const Grafo& grafo,
    const string& nombreOrigen,
    const string& regionOrigen,
    const string& nombreDestino,
    const string& regionDestino,
    const vector<string>& mediosPermitidos) {

    vector<Ruta> resultados;

    const Nodo* origen =
        grafo.buscarNodo(nombreOrigen, regionOrigen);

    const Nodo* destino =
        grafo.buscarNodo(nombreDestino, regionDestino);

    if (origen == nullptr || destino == nullptr ||
        origen == destino) {
        return resultados;
    }

    // Esta consulta acepta uno o dos medios del enunciado.
    if (mediosPermitidos.empty() ||
        mediosPermitidos.size() > 2) {
        return resultados;
    }

    for (const string& medio : mediosPermitidos) {
        if (medio != "alta_tension" &&
            medio != "subterraneo") {
            return resultados;
        }
    }

    if (mediosPermitidos.size() == 2 &&
        mediosPermitidos[0] == mediosPermitidos[1]) {
        return resultados;
    }

    vector<const Nodo*> caminoNodos;
    vector<const Arco*> caminoArcos;

    caminoNodos.push_back(origen);

    buscarRecursivo(
        origen,
        destino,
        mediosPermitidos,
        caminoNodos,
        caminoArcos,
        resultados
    );

    return resultados;
}

void Rutas::mostrar(const vector<Ruta>& rutas) {
    if (rutas.empty()) {
        cout << "No se encontraron rutas.\n";
        return;
    }

    cout << "Rutas encontradas: " << rutas.size() << '\n';

    for (size_t i = 0; i < rutas.size(); i++) {
        const Ruta& ruta = rutas[i];

        cout << "\nRuta " << i + 1 << ":\n";

        for (size_t j = 0; j < ruta.nodos.size(); j++) {
            cout << ruta.nodos[j]->nombre
                 << " (" << ruta.nodos[j]->region << ")";

            if (j < ruta.arcos.size()) {
                cout << " --["
                     << ruta.arcos[j]->medio
                     << "]--> ";
            }
        }

        cout << "\nDistancia total: "
             << ruta.distanciaTotalKm << " km\n";

        cout << "Suma de perdidas por tramo: "
             << ruta.sumaPerdidasPorcentaje
             << " puntos porcentuales\n";
    }
}
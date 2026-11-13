#include "Grafo.h"
#include <iostream>
#include <cmath>

using namespace std;

Nodo::Nodo(string nombre, string region, string tipo)
    : nombre(nombre), region(region), tipo(tipo),
      siguiente(nullptr), primerSaliente(nullptr),
      primerEntrante(nullptr) {}

Arco::Arco(Nodo* origen, Nodo* destino,
           string medio, double distancia,
           double tiempo, double capacidad, double perdida)
    : origen(origen), destino(destino), medio(medio),
      distanciaKm(distancia), tiempoMs(tiempo),
      capacidadMaxima(capacidad), perdidaPorcentaje(perdida),
      siguienteSaliente(nullptr), siguienteEntrante(nullptr),
      siguienteGlobal(nullptr) {}

Grafo::Grafo()
    : primero(nullptr), primerArco(nullptr),
      cantidadNodos(0), cantidadArcos(0) {}

Grafo::~Grafo() {
    while (primerArco != nullptr) {
        Arco* temporal = primerArco;
        primerArco = primerArco->siguienteGlobal;
        delete temporal;
    }

    while (primero != nullptr) {
        Nodo* temporal = primero;
        primero = primero->siguiente;
        delete temporal;
    }
}

Nodo* Grafo::buscarNodoInterno(string nombre, string region) const {
    Nodo* actual = primero;

    while (actual != nullptr) {
        if (actual->nombre == nombre && actual->region == region) {
            return actual;
        }

        actual = actual->siguiente;
    }

    return nullptr;
}

const Nodo* Grafo::buscarNodo(string nombre, string region) const {
    return buscarNodoInterno(nombre, region);
}

bool Grafo::agregarNodo(string nombre, string region, string tipo) {
    if (nombre.empty() || region.empty() || tipo.empty()) {
        return false;
    }

    if (buscarNodoInterno(nombre, region) != nullptr) {
        return false;
    }

    Nodo* nuevo = new Nodo(nombre, region, tipo);
    nuevo->siguiente = primero;
    primero = nuevo;

    cantidadNodos++;
    return true;
}

const Arco* Grafo::buscarArco(
    string origen, string regionOrigen,
    string destino, string regionDestino,
    string medio) const {

    const Arco* actual = primerArco;

    while (actual != nullptr) {
        if (actual->origen->nombre == origen &&
            actual->origen->region == regionOrigen &&
            actual->destino->nombre == destino &&
            actual->destino->region == regionDestino &&
            actual->medio == medio) {
            return actual;
        }

        actual = actual->siguienteGlobal;
    }

    return nullptr;
}

bool Grafo::agregarArco(
    string origen, string regionOrigen,
    string destino, string regionDestino,
    string medio, double distancia,
    double tiempo, double capacidad, double perdida) {

    Nodo* nodoOrigen = buscarNodoInterno(origen, regionOrigen);
    Nodo* nodoDestino = buscarNodoInterno(destino, regionDestino);

    if (nodoOrigen == nullptr || nodoDestino == nullptr) {
        return false;
    }

    if (nodoOrigen == nodoDestino) {
        return false;
    }

    if (medio != "alta_tension" &&
        medio != "subterraneo" &&
        medio != "red_local") {
        return false;
    }

    if (!isfinite(distancia) || !isfinite(tiempo) ||
        !isfinite(capacidad) || !isfinite(perdida) ||
        distancia <= 0 || tiempo < 0 ||
        capacidad <= 0 || perdida < 0 || perdida > 100) {
        return false;
    }

    if (buscarArco(origen, regionOrigen, destino,
                   regionDestino, medio) != nullptr) {
        return false;
    }

    Arco* nuevo = new Arco(nodoOrigen, nodoDestino,
                           medio, distancia, tiempo,
                           capacidad, perdida);

    nuevo->siguienteSaliente = nodoOrigen->primerSaliente;
    nodoOrigen->primerSaliente = nuevo;

    nuevo->siguienteEntrante = nodoDestino->primerEntrante;
    nodoDestino->primerEntrante = nuevo;

    nuevo->siguienteGlobal = primerArco;
    primerArco = nuevo;

    cantidadArcos++;
    return true;
}

void Grafo::mostrarNodos() const {
    const Nodo* actual = primero;

    while (actual != nullptr) {
        cout << "Nodo: " << actual->nombre
             << " | Region: " << actual->region
             << " | Tipo: " << actual->tipo << endl;

        actual = actual->siguiente;
    }
}

void Grafo::mostrarArcos() const {
    const Arco* actual = primerArco;

    while (actual != nullptr) {
        cout << actual->origen->nombre
             << " -> " << actual->destino->nombre
             << " | Medio: " << actual->medio
             << " | Distancia: " << actual->distanciaKm << " km"
             << " | Tiempo: " << actual->tiempoMs << " ms"
             << " | Capacidad: " << actual->capacidadMaxima
             << " | Perdida: " << actual->perdidaPorcentaje << "%"
             << endl;

        actual = actual->siguienteGlobal;
    }
}

int Grafo::getCantidadNodos() const {
    return cantidadNodos;
}

int Grafo::getCantidadArcos() const {
    return cantidadArcos;
}

#include "Grafo.h"
#include <iostream>
#include <cmath>
#include <utility>

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

bool Grafo::modificarNodo(
    string nombre, string region,
    string nuevoNombre, string nuevaRegion,
    string nuevoTipo) {

    Nodo* nodo = buscarNodoInterno(nombre, region);

    if (nodo == nullptr || nuevoNombre.empty() ||
        nuevaRegion.empty() || nuevoTipo.empty()) {
        return false;
    }

    Nodo* duplicado = buscarNodoInterno(nuevoNombre, nuevaRegion);

    if (duplicado != nullptr && duplicado != nodo) {
        return false;
    }

    nodo->nombre = nuevoNombre;
    nodo->region = nuevaRegion;
    nodo->tipo = nuevoTipo;

    return true;
}

void Grafo::eliminarArcoInterno(Arco* arco) {
    if (arco == nullptr) {
        return;
    }

    // Desenlazar de las conexiones salientes
    Arco** enlace = &arco->origen->primerSaliente;

    while (*enlace != nullptr && *enlace != arco) {
        enlace = &((*enlace)->siguienteSaliente);
    }

    if (*enlace == arco) {
        *enlace = arco->siguienteSaliente;
    }

    // Desenlazar de las conexiones entrantes
    enlace = &arco->destino->primerEntrante;

    while (*enlace != nullptr && *enlace != arco) {
        enlace = &((*enlace)->siguienteEntrante);
    }

    if (*enlace == arco) {
        *enlace = arco->siguienteEntrante;
    }

    // Desenlazar de la lista global de arcos
    enlace = &primerArco;

    while (*enlace != nullptr && *enlace != arco) {
        enlace = &((*enlace)->siguienteGlobal);
    }

    if (*enlace == arco) {
        *enlace = arco->siguienteGlobal;
    }

    delete arco;
    cantidadArcos--;
}

bool Grafo::eliminarArco(
    string origen, string regionOrigen,
    string destino, string regionDestino,
    string medio) {

    Arco* actual = primerArco;

    while (actual != nullptr) {
        if (actual->origen->nombre == origen &&
            actual->origen->region == regionOrigen &&
            actual->destino->nombre == destino &&
            actual->destino->region == regionDestino &&
            actual->medio == medio) {

            eliminarArcoInterno(actual);
            return true;
        }

        actual = actual->siguienteGlobal;
    }

    return false;
}

bool Grafo::eliminarNodo(string nombre, string region) {
    Nodo** enlace = &primero;

    while (*enlace != nullptr &&
           ((*enlace)->nombre != nombre ||
            (*enlace)->region != region)) {
        enlace = &((*enlace)->siguiente);
    }

    if (*enlace == nullptr) {
        return false;
    }

    Nodo* nodo = *enlace;

    // Eliminar primero todas las conexiones asociadas
    Arco* actual = primerArco;

    while (actual != nullptr) {
        Arco* siguiente = actual->siguienteGlobal;

        if (actual->origen == nodo || actual->destino == nodo) {
            eliminarArcoInterno(actual);
        }

        actual = siguiente;
    }

    // Ahora es seguro eliminar el nodo
    *enlace = nodo->siguiente;
    delete nodo;
    cantidadNodos--;

    return true;
}

bool Grafo::modificarArco(
    string origen, string regionOrigen,
    string destino, string regionDestino,
    string medio, double distancia,
    double tiempo, double capacidad, double perdida) {

    if (!isfinite(distancia) || !isfinite(tiempo) ||
        !isfinite(capacidad) || !isfinite(perdida) ||
        distancia <= 0 || tiempo < 0 ||
        capacidad <= 0 || perdida < 0 || perdida > 100) {
        return false;
    }

    Arco* actual = primerArco;

    while (actual != nullptr) {
        if (actual->origen->nombre == origen &&
            actual->origen->region == regionOrigen &&
            actual->destino->nombre == destino &&
            actual->destino->region == regionDestino &&
            actual->medio == medio) {

            actual->distanciaKm = distancia;
            actual->tiempoMs = tiempo;
            actual->capacidadMaxima = capacidad;
            actual->perdidaPorcentaje = perdida;

            return true;
        }

        actual = actual->siguienteGlobal;
    }

    return false;
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


const Nodo* Grafo::getPrimerNodo() const {
    return primero;
}

const Arco* Grafo::getPrimerArco() const {
    return primerArco;
}

void Grafo::intercambiar(Grafo& otro) {
    swap(primero, otro.primero);
    swap(primerArco, otro.primerArco);
    swap(cantidadNodos, otro.cantidadNodos);
    swap(cantidadArcos, otro.cantidadArcos);
}

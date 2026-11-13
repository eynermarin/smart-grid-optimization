#include "Grafo.h"
#include <iostream>

using namespace std;

Nodo::Nodo(string nombre, string region, string tipo) {
    this->nombre = nombre;
    this->region = region;
    this->tipo = tipo;
    siguiente = nullptr;
}

Grafo::Grafo() {
    primero = nullptr;
    cantidadNodos = 0;
}

Grafo::~Grafo() {
    while (primero != nullptr) {
        Nodo* temporal = primero;
        primero = primero->siguiente;
        delete temporal;
    }
}

const Nodo* Grafo::buscarNodo(string nombre, string region) const {
    const Nodo* actual = primero;

    while (actual != nullptr) {
        if (actual->nombre == nombre && actual->region == region) {
            return actual;
        }
        actual = actual->siguiente;
    }

    return nullptr;
}

bool Grafo::agregarNodo(string nombre, string region, string tipo) {
    if (nombre.empty() || region.empty() || tipo.empty()) {
        return false;
    }

    if (buscarNodo(nombre, region) != nullptr) {
        return false;
    }

    Nodo* nuevo = new Nodo(nombre, region, tipo);

    nuevo->siguiente = primero;
    primero = nuevo;
    cantidadNodos++;

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

int Grafo::getCantidadNodos() const {
    return cantidadNodos;
}

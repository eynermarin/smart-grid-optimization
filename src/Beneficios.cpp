#include "Beneficios.h"

#include <iostream>
#include <utility>

using namespace std;

namespace {

bool textoValido(const string& texto) {
    return texto.find_first_not_of(" \t\n\r") != string::npos;
}

}

Beneficio::Beneficio(
    const string& nombre,
    const string& descripcion,
    long long costoTokens)
    : nombre(nombre),
      descripcion(descripcion),
      costoTokens(costoTokens),
      siguiente(nullptr) {
}

Beneficios::Beneficios()
    : primero(nullptr), cantidad(0) {
}

Beneficios::~Beneficios() {
    while (primero != nullptr) {
        Beneficio* temporal = primero;
        primero = primero->siguiente;
        delete temporal;
    }
}

Beneficio* Beneficios::buscarInterno(const string& nombre) {
    Beneficio* actual = primero;

    while (actual != nullptr) {
        if (actual->nombre == nombre) {
            return actual;
        }

        actual = actual->siguiente;
    }

    return nullptr;
}

const Beneficio* Beneficios::buscarBeneficio(
    const string& nombre) const {

    const Beneficio* actual = primero;

    while (actual != nullptr) {
        if (actual->nombre == nombre) {
            return actual;
        }

        actual = actual->siguiente;
    }

    return nullptr;
}

bool Beneficios::agregarBeneficio(
    const string& nombre,
    const string& descripcion,
    long long costoTokens) {

    if (!textoValido(nombre) ||
        !textoValido(descripcion) ||
        costoTokens <= 0 ||
        buscarBeneficio(nombre) != nullptr) {
        return false;
    }

    Beneficio* nuevo = new Beneficio(
        nombre, descripcion, costoTokens
    );

    nuevo->siguiente = primero;
    primero = nuevo;

    cantidad++;
    return true;
}

bool Beneficios::modificarBeneficio(
    const string& nombreActual,
    const string& nuevoNombre,
    const string& nuevaDescripcion,
    long long nuevoCosto) {

    Beneficio* beneficio = buscarInterno(nombreActual);

    if (beneficio == nullptr ||
        !textoValido(nuevoNombre) ||
        !textoValido(nuevaDescripcion) ||
        nuevoCosto <= 0) {
        return false;
    }

    if (nombreActual != nuevoNombre &&
        buscarBeneficio(nuevoNombre) != nullptr) {
        return false;
    }

    beneficio->nombre = nuevoNombre;
    beneficio->descripcion = nuevaDescripcion;
    beneficio->costoTokens = nuevoCosto;

    return true;
}

bool Beneficios::eliminarBeneficio(const string& nombre) {
    Beneficio** enlace = &primero;

    while (*enlace != nullptr &&
           (*enlace)->nombre != nombre) {
        enlace = &((*enlace)->siguiente);
    }

    if (*enlace == nullptr) {
        return false;
    }

    Beneficio* temporal = *enlace;
    *enlace = temporal->siguiente;

    delete temporal;
    cantidad--;

    return true;
}

void Beneficios::mostrarRecursivo(
    const Beneficio* actual) {

    if (actual == nullptr) {
        return;
    }

    cout << "Beneficio: " << actual->nombre
         << " | Descripcion: " << actual->descripcion
         << " | Costo: " << actual->costoTokens
         << " Eco-Tokens\n";

    mostrarRecursivo(actual->siguiente);
}

void Beneficios::mostrarBeneficios() const {
    if (primero == nullptr) {
        cout << "No hay beneficios registrados.\n";
        return;
    }

    mostrarRecursivo(primero);
}

int Beneficios::getCantidad() const {
    return cantidad;
}

const Beneficio* Beneficios::getPrimero() const {
    return primero;
}

void Beneficios::intercambiar(Beneficios& otro) {
    swap(primero, otro.primero);
    swap(cantidad, otro.cantidad);
}
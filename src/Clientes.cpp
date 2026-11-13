#include "Clientes.h"
#include <iostream>
#include <cmath>
#include <limits>

using namespace std;

Cliente::Cliente(string nombre)
    : nombre(nombre), consumoAcumulado(0),
      tokens(0), siguiente(nullptr) {}

Clientes::Clientes()
    : primero(nullptr), cantidad(0) {}

Clientes::~Clientes() {
    while (primero != nullptr) {
        Cliente* temporal = primero;
        primero = primero->siguiente;
        delete temporal;
    }
}

Cliente* Clientes::buscarInterno(const string& nombre) {
    Cliente* actual = primero;

    while (actual != nullptr) {
        if (actual->nombre == nombre) {
            return actual;
        }
        actual = actual->siguiente;
    }

    return nullptr;
}

const Cliente* Clientes::buscarCliente(const string& nombre) const {
    const Cliente* actual = primero;

    while (actual != nullptr) {
        if (actual->nombre == nombre) {
            return actual;
        }
        actual = actual->siguiente;
    }

    return nullptr;
}

bool Clientes::agregarCliente(const string& nombre) {
    if (nombre.find_first_not_of(" \t\n\r") == string::npos) {
        return false;
    }

    if (buscarCliente(nombre) != nullptr) {
        return false;
    }

    Cliente* nuevo = new Cliente(nombre);

    nuevo->siguiente = primero;
    primero = nuevo;

    cantidad++;
    return true;
}

bool Clientes::eliminarCliente(const string& nombre) {
    Cliente** enlace = &primero;

    while (*enlace != nullptr &&
           (*enlace)->nombre != nombre) {
        enlace = &((*enlace)->siguiente);
    }

    if (*enlace == nullptr) {
        return false;
    }

    Cliente* temporal = *enlace;
    *enlace = temporal->siguiente;

    delete temporal;
    cantidad--;

    return true;
}

bool Clientes::asignarTokens(
    const string& nombre, long long cantidadTokens) {

    if (cantidadTokens <= 0) {
        return false;
    }

    Cliente* cliente = buscarInterno(nombre);

    if (cliente == nullptr) {
        return false;
    }

    if (cliente->tokens >
        numeric_limits<long long>::max() - cantidadTokens) {
        return false;
    }

    cliente->tokens += cantidadTokens;
    return true;
}

bool Clientes::registrarConsumo(
    const string& nombre, double consumo) {

    if (!isfinite(consumo) || consumo <= 0) {
        return false;
    }

    Cliente* cliente = buscarInterno(nombre);

    if (cliente == nullptr) {
        return false;
    }

    if (!isfinite(cliente->consumoAcumulado + consumo)) {
        return false;
    }

    cliente->consumoAcumulado += consumo;
    return true;
}

int Clientes::getCantidad() const {
    return cantidad;
}

const Cliente* Clientes::getPrimero() const {
    return primero;
}

void Clientes::mostrarClientes() const {
    const Cliente* actual = primero;

    if (actual == nullptr) {
        cout << "No hay clientes registrados.\n";
        return;
    }

    while (actual != nullptr) {
        cout << "Cliente: " << actual->nombre
             << " | Consumo: " << actual->consumoAcumulado
             << " kWh | Eco-Tokens: " << actual->tokens
             << endl;

        actual = actual->siguiente;
    }
}
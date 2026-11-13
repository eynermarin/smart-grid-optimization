#include "Beneficios.h"
#include "Clientes.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <utility>

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
bool Clientes::restaurarCliente(
    const string& nombre,
    double consumo,
    long long tokens,
    const vector<string>& subestaciones,
    const vector<string>& beneficios) {

    if (nombre.find_first_not_of(" \t\n\r") == string::npos ||
        !isfinite(consumo) || consumo < 0 || tokens < 0 ||
        buscarCliente(nombre) != nullptr) {
        return false;
    }

    for (size_t i = 0; i < subestaciones.size(); i++) {
        const string& valor = subestaciones[i];

        if (valor.find_first_not_of(" \t\n\r") == string::npos ||
            find(subestaciones.begin(), subestaciones.begin() + i,
                 valor) != subestaciones.begin() + i) {
            return false;
        }
    }

    for (const string& beneficio : beneficios) {
        if (beneficio.find_first_not_of(" \t\n\r") == string::npos) {
            return false;
        }
    }

    if (!agregarCliente(nombre)) {
        return false;
    }

    Cliente* cliente = buscarInterno(nombre);

    cliente->consumoAcumulado = consumo;
    cliente->tokens = tokens;
    cliente->subestacionesConectadas = subestaciones;
    cliente->beneficiosCanjeados = beneficios;

    return true;
}

void Clientes::intercambiar(Clientes& otro) {
    swap(primero, otro.primero);
    swap(cantidad, otro.cantidad);
}

bool Clientes::canjearBeneficio(
    const string& nombreCliente,
    const string& nombreBeneficio,
    const Beneficios& catalogo) {

    // Verificar existencia del cliente
    Cliente* cliente = buscarInterno(nombreCliente);

    if (cliente == nullptr) {
        return false;
    }

    // Verificar existencia del beneficio
    const Beneficio* beneficio =
        catalogo.buscarBeneficio(nombreBeneficio);

    if (beneficio == nullptr) {
        return false;
    }

    // Validar costo y saldo disponible
    if (beneficio->costoTokens <= 0 ||
        cliente->tokens < beneficio->costoTokens) {
        return false;
    }

    // Registrar primero el beneficio en el historial
    cliente->beneficiosCanjeados.push_back(
        beneficio->nombre
    );

    // Descontar Eco-Tokens solamente si se registro
    cliente->tokens -= beneficio->costoTokens;

    return true;
}
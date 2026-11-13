#include "OptimizadorTokens.h"
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
    const vector<string>& beneficios,
    const vector<string>& rutas) {

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

    for (const string& ruta : rutas) {
        if (ruta.find_first_not_of(" \t\n\r") == string::npos) {
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
    cliente->rutasUtilizadas = rutas;

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

bool Clientes::registrarRutaOptimizada(
    const string& nombreCliente,
    const ResultadoEcoRuta& resultado) {

    Cliente* cliente = buscarInterno(nombreCliente);

    if (cliente == nullptr ||
        !resultado.encontrada ||
        resultado.tokens <= 0 ||
        resultado.ruta.nodos.size() < 2 ||
        resultado.ruta.arcos.size() + 1 !=
            resultado.ruta.nodos.size()) {
        return false;
    }

    if (cliente->tokens >
        numeric_limits<long long>::max() - resultado.tokens) {
        return false;
    }

    // Verificar que los arcos coincidan con el camino.
    for (size_t i = 0; i < resultado.ruta.arcos.size(); i++) {
        const Arco* arco = resultado.ruta.arcos[i];

        if (arco == nullptr ||
            arco->origen != resultado.ruta.nodos[i] ||
            arco->destino != resultado.ruta.nodos[i + 1]) {
            return false;
        }
    }

    // Construir identificador de la ruta.
    string descripcion;

    for (size_t i = 0; i < resultado.ruta.nodos.size(); i++) {
        const Nodo* nodo = resultado.ruta.nodos[i];

        if (nodo == nullptr) {
            return false;
        }

        if (i > 0) {
            descripcion += " -> ";
        }

        descripcion += nodo->nombre +
            " (" + nodo->region + ")";
    }

    // Preparar los cambios antes de aplicarlos.
    vector<string> nuevasSubestaciones =
        cliente->subestacionesConectadas;

    vector<string> nuevoHistorial =
        cliente->rutasUtilizadas;

    for (const Nodo* nodo : resultado.ruta.nodos) {
        if (nodo->tipo != "subestacion") {
            continue;
        }

        string identificador =
            nodo->nombre + " (" + nodo->region + ")";

        if (find(
            nuevasSubestaciones.begin(),
            nuevasSubestaciones.end(),
            identificador
        ) == nuevasSubestaciones.end()) {
            nuevasSubestaciones.push_back(identificador);
        }
    }

    nuevoHistorial.push_back(descripcion);

    // Aplicar los cambios una vez validados.
    cliente->subestacionesConectadas.swap(
        nuevasSubestaciones);

    cliente->rutasUtilizadas.swap(nuevoHistorial);

    cliente->tokens += resultado.tokens;

    return true;
}
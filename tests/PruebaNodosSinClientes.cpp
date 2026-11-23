#include "ReportesRed.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

bool verificar(bool condicion, const string& mensaje) {
    if (!condicion) {
        cerr << "FALLO - " << mensaje << '\n';
        return false;
    }

    cout << "OK - " << mensaje << '\n';
    return true;
}

bool contiene(
    const vector<const Nodo*>& nodos,
    const string& nombre) {

    for (const Nodo* nodo : nodos) {
        if (nodo->nombre == nombre) {
            return true;
        }
    }

    return false;
}

int main() {
    cout << "===== PRUEBAS NODOS SIN CLIENTES =====\n";

    Grafo grafo;
    Clientes clientes;

    if (!verificar(
        grafo.agregarNodo("A", "Norte", "generador") &&
        grafo.agregarNodo("B", "Norte", "subestacion") &&
        grafo.agregarNodo("C", "Sur", "subestacion") &&
        grafo.agregarNodo("D", "Sur", "subestacion") &&
        grafo.agregarArco(
            "A", "Norte", "B", "Norte",
            "alta_tension", 10, 2, 100, 1) &&
        grafo.agregarArco(
            "B", "Norte", "C", "Sur",
            "subterraneo", 15, 3, 100, 1),
        "Grafo de cuatro nodos creado")) {
        return 1;
    }

    if (!verificar(
        clientes.restaurarCliente(
            "Hospital", 100, 50,
            {"B"}, {}, {}
        ),
        "Cliente asociado a B por nombre simple")) {
        return 1;
    }

    auto sinRegistro =
        ReportesRed::obtenerNodosSinClientes(
            grafo, clientes
        );

    if (!verificar(
        sinRegistro.size() == 3 &&
        contiene(sinRegistro, "A") &&
        contiene(sinRegistro, "C") &&
        contiene(sinRegistro, "D") &&
        !contiene(sinRegistro, "B"),
        "Tres nodos sin registro identificados")) {
        return 1;
    }

    if (!verificar(
        clientes.restaurarCliente(
            "Escuela", 20, 10,
            {"C (Sur)"}, {}, {}
        ),
        "Segundo cliente asociado por nombre y region")) {
        return 1;
    }

    sinRegistro =
        ReportesRed::obtenerNodosSinClientes(
            grafo, clientes
        );

    if (!verificar(
        sinRegistro.size() == 2 &&
        contiene(sinRegistro, "A") &&
        contiene(sinRegistro, "D") &&
        !contiene(sinRegistro, "C"),
        "Las asociaciones actualizan el reporte")) {
        return 1;
    }

    ostringstream captura;
    streambuf* original = cout.rdbuf(captura.rdbuf());

    ReportesRed::mostrarNodosSinClientes(
        grafo, clientes
    );

    cout.rdbuf(original);

    string salida = captura.str();

    if (!verificar(
        salida.find(
            "REPORTE DE NODOS SIN REGISTRO EN CLIENTES"
        ) != string::npos &&
        salida.find(
            "Nodos sin registro en clientes: 2"
        ) != string::npos &&
        salida.find("Nombre: A") != string::npos &&
        salida.find("Nombre: D") != string::npos,
        "Reporte impreso correctamente")) {
        return 1;
    }

    Clientes vacios;

    if (!verificar(
        ReportesRed::obtenerNodosSinClientes(
            grafo, vacios
        ).size() == 4,
        "Sin clientes, ningun nodo esta registrado")) {
        return 1;
    }

    if (!verificar(
        grafo.getCantidadNodos() == 4 &&
        grafo.getCantidadArcos() == 2 &&
        clientes.getCantidad() == 2,
        "La consulta no modifica los datos")) {
        return 1;
    }

    cout << "\nPRUEBA NODOS SIN CLIENTES EXITOSA\n";
    return 0;
}
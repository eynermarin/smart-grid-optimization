#include "ReporteClientes.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

bool verificar(bool condicion, const string& mensaje) {
    if (!condicion) {
        cerr << "FALLO - " << mensaje << '\n';
        return false;
    }

    cout << "OK - " << mensaje << '\n';
    return true;
}

int main() {
    cout << "===== PRUEBAS FILTRO DE CLIENTES =====\n";

    Clientes clientes;

    if (!verificar(
        clientes.restaurarCliente(
            "Hospital", 100, 300,
            {"Central"}, {},
            {"A -> B"}
        ) &&
        clientes.restaurarCliente(
            "Escuela", 50, 200,
            {"Sur"}, {}, {}
        ) &&
        clientes.restaurarCliente(
            "Comercio", 25, 100,
            {}, {"Descuento"}, {}
        ),
        "Tres clientes creados correctamente")) {
        return 1;
    }

    ostringstream captura;
    streambuf* original = cout.rdbuf(captura.rdbuf());

    ReporteClientes::mostrarTodos(clientes);

    cout.rdbuf(original);

    string salida = captura.str();

    if (!verificar(
        salida.find("Clientes registrados: 3") != string::npos &&
        salida.find("Consumo total acumulado: 175.00") != string::npos &&
        salida.find("Eco-Tokens totales: 600") != string::npos,
        "Estadisticas generales conservadas")) {
        return 1;
    }

    if (!verificar(
        salida.find("Cliente: Hospital") != string::npos &&
        salida.find("Cliente: Comercio") != string::npos,
        "Clientes con historial incluidos")) {
        return 1;
    }

    if (!verificar(
        salida.find("Cliente: Escuela") == string::npos,
        "Cliente sin historial omitido")) {
        return 1;
    }

    if (!verificar(
        salida.find("Clientes con historial: 2") != string::npos,
        "Cantidad de clientes con historial correcta")) {
        return 1;
    }

    Clientes sinHistorial;

    sinHistorial.restaurarCliente(
        "Cliente Nuevo", 10, 50,
        {"Central"}, {}, {}
    );

    ostringstream segundaCaptura;
    original = cout.rdbuf(segundaCaptura.rdbuf());

    ReporteClientes::mostrarTodos(sinHistorial);

    cout.rdbuf(original);

    if (!verificar(
        segundaCaptura.str().find(
            "No hay clientes con historial registrado."
        ) != string::npos,
        "Se informa cuando nadie tiene historial")) {
        return 1;
    }

    if (!verificar(
        clientes.buscarCliente("Hospital")->tokens == 300 &&
        clientes.buscarCliente("Escuela")->tokens == 200 &&
        clientes.buscarCliente("Comercio")->tokens == 100,
        "El reporte no modifica los saldos")) {
        return 1;
    }

    cout << "\nPRUEBA FILTRO DE CLIENTES EXITOSA\n";
    return 0;
}
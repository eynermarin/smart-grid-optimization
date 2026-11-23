#include "Clientes.h"
#include "ReporteClientes.h"

#include <cmath>
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
    cout << "===== PRUEBAS REPORTE DE CLIENTES =====\n";

    Clientes clientes;

    if (!verificar(
        clientes.restaurarCliente(
            "Hospital General",
            120.5,
            450,
            {"Subestacion Central"},
            {"Descuento de energia"},
            {"A (Norte) -> B (Sur)",
             "A (Norte) -> C (Sur)"}
        ) &&
        clientes.restaurarCliente(
            "Escuela Central",
            30.0,
            100,
            {"Subestacion Sur"},
            {},
            {}
        ) &&
        clientes.restaurarCliente(
            "Comercio Local",
            0.0,
            0,
            {},
            {},
            {}
        ),
        "Tres clientes de prueba creados")) {
        return 1;
    }

    ResumenClientes resumen =
        ReporteClientes::calcularResumen(clientes);

    if (!verificar(
        resumen.cantidadClientes == 3 &&
        clientes.getCantidad() == 3,
        "Cantidad de clientes correcta")) {
        return 1;
    }

    if (!verificar(
        fabs(resumen.consumoTotal - 150.5) < 0.00001,
        "Consumo total calculado correctamente")) {
        return 1;
    }

    if (!verificar(
        resumen.tokensTotales == 550,
        "Eco-Tokens totales correctos")) {
        return 1;
    }

    if (!verificar(
        resumen.rutasRegistradas == 2,
        "Dos rutas registradas contabilizadas")) {
        return 1;
    }

    if (!verificar(
        resumen.beneficiosCanjeados == 1,
        "Beneficios canjeados contabilizados")) {
        return 1;
    }

    ostringstream captura;
    streambuf* original = cout.rdbuf(captura.rdbuf());

    ReporteClientes::mostrarTodos(clientes);
    bool encontrado = ReporteClientes::mostrarHistorial(
        clientes, "Hospital General");

    bool inexistente = ReporteClientes::mostrarHistorial(
        clientes, "Cliente Inexistente");

    cout.rdbuf(original);

    const string salida = captura.str();

    if (!verificar(
        salida.find("REPORTE GENERAL DE CLIENTES") != string::npos &&
        salida.find("Clientes registrados: 3") != string::npos &&
        salida.find("Consumo total acumulado: 150.50") != string::npos &&
        salida.find("Eco-Tokens totales: 550") != string::npos,
        "El reporte general muestra las estadisticas")) {
        return 1;
    }

    if (!verificar(
        salida.find("Hospital General") != string::npos &&
        salida.find("Escuela Central") != string::npos &&
        salida.find("Comercio Local") != string::npos,
        "Se muestran todos los clientes")) {
        return 1;
    }

    if (!verificar(
        encontrado &&
        salida.find("HISTORIAL DEL CLIENTE") != string::npos &&
        salida.find("A (Norte) -> B (Sur)") != string::npos &&
        salida.find("A (Norte) -> C (Sur)") != string::npos,
        "Historial individual mostrado correctamente")) {
        return 1;
    }

    if (!verificar(
        !inexistente &&
        salida.find("Cliente no encontrado.") != string::npos,
        "Cliente inexistente rechazado")) {
        return 1;
    }

    Clientes vacios;

    ResumenClientes resumenVacio =
        ReporteClientes::calcularResumen(vacios);

    if (!verificar(
        resumenVacio.cantidadClientes == 0 &&
        resumenVacio.consumoTotal == 0 &&
        resumenVacio.tokensTotales == 0 &&
        resumenVacio.rutasRegistradas == 0,
        "Lista de clientes vacia manejada correctamente")) {
        return 1;
    }

    if (!verificar(
        clientes.getCantidad() == 3 &&
        clientes.buscarCliente("Hospital General")->tokens == 450,
        "El reporte no modifica los datos")) {
        return 1;
    }

    cout << "\nPRUEBA REPORTE DE CLIENTES EXITOSA\n";
    return 0;
}
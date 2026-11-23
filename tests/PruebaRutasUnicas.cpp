#include "ReporteRutas.h"

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
    cout << "===== PRUEBAS DE RUTAS UNICAS =====\n";

    Clientes clientes;

    if (!verificar(
        clientes.restaurarCliente(
            "Hospital", 100, 300, {}, {},
            {"A -> B -> D", "A -> B -> D", "A -> C -> D"}
        ) &&
        clientes.restaurarCliente(
            "Escuela", 50, 200, {}, {},
            {"A -> D"}
        ),
        "Clientes e historiales creados")) {
        return 1;
    }

    ostringstream captura;
    streambuf* original = cout.rdbuf(captura.rdbuf());

    ReporteRutas::mostrarRutasUnicas(clientes);

    cout.rdbuf(original);

    string salida = captura.str();

    if (!verificar(
        salida.find("RUTAS UTILIZADAS UNA SOLA VEZ") != string::npos &&
        salida.find("Total de rutas unicas: 2") != string::npos,
        "Dos rutas unicas encontradas")) {
        return 1;
    }

    if (!verificar(
        salida.find("Ruta: A -> C -> D") != string::npos &&
        salida.find("Ruta: A -> D") != string::npos,
        "Se muestran las rutas utilizadas una vez")) {
        return 1;
    }

    if (!verificar(
        salida.find("Ruta: A -> B -> D") == string::npos,
        "Se excluye la ruta utilizada dos veces")) {
        return 1;
    }

    Clientes repetidas;

    if (!verificar(
        repetidas.restaurarCliente(
            "Comercio", 20, 50, {}, {},
            {"A -> B", "A -> B"}
        ),
        "Cliente con ruta repetida creado")) {
        return 1;
    }

    ostringstream segundaCaptura;
    original = cout.rdbuf(segundaCaptura.rdbuf());

    ReporteRutas::mostrarRutasUnicas(repetidas);

    cout.rdbuf(original);

    if (!verificar(
        segundaCaptura.str().find(
            "No existen rutas utilizadas una sola vez."
        ) != string::npos,
        "Se detecta correctamente la ausencia de rutas unicas")) {
        return 1;
    }

    if (!verificar(
        clientes.buscarCliente("Hospital")->tokens == 300 &&
        clientes.buscarCliente("Escuela")->tokens == 200,
        "El reporte no modifica los saldos")) {
        return 1;
    }

    cout << "\nPRUEBA DE RUTAS UNICAS EXITOSA\n";
    return 0;
}
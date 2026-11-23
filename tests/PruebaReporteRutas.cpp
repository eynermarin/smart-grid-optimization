#include "Clientes.h"
#include "ReporteRutas.h"

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

bool tieneFrecuencia(
    const vector<FrecuenciaRuta>& frecuencias,
    const string& ruta,
    int usos) {

    for (const FrecuenciaRuta& actual : frecuencias) {
        if (actual.recorrido == ruta && actual.usos == usos) {
            return true;
        }
    }

    return false;
}

int main() {
    cout << "===== PRUEBAS REPORTE DE RUTAS =====\n";

    const string ruta1 = "A -> B -> D";
    const string ruta2 = "A -> C -> D";
    const string ruta3 = "A -> D";

    Clientes clientes;

    if (!verificar(
        clientes.restaurarCliente(
            "Hospital",
            100.0,
            300,
            {},
            {},
            {ruta1, ruta1, ruta2}
        ) &&
        clientes.restaurarCliente(
            "Escuela",
            80.0,
            200,
            {},
            {},
            {ruta1, ruta3}
        ) &&
        clientes.restaurarCliente(
            "Comercio",
            50.0,
            150,
            {},
            {},
            {ruta1, ruta2}
        ) &&
        clientes.restaurarCliente(
            "Universidad",
            120.0,
            500,
            {},
            {},
            {}
        ),
        "Cuatro clientes de prueba creados")) {
        return 1;
    }

    vector<FrecuenciaRuta> frecuencias =
        ReporteRutas::calcularFrecuencias(clientes);

    if (!verificar(
        frecuencias.size() == 3,
        "Tres rutas diferentes identificadas")) {
        return 1;
    }

    if (!verificar(
        tieneFrecuencia(frecuencias, ruta1, 4) &&
        tieneFrecuencia(frecuencias, ruta2, 2) &&
        tieneFrecuencia(frecuencias, ruta3, 1),
        "Frecuencias 4, 2 y 1 calculadas correctamente")) {
        return 1;
    }

    if (!verificar(
        frecuencias[0].recorrido == ruta1 &&
        frecuencias[0].usos == 4 &&
        frecuencias[1].recorrido == ruta2 &&
        frecuencias[2].recorrido == ruta3,
        "Ranking ordenado de mayor a menor")) {
        return 1;
    }

    ostringstream captura;
    streambuf* original = cout.rdbuf(captura.rdbuf());

    ReporteRutas::mostrarRanking(clientes);
    ReporteRutas::mostrarMasUtilizadas(clientes);
    ReporteRutas::mostrarMenosUtilizadas(clientes);

    cout.rdbuf(original);

    const string salida = captura.str();

    if (!verificar(
        salida.find("RANKING DE RUTAS UTILIZADAS") != string::npos &&
        salida.find("Rutas diferentes: 3") != string::npos &&
        salida.find("Total de recorridos registrados: 7") != string::npos,
        "Reporte general muestra los totales correctos")) {
        return 1;
    }

    if (!verificar(
        salida.find("RUTAS MAS UTILIZADAS") != string::npos &&
        salida.find("Mayor frecuencia: 4") != string::npos &&
        salida.find("Ruta: A -> B -> D") != string::npos,
        "Ruta mas utilizada identificada")) {
        return 1;
    }

    if (!verificar(
        salida.find("RUTAS MENOS UTILIZADAS") != string::npos &&
        salida.find("Menor frecuencia: 1") != string::npos &&
        salida.find("Ruta: A -> D") != string::npos,
        "Ruta menos utilizada identificada")) {
        return 1;
    }

    // Comprobar empate en la frecuencia maxima.
    Clientes empate;

    if (!verificar(
        empate.restaurarCliente(
            "Cliente Uno", 0, 10, {}, {},
            {ruta1, ruta1, ruta2, ruta2}
        ) &&
        empate.restaurarCliente(
            "Cliente Dos", 0, 20, {}, {},
            {ruta3}
        ),
        "Clientes para probar empates creados")) {
        return 1;
    }

    ostringstream capturaEmpate;
    original = cout.rdbuf(capturaEmpate.rdbuf());

    ReporteRutas::mostrarMasUtilizadas(empate);

    cout.rdbuf(original);

    string salidaEmpate = capturaEmpate.str();

    if (!verificar(
        salidaEmpate.find("Mayor frecuencia: 2") != string::npos &&
        salidaEmpate.find("Ruta: A -> B -> D") != string::npos &&
        salidaEmpate.find("Ruta: A -> C -> D") != string::npos,
        "Empate entre rutas mas utilizadas manejado")) {
        return 1;
    }

    Clientes vacios;

    if (!verificar(
        ReporteRutas::calcularFrecuencias(vacios).empty(),
        "Lista de clientes vacia manejada")) {
        return 1;
    }

    if (!verificar(
        clientes.getCantidad() == 4 &&
        clientes.buscarCliente("Hospital")->tokens == 300 &&
        clientes.buscarCliente("Escuela")->tokens == 200,
        "Los reportes no modifican los clientes")) {
        return 1;
    }

    cout << "\nPRUEBA REPORTE DE RUTAS EXITOSA\n";
    return 0;
}
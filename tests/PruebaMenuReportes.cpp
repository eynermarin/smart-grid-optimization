#include "MenuReportes.h"

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
    cout << "===== PRUEBAS MENU DE REPORTES =====\n";

    Grafo grafo;
    Clientes clientes;

    if (!verificar(
        grafo.agregarNodo("A", "Norte", "generador") &&
        grafo.agregarNodo("B", "Sur", "subestacion") &&
        grafo.agregarNodo("C", "Este", "subestacion") &&
        grafo.agregarArco(
            "A", "Norte", "B", "Sur",
            "alta_tension", 20, 5, 100, 1),
        "Grafo preparado con un nodo aislado")) {
        return 1;
    }

    if (!verificar(
        clientes.restaurarCliente(
            "Hospital", 100.0, 300,
            {"B (Sur)"},
            {"Descuento"},
            {"A -> B", "A -> B", "A -> C"}
        ) &&
        clientes.restaurarCliente(
            "Escuela", 50.0, 150,
            {},
            {},
            {"A -> C"}
        ),
        "Dos clientes con historiales preparados")) {
        return 1;
    }

    istringstream entrada(
        "1\n"
        "2\nHospital\n"
        "2\nNoExiste\n"
        "3\n"
        "4\n"
        "5\n"
        "6\n"
        "9\n"
        "0\n"
    );

    ostringstream captura;

    streambuf* entradaOriginal = cin.rdbuf(entrada.rdbuf());
    streambuf* salidaOriginal = cout.rdbuf(captura.rdbuf());

    menuReportes(grafo, clientes);

    cin.rdbuf(entradaOriginal);
    cout.rdbuf(salidaOriginal);
    cin.clear();

    const string texto = captura.str();

    const string mensajes[] = {
        "REPORTES DEL SISTEMA",
        "REPORTE GENERAL DE CLIENTES",
        "Clientes registrados: 2",
        "Consumo total acumulado: 150.00",
        "Eco-Tokens totales: 450",
        "HISTORIAL DEL CLIENTE",
        "Cliente: Hospital",
        "Cliente no encontrado.",
        "REPORTE DE NODOS SIN REGISTRO EN CLIENTES",
        "Nodos sin registro en clientes: 2",
        "Nombre: C",
        "RANKING DE RUTAS UTILIZADAS",
        "Rutas diferentes: 2",
        "Total de recorridos registrados: 4",
        "RUTAS MAS UTILIZADAS",
        "Mayor frecuencia: 2",
        "RUTAS MENOS UTILIZADAS",
        "Menor frecuencia: 2",
        "Opcion invalida.",
        "Regresando al menu principal..."
    };

    for (const string& mensaje : mensajes) {
        if (!verificar(
            texto.find(mensaje) != string::npos,
            mensaje)) {
            return 1;
        }
    }

    if (!verificar(
        grafo.getCantidadNodos() == 3 &&
        grafo.getCantidadArcos() == 1 &&
        clientes.getCantidad() == 2 &&
        clientes.buscarCliente("Hospital")->tokens == 300 &&
        clientes.buscarCliente("Hospital")->rutasUtilizadas.size() == 3,
        "Los reportes no modificaron los datos")) {
        return 1;
    }

    cout << "\nPRUEBA MENU DE REPORTES EXITOSA\n";
    return 0;
}
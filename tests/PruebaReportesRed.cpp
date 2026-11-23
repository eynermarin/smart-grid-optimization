#include "Grafo.h"
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

bool contieneNodo(
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
    cout << "===== PRUEBAS REPORTES DE RED =====\n";

    Grafo grafo;

    if (!verificar(
        grafo.agregarNodo("A", "Norte", "generador") &&
        grafo.agregarNodo("B", "Norte", "subestacion") &&
        grafo.agregarNodo("C", "Sur", "subestacion") &&
        grafo.agregarNodo("D", "Este", "subestacion") &&
        grafo.agregarNodo("E", "Oeste", "generador"),
        "Cinco nodos de prueba creados")) {
        return 1;
    }

    if (!verificar(
        grafo.agregarArco(
            "A", "Norte", "B", "Norte",
            "alta_tension", 12, 3, 100, 1.0) &&
        grafo.agregarArco(
            "B", "Norte", "C", "Sur",
            "subterraneo", 18, 4, 90, 1.5),
        "Dos conexiones dirigidas creadas")) {
        return 1;
    }

    vector<const Nodo*> inactivos =
        ReportesRed::obtenerNodosInactivos(grafo);

    if (!verificar(
        inactivos.size() == 2,
        "Se identifican exactamente dos nodos inactivos")) {
        return 1;
    }

    if (!verificar(
        contieneNodo(inactivos, "D") &&
        contieneNodo(inactivos, "E"),
        "Los nodos D y E estan inactivos")) {
        return 1;
    }

    if (!verificar(
        !contieneNodo(inactivos, "A") &&
        !contieneNodo(inactivos, "B"),
        "Nodos con conexiones salientes no son inactivos")) {
        return 1;
    }

    if (!verificar(
        !contieneNodo(inactivos, "C"),
        "Nodo solo con conexiones entrantes sigue activo")) {
        return 1;
    }

    ostringstream captura;
    streambuf* original = cout.rdbuf(captura.rdbuf());

    ReportesRed::mostrarNodosInactivos(grafo);

    cout.rdbuf(original);

    string salida = captura.str();

    if (!verificar(
        salida.find("REPORTE DE NODOS INACTIVOS") != string::npos &&
        salida.find("Nodos inactivos encontrados: 2") != string::npos &&
        salida.find("Nombre: D") != string::npos &&
        salida.find("Nombre: E") != string::npos,
        "El reporte muestra correctamente los nodos aislados")) {
        return 1;
    }

    // Conectar D y E para dejar cero nodos inactivos.
    if (!verificar(
        grafo.agregarArco(
            "C", "Sur", "D", "Este",
            "alta_tension", 20, 5, 80, 2.0) &&
        grafo.agregarArco(
            "D", "Este", "E", "Oeste",
            "red_local", 8, 2, 60, 0.5),
        "Los nodos aislados fueron conectados")) {
        return 1;
    }

    if (!verificar(
        ReportesRed::obtenerNodosInactivos(grafo).empty(),
        "Red totalmente conectada sin nodos inactivos")) {
        return 1;
    }

    if (!verificar(
        grafo.getCantidadNodos() == 5 &&
        grafo.getCantidadArcos() == 4,
        "El reporte no modifica el grafo")) {
        return 1;
    }

    cout << "\nPRUEBA REPORTES DE RED EXITOSA\n";
    return 0;
}
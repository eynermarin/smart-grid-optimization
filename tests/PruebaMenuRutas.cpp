#include "MenuRutas.h"

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
    cout << "===== PRUEBAS MENU DE RUTAS =====\n";

    Grafo grafo;

    if (!verificar(
        grafo.agregarNodo("A", "Norte", "generador") &&
        grafo.agregarNodo("B", "Norte", "subestacion") &&
        grafo.agregarNodo("C", "Sur", "subestacion") &&
        grafo.agregarNodo("D", "Sur", "subestacion"),
        "Nodos de prueba creados")) {
        return 1;
    }

    if (!verificar(
        grafo.agregarArco(
            "A", "Norte", "B", "Norte",
            "alta_tension", 4, 2, 100, 5) &&
        grafo.agregarArco(
            "B", "Norte", "D", "Sur",
            "alta_tension", 4, 2, 100, 5) &&
        grafo.agregarArco(
            "A", "Norte", "C", "Sur",
            "subterraneo", 10, 3, 100, 1) &&
        grafo.agregarArco(
            "C", "Sur", "D", "Sur",
            "subterraneo", 10, 3, 100, 1) &&
        grafo.agregarArco(
            "A", "Norte", "D", "Sur",
            "alta_tension", 30, 7, 100, 0.5),
        "Conexiones dirigidas creadas")) {
        return 1;
    }

    istringstream entrada(
        "1\nA\nNorte\nD\nSur\n3\n"
        "1\nA\nNorte\nD\nSur\n2\n"
        "2\nA\nNorte\nD\nSur\n"
        "3\nA\nNorte\nD\nSur\n"
        "1\nNoExiste\nNorte\nD\nSur\n"
        "1\nA\nNorte\nNoExiste\nSur\n"
        "1\nA\nNorte\nD\nSur\n9\n"
        "9\n"
        "0\n"
    );

    ostringstream salida;

    streambuf* entradaOriginal = cin.rdbuf(entrada.rdbuf());
    streambuf* salidaOriginal = cout.rdbuf(salida.rdbuf());

    menuRutas(grafo);

    cin.rdbuf(entradaOriginal);
    cout.rdbuf(salidaOriginal);
    cin.clear();

    const string texto = salida.str();

    const string mensajes[] = {
        "CONSULTAS DE RUTAS",
        "TODAS LAS RUTAS SIMPLES",
        "Rutas encontradas: 3",
        "Rutas encontradas: 1",
        "RUTA DE MENOR DISTANCIA:",
        "RUTA DE MENORES PERDIDAS:",
        "Distancia total: 8 km",
        "Distancia total: 30 km",
        "Suma de perdidas por tramo: 0.5",
        "Nodo origen no encontrado.",
        "Nodo destino no encontrado.",
        "Filtro invalido.",
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
        grafo.getCantidadNodos() == 4 &&
        grafo.getCantidadArcos() == 5,
        "El menu no modifico el grafo")) {
        return 1;
    }

    cout << "\nPRUEBA MENU DE RUTAS EXITOSA\n";
    return 0;
}
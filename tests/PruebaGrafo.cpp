#include <iostream>
#include <cassert>
#include "Grafo.h"

using namespace std;

int main() {
    Grafo grafo;

    cout << "===== PRUEBAS DE GRAFO =====\n";

    assert(grafo.getCantidadNodos() == 0);
    cout << "OK - Grafo inicialmente vacio\n";

    assert(grafo.agregarNodo("Planta Solar Alpha", "Zona Norte", "generador"));
    assert(grafo.agregarNodo("Subestacion Central", "Zona Norte", "subestacion"));
    assert(grafo.agregarNodo("Subestacion Sur", "Zona Sur", "subestacion"));
    cout << "OK - Insercion de nodos\n";

    assert(grafo.getCantidadNodos() == 3);
    cout << "OK - Cantidad de nodos correcta\n";

    assert(grafo.buscarNodo("Planta Solar Alpha", "Zona Norte") != nullptr);
    cout << "OK - Busqueda de nodo existente\n";

    assert(grafo.buscarNodo("Nodo Falso", "Zona Norte") == nullptr);
    cout << "OK - Busqueda de nodo inexistente\n";

    assert(!grafo.agregarNodo("Planta Solar Alpha", "Zona Norte", "generador"));
    cout << "OK - Nodo duplicado rechazado\n";

    assert(!grafo.agregarNodo("", "Zona Norte", "generador"));
    cout << "OK - Datos vacios rechazados\n";

    cout << "\nNodos registrados:\n";
    grafo.mostrarNodos();

    cout << "\nRESULTADO: 7 comprobaciones correctas.\n";
    cout << "PRUEBA DE GRAFO EXITOSA\n";

    return 0;
}

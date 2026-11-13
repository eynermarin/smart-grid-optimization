#include <iostream>
#include <cassert>
#include "Grafo.h"

using namespace std;

int main() {
    Grafo grafo;

    cout << "===== PRUEBAS DE ARCOS =====\n";

    assert(grafo.agregarNodo("Planta A", "Norte", "generador"));
    assert(grafo.agregarNodo("Subestacion B", "Norte", "subestacion"));
    assert(grafo.agregarNodo("Subestacion C", "Sur", "subestacion"));
    cout << "OK - Nodos creados\n";

    assert(grafo.agregarArco(
        "Planta A", "Norte", "Subestacion B", "Norte",
        "alta_tension", 45, 10, 100, 2.5));
    cout << "OK - Primer arco registrado\n";

    assert(grafo.agregarArco(
        "Planta A", "Norte", "Subestacion C", "Sur",
        "subterraneo", 70, 15, 80, 3.0));
    cout << "OK - Segundo arco registrado\n";

    assert(grafo.getCantidadArcos() == 2);
    cout << "OK - Cantidad de arcos correcta\n";

    const Nodo* planta = grafo.buscarNodo("Planta A", "Norte");
    const Nodo* subB = grafo.buscarNodo("Subestacion B", "Norte");
    const Nodo* subC = grafo.buscarNodo("Subestacion C", "Sur");

    assert(planta != nullptr && subB != nullptr && subC != nullptr);

    int salientes = 0;
    for (const Arco* a = planta->primerSaliente;
         a != nullptr; a = a->siguienteSaliente) {
        assert(a->origen == planta);
        salientes++;
    }
    assert(salientes == 2);
    cout << "OK - Enlaces salientes correctos\n";

    assert(subB->primerEntrante != nullptr);
    assert(subB->primerEntrante->origen == planta);
    assert(subC->primerEntrante != nullptr);
    assert(subC->primerEntrante->origen == planta);
    cout << "OK - Enlaces entrantes correctos\n";

    assert(grafo.buscarArco(
        "Planta A", "Norte", "Subestacion B", "Norte",
        "alta_tension") != nullptr);
    cout << "OK - Busqueda de arco\n";

    assert(!grafo.agregarArco(
        "Planta A", "Norte", "Subestacion B", "Norte",
        "alta_tension", 45, 10, 100, 2.5));
    cout << "OK - Arco duplicado rechazado\n";

    assert(!grafo.agregarArco(
        "Planta A", "Norte", "Nodo Falso", "Sur",
        "alta_tension", 45, 10, 100, 2.5));
    cout << "OK - Destino inexistente rechazado\n";

    assert(!grafo.agregarArco(
        "Planta A", "Norte", "Subestacion B", "Norte",
        "red_local", -5, 10, 100, 2.5));
    cout << "OK - Distancia negativa rechazada\n";

    assert(!grafo.agregarArco(
        "Planta A", "Norte", "Subestacion B", "Norte",
        "red_local", 30, 10, 100, 150));
    cout << "OK - Perdida invalida rechazada\n";

    assert(grafo.getCantidadArcos() == 2);
    cout << "OK - Los errores no modificaron el grafo\n";

    cout << "\nConexiones registradas:\n";
    grafo.mostrarArcos();

    cout << "\nPRUEBA DE ARCOS EXITOSA\n";
    return 0;
}

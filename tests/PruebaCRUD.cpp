#include <iostream>
#include <cassert>
#include "Grafo.h"

using namespace std;

int contarSalientes(const Nodo* nodo) {
    int cantidad = 0;

    for (const Arco* a = nodo->primerSaliente;
         a != nullptr; a = a->siguienteSaliente) {
        assert(a->origen == nodo);
        cantidad++;
    }

    return cantidad;
}

int contarEntrantes(const Nodo* nodo) {
    int cantidad = 0;

    for (const Arco* a = nodo->primerEntrante;
         a != nullptr; a = a->siguienteEntrante) {
        assert(a->destino == nodo);
        cantidad++;
    }

    return cantidad;
}

int main() {
    Grafo grafo;

    cout << "===== PRUEBAS CRUD =====\n";

    assert(grafo.agregarNodo("A", "Norte", "generador"));
    assert(grafo.agregarNodo("B", "Norte", "subestacion"));
    assert(grafo.agregarNodo("C", "Sur", "subestacion"));
    assert(grafo.agregarNodo("D", "Sur", "subestacion"));
    cout << "OK - Nodos iniciales creados\n";

    assert(grafo.agregarArco(
        "A", "Norte", "B", "Norte",
        "alta_tension", 40, 10, 100, 2));

    assert(grafo.agregarArco(
        "B", "Norte", "C", "Sur",
        "subterraneo", 30, 8, 80, 3));

    assert(grafo.agregarArco(
        "A", "Norte", "C", "Sur",
        "red_local", 25, 5, 50, 4));

    assert(grafo.agregarArco(
        "C", "Sur", "D", "Sur",
        "alta_tension", 20, 4, 60, 1));

    assert(grafo.getCantidadArcos() == 4);
    cout << "OK - Conexiones iniciales creadas\n";

    assert(grafo.modificarNodo(
        "B", "Norte", "B Nueva", "Norte", "subestacion"));

    assert(grafo.buscarNodo("B", "Norte") == nullptr);
    assert(grafo.buscarNodo("B Nueva", "Norte") != nullptr);
    cout << "OK - Modificacion de nodo\n";

    assert(!grafo.modificarNodo(
        "B Nueva", "Norte", "A", "Norte", "subestacion"));

    assert(!grafo.modificarNodo(
        "B Nueva", "Norte", "", "Norte", "subestacion"));

    assert(!grafo.modificarNodo(
        "Inexistente", "Norte", "X", "Norte", "generador"));

    cout << "OK - Modificaciones invalidas rechazadas\n";

    assert(grafo.modificarArco(
        "A", "Norte", "B Nueva", "Norte",
        "alta_tension", 50, 12, 120, 2.5));

    const Arco* arco = grafo.buscarArco(
        "A", "Norte", "B Nueva", "Norte", "alta_tension");

    assert(arco != nullptr);
    assert(arco->distanciaKm == 50);
    assert(arco->capacidadMaxima == 120);
    cout << "OK - Modificacion de arco\n";

    assert(!grafo.modificarArco(
        "A", "Norte", "B Nueva", "Norte",
        "alta_tension", -5, 12, 120, 2.5));

    assert(arco->distanciaKm == 50);
    cout << "OK - Modificacion invalida rechazada\n";

    assert(grafo.eliminarArco(
        "A", "Norte", "C", "Sur", "red_local"));

    assert(grafo.getCantidadArcos() == 3);
    assert(grafo.buscarArco(
        "A", "Norte", "C", "Sur", "red_local") == nullptr);

    const Nodo* nodoA = grafo.buscarNodo("A", "Norte");
    const Nodo* nodoC = grafo.buscarNodo("C", "Sur");

    assert(contarSalientes(nodoA) == 1);
    assert(contarEntrantes(nodoC) == 1);
    cout << "OK - Eliminacion de arco y enlaces\n";

    assert(!grafo.eliminarArco(
        "A", "Norte", "C", "Sur", "red_local"));
    cout << "OK - Arco inexistente rechazado\n";

    assert(grafo.eliminarNodo("B Nueva", "Norte"));

    assert(grafo.getCantidadNodos() == 3);
    assert(grafo.getCantidadArcos() == 1);

    assert(grafo.buscarNodo("B Nueva", "Norte") == nullptr);
    assert(contarSalientes(nodoA) == 0);
    assert(contarEntrantes(nodoC) == 0);
    cout << "OK - Eliminacion de nodo y sus conexiones\n";

    assert(!grafo.eliminarNodo("B Nueva", "Norte"));
    cout << "OK - Nodo inexistente rechazado\n";

    assert(grafo.eliminarNodo("A", "Norte"));
    assert(grafo.eliminarNodo("C", "Sur"));
    assert(grafo.eliminarNodo("D", "Sur"));

    assert(grafo.getCantidadNodos() == 0);
    assert(grafo.getCantidadArcos() == 0);
    cout << "OK - Grafo completamente vacio\n";

    cout << "\nPRUEBA CRUD EXITOSA\n";
    return 0;
}

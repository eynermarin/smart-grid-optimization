#include "Grafo.h"

#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;

bool verificar(bool condicion, const string& mensaje) {
    if (!condicion) {
        cerr << "FALLO - " << mensaje << '\n';
        return false;
    }

    cout << "OK - " << mensaje << '\n';
    return true;
}

bool validarIntegridad(const Grafo& grafo) {
    unordered_set<const Nodo*> nodos;
    unordered_set<const Arco*> globales;
    unordered_set<const Arco*> salientes;
    unordered_set<const Arco*> entrantes;

    // 1. Comprobar la lista principal de nodos.
    const Nodo* nodo = grafo.getPrimerNodo();

    while (nodo != nullptr) {
        if (!nodos.insert(nodo).second) {
            return false; // Ciclo o nodo repetido.
        }

        nodo = nodo->siguiente;
    }

    if (static_cast<int>(nodos.size()) !=
        grafo.getCantidadNodos()) {
        return false;
    }

    // 2. Comprobar la lista global de conexiones.
    const Arco* arco = grafo.getPrimerArco();

    while (arco != nullptr) {
        if (!globales.insert(arco).second) {
            return false; // Ciclo o arco duplicado.
        }

        if (nodos.count(arco->origen) == 0 ||
            nodos.count(arco->destino) == 0) {
            return false;
        }

        arco = arco->siguienteGlobal;
    }

    if (static_cast<int>(globales.size()) !=
        grafo.getCantidadArcos()) {
        return false;
    }

    // 3. Comprobar las listas de entrada y salida.
    for (const Nodo* actual : nodos) {
        unordered_set<const Arco*> vistosSalida;
        unordered_set<const Arco*> vistosEntrada;

        arco = actual->primerSaliente;

        while (arco != nullptr) {
            if (!vistosSalida.insert(arco).second) {
                return false;
            }

            if (globales.count(arco) == 0 ||
                arco->origen != actual) {
                return false;
            }

            if (!salientes.insert(arco).second) {
                return false;
            }

            arco = arco->siguienteSaliente;
        }

        arco = actual->primerEntrante;

        while (arco != nullptr) {
            if (!vistosEntrada.insert(arco).second) {
                return false;
            }

            if (globales.count(arco) == 0 ||
                arco->destino != actual) {
                return false;
            }

            if (!entrantes.insert(arco).second) {
                return false;
            }

            arco = arco->siguienteEntrante;
        }
    }

    // Cada arco debe figurar exactamente una vez
    // en cada una de las tres estructuras.
    return globales == salientes &&
           globales == entrantes;
}

bool crearRed(Grafo& grafo) {
    return
        grafo.agregarNodo("A", "Norte", "generador") &&
        grafo.agregarNodo("B", "Norte", "subestacion") &&
        grafo.agregarNodo("C", "Sur", "subestacion") &&
        grafo.agregarNodo("D", "Sur", "subestacion") &&
        grafo.agregarNodo("E", "Este", "subestacion") &&

        grafo.agregarArco(
            "A", "Norte", "B", "Norte",
            "alta_tension", 10, 2, 100, 1) &&

        grafo.agregarArco(
            "B", "Norte", "C", "Sur",
            "subterraneo", 15, 3, 100, 1) &&

        grafo.agregarArco(
            "A", "Norte", "C", "Sur",
            "alta_tension", 20, 4, 100, 2) &&

        grafo.agregarArco(
            "C", "Sur", "D", "Sur",
            "red_local", 8, 2, 80, 0.5) &&

        grafo.agregarArco(
            "D", "Sur", "A", "Norte",
            "subterraneo", 30, 5, 90, 2) &&

        grafo.agregarArco(
            "B", "Norte", "D", "Sur",
            "alta_tension", 12, 3, 100, 1);
}

int main() {
    cout << "===== PRUEBAS INTEGRIDAD MULTILISTAS =====\n";

    {
        Grafo vacio;

        if (!verificar(
            validarIntegridad(vacio),
            "Grafo vacio con punteros consistentes")) {
            return 1;
        }
    }

    Grafo grafo;

    if (!verificar(
        crearRed(grafo),
        "Cinco nodos y seis arcos creados")) {
        return 1;
    }

    if (!verificar(
        validarIntegridad(grafo),
        "Listas global, entrante y saliente consistentes")) {
        return 1;
    }

    if (!verificar(
        grafo.getCantidadNodos() == 5 &&
        grafo.getCantidadArcos() == 6,
        "Contadores iniciales correctos")) {
        return 1;
    }

    // B tiene una entrada y dos salidas.
    // Al eliminar B deben desaparecer sus tres arcos.
    grafo.eliminarNodo("B", "Norte");

    if (!verificar(
        grafo.buscarNodo("B", "Norte") == nullptr &&
        grafo.getCantidadNodos() == 4 &&
        grafo.getCantidadArcos() == 3,
        "Eliminar un nodo retira sus arcos asociados")) {
        return 1;
    }

    if (!verificar(
        validarIntegridad(grafo),
        "Punteros consistentes tras eliminar un nodo")) {
        return 1;
    }

    // Eliminar una conexion que sigue existiendo.
    grafo.eliminarArco(
        "A", "Norte",
        "C", "Sur", "alta_tension"
    );

    if (!verificar(
        grafo.getCantidadArcos() == 2 &&
        grafo.buscarArco(
            "A", "Norte",
            "C", "Sur", "alta_tension"
        ) == nullptr,
        "Arco eliminado de la red")) {
        return 1;
    }

    if (!verificar(
        validarIntegridad(grafo),
        "Punteros consistentes tras eliminar un arco")) {
        return 1;
    }

    // Agregar un arco nuevo despues de las eliminaciones.
    if (!verificar(
        grafo.agregarArco(
            "E", "Este", "C", "Sur",
            "subterraneo", 9, 2, 70, 0.5
        ) &&
        validarIntegridad(grafo),
        "Nuevas conexiones despues de eliminar elementos")) {
        return 1;
    }

    // Repetir construccion y destruccion de grafos.
    // Cada grafo local se destruye al terminar el ciclo.
    for (int i = 0; i < 300; i++) {
        Grafo temporal;

        if (!crearRed(temporal)) {
            cerr << "FALLO - Construccion repetida "
                 << i << '\n';
            return 1;
        }

        if (!validarIntegridad(temporal)) {
            cerr << "FALLO - Integridad en iteracion "
                 << i << '\n';
            return 1;
        }

        temporal.eliminarNodo("B", "Norte");

        if (!validarIntegridad(temporal) ||
            temporal.getCantidadNodos() != 4 ||
            temporal.getCantidadArcos() != 3) {
            cerr << "FALLO - Eliminacion en iteracion "
                 << i << '\n';
            return 1;
        }
    }

    cout << "OK - 300 ciclos de construccion, "
         << "eliminacion y destruccion\n";

    cout << "\nPRUEBA INTEGRIDAD MULTILISTAS EXITOSA\n";
    return 0;
}
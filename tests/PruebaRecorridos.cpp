#include "Recorridos.h"

#include <iostream>
#include <vector>
#include <string>
#include <set>

using namespace std;

bool verificar(bool condicion, const string& mensaje) {
    if (!condicion) {
        cerr << "FALLO - " << mensaje << '\n';
        return false;
    }

    cout << "OK - " << mensaje << '\n';
    return true;
}

bool contiene(
    const vector<const Nodo*>& nodos,
    const string& nombre) {

    for (const Nodo* nodo : nodos) {
        if (nodo->nombre == nombre) {
            return true;
        }
    }

    return false;
}

bool sinRepetidos(const vector<const Nodo*>& nodos) {
    set<const Nodo*> unicos(nodos.begin(), nodos.end());
    return unicos.size() == nodos.size();
}

int main() {
    cout << "===== PRUEBAS DE BFS Y DFS =====\n";

    Grafo grafo;

    if (!verificar(
        grafo.agregarNodo("Planta A", "Norte", "generador") &&
        grafo.agregarNodo("Subestacion B", "Norte", "subestacion") &&
        grafo.agregarNodo("Subestacion C", "Sur", "subestacion") &&
        grafo.agregarNodo("Subestacion D", "Sur", "subestacion") &&
        grafo.agregarNodo("Nodo Aislado", "Este", "subestacion"),
        "Cinco nodos creados")) {
        return 1;
    }

    if (!verificar(
        grafo.agregarArco(
            "Planta A", "Norte",
            "Subestacion B", "Norte",
            "alta_tension", 10, 5, 100, 2) &&
        grafo.agregarArco(
            "Planta A", "Norte",
            "Subestacion C", "Sur",
            "subterraneo", 20, 8, 100, 3) &&
        grafo.agregarArco(
            "Subestacion B", "Norte",
            "Subestacion D", "Sur",
            "red_local", 5, 3, 50, 1) &&
        grafo.agregarArco(
            "Subestacion C", "Sur",
            "Subestacion D", "Sur",
            "red_local", 7, 4, 50, 2) &&
        grafo.agregarArco(
            "Subestacion D", "Sur",
            "Planta A", "Norte",
            "alta_tension", 12, 6, 100, 2),
        "Cinco conexiones dirigidas creadas")) {
        return 1;
    }

    vector<const Nodo*> anchura =
        Recorridos::bfs(grafo, "Planta A", "Norte");

    if (!verificar(
        anchura.size() == 4 &&
        anchura[0]->nombre == "Planta A" &&
        contiene(anchura, "Subestacion B") &&
        contiene(anchura, "Subestacion C") &&
        contiene(anchura, "Subestacion D") &&
        !contiene(anchura, "Nodo Aislado"),
        "BFS visita los cuatro nodos alcanzables")) {
        return 1;
    }

    if (!verificar(
        sinRepetidos(anchura),
        "BFS no repite nodos en un ciclo")) {
        return 1;
    }

    vector<const Nodo*> profundidad =
        Recorridos::dfs(grafo, "Planta A", "Norte");

    if (!verificar(
        profundidad.size() == 4 &&
        profundidad[0]->nombre == "Planta A" &&
        contiene(profundidad, "Subestacion B") &&
        contiene(profundidad, "Subestacion C") &&
        contiene(profundidad, "Subestacion D"),
        "DFS visita los cuatro nodos alcanzables")) {
        return 1;
    }

    if (!verificar(
        sinRepetidos(profundidad),
        "DFS no repite nodos en un ciclo")) {
        return 1;
    }

    vector<const Nodo*> aislado =
        Recorridos::bfs(grafo, "Nodo Aislado", "Este");

    if (!verificar(
        aislado.size() == 1 &&
        aislado[0]->nombre == "Nodo Aislado",
        "BFS reconoce un nodo aislado")) {
        return 1;
    }

    if (!verificar(
        Recorridos::dfs(
            grafo, "Nodo Aislado", "Este").size() == 1,
        "DFS reconoce un nodo aislado")) {
        return 1;
    }

    if (!verificar(
        Recorridos::bfs(grafo, "Inexistente", "Norte").empty() &&
        Recorridos::dfs(grafo, "Inexistente", "Norte").empty(),
        "Nodo inicial inexistente rechazado")) {
        return 1;
    }

    // Comprobar que A -> B no implica B -> A
    Grafo dirigido;

    if (!verificar(
        dirigido.agregarNodo("Origen", "Region", "generador") &&
        dirigido.agregarNodo("Destino", "Region", "subestacion") &&
        dirigido.agregarArco(
            "Origen", "Region",
            "Destino", "Region",
            "alta_tension", 15, 5, 100, 2),
        "Grafo dirigido de control creado")) {
        return 1;
    }

    if (!verificar(
        Recorridos::bfs(
            dirigido, "Destino", "Region").size() == 1 &&
        Recorridos::dfs(
            dirigido, "Destino", "Region").size() == 1,
        "BFS y DFS respetan la direccion del arco")) {
        return 1;
    }

    cout << "\nRecorrido BFS:\n";
    Recorridos::mostrar(anchura);

    cout << "\nRecorrido DFS:\n";
    Recorridos::mostrar(profundidad);

    cout << "\nPRUEBA DE RECORRIDOS EXITOSA\n";

    return 0;
}
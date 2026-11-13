#include "OptimizadorTokens.h"

#include <iostream>
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
    cout << "===== PRUEBAS OPTIMIZACION ECO-TOKENS =====\n";

    Grafo grafo;

    if (!verificar(
        grafo.agregarNodo("A", "Norte", "generador") &&
        grafo.agregarNodo("B", "Norte", "subestacion") &&
        grafo.agregarNodo("C", "Sur", "subestacion") &&
        grafo.agregarNodo("D", "Sur", "subestacion") &&
        grafo.agregarNodo("E", "Este", "subestacion"),
        "Cinco nodos creados")) {
        return 1;
    }

    if (!verificar(
        grafo.agregarArco(
            "A", "Norte", "B", "Norte",
            "alta_tension", 10, 3, 100, 2) &&
        grafo.agregarArco(
            "B", "Norte", "D", "Sur",
            "alta_tension", 10, 3, 100, 2) &&
        grafo.agregarArco(
            "A", "Norte", "C", "Sur",
            "subterraneo", 15, 4, 100, 0.5) &&
        grafo.agregarArco(
            "C", "Sur", "D", "Sur",
            "subterraneo", 15, 4, 100, 0.5) &&
        grafo.agregarArco(
            "A", "Norte", "D", "Sur",
            "red_local", 50, 9, 80, 0.2),
        "Cinco conexiones dirigidas creadas")) {
        return 1;
    }

    ResultadoEcoRuta mejor =
        OptimizadorTokens::buscarMejorRuta(
            grafo, "A", "Norte", "D", "Sur", false);

    if (!verificar(
        mejor.encontrada &&
        mejor.ruta.nodos.size() == 3 &&
        mejor.ruta.nodos[0]->nombre == "A" &&
        mejor.ruta.nodos[1]->nombre == "C" &&
        mejor.ruta.nodos[2]->nombre == "D",
        "Mejor ruta identificada: A-C-D")) {
        return 1;
    }

    if (!verificar(
        mejor.tokens == 59 &&
        mejor.ruta.distanciaTotalKm == 30 &&
        mejor.ruta.sumaPerdidasPorcentaje == 1,
        "Recompensa calculada correctamente: 59 tokens")) {
        return 1;
    }

    ResultadoEcoRuta valle =
        OptimizadorTokens::buscarMejorRuta(
            grafo, "A", "Norte", "D", "Sur", true);

    if (!verificar(
        valle.encontrada &&
        valle.tokens == 64 &&
        valle.ruta.nodos[1]->nombre == "C",
        "Horario valle agrega cinco tokens")) {
        return 1;
    }

    if (!verificar(
        !OptimizadorTokens::buscarMejorRuta(
            grafo, "E", "Este", "D", "Sur", false
        ).encontrada,
        "Nodo aislado sin ruta")) {
        return 1;
    }

    if (!verificar(
        !OptimizadorTokens::buscarMejorRuta(
            grafo, "D", "Sur", "A", "Norte", false
        ).encontrada,
        "Direccion de arcos respetada")) {
        return 1;
    }

    if (!verificar(
        !OptimizadorTokens::buscarMejorRuta(
            grafo, "Inexistente", "Norte",
            "D", "Sur", false
        ).encontrada,
        "Origen inexistente rechazado")) {
        return 1;
    }

    if (!verificar(
        !OptimizadorTokens::buscarMejorRuta(
            grafo, "A", "Norte",
            "A", "Norte", false
        ).encontrada,
        "Ruta sin desplazamiento no genera recompensa")) {
        return 1;
    }

    if (!verificar(
        grafo.getCantidadNodos() == 5 &&
        grafo.getCantidadArcos() == 5,
        "La optimizacion no modifica el grafo")) {
        return 1;
    }

    cout << "\nRUTA CON MAYOR RECOMPENSA:\n";
    Rutas::mostrar({mejor.ruta});

    cout << "Eco-Tokens obtenibles: "
         << mejor.tokens << '\n';

    cout << "\nPRUEBA OPTIMIZACION ECO-TOKENS EXITOSA\n";
    return 0;
}
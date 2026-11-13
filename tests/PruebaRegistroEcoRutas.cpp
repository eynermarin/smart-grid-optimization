#include "Clientes.h"
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
    cout << "===== PRUEBAS REGISTRO ECO-RUTAS =====\n";

    Grafo grafo;
    Clientes clientes;

    if (!verificar(
        grafo.agregarNodo("A", "Norte", "generador") &&
        grafo.agregarNodo("B", "Norte", "subestacion") &&
        grafo.agregarNodo("C", "Sur", "subestacion") &&
        grafo.agregarNodo("D", "Sur", "subestacion") &&
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
            "subterraneo", 15, 4, 100, 0.5),
        "Grafo de prueba creado")) {
        return 1;
    }

    if (!verificar(
        clientes.agregarCliente("Hospital General") &&
        clientes.asignarTokens("Hospital General", 100),
        "Cliente con saldo inicial creado")) {
        return 1;
    }

    ResultadoEcoRuta mejor =
        OptimizadorTokens::buscarMejorRuta(
            grafo, "A", "Norte", "D", "Sur", false);

    if (!verificar(
        mejor.encontrada &&
        mejor.tokens == 59,
        "Ruta optima de 59 tokens encontrada")) {
        return 1;
    }

    if (!verificar(
        clientes.registrarRutaOptimizada(
            "Hospital General", mejor),
        "Ruta registrada correctamente")) {
        return 1;
    }

    const Cliente* hospital =
        clientes.buscarCliente("Hospital General");

    if (!verificar(
        hospital != nullptr &&
        hospital->tokens == 159 &&
        hospital->rutasUtilizadas.size() == 1,
        "Tokens acreditados e historial registrado")) {
        return 1;
    }

    if (!verificar(
        hospital->subestacionesConectadas.size() == 2,
        "Dos subestaciones registradas")) {
        return 1;
    }

    if (!verificar(
        hospital->rutasUtilizadas[0].find(
            "A (Norte) -> C (Sur) -> D (Sur)"
        ) != string::npos,
        "Historial conserva el recorrido completo")) {
        return 1;
    }

    if (!verificar(
        clientes.registrarRutaOptimizada(
            "Hospital General", mejor),
        "Segundo uso de la misma ruta registrado")) {
        return 1;
    }

    hospital = clientes.buscarCliente("Hospital General");

    if (!verificar(
        hospital->tokens == 218 &&
        hospital->rutasUtilizadas.size() == 2 &&
        hospital->subestacionesConectadas.size() == 2,
        "Los tokens se acumulan sin duplicar subestaciones")) {
        return 1;
    }

    if (!verificar(
        !clientes.registrarRutaOptimizada(
            "Cliente Inexistente", mejor),
        "Cliente inexistente rechazado")) {
        return 1;
    }

    ResultadoEcoRuta invalida;

    if (!verificar(
        !clientes.registrarRutaOptimizada(
            "Hospital General", invalida),
        "Ruta inexistente rechazada")) {
        return 1;
    }

    hospital = clientes.buscarCliente("Hospital General");

    if (!verificar(
        hospital->tokens == 218 &&
        hospital->rutasUtilizadas.size() == 2,
        "Las operaciones invalidas no alteran el saldo")) {
        return 1;
    }

    cout << "\nPRUEBA REGISTRO ECO-RUTAS EXITOSA\n";
    return 0;
}
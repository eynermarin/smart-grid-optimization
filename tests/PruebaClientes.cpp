#include <iostream>
#include <cassert>
#include <limits>
#include "Clientes.h"

using namespace std;

int main() {
    cout << "===== PRUEBAS DE CLIENTES =====\n";

    Clientes clientes;

    assert(clientes.getCantidad() == 0);
    cout << "OK - Lista inicialmente vacia\n";

    assert(clientes.agregarCliente("Hospital General"));
    assert(clientes.agregarCliente("Zona Residencial Norte"));
    assert(clientes.agregarCliente("Industria Alpha"));
    cout << "OK - Registro de clientes\n";

    assert(clientes.getCantidad() == 3);
    cout << "OK - Cantidad de clientes correcta\n";

    const Cliente* hospital =
        clientes.buscarCliente("Hospital General");

    assert(hospital != nullptr);
    assert(hospital->tokens == 0);
    assert(hospital->consumoAcumulado == 0);
    cout << "OK - Busqueda de cliente existente\n";

    assert(clientes.buscarCliente("Cliente Falso") == nullptr);
    cout << "OK - Busqueda de cliente inexistente\n";

    assert(!clientes.agregarCliente("Hospital General"));
    assert(!clientes.agregarCliente(""));
    assert(!clientes.agregarCliente("   "));
    cout << "OK - Clientes duplicados o vacios rechazados\n";

    assert(clientes.asignarTokens("Hospital General", 450));
    assert(clientes.asignarTokens("Hospital General", 50));

    hospital = clientes.buscarCliente("Hospital General");
    assert(hospital->tokens == 500);
    cout << "OK - Asignacion acumulativa de Eco-Tokens\n";

    assert(!clientes.asignarTokens("Hospital General", -20));
    assert(!clientes.asignarTokens("Hospital General", 0));
    assert(!clientes.asignarTokens("Cliente Falso", 100));
    cout << "OK - Asignaciones invalidas rechazadas\n";

    assert(clientes.registrarConsumo("Hospital General", 120.5));
    assert(clientes.registrarConsumo("Hospital General", 30.0));

    hospital = clientes.buscarCliente("Hospital General");
    assert(hospital->consumoAcumulado == 150.5);
    cout << "OK - Consumo acumulado correctamente\n";

    assert(!clientes.registrarConsumo("Hospital General", -10));
    assert(!clientes.registrarConsumo("Hospital General", 0));
    assert(!clientes.registrarConsumo(
        "Hospital General",
        numeric_limits<double>::infinity()));
    cout << "OK - Consumos invalidos rechazados\n";

    assert(clientes.eliminarCliente("Zona Residencial Norte"));
    assert(clientes.getCantidad() == 2);
    assert(clientes.buscarCliente("Zona Residencial Norte") == nullptr);
    cout << "OK - Eliminacion de cliente intermedio\n";

    assert(!clientes.eliminarCliente("Cliente Falso"));
    cout << "OK - Cliente inexistente rechazado\n";

    cout << "\nClientes restantes:\n";
    clientes.mostrarClientes();

    assert(clientes.eliminarCliente("Industria Alpha"));
    assert(clientes.eliminarCliente("Hospital General"));
    assert(clientes.getCantidad() == 0);
    assert(clientes.getPrimero() == nullptr);

    cout << "OK - Lista completamente vacia\n";

    cout << "\nPRUEBA DE CLIENTES EXITOSA\n";
    return 0;
}
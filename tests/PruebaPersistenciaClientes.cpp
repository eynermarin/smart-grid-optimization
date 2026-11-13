#include <iostream>
#include <fstream>
#include <cassert>
#include "Clientes.h"
#include "Persistencia.h"

using namespace std;

int main() {
    cout << "===== PRUEBAS JSON DE CLIENTES =====\n";

    Clientes clientes;

    assert(Persistencia::cargarClientes(
        "data/clientes.json", clientes));
    cout << "OK - Archivo de clientes cargado\n";

    assert(clientes.getCantidad() == 10);
    cout << "OK - Diez clientes registrados\n";

    const Cliente* hospital =
        clientes.buscarCliente("Hospital General");

    assert(hospital != nullptr);
    assert(hospital->tokens == 450);
    assert(hospital->consumoAcumulado == 150.5);
    assert(hospital->subestacionesConectadas.size() == 1);
    assert(hospital->subestacionesConectadas[0] ==
           "Subestacion Central");

    cout << "OK - Datos del cliente recuperados\n";

    assert(clientes.asignarTokens("Hospital General", 50));
    assert(clientes.registrarConsumo("Hospital General", 25));

    cout << "OK - Tokens y consumo modificados\n";

    assert(Persistencia::guardarClientes(
        "data/clientes_prueba.json", clientes));

    cout << "OK - Archivo JSON guardado\n";

    Clientes recuperados;

    assert(Persistencia::cargarClientes(
        "data/clientes_prueba.json", recuperados));

    const Cliente* nuevo =
        recuperados.buscarCliente("Hospital General");

    assert(nuevo != nullptr);
    assert(nuevo->tokens == 500);
    assert(nuevo->consumoAcumulado == 175.5);
    assert(recuperados.getCantidad() == 10);

    cout << "OK - Datos recuperados despues del guardado\n";

    Clientes historial;

    assert(historial.restaurarCliente(
        "Cliente Historial", 200, 80,
        {"Subestacion Central", "Subestacion Sur"},
        {"Mantenimiento prioritario de red"}));

    assert(Persistencia::guardarClientes(
        "data/clientes_historial_prueba.json", historial));

    Clientes historialRecuperado;

    assert(Persistencia::cargarClientes(
        "data/clientes_historial_prueba.json",
        historialRecuperado));

    const Cliente* registro =
        historialRecuperado.buscarCliente("Cliente Historial");

    assert(registro != nullptr);
    assert(registro->subestacionesConectadas.size() == 2);
    assert(registro->beneficiosCanjeados.size() == 1);
    assert(registro->beneficiosCanjeados[0] ==
           "Mantenimiento prioritario de red");

    cout << "OK - Subestaciones y beneficios conservados\n";

    // Probar rechazo de tokens negativos
    {
        ofstream archivo("data/clientes_invalidos_prueba.json");

        archivo << R"({
            "clientes": [
                {
                    "nombre": "Cliente Incorrecto",
                    "consumo_acumulado": 100,
                    "tokens": -20,
                    "subestaciones_conectadas": [],
                    "beneficios": []
                }
            ]
        })";
    }

    assert(!Persistencia::cargarClientes(
        "data/clientes_invalidos_prueba.json",
        recuperados));

    assert(recuperados.getCantidad() == 10);

    cout << "OK - JSON invalido rechazado sin perder datos\n";

    cout << "\nPRUEBA JSON DE CLIENTES EXITOSA\n";
    return 0;
}
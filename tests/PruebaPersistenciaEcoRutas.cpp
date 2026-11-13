#include "Clientes.h"
#include "Grafo.h"
#include "OptimizadorTokens.h"
#include "Persistencia.h"

#include <iostream>
#include <fstream>
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
    cout << "===== PRUEBAS JSON DE ECO-RUTAS =====\n";

    Clientes clientes;
    Grafo grafo;

    if (!verificar(
        Persistencia::cargarClientes(
            "data/clientes.json", clientes) &&
        clientes.getCantidad() == 10,
        "Diez clientes originales cargados")) {
        return 1;
    }

    const Cliente* inicial =
        clientes.buscarCliente("Hospital General");

    if (!verificar(
        inicial != nullptr &&
        inicial->rutasUtilizadas.empty(),
        "JSON anterior compatible con historial vacio")) {
        return 1;
    }

    if (!verificar(
        Persistencia::cargarRed(
            "data/red_electrica.json", grafo) &&
        grafo.getCantidadNodos() == 15,
        "Red definitiva cargada")) {
        return 1;
    }

    ResultadoEcoRuta mejor =
        OptimizadorTokens::buscarMejorRuta(
            grafo,
            "Planta Solar Alpha", "Zona Norte",
            "Subestacion Sur 5", "Zona Sur",
            true
        );

    if (!verificar(
        mejor.encontrada &&
        mejor.tokens > 0,
        "Ruta con recompensa encontrada")) {
        return 1;
    }

    long long saldoInicial = inicial->tokens;

    if (!verificar(
        clientes.registrarRutaOptimizada(
            "Hospital General", mejor),
        "Primera ruta registrada")) {
        return 1;
    }

    if (!verificar(
        clientes.registrarRutaOptimizada(
            "Hospital General", mejor),
        "Segundo uso registrado")) {
        return 1;
    }

    const Cliente* actualizado =
        clientes.buscarCliente("Hospital General");

    if (!verificar(
        actualizado != nullptr &&
        actualizado->tokens == saldoInicial + 2 * mejor.tokens &&
        actualizado->rutasUtilizadas.size() == 2 &&
        actualizado->rutasUtilizadas[0] ==
            actualizado->rutasUtilizadas[1],
        "Tokens acumulados y dos usos registrados")) {
        return 1;
    }

    if (!verificar(
        Persistencia::guardarClientes(
            "data/eco_rutas_prueba.json", clientes),
        "Historial guardado en JSON temporal")) {
        return 1;
    }

    Clientes recuperados;

    if (!verificar(
        Persistencia::cargarClientes(
            "data/eco_rutas_prueba.json", recuperados),
        "Clientes recuperados desde JSON")) {
        return 1;
    }

    const Cliente* restaurado =
        recuperados.buscarCliente("Hospital General");

    if (!verificar(
        restaurado != nullptr &&
        restaurado->tokens == actualizado->tokens &&
        restaurado->rutasUtilizadas.size() == 2 &&
        restaurado->rutasUtilizadas ==
            actualizado->rutasUtilizadas &&
        restaurado->subestacionesConectadas ==
            actualizado->subestacionesConectadas,
        "Saldo, rutas y subestaciones conservados")) {
        return 1;
    }

    // Comprobar que los beneficios anteriores siguen presentes.
    if (!verificar(
        restaurado->beneficiosCanjeados ==
            actualizado->beneficiosCanjeados,
        "Historial de beneficios conservado")) {
        return 1;
    }

    // Crear un archivo con un historial de tipo incorrecto.
    {
        ofstream archivo("data/eco_rutas_invalidas_prueba.json");

        archivo << R"({
            "clientes": [
                {
                    "nombre": "Cliente Incorrecto",
                    "consumo_acumulado": 10,
                    "tokens": 20,
                    "subestaciones_conectadas": [],
                    "beneficios": [],
                    "rutas_utilizadas": [123]
                }
            ]
        })";
    }

    if (!verificar(
        !Persistencia::cargarClientes(
            "data/eco_rutas_invalidas_prueba.json",
            recuperados),
        "Historial invalido rechazado")) {
        return 1;
    }

    if (!verificar(
        recuperados.getCantidad() == 10 &&
        recuperados.buscarCliente("Hospital General") != nullptr &&
        recuperados.buscarCliente("Hospital General")
            ->rutasUtilizadas.size() == 2,
        "Datos anteriores intactos tras el rechazo")) {
        return 1;
    }

    cout << "\nPRUEBA PERSISTENCIA ECO-RUTAS EXITOSA\n";
    return 0;
}
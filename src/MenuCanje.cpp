#include "MenuCanje.h"
#include "OptimizadorTokens.h"
#include "Rutas.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace {

bool leerTexto(const string& mensaje, string& valor) {
    cout << mensaje;
    return static_cast<bool>(getline(cin, valor));
}

bool leerOpcion(int& opcion) {
    string texto;

    if (!leerTexto("Seleccione una opcion: ", texto)) {
        return false;
    }

    istringstream flujo(texto);
    char extra;

    if (!(flujo >> opcion) || (flujo >> extra)) {
        opcion = -1;
    }

    return true;
}

void consultarSaldo(const Clientes& clientes) {
    string nombre;

    cout << "\n===== CONSULTAR SALDO =====\n";

    if (!leerTexto("Nombre del cliente: ", nombre)) {
        return;
    }

    const Cliente* cliente = clientes.buscarCliente(nombre);

    if (cliente == nullptr) {
        cout << "Cliente no encontrado.\n";
        return;
    }

    cout << "Cliente: " << cliente->nombre << '\n';
    cout << "Eco-Tokens disponibles: "
         << cliente->tokens << '\n';

    cout << "Beneficios canjeados: "
         << cliente->beneficiosCanjeados.size() << '\n';

    for (const string& beneficio :
         cliente->beneficiosCanjeados) {
        cout << "- " << beneficio << '\n';
    }
}

void realizarCanje(
    Clientes& clientes,
    const Beneficios& beneficios) {

    string nombreCliente;
    string nombreBeneficio;

    cout << "\n===== CANJEAR BENEFICIO =====\n";

    if (!leerTexto("Nombre del cliente: ", nombreCliente)) {
        return;
    }

    const Cliente* cliente =
        clientes.buscarCliente(nombreCliente);

    if (cliente == nullptr) {
        cout << "Cliente no encontrado.\n";
        return;
    }

    cout << "\nBeneficios disponibles:\n";
    beneficios.mostrarBeneficios();

    if (!leerTexto("Nombre del beneficio: ", nombreBeneficio)) {
        return;
    }

    const Beneficio* beneficio =
        beneficios.buscarBeneficio(nombreBeneficio);

    if (beneficio == nullptr) {
        cout << "Beneficio no encontrado.\n";
        return;
    }

    if (cliente->tokens < beneficio->costoTokens) {
        cout << "Eco-Tokens insuficientes.\n";
        cout << "Saldo actual: " << cliente->tokens << '\n';
        cout << "Costo: " << beneficio->costoTokens << '\n';
        return;
    }

    if (clientes.canjearBeneficio(
        nombreCliente, nombreBeneficio, beneficios)) {

        const Cliente* actualizado =
            clientes.buscarCliente(nombreCliente);

        cout << "Canje realizado correctamente.\n";
        cout << "Beneficio: " << beneficio->nombre << '\n';
        cout << "Eco-Tokens restantes: "
             << actualizado->tokens << '\n';
    } else {
        cout << "No se pudo realizar el canje.\n";
    }
}


void registrarEcoRuta(Clientes& clientes, const Grafo& grafo) {
    string clienteNombre;
    string origen;
    string regionOrigen;
    string destino;
    string regionDestino;
    string horario;
    string confirmacion;

    cout << "\n===== OBTENER ECO-TOKENS POR RUTA =====\n";

    if (!leerTexto("Nombre del cliente: ", clienteNombre)) {
        return;
    }

    const Cliente* cliente = clientes.buscarCliente(clienteNombre);

    if (cliente == nullptr) {
        cout << "Cliente no encontrado.\n";
        return;
    }

    if (!leerTexto("Nodo origen: ", origen) ||
        !leerTexto("Region origen: ", regionOrigen) ||
        !leerTexto("Nodo destino: ", destino) ||
        !leerTexto("Region destino: ", regionDestino) ||
        !leerTexto("Horario valle (s/n): ", horario)) {
        return;
    }

    if (horario != "s" && horario != "S" &&
        horario != "n" && horario != "N") {
        cout << "Horario invalido.\n";
        return;
    }

    bool horarioValle = horario == "s" || horario == "S";

    ResultadoEcoRuta resultado =
        OptimizadorTokens::buscarMejorRuta(
            grafo,
            origen, regionOrigen,
            destino, regionDestino,
            horarioValle
        );

    if (!resultado.encontrada) {
        cout << "No se encontro una ruta valida.\n";
        return;
    }

    cout << "\nMEJOR RUTA PARA ECO-TOKENS:\n";
    Rutas::mostrar({resultado.ruta});

    cout << "Eco-Tokens obtenibles: "
         << resultado.tokens << '\n';

    cout << "Saldo actual: " << cliente->tokens << '\n';

    if (!leerTexto("Confirmar registro (s/n): ", confirmacion)) {
        return;
    }

    if (confirmacion != "s" && confirmacion != "S") {
        cout << "Registro cancelado.\n";
        return;
    }

    if (clientes.registrarRutaOptimizada(clienteNombre, resultado)) {
        const Cliente* actualizado =
            clientes.buscarCliente(clienteNombre);

        cout << "Eco-Ruta registrada correctamente.\n";
        cout << "Eco-Tokens acreditados: "
             << resultado.tokens << '\n';
        cout << "Nuevo saldo: "
             << actualizado->tokens << '\n';
        cout << "Rutas utilizadas: "
             << actualizado->rutasUtilizadas.size() << '\n';
    } else {
        cout << "No se pudo registrar la Eco-Ruta.\n";
    }
}
} // namespace

void menuCanje(Clientes& clientes, const Beneficios& beneficios, const Grafo& grafo) {
    while (true) {
        cout << "\n===== ECO-TOKENS Y CANJE =====\n";
        cout << "1. Consultar saldo de un cliente\n";
        cout << "2. Canjear beneficio\n";
        cout << "3. Obtener Eco-Tokens por ruta optimizada\n";
        cout << "0. Volver al menu principal\n";

        int opcion;

        if (!leerOpcion(opcion)) {
            return;
        }

        switch (opcion) {
            case 1:
                consultarSaldo(clientes);
                break;

            case 2:
                realizarCanje(clientes, beneficios);
                break;

            case 3:
                registrarEcoRuta(clientes, grafo);
                break;

            case 0:
                cout << "Regresando al menu principal...\n";
                return;

            default:
                cout << "Opcion invalida.\n";
                break;
        }

        if (cin.eof()) {
            return;
        }
    }
}
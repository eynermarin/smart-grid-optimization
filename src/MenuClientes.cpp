#include "MenuClientes.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

namespace {

bool leerTexto(const string& mensaje, string& resultado) {
    cout << mensaje;
    return static_cast<bool>(getline(cin, resultado));
}

bool leerEntero(const string& mensaje, long long& resultado) {
    string texto;

    if (!leerTexto(mensaje, texto)) {
        return false;
    }

    istringstream entrada(texto);
    entrada >> resultado;

    if (!entrada) {
        return false;
    }

    entrada >> ws;
    return entrada.eof();
}

bool leerDecimal(const string& mensaje, double& resultado) {
    string texto;

    if (!leerTexto(mensaje, texto)) {
        return false;
    }

    istringstream entrada(texto);
    entrada >> resultado;

    if (!entrada || !isfinite(resultado)) {
        return false;
    }

    entrada >> ws;
    return entrada.eof();
}

void mostrarMenu() {
    cout << "\n===== GESTION DE CLIENTES =====\n"
         << "1. Mostrar clientes\n"
         << "2. Agregar cliente\n"
         << "3. Buscar cliente\n"
         << "4. Eliminar cliente\n"
         << "5. Asignar Eco-Tokens\n"
         << "6. Registrar consumo\n"
         << "0. Volver al menu principal\n";
}

void agregar(Clientes& clientes) {
    string nombre;

    if (!leerTexto("Nombre del cliente: ", nombre)) {
        return;
    }

    if (clientes.agregarCliente(nombre)) {
        cout << "Cliente agregado correctamente.\n";
    } else {
        cout << "No se pudo agregar el cliente.\n";
    }
}

void buscar(const Clientes& clientes) {
    string nombre;

    if (!leerTexto("Nombre del cliente: ", nombre)) {
        return;
    }

    const Cliente* cliente = clientes.buscarCliente(nombre);

    if (cliente == nullptr) {
        cout << "Cliente no encontrado.\n";
        return;
    }

    cout << "Cliente: " << cliente->nombre << "\n"
         << "Consumo: " << cliente->consumoAcumulado
         << " kWh\n"
         << "Eco-Tokens: " << cliente->tokens << "\n";

    cout << "Subestaciones conectadas:\n";
    for (const string& subestacion :
         cliente->subestacionesConectadas) {
        cout << "- " << subestacion << "\n";
    }

    cout << "Beneficios canjeados:\n";
    for (const string& beneficio :
         cliente->beneficiosCanjeados) {
        cout << "- " << beneficio << "\n";
    }
}

void eliminar(Clientes& clientes) {
    string nombre;
    string confirmacion;

    if (!leerTexto("Nombre del cliente: ", nombre)) {
        return;
    }

    if (clientes.buscarCliente(nombre) == nullptr) {
        cout << "Cliente no encontrado.\n";
        return;
    }

    if (!leerTexto("Confirmar eliminacion (s/n): ",
                   confirmacion)) {
        return;
    }

    if (confirmacion != "s" && confirmacion != "S") {
        cout << "Eliminacion cancelada.\n";
        return;
    }

    if (clientes.eliminarCliente(nombre)) {
        cout << "Cliente eliminado correctamente.\n";
    } else {
        cout << "No se pudo eliminar el cliente.\n";
    }
}

void asignarTokens(Clientes& clientes) {
    string nombre;
    long long cantidad;

    if (!leerTexto("Nombre del cliente: ", nombre)) {
        return;
    }

    if (!leerEntero("Cantidad de Eco-Tokens: ", cantidad)) {
        cout << "Cantidad invalida.\n";
        return;
    }

    if (clientes.asignarTokens(nombre, cantidad)) {
        cout << "Eco-Tokens asignados correctamente.\n";
    } else {
        cout << "No se pudieron asignar los Eco-Tokens.\n";
    }
}

void registrarConsumo(Clientes& clientes) {
    string nombre;
    double consumo;

    if (!leerTexto("Nombre del cliente: ", nombre)) {
        return;
    }

    if (!leerDecimal("Consumo en kWh: ", consumo)) {
        cout << "Consumo invalido.\n";
        return;
    }

    if (clientes.registrarConsumo(nombre, consumo)) {
        cout << "Consumo registrado correctamente.\n";
    } else {
        cout << "No se pudo registrar el consumo.\n";
    }
}

} // namespace

void menuClientes(Clientes& clientes) {
    while (true) {
        mostrarMenu();

        long long opcion;

        if (!leerEntero("Seleccione una opcion: ", opcion)) {
            if (cin.eof()) {
                return;
            }

            cout << "Opcion invalida.\n";
            continue;
        }

        switch (opcion) {
            case 1:
                clientes.mostrarClientes();
                break;

            case 2:
                agregar(clientes);
                break;

            case 3:
                buscar(clientes);
                break;

            case 4:
                eliminar(clientes);
                break;

            case 5:
                asignarTokens(clientes);
                break;

            case 6:
                registrarConsumo(clientes);
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
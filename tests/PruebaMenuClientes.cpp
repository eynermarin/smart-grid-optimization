#include "MenuClientes.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    Clientes clientes;

    istringstream entrada(
        "1\n"
        "2\nCliente Prueba\n"
        "2\nCliente Prueba\n"
        "5\nCliente Prueba\n75\n"
        "5\nCliente Prueba\n-2\n"
        "6\nCliente Prueba\n12.5\n"
        "6\nCliente Prueba\n-2\n"
        "3\nCliente Prueba\n"
        "4\nCliente Prueba\nn\n"
        "4\nCliente Prueba\ns\n"
        "1\n"
        "0\n"
    );

    ostringstream salida;

    streambuf* entradaOriginal = cin.rdbuf(entrada.rdbuf());
    streambuf* salidaOriginal = cout.rdbuf(salida.rdbuf());

    menuClientes(clientes);

    cin.rdbuf(entradaOriginal);
    cout.rdbuf(salidaOriginal);

    const string resultado = salida.str();

    const string mensajes[] = {
        "Cliente agregado correctamente.",
        "No se pudo agregar el cliente.",
        "Eco-Tokens asignados correctamente.",
        "No se pudieron asignar los Eco-Tokens.",
        "Consumo registrado correctamente.",
        "No se pudo registrar el consumo.",
        "Eco-Tokens: 75",
        "Consumo: 12.5 kWh",
        "Eliminacion cancelada.",
        "Cliente eliminado correctamente.",
        "No hay clientes registrados.",
        "Regresando al menu principal..."
    };

    for (const string& mensaje : mensajes) {
        if (resultado.find(mensaje) == string::npos) {
            cerr << "FALLO: " << mensaje << "\n";
            return 1;
        }

        cout << "OK - " << mensaje << "\n";
    }

    if (clientes.getCantidad() != 0) {
        cerr << "FALLO: La lista no quedo vacia.\n";
        return 1;
    }

    cout << "OK - Lista vacia despues de eliminar\n";
    cout << "\nPRUEBA MENU CLIENTES EXITOSA\n";

    return 0;
}
#include "MenuCanje.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    cout << "===== PRUEBAS MENU DE CANJE =====\n";

    Clientes clientes;
    Beneficios beneficios;
    Grafo grafo;

    if (!clientes.agregarCliente("Hospital General") ||
        !clientes.asignarTokens("Hospital General", 150) ||
        !beneficios.agregarBeneficio(
            "Descuento", "Descuento de electricidad", 100) ||
        !beneficios.agregarBeneficio(
            "Revision", "Revision tecnica", 200)) {

        cerr << "FALLO: No se pudieron preparar los datos.\n";
        return 1;
    }

    istringstream entrada(
        "1\nHospital General\n"
        "2\nHospital General\nDescuento\n"
        "1\nHospital General\n"
        "2\nHospital General\nRevision\n"
        "2\nHospital General\nInexistente\n"
        "2\nCliente Falso\n"
        "0\n"
    );

    ostringstream salida;

    streambuf* entradaOriginal = cin.rdbuf(entrada.rdbuf());
    streambuf* salidaOriginal = cout.rdbuf(salida.rdbuf());

    menuCanje(clientes, beneficios, grafo);

    cin.rdbuf(entradaOriginal);
    cout.rdbuf(salidaOriginal);
    cin.clear();

    string resultado = salida.str();

    const string mensajes[] = {
        "ECO-TOKENS Y CANJE",
        "Eco-Tokens disponibles: 150",
        "Canje realizado correctamente.",
        "Eco-Tokens restantes: 50",
        "Beneficios canjeados: 1",
        "Eco-Tokens insuficientes.",
        "Beneficio no encontrado.",
        "Cliente no encontrado.",
        "Regresando al menu principal..."
    };

    for (const string& mensaje : mensajes) {
        if (resultado.find(mensaje) == string::npos) {
            cerr << "FALLO: " << mensaje << '\n';
            return 1;
        }

        cout << "OK - " << mensaje << '\n';
    }

    const Cliente* cliente =
        clientes.buscarCliente("Hospital General");

    if (cliente == nullptr ||
        cliente->tokens != 50 ||
        cliente->beneficiosCanjeados.size() != 1 ||
        cliente->beneficiosCanjeados[0] != "Descuento") {

        cerr << "FALLO: Saldo o historial incorrectos.\n";
        return 1;
    }

    cout << "OK - Saldo e historial correctos\n";

    if (beneficios.getCantidad() != 2) {
        cerr << "FALLO: El catalogo fue modificado.\n";
        return 1;
    }

    cout << "OK - Catalogo de beneficios conservado\n";

    cout << "\nPRUEBA MENU DE CANJE EXITOSA\n";

    return 0;
}
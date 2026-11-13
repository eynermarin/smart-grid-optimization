#include "MenuCanje.h"

#include <iostream>
#include <sstream>
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
    cout << "===== PRUEBAS MENU ECO-RUTAS =====\n";

    Grafo grafo;
    Clientes clientes;
    Beneficios beneficios;

    if (!verificar(
        grafo.agregarNodo("A", "Norte", "generador") &&
        grafo.agregarNodo("C", "Sur", "subestacion") &&
        grafo.agregarNodo("D", "Sur", "subestacion") &&
        grafo.agregarArco(
            "A", "Norte", "C", "Sur",
            "subterraneo", 15, 4, 100, 0.5) &&
        grafo.agregarArco(
            "C", "Sur", "D", "Sur",
            "subterraneo", 15, 4, 100, 0.5) &&
        clientes.agregarCliente("Hospital General") &&
        clientes.asignarTokens("Hospital General", 100),
        "Red y cliente preparados")) {
        return 1;
    }

    istringstream entrada(
        // Registrar una ruta de 59 tokens.
        "3\n"
        "Hospital General\n"
        "A\nNorte\nD\nSur\nn\n"
        "s\n"

        // Consultar saldo.
        "1\nHospital General\n"

        // Intentar registrar otra ruta pero cancelar.
        "3\n"
        "Hospital General\n"
        "A\nNorte\nD\nSur\ns\n"
        "n\n"

        // Cliente inexistente.
        "3\nCliente Falso\n"

        // Ruta inexistente.
        "3\n"
        "Hospital General\n"
        "D\nSur\nA\nNorte\nn\n"

        "0\n"
    );

    ostringstream salida;

    streambuf* entradaOriginal = cin.rdbuf(entrada.rdbuf());
    streambuf* salidaOriginal = cout.rdbuf(salida.rdbuf());

    menuCanje(clientes, beneficios, grafo);

    cin.rdbuf(entradaOriginal);
    cout.rdbuf(salidaOriginal);
    cin.clear();

    const string texto = salida.str();

    const string mensajes[] = {
        "ECO-TOKENS Y CANJE",
        "OBTENER ECO-TOKENS POR RUTA",
        "MEJOR RUTA PARA ECO-TOKENS:",
        "Eco-Tokens obtenibles: 59",
        "Eco-Ruta registrada correctamente.",
        "Eco-Tokens acreditados: 59",
        "Nuevo saldo: 159",
        "Rutas utilizadas: 1",
        "Eco-Tokens disponibles: 159",
        "Registro cancelado.",
        "Cliente no encontrado.",
        "No se encontro una ruta valida.",
        "Regresando al menu principal..."
    };

    for (const string& mensaje : mensajes) {
        if (!verificar(
            texto.find(mensaje) != string::npos,
            mensaje)) {
            return 1;
        }
    }

    const Cliente* cliente =
        clientes.buscarCliente("Hospital General");

    if (!verificar(
        cliente != nullptr &&
        cliente->tokens == 159 &&
        cliente->rutasUtilizadas.size() == 1 &&
        cliente->subestacionesConectadas.size() == 2,
        "Saldo, historial y subestaciones correctos")) {
        return 1;
    }

    if (!verificar(
        grafo.getCantidadNodos() == 3 &&
        grafo.getCantidadArcos() == 2,
        "La red electrica no fue modificada")) {
        return 1;
    }

    cout << "\nPRUEBA MENU ECO-RUTAS EXITOSA\n";
    return 0;
}
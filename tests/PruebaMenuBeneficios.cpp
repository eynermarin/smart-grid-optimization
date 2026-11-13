#include "MenuBeneficios.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    Beneficios beneficios;

    istringstream entrada(
        "1\n"
        "2\nDescuento\nDescuento de electricidad\n100\n"
        "2\nDescuento\nDuplicado\n50\n"
        "2\nRevision\nRevision tecnica\n200\n"
        "3\nDescuento\n"
        "3\nInexistente\n"
        "4\nDescuento\nDescuento Especial\nDescuento premium\n150\n"
        "4\nDescuento Especial\nRevision\nDuplicado\n80\n"
        "1\n"
        "5\nDescuento Especial\nn\n"
        "5\nDescuento Especial\ns\n"
        "5\nRevision\ns\n"
        "1\n"
        "0\n"
    );

    ostringstream salida;

    streambuf* entradaOriginal = cin.rdbuf(entrada.rdbuf());
    streambuf* salidaOriginal = cout.rdbuf(salida.rdbuf());

    menuBeneficios(beneficios);

    cin.rdbuf(entradaOriginal);
    cout.rdbuf(salidaOriginal);
    cin.clear();

    const string resultado = salida.str();

    const string mensajes[] = {
        "No hay beneficios registrados.",
        "Beneficio agregado correctamente.",
        "No se pudo agregar el beneficio.",
        "Nombre: Descuento",
        "Costo: 100 Eco-Tokens",
        "Beneficio no encontrado.",
        "Beneficio modificado correctamente.",
        "No se pudo modificar el beneficio.",
        "Beneficio: Descuento Especial",
        "Costo: 150 Eco-Tokens",
        "Eliminacion cancelada.",
        "Beneficio eliminado correctamente.",
        "Regresando al menu principal..."
    };

    cout << "===== PRUEBAS MENU BENEFICIOS =====\n";

    for (const string& mensaje : mensajes) {
        if (resultado.find(mensaje) == string::npos) {
            cerr << "FALLO: " << mensaje << "\n";
            return 1;
        }

        cout << "OK - " << mensaje << "\n";
    }

    if (beneficios.getCantidad() != 0 ||
        beneficios.getPrimero() != nullptr) {
        cerr << "FALLO: La lista no quedo vacia.\n";
        return 1;
    }

    cout << "OK - Lista vacia despues de eliminar\n";

    // Segunda prueba: comprobar los datos conservados
    Beneficios otraLista;

    if (!otraLista.agregarBeneficio(
        "Prueba", "Beneficio temporal", 50)) {
        cerr << "FALLO: Registro adicional.\n";
        return 1;
    }

    const Beneficio* beneficio =
        otraLista.buscarBeneficio("Prueba");

    if (beneficio == nullptr ||
        beneficio->costoTokens != 50 ||
        beneficio->descripcion != "Beneficio temporal") {
        cerr << "FALLO: Datos del beneficio incorrectos.\n";
        return 1;
    }

    cout << "OK - Datos del beneficio conservados\n";

    cout << "\nPRUEBA MENU BENEFICIOS EXITOSA\n";

    return 0;
}
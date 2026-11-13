#include "MenuBeneficios.h"

#include <iostream>
#include <sstream>
#include <string>

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

    if (!(entrada >> resultado)) {
        return false;
    }

    entrada >> ws;
    return entrada.eof();
}

void mostrarMenu() {
    cout << "\n===== GESTION DE BENEFICIOS =====\n"
         << "1. Mostrar beneficios\n"
         << "2. Agregar beneficio\n"
         << "3. Buscar beneficio\n"
         << "4. Modificar beneficio\n"
         << "5. Eliminar beneficio\n"
         << "0. Volver al menu principal\n";
}

void agregar(Beneficios& beneficios) {
    string nombre;
    string descripcion;
    long long costo;

    cout << "\n===== AGREGAR BENEFICIO =====\n";

    if (!leerTexto("Nombre: ", nombre) ||
        !leerTexto("Descripcion: ", descripcion)) {
        return;
    }

    if (!leerEntero("Costo en Eco-Tokens: ", costo)) {
        cout << "Costo invalido.\n";
        return;
    }

    if (beneficios.agregarBeneficio(nombre, descripcion, costo)) {
        cout << "Beneficio agregado correctamente.\n";
    } else {
        cout << "No se pudo agregar el beneficio.\n";
    }
}

void buscar(const Beneficios& beneficios) {
    string nombre;

    cout << "\n===== BUSCAR BENEFICIO =====\n";

    if (!leerTexto("Nombre: ", nombre)) {
        return;
    }

    const Beneficio* beneficio =
        beneficios.buscarBeneficio(nombre);

    if (beneficio == nullptr) {
        cout << "Beneficio no encontrado.\n";
        return;
    }

    cout << "Nombre: " << beneficio->nombre << "\n"
         << "Descripcion: " << beneficio->descripcion << "\n"
         << "Costo: " << beneficio->costoTokens
         << " Eco-Tokens\n";
}

void modificar(Beneficios& beneficios) {
    string nombreActual;
    string nuevoNombre;
    string nuevaDescripcion;
    long long nuevoCosto;

    cout << "\n===== MODIFICAR BENEFICIO =====\n";

    if (!leerTexto("Nombre actual: ", nombreActual)) {
        return;
    }

    if (beneficios.buscarBeneficio(nombreActual) == nullptr) {
        cout << "Beneficio no encontrado.\n";
        return;
    }

    if (!leerTexto("Nuevo nombre: ", nuevoNombre) ||
        !leerTexto("Nueva descripcion: ", nuevaDescripcion)) {
        return;
    }

    if (!leerEntero("Nuevo costo en Eco-Tokens: ", nuevoCosto)) {
        cout << "Costo invalido.\n";
        return;
    }

    if (beneficios.modificarBeneficio(
        nombreActual, nuevoNombre, nuevaDescripcion, nuevoCosto)) {
        cout << "Beneficio modificado correctamente.\n";
    } else {
        cout << "No se pudo modificar el beneficio.\n";
    }
}

void eliminar(Beneficios& beneficios) {
    string nombre;
    string confirmacion;

    cout << "\n===== ELIMINAR BENEFICIO =====\n";

    if (!leerTexto("Nombre: ", nombre)) {
        return;
    }

    if (beneficios.buscarBeneficio(nombre) == nullptr) {
        cout << "Beneficio no encontrado.\n";
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

    if (beneficios.eliminarBeneficio(nombre)) {
        cout << "Beneficio eliminado correctamente.\n";
    } else {
        cout << "No se pudo eliminar el beneficio.\n";
    }
}

} // namespace

void menuBeneficios(Beneficios& beneficios) {
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
                cout << "\nBENEFICIOS REGISTRADOS:\n";
                beneficios.mostrarBeneficios();
                break;

            case 2:
                agregar(beneficios);
                break;

            case 3:
                buscar(beneficios);
                break;

            case 4:
                modificar(beneficios);
                break;

            case 5:
                eliminar(beneficios);
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
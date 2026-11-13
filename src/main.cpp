#include <iostream>
#include <sstream>
#include <string>
#include "Grafo.h"
#include "Persistencia.h"

using namespace std;

const string ARCHIVO_RED = "data/red_electrica.json";

int leerOpcion() {
    string entrada;
    int opcion;
    char extra;

    if (!getline(cin, entrada)) {
        return 0;
    }

    istringstream flujo(entrada);

    if (!(flujo >> opcion) || (flujo >> extra)) {
        return -1;
    }

    return opcion;
}

void mostrarMenu() {
    cout << "\n========================================\n";
    cout << "       SMART GRID OPTIMIZATION\n";
    cout << "========================================\n";
    cout << "1. Gestion de la red electrica\n";
    cout << "2. Gestion de clientes\n";
    cout << "3. Gestion de beneficios\n";
    cout << "4. Recorridos BFS y DFS\n";
    cout << "5. Consultas de rutas\n";
    cout << "6. Eco-Tokens y canje de beneficios\n";
    cout << "7. Reportes del sistema\n";
    cout << "8. Guardar datos en JSON\n";
    cout << "0. Salir\n";
    cout << "========================================\n";
    cout << "Seleccione una opcion: ";
}

void menuRed(Grafo& grafo) {
    int opcion;

    do {
        cout << "\n===== GESTION DE RED ELECTRICA =====\n";
        cout << "1. Mostrar nodos\n";
        cout << "2. Mostrar conexiones\n";
        cout << "0. Volver al menu principal\n";
        cout << "Seleccione una opcion: ";

        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                cout << "\nNODOS REGISTRADOS:\n";
                if (grafo.getCantidadNodos() == 0) {
                    cout << "No hay nodos registrados.\n";
                } else {
                    grafo.mostrarNodos();
                }
                break;

            case 2:
                cout << "\nCONEXIONES REGISTRADAS:\n";
                if (grafo.getCantidadArcos() == 0) {
                    cout << "No hay conexiones registradas.\n";
                } else {
                    grafo.mostrarArcos();
                }
                break;

            case 0:
                cout << "Regresando al menu principal...\n";
                break;

            default:
                cout << "Opcion invalida.\n";
        }

    } while (opcion != 0);
}

int main() {
    Grafo grafo;

    cout << "Iniciando Smart Grid Optimization...\n";

    if (!Persistencia::cargarRed(ARCHIVO_RED, grafo)) {
        cerr << "ERROR: No se pudo cargar la red electrica.\n";
        cerr << "Verifique el archivo: " << ARCHIVO_RED << '\n';
        return 1;
    }

    cout << "Red electrica cargada correctamente.\n";
    cout << "Nodos: " << grafo.getCantidadNodos() << '\n';
    cout << "Conexiones: " << grafo.getCantidadArcos() << '\n';

    int opcion;

    do {
        mostrarMenu();
        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                menuRed(grafo);
                break;

            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                cout << "\nModulo pendiente de implementacion.\n";
                break;

            case 8:
                if (Persistencia::guardarRed(ARCHIVO_RED, grafo)) {
                    cout << "\nDatos guardados correctamente.\n";
                } else {
                    cerr << "\nError al guardar los datos.\n";
                }
                break;

            case 0:
                cout << "\nGuardando datos antes de salir...\n";

                if (!Persistencia::guardarRed(ARCHIVO_RED, grafo)) {
                    cerr << "Error al guardar los datos.\n";
                    return 1;
                }

                cout << "Datos guardados correctamente.\n";
                cout << "Saliendo de Smart Grid...\n";
                break;

            default:
                cout << "\nOpcion invalida.\n";
        }

    } while (opcion != 0);

    return 0;
}
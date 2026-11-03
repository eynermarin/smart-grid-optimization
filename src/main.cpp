
#include <iostream>
#include <limits>
using namespace std;

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

int main() {
    int opcion = -1;

    do {
        mostrarMenu();

        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nError: ingrese un numero valido.\n";
            continue;
        }

        switch (opcion) {
            case 1:
                cout << "\nGestion de nodos y conexiones.\n";
                break;

            case 2:
                cout << "\nGestion de clientes.\n";
                break;

            case 3:
                cout << "\nGestion de beneficios.\n";
                break;

            case 4:
                cout << "\nRecorridos BFS y DFS.\n";
                break;

            case 5:
                cout << "\nConsultas de rutas posibles y optimas.\n";
                break;

            case 6:
                cout << "\nGestion de Eco-Tokens y canjes.\n";
                break;

            case 7:
                cout << "\nReportes del sistema.\n";
                break;

            case 8:
                cout << "\nGuardado de archivos JSON.\n";
                break;

            case 0:
                cout << "\nSaliendo de Smart Grid...\n";
                break;

            default:
                cout << "\nError: opcion no valida.\n";
        }

    } while (opcion != 0);

    return 0;
}

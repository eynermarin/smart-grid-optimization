#include "MenuReportes.h"
#include "ReportesRed.h"
#include "ReporteClientes.h"
#include "ReporteRutas.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace {

bool leerOpcion(int& opcion) {
    string entrada;

    cout << "Seleccione una opcion: ";

    if (!getline(cin, entrada)) {
        return false;
    }

    istringstream flujo(entrada);
    char sobrante;

    if (!(flujo >> opcion) || (flujo >> sobrante)) {
        opcion = -1;
    }

    return true;
}

bool consultarHistorial(const Clientes& clientes) {
    string nombre;

    cout << "Nombre del cliente: ";

    if (!getline(cin, nombre)) {
        return false;
    }

    ReporteClientes::mostrarHistorial(clientes, nombre);
    return true;
}

} // namespace

void menuReportes(
    const Grafo& grafo,
    const Clientes& clientes) {

    while (true) {
        cout << "\n===== REPORTES DEL SISTEMA =====\n";
        cout << "1. Reporte general de clientes\n";
        cout << "2. Historial de un cliente\n";
        cout << "3. Nodos inactivos\n";
        cout << "4. Ranking de rutas utilizadas\n";
        cout << "5. Rutas mas utilizadas\n";
        cout << "6. Rutas menos utilizadas\n";
        cout << "7. Rutas utilizadas una sola vez\n";
        cout << "0. Volver al menu principal\n";

        int opcion;

        if (!leerOpcion(opcion)) {
            return;
        }

        switch (opcion) {
            case 1:
                ReporteClientes::mostrarTodos(clientes);
                break;

            case 2:
                if (!consultarHistorial(clientes)) {
                    return;
                }
                break;

            case 3:
                ReportesRed::mostrarNodosSinClientes(grafo, clientes);
                break;

            case 4:
                ReporteRutas::mostrarRanking(clientes);
                break;

            case 5:
                ReporteRutas::mostrarMasUtilizadas(clientes);
                break;

            case 6:
                ReporteRutas::mostrarMenosUtilizadas(clientes);
                break;

            case 7:
                ReporteRutas::mostrarRutasUnicas(clientes);
                break;

            case 0:
                cout << "Regresando al menu principal...\n";
                return;

            default:
                cout << "Opcion invalida.\n";
                break;
        }
    }
}
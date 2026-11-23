#include "ReporteClientes.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace {

void mostrarLista(
    const vector<string>& elementos,
    const string& titulo) {

    cout << titulo << " (" << elementos.size() << "):\n";

    if (elementos.empty()) {
        cout << "  Ninguno\n";
        return;
    }

    for (size_t i = 0; i < elementos.size(); i++) {
        cout << "  " << (i + 1)
             << ". " << elementos[i] << '\n';
    }
}

void mostrarDetalle(const Cliente& cliente) {
    cout << "\nCliente: " << cliente.nombre << '\n';

    cout << fixed << setprecision(2);
    cout << "Consumo acumulado: "
         << cliente.consumoAcumulado << '\n';

    cout << "Eco-Tokens disponibles: "
         << cliente.tokens << '\n';

    mostrarLista(
        cliente.subestacionesConectadas,
        "Subestaciones conectadas"
    );

    mostrarLista(
        cliente.beneficiosCanjeados,
        "Beneficios canjeados"
    );

    mostrarLista(
        cliente.rutasUtilizadas,
        "Rutas utilizadas"
    );
}

} // namespace

ResumenClientes ReporteClientes::calcularResumen(
    const Clientes& clientes) {

    ResumenClientes resumen;

    const Cliente* actual = clientes.getPrimero();

    while (actual != nullptr) {
        resumen.cantidadClientes++;
        resumen.consumoTotal += actual->consumoAcumulado;
        resumen.tokensTotales += actual->tokens;

        resumen.rutasRegistradas +=
            static_cast<int>(actual->rutasUtilizadas.size());

        resumen.beneficiosCanjeados +=
            static_cast<int>(actual->beneficiosCanjeados.size());

        actual = actual->siguiente;
    }

    return resumen;
}

void ReporteClientes::mostrarTodos(
    const Clientes& clientes) {

    cout << "\n===== REPORTE GENERAL DE CLIENTES =====\n";

    ResumenClientes resumen = calcularResumen(clientes);

    cout << "Clientes registrados: "
         << resumen.cantidadClientes << '\n';

    cout << fixed << setprecision(2);
    cout << "Consumo total acumulado: "
         << resumen.consumoTotal << '\n';

    cout << "Eco-Tokens totales: "
         << resumen.tokensTotales << '\n';

    cout << "Rutas utilizadas registradas: "
         << resumen.rutasRegistradas << '\n';

    cout << "Beneficios canjeados registrados: "
         << resumen.beneficiosCanjeados << '\n';

    if (resumen.cantidadClientes == 0) {
        cout << "No hay clientes registrados.\n";
        return;
    }

    const Cliente* actual = clientes.getPrimero();
    int clientesConHistorial = 0;

    while (actual != nullptr) {
        bool tieneHistorial =
            !actual->rutasUtilizadas.empty() ||
            !actual->beneficiosCanjeados.empty();

        if (tieneHistorial) {
            cout << "\n-----------------------------------\n";
            mostrarDetalle(*actual);
            clientesConHistorial++;
        }

        actual = actual->siguiente;
    }

    cout << "\nClientes con historial: "
         << clientesConHistorial << '\n';

    if (clientesConHistorial == 0) {
        cout << "No hay clientes con historial registrado.\n";
    }
}

bool ReporteClientes::mostrarHistorial(
    const Clientes& clientes,
    const string& nombre) {

    const Cliente* cliente = clientes.buscarCliente(nombre);

    if (cliente == nullptr) {
        cout << "Cliente no encontrado.\n";
        return false;
    }

    cout << "\n===== HISTORIAL DEL CLIENTE =====\n";
    mostrarDetalle(*cliente);

    return true;
}
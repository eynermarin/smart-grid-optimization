#ifndef REPORTE_CLIENTES_H
#define REPORTE_CLIENTES_H

#include "Clientes.h"
#include <string>

struct ResumenClientes {
    int cantidadClientes = 0;
    double consumoTotal = 0;
    long long tokensTotales = 0;
    int rutasRegistradas = 0;
    int beneficiosCanjeados = 0;
};

class ReporteClientes {
public:
    static ResumenClientes calcularResumen(
        const Clientes& clientes
    );

    static void mostrarTodos(
        const Clientes& clientes
    );

    static bool mostrarHistorial(
        const Clientes& clientes,
        const std::string& nombre
    );
};

#endif
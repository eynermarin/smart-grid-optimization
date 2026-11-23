#ifndef REPORTE_RUTAS_H
#define REPORTE_RUTAS_H

#include "Clientes.h"

#include <string>
#include <vector>

struct FrecuenciaRuta {
    std::string recorrido;
    int usos = 0;
};

class ReporteRutas {
public:
    static std::vector<FrecuenciaRuta> calcularFrecuencias(
        const Clientes& clientes
    );

    static void mostrarRanking(
        const Clientes& clientes
    );

    static void mostrarMasUtilizadas(
        const Clientes& clientes
    );

    static void mostrarMenosUtilizadas(
        const Clientes& clientes
    );

    static void mostrarRutasUnicas(
        const Clientes& clientes
    );
};

#endif
#ifndef REPORTES_RED_H
#define REPORTES_RED_H

#include "Grafo.h"
#include "Clientes.h"

#include <vector>

class ReportesRed {
public:
    static std::vector<const Nodo*> obtenerNodosInactivos(
        const Grafo& grafo
    );

    static void mostrarNodosInactivos(
        const Grafo& grafo
    );

    static std::vector<const Nodo*> obtenerNodosSinClientes(
        const Grafo& grafo,
        const Clientes& clientes
    );

    static void mostrarNodosSinClientes(
        const Grafo& grafo,
        const Clientes& clientes
    );
};

#endif
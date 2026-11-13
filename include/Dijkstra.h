#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Rutas.h"

#include <string>

enum class CriterioRuta {
    Distancia,
    Perdidas
};

class Dijkstra {
public:
    static bool calcular(
        const Grafo& grafo,
        const std::string& nombreOrigen,
        const std::string& regionOrigen,
        const std::string& nombreDestino,
        const std::string& regionDestino,
        CriterioRuta criterio,
        Ruta& resultado
    );
};

#endif
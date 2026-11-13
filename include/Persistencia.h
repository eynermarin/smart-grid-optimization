#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <string>
#include "Grafo.h"

class Persistencia {
public:
    static bool cargarRed(const std::string& ruta, Grafo& grafo);
    static bool guardarRed(const std::string& ruta, const Grafo& grafo);
};

#endif

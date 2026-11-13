#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <string>
#include "Grafo.h"
#include "Clientes.h"

class Persistencia {
public:
    static bool cargarRed(const std::string& ruta, Grafo& grafo);
    static bool cargarClientes(const std::string& ruta, Clientes& clientes);
    static bool guardarClientes(const std::string& ruta, const Clientes& clientes);
    static bool guardarRed(const std::string& ruta, const Grafo& grafo);
};

#endif

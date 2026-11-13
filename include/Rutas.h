#ifndef RUTAS_H
#define RUTAS_H

#include "Grafo.h"

#include <string>
#include <vector>

struct Ruta {
    std::vector<const Nodo*> nodos;
    std::vector<const Arco*> arcos;

    double distanciaTotalKm = 0;
    double sumaPerdidasPorcentaje = 0;
};

class Rutas {
private:
    static bool medioPermitido(
        const std::string& medio,
        const std::vector<std::string>& permitidos
    );

    static void buscarRecursivo(
        const Nodo* actual,
        const Nodo* destino,
        const std::vector<std::string>& mediosPermitidos,
        std::vector<const Nodo*>& caminoNodos,
        std::vector<const Arco*>& caminoArcos,
        std::vector<Ruta>& resultados
    );

public:
    static std::vector<Ruta> buscarTodas(
        const Grafo& grafo,
        const std::string& nombreOrigen,
        const std::string& regionOrigen,
        const std::string& nombreDestino,
        const std::string& regionDestino,
        const std::vector<std::string>& mediosPermitidos
    );

    static void mostrar(const std::vector<Ruta>& rutas);
};

#endif
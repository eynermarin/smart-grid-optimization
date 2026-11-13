#ifndef OPTIMIZADOR_TOKENS_H
#define OPTIMIZADOR_TOKENS_H

#include "Rutas.h"

#include <string>
#include <vector>

struct ResultadoEcoRuta {
    Ruta ruta;
    long long tokens = 0;
    bool encontrada = false;
};

class OptimizadorTokens {
private:
    static long long calcularPuntaje(
        const Ruta& ruta,
        bool horarioValle
    );

    static void explorar(
        const Nodo* actual,
        const Nodo* destino,
        bool horarioValle,
        std::vector<const Nodo*>& nodos,
        std::vector<const Arco*>& arcos,
        ResultadoEcoRuta& mejor
    );

public:
    static ResultadoEcoRuta buscarMejorRuta(
        const Grafo& grafo,
        const std::string& nombreOrigen,
        const std::string& regionOrigen,
        const std::string& nombreDestino,
        const std::string& regionDestino,
        bool horarioValle
    );
};

#endif
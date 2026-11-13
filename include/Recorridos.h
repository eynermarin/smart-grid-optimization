#ifndef RECORRIDOS_H
#define RECORRIDOS_H

#include "Grafo.h"

#include <vector>

class Recorridos {
private:
    static void dfsRecursivo(
        const Nodo* actual,
        std::vector<const Nodo*>& resultado,
        std::vector<const Nodo*>& visitados
    );

public:
    static std::vector<const Nodo*> bfs(
        const Grafo& grafo,
        const std::string& nombre,
        const std::string& region
    );

    static std::vector<const Nodo*> dfs(
        const Grafo& grafo,
        const std::string& nombre,
        const std::string& region
    );

    static void mostrar(
        const std::vector<const Nodo*>& recorrido
    );
};

#endif
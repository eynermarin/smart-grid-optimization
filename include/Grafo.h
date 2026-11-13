#ifndef GRAFO_H
#define GRAFO_H

#include <string>

struct Nodo {
    std::string nombre;
    std::string region;
    std::string tipo;
    Nodo* siguiente;

    Nodo(std::string nombre, std::string region, std::string tipo);
};

class Grafo {
private:
    Nodo* primero;
    int cantidadNodos;

public:
    Grafo();
    ~Grafo();

    Grafo(const Grafo&) = delete;
    Grafo& operator=(const Grafo&) = delete;

    bool agregarNodo(std::string nombre, std::string region, std::string tipo);
    const Nodo* buscarNodo(std::string nombre, std::string region) const;
    void mostrarNodos() const;
    int getCantidadNodos() const;
};

#endif

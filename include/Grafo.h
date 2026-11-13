#ifndef GRAFO_H
#define GRAFO_H

#include <string>

struct Arco;

struct Nodo {
    std::string nombre;
    std::string region;
    std::string tipo;

    Nodo* siguiente;
    Arco* primerSaliente;
    Arco* primerEntrante;

    Nodo(std::string nombre, std::string region, std::string tipo);
};

struct Arco {
    Nodo* origen;
    Nodo* destino;

    std::string medio;
    double distanciaKm;
    double tiempoMs;
    double capacidadMaxima;
    double perdidaPorcentaje;

    Arco* siguienteSaliente;
    Arco* siguienteEntrante;
    Arco* siguienteGlobal;

    Arco(Nodo* origen, Nodo* destino,
         std::string medio, double distancia,
         double tiempo, double capacidad, double perdida);
};

class Grafo {
private:
    Nodo* primero;
    Arco* primerArco;

    int cantidadNodos;
    int cantidadArcos;

    Nodo* buscarNodoInterno(std::string nombre,
                            std::string region) const;

    void eliminarArcoInterno(Arco* arco);

public:
    Grafo();
    ~Grafo();

    Grafo(const Grafo&) = delete;
    Grafo& operator=(const Grafo&) = delete;

    bool agregarNodo(std::string nombre, std::string region,
                     std::string tipo);

    const Nodo* buscarNodo(std::string nombre,
                           std::string region) const;

    bool agregarArco(std::string origen, std::string regionOrigen,
                     std::string destino, std::string regionDestino,
                     std::string medio, double distancia,
                     double tiempo, double capacidad, double perdida);

    const Arco* buscarArco(std::string origen, std::string regionOrigen,
                          std::string destino, std::string regionDestino,
                          std::string medio) const;

    bool modificarNodo(std::string nombre, std::string region,
                       std::string nuevoNombre, std::string nuevaRegion,
                       std::string nuevoTipo);

    bool eliminarNodo(std::string nombre, std::string region);

    bool modificarArco(std::string origen, std::string regionOrigen,
                       std::string destino, std::string regionDestino,
                       std::string medio, double distancia,
                       double tiempo, double capacidad, double perdida);

    bool eliminarArco(std::string origen, std::string regionOrigen,
                      std::string destino, std::string regionDestino,
                      std::string medio);
    void mostrarNodos() const;
    void mostrarArcos() const;

    const Nodo* getPrimerNodo() const;
    const Arco* getPrimerArco() const;
    void intercambiar(Grafo& otro);
    int getCantidadNodos() const;
    int getCantidadArcos() const;
};

#endif




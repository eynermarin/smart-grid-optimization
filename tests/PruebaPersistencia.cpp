#include <iostream>
#include <fstream>
#include <cassert>
#include "Grafo.h"
#include "Persistencia.h"

using namespace std;

int main() {
    cout << "===== PRUEBAS DE PERSISTENCIA =====\n";

    Grafo grafo;

    assert(Persistencia::cargarRed("data/red_electrica.json", grafo));
    cout << "OK - Archivo JSON cargado\n";

    assert(grafo.getCantidadNodos() == 3);
    assert(grafo.getCantidadArcos() == 2);
    cout << "OK - Cantidad de nodos y arcos\n";

    const Arco* arco = grafo.buscarArco(
        "Planta Solar Alpha", "Zona Norte",
        "Subestacion Central", "Zona Norte",
        "alta_tension"
    );

    assert(arco != nullptr);
    assert(arco->distanciaKm == 45);
    assert(arco->capacidadMaxima == 100);
    cout << "OK - Atributos recuperados del JSON\n";

    assert(grafo.modificarArco(
        "Planta Solar Alpha", "Zona Norte",
        "Subestacion Central", "Zona Norte",
        "alta_tension", 50, 12, 120, 2.0
    ));
    cout << "OK - Modificacion en memoria\n";

    assert(Persistencia::guardarRed(
        "data/red_electrica_prueba.json", grafo
    ));
    cout << "OK - Archivo JSON guardado\n";

    Grafo recuperado;

    assert(Persistencia::cargarRed(
        "data/red_electrica_prueba.json", recuperado
    ));

    assert(recuperado.getCantidadNodos() == 3);
    assert(recuperado.getCantidadArcos() == 2);

    const Arco* modificado = recuperado.buscarArco(
        "Planta Solar Alpha", "Zona Norte",
        "Subestacion Central", "Zona Norte",
        "alta_tension"
    );

    assert(modificado != nullptr);
    assert(modificado->distanciaKm == 50);
    assert(modificado->tiempoMs == 12);
    assert(modificado->capacidadMaxima == 120);
    assert(modificado->perdidaPorcentaje == 2.0);

    cout << "OK - Cambios recuperados correctamente\n";

    // Archivo incorrecto con una conexion inexistente
    {
        ofstream archivo("data/red_invalida_prueba.json");
        archivo << R"({
            "regiones": [
                {
                    "nombre": "Zona Norte",
                    "puntos_interconexion": [
                        {"nombre": "A", "tipo": "generador"}
                    ]
                }
            ],
            "lineas_transmision": [
                {
                    "origen": {"region": "Zona Norte", "punto": "A"},
                    "destino": {"region": "Zona Norte", "punto": "NO_EXISTE"},
                    "medio": "alta_tension",
                    "distancia_km": 10,
                    "tiempo_ms": 2,
                    "capacidad_maxima": 50,
                    "perdida_porcentaje": 1
                }
            ]
        })";
    }

    assert(!Persistencia::cargarRed(
        "data/red_invalida_prueba.json", recuperado
    ));

    assert(recuperado.getCantidadNodos() == 3);
    assert(recuperado.getCantidadArcos() == 2);

    cout << "OK - JSON invalido rechazado sin perder datos\n";

    cout << "\nPRUEBA DE PERSISTENCIA EXITOSA\n";
    return 0;
}

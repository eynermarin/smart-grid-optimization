#include "Persistencia.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;
using json = nlohmann::json;

bool Persistencia::cargarRed(const string& ruta, Grafo& grafo) {
    try {
        ifstream archivo(ruta);

        if (!archivo.is_open()) {
            cerr << "No se pudo abrir: " << ruta << endl;
            return false;
        }

        json datos;
        archivo >> datos;

        if (!datos.is_object() ||
            !datos.contains("regiones") ||
            !datos.contains("lineas_transmision") ||
            !datos.at("regiones").is_array() ||
            !datos.at("lineas_transmision").is_array()) {
            return false;
        }

        Grafo temporal;

        for (const auto& region : datos.at("regiones")) {
            string nombreRegion = region.at("nombre").get<string>();

            for (const auto& punto : region.at("puntos_interconexion")) {
                string nombre = punto.at("nombre").get<string>();
                string tipo = punto.at("tipo").get<string>();

                if (!temporal.agregarNodo(nombre, nombreRegion, tipo)) {
                    cerr << "Nodo invalido o duplicado: " << nombre << endl;
                    return false;
                }
            }
        }

        for (const auto& linea : datos.at("lineas_transmision")) {
            const auto& origen = linea.at("origen");
            const auto& destino = linea.at("destino");

            string nombreOrigen = origen.at("punto").get<string>();
            string regionOrigen = origen.at("region").get<string>();

            string nombreDestino = destino.at("punto").get<string>();
            string regionDestino = destino.at("region").get<string>();

            string medio = linea.at("medio").get<string>();
            double distancia = linea.at("distancia_km").get<double>();
            double tiempo = linea.at("tiempo_ms").get<double>();
            double capacidad = linea.at("capacidad_maxima").get<double>();
            double perdida = linea.at("perdida_porcentaje").get<double>();

            if (!temporal.agregarArco(
                nombreOrigen, regionOrigen,
                nombreDestino, regionDestino,
                medio, distancia, tiempo, capacidad, perdida)) {

                cerr << "Conexion invalida: "
                     << nombreOrigen << " -> " << nombreDestino << endl;
                return false;
            }
        }

        grafo.intercambiar(temporal);

        return true;

    } catch (const exception& e) {
        cerr << "Error cargando JSON: " << e.what() << endl;
        return false;
    }
}

bool Persistencia::guardarRed(const string& ruta, const Grafo& grafo) {
    try {
        json datos;
        datos["regiones"] = json::array();
        datos["lineas_transmision"] = json::array();

        map<string, json> regiones;

        const Nodo* nodo = grafo.getPrimerNodo();

        while (nodo != nullptr) {
            string region = nodo->region;

            if (regiones.find(region) == regiones.end()) {
                regiones[region] = json::array();
            }

            regiones[region].push_back({
                {"nombre", nodo->nombre},
                {"tipo", nodo->tipo}
            });

            nodo = nodo->siguiente;
        }

        for (const auto& par : regiones) {
            datos["regiones"].push_back({
                {"nombre", par.first},
                {"puntos_interconexion", par.second}
            });
        }

        const Arco* arco = grafo.getPrimerArco();

        while (arco != nullptr) {
            datos["lineas_transmision"].push_back({
                {"origen", {
                    {"region", arco->origen->region},
                    {"punto", arco->origen->nombre}
                }},
                {"destino", {
                    {"region", arco->destino->region},
                    {"punto", arco->destino->nombre}
                }},
                {"medio", arco->medio},
                {"distancia_km", arco->distanciaKm},
                {"tiempo_ms", arco->tiempoMs},
                {"capacidad_maxima", arco->capacidadMaxima},
                {"perdida_porcentaje", arco->perdidaPorcentaje}
            });

            arco = arco->siguienteGlobal;
        }

        ofstream archivo(ruta);

        if (!archivo.is_open()) {
            cerr << "No se pudo guardar: " << ruta << endl;
            return false;
        }

        archivo << datos.dump(4);

        archivo.flush();
        return archivo.good();

    } catch (const exception& e) {
        cerr << "Error guardando JSON: " << e.what() << endl;
        return false;
    }
}

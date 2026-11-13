#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "Grafo.h"
#include "Persistencia.h"
#include "MenuClientes.h"
#include "MenuBeneficios.h"
#include "MenuCanje.h"

using namespace std;

const string ARCHIVO_RED = "data/red_electrica.json";
const string ARCHIVO_CLIENTES = "data/clientes.json";
const string ARCHIVO_BENEFICIOS = "data/beneficios.json";

string leerTexto(const string& mensaje) {
    string texto;
    cout << mensaje;
    getline(cin, texto);
    return texto;
}

int leerOpcion() {
    string entrada;
    int opcion;
    char extra;

    if (!getline(cin, entrada)) {
        return 0;
    }

    istringstream flujo(entrada);

    if (!(flujo >> opcion) || (flujo >> extra)) {
        return -1;
    }

    return opcion;
}

bool leerNumero(const string& mensaje, double& valor) {
    string entrada = leerTexto(mensaje);
    istringstream flujo(entrada);

    if (!(flujo >> valor) || !isfinite(valor)) {
        return false;
    }

    flujo >> ws;
    return flujo.eof();
}

void mostrarMenu() {
    cout << "\n========================================\n";
    cout << "       SMART GRID OPTIMIZATION\n";
    cout << "========================================\n";
    cout << "1. Gestion de la red electrica\n";
    cout << "2. Gestion de clientes\n";
    cout << "3. Gestion de beneficios\n";
    cout << "4. Recorridos BFS y DFS\n";
    cout << "5. Consultas de rutas\n";
    cout << "6. Eco-Tokens y canje de beneficios\n";
    cout << "7. Reportes del sistema\n";
    cout << "8. Guardar datos en JSON\n";
    cout << "0. Salir\n";
    cout << "========================================\n";
    cout << "Seleccione una opcion: ";
}

void agregarNodo(Grafo& grafo) {
    cout << "\n===== AGREGAR NODO =====\n";

    string nombre = leerTexto("Nombre: ");
    string region = leerTexto("Region: ");
    string tipo = leerTexto("Tipo de nodo: ");

    if (grafo.agregarNodo(nombre, region, tipo)) {
        cout << "Nodo agregado correctamente.\n";
    } else {
        cout << "ERROR: Datos invalidos o nodo duplicado.\n";
    }
}

void modificarNodo(Grafo& grafo) {
    cout << "\n===== MODIFICAR NODO =====\n";

    string nombre = leerTexto("Nombre actual: ");
    string region = leerTexto("Region actual: ");

    string nuevoNombre = leerTexto("Nuevo nombre: ");
    string nuevaRegion = leerTexto("Nueva region: ");
    string nuevoTipo = leerTexto("Nuevo tipo: ");

    if (grafo.modificarNodo(nombre, region,
                           nuevoNombre, nuevaRegion, nuevoTipo)) {
        cout << "Nodo modificado correctamente.\n";
    } else {
        cout << "ERROR: No se pudo modificar el nodo.\n";
    }
}

void eliminarNodo(Grafo& grafo) {
    cout << "\n===== ELIMINAR NODO =====\n";

    string nombre = leerTexto("Nombre: ");
    string region = leerTexto("Region: ");

    const Nodo* nodo = grafo.buscarNodo(nombre, region);

    if (nodo == nullptr) {
        cout << "ERROR: Nodo inexistente.\n";
        return;
    }

    string confirmar = leerTexto(
        "Se eliminaran sus conexiones. Confirmar (s/n): "
    );

    if (confirmar != "s" && confirmar != "S") {
        cout << "Operacion cancelada.\n";
        return;
    }

    if (grafo.eliminarNodo(nombre, region)) {
        cout << "Nodo y conexiones eliminados correctamente.\n";
    } else {
        cout << "ERROR: No se pudo eliminar el nodo.\n";
    }
}

bool leerAtributosArco(double& distancia, double& tiempo,
                       double& capacidad, double& perdida) {
    if (!leerNumero("Distancia en km: ", distancia) ||
        !leerNumero("Tiempo en ms: ", tiempo) ||
        !leerNumero("Capacidad maxima: ", capacidad) ||
        !leerNumero("Perdida en porcentaje: ", perdida)) {
        cout << "ERROR: Los atributos deben ser numeros validos.\n";
        return false;
    }

    return true;
}

void agregarConexion(Grafo& grafo) {
    cout << "\n===== AGREGAR CONEXION =====\n";

    string origen = leerTexto("Nodo origen: ");
    string regionOrigen = leerTexto("Region origen: ");
    string destino = leerTexto("Nodo destino: ");
    string regionDestino = leerTexto("Region destino: ");
    string medio = leerTexto(
        "Medio (alta_tension/subterraneo/red_local): "
    );

    double distancia, tiempo, capacidad, perdida;

    if (!leerAtributosArco(distancia, tiempo, capacidad, perdida)) {
        return;
    }

    if (grafo.agregarArco(origen, regionOrigen,
                         destino, regionDestino, medio,
                         distancia, tiempo, capacidad, perdida)) {
        cout << "Conexion agregada correctamente.\n";
    } else {
        cout << "ERROR: Conexion invalida o duplicada.\n";
    }
}

void modificarConexion(Grafo& grafo) {
    cout << "\n===== MODIFICAR CONEXION =====\n";

    string origen = leerTexto("Nodo origen: ");
    string regionOrigen = leerTexto("Region origen: ");
    string destino = leerTexto("Nodo destino: ");
    string regionDestino = leerTexto("Region destino: ");
    string medio = leerTexto("Medio actual: ");

    double distancia, tiempo, capacidad, perdida;

    if (!leerAtributosArco(distancia, tiempo, capacidad, perdida)) {
        return;
    }

    if (grafo.modificarArco(origen, regionOrigen,
                           destino, regionDestino, medio,
                           distancia, tiempo, capacidad, perdida)) {
        cout << "Conexion modificada correctamente.\n";
    } else {
        cout << "ERROR: Conexion inexistente o datos invalidos.\n";
    }
}

void eliminarConexion(Grafo& grafo) {
    cout << "\n===== ELIMINAR CONEXION =====\n";

    string origen = leerTexto("Nodo origen: ");
    string regionOrigen = leerTexto("Region origen: ");
    string destino = leerTexto("Nodo destino: ");
    string regionDestino = leerTexto("Region destino: ");
    string medio = leerTexto("Medio: ");

    if (grafo.buscarArco(origen, regionOrigen,
                        destino, regionDestino, medio) == nullptr) {
        cout << "ERROR: Conexion inexistente.\n";
        return;
    }

    string confirmar = leerTexto("Confirmar eliminacion (s/n): ");

    if (confirmar != "s" && confirmar != "S") {
        cout << "Operacion cancelada.\n";
        return;
    }

    if (grafo.eliminarArco(origen, regionOrigen,
                          destino, regionDestino, medio)) {
        cout << "Conexion eliminada correctamente.\n";
    } else {
        cout << "ERROR: No se pudo eliminar la conexion.\n";
    }
}

void menuRed(Grafo& grafo) {
    int opcion;

    do {
        cout << "\n===== GESTION DE RED ELECTRICA =====\n";
        cout << "1. Mostrar nodos\n";
        cout << "2. Mostrar conexiones\n";
        cout << "3. Agregar nodo\n";
        cout << "4. Modificar nodo\n";
        cout << "5. Eliminar nodo\n";
        cout << "6. Agregar conexion\n";
        cout << "7. Modificar conexion\n";
        cout << "8. Eliminar conexion\n";
        cout << "0. Volver al menu principal\n";
        cout << "Seleccione una opcion: ";

        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                cout << "\nNODOS REGISTRADOS:\n";
                if (grafo.getCantidadNodos() == 0)
                    cout << "No hay nodos registrados.\n";
                else
                    grafo.mostrarNodos();
                break;

            case 2:
                cout << "\nCONEXIONES REGISTRADAS:\n";
                if (grafo.getCantidadArcos() == 0)
                    cout << "No hay conexiones registradas.\n";
                else
                    grafo.mostrarArcos();
                break;

            case 3:
                agregarNodo(grafo);
                break;

            case 4:
                modificarNodo(grafo);
                break;

            case 5:
                eliminarNodo(grafo);
                break;

            case 6:
                agregarConexion(grafo);
                break;

            case 7:
                modificarConexion(grafo);
                break;

            case 8:
                eliminarConexion(grafo);
                break;

            case 0:
                cout << "Regresando al menu principal...\n";
                break;

            default:
                cout << "Opcion invalida.\n";
        }

    } while (opcion != 0);
}

int main() {
    Grafo grafo;
    Clientes clientes;
    Beneficios beneficios;

    cout << "Iniciando Smart Grid Optimization...\n";

    if (!Persistencia::cargarRed(ARCHIVO_RED, grafo)) {
        cerr << "ERROR: No se pudo cargar la red electrica.\n";
        cerr << "Verifique el archivo: " << ARCHIVO_RED << '\n';
        return 1;
    }

    cout << "Red electrica cargada correctamente.\n";
    cout << "Nodos: " << grafo.getCantidadNodos() << '\n';
    cout << "Conexiones: " << grafo.getCantidadArcos() << '\n';

    if (!Persistencia::cargarClientes(ARCHIVO_CLIENTES, clientes)) {
        cerr << "ERROR: No se pudo cargar clientes.json.\n";
        return 1;
    }

    cout << "Clientes cargados: "
         << clientes.getCantidad() << '\n';

    if (!Persistencia::cargarBeneficios(
            ARCHIVO_BENEFICIOS, beneficios)) {
        cerr << "ERROR: No se pudo cargar beneficios.json.\n";
        return 1;
    }

    cout << "Beneficios cargados: "
         << beneficios.getCantidad() << '\n';
    int opcion;

    do {
        mostrarMenu();
        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                menuRed(grafo);
                break;

            case 2:
                menuClientes(clientes);
                break;

            case 3:
                menuBeneficios(beneficios);
                break;

            case 4:
            case 5:
            case 7:
                cout << "\nModulo pendiente de implementacion.\n";
                break;

            case 6:
                menuCanje(clientes, beneficios);
                break;

            case 8: {
                bool redGuardada =
                    Persistencia::guardarRed(ARCHIVO_RED, grafo);

                bool clientesGuardados =
                    Persistencia::guardarClientes(
                        ARCHIVO_CLIENTES, clientes);

                bool beneficiosGuardados =
                    Persistencia::guardarBeneficios(
                        ARCHIVO_BENEFICIOS, beneficios);

                if (redGuardada && clientesGuardados && beneficiosGuardados)
                    cout << "\nDatos guardados correctamente.\n";
                else
                    cerr << "\nError al guardar los datos.\n";

                break;
            }

            case 0: {
                cout << "\nGuardando datos antes de salir...\n";

                bool redGuardada =
                    Persistencia::guardarRed(ARCHIVO_RED, grafo);

                bool clientesGuardados =
                    Persistencia::guardarClientes(
                        ARCHIVO_CLIENTES, clientes);

                bool beneficiosGuardados =
                    Persistencia::guardarBeneficios(
                        ARCHIVO_BENEFICIOS, beneficios);

                if (!redGuardada || !clientesGuardados || !beneficiosGuardados) {
                    cerr << "Error al guardar los datos.\n";
                    return 1;
                }

                cout << "Datos guardados correctamente.\n";
                cout << "Saliendo de Smart Grid...\n";
                break;
            }

            default:
                cout << "\nOpcion invalida.\n";
        }

    } while (opcion != 0);

    return 0;
}
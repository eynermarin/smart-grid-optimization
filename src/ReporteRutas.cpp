#include "ReporteRutas.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

vector<FrecuenciaRuta> ReporteRutas::calcularFrecuencias(
    const Clientes& clientes) {

    map<string, int> contador;

    const Cliente* actual = clientes.getPrimero();

    while (actual != nullptr) {
        for (const string& ruta : actual->rutasUtilizadas) {
            if (!ruta.empty()) {
                contador[ruta]++;
            }
        }

        actual = actual->siguiente;
    }

    vector<FrecuenciaRuta> resultado;

    for (const auto& elemento : contador) {
        resultado.push_back({
            elemento.first,
            elemento.second
        });
    }

    sort(
        resultado.begin(),
        resultado.end(),
        [](const FrecuenciaRuta& a, const FrecuenciaRuta& b) {
            if (a.usos != b.usos) {
                return a.usos > b.usos;
            }

            return a.recorrido < b.recorrido;
        }
    );

    return resultado;
}

void ReporteRutas::mostrarRanking(
    const Clientes& clientes) {

    cout << "\n===== RANKING DE RUTAS UTILIZADAS =====\n";

    vector<FrecuenciaRuta> frecuencias =
        calcularFrecuencias(clientes);

    if (frecuencias.empty()) {
        cout << "No existen rutas registradas.\n";
        return;
    }

    int usosTotales = 0;

    for (const FrecuenciaRuta& ruta : frecuencias) {
        usosTotales += ruta.usos;
    }

    cout << "Rutas diferentes: "
         << frecuencias.size() << '\n';

    cout << "Total de recorridos registrados: "
         << usosTotales << '\n';

    for (size_t i = 0; i < frecuencias.size(); i++) {
        cout << "\nPosicion " << (i + 1) << ":\n";
        cout << "Ruta: " << frecuencias[i].recorrido << '\n';
        cout << "Usos: " << frecuencias[i].usos << '\n';
    }
}

void ReporteRutas::mostrarMasUtilizadas(
    const Clientes& clientes) {

    cout << "\n===== RUTAS MAS UTILIZADAS =====\n";

    vector<FrecuenciaRuta> frecuencias =
        calcularFrecuencias(clientes);

    if (frecuencias.empty()) {
        cout << "No existen rutas registradas.\n";
        return;
    }

    const int maximo = frecuencias.front().usos;

    cout << "Mayor frecuencia: " << maximo << '\n';

    for (const FrecuenciaRuta& ruta : frecuencias) {
        if (ruta.usos != maximo) {
            break;
        }

        cout << "Ruta: " << ruta.recorrido << '\n';
        cout << "Usos: " << ruta.usos << '\n';
    }
}

void ReporteRutas::mostrarMenosUtilizadas(
    const Clientes& clientes) {

    cout << "\n===== RUTAS MENOS UTILIZADAS =====\n";

    vector<FrecuenciaRuta> frecuencias =
        calcularFrecuencias(clientes);

    if (frecuencias.empty()) {
        cout << "No existen rutas registradas.\n";
        return;
    }

    const int minimo = frecuencias.back().usos;

    cout << "Menor frecuencia: " << minimo << '\n';

    for (const FrecuenciaRuta& ruta : frecuencias) {
        if (ruta.usos == minimo) {
            cout << "Ruta: " << ruta.recorrido << '\n';
            cout << "Usos: " << ruta.usos << '\n';
        }
    }
}
#include "Beneficios.h"
#include "Persistencia.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

using namespace std;

int main() {
    cout << "===== PRUEBAS JSON DE BENEFICIOS =====\n";

    Beneficios beneficios;

    assert(Persistencia::cargarBeneficios(
        "data/beneficios.json", beneficios));

    cout << "OK - Catalogo cargado correctamente\n";

    assert(beneficios.getCantidad() == 5);
    cout << "OK - Cinco beneficios recuperados\n";

    const Beneficio* descuento =
        beneficios.buscarBeneficio("Descuento de energia");

    assert(descuento != nullptr);
    assert(descuento->costoTokens == 100);

    cout << "OK - Atributos recuperados correctamente\n";

    assert(beneficios.modificarBeneficio(
        "Descuento de energia",
        "Descuento especial",
        "Descuento para usuarios",
        175
    ));

    assert(beneficios.agregarBeneficio(
        "Beneficio temporal",
        "Prueba de persistencia",
        50
    ));

    cout << "OK - Catalogo modificado en memoria\n";

    assert(Persistencia::guardarBeneficios(
        "data/beneficios_prueba.json",
        beneficios
    ));

    cout << "OK - Catalogo guardado en JSON\n";

    Beneficios recuperados;

    assert(Persistencia::cargarBeneficios(
        "data/beneficios_prueba.json",
        recuperados
    ));

    assert(recuperados.getCantidad() == 6);

    const Beneficio* especial =
        recuperados.buscarBeneficio("Descuento especial");

    assert(especial != nullptr);
    assert(especial->costoTokens == 175);
    assert(especial->descripcion == "Descuento para usuarios");

    assert(recuperados.buscarBeneficio(
        "Beneficio temporal") != nullptr);

    cout << "OK - Modificaciones recuperadas desde JSON\n";

    {
        ofstream archivo("data/beneficios_invalidos_prueba.json");

        archivo << R"({
            "beneficios": [
                {
                    "nombre": "Beneficio incorrecto",
                    "descripcion": "Costo invalido",
                    "costo_tokens": -100
                }
            ]
        })";
    }

    assert(!Persistencia::cargarBeneficios(
        "data/beneficios_invalidos_prueba.json",
        recuperados
    ));

    assert(recuperados.getCantidad() == 6);

    cout << "OK - Costo negativo rechazado sin perder datos\n";

    {
        ofstream archivo("data/beneficios_invalidos_prueba.json");

        archivo << R"({
            "beneficios": [
                {
                    "nombre": "Duplicado",
                    "descripcion": "Primer registro",
                    "costo_tokens": 100
                },
                {
                    "nombre": "Duplicado",
                    "descripcion": "Segundo registro",
                    "costo_tokens": 200
                }
            ]
        })";
    }

    assert(!Persistencia::cargarBeneficios(
        "data/beneficios_invalidos_prueba.json",
        recuperados
    ));

    assert(recuperados.getCantidad() == 6);

    cout << "OK - Duplicados rechazados sin perder datos\n";

    cout << "\nPRUEBA JSON DE BENEFICIOS EXITOSA\n";

    return 0;
}
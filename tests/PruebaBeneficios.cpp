#include "Beneficios.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

using namespace std;

int main() {
    cout << "===== PRUEBAS DE BENEFICIOS =====\n";

    Beneficios beneficios;

    assert(beneficios.getCantidad() == 0);
    assert(beneficios.getPrimero() == nullptr);

    cout << "OK - Lista inicialmente vacia\n";

    assert(beneficios.agregarBeneficio(
        "Descuento", "Descuento en factura", 100));

    assert(beneficios.agregarBeneficio(
        "Revision", "Revision del servicio", 200));

    assert(beneficios.agregarBeneficio(
        "Prioridad", "Atencion prioritaria", 300));

    assert(beneficios.getCantidad() == 3);

    cout << "OK - Registro de tres beneficios\n";

    const Beneficio* descuento =
        beneficios.buscarBeneficio("Descuento");

    assert(descuento != nullptr);
    assert(descuento->costoTokens == 100);
    assert(descuento->descripcion == "Descuento en factura");

    cout << "OK - Busqueda de beneficio existente\n";

    assert(beneficios.buscarBeneficio("Inexistente") == nullptr);

    cout << "OK - Busqueda de beneficio inexistente\n";

    assert(!beneficios.agregarBeneficio(
        "Descuento", "Duplicado", 50));

    assert(!beneficios.agregarBeneficio(
        "", "Sin nombre", 50));

    assert(!beneficios.agregarBeneficio(
        "Invalido", "", 50));

    assert(!beneficios.agregarBeneficio(
        "Gratis", "Costo incorrecto", 0));

    assert(!beneficios.agregarBeneficio(
        "Negativo", "Costo incorrecto", -10));

    assert(beneficios.getCantidad() == 3);

    cout << "OK - Datos invalidos y duplicados rechazados\n";

    assert(beneficios.modificarBeneficio(
        "Descuento",
        "Descuento Especial",
        "Descuento de energia",
        150
    ));

    descuento = beneficios.buscarBeneficio("Descuento Especial");

    assert(descuento != nullptr);
    assert(descuento->costoTokens == 150);
    assert(descuento->descripcion == "Descuento de energia");
    assert(beneficios.buscarBeneficio("Descuento") == nullptr);

    cout << "OK - Modificacion correcta\n";

    assert(!beneficios.modificarBeneficio(
        "Descuento Especial",
        "Revision",
        "Nombre duplicado",
        100
    ));

    assert(!beneficios.modificarBeneficio(
        "No Existe",
        "Otro",
        "Descripcion",
        100
    ));

    assert(!beneficios.modificarBeneficio(
        "Descuento Especial",
        "Descuento Especial",
        "Descripcion",
        -1
    ));

    cout << "OK - Modificaciones invalidas rechazadas\n";

    ostringstream captura;
    streambuf* salidaOriginal = cout.rdbuf(captura.rdbuf());

    beneficios.mostrarBeneficios();

    cout.rdbuf(salidaOriginal);

    string salida = captura.str();

    size_t posicionPrioridad = salida.find("Beneficio: Prioridad");
    size_t posicionRevision = salida.find("Beneficio: Revision");
    size_t posicionDescuento = salida.find(
        "Beneficio: Descuento Especial");

    assert(posicionPrioridad != string::npos);
    assert(posicionRevision != string::npos);
    assert(posicionDescuento != string::npos);

    assert(posicionPrioridad < posicionRevision);
    assert(posicionRevision < posicionDescuento);

    cout << "OK - Recorrido recursivo en orden correcto\n";

    assert(beneficios.eliminarBeneficio("Revision"));
    assert(beneficios.getCantidad() == 2);
    assert(beneficios.buscarBeneficio("Revision") == nullptr);

    cout << "OK - Eliminacion de nodo intermedio\n";

    assert(beneficios.eliminarBeneficio("Prioridad"));
    assert(beneficios.getCantidad() == 1);

    cout << "OK - Eliminacion del primer nodo\n";

    assert(!beneficios.eliminarBeneficio("Inexistente"));

    cout << "OK - Eliminacion inexistente rechazada\n";

    assert(beneficios.eliminarBeneficio("Descuento Especial"));
    assert(beneficios.getCantidad() == 0);
    assert(beneficios.getPrimero() == nullptr);

    cout << "OK - Lista completamente vacia\n";

    cout << "\nPRUEBA DE BENEFICIOS EXITOSA\n";

    return 0;
}
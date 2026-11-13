#include "Clientes.h"
#include "Beneficios.h"
#include "Persistencia.h"

#include <iostream>
#include <string>

using namespace std;

int main() {
    cout << "===== PRUEBAS DE CANJE DE ECO-TOKENS =====\n";

    Clientes clientes;
    Beneficios beneficios;

    auto verificar = [](bool condicion, const string& mensaje) {
        if (!condicion) {
            cerr << "FALLO - " << mensaje << '\n';
            return false;
        }

        cout << "OK - " << mensaje << '\n';
        return true;
    };

    if (!verificar(
        clientes.agregarCliente("Hospital General") &&
        beneficios.agregarBeneficio(
            "Descuento de energia",
            "Descuento en la factura", 100) &&
        beneficios.agregarBeneficio(
            "Revision tecnica",
            "Revision de instalaciones", 200),
        "Cliente y beneficios creados")) {
        return 1;
    }

    if (!verificar(
        clientes.asignarTokens("Hospital General", 450),
        "Asignacion de 450 Eco-Tokens")) {
        return 1;
    }

    if (!verificar(
        clientes.canjearBeneficio(
            "Hospital General",
            "Descuento de energia", beneficios),
        "Primer canje exitoso")) {
        return 1;
    }

    const Cliente* cliente =
        clientes.buscarCliente("Hospital General");

    if (!verificar(
        cliente != nullptr &&
        cliente->tokens == 350 &&
        cliente->beneficiosCanjeados.size() == 1 &&
        cliente->beneficiosCanjeados[0] == "Descuento de energia",
        "Descuento aplicado e historial registrado")) {
        return 1;
    }

    if (!verificar(
        clientes.canjearBeneficio(
            "Hospital General",
            "Revision tecnica", beneficios),
        "Segundo canje exitoso")) {
        return 1;
    }

    cliente = clientes.buscarCliente("Hospital General");

    if (!verificar(
        cliente->tokens == 150 &&
        cliente->beneficiosCanjeados.size() == 2 &&
        cliente->beneficiosCanjeados[1] == "Revision tecnica",
        "Saldo e historial actualizados")) {
        return 1;
    }

    if (!verificar(
        !clientes.canjearBeneficio(
            "Hospital General",
            "Revision tecnica", beneficios),
        "Canje rechazado por saldo insuficiente")) {
        return 1;
    }

    if (!verificar(
        !clientes.canjearBeneficio(
            "Cliente Falso",
            "Descuento de energia", beneficios),
        "Cliente inexistente rechazado")) {
        return 1;
    }

    if (!verificar(
        !clientes.canjearBeneficio(
            "Hospital General",
            "Beneficio Inexistente", beneficios),
        "Beneficio inexistente rechazado")) {
        return 1;
    }

    cliente = clientes.buscarCliente("Hospital General");

    if (!verificar(
        cliente->tokens == 150 &&
        cliente->beneficiosCanjeados.size() == 2,
        "Canjes rechazados no modifican al cliente")) {
        return 1;
    }

    if (!verificar(
        clientes.canjearBeneficio(
            "Hospital General",
            "Descuento de energia", beneficios),
        "Canje repetido permitido")) {
        return 1;
    }

    cliente = clientes.buscarCliente("Hospital General");

    if (!verificar(
        cliente->tokens == 50 &&
        cliente->beneficiosCanjeados.size() == 3,
        "Historial conserva los tres canjes")) {
        return 1;
    }

    if (!verificar(
        Persistencia::guardarClientes(
            "data/canje_prueba.json", clientes),
        "Historial guardado en JSON")) {
        return 1;
    }

    Clientes recuperados;

    if (!verificar(
        Persistencia::cargarClientes(
            "data/canje_prueba.json", recuperados),
        "Historial recuperado desde JSON")) {
        return 1;
    }

    const Cliente* restaurado =
        recuperados.buscarCliente("Hospital General");

    if (!verificar(
        restaurado != nullptr &&
        restaurado->tokens == 50 &&
        restaurado->beneficiosCanjeados.size() == 3 &&
        restaurado->beneficiosCanjeados[0] == "Descuento de energia" &&
        restaurado->beneficiosCanjeados[1] == "Revision tecnica" &&
        restaurado->beneficiosCanjeados[2] == "Descuento de energia",
        "Saldo e historial conservados correctamente")) {
        return 1;
    }

    cout << "\nPRUEBA DE CANJE EXITOSA\n";
    return 0;
}
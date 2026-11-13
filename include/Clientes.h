#ifndef CLIENTES_H
#define CLIENTES_H

#include <string>
#include <vector>

class Beneficios;
struct ResultadoEcoRuta;

struct Cliente {
    std::string nombre;
    double consumoAcumulado;
    long long tokens;

    std::vector<std::string> subestacionesConectadas;
    std::vector<std::string> beneficiosCanjeados;

    std::vector<std::string> rutasUtilizadas;

    Cliente* siguiente;

    Cliente(std::string nombre);
};

class Clientes {
private:
    Cliente* primero;
    int cantidad;

    Cliente* buscarInterno(const std::string& nombre);

public:
    Clientes();
    ~Clientes();

    Clientes(const Clientes&) = delete;
    Clientes& operator=(const Clientes&) = delete;

    bool agregarCliente(const std::string& nombre);
    const Cliente* buscarCliente(const std::string& nombre) const;
    bool eliminarCliente(const std::string& nombre);

    bool asignarTokens(const std::string& nombre, long long cantidad);
    bool registrarConsumo(const std::string& nombre, double consumo);

    bool restaurarCliente(
        const std::string& nombre,
        double consumo,
        long long tokens,
        const std::vector<std::string>& subestaciones,
        const std::vector<std::string>& beneficios,
        const std::vector<std::string>& rutas = {}
    );

    void intercambiar(Clientes& otro);
    bool canjearBeneficio(
        const std::string& nombreCliente,
        const std::string& nombreBeneficio,
        const Beneficios& catalogo
    );
    bool registrarRutaOptimizada(
        const std::string& nombreCliente,
        const ResultadoEcoRuta& resultado
    );
    int getCantidad() const;
    const Cliente* getPrimero() const;
    void mostrarClientes() const;
};

#endif
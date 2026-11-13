#ifndef CLIENTES_H
#define CLIENTES_H

#include <string>
#include <vector>

struct Cliente {
    std::string nombre;
    double consumoAcumulado;
    long long tokens;

    std::vector<std::string> subestacionesConectadas;
    std::vector<std::string> beneficiosCanjeados;

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
        const std::vector<std::string>& beneficios
    );

    void intercambiar(Clientes& otro);
    int getCantidad() const;
    const Cliente* getPrimero() const;
    void mostrarClientes() const;
};

#endif
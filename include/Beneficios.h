#ifndef BENEFICIOS_H
#define BENEFICIOS_H

#include <string>

struct Beneficio {
    std::string nombre;
    std::string descripcion;
    long long costoTokens;

    Beneficio* siguiente;

    Beneficio(
        const std::string& nombre,
        const std::string& descripcion,
        long long costoTokens
    );
};

class Beneficios {
private:
    Beneficio* primero;
    int cantidad;

    Beneficio* buscarInterno(const std::string& nombre);

    static void mostrarRecursivo(const Beneficio* actual);

public:
    Beneficios();
    ~Beneficios();

    Beneficios(const Beneficios&) = delete;
    Beneficios& operator=(const Beneficios&) = delete;

    bool agregarBeneficio(
        const std::string& nombre,
        const std::string& descripcion,
        long long costoTokens
    );

    const Beneficio* buscarBeneficio(
        const std::string& nombre
    ) const;

    bool modificarBeneficio(
        const std::string& nombreActual,
        const std::string& nuevoNombre,
        const std::string& nuevaDescripcion,
        long long nuevoCosto
    );

    bool eliminarBeneficio(const std::string& nombre);

    void mostrarBeneficios() const;

    int getCantidad() const;
    const Beneficio* getPrimero() const;

    void intercambiar(Beneficios& otro);
};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:34:31 by rmarrero          #+#    #+#             */
/*   Updated: 2025/02/19 19:07:39 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minirt.h"
// Calcula el producto escalar de dos vectores.
// Útil para calcular ángulos entre vectores, iluminación difusa, etc.
double  vec3_dot(t_vec3 v1, t_vec3 v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}
// Calcula el producto vectorial de dos vectores.
// El resultado es un vector perpendicular a ambos vectores originales.
// Útil para calcular normales de superficie o ejes de coordenadas.
t_vec3  vec3_cross(t_vec3 v1, t_vec3 v2)
{
    return (vec3_init(v1.y * v2.z - v1.z * v2.y,
                      v1.z * v2.x - v1.x * v2.z,
                      v1.x * v2.y - v1.y * v2.x));
}
// Calcula la longitud (magnitud) de un vector.
double  vec3_length(t_vec3 v)
{
    return (sqrt(vec3_dot(v, v))); // sqrt(x*x + y*y + z*z)
}

// Normaliza un vector (lo convierte en un vector unitario de longitud 1).
// Crucial para direcciones y normales.
t_vec3  vec3_normalize(t_vec3 v)
{
    double length;

    length = vec3_length(v);
    if (length == 0.0)
        // Manejar el caso de un vector nulo para evitar división por cero.
        // Podría devolver un vector nulo o generar un error.
        return (vec3_init(0, 0, 0));
    return (vec3_div(v, length));
}

// Calcula un punto a lo largo de un rayo dado un parámetro 't'.
// point = origin + t * direction
t_vec3  ray_at(t_ray r, double t)
{
    return (vec3_add(r.origin, vec3_mul(r.direction, t)));
}
// Calcula el vector reflejado de un rayo incidente sobre una superficie.
// v: vector incidente (dirección del rayo que llega a la superficie)
// n: normal de la superficie en el punto de impacto (normalizada)
// Formula: R = V - 2 * (V . N) * N
t_vec3  vec3_reflect(t_vec3 v, t_vec3 n)
{
    return (vec3_sub(v, vec3_mul(n, 2 * vec3_dot(v, n))));
}
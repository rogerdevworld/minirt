#include "../../include/minirt.h"

void    animate_lights(void *param)
{
    t_data  *data = (t_data *)param;
    t_light **lights;
    int     i;

    // Salir si no hay luces o la animación no está activa
    if (!data->scene.lights || data->scene.num_lights == 0)
        return;

    lights = (t_light **)data->scene.lights;

    // Aumenta el tiempo de animación para el movimiento continuo
    data->light_move_time += 0.05; // Ajusta este valor para controlar la velocidad

    i = 0;
    while (i < data->scene.num_lights)
    {
        if (lights[i]->movement_type != MOVEMENT_NONE)
        {
            if (lights[i]->movement_type == MOVEMENT_CIRCLE)
            {
                // Movimiento circular en el plano XZ
                double radius = 10.0;
                double speed = 1.0;
                
                lights[i]->position.x = radius * cos(data->light_move_time * speed);
                lights[i]->position.z = radius * sin(data->light_move_time * speed);
            }
            else if (lights[i]->movement_type == MOVEMENT_TRIANGLE)
            {
                // Movimiento triangular
                double  side_length = 15.0;
                t_vec3  p1 = {0, 0, 0};
                t_vec3  p2 = {side_length, 0, 0};
                t_vec3  p3 = {side_length / 2, 0, side_length * sin(M_PI / 3)};
                
                double normalized_time = fmod(data->light_move_time, 3.0);
                
                if (normalized_time < 1.0)
                {
                    // Primer segmento: P1 a P2
                    lights[i]->position = vec3_add(p1, vec3_mul(vec3_sub(p2, p1), normalized_time));
                }
                else if (normalized_time < 2.0)
                {
                    // Segundo segmento: P2 a P3
                    lights[i]->position = vec3_add(p2, vec3_mul(vec3_sub(p3, p2), normalized_time - 1.0));
                }
                else
                {
                    // Tercer segmento: P3 a P1
                    lights[i]->position = vec3_add(p3, vec3_mul(vec3_sub(p1, p3), normalized_time - 2.0));
                }
            }
            else if (lights[i]->movement_type == MOVEMENT_LINE)
            {
                // Movimiento lineal de ida y vuelta en el eje X
                double start_pos = -10.0;
                double end_pos = 10.0;
                double travel_range = end_pos - start_pos;
                double speed = 0.5;
                
                lights[i]->position.x = start_pos + travel_range * 0.5 * (1 + sin(data->light_move_time * speed));
            }
        }
        i++;
    }

    render_threaded(data);
}
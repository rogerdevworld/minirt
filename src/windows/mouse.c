#include "../../include/minirt.h"

void    ft_zoom(t_data *data, double zoom_factor)
{
    data->scene.camera.fov -= zoom_factor;
    if (data->scene.camera.fov < 10)
        data->scene.camera.fov = 10;
    if (data->scene.camera.fov > 170)
        data->scene.camera.fov = 170;
}

void init_mouse_state(t_data *data)
{
    data->mouse_state.is_pressed = false;
    data->mouse_state.last_x = 0.0;
    data->mouse_state.last_y = 0.0;
}

#include "../../include/minirt.h"

void    key_hook(mlx_key_data_t keydata, void *param)
{
    t_data  *data = (t_data *)param;
    t_vec3  move_dir;

    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
        mlx_close_window(data->mlx.mlx_ptr);
	if (keydata.key == MLX_KEY_R && keydata.action == MLX_PRESS)
    {
		ft_printf("R\n");
        data->auto_rotate_active = !data->auto_rotate_active;
    }
    move_dir = vec3_init(0, 0, 0);
    if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
    {
        if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_UP)
            move_dir = vec3_add(move_dir, data->scene.camera.forward);
        else if (keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_DOWN)
            move_dir = vec3_sub(move_dir, data->scene.camera.forward);
        if (keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_LEFT)
            move_dir = vec3_sub(move_dir, data->scene.camera.right);
        else if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_RIGHT)
            move_dir = vec3_add(move_dir, data->scene.camera.right);
        if (keydata.key == MLX_KEY_SPACE)
            move_dir.y += 1.0;
        else if (keydata.key == MLX_KEY_C)
            move_dir.y -= 1.0;
        if (vec3_length(move_dir) > 0)
        {
            data->scene.camera.position = vec3_add(data->scene.camera.position, vec3_mul(vec3_normalize(move_dir), MOVE_SPEED));
            render_threaded(data);
        }
    }
}

void mouse_move_hook(double x, double y, void *param)
{
    t_data *data = (t_data *)param;
    double dx, dy;

    if (data->mouse_state.is_pressed)
    {
        dx = x - data->mouse_state.last_x;
        dy = y - data->mouse_state.last_y;
        double move_distance = sqrt(dx * dx + dy * dy);
        if (move_distance > MOUSE_MOVE_THRESHOLD)
        {
            t_vec3 axis_x = {0, 1, 0}; 
            t_vec3 axis_y = {1, 0, 0}; 

            data->scene.camera.orientation = rotate_vector(data->scene.camera.orientation, axis_x, dx * 0.01);
            data->scene.camera.orientation = rotate_vector(data->scene.camera.orientation, axis_y, dy * 0.01);
            data->scene.camera.forward = vec3_normalize(data->scene.camera.orientation);
            data->scene.camera.right = vec3_normalize(vec3_cross(data->scene.camera.forward, (t_vec3){0, 1, 0}));
            data->scene.camera.up = vec3_normalize(vec3_cross(data->scene.camera.right, data->scene.camera.forward));
            render_threaded(data);
        }
    }
    data->mouse_state.last_x = x;
    data->mouse_state.last_y = y;
}

void mouse_press_hook(mouse_key_t button, action_t action, modifier_key_t modifier, void* param)
{
    t_data *data = (t_data *)param;
    (void)modifier;
    if (button == MLX_MOUSE_BUTTON_LEFT)
    {
        if (action == MLX_PRESS)
            data->mouse_state.is_pressed = true;
        else
            data->mouse_state.is_pressed = false;
    }
}

void scroll_hook(double xdelta, double ydelta, void* param)
{
    t_data* data = (t_data*)param;
    int32_t mouse_x, mouse_y;
    double zoom_factor = 0.1;

    mlx_get_mouse_pos(data->mlx.mlx_ptr, &mouse_x, &mouse_y);
    t_ray temp_ray = generate_antialiased_ray((int)mouse_x, (int)mouse_y, 0, 0, &data->scene);
    t_hit_record rec = find_closest_hit(&temp_ray, &data->scene);
    t_vec3 target_point;
    if (rec.object != NULL)
        target_point = ray_at(temp_ray, rec.t);
    else
        target_point = ray_at(temp_ray, 10.0);
    if (ydelta > 0)
        data->scene.camera.position = vec3_add(data->scene.camera.position, vec3_mul(vec3_sub(target_point, data->scene.camera.position), zoom_factor));
    else if (ydelta < 0)
        data->scene.camera.position = vec3_sub(data->scene.camera.position, vec3_mul(vec3_sub(target_point, data->scene.camera.position), zoom_factor));
    render_threaded(data);
}

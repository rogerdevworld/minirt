/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* window.c                                           :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/02/19 18:34:31 by rmarrero          #+#    #+#             */
/* Updated: 2025/02/19 19:07:39 by rmarrero         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "../../include/minirt.h"

t_vec3 rotate_vector(t_vec3 v, t_vec3 axis, double angle)
{
    t_vec3 result;
    double c = cos(angle);
    double s = sin(angle);
    double t = 1.0 - c;
    
    axis = vec3_normalize(axis);

    double x = axis.x;
    double y = axis.y;
    double z = axis.z;

    result.x = (t * x * x + c) * v.x + (t * x * y - s * z) * v.y + (t * x * z + s * y) * v.z;
    result.y = (t * x * y + s * z) * v.x + (t * y * y + c) * v.y + (t * y * z - s * x) * v.z;
    result.z = (t * x * z - s * y) * v.x + (t * y * z + s * x) * v.y + (t * z * z + c) * v.z;

    return result;
}

// This function adjusts the camera's Field of View (FOV) for zooming.
void	zoom(t_data *data, double zoom_factor)
{
	data->scene.camera.fov -= zoom_factor;
	// Clamp the FOV to a valid range to prevent visual issues.
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


void mouse_move_hook(double x, double y, void *param)
{
    t_data *data = (t_data *)param;
    double dx, dy;

    if (data->mouse_state.is_pressed)
    {
        dx = x - data->mouse_state.last_x;
        dy = y - data->mouse_state.last_y;

        t_vec3 axis_x = {0, 1, 0}; 
        t_vec3 axis_y = {1, 0, 0}; 

        data->scene.camera.orientation = rotate_vector(data->scene.camera.orientation, axis_x, dx * 0.01);
        data->scene.camera.orientation = rotate_vector(data->scene.camera.orientation, axis_y, dy * 0.01);
        
        data->scene.camera.forward = vec3_normalize(data->scene.camera.orientation);
        data->scene.camera.right = vec3_normalize(vec3_cross(data->scene.camera.forward, (t_vec3){0, 1, 0}));
        data->scene.camera.up = vec3_normalize(vec3_cross(data->scene.camera.right, data->scene.camera.forward));

        render_threaded(data);
    }
    data->mouse_state.last_x = x;
    data->mouse_state.last_y = y;
}

void mouse_press_hook(mouse_key_t button, action_t action, modifier_key_t modifier, void* param)
{
    t_data *data = (t_data *)param;
    (void)modifier; // Suppress unused parameter warning

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

int	mlx_setup(t_data *data)
{
	data->mlx.mlx_ptr = mlx_init(data->scene.width, data->scene.height, "miniRT", true);
	if (!data->mlx.mlx_ptr)
		return (1);
	data->mlx.img.img_ptr = mlx_new_image(data->mlx.mlx_ptr, data->scene.width, data->scene.height);
	if (!data->mlx.img.img_ptr)
	{
		mlx_terminate(data->mlx.mlx_ptr);
		return (1);
	}
	
	mlx_key_hook(data->mlx.mlx_ptr, key_hook, data);
	mlx_scroll_hook(data->mlx.mlx_ptr, scroll_hook, data);
    mlx_cursor_hook(data->mlx.mlx_ptr, mouse_move_hook, data);
    mlx_mouse_hook(data->mlx.mlx_ptr, mouse_press_hook, data);
	
	if (mlx_image_to_window(data->mlx.mlx_ptr, data->mlx.img.img_ptr, 0, 0) < 0)
	{
		mlx_terminate(data->mlx.mlx_ptr);
		return (1);
	}
	return (0);
}

int	close_window(t_data *data)
{
	cleanup_program(data);
	exit(0);
	return (0);
}

// Define a movement speed constant for the camera
#define MOVE_SPEED 0.5
#define ROTATION_SPEED 0.05

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_data	*data = (t_data *)param;
	t_vec3	move_dir;

	// Exit the program
    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
    {
        mlx_close_window(data->mlx.mlx_ptr);
    }
    
    // Movement controls (WASD and arrow keys)
	move_dir = vec3_init(0, 0, 0);
	if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
	{
		// Forward and backward movement
		if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_UP)
			move_dir = vec3_add(move_dir, data->scene.camera.forward);
		else if (keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_DOWN)
			move_dir = vec3_sub(move_dir, data->scene.camera.forward);

		// Strafe left and right
		if (keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_LEFT)
			move_dir = vec3_sub(move_dir, data->scene.camera.right);
		else if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_RIGHT)
			move_dir = vec3_add(move_dir, data->scene.camera.right);

		// Vertical movement
		if (keydata.key == MLX_KEY_SPACE)
			move_dir.y += 1.0;
		else if (keydata.key == MLX_KEY_C)
			move_dir.y -= 1.0;
		
		// Apply movement
		if (vec3_length(move_dir) > 0)
		{
			data->scene.camera.position = vec3_add(data->scene.camera.position, vec3_mul(vec3_normalize(move_dir), MOVE_SPEED));
			render_threaded(data);
		}
	}
}
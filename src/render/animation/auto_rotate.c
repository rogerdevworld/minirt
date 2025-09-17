#include "../../include/minirt.h"

// En tu archivo principal o donde tengas el bucle mlx_loop_hook
void	auto_rotate(void *param)
{
	t_data *data = (t_data *)param;

	if (!data->auto_rotate_active)
		return ;
	data->rotation_angle += 0.01;
	if (data->rotation_angle > 2 * M_PI)
		data->rotation_angle -= 2 * M_PI;
	double radius = 10.0;
	t_vec3 center = vec3_init(0, 0, 0);
	data->scene.camera.position.x = center.x + radius
		* cos(data->rotation_angle);
	data->scene.camera.position.z = center.z + radius
		* sin(data->rotation_angle);
	data->scene.camera.forward = vec3_normalize(vec3_sub(center,
				data->scene.camera.position));
	data->scene.camera.right = vec3_normalize(vec3_cross(data->scene.camera.forward,
				(t_vec3){0, 1, 0}));
	data->scene.camera.up = vec3_normalize(vec3_cross(data->scene.camera.right,
				data->scene.camera.forward));
	render_threaded(data);
}
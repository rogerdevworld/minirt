#include "../include/minirt.h"

// 4k
void *render_thread_func(void *arg)
{
    t_thread_data   *thread_data;
    t_data          *data;
    int             y, x;
    int             sub_x, sub_y;
    t_ray           ray;
    t_hit_record    rec;
    t_color         final_color;
    t_color         subpixel_color;
    double          inv_sub_sq = 1.0 / (double)(SUBPIXEL_SAMPLES * SUBPIXEL_SAMPLES);

    thread_data = (t_thread_data *)arg;
    data = thread_data->global_data;
    y = thread_data->start_row;
    while (y < thread_data->end_row)
    {
        x = 0;
        while (x < data->scene.width)
        {
            final_color = vec3_init(0.0, 0.0, 0.0);
            sub_y = 0;
            while (sub_y < SUBPIXEL_SAMPLES)
            {
                sub_x = 0;
                while (sub_x < SUBPIXEL_SAMPLES)
                {
                    ray = generate_antialiased_ray(x, y, sub_x, sub_y, &data->scene);
                    pthread_mutex_lock(&data->progress_mutex);
                    data->ray_count++; // Incrementar el contador
                    pthread_mutex_unlock(&data->progress_mutex);

                    rec = find_closest_hit(&ray, &data->scene);
                    
                    if (rec.object != NULL)
                        subpixel_color = calculate_light(&rec, &data->scene, &ray, 0);
                    else
                        subpixel_color = data->scene.background_color; 
                    
                    final_color = vec3_add(final_color, subpixel_color);
                    sub_x++;
                }
                sub_y++;
            }
            final_color = vec3_mul(final_color, inv_sub_sq);
            mlx_put_pixel(data->mlx.img.img_ptr, x, y, color_to_int(final_color));
            x++;
        }
        pthread_mutex_lock(&data->progress_mutex);
        data->rendered_rows++;
        pthread_mutex_unlock(&data->progress_mutex);
        
        y++;
    }
    return (NULL);
}

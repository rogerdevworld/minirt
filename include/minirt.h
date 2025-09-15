/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* minirt.h                                           :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/07/23 14:24:12 by rmarrero          #+#    #+#             */
/* Updated: 2025/07/23 14:24:32 by rmarrero         ###   ########.fr       */
/* */
/* ************************************************************************** */
#ifndef MINIRT_H
# define MINIRT_H

// --- 0. Includes de Librerías ---
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/X.h>
#include "../MLX42/include/MLX42/MLX42.h"
# include "../src/libft/libft.h"
# include <math.h>
# include <pthread.h>
# include <stdlib.h>
# include <X11/keysym.h>
#include <errno.h>

// --- 1. Constantes y Macros ---
# define EPSILON 1e-6
#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif
# define MAX_RECURSION_DEPTH 5
#define EISDIR 21
#define SUBPIXEL_SAMPLES 1
# define KEY_W XK_w
# define KEY_A XK_a
# define KEY_S XK_s
# define KEY_D XK_d
# define KEY_ESC XK_Escape

// --- 2. Macros de Errores ---
# define ERR_TOO_MANY_ARGS "Error\nToo many arguments.\n"
# define ERR_INVALID_FILE "Error\nInvalid file or path.\n"
# define ERR_INVALID_EXT "Error\nInvalid file extension. Use .rt\n"
# define ERR_INVALID_LINE "Error\nInvalid line in .rt file.\n"
# define ERR_MISSING_ELEMENT "Error\nMissing mandatory element (A, C).\n"
# define ERR_DUPLICATE_ELEMENT "Error\nDuplicate mandatory element (A, C).\n"
# define ERR_INVALID_PARAMS "Error\nInvalid parameters for element.\n"
# define ERR_MEMORY "Error\nMemory allocation failed.\n"

#define WIDTH_8K 7680
#define HEIGHT_8K 4320
#define WIDTH_4K 3840
#define HEIGHT_4K 2160

// --- 3. Estructuras de Datos Primarias ---

// Vector y color
typedef struct s_vec2
{
    double x;
    double y;
} t_vec2;

typedef struct s_vec3
{
    double x;
    double y;
    double z;
} t_vec3;

typedef struct s_mat3
{
    t_vec3 c1;
    t_vec3 c2;
    t_vec3 c3;
} t_mat3;

typedef t_vec3 t_color;

// Rayo
typedef struct s_ray
{
    t_vec3 origin;
    t_vec3 direction;
} t_ray;

// Información de un impacto (hit)
typedef struct s_hit_record
{
    t_vec3 point;
    t_vec3 normal;
    double t;
    t_ray ray;
    struct s_object *object;
} t_hit_record;

// Parámetros de especularidad (modelo de Phong)
typedef struct s_specular
{
    float intensity;
    int shininess;
} t_specular;

typedef struct s_material
{
    t_specular specular;
    double mirror_ratio;
    bool has_texture;
    mlx_texture_t *color_img;
    bool has_normal_map;
    mlx_texture_t *texture_img;
    int has_checkerboard;
    t_vec3 check_color1;
    t_vec3 check_color2;
    double check_scale;
} t_material;

typedef struct s_texture
{
    int is_checkerboard;
    int is_bump_map;
    char *file_path;
} t_texture;

// --- 4. Estructuras de Elementos de la Escena ---

// Cámara
typedef struct s_camera
{
    t_vec3 position;
    t_vec3 orientation;
    double fov;
    t_vec3 forward;
    t_vec3 right;
    t_vec3 up;
} t_camera;

// Luces
typedef struct s_ambient_light
{
    double ratio;
    t_vec3 color;
} t_ambient_light;

typedef enum e_movement_type
{
    MOVEMENT_NONE,
    MOVEMENT_CIRCLE,
    MOVEMENT_TRIANGLE,
    MOVEMENT_LINE
} t_movement_type;

typedef struct s_light
{
    t_vec3  position;
    double  brightness;
    t_color color;       // Corregido a t_color
    bool    is_moving;   // Nuevo campo para luces en movimiento
	t_movement_type movement_type;
} t_light;
// Tipos de objetos geométricos (incluye bonificaciones)
typedef enum e_object_type
{
    SPHERE,
    PLANE,
    CYLINDER,
    CONE,
    HYPERBOLOID,
    PARABOLOID,
} t_object_type;

// Esfera
typedef struct s_sphere
{
    t_vec3 center;
    double radius;
} t_sphere;

// Plano
typedef struct s_plane
{
    t_vec3 position;
    t_vec3 normal;
} t_plane;

// Cilindro
typedef struct s_cylinder
{
    t_vec3 position;
    t_vec3 axis;
    double radius;
    double height;
} t_cylinder;

// Cono (bonificación)
typedef struct s_cone
{
    t_vec3 position;
    t_vec3 axis;
    double radius;
    double height;
} t_cone;

// Hiperboloide (bonificación)
typedef struct s_hyperboloid
{
    t_vec3 position;
    t_vec3 axis;
    double radius_a;
    double radius_b;
    double height;
} t_hyperboloid;

typedef struct s_parab
{
    t_vec3 position;
    t_vec3 axis;
    double focal_lenght;
    double height;
} t_parab;

// Objeto genérico - Ahora más limpio con la nueva estructura de material
typedef struct s_object
{
    t_object_type type;
    t_vec3 color;
    void *data;
    t_material *material;
} t_object;

// --- 5. Estructuras de Control del Programa ---

// Escena
typedef struct s_scene
{
    t_ambient_light ambient;
    t_camera camera;
    t_light **lights;
    t_object **objects;
    int width;
    int height;
    int has_camera;
    int has_ambient;
    t_color background_color;
	int     num_lights;
} t_scene;

// Minilibx y la imagen
typedef struct s_img
{
    char *addr;
    void *img_ptr;
    int bpp;
    int line_len;
    int endian;
    int width;
    int height;
} t_img;

typedef struct s_mlx
{
    void *mlx_ptr;
    void *win_ptr;
    t_img img;
} t_mlx;

typedef struct s_mouse_state
{
    bool is_pressed;
    double last_x;
    double last_y;
} t_mouse_state;

// Datos globales para la multihilo
typedef struct s_data
{
    t_mlx mlx;
    t_scene scene;
    int num_threads;
    int rendered_rows;
    int show_progress;
    pthread_mutex_t progress_mutex;
    unsigned long ray_count;
	t_mouse_state   mouse_state;
	bool    auto_rotate_active;
    double  rotation_angle;
    t_vec3  rotation_center;
	double  light_move_time;
} t_data;

// Datos específicos del hilo
typedef struct s_thread_data
{
    int id;
    int start_row;
    int end_row;
    t_data *global_data;
} t_thread_data;

// --- 6. Funciones de Renderizado y MLX ---
int mlx_setup(t_data *data);
int close_window(t_data *data);
void key_hook(mlx_key_data_t keydata, void *param);
int mouse_press(int button, int x, int y, t_data *data);
int mouse_release(int button, int x, int y, t_data *data);
int mouse_move(int x, int y, t_data *data);
void render_threaded(t_data *data);
void *thread_render_rows(void *arg);
void put_pixel_to_img(t_img *img, int x, int y, int color);

// --- 7. Funciones de Utilidad del Ray Tracer ---
t_ray generate_ray(int x, int y, t_scene *scene);
t_hit_record find_closest_hit(t_ray *ray, t_scene *scene);
int color_to_int(t_color color);
t_vec3 apply_bump_map(t_hit_record *rec, t_object *obj);
t_color calculate_light(t_hit_record *rec, t_scene *scene, t_ray *ray, int depth);
t_color get_texture_color(t_hit_record *rec);
t_ray generate_antialiased_ray(int x, int y, int sub_x, int sub_y, t_scene *scene);
t_vec3 get_normal_from_map(t_hit_record *rec, t_vec3 geom_normal);
t_vec2 get_uv_sphere(t_hit_record *rec);
t_vec2 get_uv_plane(t_hit_record *rec);
t_vec2 get_uv_cylinder(t_hit_record *rec);
t_vec2 get_uv_cone(t_hit_record *rec);
t_vec2 get_uv_hyperboloid(t_hit_record *rec);
t_vec2 get_uv_paraboloid(t_hit_record *rec);

// --- 8. Funciones de Intersección (incluye bonificaciones) ---
int intersect_sphere(t_ray *ray, t_sphere *sp, t_hit_record *rec);
int intersect_plane(t_ray *ray, t_plane *pl, t_hit_record *rec);
int intersect_cylinder(t_ray *ray, t_cylinder *cy, t_hit_record *rec);
int intersect_cone(t_ray *ray, t_cone *co, t_hit_record *rec);
int intersect_hyperboloid(t_ray *ray, t_hyperboloid *hb, t_hit_record *rec);
int intersect_paraboloid(t_ray *ray, t_parab *pb, t_hit_record *rec);

// --- 9. Funciones del Parser (incluye bonificaciones) ---
void apply_object_modifiers(t_object *obj, char **tokens, int start_idx);
int open_filename(const char *filename);
int valid_extension_rt(const char *file_name);
int validate_texture_extension(const char *file_name);
void validate_file(int fd, const char *file_name);
double parse_fov(char *str);
double parse_mirror(char *token);
double parse_angle(char *str);
double parse_positive_double(char *str);
double validate_color_component(char *str);
void parse_specular(char *token, t_specular *spec);
void parse_checkboard(char *token, t_material *material);
void parse_rt_file(t_scene *scene, const char *file_path);
void parse_ambient(t_scene *scene, char **tokens);
void parse_camera(t_scene *scene, char **tokens);
void parse_light(t_scene *scene, char **tokens);
void parse_sphere(t_scene *scene, char **tokens);
void parse_plane(t_scene *scene, char **tokens);
void parse_cylinder(t_scene *scene, char **tokens);
void parse_cone(t_scene *scene, char **tokens);
void parse_hyperboloid(t_scene *scene, char **tokens);
void parse_paraboloid(t_scene *scene, char **tokens);
t_vec3 parse_vec3(char *str);
t_vec3 parse_vec3_color(char *str);
t_vec3 parse_vec3_normalized(char *str);
t_object *create_object(t_object_type type, void *data, t_vec3 color);

// --- 10. Funciones de Inicialización ---
t_vec3 vec3_init(double x, double y, double z);
t_ray ray_init(t_vec3 origin, t_vec3 direction);
t_camera camera_init(t_vec3 position, t_vec3 orientation, double fov);
t_ambient_light ambient_light_init(double ratio, t_vec3 color);
t_light light_init(t_vec3 position, double brightness, t_vec3 color);
t_sphere sphere_init(t_vec3 center, double radius);
t_plane plane_init(t_vec3 position, t_vec3 normal);
t_cylinder cylinder_init(t_vec3 position, t_vec3 axis, double radius, double height);
t_cone cone_init(t_vec3 position, t_vec3 axis, double radius, double height);
t_hyperboloid hyperboloid_init(t_vec3 position, t_vec3 axis);

// --- 11. Funciones a Mover a Libft / Utilidades Generales ---

t_vec2 vec2_init(double x, double y);
t_vec3 vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3 vec3_sub(t_vec3 v1, t_vec3 v2);
t_vec3 vec3_mul(t_vec3 v, double scalar);
t_vec3 vec3_div(t_vec3 v, double scalar);
t_vec3 vec3_mult_vec(t_vec3 v1, t_vec3 v2);
double vec3_dot(t_vec3 v1, t_vec3 v2);
t_vec3 vec3_cross(t_vec3 v1, t_vec3 v2);
double vec3_length(t_vec3 v);
t_vec3 vec3_normalize(t_vec3 v);
t_vec3 ray_at(t_ray r, double t);
t_vec3 vec3_reflect(t_vec3 v, t_vec3 n);
t_vec3 rotate_vector(t_vec3 v, t_vec3 axis, double angle);

void validate_args(int argc, char **argv);
int is_valid_float(char *str);
int is_valid_int(char *str);
int is_valid_color_component(char *str);
int is_normalized_vector(t_vec3 vec);
void ft_error_exit(const char *msg);
void add_object_to_scene(t_scene *scene, t_object *obj);
void add_light_to_scene(t_scene *scene, t_light *light);
int ft_strarr_len(char **arr);
double ft_atod(const char *str);
void ft_free_str_array(char **arr);
void free_tokens(char **tokens);
void free_scene_data(t_scene *scene);
void cleanup_program(t_data *data);
int is_in_shadow(t_ray *shadow_ray, t_scene *scene, t_light *light);
void init_scene(t_scene *scene);
int get_num_processors(void);
void mouse_move_hook(double x, double y, void *param);
// In src/windows/../../include/minirt.h
void mouse_press_hook(mouse_key_t button, action_t action, modifier_key_t modifier, void* param);
#endif